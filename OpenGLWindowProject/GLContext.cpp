#define _USE_MATH_DEFINES
#include <math.h>
#include "Mouse.h"
#include "OpenGL.h"
#include "GLContext.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "GLContextDescriptor.h"
#include <glm/gtc/type_ptr.hpp>

static std::wstring spotLightVertexShaderPath(L"Shaders/spotLight.vs");
static std::wstring spotLightFragmentShaderPath(L"Shaders/spotLight.fs");
static ShaderProgram* spotLightProgram;

static std::wstring pointLightVertexShaderPath(L"Shaders/pointLight.vs");
static std::wstring pointLightFragmentShaderPath(L"Shaders/pointLight.fs");
static ShaderProgram* pointLightProgram;

static std::wstring directionalLigthVertexShaderPath(L"Shaders/directionalLight.vs");
static std::wstring directionalLigthFragmentShaderPath(L"Shaders/directionalLight.fs");
static ShaderProgram* directionalLightProgram;

static std::string sphereTexture = "Textures/texture.tga";
static std::string lightPointTexture = "Textures/blank.tga";

static glm::vec3 cameraTarget(0.0f);
static glm::vec3 cameraPos(0.0f, 0.0f, 30.0f);
static glm::vec3 upVector(0.0f, 1.0f, 0.0f);

static glm::vec4 pointLightPos(-5.0f, 5.0f, 10.0f, 1.0f);
static glm::vec4 spotLightPos(0.0f, 0.0f, 10.0f, 1.0f);
static glm::vec3 spotLightDirection(-1.0f, -2.0f, -15.0f);

static Material sphereMaterial(
	glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
	glm::vec4(0.8f, 1.0f, 1.0f, 1.0f),
	glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
	glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
	20.0f);
static Material lightPointMaterial(
	glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
	glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
	glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
	glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
	0.0f);

static PointLight pointLight(pointLightPos);
static DirectionalLight directionalLight(pointLightPos);
static SpotLight spotLight(spotLightPos, spotLightDirection);

GLContext::GLContext(GLContextDescriptor *_glContextDescriptor) 
	: glContextDescriptor(_glContextDescriptor),
	  m_camera(cameraPos, cameraTarget, upVector)
{
	initialize();
	ASSERT(glContextDescriptor);
}

void GLContext::render()
{
	m_camera.draw(*m_program);

	const int drawObjectCount = m_drawObjects.size();
	for (int i = 0; i < drawObjectCount; ++i)
	{
		m_drawObjects[i]->draw(*m_program);
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

	//pointlight
	pointLightProgram = new ShaderProgram(pointLightVertexShaderPath, pointLightFragmentShaderPath);
	lightPointMaterial.loadTexture(*m_program, lightPointTexture);
	Sphere* sphere = new Sphere(*m_program, lightPointMaterial);
	sphere->translate(pointLightPos);
	sphere->scale(0.1f);
	pointLight.setBody(sphere);
	pointLight.setAmbient(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	pointLight.setAttenuation(glm::vec3(0.5f, 0.0f, 0.02f));

	//directional light
	directionalLightProgram = new ShaderProgram(directionalLigthVertexShaderPath, directionalLigthFragmentShaderPath);
	glm::vec4 directionalLightPos = pointLightPos;
	directionalLightPos.w = 0;
	directionalLight.setPosition(directionalLightPos);
	
	//spotLight
	spotLightProgram = new ShaderProgram(spotLightVertexShaderPath, spotLightFragmentShaderPath);
	spotLight.setAmbient(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	spotLight.setAttenuation(glm::vec3(0.5f, 0.0f, 0.02f));
	spotLight.setCosCutoff(cosf(45.0f * (M_PI / 180.0f)));
	spotLight.setExponent(5.0f);

	//choose light type
	m_light = &spotLight;
	m_program = spotLightProgram;

	sphereMaterial.loadTexture(*m_program, sphereTexture);
	sphere = new Sphere(*m_program, sphereMaterial);
	sphere->scale(0.6);
	m_drawObjects.push_back(sphere);

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
	m_program->use();
	m_light->set(*m_program);

	glUniformMatrix4fv(glGetUniformLocation(*m_program, "transform.projection"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
}

void GLContext::update()
{

	static ScreenPoint prevPos = m_mouse->getCurrentPos();
	if (!m_mouse) return;
	if (m_mouse->isLButtonPressed()) {
		ScreenPoint currPos = m_mouse->getCurrentPos();

		glm::vec3 rotate = glm::vec3(-(currPos.x - prevPos.x), currPos.y - prevPos.y, 0.0f);
		m_camera.rotate(0.0f, rotate);

		prevPos = currPos;
	}

	if (m_mouse->isRButtonPressed())
	{
		m_light = &pointLight;
		m_program = pointLightProgram;
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