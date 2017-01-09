#pragma once

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
		for (auto object1 = entityMap.begin(); object1 != entityMap.end(); ++object1) {
			if ((object1->second & mRequiredComponents) != mRequiredComponents) { continue; }

			PositionComponent& thisP = positionComponentArray->getComponent(object1->first);
			InertialComponent& thisI = inertialComponentArray->getComponent(object1->first);

			for (auto object2 = object1; object2 != entityMap.end(); ++object2) {
				if (object2 == object1) { 
					++object2; 
					if (object2 == entityMap.end()) { break; }
				}
				if ((object2->second & mRequiredComponents) != mRequiredComponents) { continue; }

				// Apply the gravity force to both bodies
				PositionComponent& otherP = positionComponentArray->getComponent(object2->first);
				InertialComponent& otherI = inertialComponentArray->getComponent(object2->first);

				// Apply a gravity force to first entity
				thisI.frameForce += calculateGravity(
					thisI.mass,
					otherI.mass,
					thisP.position,
					otherP.position
				);

				// Apply a gravity force to second entity
				otherI.frameForce += calculateGravity(
					otherI.mass,
					thisI.mass,
					otherP.position,
					thisP.position
				);

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