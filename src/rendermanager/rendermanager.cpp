
#include "rendermanager.h"
#include "../logmanager/logmanager.h"
#include "../configuration/configuration.h"
#include "../strings.h"
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* RenderManager::window = NULL;

bool RenderManager::startup() {
    if(!open_window())
        return false;
    if(!init_glew())
        return false;

    LogManager::log_info("RenderManager started.", 2);
    return true;
}

void RenderManager::shutdown() {
    if(window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();

    LogManager::log_info("RenderManager stopped.", 2);
}

void RenderManager::glfw_error_callback(int error, const char* description) {
    std::string desc(description); //turn into a std::string
    LogManager::log_error("GLFW error: " + desc);
}

bool RenderManager::open_window() {
    glfwSetErrorCallback(RenderManager::glfw_error_callback);
    if (!glfwInit()) {
        LogManager::log_error("Failed to initialize GLFW.");
        window = NULL;
        return false;
    }

#ifndef WIN32
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_DEPTH_BITS, 24);
#endif
#ifdef WIN32
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
        window = NULL;
        return false;
    }

    glfwMakeContextCurrent(window);
    // TODO
    //glfwSetKeyCallback(window, InputHandler::key_callback);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    return true;
}

// Requires a current OpenGL context
bool RenderManager::init_glew() {
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if(error != GLEW_OK) {
        LogManager::log_error(std::string("Could not initialize glew: ") + (char *)glewGetErrorString(error));
        return false;
    }

    LogManager::log_info(std::string("Using GLEW ") + (char *)glewGetString(GLEW_VERSION), 1);
    LogManager::log_info(std::string("Using OpenGL ") + (char *)glGetString(GL_VERSION), 1);

    return true;
}

