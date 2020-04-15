#include <fstream>
#include "AsteroidBelt.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>

static std::string asteroidObjPath = "C:/Users/Владислав/Downloads/rubik-s-cube-master/rubik-s-cube-master/OpenGLWindowProject/Objects/rock/rock.obj";

AsteroidBelt::AsteroidBelt(int amount, float radius, float offset)
	: Object3D(asteroidObjPath)
{
	initGLData();
}

void AsteroidBelt::initModels(int amount, float radius, float offset)
{
    for (int i = 0; i < amount; ++i)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        m_models.push_back(model);
    }
}

void AsteroidBelt::initGLData()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    const int dataStorageSize = m_vertexData.size() * sizeof(Vertex);
    glBufferData(GL_ARRAY_BUFFER, dataStorageSize, NULL, GL_STATIC_DRAW);
    GLfloat* mapBuffer = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    if (mapBuffer) {
        memcpy(mapBuffer, m_vertexData.data(), m_vertexData.size() * sizeof(Vertex));
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataStorageSize * sizeof(GLfloat), m_vertexData.data());
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);


    int stride = sizeof(Vertex);
    int offset = 0;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    offset += sizeof(glm::vec3);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    glEnableVertexAttribArray(1);
    offset += sizeof(glm::vec3);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    OPENGL_CHECK_FOR_ERRORS();
}

void AsteroidBelt::update()
{
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, m_translate);
	m_model = m_model * m_rotate;
	m_model = glm::scale(m_model, glm::vec3(m_scale));

	m_normal = glm::transpose(glm::mat3(glm::inverse(m_model)));
}

void AsteroidBelt::deleteData()
{
    Object3D::deleteData();
	std::vector<glm::mat4>().swap(m_models);
}

void AsteroidBelt::rotate(float angle, const glm::vec3& v)
{
	m_rotate = glm::rotate(m_rotate, angle, v);
	update();
}
void AsteroidBelt::translate(const glm::vec3& v)
{
	m_translate += v;
	update();
}

void AsteroidBelt::scale(float scale)
{
	m_scale *= scale;
	update();
}

void AsteroidBelt::draw(const ShaderProgram& program) const
{
    glUniformMatrix4fv(glGetUniformLocation(program, "transform.model"), 1, GL_FALSE, glm::value_ptr(m_model));
    glUniformMatrix3fv(glGetUniformLocation(program, "transform.normal"), 1, GL_FALSE, glm::value_ptr(m_normal));

    m_material.set(program);

    glBindVertexArray(m_vao);
    GLsizei indicesCount = static_cast<GLsizei>(m_indices.size());
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    OPENGL_CHECK_FOR_ERRORS();
}

glm::mat4 AsteroidBelt::getModel() const
{
	return m_model;
}

AsteroidBelt::~AsteroidBelt()
{}