#include "Box.h"
#include "Sphere.h"
#include<glm\vec2.hpp>
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

bool Box::detectCollision(CData& data, PhysicsObject& obj)
{
	return obj.detectCollision(data, *this);
}

bool Box::detectCollision(CData& data, Plane& plane)
{
	return false;
}

bool Box::detectCollision(CData& data, Sphere& sphere)
{
	float radius = sphere.getRadius();
	glm::vec2 circlePos = sphere.getPosition() - m_position;

	for (int i = 0; i < corner_Size; i++)
	{
		glm::vec2 dp = m_corners[i] - circlePos;
		if (dp.x*dp.x + dp.y*dp.y < radius*radius)
		{
			data.numberOfContacts++;
			data.contact += getCornerLocal(i);
		}
	}

	glm::vec2 p(glm::dot(m_localX, circlePos), glm::dot(m_localY, circlePos));
	if (p.y < m_halfExtents.y && p.y > -m_halfExtents.y && p.x < m_halfExtents.x && p.x > -m_halfExtents.x)
	{
		if (p.x > 0 && p.x <   m_halfExtents.x + radius)  data.setData(glm::vec2( m_halfExtents.x, p.y),  m_localX, (m_halfExtents.x + radius) - p.x);
		if (p.x < 0 && p.x > -(m_halfExtents.x + radius)) data.setData(glm::vec2(-m_halfExtents.x, p.y), -m_localX, (m_halfExtents.x + radius) + p.x);
		if (p.y > 0 && p.y <   m_halfExtents.y + radius)  data.setData(glm::vec2(p.x,  m_halfExtents.y),  m_localY, (m_halfExtents.y + radius) - p.y);
		if (p.y < 0 && p.y > -(m_halfExtents.y + radius)) data.setData(glm::vec2(p.x, -m_halfExtents.y), -m_localY, (m_halfExtents.y + radius) + p.y);
	}

	if (data.numberOfContacts == 0) return false;

	data.contact = m_position + (1.0f / data.numberOfContacts) * (m_localX * data.contact.x + m_localY * data.contact.y);
	
	setPosition(m_position - 0.5f * data.penetration * data.normal);
	sphere.setPosition(sphere.getPosition() + 0.5f * data.penetration * data.normal);
	return rigidResolve(sphere, data.contact, &data.normal);
}

bool Box::detectCollision(CData& data, Box& box)
{
	glm::vec2 contactF1, contactF2;

	checkCorners(box, contactF1, data);
	if (box.checkCorners(*this, contactF2, data))
		data.normal = -data.normal;

	if (data.numberOfContacts == 0) 
		return false;

	data.contactForce = 0.5f * (contactF1 - contactF2);
	
	setPosition(m_position - data.contactForce);
	box.setPosition(box.m_position + data.contactForce);

	return rigidResolve(box, data.contact / data.numberOfContacts, &data.normal);
}

bool Box::detectCollision(glm::vec2& point)
{
	return false;
}


//Private Functions

bool Box::checkCorners(Box& otherBox, glm::vec2& contactForce, CData& data)
{
	data.penetration = 0;

	for (auto corner : otherBox.m_corners)
	{
		glm::vec2 p(glm::dot(corner - m_position, m_localX), glm::dot(corner - m_position, m_localY)); //otherBox's corner in local space of this box

		if (p.y > -m_halfExtents.y && p.y < m_halfExtents.y && p.x > -m_halfExtents.x && p.x < m_halfExtents.x)		//If p is within this box's extents
		{
			if (p.x > 0)	data.setData((m_position + m_halfExtents.x * m_localX + p.y * m_localY),  m_localX, m_halfExtents.x - p.x);	//Set contact, normal and penetration
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