#pragma once
#include "RigidBody.h"

class Box : public RigidBody
{
public:
	Box();
	Box(vec2 position, vec2 velocity, float rotation, float mass, float bounciness, vec2 halfExtents, vec4 colour, bool makeStatic = false);
	~Box();


	virtual void fixedUpdate(vec2 gravity, float timeStep);
	virtual void draw();


	bool detectCollision(PhysicsObject& obj) override;
	bool detectCollision(CData& data, Plane& plane) override;
	bool detectCollision(CData& data, Sphere& sphere) override;
	bool detectCollision(CData& data, Box& box) override;
	bool detectCollision(vec2& point, vec2& pointOut);


	vec2 getCorner(unsigned int id)			{ return m_corners[id]; }
	vec2 getCornerLocal(unsigned int id)	{ return m_cornerExtent[id]; }
	
	vec2 getLocalXAxis()	{ return m_localX; }
	vec2 getLocalYAxis()	{ return m_localY; }
	vec2 getHalfExtents()	{ return m_halfExtents; }

	const static int CORNER_SIZE = 4;

private:
	bool checkCorners(Box& box, vec2& contactForce, CData& data);
	void updateVariables();

	vec2 m_localX;
	vec2 m_localY;
	vec2 m_corners[CORNER_SIZE];			//Corners in world space. Indices return clockwise corners starting from TopLeft.
	vec2 m_cornerExtent[CORNER_SIZE];		//Stores the extents of each corner.
	vec2 m_halfExtents;
};