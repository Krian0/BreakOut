#pragma once
#include "RigidBody.h"
#include <glm\ext.hpp>

class Plane : public PhysicsObject
{

public:
	Plane();
	Plane(vec2 normal, float distance, float bounciness);
	Plane(vec2 pointA, vec2 pointB, float bounciness);
	~Plane();


	virtual void fixedUpdate(vec2 gravity, float timeStep);
	virtual void draw();


	bool detectCollision(PhysicsObject& obj) override;
	bool detectCollision(CData& data, Plane& plane) override;
	bool detectCollision(CData& data, Sphere& sphere) override;
	bool detectCollision(CData& data, Box& box) override;
	bool detectCollision(vec2& point);


	float distanceToPoint(vec2 point);
	float getElasticity(RigidBody* obj);

	vec2 getRightPerp() { return vec2(-m_normal.y, m_normal.x); }
	vec2 getLeftPerp()	{ return vec2(m_normal.y, -m_normal.x); }

	vec2 getNormal()				{ return m_normal; }
	vec2 getPlaneOrigin()			{ return m_planeOrigin; }
	float getDistanceFromOrigin()	{ return m_distanceFromOrigin; }
	float getBounciness()			{ return m_bounciness; }


protected:
	bool resolveCollision(RigidBody* actor2, vec2 contact, vec2 contactVelocity, float r = 0);

	vec2  m_normal;
	vec2  m_planeOrigin;
	float m_distanceFromOrigin;
	float m_bounciness;
};