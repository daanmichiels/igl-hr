#define GLM_FORCE_RADIANS
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/matrix_transform.hpp"

#include <GL/glew.h>
#include "camera.h"

Camera::Camera(float fov, float ratio, float near, float far)
{
	_fov = fov;
	_ratio = ratio;
	_near = near;
	_far = far;
	_proj = glm::perspective(fov, ratio, near, far);
	_view = glm::mat4(); //defaults to identity matrix
}

glm::mat4 Camera::get_projection()
{
	return _proj;
}

glm::mat4 Camera::get_view()
{
	return _view;
}

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
// start for many isometries.
void Camera::transform(glm::mat4 transforminv)
{
	_view = transforminv * _view;
}
