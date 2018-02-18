#pragma once
#include <glm\vec2.hpp>
#include <list>
#include <utility>
#include <vector>

#include "PhysicsObject.h"

using std::vector;
using std::list;
using std::pair;
using std::make_pair;


struct Projection
{
	glm::vec2 Projection_A;
	glm::vec2 Projection_B;
	bool Intersection;

	Projection(glm::vec2 vectorA, glm::vec2 vectorB) : Projection_A(vectorA), Projection_B(vectorB)
	{
		if (Projection_A.y < Projection_B.x || Projection_B.y < Projection_A.x)
			Intersection = true;
		else
			Intersection = false;
	}

	float getDistance()
	{
		return fminf(Projection_A.y, Projection_B.y) - fmaxf(Projection_A.x, Projection_B.x);
	}
};


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
	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	glm::vec2 getGravity() const { return m_gravity; }
	float getTimeStep() const { return m_timeStep; }


protected:
	//static glm::vec2 getNormalAxis(glm::vec2 corner1, glm::vec2 corner2);
	//static glm::vec2 getVecProjection(glm::vec2 vec1, glm::vec2 vec2);

	list<PhysicsObject*> m_actors;
	glm::vec2 m_gravity;
	float m_timeStep;

};

