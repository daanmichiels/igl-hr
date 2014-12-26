
#ifndef CHARACTERMANAGER_H
#define CHARACTERMANAGER_H

#include "../data/frame.h"

class CharacterManager {
public:
    static bool startup();
    static void shutdown();
    static void handle(double dt);

    static void bind_mouse();
    static void unbind_mouse();
    static bool is_mouse_bound();

    static double meter;

    static frame get_position_left_eye();
    static frame get_position_right_eye();
    static frame get_position_eyes();
private:
    static bool rift_input;
    static bool mouse_bound;
    static frame shoulders;
    static double altitude; //in the sense of http://en.wikipedia.org/wiki/Horizontal_coordinate_system

    static void handle_keyboard(double dt);
    static void handle_mouse(double dt);
    static void handle_rift(double dt);
    
    static void move_cursor_to_center();
};

#endif

