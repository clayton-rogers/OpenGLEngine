#pragma once

#include "ComponentArray.h"

#include "local_glm.h"

struct PositionComponent {
	static ComponentEnum type;
	glm::vec3 position = glm::vec3(0.0);
};
typedef GenericComponentArray<PositionComponent> PositionComponentArrayType;


struct VelocityComponent {
	static ComponentEnum type;
	glm::vec3 velocity = glm::vec3(0.0f);
};
typedef GenericComponentArray<VelocityComponent> VelocityComponentArrayType;


struct MassComponent {
	static ComponentEnum type;
	glm::vec3 frameForce = glm::vec3(0.0f);
	float mass = 1.0f;
};
typedef GenericComponentArray<MassComponent> MassComponentArrayType;
