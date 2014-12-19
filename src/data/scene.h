#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "object.h"
#include "camera.h"
#include "../charactermanager/cameracontrols.h"
#include "mesh.h"
#include "glm/glm.hpp"

 class Scene
{
    public:
        std::vector<object*> objects;
        Camera camera;
        GLuint program;
        GLuint lens_center_loc;
        GLuint barrel_power_loc;
        // TODO: what is this?
        bool _has_printed = false;
        
        void render();
        void render_stereo(int textureScale, CameraControls control, GLuint left_framebuffer, GLuint right_framebuffer);

    private:
        double _border = 0.1;
        void render_object(object o, glm::dmat4 modelview);
        void render_mesh(mesh m);
};

#endif /* SCENE_H */

