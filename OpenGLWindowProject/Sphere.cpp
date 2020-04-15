#define _USE_MATH_DEFINES
#include <math.h>
#include "Sphere.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

const float M_PI_F = static_cast<float>(M_PI);

Sphere::Sphere(const ShaderProgram &program, const Material &material, float radius, int stackCount, int sectorCount)
{
    m_material = material;
    initVertexData(radius, stackCount, sectorCount);
    initIndexData(stackCount, sectorCount);
    initGLData();
}

void Sphere::rotate(float angle,const glm::vec3 &r)
{
    m_rotate = glm::rotate(m_rotate, angle, r);
    update();
}

void Sphere::translate(const glm::vec3 &t)
{
    m_translate += t;
    update();
}

void Sphere::scale(float s)
{
    m_scale *= s;
    update();
}

void Sphere::draw(const ShaderProgram &program) const
{
    glUniformMatrix4fv(glGetUniformLocation(program, "transform.model"), 1, GL_FALSE, glm::value_ptr(m_model));
    glUniformMatrix3fv(glGetUniformLocation(program, "transform.normal"), 1, GL_FALSE, glm::value_ptr(m_normal));

    m_material.set(program);

    glBindVertexArray(m_vao);
    GLsizei indicesCount = static_cast<GLsizei>(m_indices.size());
    glDrawElements(GL_TRIANGLES, indicesCount , GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    OPENGL_CHECK_FOR_ERRORS();
}

void Sphere::update()
{
    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_translate);
    m_model = m_model * m_rotate;
    m_model = glm::scale(m_model, glm::vec3(m_scale));

    m_normal = glm::transpose(glm::mat3(glm::inverse(m_model)));
}

glm::mat4 Sphere::getModel() const
{
    return m_model;
}

void Sphere::initVertexData(float radius, int stackCount, int sectorCount)
{
    deleteData();

    float x, y, z, zx;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sectorStep = 2.0f * M_PI_F / sectorCount;
    float stackStep  = 1.0f * M_PI_F / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI_F / 2.0f - i * stackStep;
        zx = radius * cosf(stackAngle);
        y = radius * sinf(stackAngle);


        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;

            z = zx * cosf(sectorAngle);
            x = zx * sinf(sectorAngle);
            glm::vec3 vertice(x, y, z);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            glm::vec3 normal(nx, ny, nz);

            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            glm::vec2 texCoord(s, t);

            addVertex(vertice, normal, texCoord);
        }
    }
}

void Sphere::initIndexData(int stackCount, int sectorCount)
{
    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j)
        {
            if (i != 0)
            {
                addIndices(k1 + j, k2 + j, k1 + j + 1);
            }

            if (i != (stackCount - 1))
            {
                addIndices(k1 + j + 1, k2 + j, k2 + j + 1);
            }
        }
    }
}

void Sphere::initGLData()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    const int dataStorageSize = (int)m_vertexData.size() * sizeof(Vertex);
    glBufferData(GL_ARRAY_BUFFER, dataStorageSize, NULL, GL_STATIC_DRAW);
    GLfloat* mapBuffer = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    if (mapBuffer) {
        memcpy(mapBuffer, m_vertexData.data(), dataStorageSize);
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataStorageSize * sizeof(GLfloat), m_vertexData.data());
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);


    int stride = sizeof(Vertex), offset = 0;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0); 
    offset += sizeof(glm::vec3);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset); 
    offset += sizeof(glm::vec3);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    OPENGL_CHECK_FOR_ERRORS();
}

Sphere::~Sphere()
{}