#pragma once
#include "ShaderProgram.h"
#include "Light.h"
#include "Sphere.h"
#include "Camera.h"
#include <glm/glm.hpp>

class Mouse;
class GLContextDescriptor;
struct GLWindow;

class GLContext
{
	Mouse* m_mouse;
	Light* m_light;
	Camera m_camera;
	ShaderProgram* m_program;
	glm::mat4 m_projectionMatrix;
	GLContextDescriptor *glContextDescriptor;
	std::vector<SimpleObject3D*> m_drawObjects;

	void render();
	void initialize();
	void update();
	void clear();
	void set();
public:
	GLContext(GLContextDescriptor *_glContextDescriptor);
	void mainLoop();
};

