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

	extern ComponentManager componentManager;

	void setupEnvironment(bool isFullscreen, GLuint windowWidth, GLuint windowHeight);
	
	Shader& getShader();

	void addSystem(std::unique_ptr<System> system);

	void addComponent(ComponentEnum type, std::unique_ptr<ComponentArray> component);

	bool finalSetup();

	void run(bool vsyncOn);

	template <
		typename ComponentType
	>
	ComponentType& getComponent(unsigned int UID) {
		return (dynamic_cast<GenericComponentArray<ComponentType>*>(componentManager.getComponentArray(ComponentType::type)))->getComponent(UID);
	}

}