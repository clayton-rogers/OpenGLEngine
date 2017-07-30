#pragma once

#include "EntityManager.h"
#include "ComponentType.h"
#include "ComponentArray.h"

#include <memory>
#include <array>
#include <unordered_map>


class ComponentManager {

private:
	std::array<std::unique_ptr<ComponentArray>, ComponentEnum::ComponentTypeSize> mComponents;
	std::unordered_map<unsigned int, ComponentBitset> mEntityComponentMap;

public:

	void addComponent(ComponentEnum type, std::unique_ptr<ComponentArray> component) {
		mComponents.at(type) = std::move(component);
	}

	ComponentArray* getComponentArray(ComponentEnum type) {
		return mComponents.at(type).get();
	}

	ComponentBitset& getBitsetForEntity(unsigned int UID) {
		return mEntityComponentMap.at(UID);
	}

	unsigned int addEntity() {
		unsigned int UID = getNextUID();
		ComponentBitset b;
		mEntityComponentMap.insert_or_assign(UID, b);
		return UID;
	}

	void removeEntity(unsigned int UID) {
		// TODO remove the actual components too
		mEntityComponentMap.at(UID) = NONE_COMPONENT;
	}

	void addComponentToEntity(ComponentEnum type, unsigned int UID) {
		mEntityComponentMap.at(UID)[type] = true;
		mComponents.at(type).get()->addComponentToEntity(UID);
	}

	std::unordered_map<unsigned int, ComponentBitset>& getEntities() {
		return mEntityComponentMap;
	}

};

ComponentManager componentManager;
// TODO FUTURE ComponentManager& getEntityManager() { return componentManager; }