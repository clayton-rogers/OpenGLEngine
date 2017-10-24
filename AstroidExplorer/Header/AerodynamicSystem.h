#pragma once

#include "AerodynamicComponent.h"
#include "Stock/InertialComponents.h"
#include "Stock/RotationalComponents.h"

#include "ComponentArray.h"
#include "System.h"
#include "OpenGLEngine.h"

#include "local_glm.h"

class AerodynamicSystem : public System {

	const float RHO = 1.225f; // kg/m^2

	virtual void internalRunEntity(unsigned int UID) override {
		VelocityComponent&         v = OpenGLEngine::getComponent<VelocityComponent>(UID);
		AngularPositionComponent& ap = OpenGLEngine::getComponent<AngularPositionComponent>(UID);
		AerodynamicComponent&   aero = OpenGLEngine::getComponent<AerodynamicComponent>(UID);
		MassComponent&             m = OpenGLEngine::getComponent<MassComponent>(UID);
		
		const glm::vec3 forward = ap.getForward();
		const glm::vec3 right = ap.getRight();
		const glm::vec3 up = ap.getUp();

		// Get component of the velocity projected on the center plane
		glm::vec3 antiProjectedVel = glm::dot(right, v.velocity) * right;
		glm::vec3 projectedVel = v.velocity - antiProjectedVel;

		float airspeed = glm::length(projectedVel);
		float alphaRAD = glm::angle(forward, glm::normalize(projectedVel));
		float alphaDEG = glm::degrees(alphaRAD);
		alphaDEG *= -glm::sign(glm::dot(projectedVel, up));

		// q = 1/2 rho v^2
		float dynamicPressure = 0.5f * RHO * airspeed * airspeed;

		// CL = CL_alpha * alpha
		float coefficientLift = aero.CLalpha * alphaDEG + aero.CLalphaOffset;

		// L = CL q A
		float lift = coefficientLift * dynamicPressure * aero.wingArea;

		glm::vec3 liftDirection = glm::cross(projectedVel, -right);

		glm::vec3 vectorLift = liftDirection * lift;


		// Apply the lift to the body
		m.frameForce += vectorLift;
	}

public:
	AerodynamicSystem(ComponentEnum angularPosition, ComponentEnum velocity, ComponentEnum aerodynamic, ComponentEnum mass) {
		mRequiredComponents[angularPosition] = true;
		mRequiredComponents[velocity] = true;
		mRequiredComponents[aerodynamic] = true;
		mRequiredComponents[mass] = true;
	}
};
