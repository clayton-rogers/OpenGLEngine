#pragma once

#include "System.h"
#include "ComponentManager.h"
#include "ComponentArray.h"
#include "Shader.h"

#include <memory>

namespace OpenGLEngine {

	struct InputState {
		bool keys[1024] = { false };      // Use GLFW_KEY enums to dereference
		float xOffset = 0.0;
		float yOffset = 0.0;             // Distance in pixels the mouse has moved
		float xPos = 0.0;
		float yPos = 0.0;                // Position in pixels of the mouse
		float deltaT = 0.0;             // Time in seconds since the last loop
		bool mouseButtons[3] = { false }; // Use GLFW_MOUSE_BUTTON enums to dereference
		bool leftMousePressed = false;    // Whether the left mouse button was pressed this frame
		bool rightMousePressed = false;   // Whether the right mouse button was pressed this frame
		bool middleMousePressed = false;  // Whether the middle mouse button was pressed this frame

		// Gets called ever loop so that they are only true for one frame
		void clearInputs() {
			leftMousePressed = false;
			rightMousePressed = false;
			middleMousePressed = false;

			xOffset = 0.0;
			yOffset = 0.0;
		}
	};

	extern ComponentManager componentManager;
	const InputState& getInputState();
	Shader& getShader();
	float getAspectRatio();

	void setupEnvironment(bool isFullscreen, GLuint windowWidth, GLuint windowHeight);
	void addSystem(std::unique_ptr<System> system);
	void addComponent(ComponentEnum type, std::unique_ptr<ComponentArray> component);
	void run(bool vsyncOn);

	template <typename ComponentType>
	ComponentType& getComponent(unsigned int UID) {
		return (dynamic_cast<GenericComponentArray<ComponentType>*>(componentManager.getComponentArray(ComponentType::type)))->getComponent(UID);
	}

	template<typename ComponentType>
	bool hasComponent(unsigned int UID) {
		return componentManager.getBitsetForEntity(UID).test(ComponentType::type);
	}

}