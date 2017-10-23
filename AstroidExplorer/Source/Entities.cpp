#include "Entities.h"

#include "Stock/GeneralDrawComponent.h"
#include "Stock/CameraComponent.h"
#include "Stock/RotationalComponents.h"

#include "Mesh.h"
#include "Shader.h"
#include "OpenGLEngine.h"

#include <random>
#include <cmath>


namespace Entities {

	using OpenGLEngine::getComponent;
	using OpenGLEngine::componentManager;

	Mesh planetMesh;
	Mesh laserMesh;
	Mesh planeMesh;
	Mesh groundMesh;

	void init() {
		planetMesh.load("./Resources/Planet.obj");
		laserMesh.load("./Resources/Lazer.obj");
		planeMesh.load("./Resources/plane.obj");
		groundMesh.load("./Resources/ground.obj");
	}

	unsigned int createGround() {
		unsigned int UID = componentManager.addEntity();

		componentManager.addComponentToEntity(POSITION, UID);
		{
			PositionComponent& p = getComponent<PositionComponent>(UID);
			// default initialized
		}

		componentManager.addComponentToEntity(GENERAL_DRAW, UID);
		{
			GeneralDrawComponent& g = getComponent<GeneralDrawComponent>(UID);
			g.colour = glm::vec3(0.3f, 0.15f, 0.13f); // Brown ground
			g.mesh = &groundMesh;
			//g.rotationScaleMatrix; // use default for now
			g.shininess = 1.0f;
		}

		return UID;
	}

	unsigned int createPlayer() {
		unsigned int UID = componentManager.addEntity();


		componentManager.addComponentToEntity(CAMERA, UID);
		{
			CameraComponent& c = getComponent<CameraComponent>(UID);
			// self initialized for now
		}

		componentManager.addComponentToEntity(POSITION, UID);
		{
			PositionComponent& p = getComponent<PositionComponent>(UID);
			p.position.y = 1.0f; // 1 meter above ground
		}

		componentManager.addComponentToEntity(VELOCITY, UID);
		{
			VelocityComponent& v = getComponent<VelocityComponent>(UID);
			v.velocity.z = -1.5f; // little forward vel
		}

		componentManager.addComponentToEntity(MASS, UID);
		{
			MassComponent& m = getComponent<MassComponent>(UID);
			m.mass = 100; // kg
		}

		componentManager.addComponentToEntity(ANGULAR_POSITION, UID);
		{
			AngularPositionComponent& p = getComponent<AngularPositionComponent>(UID);
		}

		componentManager.addComponentToEntity(ANGULAR_VELOCITY, UID);
		{
			AngularVelocityComponent& v = getComponent<AngularVelocityComponent>(UID);
		}

		componentManager.addComponentToEntity(MOMENT_OF_INERTIA, UID);
		{
			MomentofInertiaComponent& m = getComponent<MomentofInertiaComponent>(UID);
			const float val = 2.0f / 5.0f * 100.0f * 0.75f * 0.75f; // 2/5 M r^2  solid sphere
			m.momentofInertia = glm::mat3(
				val,  0.0, 0.0,
				0.0,  val, 0.0,
				0.0,  0.0, val
			);
		}

		componentManager.addComponentToEntity(GENERAL_DRAW, UID);
		{
			GeneralDrawComponent& g = getComponent<GeneralDrawComponent>(UID);
			g.colour = glm::vec3(0.0f, 1.0f, 0.0f); // player colour green
			g.mesh = &planeMesh;

			//glm::vec3 direction;
			//glm::mat4 model;
			//{
			//	glm::vec3 modelDirection = glm::vec3(0.0f, 1.0f, 0.0f);
			//	glm::vec3 rotationVector = glm::cross(modelDirection, glm::normalize(direction));
			//	float rotationAngle = glm::angle(modelDirection, glm::normalize(direction));

			//	model = glm::rotate(model, rotationAngle, rotationVector);
			//	model = glm::scale(model, glm::vec3(0.1f, 0.5f, 0.1f));
			//}

			//g.rotationScaleMatrix; // Use default for now
			g.shininess = 8.0f;
		}

		return UID;
	}

