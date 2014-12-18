#include <GL/glew.h>
#include "../strings.h"
#include "init.h"
#include "../configuration/configuration.h"
#include "../logmanager/logmanager.h"
#include <string>

void Init::welcome_message() {
    LogManager::log_general(HR_WELCOME_MESSAGE, 1);
}

void Init::glfw_error_callback(int error, const char* description) {
    std::string desc(description);
    LogManager::log_error("GLFW error: " + desc);
}

GLFWwindow* Init::open_window() {
    GLFWwindow* window;
    glfwSetErrorCallback(Init::glfw_error_callback);
    if (!glfwInit()) {
        LogManager::log_error("Failed to initialize GLFW.");
        return false;
    }

#ifndef _WIN32
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //what was this line for?
    //glfwWindowHint(GLFW_DEPTH_BITS, 24);
#endif
#ifdef _WIN32
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
#endif

    if(Configuration::fullscreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        window = glfwCreateWindow(glfwGetVideoMode(monitor)->width, glfwGetVideoMode(monitor)->height, HR_WINDOW_TITLE.c_str(), monitor, NULL);
    }
    else {
        window = glfwCreateWindow(800, 600, HR_WINDOW_TITLE.c_str(), NULL, NULL);
    }

    if (!window) {
        LogManager::log_error("Failed to create window. Do you have a recent OpenGL version?");
        return NULL;
    }

    glfwMakeContextCurrent(window);
    // TODO
    //glfwSetKeyCallback(window, InputHandler::key_callback);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    return window;
}

// Requires a current OpenGL context
bool Init::init_glew() {
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if(error != GLEW_OK) {
        LogManager::log_error(std::string("Could not initialize glew: ") + (char *)glewGetErrorString(error));
        return false;
    }

    LogManager::log_info(std::string("Using GLEW ") + (char *)glewGetString(GLEW_VERSION), 2);
    LogManager::log_info(std::string("Using OpenGL ") + (char *)glGetString(GL_VERSION), 2);

    return true;
}

ovrHmd Init::init_rift() {
    return NULL;
}



