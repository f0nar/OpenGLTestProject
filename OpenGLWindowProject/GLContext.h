#pragma once
#include "Camera.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include <vector>

class Mouse;
class Light;
class GLContextDescriptor;
enum Light_t { Point, Spot, Directional, Count };

class GLContext
{
	Mouse* m_mouse;
	Camera m_camera;
	bool m_shooterMode;
	Light_t m_currLight;
	Light* m_light[Light_t::Count];
	ShaderProgram m_program[Light_t::Count];
	glm::mat4 m_projectionMatrix;
	GLContextDescriptor *glContextDescriptor;
	std::vector<SimpleObject3D*> m_drawObjects;

	void setCursorToCenter();
	void render();
	void initialize();
	void update();
	void clear();
	void set();
public:
	GLContext(GLContextDescriptor *_glContextDescriptor);
	void mainLoop();
};

