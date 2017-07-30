#version 330 core


// This must be kept in line with the definition in GUI.h:27
#define COLOUR  0
#define TEXTURE 1

in vec2 TexCoords;

uniform sampler2D myTexture;
uniform vec4 colour;
uniform int shaderType;

out vec4 out_colour;

void main() {
	
	if (shaderType == COLOUR) {
		out_colour = colour;
	} else if (shaderType == TEXTURE) {
		out_colour = texture(myTexture, TexCoords);
		if (out_colour != vec4(0.0, 0.0, 0.0, 1.0)) {
			discard;
		}
	} else {
		out_colour = vec4(1.0, 0.0, 1.0, 1.0); // ouput bright pink if we fucked up
	}
}