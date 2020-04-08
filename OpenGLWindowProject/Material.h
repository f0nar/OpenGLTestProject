#pragma once
#pragma once
#include "OpenGL.h"
#include <glm/glm.hpp>
#include <string>

class ShaderProgram;

class Material
{
	GLuint texture;

	glm::vec4  m_ambient;
	glm::vec4  m_diffuse;
	glm::vec4  m_specular;
	glm::vec4  m_emission;
	float	   m_shininess;

public:
	Material(const glm::vec4 &ambient,  const glm::vec4 &diffuse,
			 const glm::vec4 &specular, const glm::vec4 &emission,
			 float shininess);
	Material(const ShaderProgram& program, std::string texturePath,
		     const glm::vec4& ambient, const glm::vec4& diffuse,
		     const glm::vec4& specular, const glm::vec4& emission,
			 float shininess);

	void set(const ShaderProgram& program) const;
	void loadTexture(const ShaderProgram& program, std::string texturePath);
};

