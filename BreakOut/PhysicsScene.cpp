#include <utility>
#include <iostream>

#include "PhysicsScene.h"
#include "RigidBody.h"

using std::pair;
using std::make_pair;

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0)) {}
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


		list<pair<PhysicsObject*, PhysicsObject*>> collisionPairs;
		for (auto anActor : m_actors)
		{
			for (auto otherActor : m_actors)
			{
				if (anActor == otherActor)
					continue;

				for (auto addedPair : collisionPairs)
					if (addedPair == make_pair(anActor, otherActor) || addedPair == make_pair(otherActor, anActor))
						continue;

				RigidBody* rbActor = dynamic_cast<RigidBody*>(anActor);
				if (rbActor->checkCollision(otherActor) == true)
				{
					collisionPairs.push_back(make_pair(anActor, otherActor));
					rbActor->applyForceToActor(dynamic_cast<RigidBody*>(otherActor), rbActor->getForce());
				}
			}
		}

		collisionPairs.clear();
	}
}

void PhysicsScene::draw()
{
	for (auto anActor : m_actors)
		anActor->draw();
}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
static fn collisionFunctionArray[] =
{
	PhysicsScene::planePlane,		PhysicsScene::planeSphere,
	PhysicsScene::sphereSphere,		PhysicsScene::spherePlane
};

void PhysicsScene::checkForCollision()
{
	int actorCount = m_actors.size();

	for (auto it = m_actors.begin(); it != m_actors.end(); it++)
	{
		PhysicsObject* ob1 = *it;
		for (auto it2 = std::next(it); it2 != m_actors.end(); it2++)
		{
			
			PhysicsObject* ob2 = *it2;
			int shapeID1 = ob1->getShapeID();
			int shapeID2 = ob2->getShapeID();

			int functionIDx = (shapeID1 * NULLSHAPE) + shapeID2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIDx];

			if (collisionFunctionPtr != nullptr)
				collisionFunctionPtr(ob1, ob2);
		}
	}
}
