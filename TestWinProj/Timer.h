#pragma once

class Timer
{
public:
	void Init();
	void Update(HWND hwnd);
	void Render(HDC dc);

	double GetDeltaTime() { return _deltaTime; }

private:
	LARGE_INTEGER	_curCount;
	LARGE_INTEGER	_prevCount;
	LARGE_INTEGER	_Frequency;

	double			_deltaTime;
	UINT			_callCount;		// �Լ��� ��� �ҷȳ���.
	double			_accDT;		// ���� DT
	UINT			_FPS;		


};

