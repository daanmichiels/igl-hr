// provides some useful functions related to the hyperboloid model
// of hyperbolic 3-space

#ifndef HYPERMATH_H
#define HYPERMATH_H

namespace hypermath
{
	float dot(glm::vec4 v, glm::vec4 w);
	float length(glm::vec4 v);
	glm::vec4 normalize(glm::vec4 v);
	float dist(glm::vec4 p1, glm::vec4 p2);
	glm::vec4 exp(glm::vec4 basepoint, glm::vec4 direction);
	glm::vec4 exp0(glm::vec4 direction);
	glm::vec4 expinv(glm::vec4 basepoint, glm::vec4 target);
	glm::mat4 translation0(glm::vec4 target);
	glm::mat4 translation0inv(glm::vec4 target);
	glm::mat4 translation(glm::vec4 source, glm::vec4 target);
}

#endif
