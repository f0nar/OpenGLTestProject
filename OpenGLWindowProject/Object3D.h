#pragma once
#include "Simple3DObject.h"

struct Vertex
{
	glm::vec3 vertice;
	glm::vec3 normal;
	glm::vec2 texCoords;
	Vertex(const glm::vec3& vert, const glm::vec3& norm, const glm::vec2& tex);
};

class Object3D : public SimpleObject3D
{
public:
	Object3D() = default;
	Object3D(const std::string& path);
protected:
	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertexData;
	Material m_material;

	void addVertex(const glm::vec3& vert, const glm::vec3& norm, const glm::vec2& tex);
	void addIndices(GLuint i1, GLuint i2, GLuint i3);
	void deleteData();
private:
	void load(const std::string& path);
};