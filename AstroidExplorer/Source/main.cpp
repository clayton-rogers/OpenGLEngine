#include "Systems.h"
#include "Components.h"
#include "Entities.h"

#include "Stock/FollowCameraSystem.h"
#include "Stock/CameraSystem.h"
#include "Stock/GeneralDrawSystem.h"
#include "Stock/InertialSystems.h"

#include "OpenGLEngine.h"

const bool FULLSCREEN = false;
const bool VSYNC_ON = true;
const bool PERFORMANCE_TEST = true;
const GLuint WINDOW_WIDTH = 1920;
const GLuint WINDOW_HEIGHT = 1080;

void runPerformance();

int main() {

	if (PERFORMANCE_TEST) {
		runPerformance();

		return 0;
	}

	OpenGLEngine::setupEnvironment(FULLSCREEN, WINDOW_WIDTH, WINDOW_HEIGHT);

	Entities::init();

	// Set up all systems
	OpenGLEngine::addSystem(std::unique_ptr<VelocitySystem>(new VelocitySystem(POSITION, VELOCITY)));
	OpenGLEngine::addSystem(std::unique_ptr<MassSystem>(new MassSystem(VELOCITY, MASS)));
	OpenGLEngine::addSystem(std::unique_ptr<GeneralDrawSystem>(new GeneralDrawSystem(GENERAL_DRAW, POSITION)));
	OpenGLEngine::addSystem(std::unique_ptr<FollowCameraSystem>(new FollowCameraSystem(CAMERA, POSITION)));
	//OpenGLEngine::addSystem(std::unique_ptr<CameraSystem>(new CameraSystem(CAMERA)));


	// Set up all components
	PositionComponent::type = POSITION;
	VelocityComponent::type = VELOCITY;
	MassComponent::type = MASS;
	GeneralDrawComponent::type = GENERAL_DRAW;
	CameraComponent::type = CAMERA;
	OpenGLEngine::addComponent(POSITION,     std::unique_ptr<PositionComponentArrayType>(new PositionComponentArrayType));
	OpenGLEngine::addComponent(VELOCITY,     std::unique_ptr<VelocityComponentArrayType>(new VelocityComponentArrayType));
	OpenGLEngine::addComponent(MASS,         std::unique_ptr<MassComponentArrayType>(new MassComponentArrayType));
	OpenGLEngine::addComponent(GENERAL_DRAW, std::unique_ptr<GeneralDrawComponentArrayType>(new GeneralDrawComponentArrayType));
	OpenGLEngine::addComponent(CAMERA,       std::unique_ptr<CameraComponentArrayType>(new CameraComponentArrayType));

	// Start the game
	Entities::createPlayer();
	Entities::createGround();

	OpenGLEngine::run(VSYNC_ON);

	return 0;
}

void runPerformance() {

	OpenGLEngine::setupEnvironment(FULLSCREEN, WINDOW_WIDTH, WINDOW_HEIGHT);
	Entities::init();


	// Set up all systems
	OpenGLEngine::addSystem(std::unique_ptr<DrawSystem>(new DrawSystem));
	OpenGLEngine::addSystem(std::unique_ptr<GeneralDrawSystem>(new GeneralDrawSystem(GENERAL_DRAW, POSITION)));
	OpenGLEngine::addSystem(std::unique_ptr<GravitySystem>(new GravitySystem));
	OpenGLEngine::addSystem(std::unique_ptr<VelocitySystem>(new VelocitySystem(POSITION, VELOCITY)));
	OpenGLEngine::addSystem(std::unique_ptr<MassSystem>(new MassSystem(VELOCITY, MASS)));
	//OpenGLEngine::addSystem(std::unique_ptr<CoalesceSystem>(new CoalesceSystem));
	OpenGLEngine::addSystem(std::unique_ptr<BulletCollisionSystem>(new BulletCollisionSystem));
	OpenGLEngine::addSystem(std::unique_ptr<LaserCreator>(new LaserCreator));
	OpenGLEngine::addSystem(std::unique_ptr<CameraSystem>(new CameraSystem(CAMERA)));


	// Set up all components
	PositionComponent::type = POSITION;
	VelocityComponent::type = VELOCITY;
	MassComponent::type = MASS;
	GeneralDrawComponent::type = GENERAL_DRAW;
	CameraComponent::type = CAMERA;
	OpenGLEngine::addComponent(DRAW, std::unique_ptr<DrawComponentArrayType>(new DrawComponentArrayType));
	OpenGLEngine::addComponent(COALESCABLE, std::unique_ptr<CoalescableComponentArrayType>(new CoalescableComponentArrayType));
	OpenGLEngine::addComponent(BULLET, std::unique_ptr<BulletComponentArrayType>(new BulletComponentArrayType));
	OpenGLEngine::addComponent(POSITION, std::unique_ptr<PositionComponentArrayType>(new PositionComponentArrayType));
	OpenGLEngine::addComponent(VELOCITY, std::unique_ptr<VelocityComponentArrayType>(new VelocityComponentArrayType));
	OpenGLEngine::addComponent(MASS, std::unique_ptr<MassComponentArrayType>(new MassComponentArrayType));
	OpenGLEngine::addComponent(CAMERA, std::unique_ptr<CameraComponentArrayType>(new CameraComponentArrayType));
	OpenGLEngine::addComponent(GENERAL_DRAW, std::unique_ptr<GeneralDrawComponentArrayType>(new GeneralDrawComponentArrayType));


	for (int i = 0; i < 1000; i++) {
		Entities::createPlanet();
	}

	{ // Add the camera entity
		using OpenGLEngine::componentManager;

		unsigned int UID = componentManager.addEntity();
		componentManager.addComponentToEntity(CAMERA, UID);
	}

	OpenGLEngine::run(false);
}