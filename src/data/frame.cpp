#include "frame.h"

void frame::correct_roundoff()
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

void frame::rotate_right(double angle) {
    glm::dvec4 newright = cos(angle) * right - sin(angle) * forward;
    glm::dvec4 newforward = cos(angle) * forward + sin(angle) * right;

    right = newright;
    forward = newforward;
}


void frame::rotate_up(double angle) {
    glm::dvec4 newup = cos(angle) * up - sin(angle) * forward;
    glm::dvec4 newforward = cos(angle) * forward + sin(angle) * up;

    up = newup;
    forward = newforward;
}

frame operator*(const glm::dmat4 mat, const frame f) {
    frame g = frame();

    g.pos = mat * f.pos;
    g.right = mat * f.right;
    g.up = mat * f.up;
    g.forward = mat * f.forward;

    return g;
}

