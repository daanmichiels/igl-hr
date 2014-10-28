#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

#include <GLFW/glfw3.h>
#include "camera.h"
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/quaternion.hpp"
#include "../thirdparty/LibOVR/Src/OVR_CAPI.h"
#include "../thirdparty/LibOVR/Include/OVR.h"

class CameraControls
{
    private:
        GLFWwindow* _window;
        Camera* _camera;
        ovrHmd* _hmd;
        float _mouse_speed = 0.3f;
        float _move_speed = 0.2f;
        float _eye_width = 0.001f;
        float _eye_width_step = 0.00001f;
        glm::vec4 _pos;
        glm::vec4 _up, _forward, _right,_sforward,_sright;
        void update_camera_transformation();
        int quatX, quatY, quatZ, quatW;

    public:
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
        float get_eye_width();
        glm::vec4 get_pos();
        void setQuatIndices(int x, int y, int z, int w);
};

#endif
