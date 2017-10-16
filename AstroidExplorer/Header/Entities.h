#pragma once

#include "Components.h"

#include "Stock/InertialComponents.h"

#include "local_glm.h"

namespace Entities {

	void init();

	unsigned int createPlanet();
	unsigned int createPlanet(
		const PositionComponent* p1, const PositionComponent* p2,
		const VelocityComponent* v1, const VelocityComponent* v2,
		const MassComponent*     m1, const MassComponent*     m2,
		const DrawComponent*     d1, const DrawComponent*     d2);
	void splitPlanet(unsigned int sourceUID);

	unsigned int createLaser(glm::vec3 position, glm::vec3 direction);

	unsigned int createPlayer();

	unsigned int createGround();

}
