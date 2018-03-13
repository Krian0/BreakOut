#include "RigidBody.h"
#include <glm\ext.hpp>
#include <math.h>


RigidBody::RigidBody(ShapeType shapeTypeID, vec2 position, vec2 velocity, float rotation, float mass, float bounciness, bool isStatic, float linearDrag, float rotationalDrag)
	: PhysicsObject(shapeTypeID, isStatic), m_position(position), m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_linearDrag(linearDrag), m_rotationalDrag(rotationalDrag), m_bounciness(bounciness) {}
RigidBody::~RigidBody() {}


void RigidBody::fixedUpdate(vec2 gravity, float timeStep)
{
	if (m_isStatic)
		return;

	m_position += m_velocity * timeStep;
	m_velocity += gravity * timeStep;

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_rotation += m_rotationalVelocity * timeStep;
	m_rotationalVelocity -= m_rotationalVelocity * m_rotationalDrag * timeStep;

	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		m_velocity = vec2(0, 0);
	if (abs(m_rotationalVelocity) < MIN_ROTATION_THRESHOLD)
		m_rotationalVelocity = 0;
}


bool RigidBody::applyForce(vec2 force, vec2 pos)
{
	if (m_isStatic) return false;
	m_velocity += force / m_mass;
	m_rotationalVelocity += (force.y * pos.x - force.x * pos.y) / (m_inertia);

	return true;
}

bool RigidBody::applyForceToActor(RigidBody& actor2, vec2 force, vec2 pos1, vec2 pos2)
{
	applyForce(-force, pos1);
	return actor2.applyForce(force, pos2);
}

bool RigidBody::rigidResolve(RigidBody& rigid, vec2 contact, vec2 normal)
{
	vec2 perp(normal.y, -normal.x);
	float r1 = dot(contact - m_position, -perp);
	float r2 = dot(contact - rigid.getPosition(), perp);

	// work out how fast each object's contact point is moving into the normal - adding up linear and rotational velocities
	float v1 = dot(m_velocity, normal) - r1 * m_rotationalVelocity;
	float v2 = dot(rigid.getVelocity(), normal) + r2 * rigid.getRotationalVelocity();

	// if the contact points are not moving into each other, we're done
	if (v1 <= v2) return false;

	// calculate "effective mass" of each object
	// this takes into account how much the contact point will rotate away when the force is applied as well
	// as the linear acceleration created
	float mass1 = 1.0f / (1.0f / getMass() + (r1*r1) / getInertia());
	float mass2 = 1.0f / (1.0f / rigid.getMass() + (r2*r2) / rigid.getInertia());

	// this is the force required to stop the contact points moving into each other, with elasticity accounted for
	vec2 force = getElasticity(&rigid) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

	// apply equal and opposite forces
	applyForce(-force, contact - m_position);
	rigid.applyForce(force, contact - rigid.getPosition());

	return true;
}

void RigidBody::setPosition(vec2 position)
{
	m_position = position;
}

void RigidBody::setPositions(RigidBody& otherRigid, vec2 contactForce)
{
	if (!m_isStatic && !otherRigid.m_isStatic)
	{
		m_position -= (0.5f * contactForce);
		otherRigid.m_position += (0.5f * contactForce);
	}

	else if (!m_isStatic) 
		m_position -= contactForce;
	else
		otherRigid.m_position += contactForce;
}

float RigidBody::getElasticity(RigidBody* obj)
{
	return 1.0f + ((m_bounciness + obj->getBounciness()) / 2.0f);
}