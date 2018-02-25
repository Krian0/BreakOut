#include "CResolution.h"

bool CResolution::resolve(CData& data, Sphere* sphere, Plane* plane)
{
}

bool CResolution::resolve(CData& data, Sphere* sphere1, Sphere* sphere2)
{
	sphere1->setPosition(sphere1->getPosition() + data.contactForce);
	sphere2->setPosition(sphere2->getPosition() - data.contactForce);

	return rigidResolve(sphere1, sphere2, 0.5f * (sphere1->getPosition() + sphere2->getPosition()));
}

bool CResolution::resolve(CData& data, Box* box, Plane* plane)
{
	return false;
}

bool CResolution::resolve(CData& data, Box* box, Sphere* sphere)
{
	return rigidResolve(box, sphere, data.contact, &data.normal);
}

bool CResolution::resolve(CData& data, Box* box1, Box* box2)
{
	box1->setPosition(box1->getPosition() - data.contactForce);
	box2->setPosition(box2->getPosition() + data.contactForce);

	return rigidResolve(box1, box2, data.contact / data.numberOfContacts, &data.normal);
}


//Private Functions:

bool CResolution::rigidResolve(RigidBody* rigid1, RigidBody* rigid2, Vector2 contact, Vector2* collisionNormal)
{
	float m1 = rigid1->getMass(), m2 = rigid2->getMass();
	Vector2 pos1 = rigid1->getPosition(), pos2 = rigid2->getPosition();

	if (rigid1->isStatic())	m1 = INFINITY;
	if (rigid2->isStatic())	m2 = INFINITY;

	Vector2 normal = collisionNormal ? *collisionNormal : UV2::normal(pos2 - pos1);
	Vector2 perp(normal.y, -normal.x);

	Vector2 R(UV2::dot(contact - pos1, -perp), UV2::dot(contact - pos2, perp));
	Vector2 V(UV2::dot(rigid1->getVelocity(), normal) - R.x * rigid1->getRotVelocity(),
			  UV2::dot(rigid2->getVelocity(), normal) + R.y * rigid2->getRotVelocity());


	if (V.x <= V.y) return;

	Vector2 M(1.0f / (1.0f / m1 + (R.x * R.x) / rigid1->getInertia()), 1.0f / (1.0f / m2 + (R.y * R.y) / rigid2->getInertia()));
	float elasticity = (rigid1->getBounciness() + rigid2->getBounciness()) / 2.0f;
	Vector2 force = (1.0f + elasticity) * M.x * M.y / (M.x + M.y) * (V.x - V.y) * normal;

	rigid1->applyForceToActor(rigid2, -force, contact - pos1, contact - pos2);
}