#ifndef TILINGS_H
#define TILINGS_H

#include "../data/mesh.h"

namespace tilings{
    /** \brief Function to generate a tiling of the hyperbolic plane. For this to work, (sides-2)(around_vertex-2) must be
      *        greater then 4
      * \param Number of sides for the ngon to tile with, how many around each vertex, the number of iterations to run,
      * \param the closeness cutoff, and whether to keep in memory
      * \return Mesh for the tiling
      */
    mesh generate_tiling(const int sides, const int around_vertex, const int iterations, const double closeness, const bool in_mem = false);
}

#endif
