#include "GLContext.h"
#include "OpenGL.h"
#include "GLContextDescriptor.h"
#include <glm/gtc/type_ptr.hpp>

static std::wstring vShaderPath(L"Shaders/vertexs.vs");
static std::wstring fShaderPath(L"Shaders/fragment.fs");

static std::string sphereTexture = "Textures/texture.tga";
static std::string lightPointTexture = "Textures/blank.tga";

static glm::vec3 cameraTarget(0.0f);
static glm::vec3 cameraPos(0.0f, 0.0f, 50.0f);
static glm::vec3 upVector(0.0f, 1.0f, 0.0f);
static glm::vec4 pointLightPos(15.0f, 15.0f, 0.0f, 0.0f);

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

GLContext::GLContext(GLContextDescriptor *_glContextDescriptor) 
	: glContextDescriptor(_glContextDescriptor),
	  m_program(vShaderPath, fShaderPath),
	  m_camera(cameraPos, cameraTarget, upVector)
{
	initialize();
	ASSERT(glContextDescriptor);
}

void GLContext::render()
{
	m_camera.draw(m_program);

	const int drawObjectCount = m_drawObjects.size();
	for (int i = 0; i < drawObjectCount; ++i)
	{
		m_drawObjects[i]->draw(m_program);
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

	lightPointMaterial.loadTexture(m_program, lightPointTexture);
	Sphere* sphere = new Sphere(m_program, lightPointMaterial);
	sphere->translate(pointLightPos);
	sphere->scale(0.2f);
	PointLight* pointLight = new PointLight(pointLightPos,
		glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.0f, 0.02f), sphere);
	m_drawObjects.push_back(pointLight);

	sphereMaterial.loadTexture(m_program, sphereTexture);
	m_drawObjects.push_back(new Sphere(m_program, sphereMaterial));

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
	m_program.use();

	glUniformMatrix4fv(glGetUniformLocation(m_program, "transform.projection"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
}

void GLContext::update()
{

}

void GLContext::mainLoop()
{
	while (glContextDescriptor->isRunning())
	{
		clear();
		set();
		render();
	}
}