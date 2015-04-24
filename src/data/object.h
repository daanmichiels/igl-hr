#ifndef OBJECT_H
#define OBJECT_H

#include "glm/glm.hpp"

#include <vector>
#include <string>
#include <string.h>
#include "mesh.h"

struct object
{
    glm::dmat4 transformation; /*! Transformation matrix */
    std::vector<object*> children; /*! Children pointers (will move with object)*/
    std::vector<mesh> meshes; /*! Vector of object meshes */
    bool visible = true; /*! Whether or not the object is visible */

    void transform(glm::dmat4 transformation);
    void set_transformation(glm::dmat4 transform);
    object();
    object(const char filename[], bool converted, double scale);
    object(mesh object_mesh);
    void write_file(std::string filename);
};

#endif /* OBJECT_H */
