#pragma once
#include "ShaderProgram.h"
#include "PointLight.h"
#include "Sphere.h"
#include "Camera.h"
#include <glm/glm.hpp>

class GLContextDescriptor;
struct GLWindow;

class GLContext
{
	GLContextDescriptor *glContextDescriptor;
	ShaderProgram m_program;
	Camera m_camera;
	std::vector<SimpleObject3D*> m_drawObjects;
	glm::mat4 m_projectionMatrix;
	void render();
	void initialize();
	void update();
	void clear();
	void set();
public:
	GLContext(GLContextDescriptor *_glContextDescriptor);
	void mainLoop();
};

