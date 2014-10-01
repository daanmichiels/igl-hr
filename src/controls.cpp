#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "hypermath.h"
#include "scene.h"
#include "controls.h"
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtx/transform.hpp"
#include "../thirdparty/glm/glm/gtc/quaternion.hpp"
#include <iostream>


CameraControls::CameraControls(GLFWwindow* window)
{
	_window = window;
}

Scene CameraControls::handle(float delta_time, Scene _s, int width, int height, double mouse_speed)
{
	Scene s_after_keyboard = CameraControls::handle_keyboard(_s);
	return CameraControls::handle_mouse(delta_time, s_after_keyboard, width, height, mouse_speed);
}

Scene CameraControls::handle_mouse(float delta_time, Scene _s, int width, int height, double mouse_speed)
{
	float y_ang = 0.0;
    float x_ang = 0.0;
    double mouse_x_pos, mouse_y_pos;
    glfwGetCursorPos(_window, &mouse_x_pos, &mouse_y_pos);
    //reset mouse then grab some angles for mouse-camera movement
    glfwSetCursorPos(_window, (int)floor(width/2),(int)floor(height/2));
    y_ang += mouse_speed * delta_time * float(width/2-mouse_x_pos);
    x_ang += mouse_speed * delta_time * float(height/2-mouse_y_pos);
    glm::quat x_quat = glm::angleAxis(float(x_ang), glm::vec3(1, 0, 0));
    glm::quat y_quat = glm::angleAxis(float(y_ang), glm::vec3(0, 1, 0));
    _s.camera.transform(hypermath::rotation0inv(x_quat*y_quat));
    return _s;

}

Scene CameraControls::handle_keyboard(Scene _s)
{
	if( glfwGetKey(_window, GLFW_KEY_UP) == 1 || glfwGetKey(_window, GLFW_KEY_W ) == 1)
    {
        glm::vec4 z_trans(0,0,-.01,1);
        _s.camera.transform(hypermath::translation0inv(z_trans));
    }
    if( glfwGetKey(_window, GLFW_KEY_DOWN) == 1 || glfwGetKey(_window, GLFW_KEY_S ) == 1)
    {
        glm::vec4 z_trans(0,0,.01,1);
        _s.camera.transform(hypermath::translation0inv(z_trans));
    }    
    if( glfwGetKey(_window, GLFW_KEY_LEFT) == 1 || glfwGetKey(_window, GLFW_KEY_A ) == 1)
    {
        glm::vec4 x_trans(-.01,0,0,1);
        _s.camera.transform(hypermath::translation0inv(x_trans));
    }
    if( glfwGetKey(_window, GLFW_KEY_RIGHT) == 1 || glfwGetKey(_window, GLFW_KEY_D ) == 1)
    {
        glm::vec4 x_trans(.01,0,0,1);
        _s.camera.transform(hypermath::translation0inv(x_trans));
    }
    if( glfwGetKey(_window, GLFW_KEY_PAGE_UP) == 1)
    {
        glm::vec4 y_trans(0,.01,0,1);
        _s.camera.transform(hypermath::translation0inv(y_trans));
    }
    if( glfwGetKey(_window, GLFW_KEY_PAGE_DOWN) == 1)
	{
        glm::vec4 y_trans(0,-.01,0,1);
        _s.camera.transform(hypermath::translation0inv(y_trans));
    }
    return _s;
}
