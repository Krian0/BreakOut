#pragma once
#include<glm\vec2.hpp>
using glm::vec2;

class CData
{
public:
	CData(); 
	~CData();


	void setContact(vec2 contact);
	void setNormal(vec2 normal);
	void setPenetration(float penetration);
	void setNormalAndPenetration(vec2 normal, float penetration);
	
	vec2 getContact()			{ return m_contact; }
	vec2 getContactForce()		{ return m_penetration * m_normal; }
	vec2 getNormal()			{ return m_normal; }
	float getPenetration()		{ return m_penetration; }
	float getNumberOfContacts() { return m_numberOfContacts; }
	
	void increaseNumberOfContacts();
	void divideContact();
	void flipNormal();

	
	bool noContactsFound();

	void onContact(vec2 contactAdd, vec2 normalV, float penetrationF);
	void onContact(vec2 contactAdd, vec2 normalV);
	void onContact(vec2 contactAdd);


private:

	vec2 m_contact;
	vec2 m_normal;
	float m_penetration;
	float m_numberOfContacts;
};