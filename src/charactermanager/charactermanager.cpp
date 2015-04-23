#if defined(_WIN32)
 #define GLFW_EXPOSE_NATIVE_WIN32
 #define GLFW_EXPOSE_NATIVE_WGL
 #define OVR_OS_WIN32
#elif defined(__APPLE__)
 #define GLFW_EXPOSE_NATIVE_COCOA
 #define GLFW_EXPOSE_NATIVE_NSGL
 #define OVR_OS_MAC
#elif defined(__linux__)
 #define GLFW_EXPOSE_NATIVE_X11
 #define GLFW_EXPOSE_NATIVE_GLX
 #define OVR_OS_LINUX
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <OVR_CAPI_GL.h>

#include "charactermanager.h"
#include "../logmanager/logmanager.h"
#include "../rendermanager/rendermanager.h"
#include "../configuration/configuration.h"
#include <GL/glew.h>
#include "glm/gtc/quaternion.hpp"
#include "glm/ext.hpp"

frame CharacterManager::feet = frame();
bool CharacterManager::rift_input = Configuration::rift_input == OnOffAuto::on;
bool CharacterManager::mouse_bound = false;
double CharacterManager::meter = 0.0012;
double CharacterManager::altitude = 0.0;
glm::dquat CharacterManager::rift_orientation = glm::dquat(0,0,0,0);
ovrHmd CharacterManager::_hmd = NULL;

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

    feet.correct_roundoff();
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
    glfwSetInputMode(RenderManager::window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

/** \brief Unbinds the mouse from the window
 *  \param void
 * \return void
 */
void CharacterManager::unbind_mouse() {
    mouse_bound = false;
    // TODO: make cursor visible
    glfwSetInputMode(RenderManager::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

/** \brief Check if the mouse is bound
 *  \param void
 * \return void
 */
bool CharacterManager::is_mouse_bound() {
    return mouse_bound;
}

void CharacterManager::set_hmd(ovrHmd hmd) {
    _hmd = hmd;
    rift_input = hmd != NULL;
}

/** \brief Start the character manager. Sets the shoulders up, moves the mouse to center, and logs starting of character
 *  manager
 *  \param void
 * \return boolean (true)
 */
bool CharacterManager::startup() {
    // TODO: read the initial position from the level data instead
    feet.pos = glm::dvec4(0,0,0,1);
    feet.right = glm::dvec4(1,0,0,0);
    feet.up = glm::dvec4(0,1,0,0);
    feet.forward = glm::dvec4(0,0,-1,0);

    move_cursor_to_center();
    bind_mouse();

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
        walking_direction += glm::dvec3(0,0,-1);
    }
    if( glfwGetKey(win, GLFW_KEY_DOWN) || glfwGetKey(win, GLFW_KEY_S )) {
        walking_direction += glm::dvec3(0,0,1);
    }    
    if( glfwGetKey(win, GLFW_KEY_LEFT) || glfwGetKey(win, GLFW_KEY_A )) {
        walking_direction += glm::dvec3(-1,0,0);
    }
    if( glfwGetKey(win, GLFW_KEY_RIGHT) || glfwGetKey(win, GLFW_KEY_D )) {
        walking_direction += glm::dvec3(1,0,0);
    }

    //length is either zero or at least one, mathematically
    //but floating point
    if(glm::length(walking_direction) >= 0.5)
    {
        walking_direction = glm::normalize(walking_direction);
        if(glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(win, GLFW_KEY_LEFT_SHIFT)) {
            walking_direction *= Configuration::running_speed * dt * meter;
        } else {
            walking_direction *= Configuration::walking_speed * dt * meter;
        }
        glm::dvec4 newpos = hypermath::exp(feet.pos, walking_direction.x*feet.right + walking_direction.y*feet.up + walking_direction.z*feet.forward);
        glm::dmat4 transf = hypermath::translation(feet.pos,newpos);

        feet = transf * feet;
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

    double angle_hor = Configuration::mouse_speed * (center_x - mouse_x);
    feet.rotate_right(angle_hor);

    return; 
}
/** \brief Handles rift input
 *  \param deltatime as a double
 *  \return void
 */
void CharacterManager::handle_rift(double dt) {
    // Query the HMD for the current tracking state.
    ovrTrackingState ts  = ovrHmd_GetTrackingState(_hmd, ovr_GetTimeInSeconds());
    if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)) {
        ovrQuatf orientation = ts.HeadPose.ThePose.Orientation;
        rift_orientation = glm::dquat(orientation.w, orientation.x, orientation.y, orientation.z);
    }
}


glm::dvec4 CharacterManager::get_position_feet() {
    return feet.pos;
}

/** \brief Get left eye position (needs fixed)
 *  \param void
 *  \return frame of the eye position
 */
frame CharacterManager::get_position_left_eye() {
    // TODO: fix this
    frame result = feet;

    glm::dvec4 newpos_up = hypermath::exp(result.pos, Configuration::eye_height * meter * result.up);
    glm::dmat4 transf_up = hypermath::translation(result.pos,newpos_up);

    result = transf_up * result;

    if (rift_input) {
        glm::dmat3 eye_orientation = glm::mat3_cast(rift_orientation);
        
        result = eye_orientation * result;

        //if getting input from the rift, rotate first, then move left
        glm::dvec4 newpos_right = hypermath::exp(result.pos, -Configuration::ipd * 0.5 * meter * result.right);
        glm::dmat4 transf_right = hypermath::translation(result.pos,newpos_right);

        result = transf_right * result;
    }
    else {        
        //if not getting rift input move right then rotate (does this order make sense?)
        glm::dvec4 newpos_right = hypermath::exp(result.pos, -Configuration::ipd * 0.5 * meter * result.right);
        glm::dmat4 transf_right = hypermath::translation(result.pos,newpos_right);

        result = transf_right * result;

        result.rotate_up(altitude);
    }

    return result;
}
/** \brief Get right eye position (needs fixed)
 *  \param void
 *  \return frame of the eye position
 */
frame CharacterManager::get_position_right_eye() {
    // TODO: fix this
    frame result = feet;
    
    glm::dvec4 newpos_up = hypermath::exp(result.pos, Configuration::eye_height * meter * result.up);
    glm::dmat4 transf_up = hypermath::translation(result.pos,newpos_up);

    result = transf_up * result;

    if (rift_input) {
        glm::dmat3 eye_orientation = glm::mat3_cast(rift_orientation);
        
        result = eye_orientation * result;

        //if getting input from the rift, rotate first, then move right
        glm::dvec4 newpos_right = hypermath::exp(result.pos, Configuration::ipd * 0.5 * meter * result.right);
        glm::dmat4 transf_right = hypermath::translation(result.pos,newpos_right);

        result = transf_right * result;
    }
    else {
        //if not getting rift input move right then rotate (does this order make sense?)
        glm::dvec4 newpos_right = hypermath::exp(result.pos, Configuration::ipd * 0.5 * meter * result.right);
        glm::dmat4 transf_right = hypermath::translation(result.pos,newpos_right);

        result = transf_right * result;

        result.rotate_up(altitude);
    }

    return result;
}
/** \brief Get eye positions
 *  \param void
 *  \return frame of the eye position
 */
frame CharacterManager::get_position_eyes() {
    frame result = feet;
    glm::dvec4 newpos = hypermath::exp(result.pos, Configuration::eye_height * meter * result.up);
    result = hypermath::translation(result.pos, newpos) * result;

    if (rift_input) {
        glm::dmat3 eye_orientation = glm::mat3_cast(rift_orientation);
        
        result = eye_orientation * result;
    }
    else {
        result.rotate_up(altitude);
    }

    return result;
}

/**
 * \brief Moves the character to the origin and faces them forward
 * \param void
 * \return void
 */
void CharacterManager::reset_to_origin() {
    feet.pos = glm::dvec4(0, 0, 0, 1);
    feet.forward = glm::dvec4(0,0,-1,0);
    feet.right = glm::dvec4(1,0,0,0);
    feet.up = glm::dvec4(0,1,0,0);
    altitude = 0.0;
    meter = 0.0012;
    RenderManager::handle_scale_change();
}

/**
 * \brief "Scales" the character by making a meter a different size
 * \param scale as a double
 * \return void
 */
void CharacterManager::scale(double scale) {
    meter = fmax(0.00001, fmin(2, meter * scale));
}

/** 
 *  \brief Moves the mouse cursor to the center of the window
 *  \param void
 *  \return void
 */
void CharacterManager::move_cursor_to_center() {
    double center_x = /*floor(*/RenderManager::get_window_width() / 2/*)*/;
    double center_y = /*floor(*/RenderManager::get_window_height() / 2/*)*/;
    GLFWwindow* win = RenderManager::window;
    glfwSetCursorPos(win, center_x, center_y);
}

/*
   //What does this do?
void CharacterManager::update_shoulder_frame() {
    //calculate the exact up vector at shoulder position
    glm::dvec4 exact_up = hypermath::translation(glm::dvec4(0,0,0,1), shoulders.pos) * glm::dvec4(0,1,0,0);

    shoulders.up = exact_up;
    shoulders.correct_roundoff();
}
*/

