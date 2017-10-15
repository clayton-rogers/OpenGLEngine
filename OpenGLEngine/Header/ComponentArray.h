#pragma once

#include <unordered_map>
#include <vector>
#include <utility>

enum ComponentEnum : char;

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
	std::vector<ComponentType> mComponentElements;
public:
	ComponentType& getComponent(unsigned int UID) {
		return mComponentElements.at(UID);
	}

	virtual void addComponentToEntity(unsigned int UID) override {
		if (UID >= mComponentElements.size()) {
			// resize to fit the new element
			// components added are default-inserted
			mComponentElements.resize(UID + 1);
		}
	}
};
