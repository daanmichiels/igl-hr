
#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "../data/scene.h"
#include "../data/object.h"
#include "../data/mesh.h"
#include "OVR.h"
#include "OVR_CAPI.h"
#include "OVR_CAPI_GL.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class RenderManager {
public:
    static bool startup();
    static void shutdown();
    static void render();

    static GLFWwindow* window;
    static Scene scene;

    static int get_window_width();
    static int get_window_height();

    static void set_hmd(ovrHmd newHmd);
    static void handle_scale_change();

    // doesn't change often, so we store it
    // (the view matrix is recalculated every frame)
    // no need for double precision, as this is only
    // passed to the GPU
    static glm::mat4 projection;
    static glm::mat4 projection_one_eye;
    static glm::dmat4 view_matrix_from_frame(frame eyes);

private:
    // rift-related variables
    static ovrHmd hmd;
    static ovrSizei eyeres[2];
    static ovrEyeRenderDesc eye_rdesc[2];
    static ovrGLTexture fb_ovr_tex[2];
    static union ovrGLConfig glcfg;
    static unsigned int distort_caps;
    static unsigned int hmd_caps;

    static GLuint framebuffer;
    static GLuint texture;
    static GLuint depth_buffer;
    static GLuint vao;

    static int fb_width, fb_height;
    static int tex_width, tex_height;

    // are we rendering for the rift?
    static bool rift_render;

    static bool init_glew();
    static bool open_window();
    static void glfw_error_callback(int error, const char* description);

    // TODO: reduce precision of this matrix?
    static void render_object(object o, glm::dmat4 modelview);
    static void render_mesh(mesh m);
    static void calculate_projection();
    static void handle_resize(GLFWwindow* win, int width, int height);
    static void render_flags(glm::dmat4 modelview, glm::mat4 projection);

    static void setup_rift_rendering();
    static void update_target(int width, int height);
    static unsigned int next_pow2(unsigned int x);

    // to set up the intermediate framebuffer/texture/depthbuffer for rift rendering
    static bool create_eye_framebuffer(GLuint &framebuffer, GLuint &texture, GLuint &depth_buffer);
    // to set up the geometry that will be textured for each eye
    static void create_eye_vaos();

    // we store these, but we could also call
    // glfwGetFrameBufferSize every time
    static int window_width;
    static int window_height;

    static mesh flag_mesh;
};


#endif

