#include "Camera.h"
#include "OpenGL.h"
#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

void Camera::rotate(float angle, const glm::vec3 &r)
{
    const GLfloat senstive = 0.25f;

    float xOffset = r.x * senstive;
    float yOffset = r.y * senstive;

    m_yaw += xOffset;
    m_pitch += yOffset;

    updateModel();
}

void Camera::translate(const glm::vec3 &t)
{
    
}

void Camera::scale(float scale)
{

}

void Camera::updateModel()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_targetPos = m_position + glm::normalize(front);
    m_rightVector = glm::normalize(glm::cross(m_targetPos, m_upVector));  
    m_upVector = glm::normalize(glm::cross(m_rightVector, m_targetPos));

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
      m_upVector(up), 
      m_yaw(-90.0f),
      m_pitch(0.0f)
{
    updateModel();
}