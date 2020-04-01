#include <windows.h>
#include "GLWindowTest.h"
#include "Loger.h"
#include "GLContext.h"
#include "GLContextDescriptor.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	LoggerCreate("OpenGLWindowProject.log");
	GLWindowTest *glWindow = GLWindowCreate(L"GLWindow", 800, 600);
	GLContextDescriptor glContextDescriptor (glWindow);
	GLContext glContext(&glContextDescriptor);
	glContext.mainLoop();
	return 0;
}
