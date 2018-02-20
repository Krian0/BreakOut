#pragma once
#include "VecLib\Vector2.h"
#include <math.h>

class Projection
{
public:
	Projection(Vector2 vectorA, Vector2 vectorB);
	~Projection() {};

	float getDistance() { return fminf(p1.y, p2.y) - fmaxf(p1.x, p2.x); }

	Vector2 p1;
	Vector2 p2;
	bool Intersection;

protected:
	bool contactsOn(float pValue, Vector2 otherP) { return pValue >= otherP.x && pValue <= otherP.y; };
	Projection() {};
};