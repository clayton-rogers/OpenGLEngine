#pragma once

#include "System.h"
#include "ComponentManager.h"

#include <vector>
#include <memory>

class SystemManager {
	std::vector<std::unique_ptr<System>> mSystems;

public:

	void addSystem(std::unique_ptr<System> system) {
		mSystems.push_back(std::move(system));
	}

	void run() {
		for (auto& systemPtr : mSystems) {
			System* system = systemPtr.get();
			system->runSystem();
		}
	}

};


SystemManager systemManager;