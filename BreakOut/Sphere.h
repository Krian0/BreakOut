#pragma once
#include "RigidBody.h"


class Sphere : public RigidBody
{

public:
	Sphere();
	Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
	~Sphere();

	virtual void draw();
	void resetVelocity();

	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }


protected:

	float m_radius;

};