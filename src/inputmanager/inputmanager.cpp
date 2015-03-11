#include "inputmanager.h"
#include "../logmanager/logmanager.h"
#include "../charactermanager/charactermanager.h"
#include "../rendermanager/rendermanager.h"
#include "../loopmanager/loopmanager.h"
#include "../logicmanager/logicmanager.h"
#include "../data/object.h"
#include <vector>
#include "../data/mesh.h"
#include "glm/glm.hpp"
#include <iostream>

object* InputManager::grid = NULL;

/** \brief Starts the input manager. Logs inputmanager started at level 2
 * \param void
 * \return boolean true
 */
bool InputManager::startup() {
    assert(RenderManager::window);
    glfwSetKeyCallback(RenderManager::window, key_callback);
    glfwSetScrollCallback(RenderManager::window, scroll_callback);

    LogManager::log_info("InputManager started.", 2);
    return true;
}

/** \brief Shutdown the input manager. Logs inputmanager stopped at level 2.
 * \param void
 * \return void
 */
void InputManager::shutdown() {
    LogManager::log_info("InputManager stopped.", 2);
}

void InputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    CharacterManager::scale(pow(1.2, -yoffset));
    RenderManager::handle_scale_change();
}

/** \brief Key callback for glfw
 * \param GLFWwindow*, key, scan code, action, and mods
 * \return void
 */
void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } 
    if(key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS) {
        std::string info = std::to_string(LoopManager::fpscounter.fps) + " fps";
        if(LogicManager::flags.size() > 0) {
            // TODO: fix the distance calculation
            info += "  |   dist to flag: " + std::to_string(hypermath::dist(CharacterManager::get_position_feet(), LogicManager::flag_locations[LogicManager::flags.size() - 1]));
        }
            
        LogManager::log_info(info, 0);
    }
    if(key == GLFW_KEY_M && action == GLFW_PRESS) {
        // TODO: ensure charactermanager is started
        //       because it is later in the startup sequence
        //       (cyclic dependency?)
        if(CharacterManager::is_mouse_bound()) {
            CharacterManager::unbind_mouse();
        } else {
            CharacterManager::bind_mouse();
        }
    }
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        CharacterManager::reset_to_origin();
    }
    if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
        CharacterManager::scale(1.2);
        RenderManager::handle_scale_change();
    }
    if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
        CharacterManager::scale(1.0/1.2);
        RenderManager::handle_scale_change();
    }
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        LogicManager::add_flag();
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

/** \brief Handle input. Empty function
 * \param deltatime, width and height of window
 * \return void
 */
void InputManager::handle(double delta_time, int width, int height)
{
    //cameracontrols.handle(delta_time, width, height);
}

