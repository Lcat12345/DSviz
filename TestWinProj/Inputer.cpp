#include "pch.h"
#include "Inputer.h"

void Inputer::Init(Vec2 pos, Vec2 sclae)
{
	_pos = pos;
	_scale = sclae;
}

void Inputer::Update()
{
	if (GetAsyncKeyState(VK_BACK) & 0x0001)
	{
		if (!_buffer.empty())
		{
			_buffer.pop_back();
		}
	}

}

void Inputer::Render(HDC dc)
{
	// description
	wchar_t buffer[255] = {};
	swprintf_s(buffer, L"Insert Key : Enter");
	TextOut(dc, _pos.x + 150.0, _pos.y - 20.0, buffer, lstrlen(buffer));

	// set str
	_buffer.push_back('\0');
	char* str = _buffer.data();

	// box
	Vec2 radius;
	radius.x = _scale.x / 2.0;
	radius.y = _scale.y / 2.0;

	RoundRect(dc
		, (int)(_pos.x - radius.x)
		, (int)(_pos.y - radius.y)
		, (int)(_pos.x + radius.x)
		, (int)(_pos.y + radius.y)
		, 50, 50);

	// text
	RECT textRect = { 0, 0, 0, 0 };
	DrawTextA(dc, str, -1, &textRect, DT_CALCRECT);	// text 크기 알아내기
	
	SetTextAlign(dc, TA_CENTER);	// 텍스트의 중앙을 postion으로
	double padding = textRect.bottom / 2.0;	// 텍스트 높이의 절반 만큼 올리기
	TextOutA(dc, (int)_pos.x, (int)(_pos.y - padding), str, (int)strlen(str));
	SetTextAlign(dc, TA_TOP);	// 원상복구

	// back origin
	_buffer.pop_back();
}

void Inputer::CaptureNum(char num)
{
	if (num >= '0' && num <= '9')
	{
		if(_buffer.size() < 5)
			_buffer.push_back(num);
	}
}

int Inputer::PopData(HWND hwnd)
{
	if (_buffer.empty())
	{
		return -1;
	}

	if (_buffer.front() == '0')
	{
		MessageBox(hwnd, L"0으로 시작할 수 없습니다.", L"error", MB_OK);
		return -1;
	}

	int data = atoi(_buffer.data());
	_buffer.clear();

	return data;
}
