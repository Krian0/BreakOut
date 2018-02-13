#include "RigidBody.h"
#include <glm\ext.hpp>
#include <math.h>


RigidBody::RigidBody(ShapeType shapeTypeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float linearDrag, float rotationalDrag)
	: PhysicsObject(shapeTypeID), m_position(position), m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_linearDrag(linearDrag), m_rotationalDrag(rotationalDrag) {}
RigidBody::~RigidBody() {}


void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
		m_velocity = glm::vec2(0, 0);
	if (abs(m_rotationalVelocity) < MIN_ROTATION_THRESHOLD)
		m_rotationalVelocity = 0;

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_rotationalVelocity -= m_rotationalVelocity * m_rotationalDrag * timeStep;

	m_position += m_velocity * timeStep;
	applyForce(gravity * m_mass * timeStep);
}

void RigidBody::debug()
{

}


void RigidBody::applyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void RigidBody::applyForceToActor(RigidBody* actor2, glm::vec2 force)
{
	actor2->applyForce(force);
	applyForce(-force);
}

void RigidBody::resolveCollision(RigidBody* actor2)
{
	glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;
	if (glm::dot(normal, relativeVelocity) > 0)
		return;

	float elasticity = (m_bounciness + actor2->getBounciness()) / 2.0f;
	float impulse = glm::dot(-(1 + elasticity) * relativeVelocity, normal / glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass())) ) );

	applyForceToActor(actor2, normal * impulse);
}


void RigidBody::setLinearDrag(float drag)
{
	drag = fmin(drag, 1);
	drag = fmax(drag, 0);

	m_linearDrag = drag;
}

void RigidBody::setRotationalDrag(float drag)
{
	drag = fmin(drag, 1);
	drag = fmax(drag, 0);

	m_rotationalDrag = drag;
}