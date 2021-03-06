#pragma once

#include "ComponentArray.h"

#include <memory>
#include <unordered_map>
#include <limits>
#include <bitset>
#include <exception>
#include <iostream>


typedef std::bitset<32> ComponentBitset; // TODO Max number of components should be configurable.
const ComponentBitset NONE_COMPONENT;

static unsigned int getNextUID() {
	static unsigned int nextUID = 0;
	if (std::numeric_limits<decltype(nextUID)>::max() == nextUID) {
		throw new std::overflow_error("Ran out of UIDs");
	}
	return nextUID++;
}

class ComponentManager {

private:
	std::unordered_map<ComponentEnum, std::unique_ptr<ComponentArray>> mComponents;
	std::unordered_map<unsigned int, ComponentBitset> mEntityComponentMap;

public:

	void addComponent(ComponentEnum type, std::unique_ptr<ComponentArray> component) {
		if (type != component->getType()) {
			std::cout << "Component types do not match: Type: " << type << " componentType: " << component->getType() << std::endl;
			throw new std::logic_error("Component types do not match");
		}
		mComponents[type] = std::move(component);
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

		mEntityComponentMap.erase(UID);

		// remove the actual components too
		for (auto& componentArrayPair : mComponents) {
			componentArrayPair.second->removeComponentFromEntity(UID);
		}
	}

	void addComponentToEntity(ComponentEnum type, unsigned int UID) {
		mEntityComponentMap.at(UID)[type] = true;
		mComponents.at(type)->addComponentToEntity(UID);
	}

	std::unordered_map<unsigned int, ComponentBitset>& getEntities() {
		return mEntityComponentMap;
	}

};
