#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "shaders.h"

// Shaders will be linked to these
extern char _source_vs_start[];
extern char _source_vs_end[];
extern char _source_vs_size[];
extern char _source_fs_start[];
extern char _source_fs_end[];
extern char _source_fs_size[];

// Create and compile a shader
GLuint compile_shader(std::string source, GLenum shaderType)
{
	const char* src [1];
	src[0] = source.c_str();

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, src, NULL);
	glCompileShader(shader);

	return shader;
}

// Compile shaders and link program
GLuint build_program()
{
	std::string source_vs(_source_vs_start, (size_t)_source_vs_size);
	std::string source_fs(_source_fs_start, (size_t)_source_fs_size);

	GLuint vs = compile_shader(source_vs, GL_VERTEX_SHADER);
	GLuint fs = compile_shader(source_fs, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	return program;
}

