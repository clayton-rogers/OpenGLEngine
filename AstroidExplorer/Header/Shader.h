#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h> // include glew to get all the required OpenGL headers

class Shader {
public:

	GLuint Program;

	// Constructor reads and builds the shader
	Shader(
		const GLchar* vertexShaderFilepath,
		const GLchar* fragmentShaderFilepath) {

		std::string vertexShaderCode;
		std::string fragmentShaderCode;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;

		// ensure ifstream objects can throw exceptions
		vertexShaderFile.exceptions(std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::badbit);

		try {
			// Open files
			vertexShaderFile.open(vertexShaderFilepath);
			if (vertexShaderFile.fail()) {
				std::cout << "Could not open file: " << vertexShaderFilepath << std::endl;
				return;
			}
			fragmentShaderFile.open(fragmentShaderFilepath);
			if (fragmentShaderFile.fail()) {
				std::cout << "Could not open file: " << fragmentShaderFilepath << std::endl;
				return;
			}
			std::stringstream vertexShaderStream, fragmentShaderStream;

			// Read file's buffer contents into streams
			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();

			// Close files
			vertexShaderFile.close();
			fragmentShaderFile.close();

			// Convert to GLchar
			vertexShaderCode = vertexShaderStream.str();
			fragmentShaderCode = fragmentShaderStream.str();
		} catch (std::ifstream::failure e) {
			std::cout << "Shader file failure:" << e.what() << std::endl;
			this->Program = -1;
			return;
		}

		const GLchar* vertexShaderSource = vertexShaderCode.c_str();
		const GLchar* fragmentShaderSource = fragmentShaderCode.c_str();

		// Vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "Vertex shader compile: " << infoLog << std::endl;
			this->Program = -1;
			return;
		}

		// Fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "Fragment shader compile: " << infoLog << std::endl;
			this->Program = -1;
			return;
		}

		// Shader program
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "Shader program compile: " << infoLog << std::endl;
			this->Program = -1;
			return;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		this->Program = shaderProgram;
	}

	void Use() {
		glUseProgram(this->Program);
	}
};