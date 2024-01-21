#pragma once

#include "Object.h"

class Inputer : public Object
{
public:
	void Init(Vec2 pos, Vec2 sclae);
	void Update();
	void Render(HDC dc);

	void CaptureNum(char num);
	int PopData(HWND hwnd);

private:
	vector<char> _buffer;
};

