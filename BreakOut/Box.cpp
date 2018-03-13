#include "Box.h"
#include "Plane.h"
#include "Sphere.h"
#include<glm\ext.hpp>

Box::Box() : RigidBody(BOX, vec2(0, 0), vec2(0, 0), 0, 1, 1, false), m_halfExtents(vec2(2, 2))
{
	m_colour = vec4(0, 0, 1, 1);

	m_inertia = 1.0f / 12.0f * 1 * dot(m_halfExtents, m_halfExtents);
	m_rotationalVelocity = 0;

	updateVariables();
	m_cornerExtent[0] = vec2(-m_halfExtents.x, m_halfExtents.y);
	m_cornerExtent[1] = m_halfExtents;
	m_cornerExtent[2] = vec2(m_halfExtents.x, -m_halfExtents.y);
	m_cornerExtent[3] = -m_halfExtents;
}

Box::Box(vec2 position, vec2 velocity, float rotation, float mass, float bounciness, vec2 halfExtents, vec4 colour, bool makeStatic) :
	RigidBody(BOX, position, velocity, rotation, mass, bounciness, makeStatic), m_halfExtents(halfExtents)
{
	m_colour = colour;

	m_inertia = 1.0f / 12.0f * mass * dot(halfExtents, halfExtents);
	m_rotationalVelocity = 0;

	updateVariables();
	m_cornerExtent[0] = vec2(-halfExtents.x, halfExtents.y);
	m_cornerExtent[1] = halfExtents;
	m_cornerExtent[2] = vec2(halfExtents.x, -halfExtents.y);
	m_cornerExtent[3] = -halfExtents;
}
Box::~Box() {}


void Box::fixedUpdate(vec2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);
	updateVariables();
}

void Box::draw()
{
	aie::Gizmos::add2DTri(m_corners[0], m_corners[2], m_corners[1], m_colour);
	aie::Gizmos::add2DTri(m_corners[0], m_corners[3], m_corners[2], m_colour);
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
	for (int i = 0; i < CORNER_SIZE; i++)
		if (sphere.detectCollision(m_corners[i]))
		{
			//Do a sphere vs point check with each corner, set contact if it collides
			float pen = sphere.getRadius() - length(m_corners[i] - sphere.getPosition());
			vec2 normal = normalize(sphere.getPosition() - m_corners[i]);
			data.onContact(getCornerLocal(i), normal, pen);
		}

	vec2 p(dot(sphere.getPosition() - m_position, m_localX), dot(sphere.getPosition() - m_position, m_localY));
	float radius = sphere.getRadius();

	vec2 extents = m_halfExtents + radius;
	
	if (p.y < m_halfExtents.y && p.y > -m_halfExtents.y)
	{
		if (p.x > 0 && p.x < extents.x)
			data.onContact(vec2(m_halfExtents.x, p.y), m_localX, extents.x - p.x);
		if (p.x < 0 && p.x > -extents.x)
			data.onContact(vec2(-m_halfExtents.x, p.y), -m_localX, extents.x + p.x);
	}
	if (p.x < m_halfExtents.x && p.x > -m_halfExtents.x)
	{
		if (p.y > 0 && p.y < extents.y)
			data.onContact(vec2(p.x, m_halfExtents.y), m_localY, extents.y - p.y);
		if (p.y < 0 && p.y > -extents.y)
			data.onContact(vec2(p.x, -m_halfExtents.y), -m_localY, extents.y + p.y);
	}

	if (data.noContactsFound()) return false;

	data.setContact(m_position + (1.0f / data.getNumberOfContacts()) * (m_localX * data.getContact().x + m_localY * data.getContact().y));
	

	setPositions(sphere, data.getContactForce());
	return rigidResolve(sphere, data.getContact(), data.getNormal());
}

bool Box::detectCollision(CData& data, Box& box)
{
	vec2 contactF1, contactF2;

	checkCorners(box, contactF1, data);
	if (box.checkCorners(*this, contactF2, data))
		data.flipNormal();

	if (data.noContactsFound()) return false;

	data.divideContact();
	setPositions(box, (contactF1 - contactF2));
	return rigidResolve(box, data.getContact(), data.getNormal());
}

bool Box::detectCollision(vec2& point, vec2& pointOut)
{
	pointOut = vec2(dot(point - m_position, m_localX), dot(point - m_position, m_localY));							//Point in local space of this box

	if (pointOut.y > -m_halfExtents.y && pointOut.y < m_halfExtents.y && pointOut.x > -m_halfExtents.x && pointOut.x < m_halfExtents.x)
		return true;

	return false;
}


//Private Functions

bool Box::checkCorners(Box& otherBox, vec2& contactForce, CData& data)
{
	data.setPenetration(0);

	for (auto corner : otherBox.m_corners)
	{
		vec2 p;
		if (!detectCollision(corner, p)) continue;
		float penX1 = m_halfExtents.x - p.x,	penX2 = m_halfExtents.x + p.x,		penY1 = m_halfExtents.y - p.y,		penY2 = m_halfExtents.y + p.y;

		if (p.x > 0)
			data.onContact((m_position + m_halfExtents.x * m_localX + p.y * m_localY), m_localX, penX1);
		if (p.x < 0)
			data.onContact((m_position - m_halfExtents.x * m_localX + p.y * m_localY), -m_localX, penX2);
		if (p.y > 0)
		{
			data.onContact(m_position + p.x * m_localX + m_halfExtents.y * m_localY);
			if (penY1 < data.getPenetration() || data.getPenetration() == 0)
				data.setNormalAndPenetration(m_localY, penY1);
		}
		if (p.y < 0)
		{
			data.onContact(m_position + p.x * m_localX - m_halfExtents.y * m_localY);
			if (penY2 < data.getPenetration() || data.getPenetration() == 0)
				data.setNormalAndPenetration(-m_localY, penY2);
		}
	}

	contactForce = data.getContactForce();
	return (data.getPenetration() != 0);
}

//Set the local X and Y axes, update the position of each corner
void Box::updateVariables()
{
	m_localX = vec2(cosf(m_rotation), sinf(m_rotation));
	m_localY = vec2(-m_localX.y, m_localX.x);

	m_corners[0] = m_position - ((m_localX * m_halfExtents.x) - (m_localY * m_halfExtents.y));
	m_corners[1] = m_position + ((m_localX * m_halfExtents.x) + (m_localY * m_halfExtents.y));
	m_corners[2] = m_position + ((m_localX * m_halfExtents.x) - (m_localY * m_halfExtents.y));
	m_corners[3] = m_position - ((m_localX * m_halfExtents.x) + (m_localY * m_halfExtents.y));
}