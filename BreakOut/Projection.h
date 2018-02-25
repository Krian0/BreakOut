#pragma once
#include "VecLib\Vector2.h"
#include <math.h>

class ProjectionHandler
{
	struct Projection
	{
		float min, max;
		Projection() {}
		Projection(float minF, float maxF) : min(minF), max(maxF) {}
	};

public:
	ProjectionHandler(Vector2 vectorA, Vector2 vectorB);
	~ProjectionHandler() {};

	bool overlaps();
	float getPenetration() { return fminf(p1.max, p2.max) - fmaxf(p1.min, p2.min); }

	Projection p1;
	Projection p2;

protected:
	ProjectionHandler() {};
};