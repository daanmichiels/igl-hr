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
#include <math.h>

//should that be here?
const double PI  =3.141592653589793238463;

CameraControls::CameraControls(GLFWwindow* window, Camera* camera)
{
    _window = window;
    _camera = camera;
    _pos = glm::vec4(0,0,0,1);

    //shoulder orientation
    _sup = glm::vec4(0,1,0,0);
    _sforward = glm::vec4(0,0,-1,0);
    _sright = glm::vec4(1,0,0,0);
}

void CameraControls::handle(float delta_time, int width, int height)
{
    handle_keyboard(delta_time);
    if(bind_mouse)
    {
        handle_mouse(delta_time, width, height);
    }
    update_camera_transformation();
}

void CameraControls::handle_mouse(float delta_time, int width, int height)
{
    double mouse_x, mouse_y;
    double center_x = floor(width/2);
    double center_y = floor(height/2);

    glfwGetCursorPos(_window, &mouse_x, &mouse_y);
    glfwSetCursorPos(_window, center_x, center_y);

    _angle_ver += _mouse_speed * delta_time * (center_y - mouse_y);
    _angle_ver = fmin(PI/2, fmax(-PI/2, _angle_ver));

    double angle_hor = _mouse_speed * delta_time * (center_x - mouse_x);
    _sforward = ((float)cos(angle_hor))*_sforward - ((float)sin(angle_hor))*_sright;
    _sright = ((float)sin(angle_hor))*_sforward + ((float)cos(angle_hor))*_sright;

    return; 
}

void CameraControls::update_camera_transformation()
{
    glm::vec4 up, forward, right;
    right = _sright;
    forward = ((float)cos(_angle_ver))*_sforward + ((float)sin(_angle_ver))*_sup;
    up = ((float)cos(_angle_ver))*_sup - ((float)sin(_angle_ver))*_sforward;

    glm::mat4 transf = glm::mat4();
    transf[0][0] = right.x;
    transf[0][1] = right.y;
    transf[0][2] = right.z;
    transf[0][3] = right.w;

    transf[1][0] = up.x;
    transf[1][1] = up.y;
    transf[1][2] = up.z;
    transf[1][3] = up.w;

    transf[2][0] = -1*forward.x;
    transf[2][1] = -1*forward.y;
    transf[2][2] = -1*forward.z;
    transf[2][3] = -1*forward.w;

    transf[3][0] = _pos.x;
    transf[3][1] = _pos.y;
    transf[3][2] = _pos.z;
    transf[3][3] = _pos.w;
    //potentially change this so that we calculate inverse ourselves. Slightly faster maybe.
    transf = glm::inverse(transf);
    _camera->set_transformation(transf);
}

void CameraControls::handle_keyboard(float delta_time)
{
    //direction of walking in coordinates (right, up, forward)
    glm::vec3 walking_direction = glm::vec3();
    if( glfwGetKey(_window, GLFW_KEY_UP) || glfwGetKey(_window, GLFW_KEY_W ))
    {
        walking_direction += glm::vec3(0,0,1);
    }
    if( glfwGetKey(_window, GLFW_KEY_DOWN) || glfwGetKey(_window, GLFW_KEY_S ))
    {
        walking_direction += glm::vec3(0,0,-1);
    }    
    if( glfwGetKey(_window, GLFW_KEY_LEFT) || glfwGetKey(_window, GLFW_KEY_A ))
    {
        walking_direction += glm::vec3(-1,0,0);
    }
    if( glfwGetKey(_window, GLFW_KEY_RIGHT) || glfwGetKey(_window, GLFW_KEY_D ))
    {
        walking_direction += glm::vec3(1,0,0);
    }
    if( glfwGetKey(_window, GLFW_KEY_PAGE_UP) || glfwGetKey(_window, GLFW_KEY_R))
    {
        walking_direction += glm::vec3(0,1,0);
    }
    if( glfwGetKey(_window, GLFW_KEY_PAGE_DOWN) || glfwGetKey(_window, GLFW_KEY_F))
    {
        walking_direction += glm::vec3(0,-1,0);
    }

    //length is either zero or at least one, mathematically
    //but floating point
    if(glm::length(walking_direction) >= 0.5)
    {
        walking_direction = glm::normalize(walking_direction);
        walking_direction *= _move_speed * delta_time;
        glm::vec4 newpos = hypermath::exp(_pos, walking_direction.x*_sright + walking_direction.y*_sup + walking_direction.z*_sforward);
        glm::mat4 transf = hypermath::translation(_pos,newpos);
        _pos = newpos;
        _sup = transf * _sup;
        _sright = transf * _sright;
        _sforward = transf * _sforward;
    }
    if( glfwGetKey(_window, GLFW_KEY_GRAVE_ACCENT))
    {
        _pos = glm::vec4(0,0,0,1);
        _sforward = glm::vec4(0,0,-1,0);
        _sright = glm::vec4(1,0,0,0);
        _sup = glm::vec4(0,1,0,0);
        _angle_ver = 0.0;
    }
}


void CameraControls::set_mouse_speed(float speed)
{
    _mouse_speed = speed;
}

void CameraControls::set_step_size(float size)
{
    _move_speed = size;
}

glm::vec4 CameraControls::get_pos()
{
    return _pos;
}
//increase step size. max is 1.0f
void CameraControls::increase_speed()
{
    float size = _move_speed + .05f;
    if(size > 1.0f)
    {
        size = 1.0f;
    }
    set_step_size(size);
}
//decrease step size. minimum is 0.01f
void CameraControls::decrease_speed()
{
    float size = _move_speed - .05f;
    if(size <= 0)
    {
        size = 0.01f;
    }

    set_step_size(size);
}
