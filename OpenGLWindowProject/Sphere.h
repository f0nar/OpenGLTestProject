#pragma once
#include "OpenGL.h"
#include "Object3D.h"

class Sphere : public Object3D
{
	GLuint m_vbo;
	GLuint m_vao;
	GLuint m_mbo;
	GLuint m_ebo;
	GLuint m_tbo;
	GLuint m_textureBuffer;
	void initVertexData(float radius, int stackCount, int sectorCount);
	void initIndexData(int stackCount, int sectorCount);
	void initGLData();
	void update();
public:
	Sphere(const Sphere&) = default;
	Sphere(const ShaderProgram &program, const Material &material, float radius = 10.0f, int stackCount = 25, int sectorCount = 50);
	void rotate(float angle, const glm::vec3 &v) override;
	void translate(const glm::vec3 &v) override;
	void scale(float) override;
	void draw(const ShaderProgram & program) const override;
	glm::mat4 getModel() const override;
	~Sphere() override;
};

