#pragma once
#include "PhysicsObject.h"


class RigidBody : public PhysicsObject
{

public:

	RigidBody(ShapeType shapeTypeID, Vector2 position, Vector2 velocity, float rotation, float mass, float linearDrag = 0.3f, float rotationalDrag = 0.3f, bool isStatic = false);
	~RigidBody();

	virtual void fixedUpdate(Vector2 gravity, float timeStep);
	virtual void debug();

	bool applyForce(Vector2 force, Vector2 pos);
	bool applyForceToActor(RigidBody* actor2, Vector2 force, Vector2 pos1, Vector2 pos2);
	bool resolveCollision(RigidBody* actor2, Vector2 contact, Vector2* collisionNormal = nullptr);

	void setLinearDrag(float drag);
	void setRotationalDrag(float drag);

	Vector2 getPosition() { return m_position; }
	Vector2 getVelocity() { return m_velocity; }
	Vector2 getForce() { return m_velocity * m_mass; }
	float getRotation() { return m_rotation; }
	float getMass() { return m_mass; }
	float getInertia() { return m_inertia; }
	float getBounciness() { return m_bounciness; }
	float getLinearDrag() { return m_linearDrag; }
	float getRotationalDrag() { return m_rotationalDrag; }


protected:
	Vector2 m_position;
	Vector2 m_velocity;
	float m_rotationalVelocity;
	float m_rotation;
	float m_inertia;
	float m_mass;
	float m_bounciness;
	float m_linearDrag;
	float m_rotationalDrag;

const double MIN_LINEAR_THRESHOLD = 0.05;
const double MIN_ROTATION_THRESHOLD = 0.05;
};