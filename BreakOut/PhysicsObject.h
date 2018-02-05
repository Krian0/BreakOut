#pragma once
#include<glm\vec2.hpp>
#include<glm\vec4.hpp>
#include<Gizmos.h>

enum ShapeType 
{
	PLANE = 0,
	SPHERE,
	BOX,
	SPRING
};

class PhysicsObject
{

protected:
	PhysicsObject(ShapeType shapeTypeID) : m_shapeID(shapeTypeID) {}

	ShapeType m_shapeID;
	glm::vec4 m_shapeTypeColour;
	glm::vec4 m_colour;

public:

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void draw() = 0;
	virtual void resetPosition() {};

};

