#pragma once

#include <glm/glm.hpp>

#include "ComponentArray.h"
#include "Shader.h"
#include "Mesh.h"


struct DrawComponent {
	Shader* shader;
	Mesh* mesh;
	glm::vec3 colour = glm::vec3(1.0f, 0.0f, 1.0f); // PINK!
	float shininess = 64;
};

typedef GenericComponentArray<DrawComponent> DrawComponentArrayType;

struct GeneralDrawComponent {
	Shader* shader;
	Mesh* mesh;
	glm::vec3 colour = glm::vec3(1.0f, 0.0f, 1.0f);
	float shininess = 64;
	glm::mat4 rotationScaleMatrix;
};

typedef GenericComponentArray<GeneralDrawComponent> GeneralDrawComponentArrayType;

struct PositionComponent {
	glm::vec3 position = glm::vec3(0.0);
};

typedef GenericComponentArray<PositionComponent> PositionComponentArrayType;


struct VelocityComponent {
	glm::vec3 velocity = glm::vec3(0.0f);
};

typedef GenericComponentArray<VelocityComponent> VelocityComponentArrayType;

struct MassComponent{
	glm::vec3 frameForce = glm::vec3(0.0f);
	float mass = 1.0f;
};

typedef GenericComponentArray<MassComponent> MassComponentArrayType;

struct CoalescableComponent {
	float radius;
};

typedef GenericComponentArray<CoalescableComponent> CoalescableComponentArrayType;

struct BulletComponent {
	float radius;
};

typedef GenericComponentArray<BulletComponent> BulletComponentArrayType;