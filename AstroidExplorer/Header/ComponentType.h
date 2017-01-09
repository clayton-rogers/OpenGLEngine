#pragma once

#include <bitset>

enum ComponentEnum {
	DRAW,
	POSITION,
	INERTIAL,
	RADIUS,
	ComponentTypeSize
};

typedef std::bitset<ComponentEnum::ComponentTypeSize> ComponentBitset;

const ComponentBitset NONE_COMPONENT;