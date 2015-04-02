
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

#ifdef WIN32
#define OVR_OS_WIN32
#elif defined(__APPLE__)
#define OVR_OS_MAC
#else
#define OVR_OS_LINUX
#endif

using namespace OVR;

int RenderManager::window_width = 100; // These don't matter: they get overwritten
int RenderManager::window_height = 100;
GLFWwindow* RenderManager::window = NULL;
glm::mat4 RenderManager::projection = glm::mat4();
glm::mat4 RenderManager::projection_one_eye = glm::mat4();

ovrHmd RenderManager::hmd;
ovrSizei RenderManager::eyeres[2];
ovrEyeRenderDesc RenderManager::eye_rdesc[2];
ovrGLTexture RenderManager::fb_ovr_tex[2];
union ovrGLConfig RenderManager::glcfg;
unsigned int RenderManager::distort_caps;
unsigned int RenderManager::hmd_caps;

bool RenderManager::rift_render = false;
GLuint RenderManager::framebuffer = 0;
GLuint RenderManager::texture = 0;
GLuint RenderManager::depth_buffer = 0;
GLuint RenderManager::vao = 0;
int RenderManager::fb_width, RenderManager::fb_height;
int RenderManager::tex_width, RenderManager::tex_height;

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
        create_eye_vaos(); //TODO: should we check for failure here?

        setup_rift_rendering();
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

void RenderManager::set_hmd(ovrHmd newHmd)
{
    hmd = newHmd;
}

void RenderManager::setup_rift_rendering()
{
    /* retrieve the optimal render target resolution for each eye */
    eyeres[0] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->DefaultEyeFov[0], 1.0);
    eyeres[1] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1.0);

    /* and create a single render target texture to encompass both eyes */
   fb_width = eyeres[0].w + eyeres[1].w;
   fb_height = eyeres[0].h > eyeres[1].h ? eyeres[0].h : eyeres[1].h;
   update_target(fb_width, fb_height);

    /* fill in the ovrGLTexture structures that describe our render target texture */
    for(int i = 0; i < 2; i++) {
        fb_ovr_tex[i].OGL.Header.API = ovrRenderAPI_OpenGL;
        fb_ovr_tex[i].OGL.Header.TextureSize.w = tex_width;
        fb_ovr_tex[i].OGL.Header.TextureSize.h = tex_height;
        fb_ovr_tex[i].OGL.Header.RenderViewport.Pos.x = i == 0 ? 0 : fb_width / 2.0; // only field that differs between eyes
        fb_ovr_tex[i].OGL.Header.RenderViewport.Pos.y = 0;
        fb_ovr_tex[i].OGL.Header.RenderViewport.Size.w = fb_width / 2.0;
        fb_ovr_tex[i].OGL.Header.RenderViewport.Size.h = fb_height;
        fb_ovr_tex[i].OGL.TexId = texture; /* both eyes will use the same texture id */
    }

    /* fill in the ovrGLConfig structure */
    glcfg.OGL.Header.API = ovrRenderAPI_OpenGL;
    glcfg.OGL.Header.BackBufferSize.w = window_width;
    glcfg.OGL.Header.BackBufferSize.h = window_height;
    glcfg.OGL.Header.Multisample = 1;

    #ifdef OVR_OS_WIN32
        glcfg.OGL.Window = GetActiveWindow();
        glcfg.OGL.DC = wglGetCurrentDC();
    #elif defined(OVR_OS_LINUX)
        glcfg.OGL.Disp = glXGetCurrentDisplay();
    #endif

    if(hmd->HmdCaps & ovrHmdCap_ExtendDesktop) {
        LogManager::log_info("Running in extended desktop mode", 1);
    }
    else {
     /* to sucessfully draw to the HMD display in "direct-hmd" mode, we have to
      * call ovrHmd_AttachToWindow
      * XXX: this doesn't work properly yet due to bugs in the oculus 0.4.1 sdk/driver
      */
        #ifdef WIN32
            ovrHmd_AttachToWindow(hmd, glcfg.OGL.Window, 0, 0);
        #elif defined(OVR_OS_LINUX)
            ovrHmd_AttachToWindow(hmd, (void*)glXGetCurrentDrawable(), 0, 0);
        #endif

        LogManager::log_info("Running in direct hmd mode", 1);
    }

    /* enable low-persistence display and dynamic prediction for lattency compensation */
    hmd_caps = ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction;
    ovrHmd_SetEnabledCaps(hmd, hmd_caps);
  
    /* configure SDK-rendering and enable chromatic abberation correction, vignetting, and
     * timewrap, which shifts the image before drawing to counter any lattency between the call
     * to ovrHmd_GetEyePose and ovrHmd_EndFrame.
     */
    distort_caps = ovrDistortionCap_Chromatic | ovrDistortionCap_TimeWarp | ovrDistortionCap_Overdrive | ovrDistortionCap_Vignette;
    if(!ovrHmd_ConfigureRendering(hmd, &glcfg.Config, distort_caps, hmd->DefaultEyeFov, eye_rdesc)) {
        LogManager::log_error("Failed to configer distortion rendering");
    }

    /* disable the health and safety warning */
    ovrhmd_EnableHSWDisplaySDKRender(hmd, 0);
}

