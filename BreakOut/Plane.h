#pragma once
#include <glm\ext.hpp>
#include "PhysicsObject.h"
#include "RigidBody.h"

class Plane : public PhysicsObject
{

public:
	Plane();
	Plane(glm::vec2 normal, float distance, glm::vec4 colour);
	~Plane();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void draw();
	virtual void debug() {}
	virtual void resetNormal();
	virtual void resetPosition();
	
	bool resolveCollision(RigidBody* actor2, glm::vec2 contact);
	float distance(glm::vec2 point) { return glm::dot(point, m_normal) - m_distanceFromOrigin; }

	glm::vec2 getNormal() { return m_normal; }
	float getDistance() { return m_distanceFromOrigin; }
	float getBounciness() { return m_bounciness; }


protected:
	glm::vec2 m_normal;
	float m_distanceFromOrigin;
	float m_bounciness;

};

