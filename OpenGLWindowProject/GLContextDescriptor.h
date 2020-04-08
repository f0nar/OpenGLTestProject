#pragma once
#include <windows.h>

class GLWindowTest;

class GLContextDescriptor
{
	HGLRC g_hRC;
	GLWindowTest *glWindow;
	bool initContext();
public:
	GLContextDescriptor(GLWindowTest* _glWindow);
	bool isRunning();
	bool isActive();
	int getWidth();
	int getHeight();
	HWND getWindowDescriptor();
	HGLRC getRenderContext();
	HDC getDisplayContext();
	~GLContextDescriptor();
};

