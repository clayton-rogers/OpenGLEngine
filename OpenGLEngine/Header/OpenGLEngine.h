#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <memory>

#include "System.h"
#include "ComponentManager.h"
#include "ComponentArray.h"
#include "Shader.h"

namespace OpenGLEngine {

	void setupEnvironment(bool isFullscreen, GLuint windowWidth, GLuint windowHeight);
	
	Shader& getShader();

	void addSystem(std::unique_ptr<System> system);

	void addComponent(ComponentEnum type, std::unique_ptr<ComponentArray> component);

	ComponentManager* getComponentManager();

	bool finalSetup();

	void run();

}