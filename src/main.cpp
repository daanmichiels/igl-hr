
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
#include "../thirdparty/libovr/src/OVR_CAPI.h"
#include "../thirdparty/libovr/include/OVR.h"

using namespace OVR;

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
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 6.4 * textureScale, 8 * textureScale, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
     
    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // The depth buffer
    GLuint left_depth_buffer;
    glGenRenderbuffers(1, &left_depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, left_depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 6.4 * textureScale, 8 * textureScale);
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
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 6.4 * textureScale, 8 * textureScale, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
     
    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // The depth buffer
    GLuint right_depth_buffer;
    glGenRenderbuffers(1, &right_depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, right_depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 6.4 * textureScale, 8 * textureScale);
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

    GLuint tex_id = glGetUniformLocation(quad_program, "sourceTexture");

    // create some meshes
    // glm::vec4 a = hypermath::exp0(glm::vec4(0.0f, 0.05f, 0.0f, 0.0f));
    // glm::vec4 b = hypermath::exp0(glm::vec4(0.05f, -0.05f, 0.0f, 0.0f));
    // glm::vec4 c = hypermath::exp0(glm::vec4(-0.05f, -0.05f, 0.0f, 0.0f));
    // mesh mesh_triangle = primitives::triangle(a,b,c);
    // mesh mesh_tetra    = primitives::tetrahedron(0.04);

    // create some objects
    // object o1, o2, o3, t1, t2, t3;
    // o1.meshes.push_back(mesh_triangle);
    // o2.meshes.push_back(mesh_tetra);
    // o3.meshes.push_back(mesh_tetra);
    // glm::vec4 location1 = hypermath::exp0(glm::vec4(0.04,0,-0.1,0));
    // glm::vec4 location2 = hypermath::exp0(glm::vec4(0,0,-0.2,0));
    // glm::vec4 location3 = hypermath::exp0(glm::vec4(0,0,0.07,0));
    // o1.transform(hypermath::translation0(location1));
    // o2.transform(hypermath::translation0(location2));
    // o3.transform(hypermath::translation0(location3));

    // create a camera
    Camera cam(1.2f, 640.0f/800.0f, 0.001f, 100.0f);

    // set up the scene
    Scene s = Scene();

    s.camera = cam;
    s.program = program;

    CameraControls cam_controls = CameraControls(window, &s.camera, &hmd);;
    flagmanager flag_manager = flagmanager(&s, cam_controls);

    s.lens_center_loc = glGetUniformLocation(quad_program, "lensCenter");
    s.barrel_power_loc = glGetUniformLocation(quad_program, "BarrelPower");

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

    object t1;
    t1 = object(filename, false, 1);
    s.objects.push_back(&t1);

    // setup grid
    object grid(primitives::grid(.5));
    s.objects.push_back(&grid);
    grid.visible = false;

    //make sierpinski octahedron.
    object sierpinski_octahedron(primitives::subdivided_octahedron(2, 7, true));
    s.objects.push_back(&sierpinski_octahedron);

    // Set up input handler
    InputHandler::cameracontrols = cam_controls;
    InputHandler::grid = &grid;
    InputHandler::flag_manager = &flag_manager;

    FpsCounter fps = FpsCounter(false);
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

        s.render_stereo(textureScale, cam_controls, left_framebuffer, right_framebuffer);

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
        glUniform1i(tex_id, 0);

        if (s.lens_center_loc != -1)
        {
           glUniform2f(s.lens_center_loc, cam_controls.left_lens_center.x, cam_controls.left_lens_center.y);
        }

        // Draw the left triangles !
        glBindVertexArray(render_left_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Bind our right texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, right_texture);
        // Set our "right_texture" sampler to user Texture Unit 0
        glUniform1i(tex_id, 0);

        if (s.lens_center_loc != -1)
        {
           glUniform2f(s.lens_center_loc, cam_controls.right_lens_center.x, cam_controls.right_lens_center.y);
        }

        // Draw the right triangles !
        glBindVertexArray(render_right_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();

        InputHandler::handle(delta_time, width, height);
	
        fps.update(current_time);

    }

    // Finished while loop. Time to destroy the window and exit.
    glfwDestroyWindow(window);
    glfwTerminate();
    ovrHmd_Destroy(hmd);
    ovr_Shutdown();
    exit(EXIT_SUCCESS);
}

