#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

#include "Plane.h"
#include "Sphere.h"
#include "Box.h"

class BreakOutApp : public aie::Application {
public:

	BreakOutApp();
	virtual ~BreakOutApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void setupContinuousDemo(glm::vec2 startPos, float angle, float acceleration, float gravity);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene*		m_physicsScene;

	Sphere* s1;
	Sphere* s2;
	Sphere* sa;
	Sphere* sb;
	Sphere* sc;

	Box* b1;
	Box* b2;

};