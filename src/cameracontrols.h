#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

#include <GLFW/glfw3.h>
#include "camera.h"
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/quaternion.hpp"

class CameraControls
{
    private:
        GLFWwindow* _window;
        Camera* _camera;
        float _mouse_speed = 0.3;
		glm::vec4 _pos;
        glm::vec4 _up, _forward, _right;
        void update_camera_transformation();

    public:
        CameraControls(GLFWwindow* window, Camera* camera);
        bool bind_mouse = true;
        void handle(float delta_time, int width, int height);
        void handle_mouse(float delta_time, int width, int height);
        void handle_keyboard(float delta_time);
        //void set_mouse_speed(float speed);
        //glm::vec4 get_pos();
};

#endif
