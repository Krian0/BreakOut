#pragma once
#include "RigidBody.h"

class Box : public RigidBody
{
public:
	Box();
	Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float bounciness, float halfWidth, float halfHeight, bool makeStatic = false, glm::vec4* colour = nullptr);
	~Box();


	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void draw();


	bool detectCollision(PhysicsObject& obj) override;
	bool detectCollision(CData& data, Plane& plane) override;
	bool detectCollision(CData& data, Sphere& sphere) override;
	bool detectCollision(CData& data, Box& box) override;
	bool detectCollision(glm::vec2& point, glm::vec2& pointOut) override;


	glm::vec2 getCorner(unsigned int id)		{ return m_corners[id]; }
	glm::vec2 getCornerLocal(unsigned int id)	{ return m_cornerExtent[id]; }
	
	glm::vec2 getLocalXAxis()	{ return m_localX; }
	glm::vec2 getLocalYAxis()	{ return m_localY; }
	glm::vec2 getHalfExtents()	{ return m_halfExtents; }

	const static int corner_Size = 4;
	const static int axis_Size = 2;

private:
	bool checkCorners(Box& box, glm::vec2& contactForce, CData& data);
	void updateVariables();

	glm::vec2 m_localX;
	glm::vec2 m_localY;
	glm::vec2 m_corners[corner_Size];			//Corners in world space. Indices return clockwise corners starting from TopLeft.
	glm::vec2 m_cornerExtent[corner_Size];		//Stores the extents of each corner.
	glm::vec2 m_halfExtents;
};