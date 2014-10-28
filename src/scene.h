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
        bool _has_printed = false;
        
        void render();
        void render_stereo(int width, int height, CameraControls control);
        void render_all_permutations(int width, int height, CameraControls control);

    private:
        void render_object(object o, glm::mat4 modelview);
        void render_mesh(mesh m);
};

#endif /* SCENE_H */

