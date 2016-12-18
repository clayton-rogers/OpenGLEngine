#pragma once

#include "Drawable.h"
#include "ObjectLoader.h"
#include "InertialCore3DOF.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

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
	static GLuint VAO;
	static GLuint VBO;
	static GLsizei numVertex;

	
	static const std::string planetObjectPath;

	glm::vec3 mColour;
	GLfloat mShininess;
	
	float mRadius;
public:

	InertialCore3DOF mInertialCore;

	Planet() {
		// Load the mesh the first time a planet is created
		if (VAO == 0) {
			std::vector<GLfloat> planetVertices;
			planetVertices = ObjectLoader::loadObject(planetObjectPath);
			numVertex = planetVertices.size() / 6; // 6 float attributes per vertex
			if (planetVertices != ObjectLoader::EMPTY_VECTOR) {
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);

				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *planetVertices.size(), &planetVertices[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(0); // vertices
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
				glEnableVertexAttribArray(1); // normals
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
				glBindVertexArray(0);
			} else {
				std::cout << "Error loading planet model!" << std::endl;
			}
		}

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<GLfloat> posGen(-20.0f, 20.0f);
		mInertialCore.mPosition.x = posGen(gen);
		mInertialCore.mPosition.y = posGen(gen);
		mInertialCore.mPosition.z = posGen(gen);
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
		std::uniform_real_distribution<GLfloat> shininessGen(20.0f, 100.0f);
		mShininess = shininessGen(gen);
	}

	
	void Draw(GLuint program) const override {
		
		glUniform3f(glGetUniformLocation(program, "material.colour"), mColour.r, mColour.g, mColour.b);
		glUniform1f(glGetUniformLocation(program, "material.shininess"), mShininess);

		glm::mat4 model;
		model = glm::translate(model, mInertialCore.mPosition);
		model = glm::scale(model, glm::vec3(mRadius));
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, numVertex);
		glBindVertexArray(0);
	};

};

// TODO: move to cpp if we end up using multiple files
GLuint Planet::VAO = 0;
GLuint Planet::VBO = 0;
GLsizei Planet::numVertex = 0;
const std::string Planet::planetObjectPath = "./Resources/Planet.obj";