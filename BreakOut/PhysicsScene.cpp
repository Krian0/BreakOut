#include <glm\ext.hpp>
#include <iostream>

#include "PhysicsScene.h"
#include "RigidBody.h"
#include "Sphere.h"
#include "Box.h"
#include "Plane.h"
#include "Spring.h"


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
	return boxPlane(box, plane);
}

bool PhysicsScene::sphereSphere(PhysicsObject* sphere_1, PhysicsObject* sphere_2)
{
	Sphere* s1 = dynamic_cast<Sphere*>(sphere_1);
	Sphere* s2 = dynamic_cast<Sphere*>(sphere_2);
	if (s1 == nullptr || s2 == nullptr) return false;


	if (glm::distance(s1->getPosition(), s2->getPosition()) < (s1->getRadius() + s2->getRadius()))
		return s1->resolveCollision(s2, 0.5f * (s1->getPosition() + s2->getPosition()));

	return false;
}

bool PhysicsScene::sphereBox(PhysicsObject* sphere, PhysicsObject* box)
{
	//Sphere* s = dynamic_cast<Sphere*>(sphere);
	//Box* b = dynamic_cast<Box*>(box);
	//if (s == nullptr || b == nullptr) return false;

	//glm::vec2 distance = s->getPosition() - b->getPosition();

	//int numberOfContacts = 0;
	//glm::vec2 contact(0, 0);

	//for (int i = 0; i < 4; i++)
	//{
	//	glm::vec2 corner = b->getCorner((BoxCorner)i);
	//	glm::vec2 p = (b->getPosition().x - corner.x) * b->getLocalXAxis() + (b->getPosition().y - corner.y) * b->getLocalYAxis();

	//	glm::vec2 dp = p - distance;

	//	if (dp.x * dp.x + dp.y * dp.y < s->getRadius() * s->getRadius())
	//	{
	//		numberOfContacts++;
	//		contact += b->getPosition() - corner;
	//	}
	//}

	return false;
}

bool PhysicsScene::spherePlane(PhysicsObject* sphere, PhysicsObject* plane)
{
	Sphere* s = dynamic_cast<Sphere*>(sphere);
	Plane* p = dynamic_cast<Plane*>(plane);
	if (s == nullptr || p == nullptr) return false;

	float sphereToPlane = p->distance(s->getPosition());
	glm::vec2 contact = s->getPosition() + (p->getNormal() * -s->getRadius());
	if (sphereToPlane < 0)
	{
		sphereToPlane *= -1;
		contact *= -1;
	}
		
	if ((s->getRadius() - sphereToPlane) > 0)
		return (p->resolveCollision(s, contact));

	return false;
}

bool PhysicsScene::boxBox(PhysicsObject* box_1, PhysicsObject* box_2)
{
	Box* b1 = dynamic_cast<Box*>(box_1);
	Box* b2 = dynamic_cast<Box*>(box_2);

	vector<glm::vec2> axesToTest;
	axesToTest.push_back(b1->getLocalXAxis());
	axesToTest.push_back(b1->getLocalYAxis());

	if (b1->getRotation() != b2->getRotation())
	{
		axesToTest.push_back(b2->getLocalXAxis());
		axesToTest.push_back(b2->getLocalYAxis());
	}

	for (int i = 0; i < axesToTest.size(); i++)
	{
		Projection p(b1->getProjection(axesToTest[i]), b2->getProjection(axesToTest[i]));
		if (!p.Intersection) return false;

		//ask about a compatible collision resolution
	}

	return true;
}

bool PhysicsScene::boxSphere(PhysicsObject* box, PhysicsObject* sphere)
{
	return false;
}

bool PhysicsScene::boxPlane(PhysicsObject* box, PhysicsObject* plane)
{
	//Box* b = dynamic_cast<Box*>(box);
	//Plane* p = dynamic_cast<Plane*>(plane);
	//if (b == nullptr || p == nullptr) return false;

	//int numberOfContacts = 0;
	//float contactVelocity = 0;
	//glm::vec2 contact(0, 0);

	//float centerFromPlane = p->distance(b->getPosition());

	//
	//for (int i = 0; i < 4; i++)
	//{
	//	glm::vec2 corner = b->getCorner((BoxCorner)i);

	//	float cornerFromPlane = p->distance(corner);
	//	glm::vec2 thing1 = -(b->getPosition().y - corner.y) * b->getLocalXAxis();										//NOTE:
	//	glm::vec2 thing2 =  (b->getPosition().x - corner.x) * b->getLocalYAxis();										//ASK WHAT THIS DOES,
	//	float velocityIntoPlane = glm::dot(b->getVelocity() + b->getRotation() * (thing1 + thing2), p->getNormal());	//AND FOR A BETTER WAY TO CALCULATE IT

	//	if ((cornerFromPlane > 0 && centerFromPlane < 0) || (cornerFromPlane < 0 && centerFromPlane > 0))
	//	{
	//		numberOfContacts++;
	//		contact += corner;
	//		contactVelocity += velocityIntoPlane;
	//	}
	//}

	//if (numberOfContacts == 0) 
		return false;

	//float elasticity = (b->getBounciness() + p->getBounciness()) / 2.0f;
	//glm::vec2 acceleration = -p->getNormal() * ((1 + elasticity) * (contactVelocity / numberOfContacts));

	//glm::vec2 localContact = (contact / (float)numberOfContacts) - b->getPosition();
	//float r = glm::dot(localContact, glm::vec2(p->getNormal().y, -p->getNormal().x));
	//float mass0 = 1.0f / (1.0f / b->getMass() + (r*r) / b->getInertia());

	//return b->applyForce(acceleration * mass0, localContact);
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

//glm::vec2 PhysicsScene::getNormalAxis(glm::vec2 corner1, glm::vec2 corner2)
//{
//	return glm::vec2((corner2.y - corner1.y), -(corner2.x - corner1.x));
//}
//
//glm::vec2 PhysicsScene::getVecProjection(glm::vec2 vec1, glm::vec2 vec2)
//{
//	float dpDividedByLength = glm::dot(vec1, vec2) / (vec2.x * vec2.x + vec2.y * vec2.y);
//	return glm::vec2((dpDividedByLength * vec2.x), (dpDividedByLength * vec2.y));
//}