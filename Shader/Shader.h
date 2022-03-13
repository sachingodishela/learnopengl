#pragma once
#include <glad/glad.h>

class Shader
{
public:
	GLuint id;
	Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
	void use();
};
