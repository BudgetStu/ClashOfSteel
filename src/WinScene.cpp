﻿#include "WinScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

WinScene::WinScene()
{
	WinScene::start();
}

WinScene::~WinScene()
= default;

void WinScene::draw()
{
	//TextureManager::Instance()->draw("background", 400, 300, 0, 255, true);

	drawDisplayList();
}

void WinScene::update()
{
	updateDisplayList();
}

void WinScene::clean()
{
	removeAllChildren();
}

void WinScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void WinScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/background.png", "background");

	const SDL_Color blue = { 0, 0, 255, 255 };
	m_pStartLabel = new Label("WIN SCENE", "Consolas", 80, blue, glm::vec2(400.0f, 40.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	/*m_pInstructionsLabel = new Label("Press 1 to Play", "Consolas", 40, blue, glm::vec2(400.0f, 120.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);*/

	//TODO Change button sprite to "Restart", Same use
	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(200.0f, 300.0f);

	m_pStartButton->addEventListener(CLICK, [&]()-> void
		{
			m_pStartButton->setActive(false);
			TheGame::Instance()->changeSceneState(PLAY_SCENE);
		});

	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pStartButton->setAlpha(128);
		});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pStartButton->setAlpha(255);
		});
	addChild(m_pStartButton);

	//TODO Change button sprite to "Main menu"
	// Exit Button
	m_pExitButton = new Button("../Assets/textures/backButton.png", "exit");
	m_pExitButton->getTransform()->position = glm::vec2(500.0f, 300.0f);

	m_pExitButton->addEventListener(CLICK, [&]()-> void
		{
			m_pExitButton->setActive(false);
			TheGame::Instance()->changeSceneState(START_SCENE);
		});

	m_pExitButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pExitButton->setAlpha(128);
		});

	m_pExitButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pExitButton->setAlpha(255);
		});
	addChild(m_pExitButton);
}