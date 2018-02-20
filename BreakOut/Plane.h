#pragma once
#include <glm\ext.hpp>
#include "PhysicsObject.h"
#include "RigidBody.h"
#include "VecLib\UtilityVec2.h"

class Plane : public PhysicsObject
{

public:
	Plane();
	Plane(Vector2 normal, float distance, glm::vec4 colour);
	~Plane();

	virtual void fixedUpdate(Vector2 gravity, float timeStep);
	virtual void draw();
	virtual void debug() {}
	virtual void resetNormal();
	virtual void resetPosition();
	
	bool resolveCollision(RigidBody* actor2, Vector2 contact);
	float distance(Vector2 point) { return (UV2::dot(point, m_normal) - m_distanceFromOrigin); }

	Vector2 getNormal() { return m_normal; }
	float getDistance() { return m_distanceFromOrigin; }
	float getBounciness() { return m_bounciness; }


protected:
	Vector2 m_normal;
	float m_distanceFromOrigin;
	float m_bounciness;

};

