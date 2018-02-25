#include "RigidBody.h"
#include "VecLib\UtilityVec2.h"
#include <math.h>


RigidBody::RigidBody(ShapeType shapeTypeID, Vector2 position, Vector2 velocity, float rotation, float mass, float linearDrag, float rotationalDrag, bool isStatic)
	: PhysicsObject(shapeTypeID, isStatic), m_position(position), m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_linearDrag(linearDrag), m_rotationalDrag(rotationalDrag) {}
RigidBody::~RigidBody() {}


void RigidBody::fixedUpdate(Vector2 gravity, float timeStep)
{
	if (isStatic())
		return;

    m_position += m_velocity * timeStep;
	m_velocity += gravity * timeStep;
	

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_rotation += m_rotationalVelocity * timeStep;
	m_rotationalVelocity -= m_rotationalVelocity * m_rotationalDrag * timeStep;

	if (UV2::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		m_velocity.SetVector(0, 0);
	if (abs(m_rotationalVelocity) < MIN_ROTATION_THRESHOLD)
		m_rotationalVelocity = 0;
}

void RigidBody::debug()
{

}


bool RigidBody::applyForce(Vector2 force, Vector2 pos)
{
	m_velocity += force / m_mass;
	m_rotationalVelocity += (force.y * pos.x - force.x * pos.y) / (m_inertia);

	return true;
}

bool RigidBody::applyForceToActor(RigidBody* actor2, Vector2 force, Vector2 pos1, Vector2 pos2)
{
	applyForce(-force, pos1);
	return actor2->applyForce(force, pos2);
}

void RigidBody::setPosition(Vector2 position)
{
	m_position = position;
}

void RigidBody::setLinearDrag(float drag)
{
	drag = (float)fmin(drag, 1);
	drag = (float)fmax(drag, 0);

	m_linearDrag = drag;
}

void RigidBody::setRotationalDrag(float drag)
{
	drag = (float)fmin(drag, 1);
	drag = (float)fmax(drag, 0);

	m_rotationalDrag = drag;
}