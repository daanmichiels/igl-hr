
#include "../logmanager/logmanager.h"
#include "loopmanager.h"
#include "../rendermanager/rendermanager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool LoopManager::startup() {
    LogManager::log_info("LoopManager started.", 2);
    return true;
}

void LoopManager::shutdown() {
    LogManager::log_info("LoopManager stopped.", 2);
}

void LoopManager::run() {

    double t = glfwGetTime();
    double t_prev = t;
    double dt = 0.0;

    while(!glfwWindowShouldClose(RenderManager::window)) {
        t_prev = t;
        t = glfwGetTime();
        dt = t - t_prev;

        //TODO: handle input
        //TODO: handle animation

        RenderManager::render();

        glfwPollEvents();
    }
}

