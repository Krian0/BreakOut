#include "Sphere.h"


Sphere::Sphere() : RigidBody(SPHERE, glm::vec2(0, 0), glm::vec2(0, 0), 0, 1), m_radius(1)
{
	m_colour = glm::vec4(0, 1, 0, 1);
	m_shapeTypeColour = glm::vec4(0, 1, 0, 1);
}
Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : RigidBody(SPHERE, position, velocity, 0, mass), m_radius(radius)
{
	m_colour = colour;
	m_shapeTypeColour = glm::vec4(0, 1, 0, 1);
}

Sphere::~Sphere()
{
}


void Sphere::draw()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 32, m_colour);
}

void Sphere::resetVelocity()
{
	m_velocity = glm::vec2(0, 0);
}