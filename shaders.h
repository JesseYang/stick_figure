#pragma once

#include <GL/glew.h>
#include <iostream>
#include "file_proc.h"

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
GLuint CompileShaders(std::string pVSFileName, std::string pFSFileName);
