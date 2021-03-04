#pragma once
#ifndef __ETANK__
#define __ETANK__
#include "NavigationObject.h"

class ETank final : public NavigationObject
{
public:
	// constructors
	ETank();
	
	// destructor
	~ETank();
	
	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;
	bool moveRight = true;

	// getters and setters
	void setDestination(glm::vec2 destination);
	void setMaxSpeed(float speed);
	glm::vec2 getOrientation() const;
	void setOrientation(glm::vec2 orientation);
	float getRotation() const;
	void setRotation(float angle);
	float getTurnRate() const;
	void setTurnRate(float rate);
	float getAccelerationRate() const;
	void setAccelerationRate(float rate);
	void setStopRadius(float stopR);

	//CheckPoints
	bool p0 = false;
	bool p1 = false;
	bool p2 = false;
	bool p3 = false;
	bool p4 = false;
	
	float m_stopRadius;
	bool move = false;
	int moveCounterShip = 0;
private:

	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	glm::vec2 m_orientation;
	float m_rotationAngle;
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;

	// private function
	void m_Move();
private:

	const float speed = 2.0f;
};

#endif /* defined (__OBSTACLE__) */