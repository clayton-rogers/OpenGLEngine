#pragma once

#include <glm/gtc/type_ptr.hpp>

#include "GUI.h"
#include "Text.h"
#include "Object.h"

namespace GUI {

	class TextLine : public Object {
		GLuint VAO;
		GLuint VBO;

		glm::vec3 mColour;
		Text mText;

		void genAndLoadQuads() {
			std::vector<GLfloat> quad;
			std::vector<GLfloat> vertices;

			quad.push_back(0.0f); // lower left
			quad.push_back(0.0f);

			quad.push_back(0.0f); // upper left
			quad.push_back(mSize.y);

			quad.push_back(mSize.x); // lower right
			quad.push_back(0.0f);

			quad.push_back(mSize.x); // upper right
			quad.push_back(mSize.y);

			for (int index : QUAD_INDICES) {
				vertices.push_back(quad.at(index * 2 + 0));
				vertices.push_back(quad.at(index * 2 + 1));
			}

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(0));
		}
	public:

		TextLine(glm::vec2 position, std::string text, glm::vec3 backgroundColour = glm::vec3(200.0f)) :
			Object(position, glm::vec2(2 * BORDER_SIZE + text.size() * characterWidth, 2 * BORDER_SIZE + characterHeight)),
			mColour(backgroundColour / 255.0f),
			mText(textTexID, &vcr, text, position + BORDER_SIZE)
		{
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			genAndLoadQuads();
		}

		~TextLine()
		{
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
		}

		virtual void doAction(GLuint program) override {

			// Draw the background colour
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(mPosition, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniform1i(glGetUniformLocation(program, "shaderType"), COLOUR);
			glUniform4f(glGetUniformLocation(program, "colour"), mColour.r, mColour.r, mColour.b, 1.0f);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

			// Draw the text on top of it
			glUniform1i(glGetUniformLocation(program, "shaderType"), TEXTURE);
			mText.doAction(program);
		}

		void setPosition(glm::vec2 position) {
			this->mPosition = position;
			this->mText.setPosition(position + BORDER_SIZE);
		}

		void setText(std::string text) {
			this->mText.setText(text);
			this->mSize = glm::vec2(2 * BORDER_SIZE + text.size() * characterWidth, 2 * BORDER_SIZE + characterHeight);
			genAndLoadQuads();
		}

	};

}