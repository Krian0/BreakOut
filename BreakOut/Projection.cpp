#include "Projection.h"

ProjectionHandler::ProjectionHandler(glm::vec2 vectorA, glm::vec2 vectorB) : p1(vectorA.x, vectorA.y), p2(vectorB.x, vectorB.y)
{
	
}

bool ProjectionHandler::overlaps()
{
	return !(p1.min > p2.max || p2.min > p1.max);

	//If the above is screwy, try below
	//return ((p1.min >= p2.min && p1.min <= p2.max) || (p1.max >= p2.min && p1.max <= p2.max) || (p2.min >= p1.min && p2.min <= p1.max) || (p2.max >= p1.min && p2.max <= p1.max))

	// p1 = 2, 4
	// p2 = 1, 3

	// 2 >= 1 && 2 <= 3 true
	// 1 >= 2 && 1 <= 4 false
	// returns true
}