#pragma once

#include <limits>

static unsigned int getNextUID() {
	static unsigned int nextUID = 0;
	if (std::numeric_limits<decltype(nextUID)>::max() == nextUID) {
		throw "Ran out of UIDs";
	}
	return nextUID++;
}