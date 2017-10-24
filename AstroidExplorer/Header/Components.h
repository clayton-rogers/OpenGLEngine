#pragma once


#include "Shader.h"
#include "Mesh.h"


#include "ComponentArray.h"

#include "local_glm.h"


enum ComponentEnum : char {
	DRAW,
	GENERAL_DRAW,
	POSITION,
	VELOCITY,
	MASS,
	COALESCABLE,
	BULLET,
	CAMERA,
	ANGULAR_POSITION,
	ANGULAR_VELOCITY,
	MOMENT_OF_INERTIA,
	AERODYNAMIC,
	ComponentTypeSize
};


struct DrawComponent {
	static const ComponentEnum type = DRAW;
	Mesh* mesh;
	glm::vec3 colour = glm::vec3(1.0f, 0.0f, 1.0f); // PINK!
	float shininess = 64;
};

typedef GenericComponentArray<DrawComponent> DrawComponentArrayType;


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