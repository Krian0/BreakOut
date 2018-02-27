#include "Plane.h"
#include "Sphere.h"
#include "Box.h"
#include<glm\vec2.hpp>
#include<glm\ext.hpp>

Plane::Plane() : PhysicsObject(PLANE, 1, true), m_normal(glm::vec2(0, 1)), m_distanceFromOrigin(0)
{ 
	m_colour = glm::vec4(0, 0, 1, 1); 
	m_shapeTypeColour = glm::vec4(0, 1, 1, 1);
}
Plane::Plane(glm::vec2 normal, float distance, float bounciness, glm::vec4 colour) : PhysicsObject(PLANE, bounciness, true), m_normal(glm::normalize(normal)), m_distanceFromOrigin(distance)
{ 
	m_colour = colour;
	m_shapeTypeColour = glm::vec4(0, 1, 1, 1);
}

Plane::Plane(glm::vec2 pointA, glm::vec2 pointB, float bounciness, glm::vec4 colour) : PhysicsObject(PLANE, bounciness, true)
{
	glm::vec2 n = glm::normalize(pointB - pointA);
	m_normal = glm::vec2(-n.y, n.x);
	m_distanceFromOrigin = glm::dot(pointA, n);

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

	aie::Gizmos::add2DLine(glm::vec2(start.x, start.y), glm::vec2(end.x, end.y), m_colour);
}

bool Plane::detectCollision(CData& data, PhysicsObject& obj)
{
	return obj.detectCollision(data, *this);
}

bool Plane::detectCollision(CData& data, Plane& plane)		{ return false; }	//No need to test for plane vs plane
bool Plane::detectCollision(CData& data, Sphere& sphere)	
{ 
	data.normal = m_normal;
	float sphereDistance = distance(sphere.getPosition());
	if (sphereDistance < 0)
	{
		sphereDistance *= -1;
		data.normal *= -1;
	}

	data.penetration = sphere.getRadius() - sphereDistance;
	if (data.penetration <= 0) return false;

	data.contact = sphere.getPosition() + (data.normal * sphere.getRadius());

	sphere.setPosition(sphere.getPosition() - m_normal * (sphere.getRadius() - sphereDistance));
	return resolveCollision(&sphere, data);
}
bool Plane::detectCollision(CData& data, Box& box)			
{ 
	glm::vec2 planeOrigin = m_normal * m_distanceFromOrigin;
	return box.detectCollision(data, *this); 
}
bool Plane::detectCollision(glm::vec2& point)
{
	if (distance(point) == 0) return true;
	else return false;
}

bool Plane::resolveCollision(RigidBody* rigidBody, CData& data)
{
	float elasticity = (m_bounciness + rigidBody->getBounciness()) / 2.0f;
	float RHS = (glm::dot(-(1 + elasticity) * rigidBody->getVelocity(), m_normal) / (1 / rigidBody->getMass()));

	return rigidBody->applyForce((m_normal * RHS), data.contact - rigidBody->getPosition());
}