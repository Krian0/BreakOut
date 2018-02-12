#include "Box.h"
#include <vector>

Box::Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float halfWidth, float halfHeight) : RigidBody(BOX, position, velocity, 0, mass)
{
	m_halfExtents = glm::vec2(halfWidth, halfHeight);
}
Box::~Box() {}


void Box::draw()
{
	aie::Gizmos::add2DAABBFilled(m_position, m_halfExtents, m_colour);
}

void Box::resetVelocity()
{
	m_velocity = glm::vec2(0, 0);
}

//Returns corner position (glm::vec2) by index. TopLeft corner == index 0, TopRight, BottomLeft, BottomRight. Takes unsigned int parameter (index).
glm::vec2 Box::getCorner(unsigned int index)
{
	if (index == 0)
		return getCornerPosition(glm::vec2(m_position.x - m_halfExtents.x, m_position.y + m_halfExtents.y));

	if (index == 1)
		return getCornerPosition(glm::vec2(m_position + m_halfExtents));

	if (index == 2)
		return getCornerPosition(glm::vec2(m_position - m_halfExtents));
			
	if (index >= 3)
		return getCornerPosition(glm::vec2(m_position.x + m_halfExtents.x, m_position.y - m_halfExtents.y));
}

//Returns rotated corner position (glm::vec2). Takes glm::vec2 parameter (cornerPosition).
glm::vec2 Box::getCornerPosition(glm::vec2 cornerPosition)
{
	if (m_rotation == 0)
		return cornerPosition;

	float xPointToOrigin = cornerPosition.x - m_position.x;
	float yPointToOrigin = cornerPosition.y - m_position.y;

	float xPosRotated = (xPointToOrigin * cos(m_rotation)) - (yPointToOrigin * sin(m_rotation));
	float yPosRotated = (xPointToOrigin * sin(m_rotation)) + (yPointToOrigin * cos(m_rotation));

	return glm::vec2(xPosRotated + m_position.x, yPosRotated + m_position.y);
}