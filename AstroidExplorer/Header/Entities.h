#pragma once

#include <glm/glm.hpp>

#include "ComponentManager.h"
#include "Components.h"

#include <random>
#include <cmath>

namespace Entities {

	Mesh planetMesh;
	Mesh laserMesh;
	Shader* generalShader;

	void init(Shader* generalShader_) {
		planetMesh.load("./Resources/Planet.obj");
		laserMesh.load("./Resources/Lazer.obj");
		generalShader = generalShader_;
	}

	unsigned int createPlanet() {
		unsigned int UID = componentManager.addEntity();
		componentManager.addComponentToEntity(DRAW, UID);
		componentManager.addComponentToEntity(POSITION, UID);
		componentManager.addComponentToEntity(VELOCITY, UID);
		componentManager.addComponentToEntity(MASS, UID);
		componentManager.addComponentToEntity(COALESCABLE, UID);

		PositionComponent&    p = getComponent<PositionComponent>(POSITION, UID);
		VelocityComponent&    i = getComponent<VelocityComponent>(VELOCITY, UID);
		MassComponent&        m = getComponent<MassComponent>(MASS, UID);
		CoalescableComponent& r = getComponent<CoalescableComponent>(COALESCABLE, UID);
		DrawComponent&        d = getComponent<DrawComponent>(DRAW, UID);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<GLfloat> posGen(-30.0f, 30.0f);
		p.position.x = posGen(gen);
		p.position.y = posGen(gen);
		p.position.z = posGen(gen);
		std::uniform_real_distribution<GLfloat> velGen(-5.0f, 5.0f);
		i.velocity.x = velGen(gen);
		i.velocity.y = velGen(gen);
		i.velocity.z = velGen(gen);
		std::uniform_real_distribution<GLfloat> colourGen(0.0f, 1.0f);
		d.colour.r = colourGen(gen);
		d.colour.g = colourGen(gen);
		d.colour.b = colourGen(gen);
		std::uniform_real_distribution<GLfloat> massGen(500.0f, 50000.0f);
		{
			m.mass = massGen(gen);
			float volume = m.mass / 5540; // density of earth
			r.radius = std::cbrt(volume * (3.0f / 4.0f) / 3.1415927f);
		}

		d.mesh = &planetMesh;
		d.shader = generalShader;

		return UID;
	}

	unsigned int createPlanet(
		const PositionComponent* p1, const PositionComponent* p2,
		const VelocityComponent* v1, const VelocityComponent* v2,
		const MassComponent*     m1, const MassComponent*     m2,
		const DrawComponent*     d1, const DrawComponent*     d2)
	{
		unsigned int UID = componentManager.addEntity();
		componentManager.addComponentToEntity(DRAW, UID);
		componentManager.addComponentToEntity(POSITION, UID);
		componentManager.addComponentToEntity(VELOCITY, UID);
		componentManager.addComponentToEntity(MASS, UID);
		componentManager.addComponentToEntity(COALESCABLE, UID);

		PositionComponent&    p = getComponent<PositionComponent>(POSITION, UID);
		VelocityComponent&    i = getComponent<VelocityComponent>(VELOCITY, UID);
		MassComponent&        m = getComponent<MassComponent>(MASS, UID);
		CoalescableComponent& r = getComponent<CoalescableComponent>(COALESCABLE, UID);
		DrawComponent&        d = getComponent<DrawComponent>(DRAW, UID);

		m.mass = m1->mass + m2->mass;
		p.position = (m1->mass * p1->position + m2->mass * p2->position) / (m1->mass + m2->mass);
		i.velocity = (m1->mass * v1->velocity + m2->mass * v2->velocity) / (m1->mass + m2->mass);
		d.colour   = (m1->mass * d1->colour   + m2->mass * d2->colour  ) / (m1->mass + m2->mass);
		d.shininess = (d1->shininess + d2->shininess) / 2.0f;

		float volume = m.mass / 5540; // density of earth
		r.radius = std::cbrt(volume * (3.0f / 4.0f) / 3.1415927f);

		d.mesh = &planetMesh;
		d.shader = generalShader;

		return UID;
	}



	unsigned int createLaser(glm::vec3 position, glm::vec3 direction) {
		const float LASER_VELOCITY = 30.0f;

		unsigned int UID = componentManager.addEntity();
		componentManager.addComponentToEntity(GENERAL_DRAW, UID);
		componentManager.addComponentToEntity(POSITION, UID);
		componentManager.addComponentToEntity(VELOCITY, UID);
		componentManager.addComponentToEntity(BULLET, UID);

		PositionComponent&    p = getComponent<PositionComponent>(POSITION, UID);
		VelocityComponent&    i = getComponent<VelocityComponent>(VELOCITY, UID);
		BulletComponent&      b = getComponent<BulletComponent>(BULLET, UID);
		GeneralDrawComponent& d = getComponent<GeneralDrawComponent>(GENERAL_DRAW, UID);

		p.position = position;
		
		i.velocity = glm::normalize(direction) * LASER_VELOCITY;

		b.radius = 1.0f;


		glm::mat4 model;
		{
			glm::vec3 modelDirection = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec3 rotationVector = glm::cross(modelDirection, glm::normalize(direction));
			float rotationAngle = glm::angle(modelDirection, glm::normalize(direction));

			model = glm::rotate(model, rotationAngle, rotationVector);
			model = glm::scale(model, glm::vec3(0.1f, 0.5f, 0.1f));
		}

		d.colour = glm::vec3(1.0f, 0.0f, 0.0f);
		d.mesh = &laserMesh;
		d.rotationScaleMatrix = model;
		d.shader = generalShader;
		d.shininess = 128.0f;
	}

}