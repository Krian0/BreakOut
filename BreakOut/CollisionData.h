#pragma once
#include<glm\vec2.hpp>
using glm::vec2;

class CData
{
public:
	CData(); 
	~CData();

	//Sets variable named to given parameter.

	void setContact(vec2 contact);
	void setNormal(vec2 normal);
	void setPenetration(float penetration);
	void setNormalAndPenetration(vec2 normal, float penetration);
	
	//~

	//Return variable named
	
	vec2 getContact()			{ return m_contact; }
	vec2 getContactForce()		{ return m_penetration * m_normal; }
	vec2 getNormal()			{ return m_normal; }
	float getPenetration()		{ return m_penetration; }
	float getNumberOfContacts() { return m_numberOfContacts; }
	
	//~


	//Increase m_numberOfContacts by 1.
	void increaseNumberOfContacts();

	//Divides m_contact by m_numberOfContacts.
	void divideContact();

	//Sets m_normal to the negative of m_normal.
	void flipNormal();

	
	//Returns true if m_numberOfContacts is 0.
	bool noContactsFound();

	//Increments numberOfContacts, adds contactAdd to m_contact, set m_normal to normalV, set m_penetration to penetrationF.
	void onContact(vec2 contactAdd, vec2 normalV, float penetrationF);
	
	//Increments numberOfContacts, adds contactAdd to m_contact, set m_normal to normalV.
	void onContact(vec2 contactAdd, vec2 normalV);
	
	//Increments numberOfContacts, adds contactAdd.
	void onContact(vec2 contactAdd);


private:
	vec2 m_contact				= vec2(0, 0);
	vec2 m_normal				= vec2(0, 0);
	float m_penetration			= 0;
	float m_numberOfContacts	= 0;
};