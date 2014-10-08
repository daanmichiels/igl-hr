#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <GL/glew.h>
#include "../thirdparty/glm/glm/glm.hpp"
#include "mesh.h"

namespace primitives
{
    mesh triangle(glm::vec4 a, glm::vec4 b, glm::vec4 c);
    mesh rectangle(float width, float depth, glm::vec4 color);
    mesh tetrahedron(float radius);
    mesh generic(std::vector<glm::vec4> v);
    mesh object(std::vector<glm::vec4> v);
}

#endif
