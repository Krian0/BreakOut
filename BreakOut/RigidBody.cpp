#include "RigidBody.h"
#include <glm\ext.hpp>
#include <math.h>


RigidBody::RigidBody(ShapeType shapeTypeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float bounciness, bool isStatic, float linearDrag, float rotationalDrag)
	: PhysicsObject(shapeTypeID, bounciness, isStatic), m_position(position), m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_linearDrag(linearDrag), m_rotationalDrag(rotationalDrag) {}
RigidBody::~RigidBody() {}


void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (m_isStatic)
		return;

    m_position += m_velocity * timeStep;
	m_velocity += gravity * timeStep;
	

	m_velocity -= m_velocity * (m_linearDrag * timeStep);
	m_rotation += m_rotationalVelocity * timeStep;
	m_rotationalVelocity -= m_rotationalVelocity * m_rotationalDrag * timeStep;

	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		m_velocity = glm::vec2(0, 0);
	if (abs(m_rotationalVelocity) < MIN_ROTATION_THRESHOLD)
		m_rotationalVelocity = 0;
}


bool RigidBody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / m_mass;
	m_rotationalVelocity += (force.y * pos.x - force.x * pos.y) / (m_inertia);

	return true;
}

bool RigidBody::applyForceToActor(RigidBody& actor2, glm::vec2 force, glm::vec2 pos1, glm::vec2 pos2)
{
	applyForce(-force, pos1);
	return actor2.applyForce(force, pos2);
}

bool RigidBody::rigidResolve(RigidBody& rigid, glm::vec2 contact, glm::vec2 normal)
{
	glm::vec2 perp(normal.y, -normal.x);
	float r1 = glm::dot(contact - m_position, -perp),						r2 = glm::dot(contact - rigid.getPosition(), perp);
	float v1 = glm::dot(m_velocity, normal) - r1 * m_rotationalVelocity,	v2 = glm::dot(rigid.getVelocity(), normal) + r2 * rigid.getRotVelocity();

	if (v1 <= v2) return false;

	float mass1 = 1.0f / (1.0f / m_mass + (r1*r1) / m_inertia),		mass2 = 1.0f / (1.0f / rigid.getMass() + (r2*r2) / rigid.getInertia());
	glm::vec2 force = getElasticity(&rigid) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

	return applyForceToActor(rigid, force, contact - m_position, contact - rigid.getPosition());
}

void RigidBody::setPosition(glm::vec2 position)
{
	m_position = position;
}

void RigidBody::setPositions(RigidBody& otherRigid, glm::vec2 contactForce)
{
	if (!m_isStatic && !otherRigid.m_isStatic)
		contactForce *= 0.5f;

	if (!m_isStatic) 
		m_position -= contactForce;
	if (!otherRigid.m_isStatic) 
		otherRigid.m_position += contactForce;
}