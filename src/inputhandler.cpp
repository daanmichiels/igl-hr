#include "inputhandler.h"

CameraControls InputHandler::cameracontrols = CameraControls(NULL, NULL, NULL);

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
        cameracontrols.bind_mouse = ! cameracontrols.bind_mouse;
        if(cameracontrols.bind_mouse)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        }
    }
}

void InputHandler::handle(double delta_time, int width, int height)
{
    cameracontrols.handle(delta_time, width, height);
}

