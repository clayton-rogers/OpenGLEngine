#pragma once

#include "InertialComponents.h"

#include "ComponentArray.h"
#include "System.h"
#include "OpenGLEngine.h"

class MassSystem : public System {

	virtual void internalRunEntity(unsigned int UID) override {
		VelocityComponent& i = OpenGLEngine::getComponent<VelocityComponent>(UID);
		MassComponent&     m = OpenGLEngine::getComponent<MassComponent>(UID);

		glm::vec3 acceleration = m.frameForce / m.mass;
		i.velocity += acceleration * OpenGLEngine::getInputState().deltaT;

		// reset the force to zero
		m.frameForce = glm::vec3();
	}

public:

	MassSystem(ComponentEnum velocity, ComponentEnum mass) {
		mRequiredComponents[velocity] = true;
		mRequiredComponents[mass] = true;
	}
};

class VelocitySystem : public System {
	virtual void internalRunEntity(unsigned int UID) override {
		PositionComponent& p = OpenGLEngine::getComponent<PositionComponent>(UID);
		VelocityComponent& v = OpenGLEngine::getComponent<VelocityComponent>(UID);

		p.position += v.velocity * OpenGLEngine::getInputState().deltaT;
	}

public:

	VelocitySystem(ComponentEnum position, ComponentEnum velocity) {
		mRequiredComponents[position] = true;
		mRequiredComponents[velocity] = true;
	}
};