void RenderManager::update_target(int width, int height)
{
    // do we have a framebuffer setup? if not create one
    if (!framebuffer) {
        glGenFramebuffers(1, &framebuffer);
        glGenTextures(1, &texture);
        glGenRenderbuffers(1, &depth_buffer);
  
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    /* calculate the next power of two in both dimensions and use that as a texture size */
    tex_width = next_pow2(width);
    tex_height = next_pow2(height);

    /* create and attach the texture that will be used as a color buffer */
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    /* create and attach the renderbuffer that will serve as our z-buffer */
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, tex_width, tex_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LogManager::log_error("Incomplete Framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    LogManager::log_info("Render Target created", 1);
}

unsigned int RenderManager::next_pow2(unsigned int x) {
   x -= 1;
   x |= x >> 1;
   x |= x >> 2;
   x |= x >> 4;
   x |= x >> 8;
   x |= x >> 16;
   return x + 1;
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

    glDeleteRenderbuffers(1, &depth_buffer);
    glDeleteTextures(1, &texture);
    glDeleteFramebuffers(1, &framebuffer);
    
    update_target(window_width, window_height);
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

        render_flags(view, projection);

        glUseProgram(0);

        glfwSwapBuffers(window);
    } 
    else {
        ovrPosef pose[2];

        /* the drawing starts with a call to ovrHmd_BeginFrame */
        ovrHmd_BeginFrame(hmd, 0);

        /* start drawing onto our texture render target */
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.0, 0.2, 0.7, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* for each eye ... */
        for(int i = 0; i < 2; i++) {
            ovrEyeType eye = hmd->EyeRenderOrder[i];

            glUseProgram(ShaderManager::default_program);

            // draw on the correct side of the framebuffer
            glViewport(eye == ovrEye_Left ? 0 : fb_width / 2, 0, fb_width / 2, fb_height);

            //get the correct view from CharacterManager
            glm::dmat4 view = (eye == ovrEye_Left ? view_matrix_from_frame(CharacterManager::get_position_left_eye()) :
                view_matrix_from_frame(CharacterManager::get_position_right_eye()));

            // glm::dmat4 view = view_matrix_from_frame(CharacterManager::get_position_eyes());
            glUniformMatrix4fv(glGetUniformLocation(ShaderManager::default_program, "projection"), 1, GL_FALSE, 
                glm::value_ptr(projection_one_eye));


            pose[eye] = ovrHmd_GetHmdPosePerEye(hmd, eye);

            //draw the scene for this eye
            for(object* o : LevelManager::scene.objects) {
                if(o->visible) {
                    render_object(*o, view);
                }
            }

            render_flags(view, projection_one_eye);
        }

        /* after drawing both eyes into the texture render target, revert to drawing directly to the
         * display, and we call ovrHmd_EndFrame, to let the Oculus SDK draw both images properly
         * compensated for lens distortion and chromatic abberation onto the HMD screen.
         */
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
      
        ovrHmd_EndFrame(hmd, pose, &fb_ovr_tex[0].Texture);

        /* workaround for the oculus sdk distortion renderer bug, which uses a shader
         * program, and doesn't restore the original binding when it's done.
         */
        glUseProgram(0);
  
        // assert(glGetError() == GL_NO_ERROR);
    }
}

//TODO: do we still need this?
void RenderManager::create_eye_vaos() {
    // glGenVertexArrays(1, &left_vao);
    // glBindVertexArray(left_vao);
    // GLuint buffer;
    // glGenBuffers(1, &buffer);
    // glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // // data in the form
    // // (   x ,    y ,    u ,   v )
    // const float left_data[] = {
    //     -1.0f, -1.0f,  0.0f, 0.0f,
    //      0.0f, -1.0f,  1.0f, 0.0f,
    //     -1.0f,  1.0f,  0.0f, 1.0f,
    //     -1.0f,  1.0f,  0.0f, 1.0f,
    //      0.0f, -1.0f,  1.0f, 0.0f,
    //      0.0f,  1.0f,  1.0f, 1.0f};
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, left_data, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2*sizeof(float)));
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // glGenVertexArrays(1, &right_vao);
    // glBindVertexArray(right_vao);
    // buffer;
    // glGenBuffers(1, &buffer);
    // glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // // data in the form
    // // (   x ,    y ,    u ,   v )
    // const float right_data[] = {
    //      0.0f, -1.0f,  0.0f, 0.0f,
    //      1.0f, -1.0f,  1.0f, 0.0f,
    //      0.0f,  1.0f,  0.0f, 1.0f,
    //      0.0f,  1.0f,  0.0f, 1.0f,
    //      1.0f, -1.0f,  1.0f, 0.0f,
    //      1.0f,  1.0f,  1.0f, 1.0f};
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, right_data, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2*sizeof(float)));
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);
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

