#pragma once
#include<glm\vec4.hpp>
#include<Gizmos.h>
#include "ShapeEnum.h"
#include "CollisionData.h"

class Plane;
class Sphere;
class Box;

using glm::vec2;
using glm::vec4;

class PhysicsObject
{

public:

	virtual void fixedUpdate(vec2 gravity, float timeStep) = 0;
	virtual void draw() = 0;

	virtual bool detectCollision(PhysicsObject& obj) = 0;
	virtual bool detectCollision(CData& data, Plane& plane) = 0;
	virtual bool detectCollision(CData& data, Sphere& sphere) = 0;
	virtual bool detectCollision(CData& data, Box& box) = 0;

	void setStatic(bool value)	{ m_isStatic = value; }
	bool isStatic()				{ return m_isStatic; }
	ShapeType getShapeID()		{ return m_shapeID; }

protected:
	PhysicsObject(ShapeType shapeTypeID, bool isStatic) : m_shapeID(shapeTypeID), m_isStatic(isStatic) {}

	ShapeType m_shapeID;
	vec4 m_colour;
	bool m_isStatic;
};