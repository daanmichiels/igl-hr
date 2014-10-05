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


CameraControls::CameraControls(GLFWwindow* window, Camera* camera)
{
    _window = window;
    _camera = camera;
    _pos = glm::vec4(0,0,0,1);
    _dir = glm::quat(0,glm::vec3(0,0,-1));
}

void CameraControls::handle(float delta_time, int width, int height)
{
    handle_keyboard(delta_time);
    handle_mouse(delta_time, width, height);
}

void CameraControls::handle_mouse(float delta_time, int width, int height)
{
    float y_ang = 0.0;
    float x_ang = 0.0;
    double mouse_x_pos, mouse_y_pos;
    glfwGetCursorPos(_window, &mouse_x_pos, &mouse_y_pos);
    //reset mouse then grab some angles for mouse-camera movement
    glfwSetCursorPos(_window, (int)floor(width/2),(int)floor(height/2));
    y_ang += _mouse_speed * delta_time * float(width/2-mouse_x_pos);
    x_ang += _mouse_speed * delta_time * float(height/2-mouse_y_pos);
    glm::quat x_quat = glm::angleAxis(float(x_ang), glm::vec3(1, 0, 0));
    glm::quat y_quat = glm::angleAxis(float(y_ang), glm::vec3(0, 1, 0));
    _dir = x_quat*y_quat*_dir;
    _camera->transform(hypermath::rotationinv(_pos,x_quat*y_quat));
}

void CameraControls::moveCamera(glm::vec4 trans) 
{
    trans += _pos;
    trans.w = sqrt(trans.x*trans.x+trans.y*trans.y+trans.z*trans.z+1);
    glm::mat4 move = hypermath::translation(trans,_pos);
    glm::mat4 movinv = hypermath::translation(_pos,trans);
    _pos = movinv*_pos;
    _pos.w = sqrt(_pos.x*_pos.x+_pos.y*_pos.y+_pos.z*_pos.z+1);
    _camera->transform(move);
}

void CameraControls::handle_keyboard(float delta_time)
{
    if( glfwGetKey(_window, GLFW_KEY_UP) == 1 || glfwGetKey(_window, GLFW_KEY_W ) == 1)
    {
        glm::vec3 axis = glm::axis(_dir);
        glm::vec4 trans = glm::vec4(.001*axis.x/sqrt(axis.x*axis.x+axis.y*axis.y+axis.z*axis.z),.001*axis.y/sqrt(axis.x*axis.x+axis.y*axis.y+axis.z*axis.z),.001*axis.z/sqrt(axis.x*axis.x+axis.y*axis.y+axis.z*axis.z),0);
        moveCamera(trans);
    }
    if( glfwGetKey(_window, GLFW_KEY_DOWN) == 1 || glfwGetKey(_window, GLFW_KEY_S ) == 1)
    {
        glm::vec3 axis = glm::axis(_dir);
        glm::vec4 trans = glm::vec4(-.001*axis.x/sqrt(axis.x*axis.x+axis.y*axis.y+axis.z*axis.z),-.001*axis.y/sqrt(axis.x*axis.x+axis.y*axis.y+axis.z*axis.z),-.001*axis.z/sqrt(axis.x*axis.x+axis.y*axis.y+axis.z*axis.z),0);
        moveCamera(trans);
    }    
    if( glfwGetKey(_window, GLFW_KEY_LEFT) == 1 || glfwGetKey(_window, GLFW_KEY_A ) == 1)
    {
        glm::vec4 trans(-.001,0,0,0);
        moveCamera(trans);
    }
    if( glfwGetKey(_window, GLFW_KEY_RIGHT) == 1 || glfwGetKey(_window, GLFW_KEY_D ) == 1)
    {
        glm::vec4 trans(.001,0,0,0);
        moveCamera(trans);
    }
    if( glfwGetKey(_window, GLFW_KEY_PAGE_UP) == 1)
    {
        glm::vec4 trans(0,.001,0,0);
        moveCamera(trans);
    }
    if( glfwGetKey(_window, GLFW_KEY_PAGE_DOWN) == 1)
    {
        glm::vec4 trans(0,-.001,0,0);
        moveCamera(trans);
    }
}

void CameraControls::set_mouse_speed(float speed)
{
    _mouse_speed = speed;
}

