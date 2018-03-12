#include "Plane.h"
#include "Sphere.h"
#include "Box.h"
#include<glm\ext.hpp>

Plane::Plane() : PhysicsObject(PLANE, true), m_normal(vec2(0, 1)), m_distanceFromOrigin(0), m_bounciness(1)
{ 
	m_planeOrigin = m_normal * m_distanceFromOrigin;
	m_colour = vec4(0, 0, 1, 1); 
}
Plane::Plane(vec2 normal, float distance, float bounciness) : PhysicsObject(PLANE, true), m_normal(normalize(normal)), m_distanceFromOrigin(distance), m_bounciness(bounciness)
{ 
	m_planeOrigin = m_normal * m_distanceFromOrigin;
	m_colour = vec4(0, 0, 1, 1);
}

Plane::Plane(vec2 pointA, vec2 pointB, float bounciness) : PhysicsObject(PLANE, true), m_normal(pointB - pointA), m_distanceFromOrigin(dot(pointA, m_normal)), m_bounciness(bounciness)
{
	m_normal = getRightPerp();

	m_planeOrigin = m_normal * m_distanceFromOrigin;
	m_colour = vec4(0, 0, 1, 1);
}

Plane::~Plane() {}


void Plane::fixedUpdate(vec2 gravity, float timeStep) {}

void Plane::draw()
{
	float lineLength = 300;
	vec2 parallelLine(m_normal.y * lineLength, -m_normal.x * lineLength);

	aie::Gizmos::add2DLine((m_planeOrigin + parallelLine), (m_planeOrigin - parallelLine), m_colour);
}

bool Plane::detectCollision(PhysicsObject& obj)
{
	return obj.detectCollision(CData(), *this);
}

bool Plane::detectCollision(CData& data, Plane& plane)		{ return false; }	//No need to test for plane vs plane

bool Plane::detectCollision(CData& data, Sphere& sphere)	
{ 
	data.setNormal(m_normal);
	float sphereDistance = distanceToPoint(sphere.getPosition());
	if (sphereDistance < 0)
	{
		sphereDistance *= -1;
		data.flipNormal();
	}

	if (sphere.getRadius() - sphereDistance <= 0) return false;

	sphere.setPosition(sphere.getPosition() - m_normal * (sphere.getRadius() - sphereDistance));
	return resolveCollision(&sphere, (data.getNormal() * sphere.getRadius()), sphere.getVelocity());
}

bool Plane::detectCollision(CData& data, Box& box)			
{ 
	vec2 contactVelocity(0, 0);
	float distToCentre = distanceToPoint(box.getPosition());

	for (int i = 0; i < box.corner_Size; i++)
	{
		vec2 corner = box.getCorner(i), localCorner = box.getCornerLocal(i);
		vec2 contactV = box.getVelocity() + box.getRotationalVelocity() * (-localCorner.y * box.getLocalXAxis() + localCorner.x * box.getLocalYAxis());

		float distToCorner = distanceToPoint(corner), velocityIntoPlane = dot(contactV, m_normal);

		if ((distToCorner > 0 && distToCentre < 0 && velocityIntoPlane >= 0) || (distToCorner < 0 && distToCentre > 0 && velocityIntoPlane <= 0))
		{
			data.onContact(corner);
			contactVelocity += contactV;

			if ((distToCentre >= 0 && data.getPenetration() > distToCorner) || (distToCentre < 0 && data.getPenetration() < distToCorner))
				data.setPenetration(distToCorner);
		}
	}


	if (data.noContactsFound()) return false;


	data.divideContact();
	data.setNormal(m_normal);
	float r = dot(data.getContact() - box.getPosition(), getLeftPerp());

	box.setPosition(box.getPosition() - data.getContactForce());
	return resolveCollision(&box, data.getContact() - box.getPosition(), contactVelocity / data.getNumberOfContacts(), r);
}

bool Plane::detectCollision(vec2& point)
{
	if (distanceToPoint(point) == 0) return true;
	else return false;
}

float Plane::distanceToPoint(vec2 point)
{
	return (dot(point, m_normal) - m_distanceFromOrigin);
}

float Plane::getElasticity(RigidBody* obj)
{
	return 1.0f + ((m_bounciness + obj->getBounciness()) / 2.0f);
}


//Private Functions

bool Plane::resolveCollision(RigidBody* rigidBody, vec2 contact, vec2 contactVelocity, float r)
{
	float mass0 = 1.0f / (1.0f / rigidBody->getMass() + (r*r) / rigidBody->getInertia());
	vec2 force = m_normal * (dot(-getElasticity(rigidBody) * contactVelocity, m_normal) * mass0);

	return rigidBody->applyForce(force, contact);
}