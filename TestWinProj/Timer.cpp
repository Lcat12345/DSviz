#include "pch.h"
#include "Timer.h"

void Timer::Init()
{
	QueryPerformanceCounter(&_prevCount);

	// ���ػ� Ÿ�̸��� �ֱ�(1�ʴ� ���� ��)�� ��ȯ
	// 1�ʿ� ����� ī��Ʈ�� ���� �� ������.
	QueryPerformanceFrequency(&_Frequency);
}

void Timer::Update(HWND hwnd)
{
	_callCount++;
	
	QueryPerformanceCounter(&_curCount);

	// �� �������� ������ �� �� ��ŭ�� ī��Ʈ�� ��������.
	// _curCount.QuadPart - _prevCount.QuadPart;

	// �� ������ �� �ɸ��� ���ʰ� �ɸ���..
	// �󵵼��� �ʴ� ī��Ʈ
	// �󵵼� = 1��
	//  ī��Ʈ ���� / 1�ʸ�ŭ�� ī��Ʈ = �� �����ӿ� ����
	_deltaTime = (double)(_curCount.QuadPart - _prevCount.QuadPart) / (double)_Frequency.QuadPart;

	_prevCount = _curCount;

	// accDT�� 1�� �Ǹ� 1��, DT�� �м��̹Ƿ�
	_accDT += _deltaTime;

	if (_accDT >= 1.0)
	{
		_FPS = _callCount;	// �� �Լ��� 1�ʿ� ��� ȣ�� �������?
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
