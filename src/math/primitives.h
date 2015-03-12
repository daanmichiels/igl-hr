#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "../data/mesh.h"

namespace primitives
{
    mesh line(glm::dvec4 a, glm::dvec4 b, glm::dvec4 col);
    mesh axes(double radius);
    mesh grid(double grid_space);
    mesh gravity_field(double grid_space);
    mesh triangle(glm::dvec4 a, glm::dvec4 b, glm::dvec4 c);
    mesh rectangle(double width, double depth, glm::dvec4 color);
    mesh circumscribed_ngon(int n, double radius, glm::dvec4 color);
    mesh ngon(int n, double radius);
    mesh tetrahedron(double radius);
    mesh octahedron(double radius);
    mesh object(std::vector<glm::dvec4> v);
    mesh subdivided_triangle(glm::dvec4 a, glm::dvec4 b, glm::dvec4 c, int divisions, bool sierpinski = false);
    mesh subdivided_octahedron(double radius, int divisions, bool sierpinski = false);
    mesh sphere(double radius, int divisions, glm::dvec4 col, bool sierpinski = false);
}

#endif
