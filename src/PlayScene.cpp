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
	TextureManager::Instance()->load("../Assets/textures/Tiles.png", "tiles");
	SoundManager::Instance().load("../Assets/audio/Bgm.mp3", "Bgm", SOUND_MUSIC);
	SoundManager::Instance().load("../Assets/audio/Exp.wav", "Expl", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Goal.ogg", "Goal", SOUND_SFX);
	SoundManager::Instance().playMusic("Bgm", -1, 0);
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();
	updateDisplayList();

	int mx, my;
	SDL_GetMouseState(&mx, &my);

	//Enemy movements
	m_move();

	//Timer for Cooldowns
	GunCD += 1 * deltaTime;
	for (auto i = 0; i < 8; i++)
	{
		m_pEnemyTank[i]->cd += 1 * deltaTime;
		//std::cout << "Tank " << i << " Cd: " << m_pEnemyTank[i]->cd << std::endl;
	}
	if ((m_pPlayerTank->isEnabled() == false)||(EnemiesDestroyed==8))
	{
		StageEndCD += 1 * deltaTime;
	}
	std::cout << GunCD << std::endl;

	//Set Enemy turret destination
	for (int i = 0; i < 8; i++)
	{
		m_pETurret[i]->setDestination(m_pPlayerTurret->getTransform()->position);
	}

	//Enemies turret bind
	for (int i = 0; i < 8; i++)
	{
		m_pETurret[i]->getTransform()->position = m_pEnemyTank[i]->getTransform()->position;
	}

	//Player Turret Bind
	m_pPlayerTurret->getTransform()->position = m_pPlayerTank->getTransform()->position;

	//Set Player turret destiantion
	m_pPlayerTurret->setDestination(glm::vec2(mx,my));

	//Player Bullet Off Screen
	for (int i = 0; i < m_pBullet.size(); i++)
	{
		if (m_pBullet[i]->getTransform()->position.x >= 800.0f ||
			m_pBullet[i]->getTransform()->position.x <= 0.0f ||
			m_pBullet[i]->getTransform()->position.y >= 600.0f ||
			m_pBullet[i]->getTransform()->position.y <= 0)
		{
			//delete m_pBullet[i];
			m_pBullet[i]->setEnabled(false);
			//m_pBullet.erase(m_pBullet.begin() + i);
			//m_pBullet.shrink_to_fit();
			std::cout << "BOOOOOOM" << std::endl;
			break;
		}
	}

	//Collisions

	//Player and enemy Collision
	for (int EnemyTanks = 0; EnemyTanks < 8; EnemyTanks++)
	{
		if (m_pEnemyTank[EnemyTanks]->isEnabled() == true)
		{
			if (m_pPlayerTank->isEnabled() == true)
			{
				if (CollisionManager::CircleAABBTanks(m_pPlayerTank, m_pEnemyTank[EnemyTanks]))
				{
					std::cout << "Collision" << std::endl;
					m_pPlayerTank->setEnabled(false);
					m_pPlayerTurret->setEnabled(false);
					m_pEnemyTank[EnemyTanks]->setEnabled(false);
					m_pETurret[EnemyTanks]->setEnabled(false);
					SoundManager::Instance().playSound("Expl", 0, -1);
				}
			}
		}
	}

	//Player bullet and enemy tank collision
	for (int i = 0; i < m_pBullet.size(); i++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (m_pEnemyTank[y]->isEnabled() == true)
			{
				if (CollisionManager::CircleAABBTanks(m_pBullet[i], m_pEnemyTank[y]))
				{

					std::cout << "Collision" << std::endl;
					m_pBullet[i]->setEnabled(false);
					m_pEnemyTank[y]->setEnabled(false);
					m_pETurret[y]->setEnabled(false);
					EnemiesDestroyed++;
					SoundManager::Instance().playSound("Expl", 0, -1);
				}
			}
		}
	}

	//Enemy Bullet and player Tank Collision
	if (m_pPlayerTank->isEnabled() == true)
	{
		for (int i = 0; i < m_pEnemyBullet.size(); i++)
		{

			if (CollisionManager::CircleAABBTanks(m_pEnemyBullet[i], m_pPlayerTank))
			{
				std::cout << "Collision" << std::endl;
				m_pEnemyBullet[i]->setEnabled(false);
				m_pPlayerTank->setEnabled(false);
				m_pPlayerTurret->setEnabled(false);
				SoundManager::Instance().playSound("Expl", 0, -1);
			}
		}
	}
}
	

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

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

	//Player BulletShooting

	// (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE) && GunCD > 1) shoot with spacebar
	if (EventManager::Instance().getMouseButton(0) && GunCD > 1)
	{
		if (m_pPlayerTank->isEnabled() == true)
		{
			GunCD = 0;
			m_pBullet.push_back(new Bullet(m_pPlayerTurret->m_rotationAngle, m_pPlayerTurret->getTransform()->position, true));
			addChild(m_pBullet[TotalBullets]);
			TotalBullets++;
		}
	}

	//Enemy BulletShooting
	if (m_pPlayerTank->isEnabled() == true)
	{
		for (int i = 0; i < 8; i++)
		{
			if (m_pEnemyTank[i]->isEnabled() == true)
			{
				if (m_pEnemyTank[i]->cd > 4.0f)
				{
					//TODO Change to a line of sight so that tanks dont try to shoot pass objects
					if (Util::distance(m_pEnemyTank[i]->getTransform()->position, m_pPlayerTank->getTransform()->position) < 150)
					{
						m_pEnemyTank[i]->cd = 0;
						m_pEnemyBullet.push_back(new Bullet(m_pETurret[i]->getRotation(), m_pETurret[i]->getTransform()->position, true));
						addChild(m_pEnemyBullet[TotalEBullets]);
						TotalEBullets++;
						//std::cout << "Tank " << i << " Shoot!" << std::endl;
					}
				}
			}
		}
	}
	
	//Win Condition TODO Update
	if(m_pPlayerTank->isEnabled()==false)
	{
		if(StageEndCD>1)
		{
			TheGame::Instance()->changeSceneState(LOSE_SCENE);
			//SoundManager::Instance().playSound("Goal", 0, -1);
		}
	}
	else if(EnemiesDestroyed==8)
	{
		if (StageEndCD > 1)
		{
			TheGame::Instance()->changeSceneState(WIN_SCENE);
			//SoundManager::Instance().playSound("Goal", 0, -1);
		}
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
	m_pEnemyTank[0] = new ETank();
	m_pEnemyTank[0]->getTransform()->position = m_getTile(1, 1)->getTransform()->position + offset;
	m_pEnemyTank[0]->moveRight = true;
	addChild(m_pEnemyTank[0]);

	m_pEnemyTank[1] = new ETank();
	m_pEnemyTank[1]->getTransform()->position = m_getTile(3, 14)->getTransform()->position + offset;
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
	m_pETurret[0] = new eTurret();
	m_pETurret[0]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pETurret[0]);

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

	//PlayerTank
	m_pPlayerTank = new PlayerTank();
	m_pPlayerTank->getTransform()->position = glm::vec2(100.0f, 300.0f);
	m_pPlayerTank->setEnabled(true);
	addChild(m_pPlayerTank);

	//Player Turret
	m_pPlayerTurret = new pTurret();
	m_pPlayerTurret->getTransform()->position == glm::vec2(100.0f, 300.0f);
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
	//
	std::ifstream inFile("../Assets/data/Tiledata.txt");
	if (inFile.is_open())
	{
		char key;
		int x, y;
		bool obs, haz;
		while (!inFile.eof())
		{
			inFile >> key >> x >> y >> obs >> haz;
			m_tiles.emplace(key, new TileC({ x * tileSize,y * tileSize, }, { 0.0f, 0.0f }, obs, haz));
		}
	}
	inFile.close();
	//
	//inFile.open("../Assets/data/Level1.txt");
	//if (inFile.is_open())
	//{
	//	char key;
	//	//Add tiles to the grid
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

			//	//TODO Try to make this like a basic tile, create a new class derived from it
			//	inFile >> key;
			//	//m_level[row][col] = m_tiles[key]->Clone();
			//	m_level[row][col]->getTransform()->position= glm::vec2(col * tileSize, row * tileSize);
			//	if (m_level[row][col]->IsObstacle())
			//		if (m_level[row][col]->IsObstacle())
			//		{
			//			m_obstacles.push_back(m_level[row][col]);
			//		}
			//	m_level[row][col]->draw();
			//	m_level[row][col]->setEnabled(true);
			//	addChild(m_level[row][col]);
			}
		}
	//}
	//inFile.close();
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
	//Tank 3 //TODO
}