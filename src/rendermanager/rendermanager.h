
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

private:
    static ovrHmd hmd;
    static bool rift_render;

    static bool init_glew();
    static bool open_window();
    static void glfw_error_callback(int error, const char* description);

    // TODO: reduce precision of this matrix?
    static void render_object(object o, glm::dmat4 modelview);
    static void render_mesh(mesh m);
};


#endif

