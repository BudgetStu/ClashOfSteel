#pragma
#ifndef __PLAYER_TANK__
#define __PLAYER_TANK__

#include "DisplayObject.h"
#include "TextureManager.h"

class PlayerTank final: public DisplayObject
{
public:
	PlayerTank();

	~PlayerTank();
	
	void draw() override;
	void update() override;
    void handleEvents();
	void clean() override;

	void setMaxSpeed(float speed);
	glm::vec2 getOrientation() const;
	void setOrientation(glm::vec2 orientation);
	float getRotation() const;
	void setRotation(float angle);
	float getTurnRate() const;
	void setTurnRate(float rate);
	float getAccelerationRate() const;
	void setAccelerationRate(float rate);

private:
	glm::vec2 m_orientation;
	float m_rotationAngle;
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;

	void m_Move();
};

#endif
