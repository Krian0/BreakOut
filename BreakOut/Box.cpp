#include "Box.h"
#include <vector>

Box::Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float halfWidth, float halfHeight) : RigidBody(BOX, position, velocity, rotation, mass)
{
	m_halfExtents = glm::vec2(halfWidth, halfHeight);
	m_colour = glm::vec4(0, 1, 1, 1);
	m_shapeTypeColour = glm::vec4(0, 0, 1, 1);

	m_bounciness = 1;
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
	glm::vec2 p1 = getCorner(BoxCorner::TOP_LEFT);
	glm::vec2 p3 = getCorner(BoxCorner::BOTTOM_RIGHT);
	aie::Gizmos::add2DTri(p1, p3, getCorner(BoxCorner::TOP_RIGHT), m_colour);
	aie::Gizmos::add2DTri(p1, getCorner(BoxCorner::BOTTOM_LEFT), p3, m_colour);

	aie::Gizmos::add2DCircle(m_position, 1, 4, m_shapeTypeColour);
}

void Box::resetVelocity()
{
	m_velocity = glm::vec2(0, 0);
}

//Returns corner position (glm::vec2) by enum BoxCorner. TopLeft corner == index 0, TopRight, BottomRight, BottomLeft. Takes enum BoxCorner (id).
glm::vec2 Box::getCorner(BoxCorner id)
{
	if (id == BoxCorner::TOP_LEFT)
		return m_corners[(int)BoxCorner::TOP_LEFT];

	if (id == BoxCorner::TOP_RIGHT)
		return m_corners[(int)BoxCorner::TOP_RIGHT];

	if (id == BoxCorner::BOTTOM_RIGHT)
		return m_corners[(int)BoxCorner::BOTTOM_RIGHT];
			
	if (id == BoxCorner::BOTTOM_LEFT)
		return m_corners[(int)BoxCorner::BOTTOM_LEFT];

	return glm::vec2(0,0);
}

glm::vec2 Box::getProjection(glm::vec2 axis)
{
	double min = glm::dot(axis, m_corners[0]);
	double max = min;

	for (int i = 1; i < 4; i++)
	{
		double p = glm::dot(axis, m_corners[i]);

		if (p < min)
			min = p;
		else if (p > max)
			max = p;
	}

	return glm::vec2(min, max);
}

void Box::updateVariables()
{
	m_localAxisX = glm::vec2(cosf(m_rotation), sinf(m_rotation));
	m_localAxisY = glm::vec2(-m_localAxisX.y, m_localAxisX.x);

	m_corners[0] = m_position - ((m_localAxisX * m_halfExtents.x) - (m_localAxisY * m_halfExtents.y));
	m_corners[1] = m_position + ((m_localAxisX * m_halfExtents.x) + (m_localAxisY * m_halfExtents.y));
	m_corners[2] = m_position + ((m_localAxisX * m_halfExtents.x) - (m_localAxisY * m_halfExtents.y));
	m_corners[3] = m_position - ((m_localAxisX * m_halfExtents.x) + (m_localAxisY * m_halfExtents.y));
}