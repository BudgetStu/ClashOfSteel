#pragma once
#ifndef _BULLET_
#define _BULLET_

#include "DisplayObject.h"
#include "TextureManager.h"
#include "PlayerTank.h"

class Bullet :public PlayerTank
{
public:
	Bullet();

	~Bullet();

	void draw() override;
	void update() override;
	void handeEvents();
	void clean() override;

	//glm::vec2 getOrientation() const;
	//void setOrientation(glm::vec2 orientation);
	//float getRotation() const;
	//void setRotation(float angle);
private:
	//glm::vec2 m_orientation;
	//float m_rotationAngle;
	const float m_Speed= 4.0f;

	void Shoot();

};
#endif


