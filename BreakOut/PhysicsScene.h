#pragma once
#include "PhysicsObject.h"
#include <glm\vec2.hpp>
#include <list>

using std::list;


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

	void checkForCollision();

	static bool planePlane (PhysicsObject*, PhysicsObject*);
	static bool planeSphere (PhysicsObject*, PhysicsObject*);
	static bool sphereSphere (PhysicsObject*, PhysicsObject*);
	static bool spherePlane (PhysicsObject*, PhysicsObject*);


	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }


protected:
	glm::vec2 m_gravity;
	list<PhysicsObject*> m_actors;
	float m_timeStep;

};

