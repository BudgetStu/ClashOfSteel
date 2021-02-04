#pragma once
#ifndef __ETANK__
#define __ETANK__
#include "DisplayObject.h"

class Tank final : public DisplayObject
{
public:
	// constructors
	Tank();
	
	// destructor
	~Tank();
	
	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;
	bool moveRight = true;
private:

	const float speed = 2.0f;
};

#endif /* defined (__OBSTACLE__) */