#version 330 core


in struct VS_OUT {
	vec3 normal;
	vec3 fragPos;
} vs_out;
out vec4 colourOut;


struct Material {
	vec3 colour;
	float shininess;
};


uniform vec3     lightDirection;
uniform vec3     eyePosition;
uniform Material material;

vec3 calculateAmbient() {
	const float ambientBrightness = 0.1;

	return material.colour * ambientBrightness;
}

vec3 calculateDiffuse(vec3 normal, vec3 lightDirection) {
	const float diffuseBrightness = 0.7;

	float diffuse = dot(normal, -lightDirection);
	diffuse = max(diffuse, 0.0);

	return material.colour * diffuseBrightness * diffuse;
}


// Blinn-Phong specular lighting
vec3 calculateSpecular(vec3 normal, vec3 eyeDirection, vec3 lightDirection) {
	const float specularBrightness = 0.5;

	vec3 halfwayDirection = normalize((-lightDirection) + (-eyeDirection));
	float specular = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);

	return vec3(specularBrightness * specular);
}

void main() {
	// TODO: all lights	

	vec3 normal = normalize(vs_out.normal);
	vec3 lightDir = normalize(lightDirection);
	vec3 eyeDirection = normalize(vs_out.fragPos - eyePosition);

	vec3 colour = calculateAmbient();
	colour += calculateDiffuse(normal, lightDir);
	colour += calculateSpecular(normal, eyeDirection, lightDir);

	colourOut = vec4(colour, 1.0);
}