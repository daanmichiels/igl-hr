#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "data/object.h"
#include <GLFW/glfw3.h>
#include "charactermanager/cameracontrols.h"
#include <vector>
#include "data/camera.h"
#include "data/mesh.h"
#include "flagmanager.h"
#include "../thirdparty/glm/glm/glm.hpp"

class InputHandler
{
    public:
        static object* grid;
        static CameraControls cameracontrols;
        static flagmanager* flag_manager;
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void handle(double delta_time, int width, int height);
};

#endif

