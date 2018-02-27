#pragma once
#include <glm\vec2.hpp>
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
	ProjectionHandler(glm::vec2 vectorA, glm::vec2 vectorB);
	~ProjectionHandler() {};

	bool overlaps();
	float getPenetration() { return fminf(p1.max, p2.max) - fmaxf(p1.min, p2.min); }

	Projection p1;
	Projection p2;

protected:
	ProjectionHandler() {};
};