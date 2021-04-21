#include "PlayScene2.h"
#include "Game.h"
#include "EventManager.h"


// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene2::PlayScene2()
{
	PlayScene2::start();

	TextureManager::Instance()->load("../Assets/grid/Bg.png", "Bg");
	TextureManager::Instance()->load("../Assets/textures/Tiles.png", "tiles");
	SoundManager::Instance().load("../Assets/audio/Bgm.mp3", "Bgm", SOUND_MUSIC);
	SoundManager::Instance().load("../Assets/audio/Exp.wav", "Expl", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Goal.ogg", "Goal", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Ff.mp3", "ff", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Ef.mp3", "ef", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/eE.mp3", "ee", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/fE.mp3", "fe", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/sS.mp3", "ss", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/sE.mp3", "se", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/cE.mp3", "ce", SOUND_SFX);
	SoundManager::Instance().playMusic("Bgm", -1, 0);
	Game::Instance()->currentStage = 2;
}

PlayScene2::~PlayScene2()
= default;

void PlayScene2::draw()
{

	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene2::update()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();
	updateDisplayList();

	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	
	//Enemy movevents
	m_move();

	//Timer for Cooldowns
	GameTimer += 1 * deltaTime;
	GunCD += 1 * deltaTime;
	ChangeGun += 1 * deltaTime;
	for (auto i = 0; i < totalEnemies; i++)
	{
		m_pEnemyTank[i]->cd += 1 * deltaTime;
		m_pEnemyTank[i]->avocd += 1 * deltaTime;
	}
	if ((m_pPlayerTank->isEnabled() == false) || (EnemiesDestroyed == 8))
	{
		StageEndCD += 1 * deltaTime;
	}
	for (int i=0;i<TotalFire;i++)
	{
		if(m_pFire[i]->on==true)
		{
			fire += 1 * deltaTime;
			if (fire > 1)
			{
				m_pFire[i]->setEnabled(false);
				m_pFire[i]->on = false;
				fire = 0;
			}
		}
	}
	std::cout << GameTimer << std::endl;

	//Los check
	for (auto i = 0; i < totalEnemies; i++)
	{
		if ((m_pEnemyTank[i]->getTransform()->position.x > 0.0f) && (m_pEnemyTank[i]->getTransform()->position.x < 800.0f))
		{
			if ((m_pEnemyTank[i]->getTransform()->position.y > 0.0f) && (m_pEnemyTank[i]->getTransform()->position.y < 600.0f))
			{
				m_CheckShipLOS(m_pETurret[i]);
				if (m_pETurret[i]->hasLOS())
				{
					m_pEnemyTank[i]->seek = true;
				}
			}
		}
	}

	//Set Enemy turret destination
	for (int i = 0; i < totalEnemies; i++)
	{
		//TODO Probably Needs to add an offset
		m_pETurret[i]->setDestination(m_pPlayerTurret->getTransform()->position);
	}

	//Enemies turret bind
	for (int i = 0; i < totalEnemies; i++)
	{
		m_pETurret[i]->getTransform()->position = m_pEnemyTank[i]->getTransform()->position;
	}

	//Player Turret Bind
	m_pPlayerTurret->getTransform()->position = m_pPlayerTank->getTransform()->position;
	m_pExtraTurret->getTransform()->position = m_pPlayerTank->getTransform()->position;

	//Set Player turret destiantion
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	m_pPlayerTurret->setDestination(glm::vec2(mx, my));
	m_pExtraTurret->setDestination(glm::vec2(mx, my));

	//Player Bullet Off Screen
	for (int i = 0; i < m_pBullet.size(); i++)
	{
		if (m_pBullet[i]->getTransform()->position.x >= 800.0f ||
			m_pBullet[i]->getTransform()->position.x <= 0.0f ||
			m_pBullet[i]->getTransform()->position.y >= 600.0f ||
			m_pBullet[i]->getTransform()->position.y <= 0)
		{
			m_pBullet[i]->setEnabled(false);
			break;
		}
	}

	//Extra Weapon grab
	if (extraDrop->isEnabled())
	{
		if (Util::distance(m_pPlayerTank->getTransform()->position, extraDrop->getTransform()->position) < 20.0f)
		{
			extraDrop->setEnabled(false);
			m_pPlayerTank->hasExtraWeapon = true;
		}
	}

	//Collisions

	//Player and stage Collision

	if (m_pPlayerTank->isEnabled())
	{
		for (int i = 0; i < totalBuildings; i++)
		{
			if (CollisionManager::CircleAABBTanks(m_pPlayerTank, m_field[i]))
			{
				float xLess = m_field[i]->getTransform()->position.x - m_pPlayerTank->getTransform()->position.x;//collision right
				float xLess2 = m_pPlayerTank->getTransform()->position.x - m_field[i]->getTransform()->position.x;//collision left
				float yLess = m_field[i]->getTransform()->position.y - m_pPlayerTank->getTransform()->position.y;//collision down
				float yLess2 = m_pPlayerTank->getTransform()->position.y - m_field[i]->getTransform()->position.y;//collision up

				//RightCollision
				if ((xLess > xLess2) && (xLess > yLess) && (xLess > yLess2))
					m_pPlayerTank->getTransform()->position.x = m_pPlayerTank->getTransform()->position.x - 2.0f;
				//LeftCollision
				else if ((xLess2 > xLess) && (xLess2 > yLess) && (xLess2 > yLess2))
					m_pPlayerTank->getTransform()->position.x = m_pPlayerTank->getTransform()->position.x + 2.0f;
				//DownCollision
				else if ((yLess > xLess) && (yLess > xLess2) && (yLess > yLess2))
					m_pPlayerTank->getTransform()->position.y = m_pPlayerTank->getTransform()->position.y - 2.0f;
				//UpCollision
				else if ((yLess2 > xLess) && (yLess2 > xLess2) && (yLess2 > yLess))
					m_pPlayerTank->getTransform()->position.y = m_pPlayerTank->getTransform()->position.y + 2.0f;
			}
		}
	}

	//Enemy and stage Collision
	for (int e=0;e<totalEnemies;e++)
	{
		if (m_pEnemyTank[e]->isEnabled())
		{
			if(m_pEnemyTank[e]->seek==true)
			{
				for (int i = 0; i < totalBuildings; i++)
				{
					if (CollisionManager::CircleAABBTanks(m_pEnemyTank[e], m_field[i]))
					{
						float xLess = m_field[i]->getTransform()->position.x - m_pEnemyTank[e]->getTransform()->position.x;//collision right
						float xLess2 = m_pEnemyTank[e]->getTransform()->position.x - m_field[i]->getTransform()->position.x;//collision left
						float yLess = m_field[i]->getTransform()->position.y - m_pEnemyTank[e]->getTransform()->position.y;//collision down
						float yLess2 = m_pEnemyTank[e]->getTransform()->position.y - m_field[i]->getTransform()->position.y;//collision up

						//RightCollision
						if ((xLess > xLess2) && (xLess > yLess) && (xLess > yLess2))
							m_pEnemyTank[e]->getTransform()->position.x = m_pEnemyTank[e]->getTransform()->position.x - 2.0f;
						//LeftCollision
						else if ((xLess2 > xLess) && (xLess2 > yLess) && (xLess2 > yLess2))
							m_pEnemyTank[e]->getTransform()->position.x = m_pEnemyTank[e]->getTransform()->position.x + 2.0f;
						//DownCollision
						else if ((yLess > xLess) && (yLess > xLess2) && (yLess > yLess2))
							m_pEnemyTank[e]->getTransform()->position.y = m_pEnemyTank[e]->getTransform()->position.y - 2.0f;
						//UpCollision
						else if ((yLess2 > xLess) && (yLess2 > xLess2) && (yLess2 > yLess))
							m_pEnemyTank[e]->getTransform()->position.y = m_pEnemyTank[e]->getTransform()->position.y + 2.0f;
					}
				}
			}
		}
	}

	//Enemy and enemy //TODO could improve
	for (int e = 0; e < totalEnemies; e++)
	{
		if (m_pEnemyTank[e]->isEnabled())
		{
			if (m_pEnemyTank[e]->seek == true)
			{
				for (int i = 0; i < totalEnemies; i++)
				{
					if (m_pEnemyTank[i]->seek == true)
					{
						if (CollisionManager::CircleAABBTanks(m_pEnemyTank[e], m_field[i]))
						{
							float xLess = m_pEnemyTank[i]->getTransform()->position.x - m_pEnemyTank[e]->getTransform()->position.x;//collision right
							float xLess2 = m_pEnemyTank[e]->getTransform()->position.x - m_pEnemyTank[i]->getTransform()->position.x;//collision left
							float yLess = m_pEnemyTank[i]->getTransform()->position.y - m_pEnemyTank[e]->getTransform()->position.y;//collision down
							float yLess2 = m_pEnemyTank[e]->getTransform()->position.y - m_pEnemyTank[i]->getTransform()->position.y;//collision up

							//RightCollision
							if ((xLess > xLess2) && (xLess > yLess) && (xLess > yLess2))
								m_pEnemyTank[e]->getTransform()->position.x = m_pEnemyTank[e]->getTransform()->position.x - 2.0f;
							//LeftCollision
							else if ((xLess2 > xLess) && (xLess2 > yLess) && (xLess2 > yLess2))
								m_pEnemyTank[e]->getTransform()->position.x = m_pEnemyTank[e]->getTransform()->position.x + 2.0f;
							//DownCollision
							else if ((yLess > xLess) && (yLess > xLess2) && (yLess > yLess2))
								m_pEnemyTank[e]->getTransform()->position.y = m_pEnemyTank[e]->getTransform()->position.y - 2.0f;
							//UpCollision
							else if ((yLess2 > xLess) && (yLess2 > xLess2) && (yLess2 > yLess))
								m_pEnemyTank[e]->getTransform()->position.y = m_pEnemyTank[e]->getTransform()->position.y + 2.0f;

						}
					}
				}
			}
		}
	}

	//Player and enemy Collision
	for (int EnemyTanks = 0; EnemyTanks < totalEnemies; EnemyTanks++)
	{
		if (m_pEnemyTank[EnemyTanks]->isEnabled() == true)
		{
			if (m_pPlayerTank->isEnabled() == true)
			{
				if (CollisionManager::CircleAABBTanks(m_pPlayerTank, m_pEnemyTank[EnemyTanks]))
				{
					m_pPlayerTank->setEnabled(false);
					if (m_pPlayerTurret->isEnabled() == true)
						m_pPlayerTurret->setEnabled(false);
					if (m_pExtraTurret->isEnabled() == true)
						m_pExtraTurret->setEnabled(false);
					m_pEnemyTank[EnemyTanks]->setEnabled(false);
					m_pETurret[EnemyTanks]->setEnabled(false);
					SoundManager::Instance().playSound("fe", 0, -1);
				}
			}
		}
	}

	//Player bullet and enemy tank collision
	for (int i = 0; i < m_pBullet.size(); i++)
	{
		for (int y = 0; y < totalEnemies; y++)
		{
			if (m_pBullet[i]->isEnabled())
			{
				if ((m_pEnemyTank[y]->getTransform()->position.x > 0.0f) && (m_pEnemyTank[y]->getTransform()->position.x < 800.0f))
				{
					if ((m_pEnemyTank[y]->getTransform()->position.y > 0.0f) && (m_pEnemyTank[y]->getTransform()->position.y < 600.0f))
					{
						if (m_pEnemyTank[y]->isEnabled() == true)
						{
							if (CollisionManager::CircleAABBTanks(m_pBullet[i], m_pEnemyTank[y]))
							{
								m_pBullet[i]->setEnabled(false);
								m_pEnemyTank[y]->setEnabled(false);
								m_pETurret[y]->setEnabled(false);
								EnemiesDestroyed++;
								SoundManager::Instance().playSound("ee", 0, -1);//TODO Change
							}
						}
					}
				}
			}
		}
	}

	//Player extra bullet and enemy tank collision
	for (int i = 0; i < m_pExtraBullet.size(); i++)
	{
		for (int y = 0; y < totalEnemies; y++)
		{
			if (m_pExtraBullet[i]->isEnabled())
			{
				if ((m_pEnemyTank[y]->getTransform()->position.x > 0.0f) && (m_pEnemyTank[y]->getTransform()->position.x < 800.0f))
				{
					if ((m_pEnemyTank[y]->getTransform()->position.y > 0.0f) && (m_pEnemyTank[y]->getTransform()->position.y < 600.0f))
					{
						if (m_pEnemyTank[y]->isEnabled() == true)
						{
							if (CollisionManager::CircleAABBTanks(m_pExtraBullet[i], m_pEnemyTank[y]))
							{
								//int timer = GameTimer;
								m_pFire.push_back(new TileC("../Assets/textures/boom.png", "boom"));
								m_pFire[i]->getTransform()->position = m_pExtraBullet[i]->getTransform()->position;
								addChild(m_pFire[i], 4);
								m_pFire[i]->on = true;
								TotalFire++;
								SoundManager::Instance().playSound("se", 0, -1);
								aoeDamage(m_pExtraBullet[i]);
								m_pExtraBullet[i]->setEnabled(false);
								
							}
						}
					}
				}
			}
		}
	}

	//Player bullet and Stage collision
	for (int i = 0; i < m_pBullet.size(); i++)
	{
		for (int y = 0; y < totalBuildings; y++)
		{
			if (m_pBullet[i]->isEnabled())
			{
				if (CollisionManager::CircleAABBTanks(m_pBullet[i], m_field[y]))
				{
					m_pBullet[i]->setEnabled(false);
					SoundManager::Instance().playSound("ce", 0, -1);//TODO Change
				}
			}
		}
	}

	//PlayerExtra bullet and Stage collision //TODO
	for (int i = 0; i < m_pExtraBullet.size(); i++)
	{
		for (int y = 0; y < totalBuildings; y++)
		{
			if (m_pExtraBullet[i]->isEnabled())
			{
				if (CollisionManager::CircleAABBTanks(m_pExtraBullet[i], m_field[y]))
				{
					int timer = GameTimer;
					m_pFire.push_back(new TileC("../Assets/textures/boom.png", "boom"));
					m_pFire[i]->getTransform()->position = m_pExtraBullet[i]->getTransform()->position;
					addChild(m_pFire[i], 4);
					m_pFire[i]->on = true;
					TotalFire++;
					SoundManager::Instance().playSound("se", 0, -1);
					aoeDamage(m_pExtraBullet[i]);
					m_pExtraBullet[i]->setEnabled(false);
					
				}
			}
		}
	}

	//Enemy Bullet and player Tank Collision
	if (m_pPlayerTank->isEnabled() == true)
	{
		for (int i = 0; i < m_pEnemyBullet.size(); i++)
		{
			if (m_pEnemyBullet[i]->isEnabled())
			{
				if (CollisionManager::CircleAABBTanks(m_pEnemyBullet[i], m_pPlayerTank))
				{
					m_pEnemyBullet[i]->setEnabled(false);
					m_pPlayerTank->setEnabled(false);
					if(m_pPlayerTurret->isEnabled()==true)
						m_pPlayerTurret->setEnabled(false);
					if (m_pExtraTurret->isEnabled() == true)
						m_pExtraTurret->setEnabled(false);
					SoundManager::Instance().playSound("fe", 0, -1);//TODO CHANGE
				}
			}
		}
	}

	//Enemy Bullet and stage Collision

	for (int i = 0; i < m_pEnemyBullet.size(); i++)
	{
		if (m_pEnemyBullet[i]->isEnabled() == true)
		{
			for (int u = 0; u < totalBuildings; u++)
			{
				if (CollisionManager::CircleAABBTanks(m_pEnemyBullet[i], m_field[u]))
				{
					m_pEnemyBullet[i]->setEnabled(false);

					SoundManager::Instance().playSound("ce", 0, -1);//TODO CHANGE
				}
			}
		}
	}

	//Avoidance TODO
	for (int EnemyTanks = 0; EnemyTanks < totalEnemies; EnemyTanks++)
	{
		if (m_pEnemyTank[EnemyTanks]->seek == true)
		{
			for (int y = 0; y < totalEnemies; y++)
			{
				//Left whishker
				if (CollisionManager::lineRectCheck(m_pEnemyTank[EnemyTanks]->m_LWhishker.Start(),
					m_pEnemyTank[EnemyTanks]->m_LWhishker.End(), m_field[y]->getTransform()->position -
					glm::vec2(m_field[y]->getWidth() / 2, m_field[y]->getHeight() / 2),
					m_field[y]->getWidth(), m_field[y]->getHeight()))
				{
					m_pEnemyTank[EnemyTanks]->ColObsL = y;
					m_pEnemyTank[EnemyTanks]->tRight = true;
					m_pEnemyTank[EnemyTanks]->avoidance = true;
				}
				if (m_pEnemyTank[EnemyTanks]->ColObsL != 100)
				{
					if (!(CollisionManager::lineRectCheck(m_pEnemyTank[EnemyTanks]->m_LWhishker.Start(),
						m_pEnemyTank[EnemyTanks]->m_LWhishker.End(), m_field[m_pEnemyTank[EnemyTanks]->ColObsL]->getTransform()->position -
						glm::vec2(m_field[m_pEnemyTank[EnemyTanks]->ColObsL]->getWidth() / 2, m_field[m_pEnemyTank[EnemyTanks]->ColObsL]->getHeight() / 2),
						m_field[m_pEnemyTank[EnemyTanks]->ColObsL]->getWidth(), m_field[m_pEnemyTank[EnemyTanks]->ColObsL]->getHeight())))
					{
						m_pEnemyTank[EnemyTanks]->tRight = false;
						m_pEnemyTank[EnemyTanks]->avocd = 0;
						if (m_pEnemyTank[EnemyTanks]->avocd > 1)
						{
							m_pEnemyTank[EnemyTanks]->avoidance = false;
							m_pEnemyTank[EnemyTanks]->ColObsL = 100;
						}
					}
				}
				//Right Whishker
				if (CollisionManager::lineRectCheck(m_pEnemyTank[EnemyTanks]->m_RWhishker.Start(),
					m_pEnemyTank[EnemyTanks]->m_RWhishker.End(), m_field[y]->getTransform()->position -
					glm::vec2(m_field[y]->getWidth() / 2, m_field[y]->getHeight() / 2),
					m_field[y]->getWidth(), m_field[y]->getHeight()))
				{
					m_pEnemyTank[EnemyTanks]->ColObsR = y;
					m_pEnemyTank[EnemyTanks]->avoidance = true;
					m_pEnemyTank[EnemyTanks]->tLeft = true;
				}
				if (m_pEnemyTank[EnemyTanks]->ColObsR != 100)
				{
					if (!(CollisionManager::lineRectCheck(m_pEnemyTank[EnemyTanks]->m_RWhishker.Start(),
						m_pEnemyTank[EnemyTanks]->m_RWhishker.End(), m_field[m_pEnemyTank[EnemyTanks]->ColObsR]->getTransform()->position -
						glm::vec2(m_field[m_pEnemyTank[EnemyTanks]->ColObsR]->getWidth() / 2, m_field[m_pEnemyTank[EnemyTanks]->ColObsR]->getHeight() / 2),
						m_field[m_pEnemyTank[EnemyTanks]->ColObsR]->getWidth(), m_field[m_pEnemyTank[EnemyTanks]->ColObsR]->getHeight())))
					{
						m_pEnemyTank[EnemyTanks]->tLeft = false;
						m_pEnemyTank[EnemyTanks]->avocd = 0;
						if (m_pEnemyTank[EnemyTanks]->avocd > 1)
						{
							m_pEnemyTank[EnemyTanks]->avoidance = false;
							m_pEnemyTank[EnemyTanks]->ColObsL = 100;
						}
					}
				}
			}
		}
	}
}


