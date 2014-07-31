#define GLM_FORCE_RADIANS
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/matrix_transform.hpp"
#include "../thirdparty/glm/glm/gtc/type_ptr.hpp"

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

void Camera::use(GLuint program)
{
	// eventually, we'll want to have a modelview matrix (premultiplied)
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(_proj));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"      ), 1, GL_FALSE, glm::value_ptr(_view));
}
