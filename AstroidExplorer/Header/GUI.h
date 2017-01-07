#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm\glm.hpp>

#include "Shader.h"
#include "GenericActionList.h"
#include "Font.h"

#include <string>
#include <vector>


namespace GUI {

	Shader fontShader;
	Font vcr("./Resources/Font/VCR-OSD-mono2.png", "./Resources/Font/VCR-OSD-mono.txt");
	const float TEXT_SCALE = 1.0 / 3.0;
	const float characterWidth = float(vcr.getFontCharacter('a').xadvance) * TEXT_SCALE;
	const float characterHeight = float(vcr.getFontCharacter('L').height) * TEXT_SCALE;
	GLuint textTexID;
	const float BORDER_SIZE = 3.0f; // pixels

	GLuint WINDOW_WIDTH;
	GLuint WINDOW_HEIGHT;

	// This must be kept in line with the definition in text.frag:5
	enum ShaderType {COLOUR, TEXTURE};

	/* 1 --- 3
	*   | |
	* 0 --- 2
	*/
	const std::vector<int> QUAD_INDICES = { 0, 3, 1, 0, 2, 3 };

	GenericActionList<GLuint> GUIdrawlist;

	void init(GLuint windowWidth, GLuint windowHeight) {
		fontShader = Shader("./Shaders/text.vert", "./Shaders/text.frag");
		WINDOW_WIDTH = windowWidth;
		WINDOW_HEIGHT = windowHeight;
		vcr.uploadTextureToGPU(textTexID);
	}

	void draw() {
		// TODO FUTURE window width and height should be params in case they change.
		fontShader.Use();
		glm::mat4 projection = glm::ortho(0.0f, float(WINDOW_WIDTH), 0.0f, float(WINDOW_HEIGHT), 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(fontShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		GUIdrawlist.doActions(fontShader.Program);
	}

	class Object : public Action<GLuint> {
	private:

		glm::vec2 mClickPosition;
		glm::vec2 mClickSize;

		void(*mClickCallback)(bool& handled) = nullptr;

	protected:
		glm::vec2 mPosition;
		glm::vec2 mSize;

	public:
		Object(
			glm::vec2 position = glm::vec2(0.0f),
			glm::vec2 size = glm::vec2(0.0f),
			glm::vec2 clickPosition = glm::vec2(0.0f),
			glm::vec2 clickSize = glm::vec2(0.0f)
		) :
			mPosition(position),
			mSize(size),
			mClickPosition(clickPosition),
			mClickSize(clickSize) {
		}

		bool isClickInside(glm::vec2 clickPosition) const {
			// A vector from bottom right to click position
			glm::vec2 a = clickPosition - mClickPosition;
			// A vector from click position to upper left
			glm::vec2 b = (mClickPosition + mClickSize) - clickPosition;

			// if the click position is inside of the angle betweeen a and b
			// will be 90d < theta < 180d 
			//       = pi/2 < theta < pi
			float theta = glm::angle(a, b);
			return (glm::pi<float>() / 2.0f) < theta &&
				theta < glm::pi<float>();
		}

		void handleClick(bool& handled) {
			if (mClickCallback != nullptr) {
				mClickCallback(handled);
			} else {
				handled = false;
			}
		}

		void registerClickCallback(void(*clickCallback)(bool& handled)) {
			mClickCallback = clickCallback;
		}
	};

	class Text : public Action<GLuint> {
	private:

		GLuint mTextTexID = -1;
		GLuint VAO = -1;
		GLuint VBO = -1;
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

			for (int i = 0; i < mText.size(); i++) {
				std::vector<GLfloat> quad;
				GLfloat height = mFont->getFontCharacter(mText.at(i)).height;
				GLfloat width = mFont->getFontCharacter(mText.at(i)).width;
				GLfloat xOffset = mFont->getFontCharacter(mText.at(i)).xoffset;
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
			Object(position, glm::vec2(2*BORDER_SIZE + text.size() * characterWidth, 2*BORDER_SIZE + characterHeight)),
			mColour(backgroundColour / 255.0f), 
			mText(textTexID, &vcr, text, position+BORDER_SIZE)
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
			this->mText.setPosition(position+BORDER_SIZE);
		}

	};

}