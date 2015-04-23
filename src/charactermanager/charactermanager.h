
#ifndef CHARACTERMANAGER_H
#define CHARACTERMANAGER_H

#include "../data/frame.h"
#include "glm/glm.hpp"
#include "OVR_CAPI.h"

class CharacterManager {
public:
    static bool startup();
    static void shutdown();
    static void handle(double dt);

    static void bind_mouse();
    static void unbind_mouse();
    static bool is_mouse_bound();

    static void set_hmd(ovrHmd hmd);

    static double meter;

    //TODO: rename the first three to get_frame_xxx
    static frame get_position_left_eye();
    static frame get_position_right_eye();
    static frame get_position_eyes();
    static glm::dvec4 get_position_feet();

    static void reset_to_origin();

    static void scale(double scale);
private:
    static bool rift_input;
    static bool mouse_bound;
    static frame feet;
    static double altitude; //in the sense of http://en.wikipedia.org/wiki/Horizontal_coordinate_system
    static glm::dquat rift_orientation;
    static ovrHmd _hmd;

    static void handle_keyboard(double dt);
    static void handle_mouse(double dt);
    static void handle_rift(double dt);
    
    static void move_cursor_to_center();
};

#endif

