#pragma once

#include "ComponentArray.h"

#include "local_glm.h"


struct AngularPositionComponent {
	static ComponentEnum type;

	glm::quat angularPosition;
};
typedef GenericComponentArray<AngularPositionComponent> AngularPositionComponentArrayType;


struct AngularVelocityComponent {
	static ComponentEnum type;

	glm::vec3 angularVelocity = glm::vec3(0.0f);
};
typedef GenericComponentArray<AngularVelocityComponent> AngularVelocityComponentArrayType;


struct MomentofInertiaComponent {
	static ComponentEnum type;

	glm::vec3 frameTorque = glm::vec3(0.0f);
	glm::mat3 momentofInertia = glm::mat3(1.0f);
};
typedef GenericComponentArray<MomentofInertiaComponent> MomentofInertiaComponentArrayType;
