#include "Plane.h"
#include<glm\ext.hpp>

Plane::Plane() : PhysicsObject(PLANE, true), m_normal(glm::vec2(0, 0)), m_distanceFromOrigin(0) 
{ 
	m_colour = glm::vec4(0, 0, 1, 1); 
	m_shapeTypeColour = glm::vec4(0, 1, 1, 1);
	m_bounciness = 1.0f;
}
Plane::Plane(glm::vec2 normal, float distance, glm::vec4 colour) : PhysicsObject(PLANE, true), m_normal(glm::normalize(normal)), m_distanceFromOrigin(distance) 
{ 
	m_colour = colour;
	m_shapeTypeColour = glm::vec4(0, 1, 1, 1);
	m_bounciness = 1.0f;
}

Plane::~Plane() {}


void Plane::fixedUpdate(glm::vec2 gravity, float timeStep) {}

void Plane::draw()
{
	float lineLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceFromOrigin;
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec2 start = centerPoint + (parallel * lineLength);
	glm::vec2 end = centerPoint - (parallel * lineLength);

	aie::Gizmos::add2DLine(start, end, m_colour);
}

void Plane::resetNormal()
{
	m_normal = glm::vec2(0, 0);
}

void Plane::resetPosition()
{
	m_distanceFromOrigin = 0;
}

bool Plane::resolveCollision(RigidBody * actor2, glm::vec2 contact)
{
	float elasticity = (m_bounciness + actor2->getBounciness()) / 2.0f;
	glm::vec2 force = m_normal * (glm::dot(-(1 + elasticity) * actor2->getVelocity(), m_normal) / (1 / actor2->getMass()));

	return actor2->applyForce(force, contact - actor2->getPosition());
}