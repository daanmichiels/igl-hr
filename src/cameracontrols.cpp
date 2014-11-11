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

using namespace OVR;

CameraControls::CameraControls(GLFWwindow* window, Camera* camera, ovrHmd* hmd)
{
    _window = window;
    _camera = camera;
    _hmd = hmd;
    _pos = glm::vec4(0,0,0,1);
    _up = glm::vec4(0,1,0,0);
    _forward = glm::vec4(0,0,-1,0);
    _right = glm::vec4(1,0,0,0);
    //shoulder orientation
    _sforward = glm::vec4(0,0,-1,0);
    _sright = glm::vec4(1,0,0,0);
}

void CameraControls::handle(float delta_time, int width, int height)
{
    handle_keyboard(delta_time);
    handle_hmd();
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
    glm::vec4 sright2 = ((float)cos(angle_y)) * _sright + ((float)sin(angle_y)) * _sforward;
    _sforward = -1*((float)sin(angle_y)) * _sright + ((float)cos(angle_y)) * _sforward;
    _sright = sright2;

    update_camera_transformation();

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
    //potentially change this so that we calculate inverse ourselves. Slightly faster maybe.
    transf = glm::inverse(transf);
    _camera->set_transformation(transf);
}

void CameraControls::move_right(float distance)
{
    glm::vec4 newpos = hypermath::exp(_pos, distance * _right);
    glm::mat4 transf = hypermath::translation(_pos,newpos);
    _pos = newpos;
    _up = transf * _up;
    _right = transf * _right;
    _forward = transf * _forward;
    _sright = transf * _sright;
    _sforward = transf * _sforward;
    update_camera_transformation();
}

void CameraControls::printShoulder()
{
    std::cout << "_sforward: " << _sforward.x << ", " << _sforward.y << ", " 
        << _sforward.z << ", " << _sforward.w << '\n';
    std::cout << "_sright: " << _sright.x << ", " << _sright.y << ", " 
        << _sright.z << ", " << _sright.w << '\n';
}

void CameraControls::handle_keyboard(float delta_time)
{
    if( glfwGetKey(_window, GLFW_KEY_UP) || glfwGetKey(_window, GLFW_KEY_W ))
    {
        // printShoulder();
        glm::vec4 newpos = hypermath::exp(_pos, _move_speed * ((float)delta_time) * _sforward);
        glm::mat4 transf = hypermath::translation(_pos,newpos);
        _pos = newpos;
        _up = transf * _up;
        _right = transf * _right;
        _forward = transf * _forward;
        _sright = transf * _sright;
        _sforward = transf * _sforward;
        update_camera_transformation();
        // printShoulder();
    }
    if( glfwGetKey(_window, GLFW_KEY_DOWN) || glfwGetKey(_window, GLFW_KEY_S ))
    {
        // printShoulder();
        glm::vec4 newpos = hypermath::exp(_pos, -_move_speed * ((float)delta_time) * _sforward);
        glm::mat4 transf = hypermath::translation(_pos,newpos);
        _pos = newpos;
        _up = transf * _up;
        _right = transf * _right;
        _forward = transf * _forward;
        _sright = transf * _sright;
        _sforward = transf * _sforward;
        update_camera_transformation();
        // printShoulder();
    }    
    if( glfwGetKey(_window, GLFW_KEY_LEFT) || glfwGetKey(_window, GLFW_KEY_A ))
    {
        // printShoulder();
        glm::vec4 newpos = hypermath::exp(_pos, -_move_speed * ((float)delta_time) * _sright);
        glm::mat4 transf = hypermath::translation(_pos,newpos);
        _pos = newpos;
        _up = transf * _up;
        _right = transf * _right;
        _forward = transf * _forward;
        _sright = transf * _sright;
        _sforward = transf * _sforward;
        update_camera_transformation();
        // printShoulder();
    }
    if( glfwGetKey(_window, GLFW_KEY_RIGHT) || glfwGetKey(_window, GLFW_KEY_D ))
    {
        // printShoulder();
        glm::vec4 newpos = hypermath::exp(_pos, _move_speed * ((float)delta_time) * _sright);
        glm::mat4 transf = hypermath::translation(_pos,newpos);
        _pos = newpos;
        _up = transf * _up;
        _right = transf * _right;
        _forward = transf * _forward;
        _sright = transf * _sright;
        _sforward = transf * _sforward;
        update_camera_transformation();
        // printShoulder();
    }
    if( glfwGetKey(_window, GLFW_KEY_PAGE_UP) || glfwGetKey(_window, GLFW_KEY_R))
    {
        // printShoulder();
        glm::vec4 newpos = hypermath::exp(_pos, _move_speed * ((float)delta_time) * _up);
        glm::mat4 transf = hypermath::translation(_pos,newpos);
        _pos = newpos;
        _up = transf * _up;
        _right = transf * _right;
        _forward = transf * _forward;
        _sright = transf * _sright;
        _sforward = transf * _sforward;
        update_camera_transformation();
        // printShoulder();
    }
    if( glfwGetKey(_window, GLFW_KEY_PAGE_DOWN) || glfwGetKey(_window, GLFW_KEY_F))
    {
        glm::vec4 newpos = hypermath::exp(_pos, -_move_speed * ((float)delta_time) * _up);
        glm::mat4 transf = hypermath::translation(_pos,newpos);
        _pos = newpos;
        _up = transf * _up;
        _right = transf * _right;
        _forward = transf * _forward;
        _sright = transf * _sright;
        _sforward = transf * _sforward;
        update_camera_transformation();
    }
    if( glfwGetKey(_window, GLFW_KEY_GRAVE_ACCENT))
    {
        _pos = glm::vec4(0,0,0,1);
        _up = glm::vec4(0,1,0,0);
        _forward = glm::vec4(0,0,-1,0);
        _right = glm::vec4(1,0,0,0);
        _sforward = glm::vec4(0,0,-1,0);
        _sright = glm::vec4(1,0,0,0);
    }
    if ( glfwGetKey(_window, GLFW_KEY_COMMA))
    {
        left_lens_center.x -= _eye_width_step;
        // std::cout << "eye width: " << _eye_width << '\n';
    }
    if ( glfwGetKey(_window, GLFW_KEY_PERIOD))
    {
        left_lens_center.x += _eye_width_step;
        // std::cout << "eye width: " << _eye_width << '\n';
    }
}

void CameraControls::set_orientation(glm::quat rotation)
{
    glm::mat4 rotate = hypermath::rotation0(rotation);
    glm::mat4 rotatez = hypermath::rotationz(rotation);
    glm::mat4 transf = hypermath::translation(glm::vec4(0,0,0,1),_pos);
    _up = transf*rotate*glm::vec4(0,1,0,0);
    _forward = transf*rotate*glm::vec4(0,0,-1,0);
    _right = transf*rotate*glm::vec4(1,0,0,0);
    _sforward = transf*rotatez*glm::vec4(0,0,-1,0);
    _sright = transf*rotatez*glm::vec4(1,0,0,0);
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

glm::vec4 CameraControls::get_pos()
{
    return _pos;
}

float CameraControls::get_eye_width()
{
    return _eye_width;
}

void CameraControls::setQuatIndices(int x, int y, int z, int w)
{
    quatX = x;
    quatY = y;
    quatZ = z;
    quatW = w;
}