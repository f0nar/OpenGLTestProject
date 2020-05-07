#pragma once
#include "Material.h"
#include "Simple3DObject.h"

class SmartSphere : public SimpleObject3D
{
public:
	SmartSphere(const glm::vec3 &pos, float radius, const Material &material);
	void rotate(float angle, const glm::vec3& v) override;
	void translate(const glm::vec3& v) override;
	void scale(float) override;
	void draw(const ShaderProgram& program) const override;
	glm::mat4 getModel() const override;
	~SmartSphere() override;
private:
	GLuint m_vao;
	GLuint m_vbo;
	Material m_material;
	glm::mat3 m_normal;
	void initGL();
	void update();
};

