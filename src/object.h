#ifndef OBJECT_H
#define OBJECT_H

#include "../thirdparty/glm/glm/glm.hpp"

#include <vector>
#include <string>
#include <string.h>
#include "mesh.h"

struct object
{
    std::string name;
    glm::mat4 transformation;
    std::vector<object*> children;
    std::vector<mesh> meshes;
    bool visible = true;
    void transform(glm::mat4 transformation);
    void set_transformation(glm::mat4 transform);
    object();
    object(const char filename[], bool converted, double scale);
    object( object obj, int copy_num);
};

#endif /* OBJECT_H */
