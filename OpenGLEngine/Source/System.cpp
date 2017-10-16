#include "System.h"

#include "OpenGLEngine.h"

void System::runSystem() {
	preLoop();
	for (auto& entity : OpenGLEngine::componentManager.getEntities()) {
		if ((entity.second & mRequiredComponents) == mRequiredComponents) {
			internalRunEntity(entity.first);
		}
	}
	postLoop();
}