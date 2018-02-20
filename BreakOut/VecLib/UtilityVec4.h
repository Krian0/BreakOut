#pragma once
#include "Vector4.h"

namespace UV4
{
	float distance(Vector4 V1, Vector4 V2);
	float distanceSquared(Vector4 V1, Vector4 V2);
	float length(Vector4 V1);
	float lengthSquared(Vector4 V1);
	float dot(Vector4 V1, Vector4 V2);

	Vector4 cross(Vector4 V1, Vector4 V2);
	Vector4 normal(Vector4 V1);
	Vector4 perp(Vector4 V1, Vector4 V2, Vector4 V3);
	Vector4 projectOnAxis(Vector4 V1, Vector4 DIR);
	Vector4 reflect(Vector4 VEL, Vector4 DIR, float BOUNCE);
	Vector4 reflect(Vector4 VEL, Vector4 DIR);
	Vector4 unit(Vector4 V1);
}