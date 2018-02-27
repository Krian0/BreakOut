#pragma once
#include<glm\vec4.hpp>
#include<Gizmos.h>
#include "ShapeEnum.h"
#include "CollisionDataStruct.h"

class Plane;
class Sphere;
class Box;

class PhysicsObject
{

protected:
	PhysicsObject(ShapeType shapeTypeID, float bounciness, bool isStatic) : m_shapeID(shapeTypeID), m_bounciness(bounciness), m_isStatic(isStatic) {}

	ShapeType m_shapeID;
	glm::vec4 m_shapeTypeColour;
	glm::vec4 m_colour;
	bool m_isStatic;
	float m_bounciness;

public:

	virtual void resetVelocity() = 0;

	virtual bool detectCollision(CData& data, PhysicsObject& obj) = 0;
	virtual bool detectCollision(CData& data, Plane& plane) = 0;
	virtual bool detectCollision(CData& data, Sphere& sphere) = 0;
	virtual bool detectCollision(CData& data, Box& box) = 0;
	virtual bool detectCollision(glm::vec2& point) = 0;

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void draw() = 0;
	virtual void resetPosition() {};

	ShapeType getShapeID() { return m_shapeID; }
	bool isStatic() { return m_isStatic; }
};