// provides some useful functions related to the hyperboloid model
// of hyperbolic 3-space

#ifndef HYPERMATH_H
#define HYPERMATH_H

#include "glm/glm.hpp"

namespace hypermath
{
    double dot(glm::dvec4 v, glm::dvec4 w);
    double length(glm::dvec4 v);
    glm::dvec4 normalize(glm::dvec4 v);
    double dist(glm::dvec4 p1, glm::dvec4 p2);
    glm::dvec4 exp(glm::dvec4 basepoint, glm::dvec4 direction);
    glm::dvec4 exp0(glm::dvec4 direction);
    glm::dvec4 expinv(glm::dvec4 basepoint, glm::dvec4 target);
    double angle(glm::dvec4 v, glm::dvec4 w);
    double angle(glm::dvec4 a, glm::dvec4 b, glm::dvec4 c);
    glm::dmat4 translation0(glm::dvec4 target);
    glm::dmat4 translation0inv(glm::dvec4 target);
    glm::dmat4 translation(glm::dvec4 source, glm::dvec4 target);
    glm::dmat4 rotation0(glm::dquat rotation);
    glm::dmat4 rotationz(glm::dquat rotation);
    glm::dmat4 rotation0inv(glm::dquat rotation);
    glm::dmat4 rotation(glm::dvec4 basepoint, glm::dquat rotate);
    glm::dmat4 rotationinv(glm::dvec4 basepoint, glm::dquat rotate);
    glm::dmat4 movement(glm::dvec4 basepoint, glm::dvec4 target, glm::dquat rotate);
    glm::dvec4 midpoint(glm::dvec4 a, glm::dvec4 b, double t);
    glm::dvec4 gravity(glm::dvec4 basepoint);
}

#endif
