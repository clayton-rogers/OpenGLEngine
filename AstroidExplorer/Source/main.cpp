#include "OpenGLEngine.h"
#include "Systems.h"
#include "Components.h"
#include "Entities.h"


const bool FULLSCREEN = true;
const GLuint WINDOW_WIDTH = 1000;
const GLuint WINDOW_HEIGHT = 800;

int main() {

	OpenGLEngine::setupEnvironment(FULLSCREEN, WINDOW_WIDTH, WINDOW_HEIGHT);
	Entities::init(&OpenGLEngine::getShader());
	

	// Set up all systems
	OpenGLEngine::addSystem(std::unique_ptr<DrawSystem>(new DrawSystem));
	OpenGLEngine::addSystem(std::unique_ptr<GeneralDraw>(new GeneralDraw));
	OpenGLEngine::addSystem(std::unique_ptr<GravitySystem>(new GravitySystem));
	OpenGLEngine::addSystem(std::unique_ptr<VelocitySystem>(new VelocitySystem));
	OpenGLEngine::addSystem(std::unique_ptr<MassSystem>(new MassSystem));
	//OpenGLEngine::addSystem(std::unique_ptr<CoalesceSystem>(new CoalesceSystem));
	OpenGLEngine::addSystem(std::unique_ptr<BulletCollisionSystem>(new BulletCollisionSystem));


	// Set up all components
	OpenGLEngine::addComponent(DRAW,         std::unique_ptr<DrawComponentArrayType>(new DrawComponentArrayType));
	OpenGLEngine::addComponent(GENERAL_DRAW, std::unique_ptr<GeneralDrawComponentArrayType>(new GeneralDrawComponentArrayType));
	OpenGLEngine::addComponent(POSITION,     std::unique_ptr<PositionComponentArrayType>(new PositionComponentArrayType));
	OpenGLEngine::addComponent(VELOCITY,     std::unique_ptr<VelocityComponentArrayType>(new VelocityComponentArrayType));
	OpenGLEngine::addComponent(MASS,         std::unique_ptr<MassComponentArrayType>(new MassComponentArrayType));
	OpenGLEngine::addComponent(COALESCABLE,  std::unique_ptr<CoalescableComponentArrayType>(new CoalescableComponentArrayType));
	OpenGLEngine::addComponent(BULLET,       std::unique_ptr<BulletComponentArrayType>(new BulletComponentArrayType));

	
	for (int i = 0; i < 1000; i++) {
		Entities::createPlanet();
	}

	OpenGLEngine::run();

	
	return 0;
}