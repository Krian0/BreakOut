#pragma once
#include<glm\vec2.hpp>
#include "PhysicsObject.h"

struct CData
{
	glm::vec2 contact;
	glm::vec2 contactForce;
	glm::vec2 normal;
	float numberOfContacts = 0;
	float penetration = 0;

	CData() : contact(glm::vec2(0, 0)), contactForce(glm::vec2(0, 0)), normal(glm::vec2(0, 0)), numberOfContacts(0), penetration(0) {}

	//Increments numberOfContacts, sets variables to the matching parameters given. Allows normal and penetration to be set optionally through bool parameter.
	void setData(glm::vec2 contactAdd, glm::vec2 normalV, float penetrationF, bool setN_P = true)
	{
		numberOfContacts++;
		contact += contactAdd;

		if (!setN_P) return;

		normal = normalV;
		penetration = penetrationF;
	}

	void setData(glm::vec2 contactAdd, glm::vec2 normalV)
	{
		numberOfContacts++;
		contact += contactAdd;
		normal = normalV;
	}

	void setData(glm::vec2 contactAdd)
	{
		numberOfContacts++;
		contact += contactAdd;
	}
};