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
        float _mouse_speed = 0.3f;
        float _move_speed = 0.2f;
        float _eye_width = 0.001f;
        bool _left_eye = true;
		glm::vec4 _pos;
        glm::vec4 _up, _forward, _right, _sforward,_sright;
        void update_camera_transformation();

    public:
        CameraControls(GLFWwindow* window, Camera* camera);
        bool bind_mouse = true;

void CameraControls::switchEye()
{
    _left_eye = !_left_eye;
    glm::vec4 newpos = hypermath::exp(_pos, (_left_eye ? _eye_width : -_eye_width) * _right);
    glm::mat4 transf = hypermath::translation(_pos,newpos);
    _pos = newpos;
    _up = transf * _up;
    _right = transf * _right;
    _forward = transf * _forward;
    update_camera_transformation();
}
        void handle(float delta_time, int width, int height);
        void handle_mouse(float delta_time, int width, int height);
        void handle_keyboard(float delta_time);
        void set_mouse_speed(float speed);
        void set_step_size(float size);
        glm::vec4 get_pos();
};

#endif
