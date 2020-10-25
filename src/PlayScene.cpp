#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"
#include <algorithm>

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("playscene", 400, 300, 0, 255, true);

	// draw the ramp
	auto LOSColour = (!m_bPlayerHasLOS) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	Util::DrawLine(point[0], point[1], LOSColour);
	Util::DrawLine(point[1], point[2], LOSColour);
	Util::DrawLine(point[2], point[0], LOSColour);
	Util::DrawLine(point[0], glm::vec2(point[0].x + 500.0f, point[0].y), LOSColour);

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);

	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}
}

void PlayScene::update()
{
	/*if (m_pBall->doThrow())
	{
		if (!Addfriction) friction = 0.0f;
		float xAccerlation = m_pBall->Mass * m_pBall->Gravity * cos(theta);
		float yAccerlation = m_pBall->Mass * m_pBall->Gravity * sin(theta);

		if (m_pBall->getTransform()->position.y >= point[1].y - m_pBall->getHeight() / 2)
		{
			theta = 0.0f;
			yAccerlation = 0.0f;
			m_pBall->getRigidBody()->velocity.y = 0.0f;
			m_pBall->Rotation = 0.0f;
			xAccerlation = -(friction * m_pBall->Mass * m_pBall->Gravity);

			if (m_pBall->getRigidBody()->velocity.x <= 0)
			{
				xAccerlation = 0;
				m_pBall->getRigidBody()->velocity.x = 0.0f;
			}
		}

	}*/

	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	// Load background
	TextureManager::Instance()->load("../Assets/backgrounds/playscene.png", "playscene");

	// Set GUI Title
	m_guiTitle = "Play Scene";

	//Ball
	m_pBall = new Target();
	m_pBall->Mass = 12.8f;
	m_pBall->Gravity = 9.81f;
	addChild(m_pBall);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	rampHeight = 300.0f;
	rampWidth = 300.0f;
	pointPosX = 300.0f;
    pointPosY = 400.0f;

	SetPoint();
	

	//// Back Button
	//m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	//m_pBackButton->getTransform()->position = glm::vec2(300.0f, 400.0f);
	//m_pBackButton->addEventListener(CLICK, [&]()-> void
	//{
	//	m_pBackButton->setActive(false);
	//	TheGame::Instance()->changeSceneState(START_SCENE);
	//});

	//m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	//{
	//	m_pBackButton->setAlpha(128);
	//});

	//m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	//{
	//	m_pBackButton->setAlpha(255);
	//});
	//addChild(m_pBackButton);

	//// Next Button
	//m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	//m_pNextButton->getTransform()->position = glm::vec2(500.0f, 400.0f);
	//m_pNextButton->addEventListener(CLICK, [&]()-> void
	//{
	//	m_pNextButton->setActive(false);
	//	TheGame::Instance()->changeSceneState(END_SCENE);
	//});

	//m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	//{
	//	m_pNextButton->setAlpha(128);
	//});

	//m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	//{
	//	m_pNextButton->setAlpha(255);
	//});

	//addChild(m_pNextButton);

	/* Instructions Label */
	const SDL_Color Gold = { 212,175, 55, 0 };
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas", 20, Gold);
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);
	addChild(m_pInstructionsLabel);

	
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	//See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Physics Control", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if (ImGui::Button("Play")) {
		SetPoint();
		m_pBall->doThrow();
		m_pBall->throwPosition = glm::vec2(pointPosX, pointPosY);
	}

	ImGui::Separator();

	/*static bool isGravityEnabled = false;
	if (ImGui::Checkbox("Gravity", &isGravityEnabled)) {
		m_pBall->isGravityEnabled = isGravityEnabled;
	}*/
	
	/*static int xPlayerPos = 300;
	if (ImGui::SliderInt("Player Position X", &xPlayerPos, 0, 800)) {
		m_pPlayer->getTransform()->position.x = xPlayerPos;
		m_pBall->throwPosition = glm::vec2(xPlayerPos, 435);
	}*/
	
	/*static float velocity[2] = { 10, 10 };
	if (ImGui::SliderFloat2("Throw Speed", velocity, 0, 100)) {
		m_pBall->throwSpeed = glm::vec2(velocity[0], -velocity[1]);
	}*/

	if (ImGui::SliderFloat("Position", &pointPosX, 0, Config::SCREEN_WIDTH))
		SetPoint();

	if (ImGui::SliderFloat("Ramp Height", &rampHeight, 0, 300))
	    SetPoint();

	if (ImGui::SliderFloat("Ramp Width", &rampWidth, 0, 400))
		SetPoint();

	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

void PlayScene::SetPoint()
{
	point[0] = glm::vec2(pointPosX, pointPosY);
	point[1] = glm::vec2(pointPosX + rampWidth, pointPosY);
	point[2] = glm::vec2(pointPosX, pointPosY - rampHeight);

	theta = atan(rampHeight / rampWidth);

	m_pBall->Rotation = glm::degrees(theta);

	m_pBall->getTransform()->position = glm::vec2(point[2].x + m_pBall->getWidth() / 2.5f, point[2].y - m_pBall->getHeight() / 2.5f);
}



