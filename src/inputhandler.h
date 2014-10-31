#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include "object.h"
#include <GLFW/glfw3.h>
#include "cameracontrols.h"
#include <vector>
#include "camera.h"
#include "mesh.h"
#include "FlagManager.h"
#include "../thirdparty/glm/glm/glm.hpp"

class InputHandler
{
    public:
        static object* grid;
        static CameraControls cameracontrols;
        static FlagManager* flagmanager;
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void handle(double delta_time, int width, int height);
};

#endif

