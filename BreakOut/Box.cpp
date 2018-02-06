#include "Box.h"



Box::Box(glm::vec2 position, glm::vec2 velocity, float mass) : RigidBody(BOX, position, velocity, 0, mass)
{
}


Box::~Box()
{
}
