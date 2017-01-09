#pragma once

#include "ComponentManager.h"
#include "Components.h"

#include <random>
#include <cmath>

namespace Entities {

	Mesh planetMesh;

	void init() {
		planetMesh.load("./Resources/Planet.obj");
	}

	unsigned int createPlanet(Shader* planetShader) {
		unsigned int UID = componentManager.addEntity();
		componentManager.addComponentToEntity(DRAW, UID);
		componentManager.addComponentToEntity(POSITION, UID);
		componentManager.addComponentToEntity(INERTIAL, UID);
		componentManager.addComponentToEntity(RADIUS, UID);

		PositionComponent& p = getComponent<PositionComponent>(POSITION, UID);
		InertialComponent& i = getComponent<InertialComponent>(INERTIAL, UID);
		RadiusComponent&   r = getComponent<RadiusComponent>(RADIUS, UID);
		DrawComponent&     d = getComponent<DrawComponent>(DRAW, UID);


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
			i.mass = massGen(gen);
			float volume = i.mass / 5540; // density of earth
			r.radius = std::cbrt(volume * (3.0f / 4.0f) / 3.1415927f);
		}

		d.mesh = &planetMesh;
		d.shader = planetShader;

		return UID;
	}

}