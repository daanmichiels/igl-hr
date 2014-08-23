#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "object.h"
#include "camera.h"
#include "mesh.h"
#include "../thirdparty/glm/glm/glm.hpp"

 class Scene
{
    public:
        std::vector<object*> objects;
        Camera camera;
        GLuint program;
        
        void render();

    private:
        void render_object(object o, glm::mat4 modelview);
        void render_mesh(mesh m);
};

#endif /* SCENE_H */

