
#include "rendermanager.h"
#include "../logmanager/logmanager.h"
#include "../charactermanager/charactermanager.h"
#include "../levelmanager/levelmanager.h"
#include "../configuration/configuration.h"
#include "../riftmanager/riftmanager.h"
#include "../shadermanager/shadermanager.h"
#include "../logicmanager/logicmanager.h"
#include "../loopmanager/loopmanager.h"
#include "../math/primitives.h"
#include "../strings.h"
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext.hpp"

int RenderManager::window_width = 100; // These don't matter: they get overwritten
int RenderManager::window_height = 100;
GLFWwindow* RenderManager::window = NULL;
glm::mat4 RenderManager::projection = glm::mat4();
glm::mat4 RenderManager::projection_one_eye = glm::mat4();

bool RenderManager::rift_render = false;
GLuint RenderManager::left_framebuffer = 0;
GLuint RenderManager::right_framebuffer = 0;
GLuint RenderManager::left_texture = 0;
GLuint RenderManager::right_texture = 0;
GLuint RenderManager::left_depth_buffer = 0;
GLuint RenderManager::right_depth_buffer = 0;
GLuint RenderManager::left_vao = 0;
GLuint RenderManager::right_vao = 0;

mesh RenderManager::flag_mesh = mesh();

/** \brief Starts the rendermanager. Gets the rift configuration, checks various configurations, calculates the projection, 
 * and then creates eye framebuffers. Also sets window size callback, and logs Rendermanager Started at level 2
 * \param void
 * \return false if anything goes wrong in the startup, true otherwise
 */
bool RenderManager::startup() {
    if(!open_window())
        return false;
    if(!init_glew())
        return false;

    if((Configuration::rift_output == OnOffAuto::on) || (Configuration::rift_output == OnOffAuto::automatic && RiftManager::rift_connected)) {
        rift_render = true;
    } else {
        rift_render = false;
    }

    calculate_projection();

    if(rift_render) {
        if(!create_eye_framebuffer(left_framebuffer, left_texture, left_depth_buffer))
            return false;
        if(!create_eye_framebuffer(right_framebuffer, right_texture, right_depth_buffer))
            return false;
        create_eye_vaos(); //TODO: should we check for failure here?
    }

    glfwSetWindowSizeCallback(window, handle_resize);

    flag_mesh = primitives::sphere(0.1, 4, glm::dvec4(0.5, 0.5, 0.5, 1.0));

    LogManager::log_info("RenderManager started.", 2);
    return true;
}

// calculate projection or projection_one_eye depending on whether
// rift-output is used
void RenderManager::calculate_projection() {
    float fov = 1.2f; // TODO: set a sensible value
    float ratio = ((float) window_width) / window_height;
    float near = 0.08 * CharacterManager::meter; // TODO: find sensible near and far planes
    float far = 1000 * CharacterManager::meter;

    if(rift_render) {
        ratio = ratio / 2.0;
        projection_one_eye = glm::perspective(fov, ratio, near, far);
    } else {
        projection = glm::perspective(fov, ratio, near, far);
    }
}

