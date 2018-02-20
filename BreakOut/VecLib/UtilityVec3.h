#pragma once
#include "Vector3.h"

namespace UV3
{
	float distance(Vector3 V1, Vector3 V2);
	float distanceSquared(Vector3 V1, Vector3 V2);
	float length(Vector3 V1);
	float lengthSquared(Vector3 V1);
	float dot(Vector3 V1, Vector3 V2);

	Vector3 cross(Vector3 V1, Vector3 V2);
	Vector3 normal(Vector3 V1);
	Vector3 perp(Vector3 V1, Vector3 V2, Vector3 V3);
	Vector3 projectOnAxis(Vector3 V1, Vector3 DIR);
	Vector3 reflect(Vector3 VEL, Vector3 DIR, float BOUNCE);
	Vector3 reflect(Vector3 VEL, Vector3 DIR);
	Vector3 unit(Vector3 V1);
}