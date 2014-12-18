#ifndef OBJECT_H
#define OBJECT_H

#include "../../thirdparty/glm/glm/glm.hpp"

#include <vector>
#include <string>
#include <string.h>
#include "mesh.h"

struct object
{
    std::string name;
    glm::dmat4 transformation;
    std::vector<object*> children;
    std::vector<mesh> meshes;
    bool visible = true;

    void transform(glm::dmat4 transformation);
    void set_transformation(glm::dmat4 transform);
    object();
    object(const char filename[], bool converted, double scale);
    object(mesh object_mesh);
};

#endif /* OBJECT_H */
