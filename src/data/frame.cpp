#include "frame.h"

/** \brief Puts the frame back onto the hyperboloid to prevent roundoff buildup.
 * \param void
 * \return void
 */
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
    //we should normalize up first and force other vectors to be orthogonal to it,
    //as for the shoulders we will calculate up exactly each frame, and for other frames the order doesn't matter

    up = hypermath::normalize(up);
    right = right - hypermath::dot(right, up) * up;
    right = hypermath::normalize(right);forward = forward - hypermath::dot(forward,right) * right - hypermath::dot(forward,up) * up;
    forward = hypermath::normalize(forward);
}

/** \brief Rotate the frame right by a given angle
 * \param Angle to rotate as a double
 * \return void
 */
void frame::rotate_right(double angle) {
    glm::dvec4 newright = cos(angle) * right - sin(angle) * forward;
    glm::dvec4 newforward = cos(angle) * forward + sin(angle) * right;

    right = newright;
    forward = newforward;
}

/** \brief Rotate the frame up by a given angle
 * \param Angle to rotate as a double
 * \return void
 */
void frame::rotate_up(double angle) {
    glm::dvec4 newup = cos(angle) * up - sin(angle) * forward;
    glm::dvec4 newforward = cos(angle) * forward + sin(angle) * up;

    up = newup;
    forward = newforward;
}

/** \brief Set multiplication operator for frames and double 4x4 matrices.
 * \param double 4x4 glm matrix, and a frame f
 * \return frame
 */
frame operator*(const glm::dmat4 mat, const frame f) {
    frame g = frame();

    g.pos = mat * f.pos;
    g.right = mat * f.right;
    g.up = mat * f.up;
    g.forward = mat * f.forward;

    return g;
}

