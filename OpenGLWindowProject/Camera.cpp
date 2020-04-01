#include "Camera.h"
#include "OpenGL.h"
#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

void Camera::rotate(float angle,const glm::vec3 &r)
{}

void Camera::translate(const glm::vec3 &t)
{}

void Camera::scale(float scale)
{}

void Camera::updateModel()
{
    m_model = glm::lookAt(m_position, m_targetPos, m_upVector);
}

void Camera::draw(const ShaderProgram& program) const
{
    glUniformMatrix4fv(glGetUniformLocation(program, "transform.view"), 1, GL_FALSE, glm::value_ptr(m_model));
    glUniform3fv(glGetUniformLocation(program, "transform.viewPos"), 1, glm::value_ptr(m_position));
}

glm::mat4 Camera::getModel() const
{
    return m_model;
}

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up)
    : m_position(pos),
      m_targetPos(front),
      m_upVector(up)
{
    updateModel();
}