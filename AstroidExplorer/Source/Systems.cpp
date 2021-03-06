#include "Systems.h"

// Include the library
#include "OpenGLEngine.h"

#include "Components.h"
#include "Entities.h"

#include "Stock/CameraSystem.h"

#include <iostream>

using OpenGLEngine::getComponent;

// ***** LaserCreator *****//
void LaserCreator::internalRunEntity(unsigned int UID) {
	CameraComponent& c = OpenGLEngine::getComponent<CameraComponent>(UID);

	OpenGLEngine::InputState i = OpenGLEngine::getInputState();

	if (i.leftMousePressed) {
		glm::vec3 startPosition = c.position;
		startPosition -= c.up * 0.5f;
		Entities::createLaser(startPosition, c.front);
	}
}



// ***** DrawSystem *****//
// Setup the camera after finding it
void DrawSystem::preLoop() {
	ComponentBitset cameraComponentBitset;
	cameraComponentBitset[CAMERA] = true;
	unsigned int cameraUID;

	for (auto& entity : OpenGLEngine::componentManager.getEntities()) {
		if ((entity.second & cameraComponentBitset) == cameraComponentBitset) {
			cameraUID = entity.first;
			break;
		}
	}

	CameraComponent& c = getComponent<CameraComponent>(cameraUID);
	Shader& shader = OpenGLEngine::getShader();

	shader.Use();
	glm::mat4 view = glm::lookAt(c.position, c.position + c.front, c.up);
	glm::mat4 projection = glm::perspective(glm::radians(c.Zoom), OpenGLEngine::getAspectRatio(), 0.1f, 300.0f);


	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(glGetUniformLocation(shader.Program, "eyePosition"), c.position.x, c.position.y, c.position.z);
}

void DrawSystem::internalRunEntity(unsigned int UID) {
	DrawComponent& d = getComponent<DrawComponent>(UID);
	PositionComponent& p = getComponent<PositionComponent>(UID);
	CoalescableComponent& c = getComponent<CoalescableComponent>(UID);

	glm::mat4 model;
	model = glm::translate(model, p.position);
	model = glm::scale(model, glm::vec3(c.radius));

	d.mesh->Draw(OpenGLEngine::getShader().Program, model, d.colour, d.shininess);
}



// ***** GravitySystem *****//
// Calculates the force of mass2 on mass1
glm::vec3 GravitySystem::calculateGravity(float mass1, float mass2, glm::vec3 position1, glm::vec3 position2) const {
	float distanceBetweenSquared = glm::distance2(position1, position2);

	float force = (bigG*gravityScaling) * mass1 * mass2 / distanceBetweenSquared;

	// direction from mass 1 to mass 2
	glm::vec3 direction = glm::normalize(position2 - position1);

	return force * direction;
}

void GravitySystem::runSystem() {
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



// ***** CoalesceSystem *****//
void CoalesceSystem::runSystem() {
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




// ***** BulletCollisionSystem *****//
void BulletCollisionSystem::runSystem() {
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
