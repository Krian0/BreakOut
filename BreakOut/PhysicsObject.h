#pragma once
#include<glm\vec2.hpp>
#include<glm\vec4.hpp>
#include<Gizmos.h>

enum ShapeType 
{
	PLANE = 0,
	SPHERE,
	BOX,
	SHAPE_COUNT,
	SPRING
};

class PhysicsObject
{

protected:
	PhysicsObject(ShapeType shapeTypeID, bool isStatic) : m_shapeID(shapeTypeID), m_isStatic(isStatic) {}

	ShapeType m_shapeID;
	glm::vec4 m_shapeTypeColour;
	glm::vec4 m_colour;
	bool m_isStatic;

public:

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void draw() = 0;
	virtual void resetPosition() {};

	ShapeType getShapeID() { return m_shapeID; }
	bool isStatic() { return m_isStatic; }
};