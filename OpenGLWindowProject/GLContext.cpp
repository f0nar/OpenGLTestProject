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


static std::wstring vShaderPath[Light_t::Count] = { L"Shaders/Instanced/pointLight.vs", L"Shaders/Instanced/spotLight.vs", L"Shaders/Instanced/directionalLight.vs" };
static std::wstring fShaderPath[Light_t::Count] = { L"Shaders/Instanced/pointLight.fs", L"Shaders/Instanced/spotLight.fs", L"Shaders/Instanced/directionalLight.fs" };

static std::string sphereTexture = "Textures/texture.tga";
static std::string planetTexture = "Textures/planet.tga";
static std::string lightPointTexture = "Textures/blank.tga";

static glm::vec3 cameraTarget(0.0f);
static glm::vec3 cameraPos(0.0, 0.0, 30.0);
static glm::vec3 upGlobalVector(0.0f, 1.0f, 0.0f);

static glm::vec4 pointLightPos(-5.0f, 5.0f, 10.0f, 1.0f);
static glm::vec4 spotLightPos(0.0f, 0.0f, 10.0f, 1.0f);
static glm::vec3 spotLightDirection(-1.0f, -2.0f, -15.0f);

GLContext::GLContext(GLContextDescriptor *_glContextDescriptor) 
	: glContextDescriptor(_glContextDescriptor),
	  m_camera(cameraPos, cameraTarget, upGlobalVector),
	  m_shooterMode(false)
{
	initialize();
	ASSERT(glContextDescriptor);
}

void GLContext::render()
{
	set(m_program[m_currLight]);
	m_camera.draw(m_program[m_currLight]);

	m_camera.draw(m_tesProg);
	const int drawObjectCount = static_cast<int>(m_drawObjects.size());
	for (int i = 0; i < drawObjectCount; ++i)
	{
		m_drawObjects[i]->draw(m_program[m_currLight]);
	}

	set(m_tesProg);
	m_camera.draw(m_tesProg);
	m_smrtSphere->draw(m_tesProg);

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

	m_tesProg.attach(L"Shaders/Tesselation/smartSphere.vs", GL_VERTEX_SHADER);
	m_tesProg.attach(L"Shaders/Tesselation/smartSphere.fs", GL_FRAGMENT_SHADER);
	m_tesProg.attach(L"Shaders/Tesselation/smartSphere.tcs", GL_TESS_CONTROL_SHADER);
	m_tesProg.attach(L"Shaders/Tesselation/smartSphere.tes", GL_TESS_EVALUATION_SHADER);
	m_tesProg.link();

	//pointlight
	m_program[Light_t::Point].attach(vShaderPath[Light_t::Point], GL_VERTEX_SHADER);
	m_program[Light_t::Point].attach(fShaderPath[Light_t::Point], GL_FRAGMENT_SHADER);
	m_program[Light_t::Point].link();
	Material lightPointMaterial(lightPointTexture,
		glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);
	//point Light body
	Sphere* sphere = new Sphere(m_program[Light_t::Point], lightPointMaterial);
	sphere->translate(pointLightPos);
	sphere->scale(0.1f);
	PointLight* pointLight = new PointLight(pointLightPos, glm::vec3(0.5f, 0.0f, 0.02f), sphere);
	pointLight->setAmbient(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	m_light[Light_t::Point] = pointLight;

	//directional light
	m_program[Light_t::Directional].attach(vShaderPath[Light_t::Directional], GL_VERTEX_SHADER);
	m_program[Light_t::Directional].attach(fShaderPath[Light_t::Directional], GL_FRAGMENT_SHADER);
	m_program[Light_t::Directional].link();
	glm::vec4 directionalLightPos = pointLightPos; directionalLightPos.w = 0;
	m_light[Light_t::Directional] = new DirectionalLight(directionalLightPos);

	//spot light
	m_program[Light_t::Spot].attach(vShaderPath[Light_t::Spot], GL_VERTEX_SHADER);
	m_program[Light_t::Spot].attach(fShaderPath[Light_t::Spot], GL_FRAGMENT_SHADER);
	m_program[Light_t::Spot].link();
	SpotLight* spotLight = new SpotLight(spotLightPos, spotLightDirection, glm::vec3(0.5f, 0.0f, 0.02f), 5.0f, cosf(45.0f * (M_PI_F / 180.0f)));
	spotLight->setAmbient(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	m_light[Light_t::Spot] = spotLight;

	m_currLight = Light_t::Directional;


	//Main sphere
	Material sphereMaterial(planetTexture,
		glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.8f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 20.0f);
	sphere = new Sphere(m_program[m_currLight], sphereMaterial);
	sphere->scale(0.6f);
	m_drawObjects.push_back(sphere);

	//Tessellated sphere
	m_smrtSphere = new SmartSphere(glm::vec3(0.0), 10.0f, sphereMaterial);

	//Asteroid belt
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

void GLContext::set(const ShaderProgram& program)
{
	program.use();
	m_light[m_currLight]->set(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "transform.projection"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
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
		render();
	}
}

GLContext::~GLContext()
{
	for (int i = 0; i < m_drawObjects.size(); ++i)
		delete m_drawObjects[i];
}