#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <GL/glew.h>
#include "../thirdparty/glm/glm/glm.hpp"
#include "mesh.h"

namespace primitives
{
    mesh line(glm::vec4 a, glm::vec4 b, glm::vec4 col);
    mesh axes();
    mesh grid(double grid_space);
    mesh triangle(glm::vec4 a, glm::vec4 b, glm::vec4 c);
    mesh rectangle(float width, float depth, glm::vec4 color);
    mesh tetrahedron(float radius);
    mesh octahedron(float radius);
    mesh generic(std::vector<glm::vec4> v);
    mesh object(std::vector<glm::vec4> v);
    mesh subdivided_triangle(glm::vec4 a, glm::vec4 b, glm::vec4 c, int divisions, bool sierpinski = false);
    mesh subdivided_octahedron(float radius, int divisions, bool sierpinski = false);
    std::vector<glm::vec4> subdivide_triangle_vector(glm::vec4 a, glm::vec4 b, glm::vec4 c, int divisions, bool sierpinski = false);
    std::vector<glm::vec4> normalize_triangle_vector(std::vector<glm::vec4> input);

}

#endif
