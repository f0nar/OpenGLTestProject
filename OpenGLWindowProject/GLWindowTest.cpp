#include "GLWindowTest.h"
#include "Loger.h"
#include "common.h"

#define WIN32_LEAN_AND_MEAN 1

const wchar_t GLWindowClassConstructor::GLWindowClassName[] = L"GLWindow_class";
bool GLWindowClassConstructor::isRegistered = false;
GLWindowClassConstructor GLWindowTest::glWindowClassConstructor;

GLWindowTest::GLWindowTest(const wchar_t* title, int _width, int _height)
	:width(_width), height(_height), fullScreen(false),
	active(false), running(false),
	g_hInstance(NULL), g_hDC(NULL), g_hWnd(NULL)
{

	WNDCLASSEX            wcx;
	RECT                  rect;
	DWORD                 style, exStyle;
	int                   x, y, format;

	g_hInstance = (HINSTANCE)GetModuleHandle(NULL);

	// стили окна
	style = WS_OVERLAPPEDWINDOW;
	exStyle = WS_EX_APPWINDOW;

	// выровняем окно по центру экрана
	x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	rect.left = x;
	rect.right = x + width;
	rect.top = y;
	rect.bottom = y + height;

	// подгоним размер окна под стили
	AdjustWindowRectEx(&rect, style, FALSE, exStyle);

	// создаем окно
	g_hWnd = CreateWindowEx(exStyle, GLWindowClassConstructor::GLWindowClassName, title, style, rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, g_hInstance, NULL);

	if (!g_hWnd)
	{
		LOG_ERROR("CreateWindowEx fail (%d)\n", GetLastError());
		//ERROR
	}

	// получим дескриптор контекста окна
	g_hDC = GetDC(g_hWnd);

	if (!g_hDC)
	{
		LOG_ERROR("GetDC fail (%d)\n", GetLastError());
		//ERROR
	}

	// зададим размеры окна
	setSize(width, height);

	running = active = true;
}

GLWindowTest::GLWindowTest(const wchar_t* title, bool _foolscreen)
	:GLWindowTest(title, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN))
{}

GLWindowTest::~GLWindowTest()
{
	// восстановим разрешение экрана
	if (fullScreen)
	{
		ChangeDisplaySettings(NULL, CDS_RESET);
		ShowCursor(TRUE);
		fullScreen = false;
	}

	// освобождаем контекст окна
	if (g_hDC)
	{
		ReleaseDC(g_hWnd, g_hDC);
		g_hDC = NULL;
	}

	// удаляем окно
	if (g_hWnd)
	{
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
	}

	// удаляем класс окна
	if (g_hInstance)
	{
		UnregisterClass(GLWindowClassConstructor::GLWindowClassName, g_hInstance);
		g_hInstance = NULL;
	}
}

void GLWindowTest::setSize(int width, int height)
{
	RECT    rect;
	DWORD   style, exStyle;
	DEVMODE devMode;
	LONG    result;
	int     x, y,
		MAXXSCREEN = GetSystemMetrics(SM_CXSCREEN),
		MAXYSCREEN = GetSystemMetrics(SM_CYSCREEN);
	bool isFullScreenNeed = MAXXSCREEN == width &&
		MAXYSCREEN == height;


	// если мы возвращаемся из полноэкранного режима
	if (fullScreen && !isFullScreenNeed)
	{
		ChangeDisplaySettings(NULL, CDS_RESET);
		ShowCursor(TRUE);
	}

	fullScreen = isFullScreenNeed;

	// если необходим полноэкранный режим
	if (fullScreen)
	{
		memset(&devMode, 0, sizeof(devMode));
		devMode.dmSize = sizeof(devMode);
		devMode.dmPelsWidth = width;
		devMode.dmPelsHeight = height;
		devMode.dmBitsPerPel = GetDeviceCaps(g_hDC, BITSPIXEL);
		devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		// попытка установить полноэкранный режим
		result = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
		if (result != DISP_CHANGE_SUCCESSFUL)
		{
			LOG_ERROR("ChangeDisplaySettings fail %dx%d (%d)\n", width, height, result);
			fullScreen = false;
		}
	}

	// если был запрошен полноэкранный режим и его удалось установить
	if (fullScreen)
	{
		ShowCursor(FALSE);

		style = WS_POPUP;
		exStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;

		x = y = 0;
	}
	else // если полноэкранный режим не нужен, или его не удалось установить
	{
		style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		exStyle = WS_EX_APPWINDOW;

		// выровняем окно по центру экрана
		x = (MAXXSCREEN - width) / 2;
		y = (MAXYSCREEN - height) / 2;
	}

	rect.left = x;
	rect.right = x + width;
	rect.top = y;
	rect.bottom = y + height;

	// подгоним размер окна под стили
	AdjustWindowRectEx(&rect, style, FALSE, exStyle);

	// установим стили окна
	SetWindowLong(g_hWnd, GWL_STYLE, style);
	SetWindowLong(g_hWnd, GWL_EXSTYLE, exStyle);

	// обновим позицию окна
	SetWindowPos(g_hWnd, HWND_TOP, rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		SWP_FRAMECHANGED);

	// покажем окно на экране
	ShowWindow(g_hWnd, SW_SHOW);
	SetForegroundWindow(g_hWnd);
	SetFocus(g_hWnd);
	UpdateWindow(g_hWnd);

	// получим размеры окна
	GetClientRect(g_hWnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	// центрируем курсор относительно окна
	//InputSetCursorPos(width / 2, height / 2);
}

void GLWindowTest::setFullScreen(bool _fullScreen)
{
	int width = 800, height = 600;

	if (_fullScreen) {
		if (fullScreen) return;
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}
	else {
		if (!fullScreen)
			return;
	}
	setSize(width, height);
}

bool GLWindowTest::isRunning()
{
	MSG    msg;

	// обработаем сообщения из очереди сообщений
	while (PeekMessage(&msg, g_hWnd, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			running = false;
			break;
		}
		// TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return active = running;
}

bool GLWindowTest::isActive()
{
	return active;
}

HWND GLWindowTest::getHWND()
{
	return g_hWnd;
}

HDC GLWindowTest::getDisplayContext()
{
	return g_hDC;
}

int GLWindowTest::getWidth()
{
	return width;
}

int GLWindowTest::getHeight()
{
	return height;
}