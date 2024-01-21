#pragma once

#include "Object.h"

class NewNode : public Object
{
public:
	void Render();
	void Update(double speed);
	void ResetPos(Vec2 pos);
	void SetDest(Vec2 listPos, UINT listheight);
	
	int data;
	Vec2 dest;
	Vec2 deltaPos;
};

