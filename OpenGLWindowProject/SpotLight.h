#pragma once
#include "Light.h"

class SpotLight : public Light
{
	glm::vec3 m_attenuation;
	glm::vec3 m_direction;
	float m_exponent;
	float m_cosCutoff;
public:
	SpotLight(const glm::vec4 &position, 
			  const glm::vec3 &direction,
			  const glm::vec3 &attenuation = glm::vec3(1.0f, 0.0f, 0.0f),
			  float exponent = 0.0f, 
			  float cosCutoff = -1.0f);
	SpotLight(const glm::vec4 &position,    const glm::vec4 &ambient, 
			  const glm::vec4 &diffuse,     const glm::vec4 &specular,
			  const glm::vec3 &attenuation, const glm::vec3 &direction, 
			  float exponent, float cosCutoff);
	void setAttenuation(const glm::vec3 &attenuation);
	void setDirection(const glm::vec3 &direction);
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

