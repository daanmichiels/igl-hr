
#include "charactermanager.h"
#include "../logmanager/logmanager.h"
#include "../rendermanager/rendermanager.h"
#include "../configuration/configuration.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/gtc/quaternion.hpp"
#include "glm/ext.hpp"

using namespace OVR;

frame CharacterManager::shoulders = frame();
bool CharacterManager::rift_input = true;
bool CharacterManager::mouse_bound = true;
double CharacterManager::meter = 0.1;
double CharacterManager::altitude = 0.0;
glm::dquat CharacterManager::rift_orientation = glm::dquat(0,0,0,0);
ovrHmd* CharacterManager::_hmd = NULL;

/** \brief Call to handle keyboard, and the oculus rift or the mouse
 *  \param deltatime as a double
 * \return void
 */
void CharacterManager::handle(double dt) {
    handle_keyboard(dt);
    if (rift_input && _hmd != NULL) {
        handle_rift(dt);
    } 
    if (mouse_bound) {
        handle_mouse(dt);
    }

    shoulders.correct_roundoff();
}

/** \brief Binds the mouse to the window
 *  \param void
 * \return void
 */
void CharacterManager::bind_mouse() {
    // avoid centering the mouse when it was bound already
    if(mouse_bound) {
        return;
    }
    move_cursor_to_center();
    mouse_bound = true;

    // TODO: make cursor invisible. Added kyle 2/7/15 make sure this compiles before leaving it in
    //glfwSetInputMode(RenderManager::window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

/** \brief Unbinds the mouse from the window
 *  \param void
 * \return void
 */
void CharacterManager::unbind_mouse() {
    mouse_bound = false;
    // TODO: make cursor visible
    // glfwSetInputMode(RenderManager::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

/** \brief Check if the mouse is bound
 *  \param void
 * \return void
 */
bool CharacterManager::is_mouse_bound() {
    return mouse_bound;
}

void CharacterManager::set_hmd(ovrHmd* hmd) {
    _hmd = hmd;
}

/** \brief Start the character manager. Sets the shoulders up, moves the mouse to center, and logs starting of character
 *  manager
 *  \param void
 * \return boolean (true)
 */
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

/** \brief Shuts down the Character Manager. Logs character manager stopped
 *  \param void
 * \return void
 */
void CharacterManager::shutdown() {
    LogManager::log_info("CharacterManager stopped.", 2);
}

/** \brief Handles the keyboard input. 
 *  \param deltatime as a double
 * \return void
 */
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

    if ( glfwGetKey(win, GLFW_KEY_COMMA)) {
        shrink();
    }
    if ( glfwGetKey(win, GLFW_KEY_PERIOD)) {
        grow();
    }

    //length is either zero or at least one, mathematically
    //but floating point
    if(glm::length(walking_direction) >= 0.5)
    {
        walking_direction = glm::normalize(walking_direction);
        walking_direction *= Configuration::walking_speed * dt * meter;
        glm::dvec4 newpos = hypermath::exp(shoulders.pos, walking_direction.x*shoulders.right + walking_direction.y*shoulders.up + walking_direction.z*shoulders.forward);
        glm::dmat4 transf = hypermath::translation(shoulders.pos,newpos);

        shoulders = transf * shoulders;
    }
}
/** \brief Handles the mouse input. 
 *  \param deltatime as a double
 *  \return void
 */
void CharacterManager::handle_mouse(double dt) {

    double mouse_x, mouse_y, mouse_x1, mouse_y1;
    double center_x = /*floor(*/RenderManager::get_window_width() / 2/*)*/;
    double center_y = /*floor(*/RenderManager::get_window_height() / 2/*)*/;
    // TODO: put PI in a sensible place
    const double PI = 3.141592653589793238463;
	

    GLFWwindow* win = RenderManager::window;
    glfwGetCursorPos(win, &mouse_x, &mouse_y);

    //this is inefficient, because the center is calculated twice
    //move_cursor_to_center(); 
	glfwSetCursorPos(win, center_x, center_y);
	glfwGetCursorPos(win, &mouse_x1, &mouse_y1);

    double angle_ver = Configuration::mouse_speed * (center_y - mouse_y);
    altitude += angle_ver;
    altitude = fmin(PI/2, fmax(-PI/2, altitude)); //clamp

	LogManager::log_info(std::to_string(mouse_x1) ,2);
	LogManager::log_info(std::to_string(mouse_y1) ,2);

	LogManager::log_info(std::to_string(mouse_x) ,2);
	LogManager::log_info(std::to_string(mouse_y) ,2);
	LogManager::log_info(std::to_string(center_x) ,2);
	LogManager::log_info(std::to_string(center_y) ,2);
	LogManager::log_info(std::to_string(angle_ver) ,2);
	LogManager::log_info(std::to_string(altitude) ,2);

    double angle_hor = Configuration::mouse_speed * (mouse_x - center_x);
    shoulders.rotate_right(angle_hor);

    return; 
}
/** \brief Handles rift input
 *  \param deltatime as a double
 *  \return void
 */
void CharacterManager::handle_rift(double dt) {
    // Query the HMD for the current tracking state.
    ovrTrackingState ts  = ovrHmd_GetTrackingState(*_hmd, ovr_GetTimeInSeconds());
    if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)) {
        ovrQuatf orientation = ts.HeadPose.ThePose.Orientation;
        rift_orientation = glm::dquat(orientation.w, orientation.x, orientation.y, orientation.z);
    }
}
/** \brief Get left eye position (needs fixed)
 *  \param void
 *  \return frame of the eye position
 */
