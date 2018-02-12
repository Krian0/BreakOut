#pragma once
#include "RigidBody.h"

class Box : public RigidBody
{

public:
	Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float halfWidth, float halfHeight);
	~Box();


	virtual void draw();
	void resetVelocity();

	glm::vec2 getCorner(unsigned int index);
	glm::vec2 getHalfExtents() { return m_halfExtents; }
	glm::vec4 getColour() { return m_colour; }


protected:
	glm::vec2 getCornerPosition(glm::vec2 cornerPosition);
	glm::vec2 m_halfExtents;
};