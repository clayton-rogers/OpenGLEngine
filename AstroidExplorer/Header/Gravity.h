#pragma once

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Gravity {

	const float bigG = 6.6740831e-11;
	float gravityScaling = 5000000;

	// Calculates the force of mass2 on mass1
	glm::vec3 calculateGravity(float mass1, float mass2, glm::vec3 position1, glm::vec3 position2) {
		float distanceBetween = glm::distance(position1, position2);

		float force = (bigG*gravityScaling) * mass1 * mass2 / (distanceBetween * distanceBetween);

		// direction from mass 1 to mass 2
		glm::vec3 direction = glm::normalize(position2 - position1);

		return force * direction;
	}


}