
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include "init/init.h"
#include "logmanager/logmanager.h"
#include "configuration/configuration.h"
#include "shadermanager/shadermanager.h"
#include "assetmanager/assetmanager.h"
#include "loopmanager/loopmanager.h"

#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtx/string_cast.hpp"
#include "../thirdparty/glm/glm/gtc/type_ptr.hpp"
#include "../thirdparty/glm/glm/gtx/transform.hpp"
#include "../thirdparty/glm/glm/gtc/quaternion.hpp"
#include "../thirdparty/libovr/src/OVR_CAPI.h"
#include "OVR/OVR.h"

using namespace OVR;

int main(int argc, const char* argv[]) {
    // We want this to be the first thing, to be able to log errors
    LogManager::startup();
    ShaderManager::startup();
    AssetManager::startup();
    LoopManager::startup();

    Init::welcome_message();

    GLFWwindow* window = Init::open_window();
    if(!window) {
        LoopManager::shutdown();
        AssetManager::shutdown();
        ShaderManager::shutdown();
        LogManager::shutdown();
        return 0;
    }
    if(!Init::init_glew()) {
        LoopManager::shutdown();
        AssetManager::shutdown();
        ShaderManager::shutdown();
        LogManager::shutdown();
        return 0;
    }

    Configuration::configure(argc, argv);

    LoopManager::shutdown();
    AssetManager::shutdown();
    ShaderManager::shutdown();
    LogManager::shutdown();
    return 0;
}

//int oldmain(int argc, const char* argv[])
//{
    //LogManager::startup();
    //LogManager::log_error("This is an error.");
    //LogManager::log_warning("This is a warning.");
    //LogManager::log_info("This is interesting.");
//
    //Init::welcome_message();
//
    //ConfigurationManager::configure(argc, argv);
//
    //const char* filename = "resources/teapot.obj";
    //bool fullscreen = false;
    //bool rift_render = false;
    //for(int i=argc-1; i>0; i--)
    //{
        //if (strcmp(argv[i],"--fullscreen") == 0)
            //fullscreen = true;
        //else if (strcmp(argv[i],"--rift") == 0) {
            //rift_render = true;
            //fullscreen = true;
        //}
        //else
            //filename = argv[i];
    //}
//
    //GLFWwindow* window = create_window(fullscreen);
//
    //print_info();
//
    //int width, height;
    //glfwGetFramebufferSize(window, &width, &height);
//
    ///************************************
    //* RIFT STUFF
    //************************************/
    //ovr_Initialize();
    //ovrHmd hmd = ovrHmd_Create(0);
    //if (!hmd)
    //{
        //std::cout << "no hmd detected, using debug\n";
        //hmd = ovrHmd_CreateDebug(ovrHmd_DK1);
    //}
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection, 0);

    //Render to texture - tutorial at http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
    //int textureScale = 400;
//
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    //GLuint left_framebuffer = 0;
    //glGenFramebuffers(1, &left_framebuffer);
    //glBindFramebuffer(GL_FRAMEBUFFER, left_framebuffer);
//
    //// The texture we're going to render to
    //GLuint left_texture;
    //glGenTextures(1, &left_texture);
     //
    ////////// "Bind" the newly created texture : all future texture functions will modify this texture
    //glBindTexture(GL_TEXTURE_2D, left_texture);
     //
    //// Give an empty image to OpenGL ( the last "0" )
    //glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 6.4 * textureScale, 8 * textureScale, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
     //
    //// Poor filtering. Needed !
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
    //// The depth buffer
    //GLuint left_depth_buffer;
    //glGenRenderbuffers(1, &left_depth_buffer);
    //glBindRenderbuffer(GL_RENDERBUFFER, left_depth_buffer);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 6.4 * textureScale, 8 * textureScale);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, left_depth_buffer);
//
    //// Set "left_texture" as our colour attachement #0
    //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, left_texture, 0);
     //
    //// Set the list of draw buffers.
    //GLenum left_draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
    //glDrawBuffers(1, left_draw_buffers); // "1" is the size of left_draw_buffers
//
    //// Always check that our framebuffer is ok
    //if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //{
        //return false;
    //}
//
    //// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    //GLuint right_framebuffer = 0;
    //glGenFramebuffers(1, &right_framebuffer);
    //glBindFramebuffer(GL_FRAMEBUFFER, right_framebuffer);
