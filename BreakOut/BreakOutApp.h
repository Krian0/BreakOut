#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

#include "Plane.h"
#include "Sphere.h"
#include "Box.h"
#include "Spring.h"

#include <vector>
using std::vector;

class BreakOutApp : public aie::Application {
public:

	BreakOutApp();
	virtual ~BreakOutApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	RigidBody* getRandomShape(glm::vec2 normal, float distance);

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	aie::Font*			m_fontSmall;
	aie::Texture*		m_textureCannon;

	PhysicsScene*		m_physicsScene;

	vector<RigidBody*>	m_userCreatedProjectiles;

	glm::vec2 gravity	= glm::vec2(0, -9.8);
	glm::vec2 zeroVec	= glm::vec2(0, 0);
	glm::vec2 shootPos	= glm::vec2(-70, -22);
	glm::vec2 cannonVec = glm::vec2(140, 160);
	glm::vec2 mousePos	= zeroVec;

	float m_timeStep	= 0.01f;
	float m_clickDelay	= 0;
	bool  m_canLeftClick	= true;
	bool  m_canRightClick	= true;

};