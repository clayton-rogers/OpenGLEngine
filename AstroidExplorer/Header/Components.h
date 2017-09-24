#pragma once

#include <glm/glm.hpp>


#include "ComponentArray.h"


struct DrawComponent {
	static const ComponentEnum type = DRAW;
	Shader* shader;
	Mesh* mesh;
	glm::vec3 colour = glm::vec3(1.0f, 0.0f, 1.0f); // PINK!
	float shininess = 64;
};

typedef GenericComponentArray<DrawComponent> DrawComponentArrayType;

struct GeneralDrawComponent {
	static const ComponentEnum type = GENERAL_DRAW;
	Shader* shader;
	Mesh* mesh;
	glm::vec3 colour = glm::vec3(1.0f, 0.0f, 1.0f);
	float shininess = 64;
	glm::mat4 rotationScaleMatrix;
};

typedef GenericComponentArray<GeneralDrawComponent> GeneralDrawComponentArrayType;

struct PositionComponent {
	static const ComponentEnum type = POSITION;
	glm::vec3 position = glm::vec3(0.0);
};

typedef GenericComponentArray<PositionComponent> PositionComponentArrayType;


struct VelocityComponent {
	static const ComponentEnum type = VELOCITY;
	glm::vec3 velocity = glm::vec3(0.0f);
};

typedef GenericComponentArray<VelocityComponent> VelocityComponentArrayType;

struct MassComponent{
	static const ComponentEnum type = MASS;
	glm::vec3 frameForce = glm::vec3(0.0f);
	float mass = 1.0f;
};

typedef GenericComponentArray<MassComponent> MassComponentArrayType;

struct CoalescableComponent {
	static const ComponentEnum type = COALESCABLE;
	float radius;
};

typedef GenericComponentArray<CoalescableComponent> CoalescableComponentArrayType;

struct BulletComponent {
	static const ComponentEnum type = BULLET;
	float radius;
};

typedef GenericComponentArray<BulletComponent> BulletComponentArrayType;