#pragma once
#include <vector>
#include "OpenGL.h"
#include "Material.h"
#include <glm/mat4x4.hpp>
#include "Object3D.h"

class AsteroidBelt : public Object3D
{
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;
	GLuint m_tbo;
	GLuint m_textureBuffer;
	std::vector<glm::mat4> m_models;
	void initGLData();
	void initModels(int amount, float radius, float offset);
	void update();
	void deleteData();
public:
	AsteroidBelt(int amount = 100, float radius = 20.0f, float offset = 3.0f);
	void rotate(float angle, const glm::vec3 & v) override;
	void translate(const glm::vec3 & v) override;
	void scale(float) override;
	void draw(const ShaderProgram & program) const override;
	glm::mat4 getModel() const override;
	~AsteroidBelt() override;
};