bool RenderManager::create_eye_framebuffer(GLuint &framebuffer, GLuint &texture, GLuint &depth_buffer) {
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width / 2.0, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glGenRenderbuffers(1, &depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window_width / 2.0, window_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
    GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, draw_buffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void RenderManager::handle_resize(GLFWwindow* win, int width, int height) {
    window_width = width;
    window_height = height;
    glViewport(0,0,width,height);
    calculate_projection();

    glDeleteRenderbuffers(1, &left_depth_buffer);
    glDeleteRenderbuffers(1, &right_depth_buffer);
    glDeleteTextures(1, &left_texture);
    glDeleteTextures(1, &right_texture);
    glDeleteFramebuffers(1, &left_framebuffer);
    glDeleteFramebuffers(1, &right_framebuffer);
    if((!create_eye_framebuffer(left_framebuffer, left_texture, left_depth_buffer)) ||
       (!create_eye_framebuffer(right_framebuffer, right_texture, right_depth_buffer))) {
        LogManager::log_error("Failed to resize framebuffer.");
    }
}

void RenderManager::handle_scale_change() {
    calculate_projection();
}

/** \brief Shuts down the rendermanager, destroys the window, terminates glfw, and logs RenderManager stopped at level 2
 * \param void
 * \return void
 */
void RenderManager::shutdown() {
    if(window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();

    LogManager::log_info("RenderManager stopped.", 2);
}

void RenderManager::render_flags(glm::dmat4 modelview, glm::mat4 projection) {
    glUseProgram(ShaderManager::flag_program);
    glUniformMatrix4fv(glGetUniformLocation(ShaderManager::flag_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    for(int i=0; i<LogicManager::flags.size(); i++) {
        glm::dvec3 highlight = LogicManager::flag_highlights[i];
        glUniform3f(glGetUniformLocation(ShaderManager::flag_program, "theOffset"), highlight.r, highlight.g, highlight.b);
        render_object(LogicManager::flags[i], modelview);
    }
    glUseProgram(0);
}

void RenderManager::render_object(object o, glm::dmat4 modelview) {
    modelview = modelview * o.transformation;
    glUniformMatrix4fv(glGetUniformLocation(ShaderManager::default_program, "modelview"), 1, GL_FALSE, glm::value_ptr((glm::mat4)modelview));
	frame eyes = CharacterManager::get_position_eyes();
	glUniform4fv(glGetUniformLocation(ShaderManager::default_program, "camera"), 1, glm::value_ptr(glm::vec4(eyes.pos.x, eyes.pos.y, eyes.pos.z, eyes.pos.w) ));

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

/** \brief Renders to the current buffer, then swaps buffers.
 * \param void
 * \return void
 */
void RenderManager::render() {
    if(!rift_render) {
        glClearColor(0.5, 0.7, 0.8, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(ShaderManager::default_program);
        glm::dmat4 view = view_matrix_from_frame(CharacterManager::get_position_eyes());
        glUniformMatrix4fv(glGetUniformLocation(ShaderManager::default_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        for(object* o : LevelManager::scene.objects) {
            if(o->visible) {
                render_object(*o, view);
            }
        }

        glUseProgram(0);

        render_flags(view, projection);
    } 
    else {
        glClearColor(0.8, 0.6, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(ShaderManager::default_program);
        glm::dmat4 view_left = view_matrix_from_frame(CharacterManager::get_position_left_eye());
        glm::dmat4 view_right = view_matrix_from_frame(CharacterManager::get_position_right_eye());
        glUniformMatrix4fv(glGetUniformLocation(ShaderManager::default_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection_one_eye));

        // render left eye to texture
        glBindFramebuffer(GL_FRAMEBUFFER, left_framebuffer);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(object* o : LevelManager::scene.objects) {
            if(o->visible) {
                render_object(*o, view_left);
            }
        }

        // render right eye to texture
        glBindFramebuffer(GL_FRAMEBUFFER, right_framebuffer);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(object* o : LevelManager::scene.objects) {
            if(o->visible) {
                render_object(*o, view_right);
            }
        }

        // render to screen
        glUseProgram(ShaderManager::quad_program);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, left_texture);
        glUniform1i(glGetUniformLocation(ShaderManager::quad_program, "the_texture"), 0);
        glUniform1f(glGetUniformLocation(ShaderManager::quad_program, "aspect_ratio"), 1.0); 
        glUniform2f(glGetUniformLocation(ShaderManager::quad_program, "lens_center"), 0.0, 0.0); //TODO: set correct values
        glBindVertexArray(left_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6); // TODO: maybe this should be a triangle strip

        glBindTexture(GL_TEXTURE_2D, right_texture);
        glUniform1i(glGetUniformLocation(ShaderManager::quad_program, "the_texture"), 0);
        glUniform1f(glGetUniformLocation(ShaderManager::quad_program, "aspect_ratio"), ((float)window_width) / 2.0 / window_height); 
        glUniform2f(glGetUniformLocation(ShaderManager::quad_program, "lens_center"), 0.0, 0.0); //TODO: set correct values
        glBindVertexArray(right_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUseProgram(0);
    }

    glfwSwapBuffers(window);
}

void RenderManager::create_eye_vaos() {
    glGenVertexArrays(1, &left_vao);
    glBindVertexArray(left_vao);
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // data in the form
    // (   x ,    y ,    u ,   v )
    const float left_data[] = {
        -1.0f, -1.0f,  0.0f, 0.0f,
         0.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
         0.0f, -1.0f,  1.0f, 0.0f,
         0.0f,  1.0f,  1.0f, 1.0f};
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, left_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2*sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &right_vao);
    glBindVertexArray(right_vao);
    buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // data in the form
    // (   x ,    y ,    u ,   v )
    const float right_data[] = {
         0.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         0.0f,  1.0f,  0.0f, 1.0f,
         0.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f};
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, right_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2*sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
    glfwWindowHint(GLFW_SAMPLES, 4); //multisample
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

/** \brief Get the current window width
 * \param void
 * \return integer value of the window width
 */
int RenderManager::get_window_width() {
    return window_width;
}

/** \brief Get the current window height
 * \param void
 * \return integer value of the window height
 */
int RenderManager::get_window_height() {
    return window_height;
}

