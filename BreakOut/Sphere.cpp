#include "Sphere.h"
#include "Box.h"
#include "Plane.h"
#include <glm\ext.hpp>

Sphere::Sphere() : RigidBody(SPHERE, glm::vec2(0, 0), glm::vec2(0, 0), 0, 1, 1, false), m_radius(1)
{
	m_colour = glm::vec4(0, 1, 0, 1);
	m_inertia = 0.5f * m_mass * m_radius * m_radius;
	m_rotationalVelocity = 0;
}
Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float bounciness, float radius, bool makeStatic, glm::vec4* colour) : 
	RigidBody(SPHERE, position, velocity, 0, (makeStatic ? INT_MAX : mass), bounciness, makeStatic), m_radius(radius)
{
	m_colour = (colour != nullptr) ? *(colour) : glm::vec4(0, 1, 0, 1);
	m_inertia = m_isStatic ? INT_MAX : 0.5f * mass * radius * radius;
	m_rotationalVelocity = 0;
}

Sphere::~Sphere() {}


void Sphere::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);
}

void Sphere::draw()
{
	glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius;
	glm::vec2 pos(m_position.x, m_position.y);
	aie::Gizmos::add2DCircle(pos, m_radius, 32, m_colour);

	if (m_colour != glm::vec4(0, 0, 0, 0))
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

	data.contactForce = (distance - (m_radius + sphere.m_radius)) * delta / distance;
	
	sphere.setPositions(*this, data.contactForce);
	return rigidResolve(sphere, 0.5f * (m_position + sphere.m_position), glm::normalize(sphere.m_position - m_position));
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