#include "PlayerTank.h"

#include "Game.h"
#include "Util.h"
#include "EventManager.h"

PlayerTank::PlayerTank()
{
	TextureManager::Instance()->load("../Assets/textures/tiger.png", "Tiger");

	auto size = TextureManager::Instance()->getTextureSize("Tiger");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER_TANK);
	setMaxSpeed(10.0f);
	setOrientation(glm::vec2(0.0f, -1.0f));
	setRotation(0.0f);
	setAccelerationRate(00.0f);
	setTurnRate(00.0f);
}

PlayerTank::~PlayerTank()
= default;

void PlayerTank::draw()
{
	TextureManager::Instance()->draw("Tiger",
		getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255, true);

	Util::DrawLine(getTransform()->position, (getTransform()->position + getOrientation() * 60.0f));
}

void PlayerTank::update()
{
	m_Move();

}


void PlayerTank::clean()
{
}

void PlayerTank::setMaxSpeed(float speed)
{
	m_maxSpeed = speed;
}

glm::vec2 PlayerTank::getOrientation() const
{
	return m_orientation;
}

void PlayerTank::setOrientation(glm::vec2 orientation)
{
	m_orientation = orientation;
}

float PlayerTank::getRotation() const
{
	return m_rotationAngle;
}

void PlayerTank::setRotation(float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

float PlayerTank::getTurnRate() const
{
	return m_turnRate;
}

void PlayerTank::setTurnRate(float rate)
{
	m_turnRate = rate;
}

float PlayerTank::getAccelerationRate() const
{
	return m_accelerationRate;
}

void PlayerTank::setAccelerationRate(float rate)
{
	m_accelerationRate = rate;
}

void PlayerTank::m_Move()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
	{
		getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

		// using the formula pf = pi + vi*t + 0.5ai*t^2
		getRigidBody()->velocity += getOrientation() * (deltaTime)+
			0.5f * getRigidBody()->acceleration * (deltaTime);

		getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

		getTransform()->position += getRigidBody()->velocity;
	}
	else
	{
		getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	}
	EventManager::Instance().update();
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
	{
		setTurnRate(1.0f);
		setRotation(getRotation()+ getTurnRate());
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
	{
		setTurnRate(-1.0f);
		setRotation(getRotation() + getTurnRate());
	}
}

