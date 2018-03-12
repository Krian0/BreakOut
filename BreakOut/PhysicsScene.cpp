#include <iostream>
#include "PhysicsScene.h"



PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(vec2(0, 0)) 
{
	setUpValidShapePairs();
}
PhysicsScene::PhysicsScene(vec2 gravity, float timeStep) : m_gravity(gravity), m_timeStep(timeStep) 
{
	setUpValidShapePairs();
}
PhysicsScene::~PhysicsScene() 
{
	for (auto actor : m_actors)
		delete actor;
	m_actors.clear();
}


//Public Functions:

void PhysicsScene::addActor(PhysicsObject* actor)		
{
	m_actors.push_back(actor); 
}
void PhysicsScene::removeActor(PhysicsObject* actor)	
{ 
	auto iterator = std::find(m_actors.begin(), m_actors.end(), actor);
	if (iterator == m_actors.end()) return;
	m_actors.erase(iterator);
}

void PhysicsScene::removeAndDeleteActor(PhysicsObject* actor)
{
	auto iterator = std::find(m_actors.begin(), m_actors.end(), actor);
	if (iterator == m_actors.end()) return;
	delete &actor;
	m_actors.erase(iterator);
}


void PhysicsScene::update(float deltaTime)
{
	 static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto actor : m_actors)
			actor->fixedUpdate(m_gravity, m_timeStep);
		
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
	for (int first = 0; first < m_actors.size(); first++)
	{
		if (m_actors[first]->getShapeID() == SPRING) continue;

		for (int second = (first + 1); second < m_actors.size(); second++)
			if (isValidShapePair(m_actors[first], m_actors[second]))
				m_actors[first]->detectCollision(*m_actors[second]);
	}
}
//-


//Private Functions:

bool PhysicsScene::isValidShapePair(PhysicsObject* actor1, PhysicsObject* actor2)
{
	return m_validShapePairs[actor1->getShapeID()][actor2->getShapeID()];
}

void PhysicsScene::setUpValidShapePairs()
{
	for (int first = 0; first < SHAPE_COUNT; first++)
	{
		for (int second = 0; second < SHAPE_COUNT; second++)
		{
			if ((first == PLANE && second == PLANE))
				m_validShapePairs[first][second] = false;
			else
				m_validShapePairs[first][second] = true;
		}
	}
}
//-