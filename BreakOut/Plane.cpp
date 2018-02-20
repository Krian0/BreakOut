#include "Plane.h"
#include<glm\ext.hpp>

Plane::Plane() : PhysicsObject(PLANE, true), m_normal(Vector2(0, 1)), m_distanceFromOrigin(0)
{ 
	m_colour = glm::vec4(0, 0, 1, 1); 
	m_shapeTypeColour = glm::vec4(0, 1, 1, 1);
	m_bounciness = 1.0f;
}
Plane::Plane(Vector2 normal, float distance, glm::vec4 colour) : PhysicsObject(PLANE, true), m_normal(UV2::normal(normal)), m_distanceFromOrigin(distance)
{ 
	m_colour = colour;
	m_shapeTypeColour = glm::vec4(0, 1, 1, 1);
	m_bounciness = 1.0f;
}

Plane::~Plane() {}


void Plane::fixedUpdate(Vector2 gravity, float timeStep) {}

void Plane::draw()
{
	float lineLength = 300;
	Vector2 centerPoint = m_normal * m_distanceFromOrigin;
	Vector2 parallel(m_normal.y, -m_normal.x);
	Vector2 start = centerPoint + (parallel * lineLength);
	Vector2 end = centerPoint - (parallel * lineLength);

	aie::Gizmos::add2DLine(glm::vec2(start.x, start.y), glm::vec2(end.x, end.y), m_colour);
}

void Plane::resetNormal()
{
	m_normal.SetVector(0, 0);
}

void Plane::resetPosition()
{
	m_distanceFromOrigin = 0;
}

bool Plane::resolveCollision(RigidBody * actor2, Vector2 contact)
{
	float elasticity = (m_bounciness + actor2->getBounciness()) / 2.0f;
	float RHS = (UV2::dot(-(1 + elasticity) * actor2->getVelocity(), m_normal) / (1 / actor2->getMass()));

	return actor2->applyForce((m_normal * RHS), contact - actor2->getPosition());
}