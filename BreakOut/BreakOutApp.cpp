#include "BreakOutApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Sphere.h"
#include "Plane.h"

#include <glm\ext.hpp>
#include <Gizmos.h>

BreakOutApp::BreakOutApp() 
{

}

BreakOutApp::~BreakOutApp() 
{

}

bool BreakOutApp::startup() 
{
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);		//TODO: remember to change this when redistributing a build! The following path would be used instead: "./font/consolas.ttf"

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, -9.8*2));
	m_physicsScene->setTimeStep(0.01f);

	Sphere* ball1 = new Sphere(glm::vec2(0, 26), glm::vec2(-4, -100), 0.01f, 3, glm::vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(glm::vec2(-10, 20), glm::vec2(-10, 100), 0.02f, 3, glm::vec4(1, 0, 0, 1));
	Sphere* ball3 = new Sphere(glm::vec2(12, -10), glm::vec2(4, -50), 0.03f, 3, glm::vec4(1, 0, 0, 1));
	Sphere* ball4 = new Sphere(glm::vec2(28, -20), glm::vec2(10, 50), 0.04f, 3, glm::vec4(1, 0, 0, 1));
	
	Plane* ground = new Plane(glm::vec2(0, -1), 50, glm::vec4(0, 0, 1, 1));
	Plane* sideLeft = new Plane(glm::vec2(-1, 0), 40, glm::vec4(0, 0, 1, 1));
	Plane* sideRight = new Plane(glm::vec2(1, 0), 40, glm::vec4(0, 0, 1, 1));
	Plane* sky = new Plane(glm::vec2(0, 1), 50, glm::vec4(0, 0, 1, 1));

	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(ball2);
	m_physicsScene->addActor(ball3);
	m_physicsScene->addActor(ball4);
	m_physicsScene->addActor(ground);
	m_physicsScene->addActor(sideLeft);
	m_physicsScene->addActor(sideRight);
	m_physicsScene->addActor(sky);

	return true;
}

void BreakOutApp::shutdown() 
{
	delete m_font;
	delete m_2dRenderer;
}

void BreakOutApp::update(float deltaTime) 
{
	aie::Input* input = aie::Input::getInstance();


	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->draw();


	/*static const glm::vec4 colours[] =
	{
		glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1),
		glm::vec4(0, 0, 1, 1), glm::vec4(0.8f, 0, 0.5f, 1),
		glm::vec4(0, 1, 1, 1)
	};


	static const int rows = 5;
	static const int columns = 10;
	static const int hSpace = 1;
	static const int vSpace = 1;

	static const glm::vec2 scrExtents(100, 50);
	static const glm::vec2 boxExtents(7, 3);
	static const glm::vec2 startPos(-(columns >> 1)*((boxExtents.x * 2) + vSpace) + boxExtents.x + (vSpace / 2.0f), scrExtents.y - ((boxExtents.y * 2) + hSpace));


	glm::vec2 pos;
	for (int y = 0; y < rows; y++)
	{
		pos = glm::vec2(startPos.x, startPos.y - (y* ((boxExtents.y * 2) + hSpace)));
		for (int x = 0; x < columns; x++)
		{
			aie::Gizmos::add2DAABBFilled(pos, boxExtents, colours[y]);
			pos.x += (boxExtents.x * 2) + vSpace;
		}
	}


	aie::Gizmos::add2DCircle(glm::vec2(0, -35), 3, 12, glm::vec4(1, 1, 0, 1));
	aie::Gizmos::add2DAABBFilled(glm::vec2(0, -40), glm::vec2(12, 2), glm::vec4(1, 0, 1, 1));*/


	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void BreakOutApp::draw() 
{
	clearScreen();
	m_2dRenderer->begin();


	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);
	m_2dRenderer->end();
}



void BreakOutApp::setupContinuousDemo(glm::vec2 startPos, float angle, float speed, float gravity)
{
	//float t = 0; 
	//float tStep = 0.5f; 
	//float radius = 1.0f; 
	//int segments = 12; 
	//glm::vec4 colour = glm::vec4(1, 1, 0, 1); 
	//
	//float angleInRad = angle * 3.14159f / 180.0f;
	//glm::vec2 initialVelocity(cosf(angleInRad) * speed, sinf(angleInRad) * speed);
	//while (t <= 5) 
	//{ 
	//	float Dx = startPos.x + (initialVelocity.x * t);
	//	float Dy = startPos.y + (initialVelocity.y * t) + ((gravity * (t * t)) / 2);


	//	aie::Gizmos::add2DCircle(glm::vec2(Dx, Dy), radius, segments, colour); 
	//	t += tStep;
	//} 
}