// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Standard libraries
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

// Local includes
#include "Shader.h"
#include "PlanetManager.h"
#include "LazerManager.h"
#include "Camera.h"
#include "glCheckError.h"
#include "GenericActionList.h"
#include "Font.h"
#include "Text.h"

bool keys[1024];
bool firstMouse = true;
double lastX, lastY;
double deltaT;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 20.0f));
LazerManager laserManager;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When user presses the escape key, we set WindowShouldClose property
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)(xpos - lastX);
	GLfloat yoffset = (GLfloat)(lastY - ypos);
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glm::vec3 startPosition = camera.Position;
		startPosition -= camera.Up * 0.5f;
		laserManager.add(startPosition, camera.Front);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((GLfloat)yoffset);
}

void do_movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaT);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaT);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaT);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaT);
}


const bool FULLSCREEN = false;
const GLuint WINDOW_WIDTH = 1000;
const GLuint WINDOW_HEIGHT = 800;


int main() {

	// *** SETUP GLFW *** //
	GLFWwindow* window = nullptr;
	float aspectRatio = 0;
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		if (FULLSCREEN) {
			// Get primary monitor settings
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			// Create the window and check for errors
			window = glfwCreateWindow(mode->width, mode->height, "Astroid Explorer", nullptr, nullptr);
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			aspectRatio = float(mode->width) / float(mode->height);
		} else {
			window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Astroid Explorer", nullptr, nullptr);
			aspectRatio = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
		}
		if (window == nullptr) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

		// Set the required callback functions
		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		// Options
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// TODO: add input handlers
	}



	// *** GLEW SETUP *** //
	{
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err) {
			std::cout << "Failed to initialize GLEW" << std::endl;
			std::cout << "Error: " << glewGetErrorString(err) << std::endl;
			return -1;
		}
	}


	// *** VIEWPORT *** //
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}



	// *** OpenGL Options *** //
	glEnable(GL_DEPTH_TEST);



	// *** Main Game Loop *** //
	Shader vertexNormalColourShader = Shader("./Shaders/vertexNormalColour.vert", "./Shaders/vertexNormalColour.frag");
	vertexNormalColourShader.Use();
	glUniform3f(glGetUniformLocation(vertexNormalColourShader.Program, "lightDirection"), -1.0f, -3.0f, -1.0f);

	double lastFrameTime = glfwGetTime();

	PlanetManager planetManager;

	GenericActionList<GLuint> drawList;
	planetManager.drawListIndex = drawList.add(&planetManager);
	laserManager.drawListIndex = drawList.add(&laserManager);

	GenericActionList<double> physicsList;
	planetManager.physicsListIndex = physicsList.add(&planetManager);
	laserManager.physicsListIndex = physicsList.add(&laserManager);


	Shader fontShader = Shader("./Shaders/text.vert", "./Shaders/text.frag");
	Font vcr("./Resources/Font/VCR-OSD-mono2.png", "./Resources/Font/VCR-OSD-mono.txt");
	GLuint textTexID;
	vcr.uploadTextureToGPU(textTexID);
	Text testString(textTexID, &vcr, "The quick brown fox jumps over the lazy dog", glm::vec2(100.0f, 100.0f));

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		do_movement();

		// Calculate the time for this frame
		{
			double currentTime = glfwGetTime();
			deltaT = currentTime - lastFrameTime;
			lastFrameTime = currentTime;
		}

		// Perform physics for this step
		physicsList.doActions(deltaT);


		// Clear last frame
		glm::vec3 backgroundColor = glm::vec3(50.0); // just slightly lighter than black
		backgroundColor /= glm::vec3(255.0);
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw the frame
		{
			vertexNormalColourShader.Use();
			glm::mat4 view = camera.GetViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, 0.1f, 100.0f);

			glUniformMatrix4fv(glGetUniformLocation(vertexNormalColourShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(vertexNormalColourShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniform3f(glGetUniformLocation(vertexNormalColourShader.Program, "eyePosition"), camera.Position.x, camera.Position.y, camera.Position.z);

			drawList.doActions(vertexNormalColourShader.Program);
		}

		// Draw the GUI
		{
			fontShader.Use();
			glm::mat4 projection = glm::ortho(0.0f, float(WINDOW_WIDTH), 0.0f, float(WINDOW_HEIGHT), 0.1f, 100.0f);


			glUniformMatrix4fv(glGetUniformLocation(fontShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			std::stringstream ss;
			ss << "Frame time: " << deltaT * 1000.0f << " ms";
			testString.setText(ss.str());
			testString.doAction(fontShader.Program);
		}



		//glCheckError();
		glfwSwapBuffers(window);
	}

	drawList.remove(planetManager.drawListIndex);
	drawList.remove(laserManager.drawListIndex);

	physicsList.remove(planetManager.physicsListIndex);
	physicsList.remove(laserManager.physicsListIndex);

	glfwTerminate();
	return 0;
}