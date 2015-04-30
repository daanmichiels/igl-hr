
#include "../logmanager/logmanager.h"
#include "shadermanager.h"
#include "../shaders/shaders.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint ShaderManager::default_program = 0;
GLuint ShaderManager::quad_program = 0;
GLuint ShaderManager::flag_program = 0;

/** \brief Startup the shader manage. Makes sure the shaders are compiled and linked correctly logs ShaderManager started at level 2
 * \param void
 * \return false if any of the shaders do not compile, true if everything is compiled correctly
 */
bool ShaderManager::startup() {
    GLuint vertex;
    GLuint fragment;
    GLuint rift_vertex;
    GLuint rift_fragment;
    GLuint fragment_offset;

    if(!compile_shader(shader_sources["vertex"], GL_VERTEX_SHADER, vertex)) {
        return false;
    }
	LogManager::log_info("He's alive Jim.", 2);
    if(!compile_shader(shader_sources["fragment"], GL_FRAGMENT_SHADER, fragment)) {
        return false;
    }
	LogManager::log_info("He's dead Jim.", 2);
    if(!compile_shader(shader_sources["rift_vertex"], GL_VERTEX_SHADER, rift_vertex)) {
        return false;
    }
    if(!compile_shader(shader_sources["rift_fragment"], GL_FRAGMENT_SHADER, rift_fragment)) {
        return false;
    }
    if(!compile_shader(shader_sources["fragment_offset"], GL_FRAGMENT_SHADER, fragment_offset)) {
        return false;
    }

    if(!link_program(vertex, fragment, default_program)) {
        return false;
    }
    if(!link_program(rift_vertex, rift_fragment, quad_program)) {
        return false;
    }
    if(!link_program(vertex, fragment_offset, flag_program)) {
        return false;
    }

    LogManager::log_info("ShaderManager started.", 2);
    return true;
}

/** \brief Shutdown the ShaderManager. Logs ShaderManager stopped at level 2
 * \param void
 * \return void
 */
void ShaderManager::shutdown() {
    LogManager::log_info("ShaderManager stopped.", 2);
}

/**
 * Links a shading program.
 *
 * Links a shading program from the given (already compiled) vertex and fragment shader.
 *
 * \param  vertex_shader The vertex shader
 * \param  fragment_shader The fragment shader
 * \param  program Where to store the resulting handle (the return value is used to indicate success)
 * \return True on successful linking, false otherwise
 */
bool ShaderManager::link_program(GLuint vertex_shader, GLuint fragment_shader, GLuint &program) {
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(!linked) {
        // For simplicity, we only print the first 200 characters of the error.
        char error[200];
        glGetProgramInfoLog(program, 200, NULL, error);
        LogManager::log_error(std::string("Shading program linking failed: ") + error);
        program = 0;
        return false;
    }
    return true;
}


/**
 * Compiles a shader.
 *
 * Compiles a shader for the current OpenGL context.
 *
 * \param  source The shader source code
 * \param  shader_type The type of shader (vertex, fragment)
 * \param  shader Where to store the resulting handle (the return value is used to indicate success)
 * \return True on successful compilation, false otherwise
 */
bool ShaderManager::compile_shader(std::string source, GLenum shader_type, GLuint &shader) {
    const char* src [1];
    src[0] = source.c_str();

    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, src, NULL);
    glCompileShader(shader);
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        // For simplicity, we only print the first 200 characters of the error.
        char error[8000];
        glGetShaderInfoLog(shader, 8000, NULL, error);
        LogManager::log_error(std::string("Shader compilation failed: ") + error);
        shader = 0;
        return false;
    }
    return true;
}

