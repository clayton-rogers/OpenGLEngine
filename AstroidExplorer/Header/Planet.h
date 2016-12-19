#pragma once

#include "Drawable.h"
#include "Mesh.h"
#include "InertialCore3DOF.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <cmath>


class Planet : public Drawable {
private:
	static Mesh mMesh;
	static const std::string planetObjectPath;

	int mDrawListIndex;
public:
	glm::vec3 mColour;
	GLfloat mShininess = 64;
	float mRadius;
	InertialCore3DOF mInertialCore;

	Planet() {
		// Load the mesh the first time a planet is created
		if (!mMesh.isLoaded()) {
			mMesh.load(planetObjectPath);
		}

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<GLfloat> posGen(-30.0f, 30.0f);
		mInertialCore.mPosition.x = posGen(gen);
		mInertialCore.mPosition.y = posGen(gen);
		mInertialCore.mPosition.z = posGen(gen);
		std::uniform_real_distribution<GLfloat> velGen(-5.0f, 5.0f);
		mInertialCore.mVelocity.x = velGen(gen);
		mInertialCore.mVelocity.y = velGen(gen);
		mInertialCore.mVelocity.z = velGen(gen);
		std::uniform_real_distribution<GLfloat> colourGen(0.0f, 1.0f);
		mColour.r = colourGen(gen);
		mColour.g = colourGen(gen);
		mColour.b = colourGen(gen);
		std::uniform_real_distribution<GLfloat> massGen(500.0f, 50000.0f);
		{
			mInertialCore.mMass = massGen(gen);
			float volume = mInertialCore.mMass / 5540; // density of earth
			mRadius = std::cbrt(volume * (3.0f / 4.0f) / 3.1415927f);
		}
	}

	// Merge two planets
	Planet(const Planet& p1, const Planet& p2) {

		mInertialCore = InertialCore3DOF::merge(p1.mInertialCore, p2.mInertialCore);

		mShininess = (p1.mShininess + p2.mShininess) / 2;
		mColour = (p1.mInertialCore.mMass * p1.mColour + p2.mInertialCore.mMass * p2.mColour) / (p1.mInertialCore.mMass + p2.mInertialCore.mMass);

		float volume = mInertialCore.mMass / 5540; // density of earth
		mRadius = std::cbrt(volume * (3.0f / 4.0f) / 3.1415927f);
	}
	
	void Draw(GLuint program) const override {

		glm::mat4 model;
		model = glm::translate(model, mInertialCore.mPosition);
		model = glm::scale(model, glm::vec3(mRadius));

		mMesh.Draw(program, model, mColour, mShininess);
	};

};

// TODO: move to cpp if we end up using multiple files
const std::string Planet::planetObjectPath = "./Resources/Planet.obj";
Mesh Planet::mMesh;