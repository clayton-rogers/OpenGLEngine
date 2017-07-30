#pragma once


#include "Mesh.h"
#include "InertialCore3DOF.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

class Lazer {
	static Mesh mMesh;
	static const std::string lazerObjectPath;
	static const float LAZER_VELOCITY;

	InertialCore3DOF mInertialCore;
	glm::vec3 mDirection;

public:

	Lazer(glm::vec3 position, glm::vec3 direction) :
		mInertialCore(1, position), 
		mDirection(glm::normalize(direction))
	{
		if (!mMesh.isLoaded()) {
			mMesh.load(lazerObjectPath);
		}
		mInertialCore.mVelocity = glm::normalize(direction) * LAZER_VELOCITY;
	}

	void step(double deltaT) {
		mInertialCore.step(deltaT);
	}

	void draw(GLuint program) const {
		glm::mat4 model;
		model = glm::translate(model, mInertialCore.mPosition);
		
		glm::vec3 modelDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 rotationVector = glm::cross(modelDirection, mDirection);
		float rotationAngle = glm::angle(modelDirection, mDirection);

		model = glm::rotate(model, rotationAngle, rotationVector);
		model = glm::scale(model, glm::vec3(0.1f, 0.5, 0.1f));

		mMesh.Draw(program, model, glm::vec3(1.0, 0.0, 0.0), 128.0f);
	}
};

Mesh Lazer::mMesh;
const std::string Lazer::lazerObjectPath = "./Resources/Lazer.obj";
const float Lazer::LAZER_VELOCITY = 30.0f;