//
    //// The texture we're going to render to
    //GLuint right_texture;
    //glGenTextures(1, &right_texture);
     //
    //// "Bind" the newly created texture : all future texture functions will modify this texture
    //glBindTexture(GL_TEXTURE_2D, right_texture);
     //
    //// Give an empty image to OpenGL ( the last "0" )
    //glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 6.4 * textureScale, 8 * textureScale, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
     //
    //// Poor filtering. Needed !
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
    //// The depth buffer
    //GLuint right_depth_buffer;
    //glGenRenderbuffers(1, &right_depth_buffer);
    //glBindRenderbuffer(GL_RENDERBUFFER, right_depth_buffer);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 6.4 * textureScale, 8 * textureScale);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, right_depth_buffer);
//
    //// Set "left_texture" as our colour attachement #0
    //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, right_texture, 0);
     //
    //// Set the list of draw buffers.
    //GLenum rigth_draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
    //glDrawBuffers(1, rigth_draw_buffers); // "1" is the size of rigth_draw_buffers
//
    //// Always check that our framebuffer is ok
    //if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //{
        //return false;
    //}
//
    ///************************************
    //* END RIFT STUFF
    //************************************/
//
    //// build and link the shading program
    //GLuint program = build_program();
    //if(!program)
    //{
        //glfwTerminate();
        //exit(EXIT_FAILURE);
    //}
//
    //GLuint quad_program = build_quad_program();
    //if(!quad_program)
    //{
        //glfwTerminate();
        //exit(EXIT_FAILURE);
    //}
//
    //GLuint left_tex_id = glGetUniformLocation(quad_program, "left_texture");
//
    //// create some meshes
    //glm::dvec4 a = hypermath::exp0(glm::dvec4(0.0f, 0.05f, 0.0f, 0.0f));
    //glm::dvec4 b = hypermath::exp0(glm::dvec4(0.05f, -0.05f, 0.0f, 0.0f));
    //glm::dvec4 c = hypermath::exp0(glm::dvec4(-0.05f, -0.05f, 0.0f, 0.0f));
    //mesh mesh_triangle = primitives::triangle(a,b,c);
    //mesh mesh_tetra    = primitives::tetrahedron(0.04);
//
    //// create some objects
    //object t1, t2, t3;
//
    //t1 = object(filename, false, 1);
    //t2 = object(filename, false, 0.1);
    //t3 = object(filename, false, 0.01);
//
    //// create a camera
    //Camera cam(1.2f, rift_render ? 640.0f/800.0f : 1280.0f/800.0f, 0.001f, 100.0f);
//
    //// set up the scene
    //Scene s = Scene();
    //object sierpinski_octahedron(primitives::subdivided_octahedron(2, 2, true));
    ////s.objects.push_back(&sierpinski_octahedron);
    //s.objects.push_back(&t2);
    //s.objects.push_back(&t1);
    //s.camera = cam;
    //s.program = program;
    //s.lens_center_loc = glGetUniformLocation(quad_program, "lensCenter");
    //s.barrel_power_loc = glGetUniformLocation(quad_program, "BarrelPower");
//
    //// setup grid
    //object grid(primitives::grid(.5));
    //s.objects.push_back(&grid);
    //grid.visible = false;
//
    //// set up mesh to render to
    //GLuint render_left_vao;
    //glGenVertexArrays(1, &render_left_vao);
    //glBindVertexArray(render_left_vao);
//
    //GLuint render_left_buffer;
    //glGenBuffers(1, &render_left_buffer);
    //glBindBuffer(GL_ARRAY_BUFFER, render_left_buffer);
//
    //static const float render_left_data[] = {
        //-1.0f, -1.0f, 0.0f,
        //0.0f, -1.0f, 0.0f,
        //-1.0f,  1.0f, 0.0f,
        //-1.0f,  1.0f, 0.0f,
        //0.0f, -1.0f, 0.0f,
        //0.0f,  1.0f, 0.0f,
    //};
     //
    //glBufferData(GL_ARRAY_BUFFER, sizeof(render_left_data), render_left_data, GL_STATIC_DRAW);
//
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), 0);
//
    //glBindVertexArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
//
    //// set up mesh to render to
    //GLuint render_right_vao;
    //glGenVertexArrays(1, &render_right_vao);
    //glBindVertexArray(render_right_vao);
//
    //GLuint render_right_buffer;
    //glGenBuffers(1, &render_right_buffer);
    //glBindBuffer(GL_ARRAY_BUFFER, render_right_buffer);
//
    //static const float render_right_data[] = {
        //0.0f, -1.0f, 0.0f,
        //1.0f, -1.0f, 0.0f,
        //0.0f,  1.0f, 0.0f,
        //0.0f,  1.0f, 0.0f,
        //1.0f, -1.0f, 0.0f,
        //1.0f,  1.0f, 0.0f,
    //};
     //
    //glBufferData(GL_ARRAY_BUFFER, sizeof(render_right_data), render_right_data, GL_STATIC_DRAW);
