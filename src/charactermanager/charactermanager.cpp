
#include "charactermanager.h"
#include "../logmanager/logmanager.h"
#include "../rendermanager/rendermanager.h"
#include "../configuration/configuration.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

frame CharacterManager::shoulders = frame();
bool CharacterManager::rift_input = false;
double CharacterManager::meter = 0.1;
double CharacterManager::altitude = 0.0;

void CharacterManager::handle(double dt) {
    handle_keyboard(dt);
    if(rift_input) {
        handle_rift(dt);
    } else {
        handle_mouse(dt);
    }

    shoulders.correct_roundoff();
}

bool CharacterManager::startup() {
    // TODO: read the initial position from the level data instead
    shoulders.pos = glm::dvec4(0,0,0,1);
    shoulders.right = glm::dvec4(1,0,0,0);
    shoulders.up = glm::dvec4(0,1,0,0);
    shoulders.forward = glm::dvec4(0,0,-1,0);

    move_cursor_to_center();

    LogManager::log_info("CharacterManager started.", 2);
    return true;
}

void CharacterManager::shutdown() {
    LogManager::log_info("CharacterManager stopped.", 2);
}

void CharacterManager::handle_keyboard(double dt) {
    GLFWwindow* win = RenderManager::window;

    //direction of walking in coordinates (right, up, forward)
    glm::dvec3 walking_direction = glm::dvec3();

    if( glfwGetKey(win, GLFW_KEY_UP) || glfwGetKey(win, GLFW_KEY_W )) {
        walking_direction += glm::dvec3(0,0,1);
    }
    if( glfwGetKey(win, GLFW_KEY_DOWN) || glfwGetKey(win, GLFW_KEY_S )) {
        walking_direction += glm::dvec3(0,0,-1);
    }    
    if( glfwGetKey(win, GLFW_KEY_LEFT) || glfwGetKey(win, GLFW_KEY_A )) {
        walking_direction += glm::dvec3(-1,0,0);
    }
    if( glfwGetKey(win, GLFW_KEY_RIGHT) || glfwGetKey(win, GLFW_KEY_D )) {
        walking_direction += glm::dvec3(1,0,0);
    }
    // TODO: do something sensible with these keys (we want to walk on the
    // floor, eye level constant, but for debug this may be useful)
    if( glfwGetKey(win, GLFW_KEY_PAGE_UP) || glfwGetKey(win, GLFW_KEY_R)) {
        walking_direction += glm::dvec3(0,1,0);
    }
    if( glfwGetKey(win, GLFW_KEY_PAGE_DOWN) || glfwGetKey(win, GLFW_KEY_F)) {
        walking_direction += glm::dvec3(0,-1,0);
    }

    //length is either zero or at least one, mathematically
    //but floating point
    if(glm::length(walking_direction) >= 0.5)
    {
        walking_direction = glm::normalize(walking_direction);
        walking_direction *= Configuration::walking_speed * dt * meter;
        glm::dvec4 newpos = hypermath::exp(shoulders.pos, walking_direction.x*shoulders.right + walking_direction.y*shoulders.up + walking_direction.z*shoulders.forward);
        glm::dmat4 transf = hypermath::translation(shoulders.pos,newpos);

        // TODO: overload dmat4*frame and use it
        shoulders.pos = newpos;
        shoulders.up = transf * shoulders.up;
        shoulders.right = transf * shoulders.right;
        shoulders.forward = transf * shoulders.forward;
    }
}

void CharacterManager::handle_mouse(double dt) {

    double mouse_x, mouse_y;
    double center_x = floor(RenderManager::get_window_width() / 2);
    double center_y = floor(RenderManager::get_window_height() / 2);
    // TODO: put PI in a sensible place
    const double PI = 3.141592653589793238463;

    GLFWwindow* win = RenderManager::window;
    glfwGetCursorPos(win, &mouse_x, &mouse_y);
    //this is inefficient, because the center is calculated twice
    move_cursor_to_center(); 

    double angle_ver = Configuration::mouse_speed * (center_y - mouse_y);
    altitude += angle_ver;
    altitude = fmin(PI/2, fmax(-PI/2, altitude)); //clamp

    double angle_hor = Configuration::mouse_speed * (mouse_x - center_x);
    shoulders.rotate_right(angle_hor);

    return; 
}

void CharacterManager::handle_rift(double dt) {
}

frame CharacterManager::get_position_left_eye() {
    // TODO: fix this
    return shoulders;
}

frame CharacterManager::get_position_right_eye() {
    // TODO: fix this
    return shoulders;
}

frame CharacterManager::get_position_eyes() {
    // TODO: add character height
    frame result = shoulders;
    result.rotate_up(altitude);
    return result;
}

void CharacterManager::move_cursor_to_center() {
    double center_x = floor(RenderManager::get_window_width() / 2);
    double center_y = floor(RenderManager::get_window_height() / 2);
    GLFWwindow* win = RenderManager::window;
    glfwSetCursorPos(win, center_x, center_y);
}

