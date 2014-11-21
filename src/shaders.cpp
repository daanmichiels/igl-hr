#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "shaders.h"
#include "shaders/fragment.h"
#include "shaders/vertex.h"
#include "shaders/rift_fragment.h"
#include "shaders/rift_vertex.h"

// Shaders will be linked to these
// extern char _binary_src_shaders_rift_vertex_glsl_start[];
// extern char _binary_src_shaders_rift_vertex_glsl_end[];
// extern char _binary_src_shaders_rift_vertex_glsl_size[];
// extern char _binary_src_shaders_rift_fragment_glsl_start[];
// extern char _binary_src_shaders_rift_fragment_glsl_end[];
// extern char _binary_src_shaders_rift_fragment_glsl_size[];
// extern char _binary_src_shaders_vertex_glsl_start[];
// extern char _binary_src_shaders_vertex_glsl_end[];
// extern char _binary_src_shaders_vertex_glsl_size[];
// extern char _binary_src_shaders_fragment_glsl_start[];
// extern char _binary_src_shaders_fragment_glsl_end[];
// extern char _binary_src_shaders_fragment_glsl_size[];

// Create and compile a shader.
// This is not supposed to be called by other files, and
// should somehow be made inaccessible (TODO).
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
        // For simplicity, we only print the first 200 characters of the error.
        char error[200];
        glGetShaderInfoLog(shader, 200, NULL, error);
        std::cout << "Shader compilation failed: " << error << "\n";
        return 0;
    }

    return shader;
}

// Compile shaders and link program.
// Returns 0 on fail.
GLuint build_program()
{
    std::string source_vertex(_binary_src_shaders_vertex_glsl_start);
    std::string source_fragment(_binary_src_shaders_fragment_glsl_start);

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

    std::cout << "location of uniform 'projection': " << glGetUniformLocation(program, "projection") << std::endl;
    std::cout << "location of uniform 'modelview': " << glGetUniformLocation(program, "modelview") << std::endl;

    return program;
}

// Compile shaders and link program.
// Returns 0 on fail.
GLuint build_quad_program()
{
    std::string source_vertex(_binary_src_shaders_rift_vertex_glsl_start);
    std::string source_fragment(_binary_src_shaders_rift_fragment_glsl_start);

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

