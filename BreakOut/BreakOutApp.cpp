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
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	m_2dRenderer = new aie::Renderer2D();
	m_font		 = new aie::Font("../bin/font/consolas.ttf", 24);				//TODO: remember to change this when redistributing a build! The following path would be used instead: "./font/consolas.ttf"
	m_cannon	 = new aie::Texture("../bin/textures/cannonWhite.png");

	m_physicsScene = new PhysicsScene(m_gravity, m_timeStep);

	//Planes
	m_physicsScene->addActor(new Plane(vec2(0, -1), 90, 0.75f, m_blueblue));	//Ground plane
	m_physicsScene->addActor(new Plane(vec2(-1, 0), 85, 0.75f, m_blueblue));	//Left plane
	m_physicsScene->addActor(new Plane(vec2(1, 0), 90, 0.75f, m_blueblue));		//Right plane
	m_physicsScene->addActor(new Plane(vec2(0, 1), 50, 0.75f, m_blueblue));		//Sky plane

	//Static Rigidbodies
	m_physicsScene->addActor(new Box(vec2( 70, 20), m_zeroVec, 2, 3, 0.45f, vec2(8, 8), m_lightBlue, true));
	m_physicsScene->addActor(new Box(vec2(-20, 30), m_zeroVec, 0, 5, 0.50f, vec2(16, 4), m_lightBlue, true));
	m_physicsScene->addActor(new Sphere(vec2(10, -20), m_zeroVec, 4, 0.8f, 6, m_green, true));

	//Rope
	int NUMBER_OF_BALLS = 34;
	float radius = 1.5f;

	Sphere* ball1, * ball2;
	vec2 lastPosition = vec2(-83.4, -40), addVec = vec2(radius * 3.4, 0);

	ball1 = new Sphere(lastPosition, m_zeroVec, 1.0f, 0.4f, radius, m_darkBlue, true);
	m_physicsScene->addActor(ball1);

	for (int i = 0; i < NUMBER_OF_BALLS; i++)
	{
		ball2 = new Sphere(lastPosition += addVec, m_zeroVec, 1.0f, 0.4f, radius, m_darkBlue);		//Create a horizontal rope out of small Spheres near the bottom of the screen
		if (i == NUMBER_OF_BALLS - 1)
			ball2->setStatic(true);

		m_physicsScene->addActor(ball2);
		m_physicsScene->addActor(new Spring(ball1, ball2, radius * 0.48f, 96, 0.8f));
		
		ball1 = ball2;
	}

	srand((unsigned int)time(NULL));
	return true;
}

void BreakOutApp::shutdown() 
{
	delete m_2dRenderer;
	delete m_font;
	delete m_cannon;

	delete m_physicsScene;
	m_userCreatedProjectiles.clear();
}

void BreakOutApp::update(float deltaTime) 
{
	aie::Input* input = aie::Input::getInstance();
	aie::Gizmos::clear();

	m_leftClickDelayTimer += deltaTime;
	m_rightClickDelayTimer += deltaTime;
	m_physicsScene->update(deltaTime);
	m_physicsScene->draw();

	m_mousePos = vec2((float)input->getMouseX(), (float)input->getMouseY());

	//Create a random shape once per left mouse click that moves towards the mousePos, with a velocity that increases with the distance from shapeSpawn
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT) && m_userCreatedProjectiles.size() < 10 && m_leftClickDelayTimer > LEFT_CLICK_DELAY)
	{
		m_leftClickDelayTimer = 0;
		if (m_canLeftClick)
		{
			m_canLeftClick = false;
			
			RigidBody* shape = getRandomShape(normalize(m_mousePos - m_shapeSpawn), distance(m_mousePos, m_shapeSpawn) / 3);
			m_userCreatedProjectiles.push_back(shape);
			m_physicsScene->addActor(shape);
		}
	}

	//Delete last user created shape once per right mouse click
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT) && m_userCreatedProjectiles.size() > 0 && m_rightClickDelayTimer > RIGHT_CLICK_DELAY)
	{
		m_rightClickDelayTimer = 0;
		if (m_canRightClick)
		{
			m_canRightClick = false;
			
			m_physicsScene->removeActor(m_userCreatedProjectiles.back());
			m_userCreatedProjectiles.pop_back();
		}
	}

	if (!m_canLeftClick && input->isMouseButtonUp(aie::INPUT_MOUSE_BUTTON_LEFT))	m_canLeftClick = true;
	if (!m_canRightClick && input->isMouseButtonUp(aie::INPUT_MOUSE_BUTTON_RIGHT))	m_canRightClick = true;

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE)) quit();
}

void BreakOutApp::draw() 
{
	clearScreen();
	m_2dRenderer->begin();


	m_2dRenderer->drawSprite(m_cannon, m_shapeSpawn.x - 90, m_shapeSpawn.y - 30, 100, 100, 0.2f);				//Draw cannon Icon
	m_2dRenderer->drawLine(m_shapeSpawn.x + 52, m_shapeSpawn.y + 58, m_mousePos.x, m_mousePos.y, 4, 100);		//Draw line from shape spawn point to mouse position

	std::string shapeAmount = "Shapes on screen: " + std::to_string(m_userCreatedProjectiles.size());
	m_2dRenderer->drawText(m_font, shapeAmount.c_str(), 160, 50, 3);
	m_2dRenderer->drawText(m_font, "Quit: ESC  |  Left Mouse: Shoot a shape  |  Right Mouse: Remove last shape", 160, 10, 2);
	
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	
	
	m_2dRenderer->end();
}

//Generates a RigidBody to return with random variables moving in a direction specified by the normal parameter.
RigidBody* BreakOutApp::getRandomShape(glm::vec2 normal, float distance)
{
	int shapeID	= rand() % BOX + SPHERE;
	float mass = (float)(rand() % 200 + 20) / 10,	bounciness = (float)(rand() % 80 + 20) / 100;


	if (shapeID == SPHERE)
	{
		float radius = (float)(rand() % 750 + 100) / 100;
		return new Sphere(m_shapeSpawnWorld, normal * distance, mass * 0.8f, bounciness * 1.2f, radius, m_green);
	}
	else
	{
		float rotation	= (float)(rand() % 500 + 1) / 100,		width = (float)(rand() % 60 + 10) / 10,		height = (float)(rand() % 60 + 10) / 10;
		return new Box(m_shapeSpawnWorld, normal * distance, rotation, mass * 2.f, bounciness * 0.5f, vec2(width, height), m_lightBlue);
	}
}