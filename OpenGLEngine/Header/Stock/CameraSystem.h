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
	glm::vec3 right    = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);

	GLfloat yaw   = -90.0f;
	GLfloat pitch = 0;

	GLfloat MovementSpeed    = 30.0f;
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

	const glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	ComponentEnum CAMERA_ENUM;

	void updateCameraVectors(CameraComponent& camera) {
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		front.y = sin(glm::radians(camera.pitch));
		front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		camera.front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		camera.right = glm::normalize(glm::cross(camera.front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		camera.up = glm::normalize(glm::cross(camera.right, camera.front));
	}

	virtual void internalRunEntity(unsigned int UID) override {
		auto& input = OpenGLEngine::getInputState();
		auto& camera = OpenGLEngine::getComponent<CameraComponent>(UID);
		GLfloat velocity = camera.MovementSpeed * input.deltaT;

		if (input.keys[key_forwards]) {
			camera.position += camera.front * velocity;
		}
		if (input.keys[key_backwards]) {
			camera.position -= camera.front * velocity;
		}
		if (input.keys[key_left]) {
			camera.position -= camera.right * velocity;
		}
		if (input.keys[key_right]) {
			camera.position += camera.right * velocity;
		}

		camera.yaw   += input.xOffset * camera.MouseSensitivity;
		camera.pitch += input.yOffset * camera.MouseSensitivity;
		updateCameraVectors(camera);

		// TODO handle scrolling
	};

public:
	void setCameraComponentEnum(ComponentEnum camera) {
		CAMERA_ENUM = camera;
		mRequiredComponents[camera] = true;
	}
};