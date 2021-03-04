#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
	SoundManager::Instance().load("../Assets/audio/Bgm.mp3", "Bgm", SOUND_MUSIC);
	SoundManager::Instance().load("../Assets/audio/Exp.wav", "Expl", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Goal.ogg", "Goal", SOUND_SFX);
	SoundManager::Instance().playMusic("Bgm", -1, 0);
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	//for (int i = 0; i <PT.size(); i++)
	//{
	//	PT[i]->draw();
	//}
	
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();

	//Set Enemy turret destination
	m_pETurret[1]->setDestination(m_pPlayerTurret->getTransform()->position);
	m_pETurret[2]->setDestination(m_pPlayerTurret->getTransform()->position);
	m_pETurret[3]->setDestination(m_pPlayerTurret->getTransform()->position);
	m_pETurret[4]->setDestination(m_pPlayerTurret->getTransform()->position);
	m_pETurret[5]->setDestination(m_pPlayerTurret->getTransform()->position);
	m_pETurret[6]->setDestination(m_pPlayerTurret->getTransform()->position);
	m_pETurret[7]->setDestination(m_pPlayerTurret->getTransform()->position);

	//Enemies turret bind
	m_pETurret[1]->getTransform()->position = m_pEnemyTank[1]->getTransform()->position;
	m_pETurret[2]->getTransform()->position = m_pEnemyTank[2]->getTransform()->position;
	m_pETurret[3]->getTransform()->position = m_pEnemyTank[3]->getTransform()->position;
	m_pETurret[4]->getTransform()->position = m_pEnemyTank[4]->getTransform()->position;
	m_pETurret[5]->getTransform()->position = m_pEnemyTank[5]->getTransform()->position;
	m_pETurret[6]->getTransform()->position = m_pEnemyTank[6]->getTransform()->position;
	m_pETurret[7]->getTransform()->position = m_pEnemyTank[7]->getTransform()->position;

	//Player Turret Bind
	m_pPlayerTurret->getTransform()->position = m_pPlayerTank->getTransform()->position;

	//bullet player bind

		if(CollisionManager::CircleAABBTanks(m_pPlayerTank,m_pEnemyTank[1]))
		{
			std::cout << "Collision" << std::endl;
			m_pPlayerTank->setEnabled(false);
			m_pPlayerTurret->setEnabled(false);
			m_pEnemyTank[1]->setEnabled(false);
			m_pETurret[1]->setEnabled(false);
			SoundManager::Instance().playSound("Expl", 0, -1);
		}
	
		if (CollisionManager::CircleAABBTanks(m_pPlayerTank, m_pEnemyTank[2]))
		{
			std::cout << "Collision" << std::endl;
			m_pPlayerTank->setEnabled(false);
			m_pPlayerTurret->setEnabled(false);
			m_pEnemyTank[2]->setEnabled(false);
			m_pETurret[2]->setEnabled(false);
			SoundManager::Instance().playSound("Expl", 0, -1);
		}

		if (CollisionManager::CircleAABBTanks(m_pPlayerTank, m_pEnemyTank[3]))
		{
			std::cout << "Collision" << std::endl;
			m_pPlayerTank->setEnabled(false);
			m_pPlayerTurret->setEnabled(false);
			m_pEnemyTank[3]->setEnabled(false);
			m_pETurret[3]->setEnabled(false);
			SoundManager::Instance().playSound("Expl", 0, -1);
		}

		if (CollisionManager::CircleAABBTanks(m_pPlayerTank, m_pEnemyTank[4]))
		{
			std::cout << "Collision" << std::endl;
			m_pPlayerTank->setEnabled(false);
			m_pPlayerTurret->setEnabled(false);
			m_pEnemyTank[4]->setEnabled(false);
			m_pETurret[4]->setEnabled(false);
			SoundManager::Instance().playSound("Expl", 0, -1);
		}

		if (CollisionManager::CircleAABBTanks(m_pPlayerTank, m_pEnemyTank[5]))
		{
			std::cout << "Collision" << std::endl;
			m_pPlayerTank->setEnabled(false);
			m_pPlayerTurret->setEnabled(false);
			m_pEnemyTank[5]->setEnabled(false);
			m_pETurret[5]->setEnabled(false);
			SoundManager::Instance().playSound("Expl", 0, -1);
		}

		if (CollisionManager::CircleAABBTanks(m_pPlayerTank, m_pEnemyTank[6]))
		{
			std::cout << "Collision" << std::endl;
			m_pPlayerTank->setEnabled(false);
			m_pPlayerTurret->setEnabled(false);
			m_pEnemyTank[6]->setEnabled(false);
			m_pETurret[6]->setEnabled(false);
			SoundManager::Instance().playSound("Expl", 0, -1);
		}

		if (CollisionManager::CircleAABBTanks(m_pPlayerTank, m_pEnemyTank[7]))
		{
			std::cout << "Collision" << std::endl;
			m_pPlayerTank->setEnabled(false);
			m_pPlayerTurret->setEnabled(false);
			m_pEnemyTank[7]->setEnabled(false);
			m_pETurret[7]->setEnabled(false);
			SoundManager::Instance().playSound("Expl", 0, -1);
		}

		m_move();
	
	//for (unsigned i = 0; i < m_bullet.size(); i++) //size() is actual filled numbers of elements
	//	m_bullet[i]->update();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	/*m_mousePosition=EventManager::Instance().getMousePosition()* Util::Deg2Rad;
	m_mousePosition = Util::normalize(m_mousePosition);*/
	//SDL_GetMouseState(&m_mousePosition.x, &m_mousePosition.y);


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

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE))
	{
		m_bullet->setRotation(m_pPlayerTurret->m_rotationAngle) ;
		m_bullet->getTransform()->position = m_pPlayerTurret->getTransform()->position;
		m_bullet->Shoot();
	}

	//if (EventManager::Instance().isKeyDown(SDL_SCANCODE_X))
	//{

	//	for (unsigned i = 0; i < PT.size(); i++)
	//	{
	//		PT.push_back(new Bullet);
	//	}
	//	/*	PT.shrink_to_fit();*/
	//	std::cout << "Created" << std::endl;
	//}



	if(m_pPlayerTank->getTransform()->position.x>700.f)
	{
		SoundManager::Instance().playSound("Goal", 0, -1);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	
	//Labels
	//const SDL_Color blue = { 0, 0, 255, 255 };
	//m_Inst[1] = new Label("Movement Tutorial Instructions:", "Consolas",
	//	20, blue, glm::vec2(200.f, 500.f));
	//m_Inst[1]->setParent(this);
	//addChild(m_Inst[1]);
	//m_Inst[2] = new Label("Pass Through Enemies", "Consolas",
	//	20, blue, glm::vec2(140.f, 520.f));
	//m_Inst[2]->setParent(this);
	//addChild(m_Inst[2]);
	//m_Inst[3] = new Label("ETank Movements:", "Consolas",
	//	20, blue, glm::vec2(110.f, 540.f));
	//m_Inst[3]->setParent(this);
	//addChild(m_Inst[3]);
	//m_Inst[4] = new Label("W=Forward   S=Backwards   A=Rotate Left   D=Rotate Right", "Consolas",
	//	20, blue, glm::vec2(340.f, 560.f));
	//m_Inst[4]->setParent(this);
	//addChild(m_Inst[4]);

	//Tiles
	m_buildGrid();

	//Enemy ETank
	m_pEnemyTank[1] = new ETank();
	m_pEnemyTank[1]->getTransform()->position = m_getTile(3,14)->getTransform()->position+offset;
	m_pEnemyTank[1]->moveRight = true;
	addChild(m_pEnemyTank[1]);

	m_pEnemyTank[2] = new ETank();
	m_pEnemyTank[2]->getTransform()->position = m_getTile(4, 14)->getTransform()->position + offset;
	m_pEnemyTank[2]->moveRight = false;
	addChild(m_pEnemyTank[2]);

	m_pEnemyTank[3] = new ETank();
	m_pEnemyTank[3]->getTransform()->position = m_getTile(15, 0)->getTransform()->position + offset;
	addChild(m_pEnemyTank[3]);

	m_pEnemyTank[4] = new ETank();
	m_pEnemyTank[4]->getTransform()->position = m_getTile(14, 0)->getTransform()->position + offset;
	m_pEnemyTank[4]->moveRight = false;
	addChild(m_pEnemyTank[4]);

	m_pEnemyTank[5] = new ETank();
	m_pEnemyTank[5]->getTransform()->position = m_getTile(0, 5)->getTransform()->position + offset;
	addChild(m_pEnemyTank[5]);

	m_pEnemyTank[6] = new ETank();
	m_pEnemyTank[6]->getTransform()->position = m_getTile(0, 7)->getTransform()->position + offset;
	m_pEnemyTank[6]->moveRight = false;
	addChild(m_pEnemyTank[6]);

	m_pEnemyTank[7] = new ETank();
	m_pEnemyTank[7]->getTransform()->position = m_getTile(19, 6)->getTransform()->position + offset;
	m_pEnemyTank[7]->setRotation(90.0f);
	addChild(m_pEnemyTank[7]);


	// Enemy Turret
	m_pETurret[1] = new eTurret();
	m_pETurret[1]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pETurret[1]);
	
	m_pETurret[2] = new eTurret();
	m_pETurret[2]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pETurret[2]);
	
	m_pETurret[3] = new eTurret();
	m_pETurret[3]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pETurret[3]);

	m_pETurret[4] = new eTurret();
	m_pETurret[4]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pETurret[4]);

	m_pETurret[5] = new eTurret();
	m_pETurret[5]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pETurret[5]);

	m_pETurret[6] = new eTurret();
	m_pETurret[6]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pETurret[6]);

	m_pETurret[7] = new eTurret();
	m_pETurret[7]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pETurret[7]);


	m_bullet = new Bullet();
	m_bullet->getTransform()->position = glm::vec2(1000.0f, 1000.0f);
	m_bullet->setEnabled(true);
	addChild(m_bullet);

	//PlayerTank
	m_pPlayerTank = new PlayerTank();
	m_pPlayerTank->getTransform()->position = glm::vec2(100.0f, 300.0f);
	m_pPlayerTank->setEnabled(true);
	addChild(m_pPlayerTank);

	//Player Turret
	m_pPlayerTurret = new pTurret();
	m_pPlayerTurret->getTransform()->position== glm::vec2(100.0f, 300.0f);
	m_pPlayerTurret->getTransform()->position = m_pPlayerTank->getTransform()->position;
	addChild(m_pPlayerTurret);


}

