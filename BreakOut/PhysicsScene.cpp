#include <glm\ext.hpp>
#include <iostream>

#include "PhysicsScene.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Box.h"
#include "Plane.h"
#include "Spring.h"
#include "CDetectionHandler.h"


PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(Vector2(0, 0)) {}
PhysicsScene::~PhysicsScene() 
{
	for (auto anActor : m_actors)
		delete anActor;
}


void PhysicsScene::addActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
	m_actors.remove(actor);
}

void PhysicsScene::debugScene()
{
	int index = 0;
	for (auto anActor : m_actors)
	{
		std::cout << "DEBUG EVENT [actor at index " << index << "] : ";
		anActor->debug();
		index++;
	}
}


void PhysicsScene::update(float deltaTime)
{
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto anActor : m_actors)
			anActor->fixedUpdate(m_gravity, m_timeStep);
		
		accumulatedTime -= m_timeStep;

		checkForCollision();
	}
}

void PhysicsScene::draw()
{
	for (auto anActor : m_actors)
		anActor->draw();
}


void PhysicsScene::checkForCollision()
{
	Vector2 contact;
	Vector2 normal;
	float numberOfContacts = 0;

	for (auto it = m_actors.begin(); it != m_actors.end(); it++)
		for (auto it2 = std::next(it); it2 != m_actors.end(); it2++)
			if (CDetectionHandler::detectCollision((*it), (*it2), contact, normal, numberOfContacts))
				return;//HandleResolution
				
}