#pragma once

#include <glm/glm.hpp>

#include "System.h"
#include "ComponentType.h"
#include "ComponentArray.h"
#include "Components.h"
#include "Entities.h"

#include <iostream>

class DrawSystem : public System {

	virtual void internalRunEntity(unsigned int UID) override {
		DrawComponent& d = getComponent<DrawComponent>(DRAW, UID);
		PositionComponent& p = getComponent<PositionComponent>(POSITION, UID);
		CoalescableComponent& c = getComponent<CoalescableComponent>(COALESCABLE, UID);

		d.shader->Use();

		glm::mat4 model;
		model = glm::translate(model, p.position);
		model = glm::scale(model, glm::vec3(c.radius));

		d.mesh->Draw(d.shader->Program, model, d.colour, d.shininess);
	}

public:
	DrawSystem() {
		mRequiredComponents[DRAW]     = true;
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[COALESCABLE] = true;
	}
	
};

class GeneralDraw : public System {
	virtual void internalRunEntity(unsigned int UID) override {
		GeneralDrawComponent& d = getComponent<GeneralDrawComponent>(GENERAL_DRAW, UID);
		PositionComponent&    p = getComponent<PositionComponent>(POSITION, UID);

		d.shader->Use();

		glm::mat4 model;
		model = glm::translate(model, p.position);
		model *= d.rotationScaleMatrix;

		d.mesh->Draw(d.shader->Program, model, d.colour, d.shininess);
	}
public:
	GeneralDraw() {
		mRequiredComponents[GENERAL_DRAW] = true;
		mRequiredComponents[POSITION] = true;
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
		MassComponentArrayType* massComponentArray = dynamic_cast<MassComponentArrayType*>(componentManager.getComponentArray(MASS));

		std::vector<PositionComponent*> positionArray;
		std::vector<MassComponent*> massArray;
		// Load up local array once so that we only have to call getComponent once
		for (auto UIDpair = entityMap.begin(); UIDpair != entityMap.end(); ++UIDpair) {
			if ((UIDpair->second & mRequiredComponents) == mRequiredComponents) {
				positionArray.push_back(&positionComponentArray->getComponent(UIDpair->first));
				massArray.push_back(&massComponentArray->getComponent(UIDpair->first));
			}
		}
		
		const int size = positionArray.size();
		for (int i = 0; i < size; ++i) {
			for (int j = i + 1; j < size; ++j) {
				
				
				glm::vec3 force = calculateGravity(
					massArray[i]->mass,
					massArray[j]->mass,
					positionArray[i]->position,
					positionArray[j]->position
				);

				massArray[i]->frameForce += force;
				massArray[j]->frameForce -= force; // Apply gravity in the oposite direction for second object
			}
		}
	}

	GravitySystem() {
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[MASS] = true;
	}
};

class CoalesceSystem : public System {
public:
	virtual void runSystem() override {
		bool foundCollision = false;
		do {
			foundCollision = false;

			auto& entityMap = componentManager.getEntities();
			PositionComponentArrayType* positionComponentArray = dynamic_cast<PositionComponentArrayType*>(componentManager.getComponentArray(POSITION));
			CoalescableComponentArrayType* radiusComponentArray = dynamic_cast<CoalescableComponentArrayType*>(componentManager.getComponentArray(COALESCABLE));

			std::vector<PositionComponent*> positionArray;
			std::vector<CoalescableComponent*> coalesceArray;
			std::vector<unsigned int> uidArray;
			// Load up local array once so that we only have to call getComponent once
			for (auto UIDpair = entityMap.begin(); UIDpair != entityMap.end(); ++UIDpair) {
				if ((UIDpair->second & mRequiredComponents) == mRequiredComponents) {
					positionArray.push_back(&positionComponentArray->getComponent(UIDpair->first));
					coalesceArray.push_back(&radiusComponentArray->getComponent(UIDpair->first));
					uidArray.push_back(UIDpair->first);
				}
			}

			const int size = positionArray.size();
			for (int i = 0; i < size; ++i) {
				for (int j = i + 1; j < size; ++j) {

					if (
						glm::distance(positionArray[i]->position, positionArray[j]->position) <
						(coalesceArray[i]->radius + coalesceArray[j]->radius)) 
					{
						Entities::createPlanet(
							positionArray[i], positionArray[j],
							&getComponent<VelocityComponent>(VELOCITY, uidArray[i]), &getComponent<VelocityComponent>(VELOCITY, uidArray[j]),
							&getComponent<MassComponent>(MASS, uidArray[i]), &getComponent<MassComponent>(MASS, uidArray[j]),
							&getComponent<DrawComponent>(DRAW, uidArray[i]), &getComponent<DrawComponent>(DRAW, uidArray[j]));

						componentManager.removeEntity(uidArray[i]);
						componentManager.removeEntity(uidArray[j]);

						foundCollision = true;
						goto finishLoop;
					}

				}
			}

		finishLoop:
			;
		} while (foundCollision);
	}

