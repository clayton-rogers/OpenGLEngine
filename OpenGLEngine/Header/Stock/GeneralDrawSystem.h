#pragma once

#include "OpenGLEngine.h"

#include "GeneralDrawComponent.h"
#include "CameraComponent.h"
#include "InertialComponents.h"

class GeneralDrawSystem : public System {

	ComponentBitset cameraComponentBitset;

	// Setup the camera after finding it
	virtual void preLoop() {
		unsigned int cameraUID;

		for (auto& entity : OpenGLEngine::componentManager.getEntities()) {
			if ((entity.second & cameraComponentBitset) == cameraComponentBitset) {
				cameraUID = entity.first;
				break;
			}
		}

		CameraComponent& c = OpenGLEngine::getComponent<CameraComponent>(cameraUID);
		Shader& shader = OpenGLEngine::getShader();

		shader.Use();
		glm::mat4 view = glm::lookAt(c.position, c.position + c.front, c.up);
		glm::mat4 projection = glm::perspective(glm::radians(c.Zoom), OpenGLEngine::getAspectRatio(), 0.1f, 300.0f);


		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(glGetUniformLocation(shader.Program, "eyePosition"), c.position.x, c.position.y, c.position.z);
	}

	virtual void internalRunEntity(unsigned int UID) override {
		GeneralDrawComponent& d = OpenGLEngine::getComponent<GeneralDrawComponent>(UID);
		PositionComponent&    p = OpenGLEngine::getComponent<PositionComponent>(UID);

		OpenGLEngine::getShader().Use();

		glm::mat4 model;
		model = glm::translate(model, p.position);
		model *= d.rotationScaleMatrix;

		d.mesh->Draw(OpenGLEngine::getShader().Program, model, d.colour, d.shininess);
	}

public:

	GeneralDrawSystem(ComponentEnum generalDraw, ComponentEnum position, ComponentEnum camera) {
		mRequiredComponents[generalDraw] = true;
		mRequiredComponents[position] = true;

		cameraComponentBitset[camera] = true;
	}
};