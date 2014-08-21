#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "shaders.h"

// Shaders will be linked to these
extern char _source_vertex_start[];
extern char _source_vertex_end[];
extern char _source_vertex_size[];
extern char _source_fragment_start[];
extern char _source_fragment_end[];
extern char _source_fragment_size[];

// Create and compile a shader
GLuint compile_shader(std::string source, GLenum shaderType)
{
	const char* src [1];
	src[0] = source.c_str();

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, src, NULL);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
	{
		char error[200];
		glGetShaderInfoLog(shader, 200, NULL, error);
		std::cout << "Shader compilation failed: " << error << "\n";
		return 0;
	}

	return shader;
}

// Compile shaders and link program
GLuint build_program()
{
	std::string source_vertex(_source_vertex_start, (size_t)_source_vertex_size);
	std::string source_fragment(_source_fragment_start, (size_t)_source_fragment_size);

	GLuint vertex = compile_shader(source_vertex, GL_VERTEX_SHADER);
	if(!vertex)
		return 0;
	GLuint fragment = compile_shader(source_fragment, GL_FRAGMENT_SHADER);
	if(!fragment)
		return 0;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if(!linked)
	{
		char error[200];
		glGetProgramInfoLog(program, 200, NULL, error);
		std::cout << "Program linking failed: " << error << "\n";
		return 0;
	}

	return program;
}

