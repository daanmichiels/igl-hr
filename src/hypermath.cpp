#include <math.h>
#include "../thirdparty/glm/glm/glm.hpp"
#include "hypermath.h"

// exponential map at the point (0,0,0,1), the origin
// input is a tangent vector (x,y,z), interpreted as (x,y,z,0)
// (since those are the vectors tangent to H^3 at the point)
glm::vec4 exp0(glm::vec3 direction)
{
	float r = glm::length(direction);
	glm::vec3 unit = glm::normalize(direction);
	return glm::vec4(((float)sinh(r)) * unit,cosh(r));
}
