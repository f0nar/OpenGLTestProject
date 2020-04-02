#pragma once
#include "Simple3DObject.h"
#include <glm/gtc/type_ptr.hpp>

class Light : public SimpleObject3D
{
protected:
	glm::vec4 m_position;
	glm::vec4 m_ambient;
	glm::vec4 m_diffuse;
	glm::vec4 m_specular;
public:
	Light(glm::vec4 position,
		  glm::vec4 ambient  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 
		  glm::vec4 diffuse  = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
		  glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void setAmbient(glm::vec4 ambient);
	void setDiffuse(glm::vec4 diffuse);
	void setSpecular(glm::vec4 specular);
	void setPosition(glm::vec4 position);
	virtual void set(const ShaderProgram& program) const;
	virtual ~Light() = 0;
};

inline Light::Light(glm::vec4 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
	: m_position(position), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular) {}

inline void Light::setAmbient(glm::vec4 ambient)
{
	m_ambient = ambient;
}

inline void Light::setDiffuse(glm::vec4 diffuse)
{
	m_diffuse = diffuse;
}

inline void Light::setSpecular(glm::vec4 specular)
{
	m_specular = specular;
}

inline void Light::setPosition(glm::vec4 position)
{
	m_position = position;
}

inline void Light::set(const ShaderProgram& program) const
{
	glUniform4fv(glGetUniformLocation(program, "light.position"), 1, glm::value_ptr(m_position));
	glUniform4fv(glGetUniformLocation(program, "light.ambient"), 1, glm::value_ptr(m_ambient));
	glUniform4fv(glGetUniformLocation(program, "light.diffuse"), 1, glm::value_ptr(m_diffuse));
	glUniform4fv(glGetUniformLocation(program, "light.specular"), 1, glm::value_ptr(m_specular));
}

inline Light::~Light() {}