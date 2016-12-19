#pragma once

#include "Lazer.h"
#include "Drawlist.h"
#include "Drawable.h"

#include <vector>

class LazerManager : public Drawable {

	std::vector<Lazer> mLasers;
	std::vector<double> mExpiryTimes;
	int mDrawListIndex = -1;


	static const double LAZER_DURATION;
	static double currentTime;

public:

	LazerManager()
	{
		mDrawListIndex =Drawlist::add(this);
	}

	~LazerManager()
	{
		Drawlist::remove(mDrawListIndex);
	}

	void add(glm::vec3 position, glm::vec3 direction) {
		Lazer laser(position, direction);
		mLasers.push_back(laser);
		mExpiryTimes.push_back(currentTime + LAZER_DURATION);
		// TODO lasers will always be added to be back and removed from front, change to queue
	}

	void step(double deltaT) {
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

	virtual void Draw(GLuint program) const override {
		for (const Lazer& L : mLasers) {
			L.Draw(program);
		}
	}
};

const double LazerManager::LAZER_DURATION = 3.0;
double LazerManager::currentTime = 0.0;