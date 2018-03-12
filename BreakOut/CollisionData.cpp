#include "CollisionData.h"

CData::CData() {}
CData::~CData() {}

void CData::setContact(vec2 contact)			{ m_contact = contact; }
void CData::setNormal(vec2 normal)				{ m_normal = normal; }
void CData::setPenetration(float penetration)	{ m_penetration = penetration; }
void CData::setNormalAndPenetration(vec2 normal, float penetration)
{
	m_normal = normal;
	m_penetration = penetration;
}

void CData::increaseNumberOfContacts()	{ m_numberOfContacts++; }
void CData::divideContact()				{ m_contact /= m_numberOfContacts; }
void CData::flipNormal()				{ m_normal = -m_normal; }

bool CData::noContactsFound() { return (m_numberOfContacts == 0); }

void CData::onContact(vec2 contactAdd, vec2 normalV, float penetrationF)
{
	m_numberOfContacts++;
	m_contact += contactAdd;
	m_normal = normalV;
	m_penetration = penetrationF;
}

void CData::onContact(vec2 contactAdd, vec2 normalV)
{
	m_numberOfContacts++;
	m_contact += contactAdd;
	m_normal = normalV;
}

void CData::onContact(glm::vec2 contactAdd)
{
	m_numberOfContacts++;
	m_contact += contactAdd;
}