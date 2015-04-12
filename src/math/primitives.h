#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "../data/mesh.h"

namespace primitives
{
    mesh line(const glm::dvec4 a, const glm::dvec4 b, const glm::dvec4 col);
    mesh axes(const double radius);
    mesh grid(const double grid_space);
    mesh gravity_field(const double grid_space);
    mesh triangle(const glm::dvec4 a, const glm::dvec4 b, const glm::dvec4 c);
    mesh rectangle(const double width, const double depth, glm::dvec4 color);
    mesh circumscribed_ngon(const int n, const double radius, const glm::dvec4 color);
    mesh ngon(const int n, const double radius);
    mesh tetrahedron(const double radius);
    mesh octahedron(const double radius);
    mesh object(const std::vector<glm::dvec4> v);
    mesh object(const std::vector<glm::dvec4> vertices, const std::vector<glm::dvec4> colors);
    mesh subdivided_triangle(const glm::dvec4 a, const glm::dvec4 b, const glm::dvec4 c, const int divisions, bool sierpinski = false);
    mesh subdivided_octahedron(const double radius, const int divisions, bool sierpinski = false);
    mesh sphere(const double radius, const int divisions, const glm::dvec4 col, bool sierpinski = false);
}

#endif