void PlayScene2::clean()
{
	removeAllChildren();
}

void PlayScene2::handleEvents()
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

	//Player BulletShooting //

	if (EventManager::Instance().getMouseButton(0) && GunCD > 1)
	{
		if (m_pPlayerTank->isEnabled() == true)
		{
			if (m_pPlayerTurret->isEnabled() == true)
			{
				GunCD = 0;
				m_pBullet.push_back(new Bullet(m_pPlayerTurret->m_rotationAngle, m_pPlayerTurret->getTransform()->position, true, "../Assets/textures/Blt.png", "bL"));
				SoundManager::Instance().playSound("ff", 0, -1);
				addChild(m_pBullet[TotalBullets]);
				TotalBullets++;
			}

			else if(m_pExtraTurret->isEnabled()==true)
			{
				GunCD = 0;
				m_pExtraBullet.push_back(new Bullet(m_pExtraTurret->m_rotationAngle, m_pExtraTurret->getTransform()->position, true, "../Assets/textures/extraBullet.png", "extraB"));
				SoundManager::Instance().playSound("ss", 0, -1);
				addChild(m_pExtraBullet[TotalExtraBullets]);
				m_pExtraTurret->totalBullets--;
				if(m_pExtraTurret->totalBullets==0)
				{
					m_pExtraTurret->setEnabled(false);
					m_pPlayerTurret->setEnabled(true);
					m_pPlayerTank->extraWeaponOn = false;
					m_pPlayerTank->hasExtraWeapon = false;
				}
				TotalExtraBullets++;
			}
			
		}
	}

	//Enemy BulletShooting
	if (m_pPlayerTank->isEnabled() == true)
	{
		for (int i = 0; i < totalEnemies; i++)
		{
			if ((m_pEnemyTank[i]->getTransform()->position.x > 0.0f) && (m_pEnemyTank[i]->getTransform()->position.x < 800.0f))
			{
				if ((m_pEnemyTank[i]->getTransform()->position.y > 0.0f) && (m_pEnemyTank[i]->getTransform()->position.y < 600.0f))
				{
					if (m_pEnemyTank[i]->isEnabled() == true)
					{
						////Checking LOS
						m_CheckShipLOS(m_pEnemyTank[i]);
						if (m_pEnemyTank[i]->cd > 4.0f)
						{
							//LOS fire
							if (m_pEnemyTank[i]->hasLOS())
							{

								m_pEnemyTank[i]->cd = 0;
								SoundManager::Instance().playSound("ef", 0, -1);
								m_pEnemyBullet.push_back(new Bullet(m_pETurret[i]->getRotation(), m_pETurret[i]->getTransform()->position, true, "../Assets/textures/Blt.png", "bL"));
								addChild(m_pEnemyBullet[TotalEBullets]);
								TotalEBullets++;
							}
							//Radius (Probably useless)
							else if (Util::distance(m_pEnemyTank[i]->getTransform()->position, m_pPlayerTank->getTransform()->position) < 150)
							{
								m_pEnemyTank[i]->cd = 0;
								SoundManager::Instance().playSound("ef", 0, -1);
								m_pEnemyBullet.push_back(new Bullet(m_pETurret[i]->getRotation(), m_pETurret[i]->getTransform()->position, true, "../Assets/textures/Blt.png", "bL"));
								addChild(m_pEnemyBullet[TotalEBullets]);
								TotalEBullets++;
							}
						}
					}
				}
			}
		}
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE))
	{
		if ((m_pPlayerTank->hasExtraWeapon == true) && (ChangeGun > 4))
		{
			if (m_pPlayerTank->extraWeaponOn == false)
			{
				m_pPlayerTurret->setEnabled(false);
				m_pExtraTurret->setEnabled(true);
				m_pPlayerTank->extraWeaponOn = true;
				ChangeGun = 0;
			}
			else if (m_pPlayerTank->extraWeaponOn == true)
			{
				m_pPlayerTurret->setEnabled(true);
				m_pExtraTurret->setEnabled(false);
				m_pPlayerTank->extraWeaponOn = false;
				ChangeGun = 0;
			}
		}
	}
	
	//Win Condition
	if (m_pPlayerTank->isEnabled() == false)
	{
		if (StageEndCD > 4)
		{
			TheGame::Instance()->changeSceneState(LOSE_SCENE);
		}
	}
	else if (EnemiesDestroyed == totalEnemies)
	{
		if (StageEndCD > 1)
		{
			TheGame::Instance()->changeSceneState(WIN_SCENE);
		}
	}
}

