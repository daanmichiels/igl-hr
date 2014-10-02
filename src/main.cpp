
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
// Bind ESC to window close 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
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

    // This should not be included on Windows
    // Is this a platform-issue or a hardware/driver-issue?
    #ifndef _WIN32
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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
    mesh mesh_tetra    = primitives::tetrahedron(0.04);

    // create some objects
    object o1, o2, o3;
    o1.meshes.push_back(mesh_triangle);
    o2.meshes.push_back(mesh_tetra);
    o3.meshes.push_back(mesh_tetra);
    glm::vec4 location1 = hypermath::exp0(glm::vec4(0.04,0,-0.1,0));
    glm::vec4 location2 = hypermath::exp0(glm::vec4(0,0,-0.2,0));
    glm::vec4 location3 = hypermath::exp0(glm::vec4(0,0,0.07,0));
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

    // make it more interesting
    object grid[240];
    int count = 0;
    for(int i=0; i<10; i++)
    {
        float x = -0.2+0.04*i;
        for(int j=0; j<6; j++)
        {
            float y = -0.2 + 0.07 * j;
            for(int k=0; k<4; k++)
            {
                float z = -0.8 + k*0.1;
                grid[count].meshes.push_back(mesh_tetra);
                glm::vec4 location = hypermath::exp0(glm::vec4(x,y,z,0));
                grid[count].transform(hypermath::translation0(location));
                s.objects.push_back(&(grid[count]));
                count++;
            }
        }
    }
                
//setup delta_time (must be outside of main loop)
    double current_time = glfwGetTime();
    double last_time = current_time;
    double delta_time;
    FpsCounter fps = FpsCounter(true);
    CameraControls control = CameraControls(window, &s.camera);

    while (!glfwWindowShouldClose(window))
    {
        double t = glfwGetTime();
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
        // obj movement
        glm::mat4 rotation1 = glm::rotate((float)t,glm::vec3(0.0f,1.0f,0.0f));
        glm::mat4 rotation2 = glm::rotate((float)(2*t),glm::vec3(0.0f,1.0f,0.0f));
        location2 = hypermath::exp0(glm::vec4(0,0.1*sin(0.3*t),-0.2,0));
        o1.transformation = rotation1 * hypermath::translation0(location1);
        o2.transformation = hypermath::translation0(location2) * rotation2;

        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        s.render();
        glfwSwapBuffers(window);
        glfwPollEvents();

        /*Keyboard Mapping*/
        control.handle(delta_time, width, height);
	
        //set variables for camera movement. but first, set cursor mode invisible
        glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_HIDDEN);   
        
        
        fps.update(current_time);
    }

    // Finished while loop. Time to destroy the window and exit.
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

