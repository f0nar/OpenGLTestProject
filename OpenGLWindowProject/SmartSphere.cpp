#include "SmartSphere.h"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>

SmartSphere::SmartSphere(const glm::vec3& pos, float radius, const Material& material)
	: m_material(material)
{
	m_scale = radius;
	m_translate = pos;
	update();
	initGL();
}

void SmartSphere::initGL()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glm::vec4 sphrData(m_translate, m_scale);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphrData), glm::value_ptr(sphrData), GL_STATIC_DRAW);

	int stride = sizeof(sphrData), offset = 0;
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride, (void*)offset);

	glEnableVertexAttribArray(0);

	glPatchParameteri(GL_PATCH_VERTICES, 1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SmartSphere::rotate(float angle, const glm::vec3& v)
{
	m_rotate = glm::rotate(m_rotate, angle, v);
	update();
}

void SmartSphere::translate(const glm::vec3& v)
{
	m_translate += v;
	update();
}

void SmartSphere::scale(float s)
{
	m_scale *= s;
	update();
}

void SmartSphere::draw(const ShaderProgram& program) const
{
	glBindVertexArray(m_vao);

	m_material.set(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "transform.model"),  1, GL_FALSE, glm::value_ptr(m_model));
	glUniformMatrix3fv(glGetUniformLocation(program, "transform.normal"), 1, GL_FALSE, glm::value_ptr(m_normal));

	glDrawArrays(GL_PATCHES, 0, 1);
}

glm::mat4 SmartSphere::getModel() const
{
	return m_model;
}

void SmartSphere::update()
{
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, m_translate);
	m_model = m_model * m_rotate;
	m_model = glm::scale(m_model, glm::vec3(m_scale));

	m_normal = glm::transpose(glm::inverse(glm::mat3(m_model)));
}

SmartSphere::~SmartSphere() 
{}