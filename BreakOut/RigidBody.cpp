#include "RigidBody.h"
#include "VecLib\UtilityVec2.h"
#include <glm\ext.hpp>
#include <math.h>


RigidBody::RigidBody(ShapeType shapeTypeID, Vector2 position, Vector2 velocity, float rotation, float mass, float linearDrag, float rotationalDrag, bool isStatic)
	: PhysicsObject(shapeTypeID, isStatic), m_position(position), m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_linearDrag(linearDrag), m_rotationalDrag(rotationalDrag) {}
RigidBody::~RigidBody() {}


void RigidBody::fixedUpdate(Vector2 gravity, float timeStep)
{
	if (isStatic())
		return;

    m_position += m_velocity * timeStep;
	m_velocity += gravity * timeStep;
	

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_rotation += m_rotationalVelocity * timeStep;
	m_rotationalVelocity -= m_rotationalVelocity * m_rotationalDrag * timeStep;

	if (UV2::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		m_velocity.SetVector(0, 0);
	if (abs(m_rotationalVelocity) < MIN_ROTATION_THRESHOLD)
		m_rotationalVelocity = 0;
}

void RigidBody::debug()
{

}


bool RigidBody::applyForce(Vector2 force, Vector2 pos)
{
	m_velocity += force / m_mass;
	m_rotationalVelocity += (force.y * pos.x - force.x * pos.y) / (m_inertia);

	return true;
}

bool RigidBody::applyForceToActor(RigidBody* actor2, Vector2 force, Vector2 pos1, Vector2 pos2)
{
	applyForce(-force, pos1);
	return actor2->applyForce(force, pos2);
}

bool RigidBody::resolveCollision(RigidBody* actor2, Vector2 contact, Vector2* collisionNormal)
{
	float M1 = m_mass, M2 = actor2->m_mass;

	if (isStatic())
		M1 = INFINITY;
	if (actor2->isStatic())
		M2 = INFINITY;

	Vector2 normal = collisionNormal ? *collisionNormal : UV2::normal(actor2->m_position - m_position);
	Vector2 perp(normal.y, -normal.x);

	Vector2 R(UV2::dot(contact - m_position, -perp), UV2::dot(contact - actor2->m_position, perp));
	Vector2 V(UV2::dot(m_velocity, normal) - R.x * m_rotationalVelocity, UV2::dot(actor2->m_velocity, normal) + R.y * actor2->m_rotationalVelocity);


	if (V.x > V.y)
	{
		Vector2 M(1.0f / (1.0f / M1 + (R.x * R.x) / m_inertia),	1.0f / (1.0f / M2 + (R.y * R.y) / actor2->m_inertia));

		float elasticity = (m_bounciness + actor2->getBounciness()) / 2.0f;
		Vector2 force = (1.0f + elasticity) * M.x * M.y / (M.x + M.y) * (V.x - V.y) * normal;

		return applyForceToActor(actor2, -force, contact - m_position, contact - actor2->m_position);
	}

	return false;
}


void RigidBody::setLinearDrag(float drag)
{
	drag = (float)fmin(drag, 1);
	drag = (float)fmax(drag, 0);

	m_linearDrag = drag;
}

void RigidBody::setRotationalDrag(float drag)
{
	drag = (float)fmin(drag, 1);
	drag = (float)fmax(drag, 0);

	m_rotationalDrag = drag;
}