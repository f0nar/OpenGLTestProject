#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include "Loger.h"

ShaderProgram::ShaderProgram()
    : compiled(false), m_program(0)
{
    m_program = glCreateProgram();
}

void ShaderProgram::use() const
{
    if(compiled) glUseProgram(m_program);
}

ShaderProgram::operator GLuint() const
{
    if(!compiled) LOG_ERROR("ERROR::PROGRAM::PROGRAM_NOT_COMPILED");
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

bool ShaderProgram::attach(const std::wstring& vpath, GLenum shader_t)
{
    std::string shaderSrc = std::move(loadCode(vpath));
    GLuint shader = createShader(shaderSrc, shader_t);
    glAttachShader(m_program, shader);
    
    int status =  ShaderStatus(shader, GL_COMPILE_STATUS);

    if (status)
        glDeleteShader(shader);

    return status;
}

bool ShaderProgram::link()
{
    glLinkProgram(m_program);

    return compiled = ShaderProgramStatus(m_program, GL_LINK_STATUS);
}
