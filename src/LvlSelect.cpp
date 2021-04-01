#include "LvlSelect.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

LvlSelect::LvlSelect()
{
	LvlSelect::start();
	SoundManager::Instance().load("../Assets/audio/WBgm.mp3", "WBgm", SOUND_MUSIC);
	SoundManager::Instance().playMusic("WBgm", -1, 0);
}

LvlSelect::~LvlSelect()
= default;

void LvlSelect::draw()
{
	TextureManager::Instance()->draw("LvlSelect", 400, 300, 0, 255, true);

	drawDisplayList();
}

void LvlSelect::update()
{
	updateDisplayList();
}

void LvlSelect::clean()
{
	removeAllChildren();
}

void LvlSelect::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	/*if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}*/
}

void LvlSelect::start()
{
	TextureManager::Instance()->load("../Assets/textures/bgLVL.png", "LvlSelect");

	/*const SDL_Color blue = { 0, 0, 255, 255 };
	m_pStartLabel = new Label("WIN SCENE", "Consolas", 80, blue, glm::vec2(400.0f, 40.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);*/

	/*m_pInstructionsLabel = new Label("Press 1 to Play", "Consolas", 40, blue, glm::vec2(400.0f, 120.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);*/

	//TODO Change button sprite to "Restart", Same use
	// Start Button
	m_pLvl1Button = new Button("../Assets/textures/LVL1Button.png", "Lvl1");
	m_pLvl1Button->getTransform()->position = glm::vec2(175.0f, 272.0f);

	m_pLvl1Button->addEventListener(CLICK, [&]()-> void
		{
			m_pLvl1Button->setActive(false);
			TheGame::Instance()->changeSceneState(PLAY_SCENE);
		});

	m_pLvl1Button->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pLvl1Button->setAlpha(128);
		});

	m_pLvl1Button->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pLvl1Button->setAlpha(255);
		});
	addChild(m_pLvl1Button);

	m_pLvl2Button = new Button("../Assets/textures/LVL2Button.png", "Lvl2");
	m_pLvl2Button->getTransform()->position = glm::vec2(355.0f, 272.0f);

	m_pLvl2Button->addEventListener(CLICK, [&]()-> void
		{
			m_pLvl2Button->setActive(false);
			TheGame::Instance()->changeSceneState(PLAY_SCENE2);
		});

	m_pLvl2Button->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pLvl2Button->setAlpha(128);
		});

	m_pLvl2Button->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pLvl2Button->setAlpha(255);
		});
	addChild(m_pLvl2Button);

	m_pLvl3Button = new Button("../Assets/textures/LVL3Button.png", "Lvl3");
	m_pLvl3Button->getTransform()->position = glm::vec2(530.0f, 260.0f);

	m_pLvl3Button->addEventListener(CLICK, [&]()-> void
		{
			m_pLvl3Button->setActive(false);
			TheGame::Instance()->changeSceneState(PLAY_SCENE);
		});

	m_pLvl3Button->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pLvl3Button->setAlpha(128);
		});

	m_pLvl3Button->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pLvl3Button->setAlpha(255);
		});
	addChild(m_pLvl3Button);

	//TODO Change button sprite to "Main menu"
	// Exit Button
	m_pExitButton = new Button("../Assets/textures/menuButton.png", "menu");
	m_pExitButton->getTransform()->position = glm::vec2(700.0f, 250.0f);

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