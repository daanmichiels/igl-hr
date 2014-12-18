#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

#include <GLFW/glfw3.h>
#include "../data/camera.h"
#include "../data/frame.h"
#include "../../thirdparty/glm/glm/glm.hpp"
#include "../../thirdparty/glm/glm/gtc/quaternion.hpp"
#include "../../thirdparty/libovr/src/OVR_CAPI.h"
#include "OVR/OVR.h"

class CameraControls
{
    private:
        GLFWwindow* _window;
        Camera* _camera;
        ovrHmd* _hmd;
        double _mouse_speed = 0.3;
        double _move_speed = 0.2;
        double _ipd = 0.065;
        double _height = 1.80;
        double _meter = 1.0;
        frame _shoulders;
        frame _head;

        void handle_mouse(double delta_time, int width, int height);
        void handle_keyboard(double delta_time);
        void handle_hmd();
        void update_camera_transformation();

    public:
        glm::dvec2 left_lens_center = glm::dvec2(0.15,0);
        glm::dvec2 right_lens_center = glm::dvec2(-0.15,0);

        CameraControls(GLFWwindow* window, Camera* camera, ovrHmd* hmd);
        bool bind_mouse = false;
        void move_right(double distance);
        void handle(double delta_time, int width, int height);
        void set_mouse_speed(double speed);
        void set_step_size(double size);
        void set_orientation(glm::dquat rotation);
        glm::dvec4 get_pos();
        void increase_speed();
        void decrease_speed();
        void grow(double factor);
        void shrink(double factor);
        glm::dmat4 get_cam_view();
        glm::dvec4 get_forward();
        void reset_to_origin();
        double get_ipd();
        double get_height();
        double get_meter();
        glm::dvec4 get_flag_pos();

};

#endif
