#pragma once
#include "PhysicsObject.h"


class RigidBody : public PhysicsObject
{

public:

	RigidBody(ShapeType shapeTypeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float linearDrag = 0.3f, float rotationalDrag = 0.3f);
	~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();

	void applyForce(glm::vec2 force);
	void applyForceToActor(RigidBody* actor2, glm::vec2 force);
	void resolveCollision(RigidBody* actor2);

	void setLinearDrag(float drag);
	void setRotationalDrag(float drag);

	glm::vec2 getPosition() { return m_position; }
	glm::vec2 getVelocity() { return m_velocity; }
	glm::vec2 getForce() { return m_velocity * m_mass; }
	float getRotation() { return m_rotation; }
	float getMass() { return m_mass; }
	float getBounciness() { return m_bounciness; }
	float getLinearDrag() { return m_linearDrag; }
	float getRotationalDrag() { return m_rotationalDrag; }


protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_rotationalVelocity;
	float m_rotation;
	float m_mass;
	float m_bounciness;
	float m_linearDrag;
	float m_rotationalDrag;

const float MIN_LINEAR_THRESHOLD = 0.1;
const float	MIN_ROTATION_THRESHOLD = 0.1;
};

