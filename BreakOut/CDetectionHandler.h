#pragma once
#include "Projection.h"
#include "Plane.h"
#include "Sphere.h"
#include "Box.h"

class CDetectionHandler
{
public:
	static bool detectCollision(PhysicsObject* object1, PhysicsObject* object2, Vector2& contact, Vector2& normal, float& numberOfContacts);

	//
	static bool planePlane(PhysicsObject* plane_1, PhysicsObject* plane_2, Vector2& contact, Vector2& normal, float& numberOfContacts);
	static bool planeSphere(PhysicsObject* plane, PhysicsObject* sphere, Vector2& contact, Vector2& normal, float& numberOfContacts);
	static bool planeBox(PhysicsObject* plane, PhysicsObject* box, Vector2& contact, Vector2& normal, float& numberOfContacts);

	static bool spherePlane(PhysicsObject* sphere, PhysicsObject* plane, Vector2& contact, Vector2& normal, float& numberOfContacts);
	static bool sphereSphere(PhysicsObject* sphere_1, PhysicsObject* sphere_2, Vector2& contact, Vector2& normal, float& numberOfContacts);
	static bool sphereBox(PhysicsObject* sphere, PhysicsObject* box, Vector2& contact, Vector2& normal, float& numberOfContacts);

	static bool boxPlane(PhysicsObject* box, PhysicsObject* plane, Vector2& contact, Vector2& normal, float& numberOfContacts);
	static bool boxSphere(PhysicsObject* box, PhysicsObject* sphere, Vector2& contact, Vector2& normal, float& numberOfContacts);
	static bool boxBox(PhysicsObject* box_1, PhysicsObject* box_2, Vector2& contact, Vector2& normal, float& numberOfContacts);
	//ask if theres a way to make these ^ protected or private without breaking shit

private:
	CDetectionHandler();
	~CDetectionHandler();
};