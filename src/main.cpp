
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
#include "../thirdparty/LibOVR/Src/OVR_CAPI.h"
#include "../thirdparty/LibOVR/Include/OVR.h"

using namespace OVR;

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
    if(key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        // toggle mouse binding in cameracontrols
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    window = glfwCreateWindow(1280, 800, "Hyperbolic space on the Rift", NULL, NULL);
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




int main(int argc, const char* argv[])
{
    const char* filename = "resources/teapot.obj";
    if (argc > 1) {
        filename = argv[1];
    }

    GLFWwindow* window = create_window();
    print_info();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    /************************************
    * RIFT STUFF
    ************************************/
    ovr_Initialize();
    ovrHmd hmd = ovrHmd_Create(0);
    if (!hmd)
    {
        std::cout << "no hmd detected, using debug\n";
        hmd = ovrHmd_CreateDebug(ovrHmd_DK1);
    }
    ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection, 0);

    //Render to texture - tutorial at http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
    int textureScale = 400;

    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    GLuint left_framebuffer = 0;
    glGenFramebuffers(1, &left_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, left_framebuffer);

    // The texture we're going to render to
    GLuint left_texture;
    glGenTextures(1, &left_texture);
     
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, left_texture);
     
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 4 * textureScale, 3 * textureScale, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
     
    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // The depth buffer
    GLuint left_depth_buffer;
    glGenRenderbuffers(1, &left_depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, left_depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 4 * textureScale, 3 * textureScale);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, left_depth_buffer);

    // Set "left_texture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, left_texture, 0);
     
    // Set the list of draw buffers.
    GLenum left_draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, left_draw_buffers); // "1" is the size of left_draw_buffers

    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        return false;
    }

    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    GLuint right_framebuffer = 0;
    glGenFramebuffers(1, &right_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, right_framebuffer);

    // The texture we're going to render to
    GLuint right_texture;
    glGenTextures(1, &right_texture);
     
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, right_texture);
     
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 4 * textureScale, 3 * textureScale, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
     
    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // The depth buffer
    GLuint right_depth_buffer;
    glGenRenderbuffers(1, &right_depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, right_depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 4 * textureScale, 3 * textureScale);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, right_depth_buffer);

    // Set "left_texture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, right_texture, 0);
     
    // Set the list of draw buffers.
    GLenum rigth_draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, rigth_draw_buffers); // "1" is the size of rigth_draw_buffers

    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        return false;
    }

    /************************************
    * END RIFT STUFF
    ************************************/

    // build and link the shading program
    GLuint program = build_program();
    if(!program)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    GLuint quad_program = build_quad_program();
    if(!quad_program)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    GLuint left_tex_id = glGetUniformLocation(quad_program, "left_texture");

    // create some meshes
    glm::vec4 a = hypermath::exp0(glm::vec4(0.0f, 0.05f, 0.0f, 0.0f));
    glm::vec4 b = hypermath::exp0(glm::vec4(0.05f, -0.05f, 0.0f, 0.0f));
    glm::vec4 c = hypermath::exp0(glm::vec4(-0.05f, -0.05f, 0.0f, 0.0f));
    mesh mesh_triangle = primitives::triangle(a,b,c);
    mesh mesh_tetra    = primitives::tetrahedron(0.04);

    // create some objects
    object o1, o2, o3, t1, t2, t3;
    o1.meshes.push_back(mesh_triangle);
    o2.meshes.push_back(mesh_tetra);
    o3.meshes.push_back(mesh_tetra);
    glm::vec4 location1 = hypermath::exp0(glm::vec4(0.04,0,-0.1,0));
    glm::vec4 location2 = hypermath::exp0(glm::vec4(0,0,-0.2,0));
    glm::vec4 location3 = hypermath::exp0(glm::vec4(0,0,0.07,0));
    o1.transform(hypermath::translation0(location1));
    o2.transform(hypermath::translation0(location2));
    o3.transform(hypermath::translation0(location3));

    t1 = object(filename, false, 1);
    t2 = object(filename, false, 0.1);
    t3 = object(filename, false, 0.01);

    // relations between the objects
    o2.children.push_back(&o3);

    // create a camera
    Camera cam(1.2f, 800.0f/600.0f, 0.001f, 100.0f);

    // set up the scene
    Scene s = Scene();
    // s.objects.push_back(&o1);
    // s.objects.push_back(&o2);
    s.objects.push_back(&t1);
    s.objects.push_back(&t2);
    s.objects.push_back(&t3);
    s.camera = cam;
    s.program = program;
    s.lens_center_loc = glGetUniformLocation(quad_program, "lensCenter");

    // set up mesh to render to
    GLuint render_left_vao;
    glGenVertexArrays(1, &render_left_vao);
    glBindVertexArray(render_left_vao);

    GLuint render_left_buffer;
    glGenBuffers(1, &render_left_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, render_left_buffer);

    static const float render_left_data[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
     
    glBufferData(GL_ARRAY_BUFFER, sizeof(render_left_data), render_left_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set up mesh to render to
    GLuint render_right_vao;
    glGenVertexArrays(1, &render_right_vao);
    glBindVertexArray(render_right_vao);

    GLuint render_right_buffer;
    glGenBuffers(1, &render_right_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, render_right_buffer);

    static const float render_right_data[] = {
        0.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
    };
     
    glBufferData(GL_ARRAY_BUFFER, sizeof(render_right_data), render_right_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // make it more interesting
    // object grid[240];
    // int count = 0;
    // for(int i=0; i<10; i++)
    // {
    //     float x = -0.2+0.04*i;
    //     for(int j=0; j<6; j++)
    //     {
    //         float y = -0.2 + 0.07 * j;
    //         for(int k=0; k<4; k++)
    //         {
    //             float z = -0.8 + k*0.1;
    //             grid[count].meshes.push_back(mesh_tetra);
    //             glm::vec4 location = hypermath::exp0(glm::vec4(x,y,z,0));
    //             grid[count].transform(hypermath::translation0(location));
    //             s.objects.push_back(&(grid[count]));
    //             count++;
    //         }
    //     }
    // }
                
    //setup delta_time (must be outside of main loop)
    double current_time = glfwGetTime();
    double last_time = current_time;
    double delta_time;

    FpsCounter fps = FpsCounter();
    CameraControls control = CameraControls(window, &s.camera, &hmd);

    while (!glfwWindowShouldClose(window))
    {
        double t = glfwGetTime();
        float initialFoV = ((float)width)/height;

        s.camera.set_ratio(initialFoV);

        
        // cam.set_ratio wouldn't work, since the scene refers to it by value, not by reference (should we change this?)

        //get current delta_time
        last_time = current_time;
        current_time = glfwGetTime();
        delta_time = current_time - last_time;
        // obj movement
        // glm::mat4 rotation1 = glm::rotate((float)t,glm::vec3(0.0f,1.0f,0.0f));
        // glm::mat4 rotation2 = glm::rotate((float)(2*t),glm::vec3(0.0f,1.0f,0.0f));
        // location2 = hypermath::exp0(glm::vec4(0,0.1*sin(0.3*t),-0.2,0));
        // o1.transformation = rotation1 * hypermath::translation0(location1);
        // o2.transformation = hypermath::translation0(location2) * rotation2;

        s.render_stereo(textureScale, control, left_framebuffer, right_framebuffer);

        // Render to the screen
        glViewport(0, 0, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(quad_program);

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind our left texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, left_texture);
        // Set our "left_texture" sampler to user Texture Unit 0
        glUniform1i(left_tex_id, 0);

        if (s.lens_center_loc != -1)
        {
           glUniform2f(s.lens_center_loc, s.left_lens_center.x, s.left_lens_center.y);
        }

        // Draw the left triangles !
        glBindVertexArray(render_left_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Bind our right texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, right_texture);
        // Set our "right_texture" sampler to user Texture Unit 0
        glUniform1i(left_tex_id, 0);

        if (s.lens_center_loc != -1)
        {
           glUniform2f(s.lens_center_loc, s.right_lens_center.x, s.right_lens_center.y);
        }

        // Draw the right triangles !
        glBindVertexArray(render_right_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();

        /*Keyboard Mapping*/
        control.handle(delta_time, width, height);
	
        fps.update(current_time);

    }

    // Finished while loop. Time to destroy the window and exit.
    glfwDestroyWindow(window);
    glfwTerminate();
    ovrHmd_Destroy(hmd);
    ovr_Shutdown();
    exit(EXIT_SUCCESS);
}

