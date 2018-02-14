#include "RigidBody.h"
#include <glm\ext.hpp>
#include <math.h>


RigidBody::RigidBody(ShapeType shapeTypeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float linearDrag, float rotationalDrag, bool isStatic)
	: PhysicsObject(shapeTypeID, isStatic), m_position(position), m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_linearDrag(linearDrag), m_rotationalDrag(rotationalDrag) {}
RigidBody::~RigidBody() {}


void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (isStatic())
		return;

    m_position += m_velocity * timeStep;
	m_velocity += gravity * timeStep;
	

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_rotation += m_rotationalVelocity * timeStep;
	m_rotationalVelocity -= m_rotationalVelocity * m_rotationalDrag * timeStep;

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
		m_velocity = glm::vec2(0, 0);
	if (abs(m_rotationalVelocity) < MIN_ROTATION_THRESHOLD)
		m_rotationalVelocity = 0;

	m_rotationalVelocity -= m_rotationalVelocity * m_rotationalDrag * timeStep;

}

void RigidBody::debug()
{

}


bool RigidBody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / m_mass;
	m_rotationalVelocity += (force.y * pos.x - force.x * pos.y) / m_inertia;

	return true;
}

bool RigidBody::applyForceToActor(RigidBody* actor2, glm::vec2 force, glm::vec2 pos)
{
	actor2->applyForce(force, pos);
	return applyForce(-force, pos);
}

bool RigidBody::resolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	float M1 = m_mass, M2 = actor2->m_mass;

	if (isStatic())
		M1 = INFINITY;
	if (actor2->isStatic())
		M2 = INFINITY;


	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(actor2->m_position - m_position);
	glm::vec2 perp(normal.y, -normal.x);

	glm::vec2 R(glm::dot(contact - m_position, -perp),					glm::dot(contact - actor2->m_position, perp));
	glm::vec2 V(glm::dot(m_velocity, normal) - R.x * m_rotationalVelocity,		glm::dot(actor2->m_velocity, normal) + R.y * actor2->m_rotationalVelocity);


	if (V.x > V.y)
	{
		glm::vec2 M(1.0f / (1.0f / M1 + (R.x * R.x) / m_inertia),	1.0f / (1.0f / M2 + (R.y * R.y) / actor2->m_inertia));
		
		float elasticity = (m_bounciness + actor2->getBounciness()) / 2.0f;
		glm::vec2 force = (1.0f + elasticity) * M.x * M.y / (M.x + M.y) * (V.x - V.y) * normal;

		//float keBefore = this-> GetEnergy() + actor2->GetEnergy();

		bool ret  = applyForceToActor(actor2, -force, contact - m_position);

		//float keAfter = this-> GetEnergy() + actor2->GetEnergy();

		return ret;
	}

	//glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;
	//if (glm::dot(normal, relativeVelocity) > 0)
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