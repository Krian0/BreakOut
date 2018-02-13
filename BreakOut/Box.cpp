#include "Box.h"
#include <vector>

Box::Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float halfWidth, float halfHeight) : RigidBody(BOX, position, velocity, 0, mass)
{
	m_halfExtents = glm::vec2(halfWidth, halfHeight);
}
Box::~Box() {}


void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);

	m_localAxisX = glm::vec2(cosf(m_rotation), sinf(m_rotation));
	m_localAxisY = glm::vec2(-m_localAxisX.y, m_localAxisX.x);
}

void Box::draw()
{
	glm::vec2 p1 = getCorner(BoxCorner::TOP_LEFT);
	glm::vec2 p4 = getCorner(BoxCorner::BOTTOM_RIGHT);
	aie::Gizmos::add2DTri(p1, getCorner(BoxCorner::TOP_RIGHT), p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, getCorner(BoxCorner::BOTTOM_RIGHT), m_colour);
}

void Box::resetVelocity()
{
	m_velocity = glm::vec2(0, 0);
}

//Returns corner position (glm::vec2) by enum BoxCorner. TopLeft corner == index 0, TopRight, BottomLeft, BottomRight. Takes enum BoxCorner (corner).
glm::vec2 Box::getCorner(BoxCorner corner)
{
	if (corner == BoxCorner::TOP_LEFT)
		return m_position - ((m_localAxisX * m_halfExtents.x) - (m_localAxisY * m_halfExtents.y));

	if (corner == BoxCorner::TOP_RIGHT)
		return  m_position + ((m_localAxisX * m_halfExtents.x) - (m_localAxisY * m_halfExtents.y));

	if (corner == BoxCorner::BOTTOM_RIGHT)
		return  m_position + ((m_localAxisX * m_halfExtents.x) + (m_localAxisY * m_halfExtents.y));
			
	if (corner == BoxCorner::BOTTOM_LEFT)
		return  m_position - ((m_localAxisX * m_halfExtents.x) + (m_localAxisY * m_halfExtents.y));

	return glm::vec2(0,0);
}