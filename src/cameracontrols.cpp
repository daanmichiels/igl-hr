#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "hypermath.h"
#include "scene.h"
#include "cameracontrols.h"
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtx/transform.hpp"
#include "../thirdparty/glm/glm/gtc/quaternion.hpp"
#include "../thirdparty/glm/glm/gtx/string_cast.hpp"
#include <math.h>
#include <iostream>

using namespace OVR;

CameraControls::CameraControls(GLFWwindow* window, Camera* camera, ovrHmd* hmd)
{
    _window = window;
    _camera = camera;
    _hmd = hmd;
    reset_to_origin();
}

void CameraControls::handle(float delta_time, int width, int height)
{
    handle_keyboard(delta_time);
    if(bind_mouse)
    {
        handle_mouse(delta_time, width, height);
    }
    handle_hmd();
    _shoulders.correct_roundoff();
    _head.correct_roundoff();
    update_camera_transformation();
}

void CameraControls::handle_mouse(float delta_time, int width, int height)
{
    double mouse_x, mouse_y;
    double center_x = floor(width/2);
    double center_y = floor(height/2);
    double PI = 3.141592653589793238463;

    glfwGetCursorPos(_window, &mouse_x, &mouse_y);
    glfwSetCursorPos(_window, center_x, center_y);

    double _angle_ver = _mouse_speed * delta_time * (center_y - mouse_y);
    _head.up = ((float)cos(_angle_ver))*_head.up - ((float)sin(_angle_ver))*_head.forward;

    double angle_hor = _mouse_speed * delta_time * (center_x - mouse_x);
    _shoulders.forward = ((float)cos(angle_hor))*_shoulders.forward - ((float)sin(angle_hor))*_shoulders.right;
    _shoulders.right = ((float)sin(angle_hor))*_shoulders.forward + ((float)cos(angle_hor))*_shoulders.right;

    _head.forward = _shoulders.forward;
    _head.right = _shoulders.right;

    return; 
}

void CameraControls::handle_hmd()
{
    // Query the HMD for the current tracking state.
    ovrTrackingState ts  = ovrHmd_GetTrackingState(*_hmd, ovr_GetTimeInSeconds());
    if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)) {
        ovrQuatf orientation = ts.HeadPose.ThePose.Orientation;
        set_orientation(glm::quat(orientation.w, orientation.x, orientation.y, orientation.z));
    }
}

void CameraControls::update_camera_transformation()
{
    glm::mat4 transf = glm::mat4();
    transf[0][0] = _head.right.x;
    transf[0][1] = _head.right.y;
    transf[0][2] = _head.right.z;
    transf[0][3] = _head.right.w;

    transf[1][0] = _head.up.x;
    transf[1][1] = _head.up.y;
    transf[1][2] = _head.up.z;
    transf[1][3] = _head.up.w;

    transf[2][0] = -1*_head.forward.x;
    transf[2][1] = -1*_head.forward.y;
    transf[2][2] = -1*_head.forward.z;
    transf[2][3] = -1*_head.forward.w;

    transf[3][0] = _shoulders.pos.x;
    transf[3][1] = _shoulders.pos.y;
    transf[3][2] = _shoulders.pos.z;
    transf[3][3] = _shoulders.pos.w;
    //potentially change this so that we calculate inverse ourselves. Slightly faster maybe.
    transf = glm::inverse(transf);
    _camera->set_transformation(transf);
}

void CameraControls::move_right(float distance)
{
    glm::vec4 newpos = hypermath::exp(_shoulders.pos, distance * _head.right);
    glm::mat4 transf = hypermath::translation(_shoulders.pos,newpos);
    _shoulders.pos = newpos;
    _shoulders.right = transf * _shoulders.right;
    _shoulders.forward = transf * _shoulders.forward;
    _head.up = transf * _head.up;
    _head.forward = transf * _head.forward;
    _head.right = transf * _head.right;
    update_camera_transformation();
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
    if ( glfwGetKey(_window, GLFW_KEY_GRAVE_ACCENT))
    {
        reset_to_origin();
    }

    //length is either zero or at least one, mathematically
    //but floating point
    if(glm::length(walking_direction) >= 0.5)
    {
        walking_direction = glm::normalize(walking_direction);
        walking_direction *= _move_speed * delta_time * _meter;
        glm::vec4 newpos = hypermath::exp(_shoulders.pos, walking_direction.x*_shoulders.right + walking_direction.y*_shoulders.up + walking_direction.z*_shoulders.forward);
        glm::mat4 transf = hypermath::translation(_shoulders.pos,newpos);
        _shoulders.pos = newpos;

        // This makes movement weird 
        // _shoulders.up = -hypermath::gravity(_shoulders.pos);
        _shoulders.up = transf * _shoulders.up;
        _shoulders.right = transf * _shoulders.right;
        _shoulders.forward = transf * _shoulders.forward;
    }
}

void CameraControls::reset_to_origin()
{

    _shoulders.pos = glm::vec4(0,0,0,1);
    _shoulders.forward = glm::vec4(0,0,-1,0);
    _shoulders.right = glm::vec4(1,0,0,0);
    _shoulders.up = glm::vec4(0,1,0,0);

    _head.pos = glm::vec4(0,0,0,1);
    _head.forward = glm::vec4(0,0,-1,0);
    _head.right = glm::vec4(1,0,0,0);
    _head.up = glm::vec4(0,1,0,0);
}

void CameraControls::set_orientation(glm::quat rotation)
{
    glm::mat4 rotate = hypermath::rotation0(rotation);
    glm::mat4 rotatez = hypermath::rotationz(rotation);
    glm::mat4 transf = hypermath::translation(glm::vec4(0,0,0,1),_shoulders.pos);
    _head.up = transf*rotate*glm::vec4(0,1,0,0);
    _head.forward = transf*rotate*glm::vec4(0,0,-1,0);
    _head.right = transf*rotate*glm::vec4(1,0,0,0);
    _shoulders.forward = transf*rotatez*glm::vec4(0,0,-1,0);
    _shoulders.right = transf*rotatez*glm::vec4(1,0,0,0);
    update_camera_transformation();
}

void CameraControls::set_mouse_speed(float speed)
{
    _mouse_speed = speed;
}

void CameraControls::set_step_size(float size)
{
    _move_speed = size;
}

float CameraControls::get_ipd()
{
    return _ipd;
}

float CameraControls::get_height()
{
    return _height;
}

float CameraControls::get_meter()
{
    return _meter;
}

glm::vec4 CameraControls::get_pos()
{
    return _shoulders.pos;
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

void CameraControls::grow(float factor)
{
    _meter *= factor;
}

void CameraControls::shrink(float factor)
{
    _meter /= factor;
}

glm::mat4 CameraControls::get_cam_view()
{
    glm::mat4 view = _camera->get_view();
    return view;
}

glm::vec4 CameraControls::get_forward()
{
    glm::vec4 forward = _shoulders.forward;
    return forward;
}

glm::vec4 CameraControls::get_flag_pos()
{
    return hypermath::midpoint(_shoulders.forward, -_shoulders.up, 0.5f);
}