#pragma once
#include <glm\vec2.hpp>
#include <list>
#include <utility>

#include "PhysicsObject.h"

using std::list;
using std::pair;
using std::make_pair;


class PhysicsScene
{

public:
	PhysicsScene();
	~PhysicsScene();


	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);
	void debugScene();
	void update(float deltaTime);
	void draw();

	//bool isValidPair(list<pair<int, int>> pairList, int index1, int index2);
	static bool planePlane(PhysicsObject*, PhysicsObject*);
	static bool planeSphere(PhysicsObject*, PhysicsObject*);
	static bool planeBox(PhysicsObject*, PhysicsObject*);
	
	static bool sphereSphere(PhysicsObject*, PhysicsObject*);
	static bool sphereBox(PhysicsObject*, PhysicsObject*);
	static bool spherePlane(PhysicsObject*, PhysicsObject*);
	
	static bool boxBox(PhysicsObject*, PhysicsObject*);
	static bool boxSphere(PhysicsObject*, PhysicsObject*);
	static bool boxPlane(PhysicsObject*, PhysicsObject*);

	void checkForCollision();


	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }


protected:
	glm::vec2 getNormalAxis(glm::vec2 corner1, glm::vec2 corner2);

	glm::vec2 m_gravity;
	list<PhysicsObject*> m_actors;
	float m_timeStep;

};

