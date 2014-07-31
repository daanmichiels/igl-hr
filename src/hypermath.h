// provides some useful functions related to the hyperboloid model
// of hyperbolic 3-space
namespace hypermath
{
	// distance between two points
	float dist(glm::vec4 p1, glm::vec4 p2)
	{
		return acosh(- p1.x*p2.x - p1.y*p2.y - p1.z*p2.z + p1.w*p2.w);
	}

	// exponential map
	glm::vec4 exp(glm::vec4 basepoint, glm::vec4 direction)
	{
		float r = glm::length(direction);
		glm::vec4 unit = glm::normalize(direction);
		return ((float)cosh(r)) * basepoint + ((float)sinh(r)) * unit;
	}

	// inverse of the exponential map
	//glm::vec4 expinv(glm::vec4 basepoint, glm::vec4 target)
	//{
	//	
	//}
}