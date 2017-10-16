#pragma once


#include "OpenGLEngine.h"
#include "ComponentArray.h"

#include "CameraComponent.h"

#include <GLFW/glfw3.h>

#include "local_glm.h"

class FollowCameraSystem : public System {
	const int key_forwards = GLFW_KEY_W;
	const int key_backwards = GLFW_KEY_S;
	const int key_left = GLFW_KEY_A;
	const int key_right = GLFW_KEY_D;

	const glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

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
		CameraComponent& camera = OpenGLEngine::getComponent<CameraComponent>(UID);
		PositionComponent& p = OpenGLEngine::getComponent<PositionComponent>(UID);

		camera.position = p.position;

		// move back and up a bit
		camera.position -= camera.front * 10.0f;
		camera.position += camera.up * 2.0f;

		// TODO actually follow the position

		//updateCameraVectors(camera);
	};

public:
	FollowCameraSystem(ComponentEnum camera, ComponentEnum position) {
		mRequiredComponents[camera] = true;
		mRequiredComponents[position] = true;
	}
};