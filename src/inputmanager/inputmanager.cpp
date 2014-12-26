#include "inputmanager.h"
#include "../logmanager/logmanager.h"
#include "../charactermanager/charactermanager.h"
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
    if(key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        // TODO: ensure charactermanager is started
        //       because it is later in the startup sequence
        //       (cyclic dependency?)
        if(CharacterManager::is_mouse_bound()) {
            CharacterManager::unbind_mouse();
        } else {
            CharacterManager::bind_mouse();
        }
    }
    /*
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

