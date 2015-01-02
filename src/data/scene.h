#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "object.h"
#include "mesh.h"
#include "frame.h"
#include "glm/glm.hpp"

 class Scene
{
    public:
        std::vector<object*> objects;
        GLuint program;
        GLuint lens_center_loc;
        GLuint barrel_power_loc;
        // TODO: what is this?
        bool _has_printed = false;
        
    private:
        // what is this?
        double _border = 0.1;
        void render_object(object o, glm::dmat4 modelview);
        void render_mesh(mesh m);
};

#endif /* SCENE_H */

