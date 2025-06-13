#include "gl/glew.h"
#include "GL/freeglut.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static GLuint shaderCompileFromFile(GLenum type, const char *filePath);
void shaderAttachFromFile(GLuint program, GLenum type, const char *filePath);
int loadShaders(const char * vertexShaderPath, const char * fragmentShaderPath);
char * shaderLoadSource(const char *filePath);
int loadComputeShader(const char* computeShaderPath);