	CoalesceSystem() {
		mRequiredComponents[DRAW] = true;
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[VELOCITY] = true;
		mRequiredComponents[COALESCABLE] = true;
		mRequiredComponents[MASS] = true;
	}
};

class MassSystem : public System {

	virtual void internalRunEntity(unsigned int UID) override {
		VelocityComponent& i = getComponent<VelocityComponent>(VELOCITY, UID);
		MassComponent&     m = getComponent<MassComponent>(MASS, UID);

		glm::vec3 acceleration = m.frameForce / m.mass;
		i.velocity += acceleration * float(DELTA_T);

		// reset the force to zero
		m.frameForce = glm::vec3();
	}

public:
	MassSystem() {
		mRequiredComponents[VELOCITY] = true;
		mRequiredComponents[MASS] = true;
	}
};

class VelocitySystem : public System {
	virtual void internalRunEntity(unsigned int UID) override {
		PositionComponent& p = getComponent<PositionComponent>(POSITION, UID);
		VelocityComponent& v = getComponent<VelocityComponent>(VELOCITY, UID);

		p.position += v.velocity * float(DELTA_T);
	}

public:
	VelocitySystem() {
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[VELOCITY] = true;
	}
};

class BulletCollisionSystem : public System {
	ComponentBitset mTargetComponents;

	virtual void runSystem() override {

		std::vector<PositionComponent*> bulletPositionArray;
		std::vector<BulletComponent*> bulletRadiusArray;
		std::vector<unsigned int> bulletUidArray;

		std::vector<PositionComponent*> targetPositionArray;
		std::vector<CoalescableComponent*> targetRadiusArray;
		std::vector<unsigned int> targetUidArray;

		// Cache the bullets and targets
		for (auto UIDpair = componentManager.getEntities().begin(); UIDpair != componentManager.getEntities().end(); ++UIDpair) {
			if ((UIDpair->second & mRequiredComponents) == mRequiredComponents) {
				bulletPositionArray.push_back(&getComponent<PositionComponent>(POSITION, UIDpair->first));
				bulletRadiusArray.push_back(&getComponent<BulletComponent>(BULLET, UIDpair->first));
				bulletUidArray.push_back(UIDpair->first);
			}
			if ((UIDpair->second & mTargetComponents) == mTargetComponents) {
				targetPositionArray.push_back(&getComponent<PositionComponent>(POSITION, UIDpair->first));
				targetRadiusArray.push_back(&getComponent<CoalescableComponent>(COALESCABLE, UIDpair->first));
				targetUidArray.push_back(UIDpair->first);
			}
		}

		const int bulletSize = bulletPositionArray.size();
		const int targetSize = targetPositionArray.size();
		for (int bulletIndex = 0; bulletIndex < bulletSize; ++bulletIndex) {
			for (int targetIndex = 0; targetIndex < targetSize; ++targetIndex) {
				if (glm::distance(bulletPositionArray[bulletIndex]->position, targetPositionArray[targetIndex]->position) <
					(bulletRadiusArray[bulletIndex]->radius + targetRadiusArray[targetIndex]->radius)) 
				{
					// Delete the bullet
					componentManager.removeEntity(bulletUidArray[bulletIndex]);

					// Split the target
					Entities::splitPlanet(targetUidArray[targetIndex]);
				}
			}
		}
	}
public:
	BulletCollisionSystem() {
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[BULLET] = true;

		mTargetComponents[POSITION] = true;
		mTargetComponents[COALESCABLE] = true;
	}
};