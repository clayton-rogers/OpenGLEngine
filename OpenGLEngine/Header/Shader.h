#pragma once

#include <GL/glew.h> // include glew to get all the required OpenGL headers

class Shader {
public:

	GLuint Program;

	// Constructor reads and builds the shader
	Shader(){}
	Shader(
		const GLchar* vertexShaderFilepath,
		const GLchar* fragmentShaderFilepath);

	void Use();
};