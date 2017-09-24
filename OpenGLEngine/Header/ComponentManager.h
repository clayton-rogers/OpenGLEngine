#pragma once

#include "ComponentArray.h"

#include <memory>
#include <unordered_map>
#include <limits>
#include <bitset>


enum ComponentEnum : int;

typedef std::bitset<16> ComponentBitset; // TODO Max number of components should be configurable.
const ComponentBitset NONE_COMPONENT;

static unsigned int getNextUID() {
	static unsigned int nextUID = 0;
	if (std::numeric_limits<decltype(nextUID)>::max() == nextUID) {
		throw "Ran out of UIDs";
	}
	return nextUID++;
}

class ComponentManager {

private:
	std::unordered_map<ComponentEnum, std::unique_ptr<ComponentArray>> mComponents;
	std::unordered_map<unsigned int, ComponentBitset> mEntityComponentMap;

public:

	void addComponent(ComponentEnum type, std::unique_ptr<ComponentArray> component) {
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
