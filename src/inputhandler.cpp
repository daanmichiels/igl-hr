#include "inputhandler.h"
#include "object.h"
#include <vector>
#include "camera.h"
#include "mesh.h"
#include "../thirdparty/glm/glm/glm.hpp"
#include "flagmanager.h"
#include <iostream>

CameraControls InputHandler::cameracontrols = CameraControls(NULL, NULL, NULL);
object* InputHandler::grid = NULL;
flagmanager* InputHandler::flag_manager = NULL;

void InputHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
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
}

void InputHandler::handle(double delta_time, int width, int height)
{
    cameracontrols.handle(delta_time, width, height);
}

