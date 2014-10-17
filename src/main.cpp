
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

#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtx/string_cast.hpp"
#include "../thirdparty/glm/glm/gtc/type_ptr.hpp"
#include "../thirdparty/glm/glm/gtx/transform.hpp"
#include "../thirdparty/glm/glm/gtc/quaternion.hpp"

// Called on GLFW error.
static void error_callback(int error, const char* description)
{
    std::cerr << description;
}

// Creates the window and gets an OpenGL context for it.
GLFWwindow* create_window()
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
     {
        exit(EXIT_FAILURE);
     }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    window = glfwCreateWindow(800, 600, "Hyperbolic space on the Rift", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create window. Do you have OpenGL 3.3 or higher?\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, InputHandler::key_callback);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    return window;
}

// Prints some information about the OpenGL context.
// Requires a currect OpenGL context.
void print_info()
{
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if(error != GLEW_OK)
    {
        std::cout << "Error: " << glewGetErrorString(error) << "\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << "\n";
    std::cout << "Using OpenGL " << glGetString(GL_VERSION) << "\n";
}

int main(int argc, const char* argv[])
{
    const char* filename = "resources/plane.obj";
    if (argc > 1) {
        filename = argv[1];
    }

    GLFWwindow* window = create_window();
    print_info();

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

    //setup grid axes 
    mesh axis_x, axis_y, axis_z;
    axis_x = primitives::line(hypermath::exp0(glm::vec4(-10,0,0,0)), hypermath::exp0(glm::vec4(10,0,0,0)), glm::vec4(1.0f,0.0f,0.0f,1.0f));
    axis_y = primitives::line(hypermath::exp0(glm::vec4(0,-10,0,0)), hypermath::exp0(glm::vec4(0,10,0,0)), glm::vec4(0.0f,0.0f,1.0f,1.0f));
    axis_z = primitives::line(hypermath::exp0(glm::vec4(0,0,-10,0)), hypermath::exp0(glm::vec4(0,0,10,0)), glm::vec4(0.0f,1.0f,0.0f,1.0f));
    //setup grid arrays *set grid_space to change the grid spacing
    double grid_space = .2;
    int steps = (int)ceil(10/grid_space);
    object x[2*steps + 1][2*steps + 1], y[2*steps + 1][2*steps + 1], z[2*steps + 1][2*steps + 1];

    //grid loop
    for(int j= 0; j<= 2*steps;j++)
    {
        for(int i= 0; i<2*steps;i++)
        { 
            int j_prime = -steps + j;
            int i_prime = -steps + i;

            x[i][j].meshes.push_back(axis_x);
            y[i][j].meshes.push_back(axis_y);
            z[i][j].meshes.push_back(axis_z);
            x[i][j].transform(hypermath::translation0(hypermath::exp0(glm::vec4(0, grid_space*j_prime, grid_space*i_prime, 0))));
            y[i][j].transform(hypermath::translation0(hypermath::exp0(glm::vec4(grid_space*i_prime, 0, grid_space*j_prime, 0))));
            z[i][j].transform(hypermath::translation0(hypermath::exp0(glm::vec4(grid_space*j_prime, grid_space*i_prime, 0, 0))));
            s.objects.push_back(&x[i][j]);
            s.objects.push_back(&y[i][j]);
            s.objects.push_back(&z[i][j]);

        }
    }
    s.camera = cam;
    s.program = program;

    // set up camera controls
    InputHandler::cameracontrols = CameraControls(window, &s.camera);

    FpsCounter fps = FpsCounter(true);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    //setup delta_time (must be outside of main loop)
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

