#pragma once

#include "Drawable.h"
#include "ObjectLoader.h"

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


class Planet : public Drawable {
private:
	static GLuint VAO;
	static GLuint VBO;
	static GLsizei numVertex;

	
	static const std::string planetObjectPath;

public:

	glm::vec3 mColour;

	Planet(glm::vec3 colour = glm::vec3(1.0f, 0.0f, 100.0f / 255.0f)) : mColour(colour) {
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
	}

	
	void Draw(GLuint program) const override {
		
		glUniform3f(glGetUniformLocation(program, "material.colour"), mColour.r, mColour.g, mColour.b);
		glUniform1f(glGetUniformLocation(program, "material.shininess"), 60.0f);

		glm::mat4 model;
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