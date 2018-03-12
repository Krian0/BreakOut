#pragma once
#include "PhysicsObject.h"
class RigidBody;

class Spring : public PhysicsObject
{

public:

	Spring(RigidBody* body1, RigidBody* body2, float restLength, float springCoefficient, float damping = 0.1f);
	~Spring();

	void fixedUpdate(glm::vec2 gravity, float timeStep);
	void draw();
	
	bool detectCollision(PhysicsObject& obj)			{ return false; }
	bool detectCollision(CData& data, Plane& plane)		{ return false; }
	bool detectCollision(CData& data, Sphere& sphere)	{ return false; }
	bool detectCollision(CData& data, Box& box)			{ return false; }


private:

	RigidBody* m_body1;
	RigidBody* m_body2;
	
	vec2 m_contact1; 
	vec2 m_contact2; 
	
	float m_damping; 
	float m_restLength;
	float m_springCoefficient;

};