
#ifndef INIT_H
#define INIT_H

#include "GLFW/glfw3.h"
#include "OVR/OVR.h"

class Init {
public:
    static void welcome_message();
    static void glfw_error_callback(int error, const char* description);
    static bool init_glew();
    static ovrHmd init_rift();
    static GLFWwindow* open_window();
};

#endif

