#include "PointLight.h"
#include "OpenGL.h"
#include <glm/gtc/type_ptr.hpp>

PointLight::PointLight( glm::vec4 position, glm::vec4 ambient,
						glm::vec4 diffuse,  glm::vec4 specular,
						glm::vec3 attenuation, SimpleObject3D* body)
	: m_position(position), m_ambient(ambient),
	m_diffuse(diffuse), m_specular(specular),
	m_attenuation(attenuation), m_body(body)
{}

void PointLight::draw(const ShaderProgram& program) const
{
	set(program);
	m_body->draw(program);
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
	glUniform4fv(glGetUniformLocation(program, "light.position"), 1, glm::value_ptr(m_position));
	glUniform4fv(glGetUniformLocation(program, "light.ambient"), 1, glm::value_ptr(m_ambient));
	glUniform4fv(glGetUniformLocation(program, "light.diffuse"), 1, glm::value_ptr(m_diffuse));
	glUniform4fv(glGetUniformLocation(program, "light.specular"), 1, glm::value_ptr(m_specular));
	glUniform3fv(glGetUniformLocation(program, "light.attenuation"), 1, glm::value_ptr(m_attenuation));
	OPENGL_CHECK_FOR_ERRORS();
}