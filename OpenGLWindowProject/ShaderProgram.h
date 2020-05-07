#pragma once
#include "OpenGL.h"
#include <string>

class ShaderProgram
{
	GLuint m_program;
	bool compiled;
	std::string loadCode(const std::wstring& path) const;
	GLuint createShader(const std::string& sCode, GLenum shader) const;
public:
	ShaderProgram();
	bool attach(const std::wstring& vpath, GLenum shader);
	bool link();
	void use() const;
	operator GLuint() const;
};