void PlayScene::GUI_Function() const
{
auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
// Always open with a NewFrame
ImGui::NewFrame();

// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
//ImGui::ShowDemoWindow();

ImGui::Begin("GAME3001 - Lab 3", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

static bool isGridEnabled = false;
if (ImGui::Checkbox("Grid Enabled", &isGridEnabled))
{
	//turn the grid on/off
	m_setGridEnabled(isGridEnabled);
}

//ImGui::Separator();
//
//auto radio = static_cast<int>(currentHeuristic);
//ImGui::LabelText("", "Heuristic Type");
//ImGui::RadioButton("Euclidean", &radio, static_cast<int>(EUCLIDEAN));
//ImGui::SameLine();
//ImGui::RadioButton("Manhattan", &radio, static_cast<int>(MANHATTAN));
//
//if (currentHeuristic != Heuristic(radio))
//{
//	currentHeuristic = Heuristic(radio);
//	m_computeTileCost();
//}
//
//ImGui::Separator();
//
//static int startPosition[] = { m_pShip->getGridPosition().x,m_pShip->getGridPosition().y };
//if (ImGui::SliderInt2("Start Position", startPosition, 0, Config::COL_NUM - 1))
//{
//	//Row adjustment so that it does not go out screen
//	if (startPosition[1] > Config::ROW_NUM - 1)
//	{
//		startPosition[1] = Config::ROW_NUM - 1;
//	}
//	SDL_RenderClear(Renderer::Instance()->getRenderer());
//	m_getTile(m_pShip->getGridPosition())->setTileStatus(UNVISITED);
//	m_pShip->getTransform()->position = m_getTile(startPosition[0], startPosition[1])->getTransform()->position + offset;
//	m_pShip->setGridPosition(startPosition[0], startPosition[1]);
//	m_getTile(m_pShip->getGridPosition())->setTileStatus(START);
//	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
//	SDL_RenderPresent(Renderer::Instance()->getRenderer());
//}
//
//static int targetPosition[] = { m_pTarget->getGridPosition().x,m_pTarget->getGridPosition().y };
//if (ImGui::SliderInt2("Target Position", targetPosition, 0, Config::COL_NUM - 1))
//{
//	//Row adjustment so that it does not go out screen
//	if (targetPosition[1] > Config::ROW_NUM - 1)
//	{
//		targetPosition[1] = Config::ROW_NUM - 1;
//	}
//	SDL_RenderClear(Renderer::Instance()->getRenderer());
//	m_getTile(m_pTarget->getGridPosition())->setTileStatus(UNVISITED);
//	m_pTarget->getTransform()->position = m_getTile(targetPosition[0], targetPosition[1])->getTransform()->position + offset;
//	m_pTarget->setGridPosition(targetPosition[0], targetPosition[1]);
//	m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
//	m_computeTileCost();
//	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
//	SDL_RenderPresent(Renderer::Instance()->getRenderer());
//}
//ImGui::Separator();
//if (ImGui::Button("Start"))
//{
//	isMoving = true;
//}
//
//ImGui::SameLine();
//
//if (ImGui::Button("Reset"))
//{
//	m_reset();
//}
//
//ImGui::Separator();

ImGui::End();

// Don't Remove this
ImGui::Render();
ImGuiSDL::Render(ImGui::GetDrawData());
ImGui::StyleColorsDark();
}

void PlayScene::m_buildGrid()
{

	auto tileSize = Config::TILE_SIZE;

	//Add tiles to the grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = new Tile();//Create empty tile
			tile->getTransform()->position = glm::vec2(col * tileSize, row * tileSize);
			tile->setGridPosition(col, row);
			addChild(tile);
			tile->addLabels();
			tile->setTileCost(tile->getGridPosition().x);
			tile->setTileStatus(tile->getGridPosition().y);
			tile->setEnabled(false);
			m_pGrid.push_back(tile);

		}
	}

	//Create references for each tile to its neighbours
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = m_getTile(col, row);

			//Topmost Row
			if (row == 0)
			{
				tile->setNeighbourTile(TOP_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(TOP_TILE, m_getTile(col, row - 1));
			}

			//Rightmost column
			if (col == Config::COL_NUM - 1)
			{
				tile->setNeighbourTile(RIGHT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(RIGHT_TILE, m_getTile(col + 1, row));

			}

			//BottomMost Row
			if (row == Config::ROW_NUM - 1)
			{
				tile->setNeighbourTile(BOTTOM_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(BOTTOM_TILE, m_getTile(col, row + 1));

			}

			//Leftmost Column
			if (col == 0)
			{
				tile->setNeighbourTile(LEFT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(LEFT_TILE, m_getTile(col - 1, row));
			}
		}

	}

	std::cout << m_pGrid.size() << std::endl;
}
void PlayScene::m_setGridEnabled(bool state) const
{
	for (auto tile : m_pGrid)
	{
		tile->setEnabled(state);
		tile->setLabelsEnabled(state);
	}
	if (state == false)
	{
		SDL_RenderClear(Renderer::Instance()->getRenderer());
	}
}


Tile* PlayScene::m_getTile(const int col, const int row) const
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}

