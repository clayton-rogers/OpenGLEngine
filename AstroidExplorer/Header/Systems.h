#pragma once

// Include the library
#include "OpenGLEngine.h"

#include "Components.h"
#include "Entities.h"

#include "local_glm.h"

#include <iostream>
#include <vector>
#include <future>

using OpenGLEngine::getComponent;

class LaserCreator : public System {

	virtual void preLoop() override {
		OpenGLEngine::InputState i = OpenGLEngine::getInputState();

		if (i.leftMousePressed) {
			/*glm::vec3 startPosition = camera.Position;
			startPosition -= camera.Up * 0.5f;
			Entities::createLaser(startPosition, camera.Front);*/
			// TODO make this system actually use the camera

			Entities::createLaser(glm::vec3(0.0), glm::vec3(1.0, 0.0, 0.0));

		}
	}

};

class DrawSystem : public System {

	// TODO add input system
	// TODO add player that handles input
	// TODO add expiry system and component

	virtual void internalRunEntity(unsigned int UID) override {
		DrawComponent& d = getComponent<DrawComponent>(UID);
		PositionComponent& p = getComponent<PositionComponent>(UID);
		CoalescableComponent& c = getComponent<CoalescableComponent>(UID);

		d.shader->Use();

		glm::mat4 model;
		model = glm::translate(model, p.position);
		model = glm::scale(model, glm::vec3(c.radius));

		d.mesh->Draw(d.shader->Program, model, d.colour, d.shininess);
	}

public:
	DrawSystem() {
		mRequiredComponents[DRAW] = true;
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[COALESCABLE] = true;
	}

};

class GeneralDraw : public System {
	virtual void internalRunEntity(unsigned int UID) override {
		GeneralDrawComponent& d = getComponent<GeneralDrawComponent>(UID);
		PositionComponent&    p = getComponent<PositionComponent>(UID);

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
	static const float bigG;
	static const float gravityScaling;

	// Calculates the force of mass2 on mass1
	static glm::vec3 calculateGravity(float mass1, float mass2, glm::vec3 position1, glm::vec3 position2) {
		float distanceBetweenSquared = glm::distance2(position1, position2);

		float force = (bigG*gravityScaling) * mass1 * mass2 / distanceBetweenSquared;

		// direction from mass 1 to mass 2
		glm::vec3 direction = glm::normalize(position2 - position1);

		return force * direction;
	}

public:

	virtual void runSystem() {
		auto& entityMap = OpenGLEngine::componentManager.getEntities();
		PositionComponentArrayType* positionComponentArray = dynamic_cast<PositionComponentArrayType*>(OpenGLEngine::componentManager.getComponentArray(POSITION));
		MassComponentArrayType* massComponentArray = dynamic_cast<MassComponentArrayType*>(OpenGLEngine::componentManager.getComponentArray(MASS));

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
		auto fn = [&positionArray, &massArray, size](int lower, int upper) {
			for (int i = lower; i < upper; ++i) {
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
		};

		std::vector<std::future<void>> futures;

		// The following are found by entering the following in wolfram-alpha:
		//A = n_4^2/2, A = (n_3 + 2 n_4)/2 * n_3, A = (n_2 + 2 n_3 + 2 n_4)/2 * n_2, A = (n_1 + 2 n_2 + 2 n_3 + 2 n_4)/2 * n_1, A = 100^2/8
		//A = n_4 ^ 2 / 2, A = (n_3 + 2 n_4) / 2 * n_3, A = (n_2 + 2 n_3 + 2 n_4) / 2 * n_2, A = (n_1 + 2 n_2 + 2 n_3 + 2 n_4) / 2 * n_1, A = 1250
		// This is the approximate solution to slicing an isoceles right triangle into equal area slices

		const int n1 = static_cast<int>(13.4 * size / 100.0);
		const int n2 = static_cast<int>(15.9 * size / 100.0);
		const int n3 = static_cast<int>(20.7 * size / 100.0);
		const int n4 = static_cast<int>(50.0 * size / 100.0);

		const int first = n1;
		const int second = n1 + n2;
		const int third = n1 + n2 + n3;

		//const int first = size / 4;
		//const int second = 2 * first;
		//const int third = 3 * first;

		futures.emplace_back(std::async(std::launch::async, fn, 0, first));
		futures.emplace_back(std::async(std::launch::async, fn, first, second));
		futures.emplace_back(std::async(std::launch::async, fn, second, third));
		futures.emplace_back(std::async(std::launch::async, fn, third, size));

		for (auto& f : futures) {
			f.get(); // join() all
		}
	}

	GravitySystem() {
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[MASS] = true;
	}
};
const float GravitySystem::bigG = 6.6740831e-11f;
const float GravitySystem::gravityScaling = 1000000;

class CoalesceSystem : public System {
public:
	virtual void runSystem() override {
		using OpenGLEngine::componentManager;

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
						(coalesceArray[i]->radius + coalesceArray[j]->radius)) {
						Entities::createPlanet(
							positionArray[i], positionArray[j],
							&getComponent<VelocityComponent>(uidArray[i]), &getComponent<VelocityComponent>(uidArray[j]),
							&getComponent<MassComponent>(uidArray[i]), &getComponent<MassComponent>(uidArray[j]),
							&getComponent<DrawComponent>(uidArray[i]), &getComponent<DrawComponent>(uidArray[j]));

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
		VelocityComponent& i = getComponent<VelocityComponent>(UID);
		MassComponent&     m = getComponent<MassComponent>(UID);

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
		PositionComponent& p = getComponent<PositionComponent>(UID);
		VelocityComponent& v = getComponent<VelocityComponent>(UID);

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
		using OpenGLEngine::componentManager;

		std::vector<PositionComponent*> bulletPositionArray;
		std::vector<BulletComponent*> bulletRadiusArray;
		std::vector<unsigned int> bulletUidArray;

		std::vector<PositionComponent*> targetPositionArray;
		std::vector<CoalescableComponent*> targetRadiusArray;
		std::vector<unsigned int> targetUidArray;

		// Cache the bullets and targets
		for (auto UIDpair = componentManager.getEntities().begin(); UIDpair != componentManager.getEntities().end(); ++UIDpair) {
			if ((UIDpair->second & mRequiredComponents) == mRequiredComponents) {
				bulletPositionArray.push_back(&getComponent<PositionComponent>(UIDpair->first));
				bulletRadiusArray.push_back(&getComponent<BulletComponent>(UIDpair->first));
				bulletUidArray.push_back(UIDpair->first);
			}
			if ((UIDpair->second & mTargetComponents) == mTargetComponents) {
				targetPositionArray.push_back(&getComponent<PositionComponent>(UIDpair->first));
				targetRadiusArray.push_back(&getComponent<CoalescableComponent>(UIDpair->first));
				targetUidArray.push_back(UIDpair->first);
			}
		}

		const int bulletSize = bulletPositionArray.size();
		const int targetSize = targetPositionArray.size();
		for (int bulletIndex = 0; bulletIndex < bulletSize; ++bulletIndex) {
			for (int targetIndex = 0; targetIndex < targetSize; ++targetIndex) {
				if (glm::distance(bulletPositionArray[bulletIndex]->position, targetPositionArray[targetIndex]->position) <
					(bulletRadiusArray[bulletIndex]->radius + targetRadiusArray[targetIndex]->radius)) {
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