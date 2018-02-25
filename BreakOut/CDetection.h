#pragma once
#include "Projection.h"
#include "PhysicsObject.h"
#include "CollisionDataStruct.h"

#include "Box.h"

class CDetection
{
public:
	static bool detectCollision(PhysicsObject* object1, PhysicsObject* object2, CData& data);

	//
	static bool planePlane(CData& data);
	static bool planeSphere(CData& data);
	static bool planeBox(CData& data);

	static bool spherePlane(CData& data);
	static bool sphereSphere(CData& data);
	static bool sphereBox(CData& data);

	static bool boxPlane(CData& data);
	static bool boxSphere(CData& data);
	static bool boxBox(CData& data);
	//ask if theres a way to make these ^ protected or private without breaking shit
	
private:
	CDetection() {};
	~CDetection() {};

	static bool checkCorners(Box* b1, Box* b2, Vector2& contactForce, CData& data);
};