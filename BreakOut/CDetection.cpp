#include "CDetection.h"
#include "CResolution.h"


typedef bool(*CollisionTest)(CData&);
static CollisionTest detect[SHAPE_COUNT][SHAPE_COUNT] =
{
	{ CDetection::planePlane,	CDetection::planeSphere,	CDetection::planeBox },
	{ CDetection::spherePlane,	CDetection::sphereSphere,	CDetection::sphereBox },
	{ CDetection::boxPlane,		CDetection::boxSphere,		CDetection::boxBox }
};

//
//ASK: If it would be fine to use virtual functions in the Shape classes for the assignment instead
//This is giving me a headache trying to keep it neat and tidy >_>

bool CDetection::detectCollision(PhysicsObject* ob1, PhysicsObject* ob2, CData& data)
{
	data.resetData(ob1, ob2);
	return detect[ob1->getShapeID()][ob2->getShapeID()](data);
}


bool CDetection::planePlane(CData& data) { return false; } //Don't check for plane v plane
bool CDetection::planeSphere(CData& data)
{
	return spherePlane(data.switchObjects());
}
bool CDetection::planeBox(CData& data)
{ 
	return boxPlane(data.switchObjects());
}

bool CDetection::spherePlane(CData& data)
{
	Sphere* s = dynamic_cast<Sphere*>(data.obj1);	
	Plane* p = dynamic_cast<Plane*>(data.obj2);
	if (s == nullptr || p == nullptr) return false;

	return false;

	//float sphereToPlane = p->distance(s->getPosition());
	//data.contact = s->getPosition() + (p->getNormal() * -s->getRadius());
	//if (sphereToPlane < 0)
	//{
	//	sphereToPlane *= -1;
	//	data.contact *= -1;
	//}

	//if ((s->getRadius() - sphereToPlane) > 0)
	//	return true;

	//return false;
}

bool CDetection::sphereSphere(CData& data)
{
	Sphere* s1 = dynamic_cast<Sphere*>(data.obj1);
	Sphere* s2 = dynamic_cast<Sphere*>(data.obj2);
	if (s1 == nullptr || s2 == nullptr) return false;

	Vector2 delta = s2->getPosition() - s1->getPosition();
	float distance = UV2::length(delta);
	float intersection = s1->getRadius() + s2->getRadius() - distance;

	if (intersection == 0) return false;

	data.contactForce = 0.5f * (distance - (s1->getRadius() + s2->getRadius())) * delta / distance;
	return CResolution::resolve(data, s1, s2);
}

bool CDetection::sphereBox(CData& data)
{
	return boxSphere(data.switchObjects());
}

bool CDetection::boxPlane(CData& data)
{
	Box* b = dynamic_cast<Box*>(data.obj1);
	Plane* p = dynamic_cast<Plane*>(data.obj2);
	if (b == nullptr || p == nullptr) return false;

	return false;

	//float contactVelocity = 0;

	//float centerFromPlane = p->distance(b->getPosition());


	//for (int i = 0; i < 4; i++)
	//{
	//	Vector2 corner = b->getCorner(i);

	//	float cornerFromPlane = p->distance(corner);
	//	Vector2 thing1 = -(b->getPosition().y - corner.y) * b->getLocalXAxis();											//NOTE:
	//	Vector2 thing2 = (b->getPosition().x - corner.x) * b->getLocalYAxis();											//ASK WHAT THIS DOES,
	//	float velocityIntoPlane = UV2::dot(b->getVelocity() + b->getRotation() * (thing1 + thing2), p->getNormal());	//AND FOR A BETTER WAY TO CALCULATE IT

	//	if ((cornerFromPlane > 0 && centerFromPlane < 0) || (cornerFromPlane < 0 && centerFromPlane > 0))
	//	{
	//		data.numberOfContacts++;
	//		data.contact += corner;
	//		contactVelocity += velocityIntoPlane;
	//	}
	//}

	//if (data.numberOfContacts == 0)
	//	return false;

	//float elasticity = (b->getBounciness() + p->getBounciness()) / 2.0f;
	//Vector2 acceleration = ((1 + elasticity) * (contactVelocity / data.numberOfContacts)) * -p->getNormal();

	//Vector2 localContact = (data.contact / data.numberOfContacts) - b->getPosition();
	//float r = UV2::dot(localContact, Vector2(p->getNormal().y, -p->getNormal().x));
	//float mass0 = 1.0f / (1.0f / b->getMass() + (r*r) / b->getInertia());

	////return b->applyForce(acceleration * mass0, localContact);
	//return true;
}

