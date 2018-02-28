#include "Sphere.h"
#include "Box.h"
#include "Plane.h"
#include <glm\ext.hpp>

Sphere::Sphere() : RigidBody(SPHERE, glm::vec2(0, 0), glm::vec2(0, 0), 0, 1, 1), m_radius(1)
{
	m_colour = glm::vec4(0, 1, 0, 1);
	m_shapeTypeColour = glm::vec4(0, 1, 0, 1);
	m_bounciness = 1.0f;
	m_inertia = 0.5f * 1 * 1 * 1;
	m_rotation = 0;
	m_rotationalVelocity = 0;
}
Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float bounciness, float radius, glm::vec4 colour) : RigidBody(SPHERE, position, velocity, 0, mass, bounciness), m_radius(radius)
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


void Sphere::fixedUpdate(glm::vec2 gravity, float timeStep)
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

bool Sphere::detectCollision(PhysicsObject& obj)
{
	return obj.detectCollision(CData(), *this);
}

bool Sphere::detectCollision(CData& data, Plane& plane)
{
	return plane.detectCollision(data, *this);
}

bool Sphere::detectCollision(CData& data, Sphere& sphere)
{
	glm::vec2 delta = sphere.m_position - m_position;
	float distance = glm::length(delta);
	float intersection = m_radius + sphere.m_radius - distance;

	if (intersection <= 0) return false;

	data.contactForce = 0.5f * (distance - (m_radius + sphere.m_radius)) * delta / distance;
	
	sphere.setPositions(*this, data.contactForce);
	return rigidResolve(sphere, 0.5f * (m_position + sphere.m_position), NULL);
}

bool Sphere::detectCollision(CData& data, Box& box)
{
	return box.detectCollision(data, *this);
}

bool Sphere::detectCollision(glm::vec2& point, glm::vec2& pointOut)
{
	pointOut = point - m_position;
	if (glm::dot(pointOut, pointOut) < m_radius * m_radius)
		return true;

	return false;
}