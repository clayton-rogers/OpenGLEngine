#pragma once

#include "ComponentArray.h"

#include "local_glm.h"


struct AerodynamicComponent {
	static ComponentEnum type;

	float wingArea = 1.0f;
	float CLalpha = 0.1f; // CL per deg of AOA
	float CLalphaOffset = 0.1f;  // CL at zero AOA
};
typedef GenericComponentArray<AerodynamicComponent> AerodynamicComponentArrayType;