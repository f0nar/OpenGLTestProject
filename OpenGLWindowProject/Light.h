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
	Light(const glm::vec4 &position,
		const glm::vec4 &ambient  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		const glm::vec4 &diffuse  = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		const glm::vec4 &specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void setAmbient(const glm::vec4 &ambient);
	void setDiffuse(const glm::vec4 &diffuse);
	void setSpecular(const glm::vec4 &specular);
	void setPosition(const glm::vec4 &position);
	virtual void set(const ShaderProgram& program) const;
	virtual ~Light() = 0;
};

inline Light::Light(const glm::vec4 &position, const glm::vec4 &ambient, const glm::vec4 &diffuse, const glm::vec4 &specular)
	: m_position(position), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular) {}

inline void Light::setAmbient(const glm::vec4 &ambient)
{
	m_ambient = ambient;
}

inline void Light::setDiffuse(const glm::vec4 &diffuse)
{
	m_diffuse = diffuse;
}

inline void Light::setSpecular(const glm::vec4 &specular)
{
	m_specular = specular;
}

inline void Light::setPosition(const glm::vec4 &position)
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