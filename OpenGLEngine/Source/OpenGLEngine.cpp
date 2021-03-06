#include "OpenGLEngine.h"

#include "Shader.h"
#include "SystemManager.h"
#include "Util/Averager.h"
#include "GUI/GUI.h"
#include "GUI/TextLine.h"

// GLFW
#include <GLFW/glfw3.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "local_glm.h"

#include <iostream>
#include <sstream>
#include <string>
#include <queue>

namespace OpenGLEngine {

	InputState inputState;

	SystemManager systemManager;
	ComponentManager componentManager;
	

	double lastFrameTime = glfwGetTime();
	Averager<double, 60> framerateAverager;
	GLFWwindow* mainWindow = nullptr;
	float aspectRatio = 0;
	Shader vertexNormalColourShader;

	GLuint WINDOW_WIDTH;
	GLuint WINDOW_HEIGHT;

	const InputState& getInputState() {
		return inputState;
	}
	
	float getAspectRatio() {
		return aspectRatio;
	}

	void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/) {
		// When user presses the escape key, we set WindowShouldClose property
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (action == GLFW_PRESS)
			inputState.keys[key] = true;
		else if (action == GLFW_RELEASE)
			inputState.keys[key] = false;
	}

	void mouse_callback(GLFWwindow* /*window*/, double xpos, double ypos) {

		float xposf = static_cast<float>(xpos);
		float yposf = static_cast<float>(ypos);

		static bool firstMouse = true;
		if (firstMouse) {
			inputState.xPos = xposf;
			inputState.yPos = yposf;
			firstMouse = false;
		}

		inputState.xOffset = xposf - inputState.xPos;
		inputState.yOffset = inputState.yPos - yposf;
		inputState.xPos = xposf;
		inputState.yPos = yposf;
	}

	void mouse_button_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (action == GLFW_PRESS) {
				inputState.leftMousePressed = true;
				inputState.mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = true;
			} else if (action == GLFW_RELEASE) {
				inputState.mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = false;
			}
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (action == GLFW_PRESS) {
				inputState.rightMousePressed = true;
				inputState.mouseButtons[GLFW_MOUSE_BUTTON_RIGHT] = true;
			} else if (action == GLFW_RELEASE) {
				inputState.mouseButtons[GLFW_MOUSE_BUTTON_RIGHT] = false;
			}
		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			if (action == GLFW_PRESS) {
				inputState.middleMousePressed = true;
				inputState.mouseButtons[GLFW_MOUSE_BUTTON_MIDDLE] = true;
			} else if (action == GLFW_RELEASE) {
				inputState.mouseButtons[GLFW_MOUSE_BUTTON_MIDDLE] = false;
			}
		}
		// Note: the mouse pressed booleans get cleared at the end of the frame ever loop

	}

	void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double /*yoffset*/) {
		// TODO add this to the input state
	}


	void setupEnvironment(bool isFullscreen, GLuint windowWidth, GLuint windowHeight) {

		WINDOW_WIDTH = windowWidth;
		WINDOW_HEIGHT = windowHeight;

		// *** SETUP GLFW *** //
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		if (isFullscreen) {
			// Get primary monitor settings
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			// Create the window and check for errors
			mainWindow = glfwCreateWindow(mode->width, mode->height, "Astroid Explorer", nullptr, nullptr);
			glfwSetWindowMonitor(mainWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			aspectRatio = float(mode->width) / float(mode->height);
		} else {
			mainWindow = glfwCreateWindow(windowWidth, windowHeight, "Astroid Explorer", nullptr, nullptr);
			aspectRatio = float(windowWidth) / float(windowHeight);
		}
		if (mainWindow == nullptr) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			throw "Failed to create GLFW window";
		}
		glfwMakeContextCurrent(mainWindow);

		// Set the required callback functions
		glfwSetKeyCallback(mainWindow, key_callback);
		glfwSetCursorPosCallback(mainWindow, mouse_callback);
		glfwSetScrollCallback(mainWindow, scroll_callback);
		glfwSetMouseButtonCallback(mainWindow, mouse_button_callback);


		// Options
		glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// *** GLEW SETUP *** //
		{
			glewExperimental = GL_TRUE;
			GLenum err = glewInit();
			if (GLEW_OK != err) {
				std::cout << "Failed to initialize GLEW" << std::endl;
				std::cout << "Error: " << glewGetErrorString(err) << std::endl;
				throw "Failed to initialize GLEW";
			}
		}


		// *** VIEWPORT *** //
		{
			int width, height;
			glfwGetFramebufferSize(mainWindow, &width, &height);
			glViewport(0, 0, width, height);
		}


		// *** OpenGL Options *** //
		glEnable(GL_DEPTH_TEST);

		// *** Setup Shaders *** //
		vertexNormalColourShader = Shader("../OpenGLEngine/Shaders/vertexNormalColour.vert", "../OpenGLEngine/Shaders/vertexNormalColour.frag");
		vertexNormalColourShader.Use();
		glUniform3f(glGetUniformLocation(vertexNormalColourShader.Program, "lightDirection"), -1.0f, -3.0f, -1.0f);
	}

	Shader& getShader() {
		return vertexNormalColourShader;
	}

	void addSystem(std::unique_ptr<System> system) {
		systemManager.addSystem(std::move(system));
	}

	void addComponent(ComponentEnum type, std::unique_ptr<ComponentArray> component) {
		componentManager.addComponent(type, std::move(component));
	}

	void run(bool vsyncOn) {

		GUI::init(WINDOW_WIDTH, WINDOW_HEIGHT);
		GUI::TextLine myLine(glm::vec2(30.0f), "Hello World!");
		GUI::GUIdrawlist.add(&myLine);

		if (!vsyncOn) {
			glfwSwapInterval(0); // framelimiter
		}
		while (!glfwWindowShouldClose(mainWindow)) {
			glfwPollEvents(); // Handle all the glfw events with callbacks

			// Calculate the time for this frame
			{
				double currentTime = glfwGetTime();
				inputState.deltaT = static_cast<float>(currentTime - lastFrameTime);
				lastFrameTime = currentTime;
			}

			// Clear last frame
			glm::vec3 backgroundColor = glm::vec3(50.0); // just slightly lighter than black
			backgroundColor /= glm::vec3(255.0);
			glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			systemManager.run();

			inputState.clearInputs();

			// Draw the GUI
			framerateAverager.push(inputState.deltaT);
			std::stringstream ss;
			ss << "Frame time: " << framerateAverager.sample() * 1000;
			myLine.setText(ss.str());
			GUI::draw();

			glfwSwapBuffers(mainWindow);
		}

		glfwTerminate();
	}
}