#pragma once

#include "Timer.h"
#include "Inputer.h"
#include "Node.h"
#include "BSTRender.h"

class Window
{
public:
	~Window();

public:
	void Init(const WindowInfo& info);
	void Update();
	void Render();

	void ResizeWindow(int width, int height);
	void DrawRectangle(Vec2 pos, Vec2 scale);
	void Clear();

	UINT GetListHeight() { return (UINT)list.size(); }
	void DrawList();

	void CreateFonts();
	void CreateBrushes();
	void CreatePens();

public:
	HWND GetHandle() { return _WndInfo.hwnd; }
	Inputer& GetInputer() { return _inputer; }

private:
	Timer				_timer;
	Inputer				_inputer;

private:
	vector<int>			list;
	Vec2				listPos;

	BSTRender			bstrender;

private:
	HDC					_hDC;
	WindowInfo			_WndInfo;

	HBITMAP				_bitMap;
	HDC					_drawDC;

	HBRUSH				_hBrush[(UINT)BRUSH_TYPE::END];
	HFONT				_hFont[(UINT)FONT_TYPE::END];
	HPEN				_hPen[(UINT)PEN_TYPE::END];

public:
	bool				IsAnimated = false;
	NewNode				node;
};

