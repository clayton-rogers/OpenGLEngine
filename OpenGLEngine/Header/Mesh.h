#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLM Mathemtics
#include "local_glm.h"

#include <string>
#include <vector>


class Mesh {
	GLuint VAO;
	GLuint VBO;
	GLsizei numVertex;
	bool mIsLoaded = false;

	std::string replaceStrChar(std::string str, const std::string& replace, char ch);

	const static std::vector<GLfloat> EMPTY_VECTOR;

public:

	bool isLoaded() const {
		return mIsLoaded;
	}

	void load(std::string path);
	const std::vector<GLfloat> loadObject(std::string path);
	void Draw(GLuint program, glm::mat4 model, glm::vec3 colour, GLfloat shininess) const;
};


