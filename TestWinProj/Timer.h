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
	UINT			_callCount;		// 함수가 몇번 불렸나요.
	double			_accDT;		// 누적 DT
	UINT			_FPS;		


};

