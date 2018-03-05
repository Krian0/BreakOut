#include "PhysicsObject.h"

float PhysicsObject::getElasticity(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return 1 + (obj1->m_bounciness + obj2->m_bounciness) / 2.0f;
}

float PhysicsObject::getElasticity(PhysicsObject* obj)
{
	return 1 + (m_bounciness + obj->m_bounciness) / 2.0f;
}