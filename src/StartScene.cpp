#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	TextureManager::Instance()->draw("background", 400, 300, 0, 255, true);
	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void StartScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/Background.png", "background");

	const SDL_Color Orange = { 255, 140, 0, 0 };
	m_pStartLabel = new Label("START SCENE", "Dock51", 80, Orange, glm::vec2(400.0f, 80.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	m_pNameLabel = new Label("MINGKUN YANG (ID:101235517)","PinkChicken-Regular", 40, Orange, glm::vec2(400.0f, 180.0f));
	m_pNameLabel->setParent(this);
	addChild(m_pNameLabel);

	m_pNameLabel2 = new Label("MARIAM OGUNLESI (ID:101285729)","PinkChicken-Regular", 40, Orange, glm::vec2(400.0f, 230.0f));
	m_pNameLabel2->setParent(this);
	addChild(m_pNameLabel2);

	/*m_pShip = new Ship();
	m_pShip->getTransform()->position = glm::vec2(400.0f, 300.0f); 
	addChild(m_pShip); */

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(400.0f, 400.0f); 

	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});
	
	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);

	
}

