#pragma once
#include "OpenGL.h"
#include "Simple3DObject.h"
#include <vector>
#include "Material.h"

class Sphere : public SimpleObject3D
{
	GLuint vbo;
	GLuint vao;
	GLuint ebo;
	//GLuint texture;
	Material material;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<GLuint> indices;
	void initVertexData(float radius, int stackCount, int sectorCount);
	void initIndexData(int stackCount, int sectorCount);
	void initGLData(const ShaderProgram &program);
	void updateModel();
	void addVertices(float x, float y, float z);
	void addNormals(float x, float y, float z);
	void addTexCoord(float x, float y);
	void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
	void deleteData();
public:
	Sphere(const Sphere&) = default;
	Sphere(const ShaderProgram &program, const Material &material, float radius = 10.f, int stackCount = 25, int sectorCount = 50);
	void rotate(float angle, const glm::vec3 &v) override;
	void translate(const glm::vec3 &v) override;
	void scale(float) override;
	void draw(const ShaderProgram & program) const override;
	glm::mat4 getModel() const override;
	~Sphere() override;
};