void PlayScene2::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene2";

	auto offsetTiles1 = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	auto offsetTiles2 = glm::vec2(Config::TILE_SIZE * 1.0f, Config::TILE_SIZE * 0.5f);
	auto offsetEnemiesDown = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f + 60.0f);
	auto offsetEnemiesUp = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f - 60.0f);
	auto offsetEnemiesRight = glm::vec2(Config::TILE_SIZE * 0.5f + 60.0f, Config::TILE_SIZE * 0.5f);
	auto offsetEnemiesLeft = glm::vec2(Config::TILE_SIZE * 0.5f - 60.0f, Config::TILE_SIZE * 0.5f);

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

	//Background
	Bg = new TileC("../Assets/grid/Bg2.png", "Bg2");
	Bg->getTransform()->position.x = 800.0f / 2;
	Bg->getTransform()->position.y = 600.0f / 2;
	addChild(Bg, 0);

	//Obstacles //

	m_field[0] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[0]->getTransform()->position = m_getTile(0, 13)->getTransform()->position + offsetTiles1;
	addChild(m_field[0], 1);
	m_pMap.push_back(m_field[0]);

	m_field[2] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[2]->getTransform()->position = m_getTile(1, 8)->getTransform()->position + offsetTiles1;
	addChild(m_field[2], 1);
	m_pMap.push_back(m_field[2]);

	m_field[3] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[3]->getTransform()->position = m_getTile(1, 3)->getTransform()->position + offsetTiles1;
	addChild(m_field[3], 1);
	m_pMap.push_back(m_field[3]);

	m_field[4] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[4]->getTransform()->position = m_getTile(1, 0)->getTransform()->position + offsetTiles1;
	addChild(m_field[4], 1);
	m_pMap.push_back(m_field[4]);

	m_field[1] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[1]->getTransform()->position = m_getTile(3, 13)->getTransform()->position + offsetTiles1;
	addChild(m_field[1], 1);
	m_pMap.push_back(m_field[1]);

	m_field[5] = new TileC("../Assets/grid/Hs.png", "h");
	m_field[5]->getTransform()->position = m_getTile(4, 8)->getTransform()->position + offsetTiles1;
	addChild(m_field[5], 1);
	m_pMap.push_back(m_field[5]);

	m_field[6] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[6]->getTransform()->position = m_getTile(6, 13)->getTransform()->position + offsetTiles1;
	addChild(m_field[6], 1);
	m_pMap.push_back(m_field[6]);

	m_field[7] = new TileC("../Assets/grid/Hs.png", "h");
	m_field[7]->getTransform()->position = m_getTile(7, 8)->getTransform()->position + offsetTiles1;
	addChild(m_field[7], 1);
	m_pMap.push_back(m_field[7]);

	m_field[8] = new TileC("/Assets/grid/Bl.png", "b");
	m_field[8]->getTransform()->position = m_getTile(7, 0)->getTransform()->position + offsetTiles1;
	addChild(m_field[8], 1);
	m_pMap.push_back(m_field[8]);

	m_field[9] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[9]->getTransform()->position = m_getTile(9, 13)->getTransform()->position + offsetTiles1;
	addChild(m_field[9], 1);
	m_pMap.push_back(m_field[9]);

	m_field[10] = new TileC("../Assets/grid/Hs.png", "h");
	m_field[10]->getTransform()->position = m_getTile(10, 8)->getTransform()->position + offsetTiles1;
	addChild(m_field[10], 1);
	m_pMap.push_back(m_field[10]);

	m_field[11] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[11]->getTransform()->position = m_getTile(10, 0)->getTransform()->position + offsetTiles1;
	addChild(m_field[11], 1);
	m_pMap.push_back(m_field[11]);

	m_field[12] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[12]->getTransform()->position = m_getTile(12, 13)->getTransform()->position + offsetTiles1;
	addChild(m_field[12], 1);
	m_pMap.push_back(m_field[12]);

	m_field[13] = new TileC("../Assets/grid/Hs.png", "h");
	m_field[13]->getTransform()->position = m_getTile(13, 8)->getTransform()->position + offsetTiles1;
	addChild(m_field[13], 1);
	m_pMap.push_back(m_field[13]);

	m_field[14] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[14]->getTransform()->position = m_getTile(13, 0)->getTransform()->position + offsetTiles1;
	addChild(m_field[14], 1);
	m_pMap.push_back(m_field[14]);

	m_field[15] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[15]->getTransform()->position = m_getTile(15, 13)->getTransform()->position + offsetTiles1;
	addChild(m_field[15], 1);
	m_pMap.push_back(m_field[15]);

	m_field[16] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[16]->getTransform()->position = m_getTile(18, 13)->getTransform()->position + offsetTiles1;
	addChild(m_field[16], 1);
	m_pMap.push_back(m_field[16]);

	m_field[17] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[17]->getTransform()->position = m_getTile(18, 10)->getTransform()->position + offsetTiles1;
	addChild(m_field[17], 1);
	m_pMap.push_back(m_field[17]);

	m_field[18] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[18]->getTransform()->position = m_getTile(18, 7)->getTransform()->position + offsetTiles1;
	addChild(m_field[18], 1);
	m_pMap.push_back(m_field[18]);

	m_field[19] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[19]->getTransform()->position = m_getTile(18, 3)->getTransform()->position + offsetTiles1;
	addChild(m_field[19], 1);
	m_pMap.push_back(m_field[19]);

	m_field[20] = new TileC("../Assets/grid/Bl.png", "b");
	m_field[20]->getTransform()->position = m_getTile(18, 0)->getTransform()->position + offsetTiles1;
	addChild(m_field[20], 1);
	m_pMap.push_back(m_field[20]);

	m_field[21] = new TileC("../Assets/grid/Bb.png", "bb");
	m_field[21]->getTransform()->position = m_getTile(7, 3)->getTransform()->position + offsetTiles1;
	addChild(m_field[21], 1);
	m_pMap.push_back(m_field[21]);

	m_field[22] = new TileC("../Assets/grid/Bb.png", "bb");
	m_field[22]->getTransform()->position = m_getTile(13, 3)->getTransform()->position + offsetTiles1;
	addChild(m_field[22], 1);
	m_pMap.push_back(m_field[22]);

	//Enemy ETank //
	
	m_pEnemyTank[0] = new ETank();
	m_pEnemyTank[0]->getTransform()->position = m_getTile(0, 6)->getTransform()->position + offsetEnemiesLeft;
	m_pEnemyTank[0]->setRotation(90.0f);
	addChild(m_pEnemyTank[0], 2);

	m_pEnemyTank[1] = new ETank();
	m_pEnemyTank[1]->getTransform()->position = m_getTile(4, 0)->getTransform()->position + offsetEnemiesUp;
	m_pEnemyTank[1]->setRotation(180.0f);
	addChild(m_pEnemyTank[1], 2);

	m_pEnemyTank[2] = new ETank();
	m_pEnemyTank[2]->getTransform()->position = m_getTile(0, 10)->getTransform()->position + offsetEnemiesLeft;
	m_pEnemyTank[2]->setRotation(90.0f);
	addChild(m_pEnemyTank[2], 2);

	m_pEnemyTank[3] = new ETank();
	m_pEnemyTank[3]->getTransform()->position = m_getTile(0, 11)->getTransform()->position + offsetEnemiesLeft;
	m_pEnemyTank[3]->setRotation(90.0f);
	addChild(m_pEnemyTank[3], 2);

	m_pEnemyTank[4] = new ETank();
	m_pEnemyTank[4]->getTransform()->position = m_getTile(16, 0)->getTransform()->position + offsetEnemiesUp;
	m_pEnemyTank[4]->setRotation(180.0f);
	addChild(m_pEnemyTank[4], 2);

	m_pEnemyTank[5] = new ETank();
	m_pEnemyTank[5]->getTransform()->position = m_getTile(19, 5)->getTransform()->position + offsetEnemiesRight;
	m_pEnemyTank[5]->setRotation(-90.0f);
	addChild(m_pEnemyTank[5], 2);
	
	m_pEnemyTank[6] = new ETank();
	m_pEnemyTank[6]->getTransform()->position = m_getTile(0, 5)->getTransform()->position + offsetEnemiesLeft;
	m_pEnemyTank[6]->setRotation(90.0f);
	addChild(m_pEnemyTank[6], 2);

	m_pEnemyTank[7] = new ETank();
	m_pEnemyTank[7]->getTransform()->position = m_getTile(0, 6)->getTransform()->position + offsetEnemiesLeft;
	m_pEnemyTank[7]->setRotation(90.0f);
	addChild(m_pEnemyTank[7], 2);

	m_pEnemyTank[8] = new ETank();
	m_pEnemyTank[8]->getTransform()->position = m_getTile(0, 5)->getTransform()->position + offsetEnemiesLeft;
	m_pEnemyTank[8]->setRotation(90.0f);
	addChild(m_pEnemyTank[8], 2);

	m_pEnemyTank[9] = new ETank();
	m_pEnemyTank[9]->getTransform()->position = m_getTile(0, 6)->getTransform()->position + offsetEnemiesLeft;
	m_pEnemyTank[9]->setRotation(90.0f);
	addChild(m_pEnemyTank[9], 2);

	m_pEnemyTank[10] = new ETank();
	m_pEnemyTank[10]->getTransform()->position = m_getTile(3, 0)->getTransform()->position + offsetEnemiesUp;
	m_pEnemyTank[10]->setRotation(180.0f);
	addChild(m_pEnemyTank[10], 2);

	m_pEnemyTank[11] = new ETank();
	m_pEnemyTank[11]->getTransform()->position = m_getTile(4, 0)->getTransform()->position + offsetEnemiesUp;
	m_pEnemyTank[11]->setRotation(180.0f);
	addChild(m_pEnemyTank[11], 2);

	m_pEnemyTank[12] = new ETank();
	m_pEnemyTank[12]->getTransform()->position = m_getTile(5, 0)->getTransform()->position + offsetEnemiesUp;
	m_pEnemyTank[12]->setRotation(180.0f);
	addChild(m_pEnemyTank[12], 2);

	m_pEnemyTank[13] = new ETank();
	m_pEnemyTank[13]->getTransform()->position = m_getTile(19, 5)->getTransform()->position + offsetEnemiesRight;
	m_pEnemyTank[13]->setRotation(-90.0f);
	addChild(m_pEnemyTank[13], 2);

	m_pEnemyTank[14] = new ETank();
	m_pEnemyTank[14]->getTransform()->position = m_getTile(16, 0)->getTransform()->position + offsetEnemiesUp;
	m_pEnemyTank[14]->setRotation(180.0f);
	addChild(m_pEnemyTank[14], 2);

	m_pEnemyTank[15] = new ETank();
	m_pEnemyTank[15]->getTransform()->position = m_getTile(15, 0)->getTransform()->position + offsetEnemiesUp;
	m_pEnemyTank[15]->setRotation(180.0f);
	addChild(m_pEnemyTank[15], 2);

	m_pEnemyTank[16] = new ETank();
	m_pEnemyTank[16]->getTransform()->position = m_getTile(0, 10)->getTransform()->position + offsetEnemiesLeft;
	m_pEnemyTank[16]->setRotation(90.0f);
	addChild(m_pEnemyTank[16], 2);

	m_pEnemyTank[17] = new ETank();
	m_pEnemyTank[17]->getTransform()->position = m_getTile(0, 11)->getTransform()->position + offsetEnemiesLeft;
	m_pEnemyTank[17]->setRotation(90.0f);
	addChild(m_pEnemyTank[17], 2);

	for (int i = 0; i < totalEnemies; i++)
	{
		m_pETurret[i] = new eTurret();
		m_pETurret[i]->getTransform()->position = m_pEnemyTank[i]->getTransform()->position;
		addChild(m_pETurret[i], 3);
	}

	//PlayerTank
	m_pPlayerTank = new PlayerTank();
	m_pPlayerTank->getTransform()->position = m_getTile(3, 11)->getTransform()->position + offsetTiles1;
	m_pPlayerTank->setRotation(90.0f);
	m_pPlayerTank->setEnabled(true);
	addChild(m_pPlayerTank, 2);
	m_pMap.push_back(m_pPlayerTank);


	//Player Turret
	m_pPlayerTurret = new pTurret("../Assets/textures/pTurret.png", "pT");
	m_pPlayerTurret->getTransform()->position == glm::vec2(100.0f, 300.0f);
	m_pPlayerTurret->getTransform()->position = m_pPlayerTank->getTransform()->position;
	addChild(m_pPlayerTurret, 3);
	//Extra Turret
	m_pExtraTurret = new pTurret("../Assets/textures/pExTurret.png", "eXturret");
	m_pExtraTurret->getTransform()->position == glm::vec2(100.0f, 300.0f);
	m_pExtraTurret->getTransform()->position = m_pPlayerTank->getTransform()->position;
	m_pExtraTurret->setEnabled(false);
	m_pExtraTurret->totalBullets = 8;
	addChild(m_pExtraTurret, 3);
	//Droped
	extraDrop = new TileC("../Assets/textures/pExTurret.png", "eXturret");
	extraDrop->getTransform()->position=m_getTile(10, 3)->getTransform()->position + offsetTiles1;
	addChild(extraDrop, 2);


}