	static float calculateRadius(float mass) {
		float volume = mass / 5540; // density of earth
		return std::cbrt(volume * (3.0f / 4.0f) / 3.1415927f);
	}

	unsigned int createPlanet() {

		unsigned int UID = componentManager.addEntity();
		componentManager.addComponentToEntity(DRAW, UID);
		componentManager.addComponentToEntity(POSITION, UID);
		componentManager.addComponentToEntity(VELOCITY, UID);
		componentManager.addComponentToEntity(MASS, UID);
		componentManager.addComponentToEntity(COALESCABLE, UID);

		PositionComponent&    p = getComponent<PositionComponent>(UID);
		VelocityComponent&    v = getComponent<VelocityComponent>(UID);
		MassComponent&        m = getComponent<MassComponent>(UID);
		CoalescableComponent& r = getComponent<CoalescableComponent>(UID);
		DrawComponent&        d = getComponent<DrawComponent>(UID);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<GLfloat> posGen(-30.0f, 30.0f);
		p.position.x = posGen(gen);
		p.position.y = posGen(gen);
		p.position.z = posGen(gen);
		std::uniform_real_distribution<GLfloat> velGen(-5.0f, 5.0f);
		v.velocity.x = velGen(gen);
		v.velocity.y = velGen(gen);
		v.velocity.z = velGen(gen);
		std::uniform_real_distribution<GLfloat> colourGen(0.0f, 1.0f);
		d.colour.r = colourGen(gen);
		d.colour.g = colourGen(gen);
		d.colour.b = colourGen(gen);
		std::uniform_real_distribution<GLfloat> massGen(500.0f, 50000.0f);
		m.mass = massGen(gen);
		r.radius = calculateRadius(m.mass);

		d.mesh = &planetMesh;

		return UID;
	}

	void splitPlanet(unsigned int sourceUID) {

		PositionComponent&    sourceP = getComponent<PositionComponent>(sourceUID);
		VelocityComponent&    sourceV = getComponent<VelocityComponent>(sourceUID);
		MassComponent&        sourceM = getComponent<MassComponent>(sourceUID);
		CoalescableComponent& sourceR = getComponent<CoalescableComponent>(sourceUID);
		DrawComponent&        sourceD = getComponent<DrawComponent>(sourceUID);

		unsigned int newUID = componentManager.addEntity();
		componentManager.addComponentToEntity(DRAW, newUID);
		componentManager.addComponentToEntity(POSITION, newUID);
		componentManager.addComponentToEntity(VELOCITY, newUID);
		componentManager.addComponentToEntity(MASS, newUID);
		componentManager.addComponentToEntity(COALESCABLE, newUID);

		PositionComponent&    p = getComponent<PositionComponent>(newUID);
		VelocityComponent&    v = getComponent<VelocityComponent>(newUID);
		MassComponent&        m = getComponent<MassComponent>(newUID);
		CoalescableComponent& r = getComponent<CoalescableComponent>(newUID);
		DrawComponent&        d = getComponent<DrawComponent>(newUID);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<GLfloat> colourGen(0.0f, 1.0f);
		d.colour.r = colourGen(gen);
		d.colour.g = colourGen(gen);
		d.colour.b = colourGen(gen);

		//v.velocity       = sourceV.velocity * (-1.0f);
		std::uniform_real_distribution<float> velGen(-10.0f, 10.0f);
		v.velocity.x = velGen(gen);
		v.velocity.y = velGen(gen);
		v.velocity.z = velGen(gen);
		sourceV.velocity.x = velGen(gen);
		sourceV.velocity.y = velGen(gen);
		sourceV.velocity.z = velGen(gen);

		m.frameForce = glm::vec3(0.0f);
		m.mass = sourceM.mass / 2.0f;
		sourceM.mass = sourceM.mass / 2.0f;
		r.radius = calculateRadius(m.mass);
		sourceR.radius = r.radius;
		d.colour.r = colourGen(gen);
		d.colour.g = colourGen(gen);
		d.colour.b = colourGen(gen);
		sourceD.colour.r = colourGen(gen);
		sourceD.colour.g = colourGen(gen);
		sourceD.colour.b = colourGen(gen);
		d.mesh = sourceD.mesh;
		d.shininess = sourceD.shininess;

		p.position = sourceP.position + glm::normalize(sourceV.velocity) * ((-r.radius) * 3.0f);
	}

