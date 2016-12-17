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

// Local includes
#include "Drawable.h"
#include "Shader.h"


const bool FULLSCREEN = false;
const GLuint WINDOW_WIDTH = 1600;
const GLuint WINDOW_HEIGHT = 1200;


int main() {

	// *** SETUP GLFW *** //
	GLFWwindow* window = nullptr;
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
		} else {
			window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Astroid Explorer", nullptr, nullptr);
		}
		if (window == nullptr) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

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
	double lastFrameTime = glfwGetTime();
	std::vector<Drawable*> drawList;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Calculate the time for this frame
		double deltaT;
		{
			double currentTime = glfwGetTime();
			deltaT = currentTime - lastFrameTime;
			lastFrameTime = currentTime;
		}

		// Perform physics for this step
		// TODO: do physics

		// Clear last frame
		glm::vec3 backgroundColor = glm::vec3(10.0); // just slightly lighter than black
		backgroundColor /= glm::vec3(255.0);
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw the frame
		vertexNormalColourShader.Use();
		for (Drawable* object : drawList) {
			object->Draw();
		}


		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}