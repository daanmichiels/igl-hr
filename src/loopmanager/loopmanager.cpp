
#include <GL/glew.h>
#if defined(_WIN32)
 #define GLFW_EXPOSE_NATIVE_WIN32
 #define GLFW_EXPOSE_NATIVE_WGL
 #define OVR_OS_WIN32
#elif defined(__APPLE__)
 #define GLFW_EXPOSE_NATIVE_COCOA
 #define GLFW_EXPOSE_NATIVE_NSGL
 #define OVR_OS_MAC
#elif defined(__linux__)
 #define GLFW_EXPOSE_NATIVE_X11
 #define GLFW_EXPOSE_NATIVE_GLX
 #define OVR_OS_LINUX
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <OVR_CAPI_GL.h>

#include "../logmanager/logmanager.h"
#include "../logicmanager/logicmanager.h"
#include "../charactermanager/charactermanager.h"
#include "loopmanager.h"
#include "../rendermanager/rendermanager.h"

FpsCounter LoopManager::fpscounter = FpsCounter();
double LoopManager::t = 0.0;

/** \brief Startup LoopManage. Logs LoopManager started at level 2
 * \param void
 * \return true
 */
bool LoopManager::startup() {
    LogManager::log_info("LoopManager started.", 2);
    return true;
}

/** \brief Shutdown LoopManager. Logs LoopManager stopped at level 2
 * \param void
 * \return void
 */
void LoopManager::shutdown() {
    LogManager::log_info("LoopManager stopped.", 2);
}

/** \brief Run the loopmanager (the main loop)
 * \param void
 * \return void
 */
void LoopManager::run() {

    t = glfwGetTime();
    double t_prev = t;
    double dt = 0.0;

    while(!glfwWindowShouldClose(RenderManager::window)) {
        t_prev = t;
        t = glfwGetTime();
        dt = t - t_prev;

        //TODO: handle input
        //TODO: handle animation
        //TODO: make sure rift orientation is polled late

        CharacterManager::handle(dt);
        LogicManager::handle();
        RenderManager::render();
        fpscounter.update(dt);
        glfwPollEvents();
    }
}

