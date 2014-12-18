#ifndef FRAME_H
#define FRAME_H

#include "../../thirdparty/glm/glm/glm.hpp"
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
    void correct_roundoff()
    {
        //correct position
        pos.w = sqrt(1+pos.x*pos.x+pos.y*pos.y+pos.z*pos.z);

        //correct tangent vectors
        //to be perpendicular to pos
        //which means they're in the tangent plane
        right.w += hypermath::dot(pos, right) / pos.w;
        up.w += hypermath::dot(pos, up) / pos.w;
        forward.w += hypermath::dot(pos, forward) / pos.w;

        //now we have to make the tangent vectors orthonormal
        //using gram-schmidt
        right = hypermath::normalize(right);
        up = up - hypermath::dot(up,right) * right;
        up = hypermath::normalize(up);
        forward = forward - hypermath::dot(forward,right) * right - hypermath::dot(forward,up) * up;
        forward = hypermath::normalize(forward);
    }
};

#endif /* FRAME_H */

