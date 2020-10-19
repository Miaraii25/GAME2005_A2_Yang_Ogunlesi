#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "Target.h"


class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	void SetPoint();

	bool isGravityEnabled = false;
private:
	

	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Target* m_pBall;
	Player* m_pPlayer;
	bool m_playerFacingRight;
	bool m_BallFacingRight;

	bool* m_isGravityEnabled;

	bool m_bPlayerHasLOS;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;
	Target* m_pPoint1;

	glm::vec2 point[3];
	float rampHeight;
	float rampWidth;
	float pointPosX;
	float pointPosY;
	
	
};

#endif /* defined (__PLAY_SCENE__) */