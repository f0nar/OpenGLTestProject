#include "SpotLight.h"
#include <glm/gtc/type_ptr.hpp>

SpotLight::SpotLight(const glm::vec4 &position, const glm::vec3 &direction, const glm::vec3 &attenuation,
			float exponent, float cosCutoff)
	: Light(position),
	  m_direction(direction), m_attenuation(attenuation),
	  m_exponent(exponent), m_cosCutoff(cosCutoff)
{}

SpotLight::SpotLight(const glm::vec4 &position,    const glm::vec4 &ambient, 
					 const glm::vec4 &diffuse,	   const glm::vec4 &specular,
					 const glm::vec3 &attenuation, const glm::vec3 &direction, 
					 float exponent, float cosCutoff)
	: Light(position, ambient, diffuse, specular),
	m_attenuation(attenuation), m_direction(direction), m_exponent(exponent), m_cosCutoff(cosCutoff)
{}

void SpotLight::setAttenuation(const glm::vec3 &attenuation)
{
	m_attenuation = attenuation;
}

void SpotLight::setDirection(const glm::vec3 &direction)
{
	m_direction = direction;
}

void SpotLight::setExponent(float exponent)
{
	m_exponent = exponent;
}

void SpotLight::setCosCutoff(float cosCutoff)
{
	m_cosCutoff = cosCutoff;
}

void SpotLight::draw(const ShaderProgram& program) const
{
	set(program);
}

void SpotLight::rotate(float angle, const glm::vec3& r)
{
	
}

void SpotLight::translate(const glm::vec3& t)
{
	
}

void SpotLight::scale(float s)
{

}

glm::mat4 SpotLight::getModel() const
{
	return m_model;
}

void SpotLight::set(const ShaderProgram& program) const
{
	Light::set(program);
	glUniform3fv(glGetUniformLocation(program, "light.attenuation"), 1, glm::value_ptr(m_attenuation));
	glUniform3fv(glGetUniformLocation(program, "light.spotDirection"), 1, glm::value_ptr(m_direction));
	glUniform1fv(glGetUniformLocation(program, "light.spotExponent"), 1, &m_exponent);
	glUniform1fv(glGetUniformLocation(program, "light.spotCosCutoff"), 1, &m_cosCutoff);
	OPENGL_CHECK_FOR_ERRORS();
}

SpotLight::~SpotLight() {}