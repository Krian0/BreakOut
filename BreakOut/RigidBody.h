#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{

public:

	RigidBody(ShapeType shapeTypeID, vec2 position, vec2 velocity, float rotation, float mass, float bounciness, bool isStatic, float linearDrag = 0.3f, float rotationalDrag = 0.5f);
	~RigidBody();

	virtual void fixedUpdate(vec2 gravity, float timeStep);

	bool applyForce(vec2 force, vec2 pos);
	bool applyForceToActor(RigidBody& actor2, vec2 force, vec2 pos1, vec2 pos2);
	bool rigidResolve(RigidBody& rigid, vec2 contact, vec2 normal);

	void setPosition(vec2 position);
	void setPositions(RigidBody& otherRigid, vec2 contactForce);

	float getElasticity(RigidBody* obj);

	vec2 getPosition() { return m_position; }
	vec2 getVelocity() { return m_velocity; }
	
	float getRotationalVelocity()	{ return m_rotationalVelocity; }
	float getRotation()				{ return m_rotation; }
	float getMass()					{ return (m_isStatic) ? INT_MAX : m_mass; }
	float getInertia()				{ return (m_isStatic) ? INT_MAX : m_inertia; }
	float getBounciness()			{ return m_bounciness; }


protected:
	vec2 m_position;
	vec2 m_velocity;

	float m_rotationalVelocity;
	float m_rotation;
	float m_mass;
	float m_inertia;
	float m_bounciness;
	float m_linearDrag;
	float m_rotationalDrag;

	const double MIN_LINEAR_THRESHOLD = 0.08;
	const double MIN_ROTATION_THRESHOLD = 0.01;
};