#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../GenericActionList.h"
#include "Font.h"
#include "GUI.h"

#include <string>
#include <vector>

namespace GUI {

	class Text : public Action<GLuint> {
	private:

		GLuint mTextTexID;
		GLuint VAO;
		GLuint VBO;
		Font* mFont;
		std::string mText;

		std::vector<GLfloat> mVertices;
		GLsizei numVertex;

	public:
		glm::vec2 mPosition;

	private:


		void generateQuads() {
			mVertices.clear();
			GLfloat xDelta = 0.0f;

			for (unsigned int i = 0; i < mText.size(); i++) {
				std::vector<GLfloat> quad;
				GLfloat height = GLfloat(mFont->getFontCharacter(mText.at(i)).height);
				GLfloat width = GLfloat(mFont->getFontCharacter(mText.at(i)).width);
				GLfloat xOffset = GLfloat(mFont->getFontCharacter(mText.at(i)).xoffset);
				CornerUV uvs = mFont->getUVs(mText.at(i));

				quad.push_back(xDelta + xOffset); // lower left
				quad.push_back(0.0f);
				quad.push_back(uvs.lowerLeft.x);
				quad.push_back(uvs.lowerLeft.y);

				quad.push_back(xDelta + xOffset); // upper left
				quad.push_back(height);
				quad.push_back(uvs.lowerLeft.x);
				quad.push_back(uvs.upperRight.y);

				quad.push_back(xDelta + width + xOffset); // lower right
				quad.push_back(0.0f);
				quad.push_back(uvs.upperRight.x);
				quad.push_back(uvs.lowerLeft.y);

				quad.push_back(xDelta + width + xOffset); // upper right
				quad.push_back(height);
				quad.push_back(uvs.upperRight.x);
				quad.push_back(uvs.upperRight.y);

				for (int index : QUAD_INDICES) {
					mVertices.push_back(quad.at(index * 4 + 0));
					mVertices.push_back(quad.at(index * 4 + 1));
					mVertices.push_back(quad.at(index * 4 + 2));
					mVertices.push_back(quad.at(index * 4 + 3));
				}

				xDelta += mFont->getFontCharacter(mText.at(i)).xadvance;
			}

			numVertex = mVertices.size() / 4; // 4 float elements per vertex
		}

		void loadQuadsToGPU() {
			if (mVertices.size() != 0) {
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mVertices.size(), &mVertices[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(0));
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
			}
		}

	public:
		Text(GLuint textTextureID, Font* font, std::string text, glm::vec2 position) :
			mTextTexID(textTextureID), mFont(font), mText(text), mPosition(position)
		{
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			generateQuads();
			loadQuadsToGPU();
		}

		~Text() {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
		}

		void setText(std::string text) {
			mText = text;
			generateQuads();
			loadQuadsToGPU();
		}

		void setPosition(glm::vec2 position) {
			mPosition = position;
		}

		virtual void doAction(GLuint program) override {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mTextTexID);
			glUniform1i(glGetUniformLocation(program, "myTexture"), 0);

			glm::mat4 model;
			model = glm::translate(model, glm::vec3(mPosition, 0.0f));
			model = glm::scale(model, glm::vec3(TEXT_SCALE));
			glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, numVertex);
			glBindVertexArray(0);
		}
	};

}