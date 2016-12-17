#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

class Drawable {
public:
	virtual void Draw(GLuint program) const = 0;
};