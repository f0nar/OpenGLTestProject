#pragma once
#include "Light.h"

class SpotLight : public Light
{
	glm::vec3 m_attenuation;
	glm::vec3 m_direction;
	float m_exponent;
	float m_cosCutoff;
public:
	SpotLight(glm::vec4 position, 
			  glm::vec3 direction,
		      glm::vec3 attenuation = glm::vec3(1.0f, 0.0f, 0.0f), 
			  float exponent = 0.0f, 
			  float cosCutoff = -1.0f);
	SpotLight(glm::vec4 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular,
			  glm::vec3 attenuation, glm::vec3 direction, float exponent, float cosCutoff);
	void setAttenuation(glm::vec3 attenuation);
	void setDirection(glm::vec3 direction);
	void setExponent(float exponent);
	void setCosCutoff(float cosCutoff);
	void draw(const ShaderProgram& program) const override;
	void rotate(float angle, const glm::vec3& v) override;
	void translate(const glm::vec3& v) override;
	void scale(float) override;
	glm::mat4 getModel() const override;
	void set(const ShaderProgram& program) const override;
	~SpotLight() override;
};

