#pragma once
#include "RigidBody.h"
class Box :
	public RigidBody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float mass);
	~Box();
};

