#pragma once
#include "RigidBody.h"

class Sphere : public RigidBody
{
public:
	Sphere();
	Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float bounciness, float radius, bool makeStatic = false, glm::vec4* colour = nullptr);
	~Sphere();


	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void draw();


	bool detectCollision(PhysicsObject& obj) override;
	bool detectCollision(CData& data, Plane& plane) override;
	bool detectCollision(CData& data, Sphere& sphere) override;
	bool detectCollision(CData& data, Box& box) override;
	bool detectCollision(glm::vec2& point, glm::vec2& pointOut) override;


	float getRadius() { return m_radius; }

protected:
	float m_radius;
};