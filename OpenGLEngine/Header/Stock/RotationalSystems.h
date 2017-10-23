#pragma once

#include "RotationalComponents.h"
#include "InertialComponents.h"

#include "ComponentArray.h"
#include "System.h"
#include "OpenGLEngine.h"

class AngularVelocitySystem : public System {
	virtual void internalRunEntity(unsigned int UID) override {
		// propagate angular velocity into angular position
		AngularPositionComponent& p = OpenGLEngine::getComponent<AngularPositionComponent>(UID);
		AngularVelocityComponent& v = OpenGLEngine::getComponent<AngularVelocityComponent>(UID);

		if (glm::length2(v.angularVelocity) != 0.0f) {
			glm::vec3 axis = glm::normalize(v.angularVelocity);
			float angle = glm::length(v.angularVelocity) * OpenGLEngine::getInputState().deltaT;
			p.angularPosition = glm::rotate(p.angularPosition, angle, axis);
		}
	}

public:
	AngularVelocitySystem(ComponentEnum angularPosition, ComponentEnum angularVelocity) {
		mRequiredComponents[angularPosition] = true;
		mRequiredComponents[angularVelocity] = true;
	}
};

class MomentofInertiaSystem : public System {
	virtual void internalRunEntity(unsigned int UID) override {
		// propagate frame torques into delta angular velocities
		AngularVelocityComponent& v = OpenGLEngine::getComponent<AngularVelocityComponent>(UID);
		MomentofInertiaComponent& m = OpenGLEngine::getComponent<MomentofInertiaComponent>(UID);

		glm::vec3 angularAcceleration = m.frameTorque / m.momentofInertia;
		glm::vec3 deltaVelocity = angularAcceleration * OpenGLEngine::getInputState().deltaT;
		v.angularVelocity += deltaVelocity;

		// Reset frame torque to zero
		m.frameTorque = glm::vec3(0.0f);
	}

public:
	MomentofInertiaSystem(ComponentEnum angularVelocity, ComponentEnum momentofInertia) {
		mRequiredComponents[angularVelocity] = true;
		mRequiredComponents[momentofInertia] = true;
	}
};