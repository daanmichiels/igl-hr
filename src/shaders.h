#ifndef SHADERS_H
#define SHADERS_H

GLuint compile_shader(std::string source, GLenum shaderType);
GLuint build_program();
GLuint build_quad_program();

#endif /* SHADERS_H */
