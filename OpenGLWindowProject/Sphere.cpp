#define _USE_MATH_DEFINES
#include <math.h>
#include "Sphere.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

const float M_PI_F = static_cast<float>(M_PI);

Sphere::Sphere(const ShaderProgram &program, const Material &material, float radius, int stackCount, int sectorCount)
    : material(material)
{
    initVertexData(radius, stackCount, sectorCount);
    initIndexData(stackCount, sectorCount);
    initGLData(program);
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

    material.set(program);

    glBindVertexArray(vao);
    GLsizei indicesCount = static_cast<GLsizei>(indices.size());
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
            addVertices(x, y, z);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            addNormals(nx, ny, nz);

            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            addTexCoord(s, t);
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

void Sphere::initGLData(const ShaderProgram &program)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    const int vertexCount     = static_cast<int>(vertices.size() / 3);
    const int dataStorageSize = vertexCount * 8;
    glBufferData(GL_ARRAY_BUFFER, dataStorageSize * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
    GLfloat* mapBuffer = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    if (mapBuffer) {
        for (int i = 0; i < vertexCount; ++i) {
            long int mpi = i * 8L, vni = i * 3L, tci = i * 2L;
            *(mapBuffer + mpi + 0) = vertices[vni];
            *(mapBuffer + mpi + 1) = vertices[vni + 1LL];
            *(mapBuffer + mpi + 2) = vertices[vni + 2LL];
            *(mapBuffer + mpi + 3) = normals [vni];
            *(mapBuffer + mpi + 4) = normals [vni + 1LL];
            *(mapBuffer + mpi + 5) = normals [vni + 2LL];
            *(mapBuffer + mpi + 6) = texCoords[tci];
            *(mapBuffer + mpi + 7) = texCoords[tci + 1LL];
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    else {
        std::vector<float> vertexData;
        vertexData.reserve(dataStorageSize);
        for (int i = 0; i < vertexCount; ++i) {
            long int vni = i * 3L, tci = i * 2L;
            vertexData.push_back(vertices[vni]);
            vertexData.push_back(vertices[vni + 1LL]);
            vertexData.push_back(vertices[vni + 2LL]);
            vertexData.push_back(normals [vni]);
            vertexData.push_back(normals [vni + 1LL]);
            vertexData.push_back(normals [vni + 2LL]);
            vertexData.push_back(texCoords[tci]);
            vertexData.push_back(texCoords[tci + 1LL]);
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataStorageSize * sizeof(GLfloat), vertexData.data());
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);


    int stride = sizeof(float) * 8;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    OPENGL_CHECK_FOR_ERRORS();
}

void Sphere::addVertices(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}
void Sphere::addNormals(float x, float y, float z)
{
    normals.push_back(x);
    normals.push_back(y);
    normals.push_back(z);
}
void Sphere::addTexCoord(float x, float y)
{
    texCoords.push_back(x);
    texCoords.push_back(y);
}
void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

void Sphere::deleteData()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<GLuint>().swap(indices);
}

Sphere::~Sphere()
{}