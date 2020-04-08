#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec4 &position)
	:Light(position)
{} 

DirectionalLight::DirectionalLight(const glm::vec4 &position, const glm::vec4 &ambient, const glm::vec4 &diffuse, const glm::vec4 &specular)
	: Light(position, ambient, diffuse, specular)
{}

void DirectionalLight::draw(const ShaderProgram& program) const
{
	set(program);
}

void DirectionalLight::rotate(float angle, const glm::vec3& r)
{

}

void DirectionalLight::translate(const glm::vec3& t)
{

}

void DirectionalLight::scale(float s)
{

}

glm::mat4 DirectionalLight::getModel() const
{
	return m_model;
}

void DirectionalLight::set(const ShaderProgram& program) const
{
	Light::set(program);
	OPENGL_CHECK_FOR_ERRORS();
}

DirectionalLight::~DirectionalLight() {}