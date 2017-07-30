#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Shader.h"
#include "../GenericActionList.h"
#include "Font.h"

#include <string>
#include <vector>


namespace GUI {

	Shader fontShader;
	Font vcr("../OpenGLEngine/Resources/Font/VCR-OSD-mono2.png", "../OpenGLEngine/Resources/Font/VCR-OSD-mono.txt");
	const float TEXT_SCALE = 1.0f / 3.0f;
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
		fontShader = Shader("../OpenGLEngine/Shaders/text.vert", "../OpenGLEngine/Shaders/text.frag");
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

}