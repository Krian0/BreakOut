#pragma once
#include "PhysicsObject.h"
#include "CollisionDataStruct.h"
#include "Plane.h"
#include "Sphere.h"
#include "Box.h"

class CResolution
{
public:
	static bool resolve(CData& data, Sphere* sphere, Plane* plane);
	static bool resolve(CData& data, Sphere* sphere1, Sphere* sphere2);

	static bool resolve(CData& data, Box* box, Plane* plane);
	static bool resolve(CData& data, Box* box, Sphere* sphere);
	static bool resolve(CData& data, Box* box1, Box* box2);

private:
	CResolution() {};
	~CResolution() {};

	static bool rigidResolve(RigidBody* actor1, RigidBody* actor2, Vector2 contact, Vector2* collisionNormal = nullptr);
};