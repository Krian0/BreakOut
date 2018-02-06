#pragma once
#include "PhysicsObject.h"


class RigidBody : public PhysicsObject
{

public:

	RigidBody(ShapeType shapeTypeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass);
	~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force);
	void applyForceToActor(RigidBody* actor2, glm::vec2 force);

	virtual bool checkCollision(PhysicsObject* otherObject) = 0;

	glm::vec2 getPosition() { return m_position; }
	glm::vec2 getVelocity() { return m_velocity; }
	glm::vec2 getForce() { return m_velocity * m_mass; }
	float getRotation() { return m_rotation; }
	float getMass() { return m_mass; }


protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_rotation;
	float m_mass;
};

