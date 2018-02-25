#pragma once
#include "RigidBody.h"
#include "CollisionDataStruct.h"


class Box : public RigidBody
{
public:
	Box(Vector2 position, Vector2 velocity, float rotation, float mass, float halfWidth, float halfHeight);
	~Box();

	virtual void fixedUpdate(Vector2 gravity, float timeStep);
	virtual void draw();
	void resetVelocity();

	Vector2 getCorner(unsigned int id) { return m_corners[id]; }
	Vector2 getCornerLocal(unsigned int id) { return Vector2(m_corners[id].x - m_position.x, m_corners[id].y - m_position.y); }
	
	Vector2 getLocalXAxis() { return m_localAxis[0]; }
	Vector2 getLocalYAxis() { return m_localAxis[1]; }
	Vector2 getHalfExtents() { return m_halfExtents; }
	glm::vec4 getColour() { return m_colour; }

	const static int axis_Size = 2;
	const static int corner_Size = 4;

protected:
	void updateVariables();

	Vector2 m_localAxis[axis_Size];
	Vector2 m_corners[corner_Size];
	Vector2 m_halfExtents;
};