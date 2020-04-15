#define _USE_MATH_DEFINES
#include <math.h>
#include "Mouse.h"
#include "OpenGL.h"
#include "Sphere.h"
#include "GLContext.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "AsteroidBelt.h"
#include "DirectionalLight.h"
#include "GLContextDescriptor.h"
#include <glm/gtc/type_ptr.hpp>


static std::wstring vShaderPath[Light_t::Count] = { L"Shaders/pointLight.vs", L"Shaders/spotLight.vs", L"Shaders/directionalLight.vs" };
static std::wstring fShaderPath[Light_t::Count] = { L"Shaders/pointLight.fs", L"Shaders/spotLight.fs", L"Shaders/directionalLight.fs" };

static std::string sphereTexture = "Textures/texture.tga";
static std::string planetTexture = "Textures/planet.tga";
static std::string lightPointTexture = "Textures/blank.tga";

static glm::vec3 cameraTarget(0.0f);
static glm::vec3 cameraPos(0.0f, 0.0f, 30.0f);
static glm::vec3 upVector(0.0f, 1.0f, 0.0f);

static glm::vec4 pointLightPos(-5.0f, 5.0f, 10.0f, 1.0f);
static glm::vec4 spotLightPos(0.0f, 0.0f, 10.0f, 1.0f);
static glm::vec3 spotLightDirection(-1.0f, -2.0f, -15.0f);

GLContext::GLContext(GLContextDescriptor *_glContextDescriptor) 
	: glContextDescriptor(_glContextDescriptor),
	  m_camera(cameraPos, cameraTarget, upVector),
	  m_shooterMode(false)
{
	initialize();
	ASSERT(glContextDescriptor);
}

void GLContext::render()
{
	m_camera.draw(m_program[m_currLight]);

	const int drawObjectCount = static_cast<int>(m_drawObjects.size());
	for (int i = 0; i < drawObjectCount; ++i)
	{
		m_drawObjects[i]->draw(m_program[m_currLight]);
	}

	SwapBuffers(glContextDescriptor->getDisplayContext());
	OPENGL_CHECK_FOR_ERRORS();
}

void GLContext::initialize()
{
	glViewport(0, 0, glContextDescriptor->getWidth(), glContextDescriptor->getHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	m_mouse = GetMouse();

	if (m_shooterMode) {
		setCursorToCenter();
		m_mouse->setVisible(false);
	}

	//pointlight
	m_program[Light_t::Point].load(vShaderPath[Light_t::Point], fShaderPath[Light_t::Point]);
	Material lightPointMaterial(lightPointTexture,
		glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);
	Sphere* sphere = new Sphere(m_program[Light_t::Point], lightPointMaterial);
	sphere->translate(pointLightPos);
	sphere->scale(0.1f);
	PointLight* pointLight = new PointLight(pointLightPos, glm::vec3(0.5f, 0.0f, 0.02f), sphere);
	pointLight->setAmbient(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	m_light[Light_t::Point] = pointLight;


	//directional light
	m_program[Light_t::Directional].load(vShaderPath[Light_t::Directional], fShaderPath[Light_t::Directional]);
	glm::vec4 directionalLightPos = pointLightPos; directionalLightPos.w = 0;
	m_light[Light_t::Directional] = new DirectionalLight(directionalLightPos);
	
	//spotLight
	m_program[Light_t::Spot].load(vShaderPath[Light_t::Spot], fShaderPath[Light_t::Spot]);
	SpotLight* spotLight = new SpotLight(spotLightPos, spotLightDirection, glm::vec3(0.5f, 0.0f, 0.02f), 5.0f, cosf(45.0f * (M_PI / 180.0f)));
	spotLight->setAmbient(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	m_light[Light_t::Spot] = spotLight;

	m_currLight = Light_t::Spot;

	/*
	Material sphereMaterial(planetTexture,
		glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 20.0f);
	sphere = new Sphere(m_program[m_currLight], sphereMaterial);
	sphere->scale(0.6);
	m_drawObjects.push_back(sphere);
	*/

	AsteroidBelt* asteroid = new AsteroidBelt();
	m_drawObjects.push_back(asteroid);

	m_projectionMatrix = glm::perspective(45.0f, (float)glContextDescriptor->getWidth() / glContextDescriptor->getHeight(), 0.1f, 100.0f);

	OPENGL_CHECK_FOR_ERRORS();
}

void GLContext::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GLContext::set()
{
	m_program[m_currLight].use();
	m_light[m_currLight]->set(m_program[m_currLight]);

	glUniformMatrix4fv(glGetUniformLocation(m_program[m_currLight], "transform.projection"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
}

void GLContext::setCursorToCenter()
{
	int width  = glContextDescriptor->getWidth() / 2;
	int height = glContextDescriptor->getHeight() / 2;
	m_mouse->setTo(width, height, glContextDescriptor->getWindowDescriptor());
}

void GLContext::update()
{
	if (!m_mouse) return;

	
	if (m_shooterMode || m_mouse->isLButtonPressed()) {

		ScreenPoint currPos = m_mouse->getCurrentPos();
		ScreenPoint prevPos = m_mouse->getPrevPos();

		glm::vec3 rotate = glm::vec3(-(currPos.x - prevPos.x), currPos.y - prevPos.y, 0.0f);
		m_camera.rotate(0.0f, rotate);
	}

	if (m_mouse->isRButtonPressed())
	{
		switch (m_currLight)
		{
			case Light_t::Point:       { m_currLight = Light_t::Spot; break; }
			case Light_t::Spot:        { m_currLight = Light_t::Directional; break; }
			case Light_t::Directional: { m_currLight = Light_t::Point; break; }
		}
	}
	if (m_shooterMode) {
		setCursorToCenter();
		m_mouse->setVisible(false);
	}
}

void GLContext::mainLoop()
{
	while (glContextDescriptor->isRunning())
	{
		clear();
		update();
		set();
		render();
	}
}

GLContext::~GLContext()
{
	for (int i = 0; i < m_drawObjects.size(); ++i)
		delete m_drawObjects[i];
}