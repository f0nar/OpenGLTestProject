#include "GLWindowTest.h"
#include "Mouse.h"
#include "Loger.h"
#include "common.h"

static GLWindowTest* glWindow = nullptr;
static Mouse* mouse = nullptr;

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
		{
			if (mouse) {
				mouse->m_lButton = true;

				mouse->m_curPos.x = LOWORD(lParam);
				mouse->m_curPos.y = HIWORD(lParam);

				mouse->m_prevPos = mouse->m_curPos;
			}
			return FALSE;
		}
		case WM_LBUTTONUP:
		{
			if(mouse) mouse->m_lButton = false;
			return FALSE;
		}
		case WM_RBUTTONDOWN:
		{
			if (mouse) mouse->m_rButton = true;
			return FALSE;
		}
		case WM_RBUTTONUP:
		{
			if (mouse) mouse->m_rButton = false;
			return FALSE;
		}
		case WM_MOUSEMOVE:
		{
			if (mouse) {
				mouse->m_prevPos = mouse->m_curPos;

				mouse->m_curPos.x = LOWORD(lParam);
				mouse->m_curPos.y = HIWORD(lParam);
			}
			return FALSE;
		}
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		case WM_ACTIVATE:
		{
			if (glWindow) {
				glWindow->active = msg == WM_ACTIVATE ? (LOWORD(wParam) != WA_INACTIVE) : (msg == WM_SETFOCUS);
			}
			return FALSE;
		}
		case WM_CLOSE:
		{
			if (glWindow) {
				glWindow->running = glWindow->active = false;
				PostQuitMessage(0);
			}
			return FALSE;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			if (glWindow) {
				if (wParam == VK_ESCAPE)
				{
					glWindow->active = glWindow->running = false;
				}
			}
			return FALSE;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