	unsigned int createPlanet(
		const PositionComponent* p1, const PositionComponent* p2,
		const VelocityComponent* v1, const VelocityComponent* v2,
		const MassComponent*     m1, const MassComponent*     m2,
		const DrawComponent*     d1, const DrawComponent*     d2)
	{

		unsigned int UID = componentManager.addEntity();
		componentManager.addComponentToEntity(DRAW, UID);
		componentManager.addComponentToEntity(POSITION, UID);
		componentManager.addComponentToEntity(VELOCITY, UID);
		componentManager.addComponentToEntity(MASS, UID);
		componentManager.addComponentToEntity(COALESCABLE, UID);

		PositionComponent&    p = getComponent<PositionComponent>(UID);
		VelocityComponent&    v = getComponent<VelocityComponent>(UID);
		MassComponent&        m = getComponent<MassComponent>(UID);
		CoalescableComponent& r = getComponent<CoalescableComponent>(UID);
		DrawComponent&        d = getComponent<DrawComponent>(UID);

		m.mass = m1->mass + m2->mass;
		p.position = (m1->mass * p1->position + m2->mass * p2->position) / (m1->mass + m2->mass);
		v.velocity = (m1->mass * v1->velocity + m2->mass * v2->velocity) / (m1->mass + m2->mass);
		d.colour = (m1->mass * d1->colour + m2->mass * d2->colour) / (m1->mass + m2->mass);
		d.shininess = (d1->shininess + d2->shininess) / 2.0f;

		r.radius = calculateRadius(m.mass);

		d.mesh = &planetMesh;

		return UID;
	}

	unsigned int createLaser(glm::vec3 position, glm::vec3 direction) {
		const float LASER_VELOCITY = 30.0f;

		unsigned int UID = componentManager.addEntity();
		componentManager.addComponentToEntity(GENERAL_DRAW, UID);
		componentManager.addComponentToEntity(POSITION, UID);
		componentManager.addComponentToEntity(VELOCITY, UID);
		componentManager.addComponentToEntity(BULLET, UID);

		PositionComponent&    p = getComponent<PositionComponent>(UID);
		VelocityComponent&    i = getComponent<VelocityComponent>(UID);
		BulletComponent&      b = getComponent<BulletComponent>(UID);
		GeneralDrawComponent& d = getComponent<GeneralDrawComponent>(UID);

		p.position = position;

		i.velocity = glm::normalize(direction) * LASER_VELOCITY;

		b.radius = 1.0f;


		glm::mat4 model;
		{
			glm::vec3 modelDirection = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec3 rotationVector = glm::cross(modelDirection, glm::normalize(direction));
			float rotationAngle = glm::angle(modelDirection, glm::normalize(direction));

			model = glm::rotate(model, rotationAngle, rotationVector);
			model = glm::scale(model, glm::vec3(0.1f, 0.5f, 0.1f));
		}

		d.colour = glm::vec3(1.0f, 0.0f, 0.0f);
		d.mesh = &laserMesh;
		d.rotationScaleMatrix = model;
		d.shininess = 128.0f;

		return UID;
	}

}
