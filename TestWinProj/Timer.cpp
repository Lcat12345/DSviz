#include "pch.h"
#include "Timer.h"

void Timer::Init()
{
	QueryPerformanceCounter(&_prevCount);

	// 고해상도 타이머의 주기(1초당 진동 수)를 반환
	// 1초에 몇번의 카운트가 나올 수 있을까.
	QueryPerformanceFrequency(&_Frequency);
}

void Timer::Update(HWND hwnd)
{
	_callCount++;
	
	QueryPerformanceCounter(&_curCount);

	// 한 프레임이 지났을 때 얼마 만큼의 카운트가 나오나요.
	// _curCount.QuadPart - _prevCount.QuadPart;

	// 한 프레임 당 걸리는 몇초가 걸리냐..
	// 빈도수는 초당 카운트
	// 빈도수 = 1초
	//  카운트 차이 / 1초만큼의 카운트 = 한 프레임에 몇초
	_deltaTime = (double)(_curCount.QuadPart - _prevCount.QuadPart) / (double)_Frequency.QuadPart;

	_prevCount = _curCount;

	// accDT가 1이 되면 1초, DT는 분수이므로
	_accDT += _deltaTime;

	if (_accDT >= 1.0)
	{
		_FPS = _callCount;	// 이 함수는 1초에 몇번 호출 됬을까요?
		_accDT = 0;
		_callCount = 0;		
		/*wchar_t buffer[255] = {};
		swprintf_s(buffer, L"FPS : %d, DT : %lf", _FPS, _deltaTime);
		SetWindowText(hwnd, buffer);*/
	}
}

void Timer::Render(HDC dc)
{
	wchar_t buffer[255] = {};
	swprintf_s(buffer, L"FPS : %d", _FPS);

	RECT textRect = { 0, 0, 0, 0 };
	DrawText(dc, buffer, -1, &textRect, DT_CALCRECT);

	SetBkMode(dc, TRANSPARENT);
	SetTextColor(dc, RGB(102, 255, 102));
	TextOut(dc, 5, 0, buffer, lstrlen(buffer));
	SetTextColor(dc, RGB(0, 0, 0));
}
