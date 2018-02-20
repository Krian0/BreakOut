#pragma once
#include "Vector2.h"

namespace UV2
{
	float distance(Vector2 V1, Vector2 V2);
	float distanceSquared(Vector2 V1, Vector2 V2);
	float length(Vector2 V1);
	float lengthSquared(Vector2 V1);
	float dot(Vector2 V1, Vector2 V2);

	Vector2 normal(Vector2 V1);
	Vector2 perp(Vector2 V1);
	Vector2 projectOnAxis(Vector2 V1, Vector2 DIR);
	Vector2 reflect(Vector2 VEL, Vector2 DIR, float BOUNCE);
	Vector2 reflect(Vector2 VEL, Vector2 DIR);
	Vector2 unit(Vector2 V1);
}