#include <fstream>
#include "AsteroidBelt.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>

static std::string asteroidObjPath = "C:/Users/Владислав/Downloads/rubik-s-cube-master/rubik-s-cube-master/OpenGLWindowProject/Objects/rock/rock.obj";

AsteroidBelt::AsteroidBelt(int amount, float radius, float offset)
	: Object3D(asteroidObjPath)
{
    initModels(amount, radius, offset);
	initGLData();
}

void AsteroidBelt::initModels(int amount, float radius, float offset)
{
    glm::mat4 mat(1.0f);
    mat = glm::translate(mat, glm::vec3(5.0f, 5.0f, 0.0f));
    
    for (int i = 0; i < amount; ++i)
    {
        glm::mat4 model = glm::mat4(1.0f);
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        float scale = (rand() % 20) / 100.0f + 0.05f;
        model = glm::scale(model, glm::vec3(scale));

        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        m_models.push_back(model);
    }
    
}

void AsteroidBelt::initGLData()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenBuffers(1, &m_mbo);
    glGenBuffers(1, &m_tbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    const int vertexCount = static_cast<const int>(m_vertexData.size());
    const int dataStorageSize = vertexCount * sizeof(Vertex);
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

    glBindBuffer(GL_TEXTURE_BUFFER, m_tbo);
    glBufferData(GL_TEXTURE_BUFFER, m_models.size() * sizeof(glm::mat4), m_models.data(), GL_STATIC_DRAW);

    glGenTextures(1, &m_textureBuffer);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_BUFFER, m_textureBuffer);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_tbo);

    int stride = sizeof(Vertex);
    __int64 offset = 0;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    offset += sizeof(glm::vec3);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(1);
    offset += sizeof(glm::vec3);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_mbo);
    glBufferData(GL_ARRAY_BUFFER, m_models.size() * sizeof(glm::mat4), m_models.data(), GL_STATIC_DRAW);

    offset = 0;
    stride = sizeof(glm::mat4);
    int modelLocation = 3;
    for (int i = 0; i < 4; ++i, offset += sizeof(glm::vec4))
    {
        glVertexAttribPointer(modelLocation + i, 4, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        glEnableVertexAttribArray(modelLocation + i);
        glVertexAttribDivisor(modelLocation + i, 1);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_TEXTURE_BUFFER, 0);

    OPENGL_CHECK_FOR_ERRORS();
}

void AsteroidBelt::update()
{
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, m_translate);
	m_model = m_model * m_rotate;
	m_model = glm::scale(m_model, glm::vec3(m_scale));
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
    GLsizei modelCount = static_cast<GLsizei>(m_models.size());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_BUFFER, m_textureBuffer);
    glUniform1i(glGetUniformLocation(program, "transform.model_tbo"), 1);
    //glUniformMatrix4fv(glGetUniformLocation(program, "transform.model"), modelCount, GL_FALSE, (float *)m_models.data());
    glUniformMatrix4fv(glGetUniformLocation(program, "transform.globalModel"), 1, GL_FALSE, glm::value_ptr(m_model));

    m_material.set(program);

    glBindVertexArray(m_vao);
    GLsizei indicesCount = static_cast<GLsizei>(m_indices.size());
    glDrawElementsInstanced(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0, modelCount);
    glBindVertexArray(0);

    OPENGL_CHECK_FOR_ERRORS();
}

glm::mat4 AsteroidBelt::getModel() const
{
	return m_model;
}

AsteroidBelt::~AsteroidBelt()
{}