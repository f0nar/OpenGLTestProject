#pragma once
#include "Simple3DObject.h"

class Camera : public SimpleObject3D
{
	float m_yaw;
	float m_pitch;

	glm::vec3 m_position;
	glm::vec3 m_targetPos;
	glm::vec3 m_upVector;
	glm::vec3 m_rightVector;

	void updateModel();
public:
	void rotate(float angle, const glm::vec3 &r) override;
	void translate(const glm::vec3& t) override;
	void scale(float) override;
	void draw(const ShaderProgram& program) const override;
	glm::mat4 getModel() const override;
	Camera(glm::vec3 pos   = glm::vec3(0.0f, 0.0f, 100.0f),
		   glm::vec3 front = glm::vec3(0.0f), 
		   glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f));
};