void PlayScene2::GUI_Function() const
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

void PlayScene2::m_buildGrid()
{

	auto tileSize = Config::TILE_SIZE;
	//Add tiles to the grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

			Tile* tile = new Tile();//Create empty tile
			tile->getTransform()->position = glm::vec2(col * tileSize, row * tileSize);
			tile->setGridPosition(col, row);
			addChild(tile, 1);
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

	//m_setGridEnabled(true);//TODO delete to set Tile info to false

	std::cout << m_pGrid.size() << std::endl;
}
void PlayScene2::m_setGridEnabled(bool state) const
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


Tile* PlayScene2::m_getTile(const int col, const int row) const
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}

Tile* PlayScene2::m_getTile(glm::vec2 grid_position) const
{
	const auto col = grid_position.x;
	const auto row = grid_position.y;
	return m_pGrid[(row * Config::COL_NUM) + col];
}

void PlayScene2::m_move()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	//Tank 0

	if (m_pEnemyTank[0]->seek == true)
	{
		m_pEnemyTank[0]->move = true;
		m_pEnemyTank[0]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[0]->p0 == false && m_pEnemyTank[0]->p1 == false)
	{
		m_pEnemyTank[0]->move = true;
		m_pEnemyTank[0]->setDestination(m_getTile(16, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[0]->getTransform()->position == m_getTile(16, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[0]->p0 = true;
		}

	}
	else if (m_pEnemyTank[0]->p0 == true)
	{
		m_pEnemyTank[0]->setDestination(m_getTile(16, 11)->getTransform()->position + offset);
		if (m_pEnemyTank[0]->getTransform()->position == m_getTile(16, 11)->getTransform()->position + offset)
		{
			m_pEnemyTank[0]->p0 = false;
			m_pEnemyTank[0]->p1 = true;
		}
	}
	else if (m_pEnemyTank[0]->p1 == true)
	{
		m_pEnemyTank[0]->setDestination(m_getTile(10, 11)->getTransform()->position + offset);
		if (m_pEnemyTank[0]->getTransform()->position == m_getTile(10, 11)->getTransform()->position + offset)
		{
			m_pEnemyTank[0]->p2 = true;
			m_pEnemyTank[0]->seek = true;
		}
	}

	////Tank 1
	if (m_pEnemyTank[1]->seek == true)
	{
		m_pEnemyTank[1]->move = true;
		m_pEnemyTank[1]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[1]->p0 == false && m_pEnemyTank[1]->p1 == false)
	{
		if (GameTimer > 2)
			m_pEnemyTank[1]->move = true;
		m_pEnemyTank[1]->setDestination(m_getTile(4, 5)->getTransform()->position + offset);
		if (m_pEnemyTank[1]->getTransform()->position == m_getTile(4, 5)->getTransform()->position + offset)
		{
			m_pEnemyTank[1]->p0 = true;
		}

	}
	else if (m_pEnemyTank[1]->p0 == true)
	{
		m_pEnemyTank[1]->setDestination(m_getTile(15, 5)->getTransform()->position + offset);
		if (m_pEnemyTank[1]->getTransform()->position == m_getTile(15, 5)->getTransform()->position + offset)
		{
			m_pEnemyTank[1]->p0 = false;
			m_pEnemyTank[1]->p1 = true;
		}
	}
	else if (m_pEnemyTank[1]->p1 == true && m_pEnemyTank[1]->p2 == false)
	{
		m_pEnemyTank[1]->setDestination(m_getTile(15, 10)->getTransform()->position + offset);
		if (m_pEnemyTank[1]->getTransform()->position == m_getTile(15, 10)->getTransform()->position + offset)
		{
			m_pEnemyTank[1]->p2 = true;
		}
	}
	else if (m_pEnemyTank[1]->p2 == true)
	{
		m_pEnemyTank[1]->setDestination(m_getTile(10, 10)->getTransform()->position + offset);
		if (m_pEnemyTank[1]->getTransform()->position == m_getTile(10, 10)->getTransform()->position + offset)
		{
			m_pEnemyTank[1]->p3 = true;
			m_pEnemyTank[1]->seek = true;
		}
	}

	////Tank 2
	if (m_pEnemyTank[2]->seek == true)
	{
		m_pEnemyTank[2]->move = true;
		m_pEnemyTank[2]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[2]->p0 == false && m_pEnemyTank[2]->p1 == false)
	{
		if (GameTimer >= 15)
			m_pEnemyTank[2]->move = true;
		m_pEnemyTank[2]->setDestination(m_getTile(10, 10)->getTransform()->position + offset);
		if (m_pEnemyTank[2]->getTransform()->position == m_getTile(10, 10)->getTransform()->position + offset)
		{
			m_pEnemyTank[2]->p0 = true;
			m_pEnemyTank[2]->move = false;
		}

	}
	else if (m_pEnemyTank[2]->p0 == true)
	{
		if(GameTimer>=35)
			m_pEnemyTank[2]->move = true;
		m_pEnemyTank[2]->setDestination(m_getTile(15, 10)->getTransform()->position + offset);
		if (m_pEnemyTank[2]->getTransform()->position == m_getTile(15, 10)->getTransform()->position + offset)
		{
			m_pEnemyTank[2]->p0 = false;
			m_pEnemyTank[2]->p1 = true;

		}
	}
	else if (m_pEnemyTank[2]->p1 == true && m_pEnemyTank[2]->p2 == false)
	{
		m_pEnemyTank[2]->setDestination(m_getTile(15, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[2]->getTransform()->position == m_getTile(15, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[2]->p2 = true;
			m_pEnemyTank[2]->seek = true;
		}
	}

	//Tank 3 //
	if (m_pEnemyTank[3]->seek == true)
	{
		m_pEnemyTank[3]->move = true;
		m_pEnemyTank[3]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[3]->p0 == false && m_pEnemyTank[3]->p1 == false)
	{
		if (GameTimer >= 15)
			m_pEnemyTank[3]->move = true;
		m_pEnemyTank[3]->setDestination(m_getTile(10, 11)->getTransform()->position + offset);
		if (m_pEnemyTank[3]->getTransform()->position == m_getTile(10, 11)->getTransform()->position + offset)
		{
			m_pEnemyTank[3]->p0 = true;
			m_pEnemyTank[3]->move = false;
		}

	}
	else if (m_pEnemyTank[3]->p0 == true)
	{
		if (GameTimer >= 37)
			m_pEnemyTank[3]->move = true;
		m_pEnemyTank[3]->setDestination(m_getTile(16, 11)->getTransform()->position + offset);
		if (m_pEnemyTank[3]->getTransform()->position == m_getTile(16, 11)->getTransform()->position + offset)
		{
			m_pEnemyTank[3]->p0 = false;
			m_pEnemyTank[3]->p1 = true;

		}
	}
	else if (m_pEnemyTank[3]->p1 == true && m_pEnemyTank[3]->p2 == false)
	{
		m_pEnemyTank[3]->setDestination(m_getTile(16, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[3]->getTransform()->position == m_getTile(16, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[3]->p2 = true;
			m_pEnemyTank[3]->seek = true;
		}
	}

	////Tank 4
	if (m_pEnemyTank[4]->seek == true)
	{
		m_pEnemyTank[4]->move = true;
		m_pEnemyTank[4]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[4]->p0 == false && m_pEnemyTank[4]->p1 == false)
	{
		//if (GameTimer >= 20)
		//	m_pEnemyTank[4]->move = true;
		//m_pEnemyTank[4]->setDestination(m_getTile(16, 0)->getTransform()->position + offset);
		//if (m_pEnemyTank[4]->getTransform()->position == m_getTile(16, 0)->getTransform()->position + offset)
		//{
			m_pEnemyTank[4]->p0 = true;
			//m_pEnemyTank[4]->move = false;
		//}
	}
	else if (m_pEnemyTank[4]->p0 == true)
	{
		if (GameTimer >= 33)
		{
			m_pEnemyTank[4]->move = true;
			m_pEnemyTank[4]->setDestination(m_getTile(16, 4)->getTransform()->position + offset);
			if (m_pEnemyTank[4]->getTransform()->position == m_getTile(16, 4)->getTransform()->position + offset)
			{
				m_pEnemyTank[4]->p0 = false;
				m_pEnemyTank[4]->p1 = true;
				m_pEnemyTank[4]->seek = true;
			}
		}
	}

	//Tank 5
	if (m_pEnemyTank[5]->seek == true)
	{
		m_pEnemyTank[5]->move = true;
		m_pEnemyTank[5]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[5]->p0 == false && m_pEnemyTank[5]->p1 == false)
	{
		if (GameTimer >= 20)
			m_pEnemyTank[5]->move = true;
		m_pEnemyTank[5]->setDestination(m_getTile(19, 5)->getTransform()->position + offset);
		if (m_pEnemyTank[5]->getTransform()->position == m_getTile(19, 5)->getTransform()->position + offset)
		{
			m_pEnemyTank[5]->p0 = true;
			m_pEnemyTank[5]->move = false;
		}
	}
	else if (m_pEnemyTank[5]->p0 == true)
	{
		if(GameTimer>=31)
			m_pEnemyTank[5]->move = true;
		m_pEnemyTank[5]->setDestination(m_getTile(16, 5)->getTransform()->position + offset);
		if (m_pEnemyTank[5]->getTransform()->position == m_getTile(16, 5)->getTransform()->position + offset)
		{
			m_pEnemyTank[5]->p0 = false;
			m_pEnemyTank[5]->p1 = true;
			m_pEnemyTank[5]->seek = true;
		}
	}

	//Tank 6
	if (m_pEnemyTank[6]->seek == true)
	{
		m_pEnemyTank[6]->move = true;
		m_pEnemyTank[6]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[6]->p0 == false && m_pEnemyTank[6]->p1 == false)
	{
		//if (GameTimer >= 16)
		//	m_pEnemyTank[6]->move = true;
		//m_pEnemyTank[6]->setDestination(m_getTile(2, 5)->getTransform()->position + offset);
		//if (m_pEnemyTank[6]->getTransform()->position == m_getTile(2, 5)->getTransform()->position + offset)
		//{
			m_pEnemyTank[6]->p0 = true;
			//m_pEnemyTank[6]->move = false;
		//}
		
	}
	else if (m_pEnemyTank[6]->p0 == true)
	{
		if (GameTimer >= 22)
			m_pEnemyTank[6]->move = true;
		m_pEnemyTank[6]->setDestination(m_getTile(10, 5)->getTransform()->position + offset);
		if (m_pEnemyTank[6]->getTransform()->position == m_getTile(10, 5)->getTransform()->position + offset)
		{
			m_pEnemyTank[6]->p0 = false;
			m_pEnemyTank[6]->p1 = true;
			m_pEnemyTank[6]->seek = true;
		}
	}

	//Tank 7
	if (m_pEnemyTank[7]->seek == true)
	{
		m_pEnemyTank[7]->move = true;
		m_pEnemyTank[7]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[7]->p0 == false && m_pEnemyTank[7]->p1 == false)
	{
		//if (GameTimer >= 16)
		//	m_pEnemyTank[7]->move = true;
		//m_pEnemyTank[7]->setDestination(m_getTile(2, 6)->getTransform()->position + offset);
		//if (m_pEnemyTank[7]->getTransform()->position == m_getTile(2, 6)->getTransform()->position + offset)
		//{
			m_pEnemyTank[7]->p0 = true;
		//	m_pEnemyTank[7]->move = false;
		//}
	}
	else if (m_pEnemyTank[7]->p0 == true)
	{
		if(GameTimer>=20)
			m_pEnemyTank[7]->move = true;
		m_pEnemyTank[7]->setDestination(m_getTile(10, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[7]->getTransform()->position == m_getTile(10, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[7]->p0 = false;
			m_pEnemyTank[7]->p1 = true;
			m_pEnemyTank[7]->seek = true;
		}
	}
	
	//Tank 8
	//m_pEnemyTank[8]->p0 = true;
	if (m_pEnemyTank[8]->seek == true)
	{
		m_pEnemyTank[8]->move = true;
		m_pEnemyTank[8]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[8]->p0 == false && m_pEnemyTank[8]->p1 == false)
	{
	//	if (GameTimer >= 28)
	//		m_pEnemyTank[8]->move = true;
	//	m_pEnemyTank[8]->setDestination(m_getTile(2, 5)->getTransform()->position + offset);
	//	if (m_pEnemyTank[8]->getTransform()->position == m_getTile(2, 5)->getTransform()->position + offset)
	//	{
			m_pEnemyTank[8]->p0 = true;
	//		m_pEnemyTank[8]->move = false;
	}

	//}
	else if (m_pEnemyTank[8]->p0 == true)
	{
		if (GameTimer >= 37)
			m_pEnemyTank[8]->move = true;
		m_pEnemyTank[8]->setDestination(m_getTile(15, 5)->getTransform()->position + offset);
		if (m_pEnemyTank[8]->getTransform()->position == m_getTile(15, 5)->getTransform()->position + offset)
		{
			m_pEnemyTank[8]->p0 = false;
			m_pEnemyTank[8]->p1 = true;
		}
	}
	else if (m_pEnemyTank[8]->p1 == true && m_pEnemyTank[8]->p2 == false)
	{
		m_pEnemyTank[8]->setDestination(m_getTile(15, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[8]->getTransform()->position == m_getTile(15, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[8]->p2 = true;
		}
	}
	else if (m_pEnemyTank[8]->p2 == true && m_pEnemyTank[8]->p3 == false)
	{
		m_pEnemyTank[8]->setDestination(m_getTile(15, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[8]->getTransform()->position == m_getTile(15, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[8]->p3 = true;
			m_pEnemyTank[8]->move = false;
		}
	}
	else if (m_pEnemyTank[8]->p3 == true && m_pEnemyTank[8]->p4 == false)
	{
		if (GameTimer >= 64)
			m_pEnemyTank[8]->move = true;
		m_pEnemyTank[8]->setDestination(m_getTile(15, 10)->getTransform()->position + offset);
		if (m_pEnemyTank[8]->getTransform()->position == m_getTile(15, 10)->getTransform()->position + offset)
		{
			m_pEnemyTank[8]->seek = true;
		}
	}
	//Tank 9
	//m_pEnemyTank[9]->p0 = true;
	if (m_pEnemyTank[9]->seek == true)
	{				 
		m_pEnemyTank[9]->move = true;
		m_pEnemyTank[9]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[9]->p0 == false && m_pEnemyTank[9]->p1 == false)
	{
		//if (GameTimer >= 28)
		//	m_pEnemyTank[9]->move = true;
		//m_pEnemyTank[9]->setDestination(m_getTile(2, 6)->getTransform()->position + offset);
		//if (m_pEnemyTank[9]->getTransform()->position == m_getTile(2, 6)->getTransform()->position + offset)
		//{
			m_pEnemyTank[9]->p0 = true;
		//	m_pEnemyTank[9]->move = false;
		//}
	}
	else if (m_pEnemyTank[9]->p0 == true)
	{
		if (GameTimer >= 37)
			m_pEnemyTank[9]->move = true;
		m_pEnemyTank[9]->setDestination(m_getTile(15, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[9]->getTransform()->position == m_getTile(15, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[9]->p0 = false;
			m_pEnemyTank[9]->p1 = true;
		}
	}
	else if (m_pEnemyTank[9]->p1 == true && m_pEnemyTank[9]->p2 == false)
	{
		m_pEnemyTank[9]->setDestination(m_getTile(15, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[9]->getTransform()->position == m_getTile(15, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[9]->p2 = true;
		}
	}
	else if (m_pEnemyTank[9]->p2 == true && m_pEnemyTank[9]->p3 == false)
	{
		m_pEnemyTank[9]->setDestination(m_getTile(15, 7)->getTransform()->position + offset);
		if (m_pEnemyTank[9]->getTransform()->position == m_getTile(15, 7)->getTransform()->position + offset)
		{
			m_pEnemyTank[9]->p3 = true;
			m_pEnemyTank[9]->move = false;
		}
	}
	else if (m_pEnemyTank[9]->p3 == true && m_pEnemyTank[9]->p4 == false)
	{
		if (GameTimer >= 62)
			m_pEnemyTank[9]->move = true;
		m_pEnemyTank[9]->setDestination(m_getTile(15, 10)->getTransform()->position + offset);
		if (m_pEnemyTank[9]->getTransform()->position == m_getTile(15, 10)->getTransform()->position + offset)
		{
			m_pEnemyTank[9]->seek = true;
		}
	}

		//Tank 10 //
	if (m_pEnemyTank[10]->seek == true)
	{
		m_pEnemyTank[10]->move = true;
		m_pEnemyTank[10]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[10]->p0 == false && m_pEnemyTank[10]->p1 == false)
	{
		if (GameTimer >= 28)
			m_pEnemyTank[10]->move = true;
		m_pEnemyTank[10]->setDestination(m_getTile(3, 4)->getTransform()->position + offset);
		if (m_pEnemyTank[10]->getTransform()->position == m_getTile(3, 4)->getTransform()->position + offset)
		{
			m_pEnemyTank[10]->p0 = true;
			m_pEnemyTank[10]->move = false;
		}

	}
	else if (m_pEnemyTank[10]->p0 == true)
	{
		if (GameTimer >= 38)
			m_pEnemyTank[10]->move = true;
		m_pEnemyTank[10]->setDestination(m_getTile(3, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[10]->getTransform()->position == m_getTile(3, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[10]->p0 = false;
			m_pEnemyTank[10]->p1 = true;
		}
	}
	else if (m_pEnemyTank[10]->p1 == true && m_pEnemyTank[10]->p2 == false)
	{
		m_pEnemyTank[10]->setDestination(m_getTile(16, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[10]->getTransform()->position == m_getTile(16, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[10]->p2 = true;
		}
	}
	else if (m_pEnemyTank[10]->p2 == true && m_pEnemyTank[10]->p3 == false)
	{
		m_pEnemyTank[10]->setDestination(m_getTile(16, 7)->getTransform()->position + offset);
		if (m_pEnemyTank[10]->getTransform()->position == m_getTile(16, 7)->getTransform()->position + offset)
		{
			m_pEnemyTank[10]->p3 = true;
			m_pEnemyTank[10]->move=false;
		}
	}
	else if (m_pEnemyTank[10]->p3 == true && m_pEnemyTank[10]->p4 == false)
	{
		if(GameTimer>=62)
			m_pEnemyTank[10]->move = true;
		m_pEnemyTank[10]->setDestination(m_getTile(16, 11)->getTransform()->position + offset);
		if (m_pEnemyTank[10]->getTransform()->position == m_getTile(16, 11)->getTransform()->position + offset)
		{
			m_pEnemyTank[10]->seek = true;
		}
	}

	//Tank 11 //
	if (m_pEnemyTank[11]->seek == true)
	{
		m_pEnemyTank[11]->move = true;
		m_pEnemyTank[11]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[11]->p0 == false && m_pEnemyTank[11]->p1 == false)
	{
		if (GameTimer >= 28)
			m_pEnemyTank[11]->move = true;
		m_pEnemyTank[11]->setDestination(m_getTile(4, 4)->getTransform()->position + offset);
		if (m_pEnemyTank[11]->getTransform()->position == m_getTile(4, 4)->getTransform()->position + offset)
		{
			m_pEnemyTank[11]->p0 = true;
			m_pEnemyTank[11]->move = false;
		}

	}
	else if (m_pEnemyTank[11]->p0 == true)
	{
		if (GameTimer >= 38)
			m_pEnemyTank[11]->move = true;
		m_pEnemyTank[11]->setDestination(m_getTile(4, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[11]->getTransform()->position == m_getTile(4, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[11]->p0 = false;
			m_pEnemyTank[11]->p1 = true;
		}
	}
	else if (m_pEnemyTank[11]->p1 == true && m_pEnemyTank[11]->p2 == false)
	{
		m_pEnemyTank[11]->setDestination(m_getTile(15, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[11]->getTransform()->position == m_getTile(15, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[11]->p2 = true;
		}
	}
	else if (m_pEnemyTank[11]->p2 == true && m_pEnemyTank[11]->p3 == false)
	{
		m_pEnemyTank[11]->setDestination(m_getTile(15, 8)->getTransform()->position + offset);
		if (m_pEnemyTank[11]->getTransform()->position == m_getTile(15, 8)->getTransform()->position + offset)
		{
			m_pEnemyTank[11]->p3 = true;
			m_pEnemyTank[11]->move = false;
		}
	}
	else if (m_pEnemyTank[11]->p3 == true && m_pEnemyTank[11]->p4 == false)
	{
		if(GameTimer>=60)
			m_pEnemyTank[11]->move = true;
		m_pEnemyTank[11]->setDestination(m_getTile(15, 10)->getTransform()->position + offset);
		if (m_pEnemyTank[11]->getTransform()->position == m_getTile(15, 10)->getTransform()->position + offset)
		{
			m_pEnemyTank[11]->seek = true;
		}
	}

	//Tank 12 //
	if (m_pEnemyTank[12]->seek == true)
	{
		m_pEnemyTank[12]->move = true;
		m_pEnemyTank[12]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[12]->p0 == false && m_pEnemyTank[12]->p1 == false)
	{
		if (GameTimer >= 28)
			m_pEnemyTank[12]->move = true;
		m_pEnemyTank[12]->setDestination(m_getTile(5, 4)->getTransform()->position + offset);
		if (m_pEnemyTank[12]->getTransform()->position == m_getTile(5, 4)->getTransform()->position + offset)
		{
			m_pEnemyTank[12]->p0 = true;
			m_pEnemyTank[12]->move = false;
		}

	}
	else if (m_pEnemyTank[12]->p0 == true)
	{
		if (GameTimer >= 38)
			m_pEnemyTank[12]->move = true;
		m_pEnemyTank[12]->setDestination(m_getTile(5, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[12]->getTransform()->position == m_getTile(5, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[12]->p0 = false;
			m_pEnemyTank[12]->p1 = true;

		}
	}
	else if (m_pEnemyTank[12]->p1 == true && m_pEnemyTank[12]->p2 == false)
	{
		m_pEnemyTank[12]->setDestination(m_getTile(16, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[12]->getTransform()->position == m_getTile(16, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[12]->p2 = true;
		}
	}
	else if (m_pEnemyTank[12]->p2 == true && m_pEnemyTank[12]->p3 == false)
	{
		m_pEnemyTank[12]->setDestination(m_getTile(16, 8)->getTransform()->position + offset);
		if (m_pEnemyTank[12]->getTransform()->position == m_getTile(16, 8)->getTransform()->position + offset)
		{
			m_pEnemyTank[12]->p3 = true;
			m_pEnemyTank[12]->move = false;
		}
	}
	else if (m_pEnemyTank[12]->p3 == true && m_pEnemyTank[12]->p4 == false)
	{
		if(GameTimer>=60)
			m_pEnemyTank[12]->move = true;
		m_pEnemyTank[12]->setDestination(m_getTile(16, 11)->getTransform()->position + offset);
		if (m_pEnemyTank[12]->getTransform()->position == m_getTile(16, 11)->getTransform()->position + offset)
		{
			m_pEnemyTank[12]->seek = true;
		}
	}

	//Tank 13

	if (m_pEnemyTank[13]->seek == true)
	{
		m_pEnemyTank[13]->move = true;
		m_pEnemyTank[13]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[13]->p0 == false && m_pEnemyTank[13]->p1 == false)
	{
		if (GameTimer >= 42)
			m_pEnemyTank[13]->move = true;
		m_pEnemyTank[13]->setDestination(m_getTile(19, 5)->getTransform()->position + offset);
		if (m_pEnemyTank[13]->getTransform()->position == m_getTile(19, 5)->getTransform()->position + offset)
		{
			m_pEnemyTank[13]->p0 = true;
			m_pEnemyTank[13]->move = false;
		}

	}
	else if (m_pEnemyTank[13]->p0 == true)
	{
		if (GameTimer >= 47)
			m_pEnemyTank[13]->move = true;
		m_pEnemyTank[13]->setDestination(m_getTile(16, 5)->getTransform()->position + offset);
		if (m_pEnemyTank[13]->getTransform()->position == m_getTile(16, 5)->getTransform()->position + offset)
		{
			m_pEnemyTank[13]->p0 = false;
			m_pEnemyTank[13]->p1 = true;

		}
	}
	else if (m_pEnemyTank[13]->p1 == true && m_pEnemyTank[13]->p2 == false)
	{
		m_pEnemyTank[13]->setDestination(m_getTile(16, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[13]->getTransform()->position == m_getTile(16, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[13]->p2 = true;
			m_pEnemyTank[13]->move = false;
		}
	}
	else if (m_pEnemyTank[13]->p2 == true && m_pEnemyTank[13]->p3 == false)
	{
		if (GameTimer >= 64)
			m_pEnemyTank[13]->move = true;
		m_pEnemyTank[13]->setDestination(m_getTile(16, 11)->getTransform()->position + offset);
		if (m_pEnemyTank[13]->getTransform()->position == m_getTile(16, 11)->getTransform()->position + offset)
		{
			m_pEnemyTank[13]->seek = true;
		}
	}

	//Tank 14

	if (m_pEnemyTank[14]->seek == true)
	{
		m_pEnemyTank[14]->move = true;
		m_pEnemyTank[14]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[14]->p0 == false && m_pEnemyTank[14]->p1 == false)
	{
		if (GameTimer >= 42)
			m_pEnemyTank[14]->move = true;
		m_pEnemyTank[14]->setDestination(m_getTile(16, 0)->getTransform()->position + offset);
		if (m_pEnemyTank[14]->getTransform()->position == m_getTile(16, 0)->getTransform()->position + offset)
		{
			m_pEnemyTank[14]->p0 = true;
			m_pEnemyTank[14]->move = false;
		}

	}
	else if (m_pEnemyTank[14]->p0 == true)
	{
		if (GameTimer >= 47)
			m_pEnemyTank[14]->move = true;
		m_pEnemyTank[14]->setDestination(m_getTile(16, 5)->getTransform()->position + offset);
		if (m_pEnemyTank[14]->getTransform()->position == m_getTile(16, 5)->getTransform()->position + offset)
		{
			m_pEnemyTank[14]->p0 = false;
			m_pEnemyTank[14]->p1 = true;
			m_pEnemyTank[14]->move = false;
		}
	}
	else if (m_pEnemyTank[14]->p1 == true && m_pEnemyTank[14]->p2 == false)
	{
		if (GameTimer >= 66)
			m_pEnemyTank[14]->move = true;
		m_pEnemyTank[14]->setDestination(m_getTile(16, 11)->getTransform()->position + offset);
		if (m_pEnemyTank[14]->getTransform()->position == m_getTile(16, 11)->getTransform()->position + offset)
		{
			m_pEnemyTank[14]->p2 = true;
			m_pEnemyTank[14]->seek = true;
		}
	}

	//Tank 15
	if (m_pEnemyTank[15]->seek == true)
	{
		m_pEnemyTank[15]->move = true;
		m_pEnemyTank[15]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[15]->p0 == false && m_pEnemyTank[15]->p1 == false)
	{
		if (GameTimer >= 42)
			m_pEnemyTank[15]->move = true;
		m_pEnemyTank[15]->setDestination(m_getTile(15, 0)->getTransform()->position + offset);
		if (m_pEnemyTank[15]->getTransform()->position == m_getTile(15, 0)->getTransform()->position + offset)
		{
			m_pEnemyTank[15]->p0 = true;
			m_pEnemyTank[15]->move = false;
		}

	}
	else if (m_pEnemyTank[15]->p0 == true)
	{
		if (GameTimer >= 47)
			m_pEnemyTank[15]->move = true;
		m_pEnemyTank[15]->setDestination(m_getTile(15, 5)->getTransform()->position + offset);
		if (m_pEnemyTank[15]->getTransform()->position == m_getTile(15, 5)->getTransform()->position + offset)
		{
			m_pEnemyTank[15]->p0 = false;
			m_pEnemyTank[15]->p1 = true;
			m_pEnemyTank[15]->move = false;
		}
	}
	else if (m_pEnemyTank[15]->p1 == true && m_pEnemyTank[15]->p2 == false)
	{
		if (GameTimer >= 66)
			m_pEnemyTank[15]->move = true;
		m_pEnemyTank[15]->setDestination(m_getTile(15, 10)->getTransform()->position + offset);
		if (m_pEnemyTank[15]->getTransform()->position == m_getTile(15, 10)->getTransform()->position + offset)
		{
			m_pEnemyTank[15]->p2 = true;
			m_pEnemyTank[15]->seek = true;
		}
	}

	//Ready

	//Tank 16
	if (m_pEnemyTank[16]->seek == true)
	{
		m_pEnemyTank[16]->move = true;
		m_pEnemyTank[16]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[16]->p0 == false && m_pEnemyTank[16]->p1 == false)
	{
		if (GameTimer >= 54)
			m_pEnemyTank[16]->move = true;
		m_pEnemyTank[16]->setDestination(m_getTile(15, 10)->getTransform()->position + offset);
		if (m_pEnemyTank[16]->getTransform()->position == m_getTile(15, 10)->getTransform()->position + offset)
		{
			m_pEnemyTank[16]->p0 = true;
		}
	}
	else if (m_pEnemyTank[16]->p0 == true)
	{
		m_pEnemyTank[16]->setDestination(m_getTile(15, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[16]->getTransform()->position == m_getTile(15, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[16]->p0 = false;
			m_pEnemyTank[16]->p1 = true;
			m_pEnemyTank[16]->seek = true;
		}
	}
	
	//Tank 17
	if (m_pEnemyTank[17]->seek == true)
	{
		m_pEnemyTank[17]->move = true;
		m_pEnemyTank[17]->setDestination(m_pPlayerTank->getTransform()->position);
	}
	else if (m_pEnemyTank[17]->p0 == false && m_pEnemyTank[17]->p1 == false)
	{
		if (GameTimer >= 56)
			m_pEnemyTank[17]->move = true;
		m_pEnemyTank[17]->setDestination(m_getTile(16, 11)->getTransform()->position + offset);
		if (m_pEnemyTank[17]->getTransform()->position == m_getTile(16, 11)->getTransform()->position + offset)
		{
			m_pEnemyTank[17]->p0 = true;
		}
	}
	else if (m_pEnemyTank[17]->p0 == true)
	{
		m_pEnemyTank[17]->setDestination(m_getTile(16, 6)->getTransform()->position + offset);
		if (m_pEnemyTank[17]->getTransform()->position == m_getTile(16, 6)->getTransform()->position + offset)
		{
			m_pEnemyTank[17]->p0 = false;
			m_pEnemyTank[17]->p1 = true;
			m_pEnemyTank[17]->seek = true;
		}
	}

}

void PlayScene2::m_CheckShipLOS(NavigationObject * object)
{
	// if ship to target distance is less than or equal to LOS distance
	auto ShipToTargetDistance = Util::distance(object->getTransform()->position, m_pPlayerTank->getTransform()->position);
	if (ShipToTargetDistance <= object->getLOSDistance())
	{
		std::vector<NavigationObject*> contactList;
		for (auto obj : m_pMap)
		{
			//Check if object is farther than the target
			auto ShipToObjectDistance = Util::distance(object->getTransform()->position, obj->getTransform()->position);
			if (ShipToObjectDistance <= ShipToTargetDistance)
			{
				if ((obj->getType() != object->getType()) && (object->getType() != m_pPlayerTank->getType()))
				{
					contactList.push_back(obj);
				}
			}
		}
		contactList.push_back(m_pPlayerTank); //add the target at the end of the list
		auto hasLOS = CollisionManager::LOSCheck(object->getTransform()->position,
			object->getTransform()->position + object->getOrientation() * object->getLOSDistance(),
			contactList, m_pPlayerTank);
		object->setHasLOS(hasLOS);
	}
}

void PlayScene2::aoeDamage(Bullet* The)
{
	for (int e = 0; e < totalEnemies; e++)
	{
		if (Util::distance(The->getTransform()->position,m_pEnemyTank[e]->getTransform()->position)
			<100.0f)
		{
			m_pEnemyTank[e]->setEnabled(false);
			m_pETurret[e]->setEnabled(false);
			SoundManager::Instance().playSound("ee", 0, -1);
			EnemiesDestroyed++;
		}
	}
}
