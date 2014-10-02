#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

#include <GLFW/glfw3.h>
#include "camera.h"

class CameraControls
{
    private:
        GLFWwindow* _window;
        Camera* _camera;
        float _mouse_speed = 0.05;

        // These can be used later to recompute the view matrix
        // every time (to prevent round-off from transforming the
        // camera every time).
        // glm::vec4 _cam_pos;
        // glm::quat _cam_orientation;
    public:
        CameraControls(GLFWwindow* window, Camera* camera);
        void handle(float delta_time, int width, int height);
        void handle_mouse(float delta_time, int width, int height);
        void handle_keyboard(float delta_time);
        void set_mouse_speed(float speed);
};

#endif

