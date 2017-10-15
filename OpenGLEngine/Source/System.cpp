#include "System.h"

#include "OpenGLEngine.h"

ComponentBitset System::mRequiredComponents;

void System::runSystem() {
	preLoop();
	for (auto& entity : OpenGLEngine::componentManager.getEntities()) {
		if ((entity.second & mRequiredComponents) == mRequiredComponents) {
			internalRunEntity(entity.first);
		}
	}
	postLoop();
}