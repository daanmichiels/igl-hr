#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "../data/object.h"
#include <GLFW/glfw3.h>
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

