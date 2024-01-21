#include "pch.h"
#include "Object.h"

void Object::Render(HDC dc)
{
	Rectangle(dc,
		(int)_pos.x,
		(int)_pos.y,
		(int)(_pos.x + _scale.x),
		(int)(_pos.y + _scale.y));
}
