
#include "rendermanager.h"
#include "../logmanager/logmanager.h"
#include "../charactermanager/charactermanager.h"
#include "../levelmanager/levelmanager.h"
#include "../configuration/configuration.h"
#include "../riftmanager/riftmanager.h"
#include "../shadermanager/shadermanager.h"
#include "../strings.h"
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

int RenderManager::window_width = 100; // These don't matter: they get overwritten
int RenderManager::window_height = 100;
bool RenderManager::rift_render = false;
GLFWwindow* RenderManager::window = NULL;
glm::mat4 RenderManager::projection = glm::mat4();

bool RenderManager::startup() {
    if(!open_window())
        return false;
    if(!init_glew())
        return false;

    calculate_projection();

    if((Configuration::rift_output == OnOffAuto::on) || (Configuration::rift_output == OnOffAuto::automatic && RiftManager::rift_connected)) {
        rift_render = true;
    } else {
        rift_render = false;
    }

    glfwSetWindowSizeCallback(window, handle_resize);

    LogManager::log_info("RenderManager started.", 2);
    return true;
}

void RenderManager::calculate_projection() {
    float fov = 1.2f; // TODO: set a sensible value
    float ratio = ((float) window_width) / window_height;
    float near = 0.03 * CharacterManager::meter; // TODO: find sensible near and far planes
    float far = 20 * CharacterManager::meter;
    projection = glm::perspective(fov, ratio, near, far);
}

void RenderManager::handle_resize(GLFWwindow* win, int width, int height) {
    window_width = width;
    window_height = height;
    glViewport(0,0,width,height);
    calculate_projection();
}

void RenderManager::shutdown() {
    if(window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();

    LogManager::log_info("RenderManager stopped.", 2);
}

void RenderManager::render_object(object o, glm::dmat4 modelview) {
    modelview = modelview * o.transformation;
    glUniformMatrix4fv(glGetUniformLocation(ShaderManager::default_program, "modelview"), 1, GL_FALSE, glm::value_ptr((glm::mat4)modelview));

    for(mesh m : o.meshes)
    {
        render_mesh(m);
    }
    for(object* child : o.children)
    {
        render_object(*child, modelview);
    }
}

// Renders a mesh.
void RenderManager::render_mesh(mesh m)
{
    glBindVertexArray(m.vao);
    glDrawArrays(m.mode, m.first, m.count);
}


void RenderManager::render() {
    if(rift_render) {
        glClearColor(0.8, 0.6, 0.0, 1.0);
    } else {
        glClearColor(0.0, 0.2, 0.7, 1.0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(ShaderManager::default_program);

    // TODO: handle rift case
    // TODO: store these in the rendermanager
    glm::dmat4 view = view_matrix_from_frame(CharacterManager::get_position_eyes());

    glUniformMatrix4fv(glGetUniformLocation(ShaderManager::default_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    for(object* o : LevelManager::scene.objects) {
        if(o->visible) {
            render_object(*o, view);
        }
    }

    glUseProgram(0);
    glfwSwapBuffers(window);
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
        window_width = glfwGetVideoMode(monitor)->width;
        window_height = glfwGetVideoMode(monitor)->height;
        window = glfwCreateWindow(window_width, window_height, HR_WINDOW_TITLE.c_str(), monitor, NULL);
    }
    else {
        window_width = Configuration::width;
        window_height = Configuration::height;
        window = glfwCreateWindow(window_width, window_height, HR_WINDOW_TITLE.c_str(), NULL, NULL);
    }

    if (!window) {
        LogManager::log_error("Failed to create window. Do you have a recent OpenGL version?");
        window = NULL;
        return false;
    }

    glfwMakeContextCurrent(window);

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

glm::dmat4 RenderManager::view_matrix_from_frame(frame eyes) {
    glm::dmat4 view = glm::dmat4();

    view[0][0] = eyes.right.x;
    view[0][1] = eyes.right.y;
    view[0][2] = eyes.right.z;
    view[0][3] = eyes.right.w;

    view[1][0] = eyes.up.x;
    view[1][1] = eyes.up.y;
    view[1][2] = eyes.up.z;
    view[1][3] = eyes.up.w;

    view[2][0] = -1*eyes.forward.x;
    view[2][1] = -1*eyes.forward.y;
    view[2][2] = -1*eyes.forward.z;
    view[2][3] = -1*eyes.forward.w;

    view[3][0] = eyes.pos.x;
    view[3][1] = eyes.pos.y;
    view[3][2] = eyes.pos.z;
    view[3][3] = eyes.pos.w;

    //TODO: invert by ourselves
    //      this should make a difference in stability
    view = glm::inverse(view);

    return view;
}

int RenderManager::get_window_width() {
    return window_width;
}

int RenderManager::get_window_height() {
    return window_height;
}

