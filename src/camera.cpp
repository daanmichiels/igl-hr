// #define GLM_FORCE_RADIANS
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/matrix_transform.hpp"

#include <GL/glew.h>
#include "camera.h"

// By default, the camera is at (0,0,0,1) and looking in the
// negative z-direction (0,0,-1,0).
Camera::Camera(float fov, float ratio, float near, float far)
{
    _fov = fov;
    _ratio = ratio;
    _near = near;
    _far = far;
    _proj = glm::perspective(fov, ratio, near, far);
    _view = glm::mat4(); //defaults to identity matrix
}

// Return the camera's current projection matrix.
glm::mat4 Camera::get_projection()
{
    return _proj;
}

// Return the camera's current view matrix.
glm::mat4 Camera::get_view()
{
    return _view;
}

// Change the camera's aspect ratio. Useful when resizing
// the window.
void Camera::set_ratio(float ratio)
{
    if(_ratio == ratio)
        return;

    _ratio = ratio;
    _proj = glm::perspective(_fov, _ratio, _near, _far);
}

// Applies a transformation to the camera, like moving
// or rotating.
// The matrix supplied must be the *inverse* of the trans-
// formation you want to apply. This avoids explicitly inverting
// the matrix, which is inferior to generating the inverse from the
// start.
void Camera::transform(glm::mat4 transforminv)
{
    _view = _view * transforminv;
}
