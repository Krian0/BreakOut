#include "Projection.h"

Projection::Projection(Vector2 vectorA, Vector2 vectorB) : p1(vectorA), p2(vectorB)
{
	if (contactsOn(p1.x, p2) || contactsOn(p1.y, p2) || contactsOn(p2.x, p1) || contactsOn(p2.y, p1))
		Intersection = true;
	else
		Intersection = false;
}