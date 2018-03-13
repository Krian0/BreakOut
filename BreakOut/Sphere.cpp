#include "Sphere.h"
#include "Box.h"
#include "Plane.h"
#include <glm\ext.hpp>

Sphere::Sphere() : RigidBody(SPHERE, vec2(0, 0), vec2(0, 0), 0, 1, 1, false), m_radius(1)
{
	m_colour = vec4(0, 1, 0, 1);
	m_inertia = 0.5f * m_mass * m_radius * m_radius;
	m_rotationalVelocity = 0;
}
Sphere::Sphere(vec2 position, vec2 velocity, float mass, float bounciness, float radius, vec4 colour, bool makeStatic) : 
	RigidBody(SPHERE, position, velocity, 0, mass, bounciness, makeStatic), m_radius(radius)
{
	m_colour = colour;
	m_inertia = 0.5f * mass * radius * radius;
	m_rotationalVelocity = 0;
}

Sphere::~Sphere() {}


void Sphere::fixedUpdate(vec2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);
}

void Sphere::draw()
{
	vec2 end = vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius;
	vec2 pos(m_position.x, m_position.y);
	aie::Gizmos::add2DCircle(pos, m_radius, 32, m_colour);

	if (m_colour != vec4(0, 0, 0, 0))
		aie::Gizmos::add2DLine(pos, pos + end, vec4(1, 1, 1, 1));
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
	vec2 delta = sphere.m_position - m_position;
	float distance = length(delta);
	float intersection = m_radius + sphere.m_radius - distance;

	if (intersection <= 0) return false;

	data.setContact(0.5f * (m_position + sphere.m_position));
	data.setNormal(glm::normalize(sphere.m_position - m_position));

	sphere.setPositions(*this, (distance - (m_radius + sphere.m_radius)) * delta / distance);
	return rigidResolve(sphere, data.getContact(), data.getNormal());
}

bool Sphere::detectCollision(CData& data, Box& box)
{
	return box.detectCollision(data, *this);
}

bool Sphere::detectCollision(vec2& point)
{
	if (dot(point - m_position, point - m_position) < m_radius * m_radius)
		return true;

	return false;
}