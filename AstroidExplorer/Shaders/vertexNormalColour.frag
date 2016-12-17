#version 330 core


in struct VS_OUT {
	vec3 normal;
	vec3 fragPos;
} vs_out;
out vec4 color;


struct Material {
	vec3 colour;
	float shininess;
};
uniform Material material;



void main() {
	// TODO: all lights	

	color = vec4(material.colour, 1.0);
}