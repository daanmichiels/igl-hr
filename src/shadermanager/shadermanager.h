#include <GL/glew.h>

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#if defined(_WIN32)
 #define GLFW_EXPOSE_NATIVE_WIN32
 #define GLFW_EXPOSE_NATIVE_WGL
 #define OVR_OS_WIN32
#elif defined(__APPLE__)
 #define GLFW_EXPOSE_NATIVE_COCOA
 #define GLFW_EXPOSE_NATIVE_NSGL
 #define OVR_OS_MAC
#elif defined(__linux__)
 #define GLFW_EXPOSE_NATIVE_X11
 #define GLFW_EXPOSE_NATIVE_GLX
 #define OVR_OS_LINUX
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <OVR_CAPI_GL.h>


class ShaderManager {
public:
    static bool startup();
    static void shutdown();

    static GLuint default_program;
    static GLuint cross_program;
    static GLuint quad_program;
    static GLuint flag_program;

private:
    static bool compile_shader(std::string source, GLenum shaderType, GLuint &shader);
    static bool link_program(GLuint vertex_shader, GLuint fragment_shader, GLuint &program);
};

#endif

