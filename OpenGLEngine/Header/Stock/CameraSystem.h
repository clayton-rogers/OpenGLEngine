#pragma once


#include "OpenGLEngine.h"
#include "Util/Camera.h"
#include "ComponentArray.h"

#include <GLFW/glfw3.h>

#include "local_glm.h"


struct CameraComponent {
	static ComponentEnum type; // to be set by the user

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 front    = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);

	GLfloat yaw   = -90.0f;
	GLfloat pitch = 0;

	GLfloat MovementSpeed    = 5.0f;
	GLfloat MouseSensitivity = 0.2f;
	GLfloat Zoom             = 60.0f;
};

ComponentEnum CameraComponent::type = ComponentEnum(0);

typedef GenericComponentArray<CameraComponent> CameraComponentArrayType;

class CameraSystem : public System {
	const int key_forwards  = GLFW_KEY_W;
	const int key_backwards = GLFW_KEY_S;
	const int key_left      = GLFW_KEY_A;
	const int key_right     = GLFW_KEY_D;

	virtual void internalRunEntity(unsigned int UID) override {
		// TODO handle inputs and move camera
	};

public:
	void setCameraComponentEnum(ComponentEnum camera) {
		mRequiredComponents[camera] = true;
	}



};