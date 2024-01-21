#include "pch.h"
#include "Window.h"

Window::~Window()
{
	ReleaseDC(HANDLE, _hDC);
	DeleteDC(_drawDC);
	DeleteObject(_bitMap);

	for (int i = 0; i < (UINT)FONT_TYPE::END; ++i)
	{
		DeleteObject(_hFont[i]);
	}

	for (int i = 0; i < (UINT)BRUSH_TYPE::END; ++i)
	{
		DeleteObject(_hBrush[i]);
	}

	for (int i = 0; i < (UINT)PEN_TYPE::END; ++i)
	{
		DeleteObject(_hPen[i]);
	}
}

void Window::Init(const WindowInfo& info)
{
	_WndInfo = info;
	ResizeWindow(info.width, info.height);

	_hDC = GetDC(HANDLE);

	// double buffering //
	_bitMap = CreateCompatibleBitmap(_hDC, info.width, info.height);
	_drawDC = CreateCompatibleDC(_hDC);

	HBITMAP oldBitmap = (HBITMAP)SelectObject(_drawDC, _bitMap);
	DeleteObject(oldBitmap);
	//================================================================//

	_timer.Init();
	_inputer.Init({ 125, 100 }, { 200, 50 });
	bstrender.Init(50.0);

	CreateFonts();
	CreateBrushes();
	CreatePens();

	node._scale.x = 100;
	node._scale.y = 50;
	listPos.x = _WndInfo.width / 2;
	listPos.y = 50;
}

void Window::Update()
{
	_timer.Update(HANDLE);

	if (!IsAnimated)
	{
		_inputer.Update();
		//============================//
		if (GetAsyncKeyState(VK_RETURN) & 0x0001)
		{
			int num = _inputer.PopData(HANDLE);
			if (-1 != num)
			{
				// list.push_back(num);
				bstrender.insert(num);
				return;

				// animation
				IsAnimated = true;
				node.ResetPos({ 100, 200 });
				node.SetDest(listPos, GetListHeight());
				node.data = num;
			}
		}

		if (GetAsyncKeyState(82) & 0x8000)
		{
			list.clear();
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x0001)
		{
			bstrender.NODE_SCALE -= 1;
		}

		if (GetAsyncKeyState(VK_UP) & 0x0001)
		{
			bstrender.NODE_SCALE += 1;
		}

	}
	else
	{	
		node.Update(120.0);

		if (node._pos.x >= listPos.x)
		{
			IsAnimated = false;
			list.push_back(node.data);
		}
	}
}

void Window::Render()
{
	Clear();
	HFONT prevFont; HPEN prevPen;

	prevFont = (HFONT)SelectObject(_drawDC, _hFont[(UINT)FONT_TYPE::FPS]);
	_timer.Render(_drawDC);

	prevFont = (HFONT)SelectObject(_drawDC, _hFont[(UINT)FONT_TYPE::NODE]);
	if(!IsAnimated)
		prevPen = (HPEN)SelectObject(_drawDC, _hPen[(UINT)PEN_TYPE::HALF_GRAY]);
	else
		prevPen = (HPEN)SelectObject(_drawDC, _hPen[(UINT)PEN_TYPE::HALF_RED]);
	_inputer.Render(_drawDC);
		
	prevPen = (HPEN)SelectObject(_drawDC, _hPen[(UINT)PEN_TYPE::BLACK]);
	// DrawList();

	bstrender.Render(_drawDC, _WndInfo.width, _WndInfo.height);

	char str[255] = {};
	sprintf_s(str, "NODE_SCALE : %lf", bstrender.NODE_SCALE);
	TextOutA(_drawDC, 250, 30, str, strlen(str));
	
	if (IsAnimated)
	{
		// box
		DrawRectangle(node._pos, node._scale);

		// text
		char str[255] = {};
		sprintf_s(str, "%d", node.data);
		RECT textRect = { 0, 0, 0, 0 }; // right = width, bottom = height
		DrawTextA(_drawDC, str, -1, &textRect, DT_CALCRECT);

		SetTextAlign(_drawDC, TA_CENTER);	// 텍스트의 중앙을 postion으로
		double padding = textRect.bottom / 2.0;	// 텍스트 높이의 절반 만큼 올리기
		TextOutA(_drawDC, (int)node._pos.x, (int)(node._pos.y - padding), str, (int)strlen(str));
		SetTextAlign(_drawDC, TA_TOP);	// 원상복구
	}


	BitBlt(_hDC, 0, 0, _WndInfo.width, _WndInfo.height, _drawDC, 0, 0, SRCCOPY);
}

