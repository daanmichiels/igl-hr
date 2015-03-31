#ifndef TILINGS_H
#define TILINGS_H

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "primitives.h"
#include "hypermath.h"
#include "../data/mesh.h"

namespace tilings{
    mesh generate_tiling(const int sides, const double radius, const int iterations);
}

#endif