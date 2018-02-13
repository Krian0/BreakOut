#include "Plane.h"
#include<glm\ext.hpp>

Plane::Plane() : PhysicsObject(PLANE), m_normal(glm::vec2(0, 0)), m_distanceFromOrigin(0) 
{ 
	m_colour = glm::vec4(0, 0, 1, 1); 
	m_shapeTypeColour = glm::vec4(0, 1, 1, 1);
}
Plane::Plane(glm::vec2 normal, float distance, glm::vec4 colour) : PhysicsObject(PLANE), m_normal(glm::normalize(normal)), m_distanceFromOrigin(distance) 
{ 
	m_colour = colour;
	m_shapeTypeColour = glm::vec4(0, 1, 1, 1);
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

void Plane::resolveCollision(RigidBody * actor2, glm::vec2 planeNormal)
{
	glm::vec2 relativeVelocity = actor2->getVelocity();
	if (glm::dot(planeNormal, relativeVelocity) > 0)
		return;

	float elasticity = (m_bounciness + actor2->getBounciness()) / 2.0f;
	actor2->applyForce((-(1 + elasticity) * glm::dot(relativeVelocity, planeNormal) * planeNormal) * actor2->getMass());
}