void Window::ResizeWindow(int width, int height)
{
	_WndInfo.width = width;
	_WndInfo.height = height;

	RECT rect = { 0,0,width,height };

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(HANDLE, 0, 100, 100, width, height, 0);

}

void Window::DrawRectangle(Vec2 pos, Vec2 scale)
{
	Vec2 radius;
	radius.x = scale.x / 2.0;
	radius.y = scale.y / 2.0;

	Rectangle(_drawDC
		, (int)(pos.x - radius.x)
		, (int)(pos.y - radius.y)
		, (int)(pos.x + radius.x)
		, (int)(pos.y + radius.y));
}

void Window::Clear()
{
	RECT rect = { -1, -1, _WndInfo.width + 1, _WndInfo.height + 1 };
	//FillRect(_drawDC, &rect, (HBRUSH)GetStockObject(GRAY_BRUSH));
	FillRect(_drawDC, &rect, _hBrush[(UINT)BRUSH_TYPE::WHITE_GRAY]);
	// Rectangle(_drawDC, -1, -1, _WndInfo.width + 1, _WndInfo.height + 1);
}

void Window::DrawList()
{
	Vec2 pos = listPos;

	for (vector<int>::iterator iter = list.begin(); iter != list.end(); ++iter)
	{
		// line
		if (list.end() != iter + 1)
		{
			MoveToEx(_drawDC, (int)pos.x, (int)pos.y, NULL);
			LineTo(_drawDC, (int)pos.x, (int)(pos.y + NODE_DISTANCE + node._scale.y / 2.0));
		}

		// box
		DrawRectangle(pos, node._scale);

		char str[255] = {};
		sprintf_s(str, "%d", *iter);
		RECT textRect = { 0, 0, 0, 0 }; // right = width, bottom = height
		DrawTextA(_drawDC, str, -1, &textRect, DT_CALCRECT);

		SetTextAlign(_drawDC, TA_CENTER);	// 텍스트의 중앙을 postion으로
		double padding = textRect.bottom / 2.0;	// 텍스트 높이의 절반 만큼 올리기
		TextOutA(_drawDC, (int)pos.x, (int)(pos.y - padding), str, (int)strlen(str));

		pos.y += node._scale.y + NODE_DISTANCE;
	}
	SetTextAlign(_drawDC, TA_TOP);	// 원상복구
}

void Window::CreateFonts()
{
	// font
	_hFont[(UINT)FONT_TYPE::FPS] = CreateFont(
		50,								// 높이
		0,								// 폭
		0,								// 각도
		0,								// 기울기
		FW_NORMAL,						// 글꼴 두께
		FALSE,							// Italic 여부
		FALSE,							// 밑줄 여부
		FALSE,							// 취소선 여부
		DEFAULT_CHARSET,				// 문자 집합
		OUT_DEFAULT_PRECIS,				// 출력 정밀도
		CLIP_DEFAULT_PRECIS,			// 클리핑 정밀도
		DEFAULT_QUALITY,				// 출력 품질
		DEFAULT_PITCH | FF_DONTCARE,	// 글꼴 속성
		L"던파 비트비트체 v2"						// 글꼴 이름
	);

	_hFont[(UINT)FONT_TYPE::NODE] = CreateFont(
		30,								// 높이
		0,								// 폭
		0,								// 각도
		0,								// 기울기
		FW_NORMAL,						// 글꼴 두께
		FALSE,							// Italic 여부
		FALSE,							// 밑줄 여부
		FALSE,							// 취소선 여부
		DEFAULT_CHARSET,				// 문자 집합
		OUT_DEFAULT_PRECIS,				// 출력 정밀도
		CLIP_DEFAULT_PRECIS,			// 클리핑 정밀도
		DEFAULT_QUALITY,				// 출력 품질
		DEFAULT_PITCH | FF_DONTCARE,	// 글꼴 속성
		L"던파 비트비트체 v2"						// 글꼴 이름
	);
	//
}

void Window::CreateBrushes()
{
	_hBrush[(UINT)BRUSH_TYPE::WHITE_GRAY] = (HBRUSH)CreateSolidBrush(RGB(192, 192, 192));
}

void Window::CreatePens()
{
	_hPen[(UINT)PEN_TYPE::HALF_GRAY] = (HPEN)CreatePen(PS_SOLID, 5, RGB(153, 153, 153));

	_hPen[(UINT)PEN_TYPE::HALF_RED] = (HPEN)CreatePen(PS_SOLID, 5, RGB(255, 51, 51));


	_hPen[(UINT)PEN_TYPE::BLACK] = (HPEN)CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
}
