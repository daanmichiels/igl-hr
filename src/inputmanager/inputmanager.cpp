#include "inputmanager.h"
#include "../logmanager/logmanager.h"
#include "../rendermanager/rendermanager.h"
#include "../loopmanager/loopmanager.h"
#include "../data/object.h"
#include <vector>
#include "../data/camera.h"
#include "../data/mesh.h"
#include "glm/glm.hpp"
#include "../flagmanager.h"
#include <iostream>

CameraControls InputManager::cameracontrols = CameraControls(NULL, NULL, NULL);
object* InputManager::grid = NULL;
flagmanager* InputManager::flag_manager = NULL;

bool InputManager::startup() {
    assert(RenderManager::window);
    glfwSetKeyCallback(RenderManager::window, key_callback);

    LogManager::log_info("InputManager started.", 2);
    return true;
}

void InputManager::shutdown() {
    LogManager::log_info("InputManager stopped.", 2);
}

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if(key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS)
    {
        LogManager::log_info(std::to_string(LoopManager::fpscounter.fps) + " fps", 0);
    }
    /*
    if(key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        double center_x = floor(width/2);
        double center_y = floor(height/2);
        glfwSetCursorPos(window, center_x, center_y);
        cameracontrols.bind_mouse = !cameracontrols.bind_mouse;
        if(cameracontrols.bind_mouse)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    if(key == GLFW_KEY_G && action == GLFW_PRESS )
    {
        grid->visible = !grid->visible;
        std::cout << "grid->visible: " << grid->visible << '\n';
    }
    if(key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
    {
        cameracontrols.increase_speed();
    }
    if(key == GLFW_KEY_MINUS && action == GLFW_PRESS)
    {
        cameracontrols.decrease_speed();
    }
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS && flag_manager != NULL)
    {

        flag_manager->drop_flag();
    }
    if(key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS)
    {
        cameracontrols.reset_to_origin();
    }
    */
}

void InputManager::handle(double delta_time, int width, int height)
{
    //cameracontrols.handle(delta_time, width, height);
}

