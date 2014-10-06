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

    void transform(glm::mat4 transformation);
    object();
    object(const char[]);
};

#endif /* OBJECT_H */
