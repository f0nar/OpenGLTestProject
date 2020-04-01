#pragma once
#include "Simple3DObject.h"

class PointLight : public SimpleObject3D
{
	glm::vec4 m_position;
	glm::vec4 m_ambient;
	glm::vec4 m_diffuse;
	glm::vec4 m_specular;
	glm::vec3 m_attenuation;
	SimpleObject3D* m_body;

public:
	PointLight(glm::vec4 position, glm::vec4 ambient,
			   glm::vec4 diffuse,  glm::vec4 specular,
		       glm::vec3 attenuation, SimpleObject3D* body);
	void draw(const ShaderProgram& program) const override;
	void rotate(float angle, const glm::vec3& v) override;
	void translate(const glm::vec3& v) override;
	void scale(float) override;
	glm::mat4 getModel() const override;
	void set(const ShaderProgram& program) const;
};

