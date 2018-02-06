#include <glm\ext.hpp>

#include "Sphere.h"



Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : RigidBody(SPHERE, position, velocity, 0, mass), m_radius(radius)
{
	m_colour = colour;
}

Sphere::~Sphere()
{
}


void Sphere::draw()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 32, m_colour);
}

bool Sphere::checkCollision(PhysicsObject* otherActor)
{
	Sphere* spherePointer;
	if ((spherePointer = dynamic_cast<Sphere*>(otherActor)) != nullptr)
		return glm::distance(m_position, spherePointer->m_position) < (m_radius + spherePointer->m_radius);
}