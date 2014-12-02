#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

#include <GLFW/glfw3.h>
#include "camera.h"
#include "frame.h"
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/quaternion.hpp"
#include "../thirdparty/libovr/src/OVR_CAPI.h"
#include "../thirdparty/libovr/include/OVR.h"

class CameraControls
{
    private:
        GLFWwindow* _window;
        Camera* _camera;
        ovrHmd* _hmd;
        float _mouse_speed = 0.3f;
        float _move_speed = 0.2f;
        float _ipd = 0.065f;
        float _height = 1.80;
        float _meter = 1.0f;
        frame _shoulders;
        frame _head;

        void handle_mouse(float delta_time, int width, int height);
        void handle_keyboard(float delta_time);
        void handle_hmd();
        void update_camera_transformation();

    public:
        glm::vec2 left_lens_center = glm::vec2(0.15,0);
        glm::vec2 right_lens_center = glm::vec2(-0.15,0);

        CameraControls(GLFWwindow* window, Camera* camera, ovrHmd* hmd);
        bool bind_mouse = false;
        void move_right(float distance);
        void handle(float delta_time, int width, int height);
        void set_mouse_speed(float speed);
        void set_step_size(float size);
        void set_orientation(glm::quat rotation);
        glm::vec4 get_pos();
        void increase_speed();
        void decrease_speed();
        void grow(float factor);
        void shrink(float factor);
        glm::mat4 get_cam_view();
        glm::vec4 get_forward();
        void reset_to_origin();
        float get_ipd();
        float get_height();
        float get_meter();
        glm::vec4 get_flag_pos();

};

#endif
