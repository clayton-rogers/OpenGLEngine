#pragma once

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
#include <fstream>
#include <sstream>


class Mesh {
	GLuint VAO = -1;
	GLuint VBO = -1;
	GLsizei numVertex;

	std::string replaceStrChar(std::string str, const std::string& replace, char ch) {

		// set our locator equal to the first appearance of any character in replace
		size_t found = str.find_first_of(replace);

		while (found != std::string::npos) { // While our position in the sting is in range.
			str[found] = ch; // Change the character at position.
			found = str.find_first_of(replace, found + 1); // Relocate again.
		}

		return str; // return our new string.
	}

	const static std::vector<GLfloat> EMPTY_VECTOR;

public:

	bool isLoaded() {
		return VAO != -1;
	}

	void load(std::string path) {
		std::vector<GLfloat> mesh = loadObject(path);

		if (mesh != EMPTY_VECTOR) {
			numVertex = mesh.size() / 6; // 6 float attributes per vertex

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *mesh.size(), &mesh[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0); // vertices
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1); // normals
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glBindVertexArray(0);
		}
	}

	const std::vector<GLfloat> loadObject(std::string path) {

		std::ifstream objectFile;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::ivec2> indicies;

		try {
			objectFile.open(path);
			if (objectFile.fail()) {
				std::cout << "Could not open file: " << path << std::endl;
				return EMPTY_VECTOR;
			}

			while (!(objectFile.eof() || objectFile.fail())) {
				std::string line;
				std::getline(objectFile, line);
				line = replaceStrChar(line, "/", ' ');  // replace any '/' in the faces lines with spaces

														// ignore blank lines
				if (line.length() == 0) {
					continue;
				}

				std::stringstream lineStream = std::stringstream(line);
				std::string ignoredChars; // the ignored first char of the line
				switch (line.at(0)) {
					case 'v':
					{
						if (line.at(1) == 'n') {
							glm::vec3 normal;
							lineStream >> ignoredChars >> normal.x >> normal.y >> normal.z;
							normals.push_back(normal);
						} else {
							glm::vec3 vertex;
							lineStream >> ignoredChars >> vertex.x >> vertex.y >> vertex.z;
							vertices.push_back(vertex);
						}
						break;
					}
					case 'f':
					{
						glm::ivec2 index[3];
						lineStream >> ignoredChars
							>> index[0].x >> index[0].y
							>> index[1].x >> index[1].y
							>> index[2].x >> index[2].y;
						indicies.insert(std::end(indicies), std::begin(index), std::end(index));
						break;
					}
					default:
						// comment lines are handled under the default case
						break;
				}
			}

		} catch (std::ifstream::failure e) {
			std::cout << "Mesh file failure: " << e.what() << std::endl;
			return EMPTY_VECTOR;
		}


		std::vector<GLfloat> output;

		for (glm::ivec2 index : indicies) {
			// vertex indicies in the file start from one, but our vector starts from 0
			output.push_back(vertices.at(index.x - 1).x);
			output.push_back(vertices.at(index.x - 1).y);
			output.push_back(vertices.at(index.x - 1).z);

			output.push_back(normals.at(index.y - 1).x);
			output.push_back(normals.at(index.y - 1).y);
			output.push_back(normals.at(index.y - 1).z);

		}

		return output;
	}

	void Draw(GLuint program, glm::mat4 model, glm::vec3 colour, GLfloat shininess) const {

		glUniform3f(glGetUniformLocation(program, "material.colour"), colour.r, colour.g, colour.b);
		glUniform1f(glGetUniformLocation(program, "material.shininess"), shininess);
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, numVertex);
		glBindVertexArray(0);
	}

};


const std::vector<GLfloat> Mesh::EMPTY_VECTOR = std::vector<GLfloat>();