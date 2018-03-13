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

class BreakOutApp : public aie::Application 
{
public:

	BreakOutApp();
	virtual ~BreakOutApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	RigidBody* getRandomShape(vec2 normal, float distance);

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	aie::Texture*		m_cannon;

	PhysicsScene*		m_physicsScene;
	vector<RigidBody*>	m_userCreatedProjectiles;

	vec2 m_zeroVec			= vec2(0, 0);
	vec2 m_mousePos			= m_zeroVec;
	vec2 m_gravity			= vec2(0, -9.8);
	vec2 m_shapeSpawnWorld	= vec2(-70, -22),	m_shapeSpawn = vec2(140, 160);

	vec4 m_darkBlue = vec4(0, 0, 1, 0), m_lightBlue = vec4(0, 1, 1, 1), m_green = vec4(0, 1, 0, 1), m_blueblue = vec4(0, 0, 1, 1);

	float m_timeStep	= 0.01f;

	bool  m_canLeftClick = true,			m_canRightClick = true;
	float m_leftClickDelayTimer	= 0,		m_rightClickDelayTimer = 0;
	const float LEFT_CLICK_DELAY = 0.6f,	RIGHT_CLICK_DELAY = 0.2f;
};