#include "GLContextDescriptor.h"
#include "GLWindowTest.h"
#include "Loger.h"
#include "OpenGL.h"

GLContextDescriptor::GLContextDescriptor(GLWindowTest* _glWindow)
	:glWindow(_glWindow)
{
	ASSERT(glWindow);
	ASSERT(initContext());
}

bool GLContextDescriptor::initContext()
{
	PIXELFORMATDESCRIPTOR pfd;
	HDC g_hDC = glWindow->getDisplayContext();
	HGLRC hRCTemp;
	int format;
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;

	format = ChoosePixelFormat(g_hDC, &pfd);
	if (!format || !SetPixelFormat(g_hDC, format, &pfd))
	{
		LOG_ERROR("Setting pixel format fail (%d)\n", GetLastError());
		return false;
	}

	hRCTemp = wglCreateContext(g_hDC);
	if (!hRCTemp || !wglMakeCurrent(g_hDC, hRCTemp))
	{
		LOG_ERROR("Ñreating temp render context fail (%d)\n", GetLastError());
		return false;
	}

	OPENGL_GET_PROC(PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRCTemp);

	g_hRC = wglCreateContextAttribsARB(g_hDC, 0, attribs);
	if (!g_hRC || !wglMakeCurrent(g_hDC, g_hRC))
	{
		LOG_ERROR("Creating render context fail (%d)\n", GetLastError());
		return false;
	}

	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	LOG_DEBUG("OpenGL render context information:\n"
		"  Renderer       : %s\n"
		"  Vendor         : %s\n"
		"  Version        : %s\n"
		"  GLSL version   : %s\n"
		"  OpenGL version : %d.%d\n",
		(const char*)glGetString(GL_RENDERER),
		(const char*)glGetString(GL_VENDOR),
		(const char*)glGetString(GL_VERSION),
		(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION),
		major, minor
		);

	if (!OpenGLInitExtensions())
		return false;
}

bool GLContextDescriptor::isRunning()
{
	return glWindow->isRunning();
}

bool GLContextDescriptor::isActive()
{
	return glWindow->isActive() && glWindow->isRunning();
}

int GLContextDescriptor::getWidth()
{
	return glWindow->getWidth();
}

int GLContextDescriptor::getHeight()
{
	return glWindow->getHeight();
}

HGLRC GLContextDescriptor::getRenderContext()
{
	return g_hRC;
}

HDC GLContextDescriptor::getDisplayContext()
{
	return glWindow->getDisplayContext();
}

GLContextDescriptor::~GLContextDescriptor()
{
	if (g_hRC)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(g_hRC);
		g_hRC = NULL;
	}
}