#version 330 core

in vec2 TexCoords;

uniform sampler2D myTexture;

out vec4 colour;

void main() {
	colour = texture(myTexture, TexCoords);
	if (colour != vec4(0.0, 0.0, 0.0, 1.0)) {
		discard;
	}
}