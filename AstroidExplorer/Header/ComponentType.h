#pragma once

#include <bitset>

enum ComponentEnum {
	DRAW,
	GENERAL_DRAW,
	POSITION,
	VELOCITY,
	MASS,
	COALESCABLE,
	BULLET,
	ComponentTypeSize
};

typedef std::bitset<ComponentEnum::ComponentTypeSize> ComponentBitset;

const ComponentBitset NONE_COMPONENT;