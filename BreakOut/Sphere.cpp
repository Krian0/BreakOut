#include "Sphere.h"
#include <glm\vec2.hpp>


Sphere::Sphere() : RigidBody(SPHERE, Vector2(0, 0), Vector2(0, 0), 0, 1), m_radius(1)
{
	m_colour = glm::vec4(0, 1, 0, 1);
	m_shapeTypeColour = glm::vec4(0, 1, 0, 1);
	m_bounciness = 1.0f;
	m_inertia = 0.5f * 1 * 1 * 1;
	m_rotation = 0;
	m_rotationalVelocity = 0;
}
Sphere::Sphere(Vector2 position, Vector2 velocity, float mass, float radius, glm::vec4 colour) : RigidBody(SPHERE, position, velocity, 0, mass), m_radius(radius)
{
	m_colour = colour;
	m_shapeTypeColour = glm::vec4(0, 1, 0, 1);
	m_bounciness = 1.0f;
	m_inertia = 0.5f * mass * radius * radius;
	m_rotation = 0;
	m_rotationalVelocity = 0;
}

Sphere::~Sphere()
{
}


void Sphere::fixedUpdate(Vector2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);
}

void Sphere::draw()
{
	glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius;
	glm::vec2 pos(m_position.x, m_position.y);
	aie::Gizmos::add2DCircle(pos, m_radius, 32, m_colour);
	aie::Gizmos::add2DLine(pos, pos + end, glm::vec4(1, 1, 1, 1));
}

void Sphere::resetVelocity()
{
	m_velocity.SetVector(0, 0);
}