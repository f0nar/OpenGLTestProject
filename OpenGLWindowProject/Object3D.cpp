#include <fstream>
#include <sstream>
#include "Loger.h"
#include "Object3D.h"

Vertex::Vertex(const glm::vec3& vert, const glm::vec3& norm, const glm::vec2& tex)
	: vertice(vert), normal(norm), texCoords(tex)
{}

Object3D::Object3D(const std::string& path)
	:Object3D()
{
	load(path);
}

void Object3D::load(const std::string& path)
{
	std::ifstream objFile;

	objFile.exceptions(std::ifstream::badbit);
	try
	{
		objFile.open(path);
		std::stringstream verticeStream;
		verticeStream << objFile.rdbuf();

		std::string head;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;

		while (verticeStream >> head)
		{
			if (head == "v") {
				float x, y, z;
				verticeStream >> x >> y >> z;
				vertices.emplace_back(x, y, z);
			}
			else if (head == "vt")
			{
				float tx, ty;
				verticeStream >> tx >> ty;
				texCoords.emplace_back(tx, ty);
			}
			else if (head == "vn") {
				float nx, ny, nz;
				verticeStream >> nx >> ny >> nz;
				normals.emplace_back(nx, ny, nz);
			}
			else if (head == "f")
			{
				std::string indices[3];
				for (int i = 0; i < 3; ++i) {
					std::getline(verticeStream, indices[0], '/');
					std::getline(verticeStream, indices[1], '/');
					std::getline(verticeStream, indices[2], i == 2 ? '\n' : ' ');
					
					int vi  = std::stoi(indices[0]) - 1;
					int ni  = std::stoi(indices[2]) - 1;
					int tci = std::stoi(indices[1]) - 1;
					addVertex(vertices[vi], normals[vi], texCoords[tci]);
				}
			}
			else if (head == "mtllib")
			{
				std::string localMtlPath;
				verticeStream >> localMtlPath;
				int index = path.find_last_of("/");
				std::string globalPath = path.substr(0, index + 1LL) + localMtlPath;
				m_material.load(globalPath);
			}
			m_indices.resize(m_vertexData.size());
			for (int i = 0; i < m_indices.size(); ++i) m_indices[i] = i;
		}

		objFile.close();
	}
	catch (std::ifstream::failure e)
	{
		LOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}
}

void Object3D::addVertex(const glm::vec3& vert, const glm::vec3& norm, const glm::vec2& tex)
{
	m_vertexData.emplace_back(vert, norm, tex);
}

void Object3D::addIndices(GLuint i1, GLuint i2, GLuint i3)
{
	m_indices.push_back(i1);
	m_indices.push_back(i2);
	m_indices.push_back(i3);
}

void Object3D::deleteData()
{
	std::vector<Vertex>().swap(m_vertexData);
	std::vector<GLuint>().swap(m_indices);
}