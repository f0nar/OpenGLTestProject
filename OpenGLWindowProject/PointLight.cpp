#include "PointLight.h"
#include "OpenGL.h"
#include <glm/gtc/type_ptr.hpp>

PointLight::PointLight(const glm::vec4 &position, const glm::vec3 &attenuation, SimpleObject3D *body)
	: Light(position),
	  m_attenuation(attenuation), 
	  m_body(body)
{}

PointLight::PointLight(const glm::vec4 &position, const glm::vec4 &ambient,
					   const glm::vec4 &diffuse,  const glm::vec4 &specular,
					   const glm::vec3 &attenuation, SimpleObject3D* body)
	: Light(position, ambient, diffuse, specular), 
	  m_attenuation(attenuation), m_body(body)
{}

void PointLight::setAttenuation(const glm::vec3 &attenuation)
{
	m_attenuation = attenuation;
}

void PointLight::setBody(SimpleObject3D* body)
{
	m_body = body;
}

void PointLight::draw(const ShaderProgram& program) const
{
	if(m_body) m_body->draw(program);
}

void PointLight::rotate(float angle, const glm::vec3& r)
{
	m_body->rotate(angle, r);
}

void PointLight::translate(const glm::vec3& t)
{
	m_body->translate(t);
}

void PointLight::scale(float s)
{
	m_body->scale(s);
}

glm::mat4 PointLight::getModel() const
{
	return m_body->getModel();
}

void PointLight::set(const ShaderProgram& program) const
{
	Light::set(program);
	glUniform3fv(glGetUniformLocation(program, "light.attenuation"), 1, glm::value_ptr(m_attenuation));
	draw(program);
	OPENGL_CHECK_FOR_ERRORS();
}

PointLight::~PointLight() {}