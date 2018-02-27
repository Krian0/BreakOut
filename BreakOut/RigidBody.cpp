#include "RigidBody.h"
#include <glm\ext.hpp>
#include <math.h>


RigidBody::RigidBody(ShapeType shapeTypeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float bounciness, float linearDrag, float rotationalDrag, bool isStatic)
	: PhysicsObject(shapeTypeID, bounciness, isStatic), m_position(position), m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_linearDrag(linearDrag), m_rotationalDrag(rotationalDrag) {}
RigidBody::~RigidBody() {}


void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (isStatic())
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

void RigidBody::debug()
{

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

bool RigidBody::rigidResolve(RigidBody& rigid, glm::vec2 contact, glm::vec2* collisionNormal)
{
	float m1 = m_mass, m2 = rigid.getMass();
	glm::vec2 pos1 = m_position, pos2 = rigid.getPosition();

	if (isStatic())	m1 = INFINITY;
	if (rigid.isStatic())	m2 = INFINITY;

	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(pos2 - pos1);
	glm::vec2 perp(normal.y, -normal.x);

	glm::vec2 R(glm::dot(contact - pos1, -perp), glm::dot(contact - pos2, perp));
	glm::vec2 V(glm::dot(m_velocity, normal) - R.x * m_rotationalVelocity, glm::dot(rigid.getVelocity(), normal) + R.y * rigid.getRotVelocity());


	if (V.x <= V.y) return false;

	glm::vec2 M(1.0f / (1.0f / m1 + (R.x * R.x) / m_inertia), 1.0f / (1.0f / m2 + (R.y * R.y) / rigid.getInertia()));
	float elasticity = (m_bounciness + rigid.getBounciness()) / 2.0f;
	glm::vec2 force = (1.0f + elasticity) * M.x * M.y / (M.x + M.y) * (V.x - V.y) * normal;

	return applyForceToActor(rigid, force, contact - pos1, contact - pos2);
}

void RigidBody::setPosition(glm::vec2 position)
{
	m_position = position;
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