#include "CDetectionHandler.h"
#include "PhysicsObject.h"
#include <vector>
using std::vector;


typedef bool(*CollisionTest)(PhysicsObject*, PhysicsObject*, Vector2&, Vector2&, float&);
static CollisionTest detect[SHAPE_COUNT][SHAPE_COUNT] =
{
	{ CDetectionHandler::planePlane,	CDetectionHandler::planeSphere,		CDetectionHandler::planeBox },
	{ CDetectionHandler::spherePlane,	CDetectionHandler::sphereSphere,	CDetectionHandler::sphereBox },
	{ CDetectionHandler::boxPlane,		CDetectionHandler::boxSphere,		CDetectionHandler::boxBox }
};


bool CDetectionHandler::detectCollision(PhysicsObject* ob1, PhysicsObject* ob2, Vector2& contact, Vector2& normal, float& numberOfContacts)
{
	return detect[ob1->getShapeID()][ob2->getShapeID()](ob1, ob2, contact, normal, numberOfContacts);
}

CDetectionHandler::CDetectionHandler(){}
CDetectionHandler::~CDetectionHandler(){}


bool CDetectionHandler::planePlane(PhysicsObject* plane_1, PhysicsObject* plane_2, Vector2& contact, Vector2& normal, float& numberOfContacts)
{ 
	return false; 
}
bool CDetectionHandler::planeSphere(PhysicsObject* plane, PhysicsObject* sphere, Vector2& contact, Vector2& normal, float& numberOfContacts)
{
	return spherePlane(sphere, plane, contact, normal, numberOfContacts); 
}
bool CDetectionHandler::planeBox(PhysicsObject* plane, PhysicsObject* box, Vector2& contact, Vector2& normal, float& numberOfContacts)
{ 
	return boxPlane(box, plane, contact, normal, numberOfContacts);
}

bool CDetectionHandler::spherePlane(PhysicsObject* sphere, PhysicsObject* plane, Vector2& contact, Vector2& normal, float& numberOfContacts)
{
	Sphere* s = dynamic_cast<Sphere*>(sphere);
	Plane* p = dynamic_cast<Plane*>(plane);
	if (s == nullptr || p == nullptr) return false;

	float sphereToPlane = p->distance(s->getPosition());
	contact = s->getPosition() + (p->getNormal() * -s->getRadius());
	if (sphereToPlane < 0)
	{
		sphereToPlane *= -1;
		contact *= -1;
	}

	if ((s->getRadius() - sphereToPlane) > 0)
		return true;

	return false;
}

bool CDetectionHandler::sphereSphere(PhysicsObject* sphere_1, PhysicsObject* sphere_2, Vector2& contact, Vector2& normal, float& numberOfContacts)
{
	Sphere* s1 = dynamic_cast<Sphere*>(sphere_1);
	Sphere* s2 = dynamic_cast<Sphere*>(sphere_2);
	if (s1 == nullptr || s2 == nullptr) return false;

	float combinedRadius = s1->getRadius() + s2->getRadius();
	if (UV2::distanceSquared(s1->getPosition(), s2->getPosition()) < combinedRadius * combinedRadius)
	{
		contact = 0.5f * (s1->getPosition() + s2->getPosition());
		return true;
	}

	return false;
}

bool CDetectionHandler::sphereBox(PhysicsObject* sphere, PhysicsObject* box, Vector2& contact, Vector2& normal, float& numberOfContacts)
{
	return boxSphere(box, sphere, contact, normal, numberOfContacts);
}

bool CDetectionHandler::boxPlane(PhysicsObject* box, PhysicsObject* plane, Vector2& contact, Vector2& normal, float& numberOfContacts)
{
	Box* b = dynamic_cast<Box*>(box);
	Plane* p = dynamic_cast<Plane*>(plane);
	if (b == nullptr || p == nullptr) return false;

	float contactVelocity = 0;

	float centerFromPlane = p->distance(b->getPosition());


	for (int i = 0; i < 4; i++)
	{
		Vector2 corner = b->getCorner((BoxCorner)i);

		float cornerFromPlane = p->distance(corner);
		Vector2 thing1 = -(b->getPosition().y - corner.y) * b->getLocalXAxis();											//NOTE:
		Vector2 thing2 = (b->getPosition().x - corner.x) * b->getLocalYAxis();											//ASK WHAT THIS DOES,
		float velocityIntoPlane = UV2::dot(b->getVelocity() + b->getRotation() * (thing1 + thing2), p->getNormal());	//AND FOR A BETTER WAY TO CALCULATE IT

		if ((cornerFromPlane > 0 && centerFromPlane < 0) || (cornerFromPlane < 0 && centerFromPlane > 0))
		{
			numberOfContacts++;
			contact += corner;
			contactVelocity += velocityIntoPlane;
		}
	}

	if (numberOfContacts == 0)
		return false;

	float elasticity = (b->getBounciness() + p->getBounciness()) / 2.0f;
	Vector2 acceleration = ((1 + elasticity) * (contactVelocity / numberOfContacts)) * -p->getNormal();

	Vector2 localContact = (contact / (float)numberOfContacts) - b->getPosition();
	float r = UV2::dot(localContact, Vector2(p->getNormal().y, -p->getNormal().x));
	float mass0 = 1.0f / (1.0f / b->getMass() + (r*r) / b->getInertia());

	//return b->applyForce(acceleration * mass0, localContact);
	return true;
}

bool CDetectionHandler::boxSphere(PhysicsObject* box, PhysicsObject* sphere, Vector2& contact, Vector2& normal, float& numberOfContacts)
{
	//Box* b = dynamic_cast<Box*>(box);
	//Sphere* s = dynamic_cast<Sphere*>(sphere);
	//if (b == nullptr || s == nullptr) return false;

	//glm::vec2 distance = b->getPosition() - s->getPosition();

	//int numberOfContacts = 0;
	//glm::vec2 contact(0, 0);

	//for (int i = 0; i < 4; i++)
	//{
	//	glm::vec2 corner = b->getCorner((BoxCorner)i);
	//	glm::vec2 p = (b->getPosition().x - corner.x) * b->getLocalXAxis() + (b->getPosition().y - corner.y) * b->getLocalYAxis();

	//	glm::vec2 dp = p - distance;

	//	if (dp.x * dp.x + dp.y * dp.y < s->getRadius() * s->getRadius())
	//	{
	//		numberOfContacts++;
	//		contact += b->getPosition() - corner;
	//	}
	//}

	return false;

	//NOT DONE
}

bool CDetectionHandler::boxBox(PhysicsObject* box_1, PhysicsObject* box_2, Vector2& contact, Vector2& normal, float& numberOfContacts)
{
	Box* b1 = dynamic_cast<Box*>(box_1);
	Box* b2 = dynamic_cast<Box*>(box_2);

	vector<Vector2> axesToTest = { b1->getLocalXAxis(), b1->getLocalYAxis(), b2->getLocalXAxis(), b2->getLocalYAxis() };

	float penetration = INFINITY;

	for (int i = 0; i < axesToTest.size(); i++)
	{
		Projection p(b1->getProjection(axesToTest[i]), b2->getProjection(axesToTest[i]));
		if (!p.Intersection)
			return false;

		float distance = p.getDistance();
		if (distance < penetration)
		{
			penetration = distance;
			normal = axesToTest[i];
		}
	}

	contact = penetration * normal;
	return true;
}