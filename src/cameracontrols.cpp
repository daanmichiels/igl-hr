#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "hypermath.h"
#include "scene.h"
#include "cameracontrols.h"
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtx/transform.hpp"
#include "../thirdparty/glm/glm/gtc/quaternion.hpp"
#include <iostream>
#include "../thirdparty/glm/glm/gtx/string_cast.hpp"


CameraControls::CameraControls(GLFWwindow* window, Camera* camera)
{
    _window = window;
    _camera = camera;
    _pos = glm::vec4(0,0,0,1);
    _up = glm::vec4(0,1,0,0);
    _forward = glm::vec4(0,0,-1,0);
    _right = glm::vec4(1,0,0,0);
}

void CameraControls::handle(float delta_time, int width, int height)
{
    handle_keyboard(delta_time);
    if(bind_mouse)
    {
        handle_mouse(delta_time, width, height);
    }
}

void CameraControls::handle_mouse(float delta_time, int width, int height)
{
    double mouse_x, mouse_y;
    double center_x = floor(width/2);
    double center_y = floor(height/2);

    glfwGetCursorPos(_window, &mouse_x, &mouse_y);
    glfwSetCursorPos(_window, center_x, center_y);

    float angle_x = _mouse_speed * delta_time * (center_y - mouse_y);
    float angle_y = _mouse_speed * delta_time * (center_x - mouse_x);

    //first rotate over x
    glm::vec4 forward2 = ((float)sin(angle_x)) * _up + ((float)cos(angle_x)) * _forward;
    _up = ((float)cos(angle_x)) * _up - ((float)sin(angle_x)) * _forward;
    _forward = forward2;

    //now rotate over y
    glm::vec4 right2 = ((float)cos(angle_y)) * _right + ((float)sin(angle_y)) * _forward;
    _forward = -1*((float)sin(angle_y)) * _right + ((float)cos(angle_y)) * _forward;
    _right = right2;

    update_camera_transformation();

    return; 
}

void CameraControls::update_camera_transformation()
{
    glm::mat4 transf = glm::mat4();

    transf[0][0] = _right.x;
    transf[0][1] = _right.y;
    transf[0][2] = _right.z;
    transf[0][3] = _right.w;

    transf[1][0] = _up.x;
    transf[1][1] = _up.y;
    transf[1][2] = _up.z;
    transf[1][3] = _up.w;

    transf[2][0] = -1*_forward.x;
    transf[2][1] = -1*_forward.y;
    transf[2][2] = -1*_forward.z;
    transf[2][3] = -1*_forward.w;

    transf[3][0] = _pos.x;
    transf[3][1] = _pos.y;
    transf[3][2] = _pos.z;
    transf[3][3] = _pos.w;

    transf = glm::inverse(transf);
    _camera->set_transformation(transf);
}

void CameraControls::handle_keyboard(float delta_time)
{
    if( glfwGetKey(_window, GLFW_KEY_UP) || glfwGetKey(_window, GLFW_KEY_W ))
    {
        glm::vec4 newpos = hypermath::exp(_pos, 0.2f * ((float)delta_time) * _forward);
        glm::mat4 transf = hypermath::translation(_pos,newpos);
        _pos = newpos;
        _up = transf * _up;
        _right = transf * _right;
        _forward = transf * _forward;
        update_camera_transformation();
    }
    if( glfwGetKey(_window, GLFW_KEY_DOWN) || glfwGetKey(_window, GLFW_KEY_S ))
    {
        glm::vec4 newpos = hypermath::exp(_pos, -0.2f * ((float)delta_time) * _forward);
        glm::mat4 transf = hypermath::translation(_pos,newpos);
        _pos = newpos;
        _up = transf * _up;
        _right = transf * _right;
        _forward = transf * _forward;
        update_camera_transformation();
    }    
    if( glfwGetKey(_window, GLFW_KEY_LEFT) || glfwGetKey(_window, GLFW_KEY_A ))
    {
        glm::vec4 newpos = hypermath::exp(_pos, -0.2f * ((float)delta_time) * _right);
        glm::mat4 transf = hypermath::translation(_pos,newpos);
        _pos = newpos;
        _up = transf * _up;
        _right = transf * _right;
        _forward = transf * _forward;
        update_camera_transformation();
    }
    if( glfwGetKey(_window, GLFW_KEY_RIGHT) || glfwGetKey(_window, GLFW_KEY_D ))
    {
        glm::vec4 newpos = hypermath::exp(_pos, 0.2f * ((float)delta_time) * _right);
        glm::mat4 transf = hypermath::translation(_pos,newpos);
        _pos = newpos;
        _up = transf * _up;
        _right = transf * _right;
        _forward = transf * _forward;
        update_camera_transformation();
    }
    if( glfwGetKey(_window, GLFW_KEY_PAGE_UP) || glfwGetKey(_window, GLFW_KEY_R))
    {
        glm::vec4 newpos = hypermath::exp(_pos, 0.2f * ((float)delta_time) * _up);
        glm::mat4 transf = hypermath::translation(_pos,newpos);
        _pos = newpos;
        _up = transf * _up;
        _right = transf * _right;
        _forward = transf * _forward;
        update_camera_transformation();
    }
    if( glfwGetKey(_window, GLFW_KEY_PAGE_DOWN) || glfwGetKey(_window, GLFW_KEY_F))
    {
        glm::vec4 newpos = hypermath::exp(_pos, -0.2f * ((float)delta_time) * _up);
        glm::mat4 transf = hypermath::translation(_pos,newpos);
        _pos = newpos;
        _up = transf * _up;
        _right = transf * _right;
        _forward = transf * _forward;
        update_camera_transformation();
    }
}

/*
void CameraControls::set_mouse_speed(float speed)
{
    _mouse_speed = speed;
}

glm::vec4 CameraControls::get_pos()
{
    return _pos;
}
*/

