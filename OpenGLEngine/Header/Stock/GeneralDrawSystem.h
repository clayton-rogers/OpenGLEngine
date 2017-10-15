#pragma once

#include "OpenGLEngine.h"

#include "GeneralDrawComponent.h"
#include "InertialComponents.h"

class GeneralDrawSystem : public System {
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

	static void setRequiredComponentEnums(ComponentEnum generalDraw, ComponentEnum position) {
		mRequiredComponents[generalDraw] = true;
		mRequiredComponents[position] = true;
	}
};