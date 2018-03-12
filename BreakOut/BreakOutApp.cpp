#include "BreakOutApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

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
	srand((unsigned int)time(NULL));

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	m_2dRenderer	= new aie::Renderer2D();
	m_font			= new aie::Font("../bin/font/consolas.ttf", 32);		//TODO: remember to change this when redistributing a build! The following path would be used instead: "./font/consolas.ttf"
	m_fontSmall		= new aie::Font("../bin/font/consolas.ttf", 24);
	m_textureCannon = new aie::Texture("../bin/textures/cannonWhite.png");


	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(gravity);
	m_physicsScene->setTimeStep(m_timeStep);

	//Planes
	m_physicsScene->addActor(new Plane(glm::vec2(0, -1), 30, 0.75f));		//Ground plane
	m_physicsScene->addActor(new Plane(glm::vec2(-1, 0), 85, 0.75f));		//Left plane
	m_physicsScene->addActor(new Plane(glm::vec2(1, 0), 90, 0.75f));		//Right plane
	m_physicsScene->addActor(new Plane(glm::vec2(0, 1), 50, 0.75f));		//Sky plane

	//Static shapes
	m_physicsScene->addActor(new Box(glm::vec2( 70, 20), zeroVec, 2, 3, 0.45f, 8, 8, true));
	m_physicsScene->addActor(new Box(glm::vec2(-20, 30), zeroVec, 0, 5, 0.50f, 16, 4, true));
	m_physicsScene->addActor(new Sphere(glm::vec2(10, -20), zeroVec, 4, 0.8f, 6, true));

	//Water Surface (Rope)
	//Sphere* ballA, * ballB;
	//glm::vec4 blue(0, 0, 1, 0);
	//float radius = 1.5, mass = 1, bounce = 0.4f;

	//ballA = new Sphere(glm::vec2(-82, -30), zeroVec, mass, bounce, radius, true, &blue);
	//m_physicsScene->addActor(ballA);
	//
	//int numberBalls = 34;
	//for (int i = 1; i < numberBalls; i++) 
	//{ 
	//	bool isStatic = (i == numberBalls - 1) ? true : false;
	//	ballB = new Sphere(ballA->getPosition() + glm::vec2(radius * 3.42, 0), zeroVec, mass, bounce, radius, isStatic, &blue);
	//	m_physicsScene->addActor(ballB);
	//	m_physicsScene->addActor(new Spring(ballA, ballB, radius * 0.50, 98, .8f));
	//	ballA = ballB;
	//}




	m_physicsScene->addActor(new Box(glm::vec2(-20, -20), zeroVec, 0, 20, 0.45f, 12, 2));
	m_physicsScene->addActor(new Box(glm::vec2(-20, 30), glm::vec2(1, -2), 2, 17, 0.45f, 2, 2));


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
	m_clickDelay += deltaTime;

	mousePos.x = (float)input->getMouseX();
	mousePos.y = (float)input->getMouseY();

	m_physicsScene->update(deltaTime);
	m_physicsScene->draw();

	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT) && m_userCreatedProjectiles.size() < 10 && m_clickDelay > 0.7)
	{
		m_clickDelay = 0;
		if (m_canLeftClick)
		{
			m_canLeftClick = false;
			
			vec2 dir = glm::normalize(mousePos - cannonVec);
			float length = glm::distance(mousePos, cannonVec);

			RigidBody* shape = getRandomShape(dir, length / 3);

			m_userCreatedProjectiles.push_back(shape);
			m_physicsScene->addActor(shape);
		}
	}
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT) && m_userCreatedProjectiles.size() >= 1 && m_clickDelay > 0.22)
	{
		m_clickDelay = 0;
		if (m_canRightClick)
		{
			m_canRightClick = false;
			
			m_physicsScene->removeActor(m_userCreatedProjectiles.back());
			m_userCreatedProjectiles.pop_back();
		}
	}

	if (!m_canLeftClick && input->isMouseButtonUp(aie::INPUT_MOUSE_BUTTON_LEFT))
		m_canLeftClick = true;
	if (!m_canRightClick && input->isMouseButtonUp(aie::INPUT_MOUSE_BUTTON_RIGHT))
		m_canRightClick = true;

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void BreakOutApp::draw() 
{
	clearScreen();
	m_2dRenderer->begin();


	m_2dRenderer->drawSprite(m_textureCannon, cannonVec.x - 90, cannonVec.y - 30, 100, 100, 0.2f);
	m_2dRenderer->drawLine(cannonVec.x + 52, cannonVec.y + 58, mousePos.x, mousePos.y, 4, 100);

	std::string shapeAmount = "Shapes on screen: " + std::to_string(m_userCreatedProjectiles.size());
	m_2dRenderer->drawText(m_fontSmall, shapeAmount.c_str(), 160, 50, 3);
	m_2dRenderer->drawText(m_fontSmall, "Quit: ESC  |  Left Mouse: Shoot a shape  |  Right Mouse: Remove last shape", 160, 10, 2);
	
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	
	
	m_2dRenderer->end();
}

RigidBody* BreakOutApp::getRandomShape(glm::vec2 normal, float distance)
{
	int shapeID			= rand() % BOX + SPHERE;
	float mass			= (float)(rand() % 200 + 20) / 10;
	float bounciness	= (float)(rand() % 80 + 20) / 100;


	if (shapeID == SPHERE)
	{
		float radius = (float)(rand() % 750 + 100) / 100;
		return new Sphere(shootPos, normal * distance, mass*0.8f, bounciness*1.2f, radius);
	}
	else if (shapeID == BOX)
	{
		float rotation	= (float)(rand() % 500 + 1) / 100;
		float width		= (float)(rand() % 60 + 10) / 10;
		float height	= (float)(rand() % 60 + 10) / 10;
		return new Box(shootPos, normal * distance, rotation, mass*2.f, bounciness*0.5f, width, height);
	}

	return nullptr;
}