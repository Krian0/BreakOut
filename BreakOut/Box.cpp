#include "Box.h"
#include "VecLib\UtilityVec2.h"
#include<glm\vec2.hpp>

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
	glm::vec2 p1(m_corners[0].x, m_corners[0].y);
	glm::vec2 p3(m_corners[2].x, m_corners[2].y);

	aie::Gizmos::add2DTri(p1, p3, glm::vec2(m_corners[1].x, m_corners[1].y), m_colour);
	aie::Gizmos::add2DTri(p1, glm::vec2(m_corners[3].x, m_corners[3].y), p3, m_colour);

	aie::Gizmos::add2DCircle(glm::vec2(m_position.x, m_position.y), 1, 4, m_shapeTypeColour);
}

void Box::resetVelocity()
{
	m_velocity.SetVector(0, 0);
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