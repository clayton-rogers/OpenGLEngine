#pragma once

#include "ComponentType.h"
#include "ComponentManager.h"

#include <memory>

class System {

	// do nothing by default
	virtual void internalRunEntity(unsigned int UID) {};

protected:
	ComponentBitset mRequiredComponents;

public:

	virtual void runSystem() {
		for (auto& entity : componentManager.getEntities()) {

			if ((entity.second & mRequiredComponents) == mRequiredComponents) {
				internalRunEntity(entity.first);
			}
		}
	}

	virtual ~System() {

	}
};