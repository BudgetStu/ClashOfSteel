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
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();

	// set the destination to the player once this code is gucci
	m_pETurret[1]->setDestination(m_pPlayerTurret->getTransform()->position);
	// set the destination to the player once this code is gucci
	m_pETurret[2]->setDestination(m_pPlayerTurret->getTransform()->position);
	// set the destination to the player once this code is gucci
	m_pETurret[3]->setDestination(m_pPlayerTurret->getTransform()->position);


	//binds the turret with the tank body
	m_pETurret[1]->getTransform()->position = m_pEnemyTank[1]->getTransform()->position;

	// set the destination to the player once this code is gucci
	m_pETurret[2]->setDestination(m_pPlayerTurret->getTransform()->position);


	//binds the turret with the tank body
	m_pETurret[2]->getTransform()->position = m_pEnemyTank[2]->getTransform()->position;

	// set the destination to the player once this code is gucci
	m_pETurret[3]->setDestination(m_pPlayerTurret->getTransform()->position);


	//binds the turret with the tank body
	m_pETurret[3]->getTransform()->position = m_pEnemyTank[3]->getTransform()->position;

	//binds the turret with the tank body
	m_pPlayerTurret->getTransform()->position = m_pPlayerTank->getTransform()->position;

	//m_pPlayerTurret->setDestination()

	//Collision
	//for (unsigned i = 0; i < 4;i++)
	//{
		if(CollisionManager::circleAABBCheck(m_pPlayerTank,m_pEnemyTank[1]))
		{
			std::cout << "Collision" << std::endl;
			m_pEnemyTank[1]->speed = 0.0f;
		}
		else
		{
			m_pEnemyTank[1]->speed = 2.0f;
		}
	//}
	
	for (unsigned i = 0; i < m_bullet.size(); i++) //size() is actual filled numbers of elements
		m_bullet[i]->update();
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
		m_bullet.push_back(new Bullet);
		for (unsigned i = 0; i < m_bullet.size(); i++)
		{
			m_bullet[i]->draw();
		}
		m_bullet.shrink_to_fit();
		std::cout << "Created" << std::endl;
	}
	
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";


	m_pEnemyTank[1] = new Tank();
	m_pEnemyTank[1]->getTransform()->position = glm::vec2(300.0f, 300.0f);
	addChild(m_pEnemyTank[1]);

	m_pEnemyTank[2] = new Tank();
	m_pEnemyTank[2]->getTransform()->position = glm::vec2(500.0f, 300.0f);
	addChild(m_pEnemyTank[2]);

	m_pEnemyTank[3] = new Tank();
	m_pEnemyTank[3]->getTransform()->position = glm::vec2(700.0f, 300.0f);
	addChild(m_pEnemyTank[3]);

	// instantiating spaceship
	m_pETurret[1] = new eTurret();
	m_pETurret[1]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	//m_pSpaceShip->setEnabled(false);
	addChild(m_pETurret[1]);
	
	// instantiating spaceship
	m_pETurret[2] = new eTurret();
	m_pETurret[2]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	//m_pSpaceShip->setEnabled(false);
	addChild(m_pETurret[2]);
	
	// instantiating spaceship
	m_pETurret[3] = new eTurret();
	m_pETurret[3]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	//m_pSpaceShip->setEnabled(false);
	addChild(m_pETurret[3]);
	
	m_pPlayerTank = new PlayerTank();
	m_pPlayerTank->getTransform()->position = glm::vec2(100.0f, 300.0f);
	m_pPlayerTank->setEnabled(true);
	addChild(m_pPlayerTank);

	m_pPlayerTurret = new pTurret();
	m_pPlayerTurret->getTransform()->position== glm::vec2(100.0f, 300.0f);
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
