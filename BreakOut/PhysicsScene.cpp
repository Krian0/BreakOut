#include <iostream>
#include "PhysicsScene.h"


PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0)) {}
PhysicsScene::PhysicsScene(glm::vec2 gravity, float timeStep) : m_gravity(gravity), m_timeStep(timeStep) {}
PhysicsScene::~PhysicsScene() 
{
	for (auto anActor : m_actors)	delete anActor;
}


void PhysicsScene::addActor(PhysicsObject* actor)		{ m_actors.push_back(actor); }
void PhysicsScene::removeActor(PhysicsObject* actor)	{ m_actors.remove(actor); }


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
	for (auto it = m_actors.begin(); it != m_actors.end(); it++)				//Loop through each actor, check if they collide with each actor after it in the list, so long as they are both shapes that we want to check.
	{
		PhysicsObject* obj1 = (*it);
		if (obj1->getShapeID() == SPRING) continue;

		for (auto it2 = std::next(it); it2 != m_actors.end(); it2++)
		{
			PhysicsObject* obj2 = (*it2);
			
			if (invalidShapePair(obj1, obj2)) 
				continue;
				
			obj1->detectCollision(*obj2);
		}
	}
}


//Private Functions:

bool PhysicsScene::invalidShapePair(PhysicsObject* actor1, PhysicsObject* actor2)
{
	
	ShapeType shape1 = actor1->getShapeID(), shape2 = actor2->getShapeID();
	return ((actor1->isStatic() && actor2->isStatic() && (shape1 != PLANE || shape2 != PLANE)) || (shape1 == PLANE && shape2 == PLANE) || shape2 == SPRING);
}