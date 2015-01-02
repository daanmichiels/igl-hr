#ifndef FRAME_H
#define FRAME_H

#include "glm/glm.hpp"
#include <math.h>
#include "../math/hypermath.h"

struct frame
{
    glm::dvec4 pos; //base point
    glm::dvec4 right; //positive x-direction
    glm::dvec4 up; //positive y-direction
    glm::dvec4 forward; //negative z-direction
    
    // Will put the frame back onto the hyperboloid
    // to prevent roundoff buildup.
    // This method is idempotent, which is important to
    // not have the frame 'wander around' when this
    // method is applied to it repeatedly.
    // TODO: is this method idempotent?
    void correct_roundoff();
    void rotate_right(double angle);
    void rotate_up(double angle);
};

// allow application of a matrix to a frame
frame operator*(glm::dmat4 mat, frame f);

#endif /* FRAME_H */

