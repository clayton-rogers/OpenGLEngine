#pragma once

#include "OpenGLEngine.h"

#include <unordered_map>
#include <utility>

class ComponentArray {
public:
	virtual void addComponentToEntity(unsigned int UID) = 0;
	virtual ~ComponentArray() {};
};

struct UIDhash {
	std::size_t operator()(unsigned int UID) const {
		return UID;
	}
};

template <
	typename ComponentType
>
class GenericComponentArray : public ComponentArray {
	std::unordered_map<unsigned int, ComponentType, UIDhash> mComponentElements;
public:
	ComponentType& getComponent(unsigned int UID) {
		return mComponentElements.at(UID);
	}

	virtual void addComponentToEntity(unsigned int UID) override {
		ComponentType c;
		mComponentElements.emplace(std::make_pair(UID, c));
		// TODO FUTURE leak: what about when an entity is removed?
	}
};

template <
	typename ComponentType
>
ComponentType& getComponent(unsigned int UID) {
	return (dynamic_cast<GenericComponentArray<ComponentType>*>(OpenGLEngine::getComponentManager()->getComponentArray(ComponentType::type)))->getComponent(UID);
}
