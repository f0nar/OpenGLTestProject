#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const glm::vec4 &position);
	DirectionalLight(const glm::vec4 &position, 
					 const glm::vec4 &ambient, 
					 const glm::vec4 &diffuse, 
					 const glm::vec4 &specular);
	void draw(const ShaderProgram& program) const override;
	void rotate(float angle, const glm::vec3& v) override;
	void translate(const glm::vec3& v) override;
	void scale(float) override;
	glm::mat4 getModel() const override;
	void set(const ShaderProgram& program) const override;
	~DirectionalLight() override;
};

