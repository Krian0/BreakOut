//#include "CResolution.h"
//#include "CollisionDataStruct.h"

//bool CResolution::resolve(CData& data, Sphere* sphere, Plane* plane)
//{
//}

//bool CResolution::resolve(CData& data, Sphere* sphere1, Sphere* sphere2)
//{
//	sphere1->setPosition(sphere1->getPosition() + data.contactForce);
//	sphere2->setPosition(sphere2->getPosition() - data.contactForce);
//
//	return rigidResolve(sphere1, sphere2, 0.5f * (sphere1->getPosition() + sphere2->getPosition()));
//}
//
//bool CResolution::resolve(CData& data, Box* box, Plane* plane)
//{
//	return false;
//}
//
//bool CResolution::resolve(CData& data, Box* box, Sphere* sphere)
//{
//	return rigidResolve(box, sphere, data.contact, &data.normal);
//}
//
//bool CResolution::resolve(CData& data, Box* box1, Box* box2)
//{
//	box1->setPosition(box1->getPosition() - data.contactForce);
//	box2->setPosition(box2->getPosition() + data.contactForce);
//
//	return rigidResolve(box1, box2, data.contact / data.numberOfContacts, &data.normal);
//}
//
//bool CResolution::resolve(CData & data, Plane * plane, Sphere * sphere)
//{
//	return false;
//}
//
//bool CResolution::resolve(CData & data, Plane * plane, Box * box)
//{
//	return false;
//}
//
//bool CResolution::resolve(CData & data, Sphere * sphere, Box * box)
//{
//	return false;
//}


//Private Functions:

//bool CResolution::rigidResolve(RigidBody* rigid1, RigidBody* rigid2, glm::vec2 contact, glm::vec2* collisionNormal)
//{
//	float m1 = rigid1->getMass(), m2 = rigid2->getMass();
//	glm::vec2 pos1 = rigid1->getPosition(), pos2 = rigid2->getPosition();
//
//	if (rigid1->isStatic())	m1 = INFINITY;
//	if (rigid2->isStatic())	m2 = INFINITY;
//
//	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(pos2 - pos1);
//	glm::vec2 perp(normal.y, -normal.x);
//
//	glm::vec2 R(glm::dot(contact - pos1, -perp), glm::dot(contact - pos2, perp));
//	glm::vec2 V(glm::dot(rigid1->getVelocity(), normal) - R.x * rigid1->getRotVelocity(),	glm::dot(rigid2->getVelocity(), normal) + R.y * rigid2->getRotVelocity());
//
//
//	if (V.x <= V.y) return false;
//
//	glm::vec2 M(1.0f / (1.0f / m1 + (R.x * R.x) / rigid1->getInertia()), 1.0f / (1.0f / m2 + (R.y * R.y) / rigid2->getInertia()));
//	float elasticity = (rigid1->getBounciness() + rigid2->getBounciness()) / 2.0f;
//	glm::vec2 force = (1.0f + elasticity) * M.x * M.y / (M.x + M.y) * (V.x - V.y) * normal;
//
//	return rigid1->applyForceToActor(rigid, -force, contact - pos1, contact - pos2);
//}