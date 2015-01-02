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
};

#endif /* SCENE_H */

