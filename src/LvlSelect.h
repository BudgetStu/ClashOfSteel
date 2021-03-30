#pragma once
#ifndef __LVL_SELECT__
#define __LVL_SELECT__

#include "Scene.h"
#include "Label.h"
#include "Button.h"

class LvlSelect final : public Scene
{
public:
	LvlSelect();
	~LvlSelect();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

private:

	Button* m_pLvl1Button;
	Button* m_pLvl2Button;
	Button* m_pLvl3Button;
	Button* m_pExitButton;
};

#endif /* defined (__START_SCENE__) */


