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
