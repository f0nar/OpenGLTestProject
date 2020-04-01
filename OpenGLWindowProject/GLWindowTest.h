#pragma once
#include <windows.h>
#include "common.h"

LRESULT CALLBACK GLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class GLWindowClassConstructor
{
public:
	static const wchar_t GLWindowClassName[];
	GLWindowClassConstructor()
	{
		if (!isRegistered) {
			WNDCLASSEX            wcx;
			HINSTANCE			  g_hInstance;

			g_hInstance = (HINSTANCE)GetModuleHandle(NULL);

			memset(&wcx, 0, sizeof(wcx));
			wcx.cbSize = sizeof(wcx);
			wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wcx.lpfnWndProc = (WNDPROC)GLWindowProc;
			wcx.hInstance = g_hInstance;
			wcx.lpszClassName = GLWindowClassName;
			wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wcx.hCursor = LoadCursor(NULL, IDC_ARROW);

			if (!RegisterClassEx(&wcx))
			{
				//ERROR
			}
			isRegistered = true;
		}
	}
private:
	static bool isRegistered;
};

class GLWindowTest
{
	friend LRESULT CALLBACK GLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int width,
		height;

	// флаги состояние
	bool fullScreen,
		active,
		running;

	HINSTANCE g_hInstance;
	HWND      g_hWnd;
	HDC       g_hDC;

	static GLWindowClassConstructor glWindowClassConstructor;

public:
	GLWindowTest(const wchar_t* title, int width = 800, int height = 600);
	GLWindowTest(const wchar_t* title, bool fullscreen);
	void setSize(int width = 800, int heigth = 600);
	void setFullScreen(bool fullscreen);
	int getWidth();
	int getHeight();
	bool isRunning();
	bool isActive();
	HWND getHWND();
	HDC getDisplayContext();
	~GLWindowTest();
};

GLWindowTest *GLWindowCreate(const wchar_t* title, int width, int height);
GLWindowTest *GLWindowCreate(const wchar_t* title, bool fullScreen);

