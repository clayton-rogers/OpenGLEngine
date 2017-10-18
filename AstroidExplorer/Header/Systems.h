#pragma once

#include "System.h"

#include "Components.h"

#include "local_glm.h"

class LaserCreator : public System {

	virtual void internalRunEntity(unsigned int UID) override;

public:
	LaserCreator() {
		mRequiredComponents[CAMERA] = true;
	}
};

class DrawSystem : public System {

	// Setup the camera after finding it
	virtual void preLoop() override;
	virtual void internalRunEntity(unsigned int UID) override;

public:
	DrawSystem() {
		mRequiredComponents[DRAW]     = true;
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[COALESCABLE] = true;
	}
	
};

class GravitySystem : public System {
	const float bigG = 6.6740831e-11f;
	const float gravityScaling = 1000000;

	// Calculates the force of mass2 on mass1
	glm::vec3 calculateGravity(float mass1, float mass2, glm::vec3 position1, glm::vec3 position2) const;

public:

	virtual void runSystem() override;

	GravitySystem() {
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[MASS] = true;
	}
};

class CoalesceSystem : public System {
public:
	virtual void runSystem() override;

	CoalesceSystem() {
		mRequiredComponents[DRAW] = true;
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[VELOCITY] = true;
		mRequiredComponents[COALESCABLE] = true;
		mRequiredComponents[MASS] = true;
	}
};

class BulletCollisionSystem : public System {
	ComponentBitset mTargetComponents;

	virtual void runSystem() override;

public:
	BulletCollisionSystem() {
		mRequiredComponents[POSITION] = true;
		mRequiredComponents[BULLET] = true;

		mTargetComponents[POSITION] = true;
		mTargetComponents[COALESCABLE] = true;
	}
};