Tile* PlayScene::m_getTile(glm::vec2 grid_position) const
{
	const auto col = grid_position.x;
	const auto row = grid_position.y;
	return m_pGrid[(row * Config::COL_NUM) + col];
}

void PlayScene::m_move()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	//Tank 0
	m_pEnemyTank[1]->move = true;
	if (m_pEnemyTank[1]->move == true)
	{
		if (m_pEnemyTank[1]->p0 == false)
		{
			m_pEnemyTank[1]->setDestination(m_getTile(3, 6)->getTransform()->position + offset);
			if (m_pEnemyTank[1]->getTransform()->position == m_getTile(3, 6)->getTransform()->position + offset)
			{
				m_pEnemyTank[1]->p0 = true;
			}

		}
		else if (m_pEnemyTank[1]->p0 == true)
		{
			m_pEnemyTank[1]->setDestination(m_getTile(9, 6)->getTransform()->position + offset);
			if (m_pEnemyTank[1]->getTransform()->position == m_getTile(9, 6)->getTransform()->position + offset)
			{
				m_pEnemyTank[1]->move = false;
			}
		}

	}
	//Tank 1
	m_pEnemyTank[2]->move = true;
	if (m_pEnemyTank[2]->move == true)
	{
		if (m_pEnemyTank[2]->p0 == false && m_pEnemyTank[2]->p1 == false)
		{
			m_pEnemyTank[2]->setDestination(m_getTile(4, 12)->getTransform()->position + offset);
			if (m_pEnemyTank[2]->getTransform()->position == m_getTile(4, 12)->getTransform()->position + offset)
			{
				m_pEnemyTank[2]->p0 = true;
			}

		}
		else if (m_pEnemyTank[2]->p0 == true)
		{
			m_pEnemyTank[2]->setDestination(m_getTile(16, 12)->getTransform()->position + offset);
			if (m_pEnemyTank[2]->getTransform()->position == m_getTile(16, 12)->getTransform()->position + offset)
			{
				m_pEnemyTank[2]->p0 = false;
				m_pEnemyTank[2]->p1 = true;
			}
		}
		else if (m_pEnemyTank[2]->p1 == true && m_pEnemyTank[2]->p2 == false)
		{
			m_pEnemyTank[2]->setDestination(m_getTile(16, 6)->getTransform()->position + offset);
			if (m_pEnemyTank[2]->getTransform()->position == m_getTile(16, 6)->getTransform()->position + offset)
			{
				m_pEnemyTank[2]->p2 = true;
			}
		}
		else if (m_pEnemyTank[2]->p2 == true)
		{
			m_pEnemyTank[2]->setDestination(m_getTile(10, 6)->getTransform()->position + offset);
			if (m_pEnemyTank[2]->getTransform()->position == m_getTile(10, 6)->getTransform()->position + offset)
			{
				m_pEnemyTank[2]->move = false;
			}
		}
	}
}
