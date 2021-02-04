#include "ETank.h"


#include "SoundManager.h"
#include "TextureManager.h"
#include "EventManager.h"

Tank::Tank()
{
	TextureManager::Instance()->load("../Assets/textures/Tank2.png", "etank");

	auto size = TextureManager::Instance()->getTextureSize("etank");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);
	//getTransform()->rotation = glm::vec2(90.0f, 0.0f);
	setType(ETANK);
	getRigidBody()->isColliding = false;

	
}

Tank::~Tank()
= default;

void Tank::draw()
{
	TextureManager::Instance()->draw("etank", 
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void Tank::update()
{
		if (moveRight == true) {
			getTransform()->position.y += speed;
			if (getTransform()->position.y == 500) {
				moveRight = false;
			}
		}
		else if (moveRight == false) {
			getTransform()->position.y -= speed;
			if (getTransform()->position.y == 100) {
				moveRight = true;
			}
		}

}

void Tank::clean()
{
}
