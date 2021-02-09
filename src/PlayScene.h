#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Bullet.h"
#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "ETank.h"
#include "eTurret.h"
#include "Target.h"
#include "PlayerTank.h"
#include "pTurret.h"
#include <vector>


class pTurret;

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
	
	glm::vec2 m_Position;
	SDL_Point m_mousePosition;

	Label* m_Inst[5];
	
	eTurret* m_pETurret[8];
	//Target* m_pTarget;
	Tank* m_pEnemyTank[8];
	
	PlayerTank* m_pPlayerTank;
	pTurret* m_pPlayerTurret;

	Bullet* m_bullet;
	//std::pmr::vector<Bullet*> m_bullet;
	std::pmr::vector<PlayerTank*> PT;
	
	// UI Items
	
};

#endif /* defined (__PLAY_SCENE__) */