bool CDetection::boxSphere(CData& data)
{
	Box* b = dynamic_cast<Box*>(data.obj1);
	Sphere* s = dynamic_cast<Sphere*>(data.obj2);
	if (b == nullptr || s == nullptr) return false;

	
	float w		= b->getHalfExtents().x,	h	= b->getHalfExtents().y;
	float radius = s->getRadius();
	Vector2 circlePos = s->getPosition() - b->getPosition();

	for (int i = 0; i < b->corner_Size; i++)
	{
		Vector2 dp = b->getCorner(i) - circlePos;
		if (dp.x*dp.x + dp.y*dp.y < radius*radius)
		{
			data.numberOfContacts++;
			data.contact += b->getCornerLocal(i);
		}
	}

	Vector2 p(UV2::dot(b->getLocalXAxis(), circlePos), UV2::dot(b->getLocalYAxis(), circlePos));
	if (p.y < h && p.y > -h && p.x < w && p.x > -w)
	{
		if (p.x > 0 && p.x <   w + radius)  data.setData(Vector2( w, p.y),  b->getLocalXAxis(), 0, true);
		if (p.x < 0 && p.x > -(w + radius)) data.setData(Vector2(-w, p.y), -b->getLocalXAxis(), 0, true);
		if (p.y > 0 && p.y <   h + radius)  data.setData(Vector2(p.x,  h),  b->getLocalYAxis(), 0, true);
		if (p.y < 0 && p.y > -(h + radius)) data.setData(Vector2(p.x, -h), -b->getLocalYAxis(), 0, true);
	}

	if (data.numberOfContacts == 0) return false;
	data.contact = b->getPosition() + (1.0f / data.numberOfContacts) * (b->getLocalXAxis * data.contact.x + b->getLocalYAxis() * data.contact.y);

	return CResolution::resolve(data, b, s);
}

bool CDetection::boxBox(CData& data)
{
	Box* b1 = dynamic_cast<Box*>(data.obj1);
	Box* b2 = dynamic_cast<Box*>(data.obj2);
	if (b1 == nullptr || b2 == nullptr) return false;

	Vector2 contactF1, contactF2;

	checkCorners(b2, b1, contactF1, data);
	if (checkCorners(b1, b2, contactF2, data))
		data.normal = -data.normal;

	if (data.numberOfContacts == 0) return false;

	data.contactForce = 0.5f * (contactF1 - contactF2);
	return CResolution::resolve(data, b1, b2);
}


//Private Functions

bool CDetection::checkCorners(Box* b1, Box* b2, Vector2& contactForce, CData& data)
{
	//Get necessary variables
	float   w		= b1->getHalfExtents().x,	h		= b1->getHalfExtents().y;
	Vector2 xAxis	= b1->getLocalXAxis(),		yAxis	= b1->getLocalYAxis();
	Vector2 pos		= b1->getPosition();

	data.penetration = 0;

	//ASK: If this is an acceptable simplification with no bugs. Works the same on paper, but still want clarity.
	//Check if each corner of b2 is within b1's extents
	for (int i = 0; i < b2->corner_Size; i++)
	{
		Vector2 p(UV2::dot(b2->getCorner(i) - pos, xAxis), UV2::dot(b2->getCorner(i) - pos, yAxis)); //b2's corner in b1's local space

		if (p.y > -h && p.y < h && p.x > -w && p.x < w)
		{
			if (p.x > 0)	data.setData((pos + w * xAxis + p.y * yAxis),  xAxis, w - p.x, true);
			if (p.x < 0)	data.setData((pos - w * xAxis + p.y * yAxis), -xAxis, w + p.x, true);
			if (p.y > 0)	data.setData((pos + p.x * xAxis + h * yAxis),  yAxis, h - p.y, (h - p.y < data.penetration) || !(data.penetration == 0));
			if (p.y < 0)	data.setData((pos + p.x * xAxis - h * yAxis), -yAxis, h + p.y, (h + p.y < data.penetration) || !(data.penetration == 0));
		} 
	}

	contactForce = data.penetration * data.normal;
	return (data.penetration != 0);
}