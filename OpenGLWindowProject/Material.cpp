#include <fstream>
#include <sstream>
#include "Texture.h"
#include "Material.h"
#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>

Material::Material(const std::string& path) 
	: Material()
{
	load(path);
}

Material::Material(const glm::vec4 &ambient,  const glm::vec4 &diffuse,
				   const glm::vec4 &specular, const glm::vec4 &emission,
				   float shininess)
	: m_ambient(ambient),   m_diffuse(diffuse),
	  m_specular(specular), m_emission(emission),
	  m_shininess(shininess), texture(0)
{}

Material::Material(const std::string &texturePath,
				   const glm::vec4 &ambient,  const glm::vec4 &diffuse,
				   const glm::vec4 &specular, const glm::vec4 &emission,
				   float shininess)
	: Material(ambient, diffuse, specular, emission, shininess)
{
	loadTexture(texturePath);
}

void Material::set(const ShaderProgram& program) const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(glGetUniformLocation(program,  "material.texture"),   0);
	glUniform4fv(glGetUniformLocation(program, "material.ambient"),   1, glm::value_ptr(m_ambient));
	glUniform4fv(glGetUniformLocation(program, "material.diffuse"),   1, glm::value_ptr(m_diffuse));
	glUniform4fv(glGetUniformLocation(program, "material.specular"),  1, glm::value_ptr(m_specular));
	glUniform4fv(glGetUniformLocation(program, "material.emission"),  1, glm::value_ptr(m_emission));
	glUniform1fv(glGetUniformLocation(program, "material.shininess"), 1, &m_shininess);
}

void Material::loadTexture(const std::string &texturePath)
{
	texture = TextureCreateFromTGA(texturePath.c_str());
}

void Material::load(const std::string& path)
{
	std::ifstream objFile;

	objFile.exceptions(std::ifstream::badbit);
	try
	{
		objFile.open(path);

		std::stringstream verticeStream;
		verticeStream << objFile.rdbuf();

		std::string head;

		while (verticeStream >> head)
		{
			if (head == "Ka") {
				verticeStream >> m_ambient.x >> m_ambient.y >> m_ambient.z;
			}
			else if (head == "Kd") {
				verticeStream >> m_diffuse.x >> m_diffuse.y >> m_diffuse.z;
			}
			else if (head == "Ks") {
				verticeStream >> m_specular.x >> m_specular.y >> m_specular.z;
			}
			else if (head == "Ke") {
				verticeStream >> m_emission.x >> m_emission.y >> m_emission.z;
			}
			else if (head == "d") {
				float d;
				verticeStream >> d;
				m_ambient.z = m_diffuse.z = m_specular.z = m_emission.z = d;
			}
			else if (head == "Ns") {
				verticeStream >> m_shininess;
			}
			else if (head == "map_Bump")
			{
				std::string texturePath;
				verticeStream >> texturePath;
				auto index = path.find_last_of("/");
				std::string globalPath = path.substr(0, index + 1LL) + texturePath;
				loadTexture(globalPath);
			}
		}

		objFile.close();
	}
	catch (std::ifstream::failure e)
	{
		LOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}
}