#pragma once

#include "GenericActionList.h"
#include "Lazer.h"

#include <vector>

class LazerManager : public Action<GLuint>, public Action<double> {

	std::vector<Lazer> mLasers;
	std::vector<double> mExpiryTimes;


	static const double LAZER_DURATION;
	static double currentTime;

public:
	int drawListIndex;
	int physicsListIndex;

	void add(glm::vec3 position, glm::vec3 direction) {
		Lazer laser(position, direction);
		mLasers.push_back(laser);
		mExpiryTimes.push_back(currentTime + LAZER_DURATION);
		// TODO lasers will always be added to be back and removed from front, change to queue
	}

	virtual void doAction(double deltaT) override {
		currentTime += deltaT;
		
		// Remove laser bolts that have exceeded their expiry
		for (int i = mLasers.size() - 1; i >= 0; i--) {
			if (mExpiryTimes[i] < currentTime) {
				mLasers.erase(mLasers.begin() + i);
				mExpiryTimes.erase(mExpiryTimes.begin() + i);
			}
		}

		// Move all lasers forward
		for (Lazer& L : mLasers) {
			L.step(deltaT);
		}
	}

	virtual void doAction(GLuint program) override {
		for (const Lazer& L : mLasers) {
			L.draw(program);
		}
	}
};

const double LazerManager::LAZER_DURATION = 3.0;
double LazerManager::currentTime = 0.0;