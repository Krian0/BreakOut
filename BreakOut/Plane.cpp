#include "Plane.h"
#include "Sphere.h"
#include "Box.h"
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

bool Plane::detectCollision(PhysicsObject& obj)
{
	return obj.detectCollision(CData(), *this);
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

	if (sphere.getRadius() - sphereDistance <= 0) return false;

	sphere.setPosition(sphere.getPosition() - m_normal * (sphere.getRadius() - sphereDistance));
	return resolveCollision(&sphere, (sphere.getPosition() + (data.normal * sphere.getRadius())) );
}

bool Plane::detectCollision(CData& data, Box& box)			
{ 
	float contactV = 0;
	float radius = 0.5f * std::fminf(box.getHalfExtents().x, box.getHalfExtents().y);

	glm::vec2 planeOrigin = m_normal * m_distanceFromOrigin;
	float comFromPlane = glm::dot(box.getPosition() - planeOrigin, m_normal);

	glm::vec2 contactVelocity(0, 0);

	for (int i = 0; i < box.corner_Size; i++)
	{
		glm::vec2 p = box.getCorner(i);
		float distanceFromPlane = glm::dot(p - planeOrigin, m_normal);

		glm::vec2 local = box.getCornerLocal(i);
		// is the minus here correct?
		glm::vec2 cv = box.getVelocity() - box.getRotVelocity() * (-local.y * box.getLocalXAxis() + local.x * box.getLocalYAxis());
		float velocityIntoPlane = glm::dot(cv, m_normal);

		if ((distanceFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane >= 0) || distanceFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane <= 0)
		{
			data.setData(p);
			contactV += velocityIntoPlane;
			contactVelocity += cv;

			if ((comFromPlane >= 0 && data.penetration > distanceFromPlane) || (comFromPlane < 0 && data.penetration < distanceFromPlane))
				data.penetration = distanceFromPlane;
		}
	}

	if (data.numberOfContacts == 0) return false;

	float collisionV = contactV / data.numberOfContacts;
	contactVelocity /= data.numberOfContacts;
	glm::vec2 acceleration = -m_normal * ((1.0f + box.getBounciness()) * collisionV);
	glm::vec2 localContact = (data.contact / data.numberOfContacts);
	float r = glm::dot(localContact - box.getPosition(), glm::vec2(m_normal.y, -m_normal.x));
	//float mass0 = 1.0f / (1.0f / box.getMass() + (r*r) / box.getInertia());


	box.setPosition(box.getPosition() - m_normal * data.penetration);
	//return box.applyForce(acceleration * mass0, localContact);

	return resolveCollision(&box, localContact, &contactVelocity, r);
}

bool Plane::detectCollision(glm::vec2 & point, glm::vec2 & pointOut)
{
	if (distance(point) == 0) return true;
	else return false;
}


//Private Functions

bool Plane::resolveCollision(RigidBody* rigidBody, glm::vec2 contact, glm::vec2* contactVelocity /*= NULL*/, float r /*=0*/)
{
	glm::vec2 velocity = contactVelocity ? *contactVelocity : rigidBody->getVelocity();
	float elasticity = (m_bounciness + rigidBody->getBounciness()) / 2.0f;

	float mass0 = 1.0f / (1.0f / rigidBody->getMass() + (r*r) / rigidBody->getInertia());

	float RHS = glm::dot(-(1 + elasticity) * velocity, m_normal) * mass0;

	return rigidBody->applyForce((m_normal * RHS), contact - rigidBody->getPosition());
}