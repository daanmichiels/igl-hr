
#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "../data/scene.h"
#include "../data/object.h"
#include "../data/mesh.h"
#include "OVR/OVR.h"
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

private:
    // rift-related variables
    static ovrHmd hmd;
    static GLuint left_framebuffer;
    static GLuint right_framebuffer;
    static GLuint left_texture;
    static GLuint right_texture;
    static GLuint left_depth_buffer;
    static GLuint right_depth_buffer;
    static GLuint left_vao;
    static GLuint right_vao;
    // are we rendering for the rift?
    static bool rift_render;

    static bool init_glew();
    static bool open_window();
    static void glfw_error_callback(int error, const char* description);

    // TODO: reduce precision of this matrix?
    static void render_object(object o, glm::dmat4 modelview);
    static void render_mesh(mesh m);
    static glm::dmat4 view_matrix_from_frame(frame eyes);
    static void calculate_projection();
    static void handle_resize(GLFWwindow* win, int width, int height);

    // to set up the intermediate framebuffer/texture/depthbuffer for rift rendering
    static bool create_eye_framebuffer(GLuint &framebuffer, GLuint &texture, GLuint &depth_buffer);
    // to set up the geometry that will be textured for each eye
    static void create_eye_vaos();

    // we store these, but we could also call
    // glfwGetFrameBufferSize every time
    static int window_width;
    static int window_height;

    // doesn't change often, so we store it
    // (the view matrix is recalculated every frame)
    // no need for double precision, as this is only
    // passed to the GPU
    static glm::mat4 projection;
    static glm::mat4 projection_one_eye;
};


#endif

