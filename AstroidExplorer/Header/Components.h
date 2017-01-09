#pragma once

#include <glm/glm.hpp>

#include "ComponentArray.h"
#include "Shader.h"
#include "Mesh.h"


struct DrawComponent {
	Shader* shader;
	Mesh* mesh;
	glm::vec3 colour;
	float shininess = 64;
};

typedef GenericComponentArray<DrawComponent> DrawComponentArrayType;


struct PositionComponent {
	glm::vec3 position;
};

typedef GenericComponentArray<PositionComponent> PositionComponentArrayType;


struct InertialComponent {
	glm::vec3 velocity;
	glm::vec3 frameForce;
	float mass;
};

typedef GenericComponentArray<InertialComponent> InertialComponentArrayType;


struct RadiusComponent {
	float radius;
};

typedef GenericComponentArray<RadiusComponent> RadiusComponentArrayType;