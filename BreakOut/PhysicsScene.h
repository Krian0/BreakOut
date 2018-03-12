#pragma once
#include <vector>
#include "VecLib\Vector2.h"
#include "PhysicsObject.h"

class PhysicsScene
{
public:
	PhysicsScene();
	PhysicsScene(vec2 gravity, float timeStep);
	~PhysicsScene();


	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);
	void removeAndDeleteActor(PhysicsObject* actor); 
	void update(float deltaTime);
	void draw();

	void checkForCollision();

	void setGravity(const vec2 gravity)		{ m_gravity = gravity; }
	void setTimeStep(const float timeStep)	{ m_timeStep = timeStep; }
	
	vec2 getGravity() const		{ return m_gravity; }
	float getTimeStep() const	{ return m_timeStep; }


private:

	bool isValidShapePair(PhysicsObject* actor1, PhysicsObject* actor2);
	void setUpValidShapePairs();

	std::vector<PhysicsObject*> m_actors;
	vec2  m_gravity;
	float m_timeStep;
	bool  m_validShapePairs[SHAPE_COUNT][SHAPE_COUNT];
};

