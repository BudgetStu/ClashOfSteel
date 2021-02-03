#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "ETank.h"
#include "eTurret.h"
#include "Target.h"
#include "PlayerTank.h"


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

private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	eTurret* m_pETurret;
	Target* m_pTarget;
	Tank* m_pEnemyTank;
	
	PlayerTank* m_pPlayerTank;
	
	// UI Items
	
};

#endif /* defined (__PLAY_SCENE__) */