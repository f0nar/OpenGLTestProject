#pragma once
#include "common.h"
#include "OpenGL.h"

// ������� �������� �� TGA-����� � ������������
GLuint TextureCreateFromTGA(const char* fileName);

// ������� ��������
void TextureDestroy(GLuint texture);

