#pragma once
#include <windows.h>

LRESULT CALLBACK GLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct  ScreenPoint
{
	ScreenPoint(int _x = 0, int _y = 0);
	int x;
	int y;
};

class Mouse
{
	friend LRESULT CALLBACK GLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool m_lButton;
	bool m_rButton;
	ScreenPoint m_curPos;
	ScreenPoint m_prevPos;
public:
	Mouse();
	bool isLButtonPressed() const;
	bool isRButtonPressed() const;
	ScreenPoint getCurrentPos() const;
	ScreenPoint getPrevPos() const;
};

Mouse* GetMouse();