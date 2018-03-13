#include "CollisionData.h"

CData::CData() : m_contact(vec2()), m_normal(vec2()), m_penetration(0), m_numberOfContacts(0) {}
CData::~CData() {}

//Set contact to given vec2
void CData::setContact(vec2 contact)			
{ 
	m_contact = contact; 
}

//Set normal to given vec2
void CData::setNormal(vec2 normal)				
{
	m_normal = normal;
}

//Set penetration to given float
void CData::setPenetration(float penetration)	
{ 
	m_penetration = penetration;
}

//Set both CData's normal and penetration
void CData::setNormalAndPenetration(vec2 normal, float penetration)
{
	m_normal = normal;
	m_penetration = penetration;
}

//Add 1 to numberOfContacts
void CData::increaseNumberOfContacts()	
{ 
	m_numberOfContacts++; 
}

//Divide contact by numberOfContacts
void CData::divideContact()				
{ 
	m_contact /= m_numberOfContacts;
}

//Set normal to the negative of itself
void CData::flipNormal()				
{
	m_normal = -m_normal; 
}

//Returns true if numberOfContacts is 0, else returns false
bool CData::noContactsFound() 
{
	return (m_numberOfContacts == 0);
}

//Increments numberOfContacts, adds contactAdd to m_contact, set m_normal to normalV, set m_penetration to penetrationF.
void CData::onContact(vec2 contactAdd, vec2 normalV, float penetrationF)
{
	m_numberOfContacts++;
	m_contact += contactAdd;
	m_normal = normalV;
	m_penetration = penetrationF;
}

//Increments numberOfContacts, adds contactAdd to m_contact, set m_normal to normalV.
void CData::onContact(vec2 contactAdd, vec2 normalV)
{
	m_numberOfContacts++;
	m_contact += contactAdd;
	m_normal = normalV;
}

//Increments numberOfContacts, adds contactAdd.
void CData::onContact(glm::vec2 contactAdd)
{
	m_numberOfContacts++;
	m_contact += contactAdd;
}