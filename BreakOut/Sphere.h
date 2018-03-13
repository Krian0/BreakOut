#pragma once
#include "RigidBody.h"

class Sphere : public RigidBody
{
public:
	Sphere();
	Sphere(vec2 position, vec2 velocity, float mass, float bounciness, float radius, vec4 colour, bool makeStatic = false);
	~Sphere();


	virtual void fixedUpdate(vec2 gravity, float timeStep);
	virtual void draw();


	bool detectCollision(PhysicsObject& obj) override;
	bool detectCollision(CData& data, Plane& plane) override;
	bool detectCollision(CData& data, Sphere& sphere) override;
	bool detectCollision(CData& data, Box& box) override;
	bool detectCollision(vec2& point);


	float getRadius() { return m_radius; }

protected:
	float m_radius;
};