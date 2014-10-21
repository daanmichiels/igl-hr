#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <GLFW/glfw3.h>
#include "cameracontrols.h"

class InputHandler
{
    public:
        static CameraControls cameracontrols;
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void handle(double delta_time, int width, int height);
};

#endif

