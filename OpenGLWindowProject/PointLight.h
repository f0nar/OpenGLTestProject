#pragma once
#include "Light.h"

class PointLight : public Light
{
	glm::vec3 m_attenuation;
	SimpleObject3D* m_body;

public:
	PointLight(glm::vec4 position, 
		       glm::vec3 attenuation = glm::vec3(1.0f, 1.0f, 0.0f), 
		       SimpleObject3D *body = nullptr);
	PointLight(glm::vec4 position, glm::vec4 ambient,
			   glm::vec4 diffuse,  glm::vec4 specular,
		       glm::vec3 attenuation, SimpleObject3D* body);
	void setAttenuation(glm::vec3 attenuation);
	void setBody(SimpleObject3D* body);
	void draw(const ShaderProgram& program) const override;
	void rotate(float angle, const glm::vec3& v) override;
	void translate(const glm::vec3& v) override;
	void scale(float) override;
	glm::mat4 getModel() const override;
	void set(const ShaderProgram& program) const override;
	~PointLight() override;
};

