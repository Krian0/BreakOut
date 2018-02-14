#include <glm\ext.hpp>
#include <iostream>
#include <vector>

#include "PhysicsScene.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Box.h"
#include "Plane.h"
#include "Spring.h"

using std::vector;


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


		checkForCollision();
		//list<pair<int, int>> pairs;
		//int indexA, indexB = 0;
		//for (auto actor1 : m_actors)
		//{
		//	RigidBody* rb1 = dynamic_cast<RigidBody*>(actor1);
		//	for (auto actor2 : m_actors)
		//	{
		//		if (isValidPair(pairs, indexA, indexB) && rb1->checkCollision(actor2) == true)
		//			rb1->applyForceToActor(dynamic_cast<RigidBody*>(actor2), rb1->getForce());
		//		
		//		indexB++;
		//	}
		//	indexA++;
		//}

		//pairs.clear();
	}
}

void PhysicsScene::draw()
{
	for (auto anActor : m_actors)
		anActor->draw();
}


//bool PhysicsScene::isValidPair(list<pair<int, int>> pairList, int index1, int index2)
//{
//	if (index1 == index2)
//		return false;
//
//	pair<int, int> indexPair1(index1, index2);
//	pair<int, int> indexPair2(index2, index1);
//	
//	for (auto addedPair : pairList)
//		if (addedPair == indexPair1 || addedPair == indexPair2)
//			return false;
//
//	pairList.push_back(indexPair1);
//	return true;
//}


typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::planePlane,	PhysicsScene::planeSphere,	PhysicsScene::planeBox,	
	PhysicsScene::spherePlane,  PhysicsScene::sphereSphere,	PhysicsScene::sphereBox,	
	PhysicsScene::boxPlane,		PhysicsScene::boxBox,		PhysicsScene::boxSphere	
};


bool PhysicsScene::planePlane(PhysicsObject* plane_1, PhysicsObject* plane_2)
{
	return false;
}

bool PhysicsScene::planeSphere(PhysicsObject* plane, PhysicsObject* sphere)
{
	return spherePlane(sphere, plane);
}

bool PhysicsScene::planeBox(PhysicsObject* plane, PhysicsObject* box)
{
	return false;
}

bool PhysicsScene::sphereSphere(PhysicsObject* sphere_1, PhysicsObject* sphere_2)
{
	Sphere* s1 = dynamic_cast<Sphere*>(sphere_1);
	Sphere* s2 = dynamic_cast<Sphere*>(sphere_2);
	if (s1 == nullptr || s2 == nullptr) return false;


	if (glm::distance(s1->getPosition(), s2->getPosition()) <= (s1->getRadius() + s2->getRadius()))
		return s1->resolveCollision(s2, (s1->getPosition() + s2->getPosition()) * 0.5f);

	return false;
}

bool PhysicsScene::sphereBox(PhysicsObject* sphere, PhysicsObject* box)
{
	return false;
}

bool PhysicsScene::spherePlane(PhysicsObject* sphere, PhysicsObject* plane)
{
	Sphere* s = dynamic_cast<Sphere*>(sphere);
	Plane* p = dynamic_cast<Plane*>(plane);
	if (s == nullptr || p == nullptr) return false;


	float sphereToPlane = glm::dot(s->getPosition(), p->getNormal()) - p->getDistance();
	glm::vec2 contact = sphereToPlane < 0	?	s->getPosition() + (p->getFlippedNormal() * -s->getRadius())	:	s->getPosition() + (p->getNormal() * -s->getRadius());
	if (sphereToPlane < 0)	sphereToPlane *= -1;
		
	if ((s->getRadius() - sphereToPlane) > 0)
		return (p->resolveCollision(s, contact));

	return false;
}

bool PhysicsScene::boxBox(PhysicsObject* box_1, PhysicsObject* box_2)
{
	Box* b1 = dynamic_cast<Box*>(box_1);
	Box* b2 = dynamic_cast<Box*>(box_2);

	vector<glm::vec2> axesToTestVec;
	axesToTestVec.push_back(b1->getLocalXAxis());
	axesToTestVec.push_back(b1->getLocalYAxis());

	if (b1->getRotation() == b2->getRotation())
	{
		axesToTestVec.push_back(b2->getLocalXAxis());
		axesToTestVec.push_back(b2->getLocalYAxis());
	}

		//proj.x = (dp / (b.x*b.x + b.y*b.y)) * b.x;
		//proj.y = (dp / (b.x*b.x + b.y*b.y)) * b.y;

		//where dp is the dotprod of a and b: dp = (a.x*b.x + a.y*b.y)

		//	Note that the result is a vector; also, (b.x*b.x + b.y*b.y) is simply the length of b squared.

		//	If b is a unit vector, (b.x*b.x + b.y*b.y) = 1, and thus a projected onto b reduces to :

		//proj.x = dp*b.x;
		//proj.y = dp*b.y;


	return true;
}

bool PhysicsScene::boxSphere(PhysicsObject* box, PhysicsObject* sphere)
{
	return false;
}

bool PhysicsScene::boxPlane(PhysicsObject* box, PhysicsObject* plane)
{
	return false;
}


void PhysicsScene::checkForCollision()
{
	for (auto it = m_actors.begin(); it != m_actors.end(); it++)
	{
		PhysicsObject* actor1 = *it;
		for (auto it2 = std::next(it); it2 != m_actors.end(); it2++)
		{
			PhysicsObject* actor2 = *it2;
			int functionIDx = (actor1->getShapeID() * SHAPE_COUNT) + actor2->getShapeID();
			fn collisionFunctionPtr = collisionFunctionArray[functionIDx];

			if (collisionFunctionPtr != nullptr)
				collisionFunctionPtr(actor1, actor2);
		}
	}
}

glm::vec2 PhysicsScene::getNormalAxis(glm::vec2 corner1, glm::vec2 corner2)
{
	return glm::vec2((corner2.y - corner1.y), -(corner2.x - corner1.x));
}