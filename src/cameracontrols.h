#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

#include <GLFW/glfw3.h>
#include "camera.h"
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/quaternion.hpp"
#include "../thirdparty/libovr/src/OVR_CAPI.h"
#include "../thirdparty/libovr/include/OVR.h"

class CameraControls
{
    private:
        Camera* _camera;
        ovrHmd* _hmd;
        float _mouse_speed = 0.3f;
        float _move_speed = 0.2f;
        float _ipd = 0.065f;
        float _meter = 1.0f;
        glm::vec4 _pos;
        glm::vec4 _up, _forward, _right,_sforward,_sright;
        void update_camera_transformation();
        int quatX, quatY, quatZ, quatW;

    public:
        GLFWwindow* _window;
        glm::vec2 left_lens_center = glm::vec2(0.15,0);
        glm::vec2 right_lens_center = glm::vec2(-0.15,0);

        CameraControls(GLFWwindow* window, Camera* camera, ovrHmd* hmd);
        bool bind_mouse = false;
        void move_right(float distance);
        void handle(float delta_time, int width, int height);
        void handle_mouse(float delta_time, int width, int height);
        void handle_keyboard(float delta_time);
        void handle_hmd();
        void printShoulder();
        void set_orientation(glm::quat rotation);
        void set_mouse_speed(float speed);
        void set_step_size(float size);
        void grow(float factor);
        void shrink(float factor);
        float get_ipd();
        float get_meter();
        glm::vec4 get_pos();
        void setQuatIndices(int x, int y, int z, int w);
};

#endif
