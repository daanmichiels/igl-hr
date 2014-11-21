#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "object.h"
#include "camera.h"
#include "cameracontrols.h"
#include "mesh.h"
#include "../thirdparty/glm/glm/glm.hpp"

 class Scene
{
    public:
        std::vector<object*> objects;
        Camera camera;
        GLuint program;
        GLuint lens_center_loc;
        GLuint barrel_power_loc;
        bool _has_printed = false;
        
        void render();
        void render_stereo(int textureScale, CameraControls control, GLuint left_framebuffer, GLuint right_framebuffer);
        void render_all_permutations(int width, int height, CameraControls control);

    private:
        float _border = 0.1f;
        void render_object(object o, glm::mat4 modelview);
        void render_mesh(mesh m);
};

#endif /* SCENE_H */

