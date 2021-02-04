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

	for (int i = 0; i <PT.size(); i++)
	{
		PT[i]->draw();
	}
	
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
	SDL_GetMouseState(&m_mousePosition.x, &m_mousePosition.y);


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

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_X))
	{

		for (unsigned i = 0; i < PT.size(); i++)
		{
			PT.push_back(new Bullet);
		}
		/*	PT.shrink_to_fit();*/
		std::cout << "Created" << std::endl;
	}



	if(m_pPlayerTank->getTransform()->position.x>700.f)
	{
		SoundManager::Instance().playSound("Goal", 0, -1);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	//Labels
	const SDL_Color blue = { 0, 0, 255, 255 };
	m_Inst[1] = new Label("Movement Tutorial Instructions:", "Consolas",
		20, blue, glm::vec2(200.f, 500.f));
	m_Inst[1]->setParent(this);
	addChild(m_Inst[1]);
	m_Inst[2] = new Label("Pass Through Enemies", "Consolas",
		20, blue, glm::vec2(140.f, 520.f));
	m_Inst[2]->setParent(this);
	addChild(m_Inst[2]);
	m_Inst[3] = new Label("Tank Movements:", "Consolas",
		20, blue, glm::vec2(110.f, 540.f));
	m_Inst[3]->setParent(this);
	addChild(m_Inst[3]);
	m_Inst[4] = new Label("W=Forward   S=Backwards   A=Rotate Left   D=Rotate Right", "Consolas",
		20, blue, glm::vec2(340.f, 560.f));
	m_Inst[4]->setParent(this);
	addChild(m_Inst[4]);
	
	//Enemy Tank
	m_pEnemyTank[1] = new Tank();
	m_pEnemyTank[1]->getTransform()->position = glm::vec2(200.0f, 250.0f);
	m_pEnemyTank[1]->moveRight = true;
	addChild(m_pEnemyTank[1]);

	m_pEnemyTank[2] = new Tank();
	m_pEnemyTank[2]->getTransform()->position = glm::vec2(280.0f, 100.0f);
	m_pEnemyTank[2]->moveRight = false;
	addChild(m_pEnemyTank[2]);

	m_pEnemyTank[3] = new Tank();
	m_pEnemyTank[3]->getTransform()->position = glm::vec2(360.0f, 400.0f);
	addChild(m_pEnemyTank[3]);

	m_pEnemyTank[4] = new Tank();
	m_pEnemyTank[4]->getTransform()->position = glm::vec2(440.0f, 250.0f);
	m_pEnemyTank[4]->moveRight = false;
	addChild(m_pEnemyTank[4]);

	m_pEnemyTank[5] = new Tank();
	m_pEnemyTank[5]->getTransform()->position = glm::vec2(520.0f, 100.0f);
	addChild(m_pEnemyTank[5]);

	m_pEnemyTank[6] = new Tank();
	m_pEnemyTank[6]->getTransform()->position = glm::vec2(600.0f, 400.0f);
	m_pEnemyTank[6]->moveRight = false;
	addChild(m_pEnemyTank[6]);

	m_pEnemyTank[7] = new Tank();
	m_pEnemyTank[7]->getTransform()->position = glm::vec2(680.0f, 250.0f);
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
//	// Always open with a NewFrame
//	ImGui::NewFrame();
//
//	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
//	//ImGui::ShowDemoWindow();
//	
//	ImGui::Begin("GAME3001 - Lab 2", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);
//
//	static float speed = 10.0f;
//	if(ImGui::SliderFloat("MaxSpeed", &speed, 0.0f, 100.0f))
//	{
//		m_pETurret->setMaxSpeed(speed);
//		
//	}
//
//	static float acceleration_rate = 2.0f;
//	if(ImGui::SliderFloat("Acceleration Rate", &acceleration_rate, 0.0f, 50.0f))
//	{
//		m_pETurret->setAccelerationRate(acceleration_rate);
//	}
//
//	static float angleInRadians = m_pETurret->getRotation();
//	if(ImGui::SliderAngle("Orientation Angle", &angleInRadians))
//	{
//		m_pETurret->setRotation(angleInRadians * Util::Rad2Deg);
//	}
//
//	static float turn_rate = 5.0f;
//	if(ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
//	{
//		m_pETurret->setTurnRate(turn_rate);
//
//	}
//	
//	if(ImGui::Button("Start"))
//	{
//		m_pETurret->setEnabled(true);
//
//	}
//
//	ImGui::SameLine();
//	
//	if (ImGui::Button("Reset"))
//	{
//		m_pETurret->getTransform()->position = glm::vec2(100.0f, 100.0f);
//		m_pETurret->setEnabled(false);
//		m_pETurret->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
//		m_pETurret->setRotation(0.0f); // set Angle to 0 degrees
//		turn_rate = 5.0f;
//		acceleration_rate = 2.0f;
//		speed = 10.0f;
//		angleInRadians = m_pETurret->getRotation();
//
//
//
//	}
//
//	ImGui::Separator();
//
//	static float targetPosition[2] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y};
//	if(ImGui::SliderFloat2("Target", targetPosition, 0.0f, 800.0f))
//	{
//		m_pTarget->getTransform()->position = glm::vec2(targetPosition[0], targetPosition[1]);
//		m_pETurret->setDestination(m_pTarget->getTransform()->position);
//	}
//
//
//	ImGui::End();
//
//	// Don't Remove this
//	ImGui::Render();
//	ImGuiSDL::Render(ImGui::GetDrawData());
//	ImGui::StyleColorsDark();
}
