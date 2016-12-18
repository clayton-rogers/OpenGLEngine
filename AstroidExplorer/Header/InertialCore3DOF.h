#pragma once

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class InertialCore3DOF {

	glm::vec3 mFrameForce = glm::vec3(0.0f);

public:
	glm::vec3 mVelocity = glm::vec3(0.0f);
	glm::vec3 mPosition;
	float mMass;
	
	InertialCore3DOF(
		float mass = 10.0f,
		glm::vec3 position = glm::vec3(0.0f)) :
	mMass(mass),
	mPosition(position)
	{
		
	}

	void addForce(glm::vec3 force) {
		mFrameForce += force;
	}

	void step(double deltaT) {
		static double totalSimTime = 0;
		totalSimTime += deltaT;
		glm::vec3 acceleration = mFrameForce / mMass;
		mVelocity += acceleration * float(deltaT);
		mPosition += mVelocity * float(deltaT);

		// reset the force to zero
		mFrameForce = glm::vec3();
	}

	static InertialCore3DOF merge(const InertialCore3DOF& a, const InertialCore3DOF& b) {
		InertialCore3DOF output;

		output.mMass = a.mMass + b.mMass;

		// average the positions
		output.mPosition = (a.mPosition + b.mPosition) / 2.0f;

		// perfectly inelastic collision
		output.mVelocity = (a.mMass * a.mVelocity + b.mMass * b.mVelocity) / (a.mMass + b.mMass);

		return output;
	}
};
