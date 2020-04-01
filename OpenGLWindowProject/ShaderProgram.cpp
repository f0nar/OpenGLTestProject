#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include "Loger.h"

ShaderProgram::ShaderProgram(const std::wstring& vpath, const std::wstring& fpath)
{
    std::string vertexCode = loadCode(vpath);
    std::string fragmentCode = loadCode(fpath);

    GLuint vertex = createShader(vertexCode, GL_VERTEX_SHADER);
    GLuint fragment = createShader(fragmentCode, GL_FRAGMENT_SHADER);

    m_program = createProgram(vertex, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void ShaderProgram::use() const
{
    glUseProgram(m_program);
}

ShaderProgram::operator GLuint() const
{
    return m_program;
}

std::string ShaderProgram::loadCode(const std::wstring& path) const
{
    std::string sCode;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::badbit);
    try
    {
        shaderFile.open(path);
        std::stringstream shaderStream;

        shaderStream << shaderFile.rdbuf();

        shaderFile.close();

        sCode = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        LOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }

    return sCode;
}

GLuint ShaderProgram::createShader(const std::string& sCode, GLenum shader_t) const
{
    GLuint shader;
    const char *shaderCode = sCode.c_str();

    shader = glCreateShader(shader_t);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    ShaderStatus(shader, GL_COMPILE_STATUS);

    return shader;
}

GLuint ShaderProgram::createProgram(GLuint vertex, GLuint fragment) const
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    ShaderProgramStatus(program, GL_LINK_STATUS);

    return program;
}
