#pragma once
#include "OpenGL.h"
#include <string>

class ShaderProgram
{
	GLuint m_program;
	std::string loadCode(const std::wstring& path) const;
	GLuint createShader(const std::string& sCode, GLenum shader) const;
	GLuint createProgram(GLuint vertex, GLuint fragment) const;
public:
	ShaderProgram(const std::wstring& vpath, const std::wstring& fpath);
	void use() const;
	operator GLuint() const;
};