frame CharacterManager::get_position_left_eye() {
    // TODO: fix this
    frame result = shoulders;

    glm::dvec4 newpos = hypermath::exp(shoulders.pos, -Configuration::ipd * 0.5 * meter * shoulders.right);
    glm::dmat4 transf = hypermath::translation(shoulders.pos,newpos);

    result = transf * result;

    if (rift_input && _hmd != NULL) {
        glm::dmat4 eye_orientation = hypermath::rotation0(rift_orientation);
        
        result = eye_orientation * result;
    }
    return result;
}
/** \brief Get right eye position (needs fixed)
 *  \param void
 *  \return frame of the eye position
 */
frame CharacterManager::get_position_right_eye() {
    // TODO: fix this
    frame result = shoulders;
    
    glm::dvec4 newpos = hypermath::exp(shoulders.pos, Configuration::ipd * 0.5 * meter * shoulders.right);
    glm::dmat4 transf = hypermath::translation(shoulders.pos,newpos);

    result = transf * result;

    if (rift_input && _hmd != NULL) {
        glm::dmat4 eye_orientation = hypermath::rotation0(rift_orientation);

        result = eye_orientation * result;
    }
    return result;
}
/** \brief Get eye positions
 *  \param void
 *  \return frame of the eye position
 */
frame CharacterManager::get_position_eyes() {
    // TODO: add character height
    frame result = shoulders;
    result.rotate_up(altitude);

    if (rift_input && _hmd != NULL) {
        glm::dmat4 eye_orientation = hypermath::rotation0(rift_orientation);

        result = eye_orientation * result;
    }

    return result;
}

/**
 * \brief Moves the character to the origin and faces them forward
 * \param void
 * \return void
 */
void CharacterManager::reset_to_origin() {
    shoulders.pos = glm::dvec4(0, 0, 0, 1);
    shoulders.forward = glm::dvec4(0,0,-1,0);
    shoulders.right = glm::dvec4(1,0,0,0);
    shoulders.up = glm::dvec4(0,1,0,0);
}

/**
 * \brief "Grows" the character by making a meter larger - defaults to an increase by 1.1
 * \param scale as a double
 * \return void
 */
void CharacterManager::grow(double scale) {
    meter *= scale;
}

/**
 * \brief "Shrinks" the character by making a meter smaller - defaults to a decrease by 1.1
 * \param scale as a double
 * \return void
 */
void CharacterManager::shrink(double scale) {
    meter /= scale;
}

/** \brief Moves the mouse cursor to the center of the window
 *  \param void
 *  \return void
 */
void CharacterManager::move_cursor_to_center() {
    double center_x = /*floor(*/RenderManager::get_window_width() / 2/*)*/;
    double center_y = /*floor(*/RenderManager::get_window_height() / 2/*)*/;
    GLFWwindow* win = RenderManager::window;
    glfwSetCursorPos(win, center_x, center_y);
}

