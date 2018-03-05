#pragma once
#include "RigidBody.h"
#include <glm\ext.hpp>

class Plane : public PhysicsObject
{

public:
	Plane();
	Plane(glm::vec2 normal, float distance, float bounciness);
	Plane(glm::vec2 pointA, glm::vec2 pointB, float bounciness);
	~Plane();


	void resetVelocity() {}

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void draw();


	bool detectCollision(PhysicsObject& obj) override;
	bool detectCollision(CData& data, Plane& plane) override;
	bool detectCollision(CData& data, Sphere& sphere) override;
	bool detectCollision(CData& data, Box& box) override;
	bool detectCollision(glm::vec2& point, glm::vec2& pointOut) override;

	float distance(glm::vec2 point)	 { return (glm::dot(point, m_normal) - m_distanceFromOrigin); }

	glm::vec2 getNormal()			{ return m_normal; }
	glm::vec2 getPlaneOrigin()		{ return m_planeOrigin; }
	float getDistanceFromOrigin()	{ return m_distanceFromOrigin; }
	float getBounciness()			{ return m_bounciness; }


protected:
	bool resolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2 contactVelocity, float r = 0);

	glm::vec2 m_normal;
	glm::vec2 m_planeOrigin;
	float m_distanceFromOrigin;
};