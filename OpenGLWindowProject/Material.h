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
	Material(const std::string& path);
	Material(const glm::vec4 &ambient  = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
			 const glm::vec4 &diffuse  = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
			 const glm::vec4 &specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			 const glm::vec4 &emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			 float shininess		   = 0.0f);
	Material(const std::string &texturePath,
		     const glm::vec4& ambient,  const glm::vec4& diffuse,
		     const glm::vec4& specular, const glm::vec4& emission,
			 float shininess);

	void set(const ShaderProgram& program) const;
	void load(const std::string& path);
	void loadTexture(const std::string &texturePath);
};

