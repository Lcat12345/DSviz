#include "pch.h"
#include "Node.h"

void NewNode::Render()
{

}

void NewNode::Update(double speed)
{
	_pos.x += deltaPos.x / speed;
	_pos.y += deltaPos.y / speed;
}

void NewNode::ResetPos(Vec2 pos)
{
	_pos = pos;
}

void NewNode::SetDest(Vec2 listPos, UINT listheight)
{
	dest.x = listPos.x;

	dest.y = listPos.y + listheight * (NODE_DISTANCE + _scale.y) + _scale.y / 2.0;

	deltaPos.x = dest.x - _pos.x;
	deltaPos.y = dest.y - _pos.y;
}

