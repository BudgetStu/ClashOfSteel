#include "Bullet.h"

#include "Game.h"
#include "Util.h"
#include "EventManager.h"

Bullet::Bullet()
{
	TextureManager::Instance()->load("../Assets/textures/Tbullet.png", "Tbullet");

	auto size = TextureManager::Instance()->getTextureSize("Tbullet");
	setWidth(size.x);
	setHeight(size.y);

	//getTransform()->position = glm::vec2(400.0f, 300.0f);
	//setType(BULLET);
	//getRigidBody()->isColliding = false;
	//setOrientation(orientation);
}

Bullet::~Bullet()
= default;

void Bullet::draw()
{
	TextureManager::Instance()->draw("Bullet",
		getTransform()->position.x, getTransform()->position.y,
		m_rotationAngle, 255, true);
}

void Bullet::update()
{
	Shoot();
}

void Bullet::handeEvents()
{
	
}

void Bullet::clean()
{
	
}

//glm::vec2 Bullet::getOrientation() const
//{
//	return m_orientation;
//}
//
//void Bullet::setOrientation(glm::vec2 orientation)
//{
//	m_orientation = orientation;
//}
//
//float Bullet::getRotation() const
//{
//	return m_rotationAngle;
//}
//
//void Bullet::setRotation(float angle)
//{
//	m_rotationAngle = angle;
//
//	const auto offset = -90.0f;
//	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;
//
//	const auto x = cos(angle_in_radians);
//	const auto y = sin(angle_in_radians);
//
//	// convert the angle to a normalized vector and store it in Orientation
//	setOrientation(glm::vec2(x, y));
//}

void Bullet::Shoot()
{
	
	
}







