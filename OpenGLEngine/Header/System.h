#pragma once

#include "ComponentType.h"
#include "ComponentManager.h"

#include <memory>

class System {

	// do nothing by default
	virtual void internalRunEntity(unsigned int UID) {};
	virtual void preLoop() {};
	virtual void postLoop() {};

protected:
	ComponentBitset mRequiredComponents;

public:

	virtual void runSystem() {
		preLoop();
		for (auto& entity : componentManager.getEntities()) {
			if ((entity.second & mRequiredComponents) == mRequiredComponents) {
				internalRunEntity(entity.first);
			}
		}
		postLoop();
	}

	virtual ~System() {

	}
};