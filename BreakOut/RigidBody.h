#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{

public:

	RigidBody(ShapeType shapeTypeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float bounciness, bool isStatic, float linearDrag = 0.3f, float rotationalDrag = 0.5f);
	~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);

	bool applyForce(glm::vec2 force, glm::vec2 pos);
	bool applyForceToActor(RigidBody& actor2, glm::vec2 force, glm::vec2 pos1, glm::vec2 pos2);
	bool rigidResolve(RigidBody& rigid, glm::vec2 contact, glm::vec2 normal);

	void setPosition(glm::vec2 position);
	void setPositions(RigidBody& otherRigid, glm::vec2 contactForce);

	glm::vec2 getPosition() { return m_position; }
	glm::vec2 getVelocity() { return m_velocity; }
	
	float getRotVelocity()	{ return m_rotationalVelocity; }
	float getRotation()		{ return m_rotation; }
	float getMass()			{ return m_mass; }
	float getInertia()		{ return m_inertia; }
	float getBounciness()	{ return m_bounciness; }


protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;

	float m_rotationalVelocity;
	float m_rotation;
	float m_inertia;
	float m_mass;
	float m_linearDrag;
	float m_rotationalDrag;

	const double MIN_LINEAR_THRESHOLD = 0.08;
	const double MIN_ROTATION_THRESHOLD = 0.08;
};