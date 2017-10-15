#pragma once


#include "Mesh.h"
#include "ComponentArray.h"

#include "local_glm.h"

struct GeneralDrawComponent {
	static ComponentEnum type;
	Mesh* mesh;
	glm::vec3 colour = glm::vec3(1.0f, 0.0f, 1.0f);
	float shininess = 64;
	glm::mat4 rotationScaleMatrix;
};

typedef GenericComponentArray<GeneralDrawComponent> GeneralDrawComponentArrayType;