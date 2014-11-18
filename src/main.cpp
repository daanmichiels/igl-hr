
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include "shaders.h"
#include "camera.h"
#include "hypermath.h"
#include "primitives.h"
#include "scene.h"
#include "object.h"
#include "mesh.h"
#include "fpscounter.h"
#include "cameracontrols.h"
#include "inputhandler.h"
#include "flagmanager.h"
#include "init.h"

#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtx/string_cast.hpp"
#include "../thirdparty/glm/glm/gtc/type_ptr.hpp"
#include "../thirdparty/glm/glm/gtx/transform.hpp"
#include "../thirdparty/glm/glm/gtc/quaternion.hpp"

int main(int argc, const char* argv[])
{
    const char* filename = "resources/plane.obj";
    bool fullscreen = false;
    for(int i=argc-1; i>0; i--)
    {
        if(strcmp(argv[i],"--fullscreen") == 0)
            fullscreen = true;
        else
            filename = argv[i];
    }

    GLFWwindow* window = create_window(fullscreen);

    // build and link the shading program
    GLuint program = build_program();
    if(!program)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    object t1;
    t1 = object(filename, false, 1);
    // create a camera
    Camera cam(1.2f, 800.0f/600.0f, 0.001f, 100.0f);

    // set up the scene
    Scene s = Scene();
    s.objects.push_back(&t1);

    // setup grid
    object grid(primitives::grid(.5));
    s.objects.push_back(&grid);
    grid.visible = false;

    //make sierpinski octahedron.
    object sierpinski_octahedron(primitives::subdivided_octahedron(2, 7, true));
    s.objects.push_back(&sierpinski_octahedron);

    s.camera = cam;
    s.program = program;
    CameraControls cam_controls = CameraControls(window, &s.camera);
    flagmanager flag_manager = flagmanager(&s, cam_controls);

    // Set up input handler
    InputHandler::cameracontrols = cam_controls;
    InputHandler::grid = &grid;
    InputHandler::flag_manager = &flag_manager;

    FpsCounter fps = FpsCounter(true);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    //setup delta_time
    double current_time = glfwGetTime();
    double last_time = current_time;
    double delta_time;
    // main loop
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float initialFoV = ((float)width)/height;

        s.camera.set_ratio(initialFoV);
        glViewport(0, 0, width, height);
        // cam.set_ratio wouldn't work, since the scene refers to it by value, not by reference (should we change this?)

        //get current delta_time
        last_time = current_time;
        current_time = glfwGetTime();
        delta_time = current_time - last_time;

        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        s.render();
        glfwSwapBuffers(window);
        glfwPollEvents();

        InputHandler::handle(delta_time, width, height);
	
        fps.update(current_time);

    }

    // Finished while loop. Time to destroy the window and exit.
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

