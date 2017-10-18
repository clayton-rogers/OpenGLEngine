#pragma once

#include "OpenGLEngine.h"
#include "ComponentArray.h"

#include <GLFW/glfw3.h>

#include "local_glm.h"

struct CameraComponent {
	static ComponentEnum type; // to be set by the user

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90.0f;
	float pitch = 0;

	float MovementSpeed = 30.0f;
	float MouseSensitivity = 0.2f;
	float Zoom = 60.0f;
};
typedef GenericComponentArray<CameraComponent> CameraComponentArrayType;
