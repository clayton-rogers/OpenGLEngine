#pragma once

#include <glm/glm.hpp>

#include "System.h"
#include "ComponentType.h"
#include "ComponentArray.h"
#include "Components.h"

#include <iostream>

class DrawSystem : public System {

	virtual void internalRunEntity(unsigned int UID) override {
		DrawComponent& d = getComponent<DrawComponent>(DRAW, UID);
		PositionComponent& p = getComponent<PositionComponent>(POSITION, UID);
		RadiusComponent& r = getComponent<RadiusComponent>(RADIUS, UID);

		d.shader->Use();

		glm::mat4 model;
		model = glm::translate(model, p.position);
		model = glm::scale(model, glm::vec3(r.radius));

		d.mesh->Draw(d.shader->Program, model, d.colour, d.shininess);
	}

public:
	DrawSystem() {
		mRequiredComponents[DRAW]     = true;
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[RADIUS]   = true;
	}
	
};

const float DELTA_T = 1.0f / 60.0f; // frame time in seconds

class GravitySystem : public System {
	const float bigG = 6.6740831e-11f;
	const float gravityScaling = 1000000;

	// Calculates the force of mass2 on mass1
	glm::vec3 calculateGravity(float mass1, float mass2, glm::vec3 position1, glm::vec3 position2) const {
		float distanceBetween = glm::distance(position1, position2);

		float force = (bigG*gravityScaling) * mass1 * mass2 / (distanceBetween * distanceBetween);

		// direction from mass 1 to mass 2
		glm::vec3 direction = glm::normalize(position2 - position1);

		return force * direction;
	}

public:

	virtual void runSystem() {
		auto& entityMap = componentManager.getEntities();
		PositionComponentArrayType* positionComponentArray = dynamic_cast<PositionComponentArrayType*>(componentManager.getComponentArray(POSITION));
		InertialComponentArrayType* inertialComponentArray = dynamic_cast<InertialComponentArrayType*>(componentManager.getComponentArray(INERTIAL));

		std::vector<PositionComponent*> positionArray;
		std::vector<InertialComponent*> inertialArray;
		// Load up local array once so that we only have to call getComponent once
		for (auto UIDpair = entityMap.begin(); UIDpair != entityMap.end(); ++UIDpair) {
			if ((UIDpair->second & mRequiredComponents) == mRequiredComponents) {
				positionArray.push_back(&positionComponentArray->getComponent(UIDpair->first));
				inertialArray.push_back(&inertialComponentArray->getComponent(UIDpair->first));
			}
		}

		for (int i = 0; i < positionArray.size(); ++i) {
			for (int j = i + 1; j < positionArray.size(); ++j) {
				
				
				glm::vec3 force = calculateGravity(
					inertialArray[i]->mass,
					inertialArray[j]->mass,
					positionArray[i]->position,
					positionArray[j]->position
				);

				inertialArray[i]->frameForce += force;
				inertialArray[j]->frameForce -= force; // Apply gravity in the oposite direction for second object
			}
		}
	}

	GravitySystem() {
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[INERTIAL] = true;
	}
};

class InertialSystem : public System {

	virtual void internalRunEntity(unsigned int UID) override {
		PositionComponent& p = getComponent<PositionComponent>(POSITION, UID);
		InertialComponent& i = getComponent<InertialComponent>(INERTIAL, UID);	

		glm::vec3 acceleration = i.frameForce / i.mass;
		i.velocity += acceleration * float(DELTA_T);
		p.position += i.velocity * float(DELTA_T);

		// reset the force to zero
		i.frameForce = glm::vec3();
	}

public:
	InertialSystem() {
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[INERTIAL] = true;
	}
};