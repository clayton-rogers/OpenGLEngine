#pragma once

#include "ComponentType.h"

#include <map>
#include <utility>

class ComponentArray {
public:
	virtual void addComponentToEntity(unsigned int UID) = 0;
	virtual ~ComponentArray() {};
};

template <
	typename ComponentType
>
class GenericComponentArray : public ComponentArray {
	std::map<unsigned int, ComponentType> mComponentElements;
public:
	ComponentType& getComponent(unsigned int UID) {
		return mComponentElements.at(UID);
	}

	virtual void addComponentToEntity(unsigned int UID) override {
		ComponentType c;
		mComponentElements.emplace(std::make_pair(UID, c));
	}
};

template <
	typename ComponentType
>
ComponentType& getComponent(ComponentEnum type, unsigned int UID) {
	return (dynamic_cast<GenericComponentArray<ComponentType>*>(componentManager.getComponentArray(type)))->getComponent(UID);
}
