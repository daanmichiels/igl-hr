#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

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

#include "../data/object.h"
#include <vector>
#include "../data/mesh.h"
#include "glm/glm.hpp"

class InputManager
{
    public:
        static bool startup();
        static void shutdown();
        static object* grid;
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        static void handle(double delta_time, int width, int height);
};

#endif

