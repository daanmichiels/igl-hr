
#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ShaderManager {
public:
    static bool startup();
    static void shutdown();

    static GLuint default_program;
    static GLuint quad_program;

private:
    static bool compile_shader(std::string source, GLenum shaderType, GLuint &shader);
    static bool link_program(GLuint vertex_shader, GLuint fragment_shader, GLuint &program);
};

#endif

