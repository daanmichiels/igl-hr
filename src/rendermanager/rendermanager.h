
#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "OVR/OVR.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class RenderManager {
public:
    static bool startup();
    static void shutdown();

private:
    static GLFWwindow* window;
    static ovrHmd hmd;

    static bool init_glew();
    static bool init_rift();
    static bool open_window();
    static void glfw_error_callback(int error, const char* description);
};


#endif

