#pragma once
#ifndef _TILED_LEVEL_
#define _TILED_LEVEL_

#include <fstream>
#include <map>
#include <vector>

#include "NavigationObject.h"
#include "Tile.h"

class TileC : public NavigationObject
{
public:
	TileC(glm::vec2 p, glm::vec2 d, bool o, bool h)
		: m_obstacle(o), m_hazard(h) {}
	TileC* Clone() { return new TileC(this->getTransform()->position, this->getTransform()->scale, m_obstacle, m_hazard); }
	bool IsObstacle() { return m_obstacle; }
	void SetXY(float x, float y) { this->getTransform()->position.x = x; this->getTransform()->position.y = y; }

	void draw() override {};
	void update() override {};
	void clean() override {};
private:
	bool m_obstacle, m_hazard;
};

class TiledLevel :public NavigationObject
{
public:
	TiledLevel(const unsigned short column, const unsigned short row,
		const char* tileData, const char* levelData, const char* tileKey);
	~TiledLevel();

	void draw() override;
	void update() override {};
	void clean() override;

private:
	const char* m_tileKey;
	int m_row, m_col;
	std::map<char, TileC* > m_tiles;
	//std::vector<std::vector<TileC*>>m_level;
	std::vector<TileC*> m_obstacles;
};

#endif


