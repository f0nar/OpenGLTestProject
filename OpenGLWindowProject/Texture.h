#pragma once
#include "common.h"
#include "OpenGL.h"

// создает текстуру из TGA-файла с изображением
GLuint TextureCreateFromTGA(const char* fileName);

// удаляет текстуру
void TextureDestroy(GLuint texture);

