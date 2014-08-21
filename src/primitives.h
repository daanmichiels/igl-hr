#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <GL/glew.h>
// #define GLM_FORCE_RADIANS
#include "../thirdparty/glm/glm/glm.hpp"

namespace primitives
{
    GLuint triangle(glm::vec4 a, glm::vec4 b, glm::vec4 c);
    GLuint rectangle(float width, float depth, int m, int n);
}

#endif
