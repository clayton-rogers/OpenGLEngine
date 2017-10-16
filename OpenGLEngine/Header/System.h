#pragma once

#include "ComponentManager.h" // for component bitset

#include <memory>

class System {

	// do nothing by default
	virtual void internalRunEntity(unsigned int /*UID*/) {};
	virtual void preLoop() {};
	virtual void postLoop() {};

protected:
	ComponentBitset mRequiredComponents;

public:

	virtual void runSystem();

	virtual ~System() {}
};