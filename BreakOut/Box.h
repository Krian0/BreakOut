#pragma once
#include "RigidBody.h"
#include <glm\ext.hpp>

enum class BoxCorner
{
	TOP_LEFT = 0,
	TOP_RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT
};

class Box : public RigidBody
{

public:
	Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float halfWidth, float halfHeight);
	~Box();


	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void draw();
	void resetVelocity();

	glm::vec2 getCorner(BoxCorner corner);
	glm::vec2 getHalfExtents() { return m_halfExtents; }
	glm::vec2 getLocalXAxis() { return m_localAxisX; }
	glm::vec2 getLocalYAxis() { return m_localAxisY; }
	glm::vec4 getColour() { return m_colour; }


protected:
	glm::vec2 m_halfExtents;
	glm::vec2 m_localAxisX;
	glm::vec2 m_localAxisY;
};