//
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), 0);
//
    //glBindVertexArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
//
    //// make it more interesting
    //// object grid[240];
    //// int count = 0;
    //// for(int i=0; i<10; i++)
    //// {
    ////     double x = -0.2+0.04*i;
    ////     for(int j=0; j<6; j++)
    ////     {
    ////         double y = -0.2 + 0.07 * j;
    ////         for(int k=0; k<4; k++)
    ////         {
    ////             double z = -0.8 + k*0.1;
    ////             grid[count].meshes.push_back(mesh_tetra);
    ////             glm::dvec4 location = hypermath::exp0(glm::dvec4(x,y,z,0));
    ////             grid[count].transform(hypermath::translation0(location));
    ////             s.objects.push_back(&(grid[count]));
    ////             count++;
    ////         }
    ////     }
    //// }
                //
    ////setup delta_time (must be outside of main loop)
    //double current_time = glfwGetTime();
    //double last_time = current_time;
    //double delta_time;
//
    //CameraControls control = CameraControls(window, &s.camera, &hmd);
    //flagmanager flag_manager = flagmanager(&s, control);
//
    //// Set up input handler
    //InputHandler::cameracontrols = control;
    //InputHandler::grid = &grid;
    //InputHandler::flag_manager = &flag_manager;
//
    //FpsCounter fps = FpsCounter(false);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
//
    //while (!glfwWindowShouldClose(window))
    //{
        //double t = glfwGetTime();
        //double initialFoV = (rift_render ? ((double)width / 2) : (double)width)/height;
//
        //s.camera.set_ratio(initialFoV);
//
        //
        //// cam.set_ratio wouldn't work, since the scene refers to it by value, not by reference (should we change this?)
//
        ////get current delta_time
        //last_time = current_time;
        //current_time = glfwGetTime();
        //delta_time = current_time - last_time;
        //// obj movement
        //// glm::dmat4 rotation1 = glm::rotate((double)t,glm::dvec3(0.0f,1.0f,0.0f));
        //// glm::dmat4 rotation2 = glm::rotate((double)(2*t),glm::dvec3(0.0f,1.0f,0.0f));
        //// location2 = hypermath::exp0(glm::dvec4(0,0.1*sin(0.3*t),-0.2,0));
        //// o1.transformation = rotation1 * hypermath::translation0(location1);
        //// o2.transformation = hypermath::translation0(location2) * rotation2;
//
        //if (rift_render) {
            //s.render_stereo(textureScale, control, left_framebuffer, right_framebuffer);
//
            //// Render to the screen
            //glViewport(0, 0, width, height);
            //glBindFramebuffer(GL_FRAMEBUFFER, 0);
            //glUseProgram(quad_program);
//
            //// Clear the screen
            //glClearColor(1.0, 1.0, 1.0, 1.0);
            //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
            //// Bind our left texture in Texture Unit 0
            //glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, left_texture);
            //// Set our "left_texture" sampler to user Texture Unit 0
            //glUniform1i(left_tex_id, 0);
//
            //if (s.lens_center_loc != -1)
            //{
               //glUniform2f(s.lens_center_loc, control.left_lens_center.x, control.left_lens_center.y);
            //}
//
            //// Draw the left triangles !
            //glBindVertexArray(render_left_vao);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
//
            //// Bind our right texture in Texture Unit 0
            //glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, right_texture);
            //// Set our "right_texture" sampler to user Texture Unit 0
            //glUniform1i(left_tex_id, 0);
//
            //if (s.lens_center_loc != -1)
            //{
               //glUniform2f(s.lens_center_loc, control.right_lens_center.x, control.right_lens_center.y);
            //}
//
            //// Draw the right triangles !
            //glBindVertexArray(render_right_vao);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
        //}
        //else {
            //// Render to the screen
            //glViewport(0, 0, width, height);
            //glBindFramebuffer(GL_FRAMEBUFFER, 0);
            //glUseProgram(quad_program);
//
            //// Clear the screen
            //glClearColor(1.0, 1.0, 1.0, 1.0);
            //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
            //s.render();
        //}
//
        //
//
        //glfwSwapBuffers(window);
        //glfwPollEvents();
//
        //InputHandler::handle(delta_time, width, height);
//
        ///*Keyboard Mapping*/
        //control.handle(delta_time, width, height);
	//
        //fps.update(current_time);
//
    //}
//
    //// Finished while loop. Time to destroy the window and exit.
    //glfwDestroyWindow(window);
    //glfwTerminate();
    //ovrHmd_Destroy(hmd);
    //ovr_Shutdown();
    //LogManager::shutdown();
    //exit(EXIT_SUCCESS);
//}
//
