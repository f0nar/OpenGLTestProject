#include "Material.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include <glm/gtc/type_ptr.hpp>

Material::Material(glm::vec4  ambient,  glm::vec4  diffuse,
				   glm::vec4  specular, glm::vec4  emission,
				   float shininess)
	: m_ambient(ambient),   m_diffuse(diffuse),
	  m_specular(specular), m_emission(emission),
	  m_shininess(shininess), texture(0)
{}

Material::Material(const ShaderProgram& program, std::string texturePath,
				   glm::vec4  ambient,  glm::vec4  diffuse,
				   glm::vec4  specular, glm::vec4  emission,
				   float shininess)
	: Material(ambient, diffuse, specular, emission, shininess)
{
	loadTexture(program, texturePath);
}

void Material::set(const ShaderProgram& program) const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(glGetUniformLocation(program, "material.texture"), 0);
	glUniform4fv(glGetUniformLocation(program, "material.ambient"),   1, glm::value_ptr(m_ambient));
	glUniform4fv(glGetUniformLocation(program, "material.diffuse"),   1, glm::value_ptr(m_diffuse));
	glUniform4fv(glGetUniformLocation(program, "material.specular"),  1, glm::value_ptr(m_specular));
	glUniform4fv(glGetUniformLocation(program, "material.emission"),  1, glm::value_ptr(m_emission));
	glUniform1fv(glGetUniformLocation(program, "material.shininess"), 1, &m_shininess);
}

void Material::loadTexture(const ShaderProgram& program, std::string texturePath)
{
	texture = TextureCreateFromTGA(texturePath.c_str());
}