
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
    static ovrHmd hmd;
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

    // we store these, but we could also call
    // glfwGetFrameBufferSize every time
    static int window_width;
    static int window_height;

    // doesn't change often, so we store it
    // (the view matrix is recalculated every frame)
    // no need for double precision, as this is only
    // passed to the GPU
    static glm::mat4 projection;
};


#endif

