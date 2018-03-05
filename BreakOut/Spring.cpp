#include "Spring.h"
#include "RigidBody.h"
#include <glm\ext.hpp>

Spring::Spring(RigidBody* body1, RigidBody* body2, float restLength, float springCoefficient, float damping) : 
	PhysicsObject(SPRING, 0, true),	m_body1(body1), m_body2(body2), m_restLength(restLength), m_springCoefficient(springCoefficient), m_damping(damping) {}
Spring::~Spring() {}

void Spring::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_contact1 = m_body1->getPosition();
	m_contact2 = m_body2->getPosition();

	glm::vec2 dist = m_contact2 - m_contact1; 
	float length = sqrtf(dist.x*dist.x + dist.y* dist.y);
	
	glm::vec2 relativeVelocity = m_body2->getVelocity() - m_body1->getVelocity();
	
	glm::vec2 force = dist * m_springCoefficient * (m_restLength - length) - m_damping * relativeVelocity;
	m_body1->applyForce(-force * timeStep, m_contact1 - m_body1->getPosition()); 
	m_body2->applyForce(force * timeStep, m_contact2 - m_body2->getPosition());
}

void Spring::draw()
{
	aie::Gizmos::add2DLine(m_contact1, m_contact2, glm::vec4(1, 1, 1, 1));
}