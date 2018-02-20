#include "Box.h"
#include "VecLib\UtilityVec2.h"
#include <vector>

Box::Box(Vector2 position, Vector2 velocity, float rotation, float mass, float halfWidth, float halfHeight) : RigidBody(BOX, position, velocity, rotation, mass)
{
	m_halfExtents.SetVector(halfWidth, halfHeight);
	m_colour = glm::vec4(0, 1, 1, 1);
	m_shapeTypeColour = glm::vec4(0, 0, 1, 1);

	m_bounciness = 1;
	m_inertia = 1.0f / 12.0f * mass * (halfWidth * 2) * (halfHeight * 2);
	m_rotationalVelocity = 0;

	updateVariables();
}
Box::~Box() {}


void Box::fixedUpdate(Vector2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);
	updateVariables();
}

void Box::draw()
{
	Vector2 p1 = getCorner(BoxCorner::TOP_LEFT);
	Vector2 p2 = getCorner(BoxCorner::TOP_RIGHT);
	Vector2 p3 = getCorner(BoxCorner::BOTTOM_RIGHT);
	Vector2 p4 = getCorner(BoxCorner::BOTTOM_LEFT);

	aie::Gizmos::add2DTri(glm::vec2(p1.x,p1.y), glm::vec2(p3.x, p3.y), glm::vec2(p2.x, p2.y), m_colour);
	aie::Gizmos::add2DTri(glm::vec2(p1.x, p1.y), glm::vec2(p4.x, p4.y), glm::vec2(p3.x, p3.y), m_colour);

	aie::Gizmos::add2DCircle(glm::vec2(m_position.x, m_position.y), 1, 4, m_shapeTypeColour);
}

void Box::resetVelocity()
{
	m_velocity.SetVector(0, 0);
}

//Returns corner position (glm::vec2) by enum BoxCorner. TopLeft corner == index 0, TopRight, BottomRight, BottomLeft. Takes enum BoxCorner (id).
Vector2 Box::getCorner(BoxCorner id)
{
	if (id == BoxCorner::TOP_LEFT)
		return m_corners[(int)BoxCorner::TOP_LEFT];

	if (id == BoxCorner::TOP_RIGHT)
		return m_corners[(int)BoxCorner::TOP_RIGHT];

	if (id == BoxCorner::BOTTOM_RIGHT)
		return m_corners[(int)BoxCorner::BOTTOM_RIGHT];
			
	if (id == BoxCorner::BOTTOM_LEFT)
		return m_corners[(int)BoxCorner::BOTTOM_LEFT];

	return Vector2(0,0);
}

Vector2 Box::getProjection(Vector2 axis)
{
	float dot = UV2::dot(m_corners[0], axis);
	Vector2 minMax(dot, dot);

	for (int i = 1; i < 4; i++)
	{
		dot = UV2::dot(m_corners[i], axis);
		if (dot < minMax.x)
			minMax.x = dot;
		else if (dot > minMax.y)
			minMax.y = dot;
	}

	return minMax;
}

void Box::updateVariables()
{
	m_localAxis[0] = Vector2(cosf(m_rotation), sinf(m_rotation));
	m_localAxis[1] = Vector2(-m_localAxis[0].y, m_localAxis[0].x);

	m_corners[0] = m_position - ((m_localAxis[0] * m_halfExtents.x) - (m_localAxis[1] * m_halfExtents.y));
	m_corners[1] = m_position + ((m_localAxis[0] * m_halfExtents.x) + (m_localAxis[1] * m_halfExtents.y));
	m_corners[2] = m_position + ((m_localAxis[0] * m_halfExtents.x) - (m_localAxis[1] * m_halfExtents.y));
	m_corners[3] = m_position - ((m_localAxis[0] * m_halfExtents.x) + (m_localAxis[1] * m_halfExtents.y));
}