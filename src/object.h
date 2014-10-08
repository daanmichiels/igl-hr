#ifndef OBJECT_H
#define OBJECT_H

#include "../thirdparty/glm/glm/glm.hpp"

#include <vector>
#include <string>
#include "mesh.h"

struct object
{
    std::string name;
    glm::mat4 transformation;
    std::vector<object*> children;
    std::vector<mesh> meshes;

    void transform(glm::mat4 transformation);
    void set_transformation(glm::mat4 transform);
};

#endif /* OBJECT_H */
