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
	Box(Vector2 position, Vector2 velocity, float rotation, float mass, float halfWidth, float halfHeight);
	~Box();

	virtual void fixedUpdate(Vector2 gravity, float timeStep);
	virtual void draw();
	void resetVelocity();

	Vector2 getCorner(BoxCorner id);
	Vector2 getProjection(Vector2 axis);
	
	Vector2 getLocalXAxis() { return m_localAxis[0]; }
	Vector2 getLocalYAxis() { return m_localAxis[1]; }
	Vector2 getHalfExtents() { return m_halfExtents; }
	glm::vec4 getColour() { return m_colour; }


protected:
	void updateVariables();

	Vector2 m_localAxis[2];
	Vector2 m_corners[4];
	Vector2 m_halfExtents;
};