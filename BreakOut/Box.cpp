#include "Box.h"
#include "Plane.h"
#include "Sphere.h"
#include<glm\ext.hpp>

Box::Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float bounciness, float halfWidth, float halfHeight) : RigidBody(BOX, position, velocity, rotation, mass, bounciness)
{
	m_halfExtents= glm::vec2(halfWidth, halfHeight);
	m_colour = glm::vec4(0, 1, 1, 1);
	m_shapeTypeColour = glm::vec4(0, 0, 1, 1);

	m_inertia = 1.0f / 12.0f * mass * (halfWidth * 2) * (halfHeight * 2);
	m_rotationalVelocity = 0;

	updateVariables();
}
Box::~Box() {}


void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);
	updateVariables();
}

void Box::draw()
{
	glm::vec2 p1(m_corners[0]);
	glm::vec2 p2(m_corners[1]);
	glm::vec2 p3(m_corners[2]);
	glm::vec2 p4(m_corners[3]);

	aie::Gizmos::add2DTri(p1, p3, p2, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);

	aie::Gizmos::add2DCircle(m_position, 1, 4, m_colour);
}

bool Box::detectCollision(PhysicsObject& obj)
{
	return obj.detectCollision(CData(), *this);
}

bool Box::detectCollision(CData& data, Plane& plane)
{
	return plane.detectCollision(data, *this);
}

bool Box::detectCollision(CData& data, Sphere& sphere)
{
	for (int i = 0; i < corner_Size; i++)
		if (sphere.detectCollision(m_corners[i], glm::vec2()))				//Do a sphere vs point check with each corner, set data if it collides
			data.setData(getCornerLocal(i), glm::vec2(), 0, false);


	glm::vec2 p;
	float radius = sphere.getRadius();
	if (detectCollision(sphere.getPosition(), p))
	{
		if (p.x > 0 && p.x <   m_halfExtents.x + radius)  data.setData(glm::vec2( m_halfExtents.x, p.y),  m_localX, (m_halfExtents.x + radius) - p.x);
		if (p.x < 0 && p.x > -(m_halfExtents.x + radius)) data.setData(glm::vec2(-m_halfExtents.x, p.y), -m_localX, (m_halfExtents.x + radius) + p.x);
		if (p.y > 0 && p.y <   m_halfExtents.y + radius)  data.setData(glm::vec2(p.x,  m_halfExtents.y),  m_localY, (m_halfExtents.y + radius) - p.y);
		if (p.y < 0 && p.y > -(m_halfExtents.y + radius)) data.setData(glm::vec2(p.x, -m_halfExtents.y), -m_localY, (m_halfExtents.y + radius) + p.y);
	}

	if (data.numberOfContacts == 0) return false;

	data.contact = m_position + (1.0f / data.numberOfContacts) * (m_localX * data.contact.x + m_localY * data.contact.y);
	data.contactForce = 0.5f * data.penetration * data.normal;
	

	setPositions(sphere, data.contactForce);
	return rigidResolve(sphere, data.contact, &data.normal);
}

bool Box::detectCollision(CData& data, Box& box)
{
	glm::vec2 contactF1, contactF2;

	checkCorners(box, contactF1, data);
	if (box.checkCorners(*this, contactF2, data))
		data.normal = -data.normal;

	if (data.numberOfContacts == 0) return false;

	data.contactForce = 0.5f * (contactF1 - contactF2);
	
	setPositions(box, data.contactForce);
	return rigidResolve(box, data.contact / data.numberOfContacts, &data.normal);
}

bool Box::detectCollision(glm::vec2& point, glm::vec2& pointOut)
{
	pointOut = glm::vec2(glm::dot(point - m_position, m_localX), glm::dot(point - m_position, m_localY)); //Point in local space of this box

	if (pointOut.y > -m_halfExtents.y && pointOut.y < m_halfExtents.y && pointOut.x > -m_halfExtents.x && pointOut.x < m_halfExtents.x)		//If p is within this box's extents
		return true;

	return false;
}


//Private Functions

bool Box::checkCorners(Box& otherBox, glm::vec2& contactForce, CData& data)
{
	data.penetration = 0;

	for (auto corner : otherBox.m_corners)
	{
		glm::vec2 p;
		if (detectCollision(corner, p))		//If p (otherBox corner in local space of this box) is within the extents of this box...
		{
			if (p.x > 0)	data.setData((m_position + m_halfExtents.x * m_localX + p.y * m_localY),  m_localX, m_halfExtents.x - p.x);	//Set contact, normal and penetration respectively
			if (p.x < 0)	data.setData((m_position - m_halfExtents.x * m_localX + p.y * m_localY), -m_localX, m_halfExtents.x + p.x);
			if (p.y > 0)	data.setData((m_position + p.x * m_localX + m_halfExtents.y * m_localY),  m_localY, m_halfExtents.y - p.y, (m_halfExtents.y - p.y < data.penetration || data.penetration == 0));
			if (p.y < 0)	data.setData((m_position + p.x * m_localX - m_halfExtents.y * m_localY), -m_localY, m_halfExtents.y + p.y, (m_halfExtents.y + p.y < data.penetration || data.penetration == 0));
		}
	}

	contactForce = data.penetration * data.normal;
	return (data.penetration != 0);
}

void Box::updateVariables()
{
	m_localX = glm::vec2(cosf(m_rotation), sinf(m_rotation));
	m_localY = glm::vec2(-m_localX.y, m_localX.x);

	m_corners[0] = m_position - ((m_localX * m_halfExtents.x) - (m_localY * m_halfExtents.y));
	m_corners[1] = m_position + ((m_localX * m_halfExtents.x) + (m_localY * m_halfExtents.y));
	m_corners[2] = m_position + ((m_localX * m_halfExtents.x) - (m_localY * m_halfExtents.y));
	m_corners[3] = m_position - ((m_localX * m_halfExtents.x) + (m_localY * m_halfExtents.y));
}