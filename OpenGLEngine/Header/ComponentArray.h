#pragma once

#include <unordered_map>
#include <utility>

enum ComponentEnum : char;

class ComponentArray {
public:
	virtual void addComponentToEntity(unsigned int UID) = 0;
	virtual void removeComponentFromEntity(unsigned int UID) = 0;
	virtual ~ComponentArray() {};
	virtual ComponentEnum getType() = 0;
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
	std::unordered_map<unsigned int, ComponentType> mComponentElements;
public:
	ComponentType& getComponent(unsigned int UID) {
		return mComponentElements.at(UID);
	}

	virtual void addComponentToEntity(unsigned int UID) override {
		mComponentElements.emplace(UID, ComponentType());
	}

	virtual void removeComponentFromEntity(unsigned int UID) override {
		mComponentElements.erase(UID);
	}

	virtual ComponentEnum getType() {
		return ComponentType::type;
	}
};
