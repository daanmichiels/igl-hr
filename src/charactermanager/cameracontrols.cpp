#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../data/camera.h"
#include "../math/hypermath.h"
#include "../data/scene.h"
#include "cameracontrols.h"
#include "../../thirdparty/glm/glm/glm.hpp"
#include "../../thirdparty/glm/glm/gtx/transform.hpp"
#include "../../thirdparty/glm/glm/gtc/quaternion.hpp"
#include "../../thirdparty/glm/glm/gtx/string_cast.hpp"
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

void CameraControls::handle(double delta_time, int width, int height)
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

void CameraControls::handle_mouse(double delta_time, int width, int height)
{
    double mouse_x, mouse_y;
    double center_x = floor(width/2);
    double center_y = floor(height/2);
    double PI = 3.141592653589793238463;

    glfwGetCursorPos(_window, &mouse_x, &mouse_y);
    glfwSetCursorPos(_window, center_x, center_y);

    double _angle_ver = _mouse_speed * delta_time * (center_y - mouse_y);
    _head.up = ((double)cos(_angle_ver))*_head.up - ((double)sin(_angle_ver))*_head.forward;

    double angle_hor = _mouse_speed * delta_time * (center_x - mouse_x);
    _shoulders.forward = ((double)cos(angle_hor))*_shoulders.forward - ((double)sin(angle_hor))*_shoulders.right;
    _shoulders.right = ((double)sin(angle_hor))*_shoulders.forward + ((double)cos(angle_hor))*_shoulders.right;

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
        set_orientation(glm::dquat(orientation.w, orientation.x, orientation.y, orientation.z));
    }
}

void CameraControls::update_camera_transformation()
{
    glm::dmat4 transf = glm::dmat4();
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

void CameraControls::move_right(double distance)
{
    glm::dvec4 newpos = hypermath::exp(_shoulders.pos, distance * _head.right);
    glm::dmat4 transf = hypermath::translation(_shoulders.pos,newpos);
    _shoulders.pos = newpos;
    _shoulders.right = transf * _shoulders.right;
    _shoulders.forward = transf * _shoulders.forward;
    _head.up = transf * _head.up;
    _head.forward = transf * _head.forward;
    _head.right = transf * _head.right;
    update_camera_transformation();
}

void CameraControls::handle_keyboard(double delta_time)
{
    //direction of walking in coordinates (right, up, forward)
    glm::dvec3 walking_direction = glm::dvec3();
    if( glfwGetKey(_window, GLFW_KEY_UP) || glfwGetKey(_window, GLFW_KEY_W ))
    {
        walking_direction += glm::dvec3(0,0,1);
    }
    if( glfwGetKey(_window, GLFW_KEY_DOWN) || glfwGetKey(_window, GLFW_KEY_S ))
    {
        walking_direction += glm::dvec3(0,0,-1);
    }    
    if( glfwGetKey(_window, GLFW_KEY_LEFT) || glfwGetKey(_window, GLFW_KEY_A ))
    {
        walking_direction += glm::dvec3(-1,0,0);
    }
    if( glfwGetKey(_window, GLFW_KEY_RIGHT) || glfwGetKey(_window, GLFW_KEY_D ))
    {
        walking_direction += glm::dvec3(1,0,0);
    }
    if( glfwGetKey(_window, GLFW_KEY_PAGE_UP) || glfwGetKey(_window, GLFW_KEY_R))
    {
        walking_direction += glm::dvec3(0,1,0);
    }
    if( glfwGetKey(_window, GLFW_KEY_PAGE_DOWN) || glfwGetKey(_window, GLFW_KEY_F))
    {
        walking_direction += glm::dvec3(0,-1,0);
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
        glm::dvec4 newpos = hypermath::exp(_shoulders.pos, walking_direction.x*_shoulders.right + walking_direction.y*_shoulders.up + walking_direction.z*_shoulders.forward);
        glm::dmat4 transf = hypermath::translation(_shoulders.pos,newpos);
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

    _shoulders.pos = glm::dvec4(0,0,0,1);
    _shoulders.forward = glm::dvec4(0,0,-1,0);
    _shoulders.right = glm::dvec4(1,0,0,0);
    _shoulders.up = glm::dvec4(0,1,0,0);

    _head.pos = glm::dvec4(0,0,0,1);
    _head.forward = glm::dvec4(0,0,-1,0);
    _head.right = glm::dvec4(1,0,0,0);
    _head.up = glm::dvec4(0,1,0,0);
}

void CameraControls::set_orientation(glm::dquat rotation)
{
    glm::dmat4 rotate = hypermath::rotation0(rotation);
    glm::dmat4 rotatez = hypermath::rotationz(rotation);
    glm::dmat4 transf = hypermath::translation(glm::dvec4(0,0,0,1),_shoulders.pos);
    _head.up = transf*rotate*glm::dvec4(0,1,0,0);
    _head.forward = transf*rotate*glm::dvec4(0,0,-1,0);
    _head.right = transf*rotate*glm::dvec4(1,0,0,0);
    _shoulders.forward = transf*rotatez*glm::dvec4(0,0,-1,0);
    _shoulders.right = transf*rotatez*glm::dvec4(1,0,0,0);
    update_camera_transformation();
}

void CameraControls::set_mouse_speed(double speed)
{
    _mouse_speed = speed;
}

void CameraControls::set_step_size(double size)
{
    _move_speed = size;
}

double CameraControls::get_ipd()
{
    return _ipd;
}

double CameraControls::get_height()
{
    return _height;
}

double CameraControls::get_meter()
{
    return _meter;
}

glm::dvec4 CameraControls::get_pos()
{
    return _shoulders.pos;
}

//increase step size. max is 1.0f
void CameraControls::increase_speed()
{
    double size = _move_speed + .05;
    if(size > 1.0)
    {
        size = 1.0;
    }
    set_step_size(size);
}

//decrease step size. minimum is 0.01f
void CameraControls::decrease_speed()
{
    double size = _move_speed - .05;
    if(size <= 0)
    {
        size = 0.01;
    }

    set_step_size(size);
}

void CameraControls::grow(double factor)
{
    _meter *= factor;
}

void CameraControls::shrink(double factor)
{
    _meter /= factor;
}

glm::dmat4 CameraControls::get_cam_view()
{
    glm::dmat4 view = _camera->get_view();
    return view;
}

glm::dvec4 CameraControls::get_forward()
{
    glm::dvec4 forward = _shoulders.forward;
    return forward;
}

glm::dvec4 CameraControls::get_flag_pos()
{
    return hypermath::midpoint(_shoulders.forward, -_shoulders.up, 0.5);
}
