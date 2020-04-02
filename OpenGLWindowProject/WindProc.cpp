#include "GLWindowTest.h"
#include "Mouse.h"
#include "Loger.h"
#include "common.h"

static GLWindowTest* glWindow;
static Mouse* mouse;

GLWindowTest* GLWindowCreate(const wchar_t* title, int width, int height)
{
	glWindow = new GLWindowTest(title, width, height);
	return glWindow;
}

GLWindowTest* GLWindowCreate(const wchar_t* title, bool fullScreen)
{
	glWindow = new GLWindowTest(title, fullScreen);
	return glWindow;
}

Mouse* GetMouse()
{
	if (!mouse)
		mouse = new Mouse();

	return mouse;
}

LRESULT CALLBACK GLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		{
			if (!mouse) return FALSE;

			//mouse->m_prevPos = mouse->m_curPos;
			mouse->m_curPos.x = LOWORD(lParam);
			mouse->m_curPos.y = HIWORD(lParam);

			if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP)
				mouse->m_lButton = (msg == WM_LBUTTONDOWN ? true : false);

			if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP)
				mouse->m_rButton = (msg == WM_RBUTTONDOWN ? true : false);

			return FALSE;
		}
		case WM_MOUSEMOVE:
		{
			mouse->m_curPos.x = LOWORD(lParam);
			mouse->m_curPos.y = HIWORD(lParam);
			return FALSE;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


