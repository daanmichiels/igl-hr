
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

// #define GLM_FORCE_RADIANS
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtx/string_cast.hpp"
#include "../thirdparty/glm/glm/gtc/type_ptr.hpp"

static void error_callback(int error, const char* description)
{
    std::cerr << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// creates the window and gets an OpenGL context for it
GLFWwindow* create_window()
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    window = glfwCreateWindow(800, 600, "Hyperbolic space on the Rift", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create window. Do you have OpenGL 3.0 or higher?\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    return window;
}

// prints some information about the OpenGL context
// requires a currect OpenGL context
void print_info()
{
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

int main()
{
    GLFWwindow* window = create_window();
    print_info();

    // build and link the shading program
    GLuint program = build_program();
    if(!program)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // create some meshes
    glm::vec4 a = hypermath::exp0(glm::vec4(0.0f, 0.05f, 0.0f, 0.0f));
    glm::vec4 b = hypermath::exp0(glm::vec4(0.05f, -0.05f, 0.0f, 0.0f));
    glm::vec4 c = hypermath::exp0(glm::vec4(-0.05f, -0.05f, 0.0f, 0.0f));
    mesh mesh_triangle = primitives::triangle(a,b,c);
    mesh mesh_plane = primitives::rectangle(1.0, 2.0, glm::vec4(1,0,0,1));

    // create some objects
    object o1, o2, o3;
    o1.meshes.push_back(mesh_triangle);
    o2.meshes.push_back(mesh_triangle);
    o3.meshes.push_back(mesh_triangle);
    glm::vec4 location1 = hypermath::exp0(glm::vec4(0.04,0,-0.1,0));
    glm::vec4 location2 = hypermath::exp0(glm::vec4(0,0,-0.2,0));
    glm::vec4 location3 = hypermath::exp0(glm::vec4(0,0,0.04,0));
    o1.transform(hypermath::translation0(location1));
    o2.transform(hypermath::translation0(location2));
    o3.transform(hypermath::translation0(location3));

    // relations between the objects
    o2.children.push_back(&o3);

    // create a camera
    Camera cam(1.2f, 800.0f/600.0f, 0.001f, 100.0f);

    // set up the scene
    Scene s = Scene();
    s.objects.push_back(&o1);
    s.objects.push_back(&o2);
    s.camera = cam;
    s.program = program;

    // let's go!
    int frames_this_second = 0;
    double previoustime = 0;
    while (!glfwWindowShouldClose(window))
    {
        double t = glfwGetTime();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        cam.set_ratio(((float)width)/height);

        // movement
        location2 = hypermath::exp0(glm::vec4(0,0.1*sin(0.6*t),-0.2,0));
        o2.transformation = hypermath::translation0(location2);

        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        s.render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        frames_this_second++;
        if(t >= previoustime + 1.0)
        {
            previoustime += 1.0;
            glfwSetWindowTitle(window, std::to_string(frames_this_second).c_str());
            std::cout << std::to_string(frames_this_second) << "\n";
            frames_this_second = 0;
        }
    }

    // done
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

