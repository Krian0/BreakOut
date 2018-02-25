#pragma once
#include "VecLib\UtilityVec2.h"
#include "PhysicsObject.h"

struct CData
{
	PhysicsObject* obj1;
	PhysicsObject* obj2;

	Vector2 contact;
	Vector2 contactForce;
	Vector2 normal;
	float numberOfContacts;
	float penetration;

	//Reset variables and set obj1 and obj2 to the respective given parameters. Takes two PhysicsObject* parameters.
	void resetData(PhysicsObject* object1, PhysicsObject* object2)
	{
		contact.SetVector(0, 0);
		contactForce.SetVector(0, 0);
		normal.SetVector(0, 0);
		numberOfContacts = 0;
		penetration = 0;

		obj1 = object1;
		obj2 = object2;
	}

	//Switches what obj1 and obj2 point to. No parameters.
	CData& switchObjects()
	{
		PhysicsObject* newObject2 = obj1;
		obj1 = obj2;
		obj2 = newObject2;

		return *this;
	}

	void setData(Vector2 contactAdd, Vector2 normalV, float penetrationF, bool noSkip)
	{
		numberOfContacts++;
		contact += contactAdd;

		if (!noSkip) return;

		normal = normalV;
		penetration = penetrationF;
	}
};