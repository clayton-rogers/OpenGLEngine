#pragma once

#include "Planet.h"
#include "Drawable.h"
#include "Gravity.h"
#include "Drawlist.h"

#include <vector>
#include <algorithm>

class PlanetManager : public Drawable {

	std::vector<Planet> planets;
	int drawListIndex;

public:
	PlanetManager(int numPlanets = 20) {
		for (int i = 0; i < numPlanets; i++) {
			planets.emplace_back();
		}

		drawListIndex = Drawlist::add(this);
	}

	~PlanetManager() {
		Drawlist::remove(drawListIndex);
	}

	void step(double deltaT) {
		int planetsSize = planets.size();
		//std::cout << "num planets: " << planetsSize << std::endl;

		bool foundCollision = false;
		do {
			foundCollision = false;

			for (unsigned int i = 0; i < planets.size(); i++) {
				Planet& p1 = planets[i];
				for (unsigned int j = i + 1; j < planets.size(); j++) {
					Planet& p2 = planets[j];
					if (glm::distance(p1.mInertialCore.mPosition, p2.mInertialCore.mPosition) < (p1.mRadius + p2.mRadius)) {
						foundCollision = true;
						planets.push_back(Planet(p1, p2));
						// must remove the greater one first because it will shift they indexes
						planets.erase(planets.begin() + std::max(i, j)); 
						planets.erase(planets.begin() + std::min(i, j));
						goto finishLoop;
					}
				}
			}
		finishLoop:
			;
		} while (foundCollision);


		for (unsigned int i = 0; i < planets.size(); i++) {
			Planet& p1 = planets[i];
			for (unsigned int j = i + 1; j < planets.size(); j++) {
				Planet& p2 = planets[j];

				// gravity of p2 on p1
				p1.mInertialCore.addForce(
					Gravity::calculateGravity(
						p1.mInertialCore.mMass,
						p2.mInertialCore.mMass,
						p1.mInertialCore.mPosition,
						p2.mInertialCore.mPosition)
				);

				// gravity of p1 on p2
				p2.mInertialCore.addForce(
					Gravity::calculateGravity(
						p2.mInertialCore.mMass,
						p1.mInertialCore.mMass,
						p2.mInertialCore.mPosition,
						p1.mInertialCore.mPosition)
				);
			}
		}

		for (Planet& p : planets) {
			p.mInertialCore.step(deltaT);
		}
	}




	// Inherited via Drawable
	virtual void Draw(GLuint program) const override {
		for (const Planet& p : planets) {
			p.Draw(program);
		}
	}

};