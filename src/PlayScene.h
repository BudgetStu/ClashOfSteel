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

#include "Tile.h"
#include "TiledLevel.h"


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

	float GunCD = 0;
	float StageEndCD = 0;
	int EnemiesDestroyed = 0;
	int TotalBullets = 0;
	int TotalEBullets = 0;
private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;

	glm::vec2 m_Position;
	SDL_Point m_mousePosition;


	//Tiles
	void m_setGridEnabled(bool state) const;
	std::vector<Tile*> m_pGrid;
	void m_buildGrid();
	//Map
	std::map<char, TileC* > m_tiles;
	std::vector<std::vector<TileC*>>m_level;
	std::vector<TileC*> m_obstacles;
	//convenience functions
	Tile* m_getTile(int col, int row) const;
	Tile* m_getTile(glm::vec2 grid_position) const;


	//Labels
	Label* m_Inst[5];

	//Enemy
	ETank* m_pEnemyTank[8];
	eTurret* m_pETurret[8];
	void m_move();
	//Player
	PlayerTank* m_pPlayerTank;
	pTurret* m_pPlayerTurret;
	//Bullets
	std::vector<Bullet*>m_pBullet;
	std::vector<Bullet*>m_pEnemyBullet;
	//Bullet* m_bullet;
	//
	//std::map<char, TileC* > m_tiles;
	//std::vector<std::vector<TileC*>>m_level;

	//std::pmr::vector<PlayerTank*> PT;

	// UI Items

};

#endif /* defined (__PLAY_SCENE__) */