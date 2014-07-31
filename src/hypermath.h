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
	glm::vec4 expinv(glm::vec4 basepoint, glm::vec4 target)
	{
		float r = dist(basepoint, target);
		if(r==0)
			return glm::vec4(0,0,0,0);
		// should be more careful with this division, probably
		// (what if r is not exactly 0, but sinh(r) is? don't think
		// that's possible, but you get what I mean by more careful)
		glm::vec4 u = ((float)(1/sinh(r)))*target - ((float)(cosh(r)/sinh(r))) * basepoint;
		return r * u;
	}

	// When doing a lot of transformations on a point of the hyperboloid,
	// it will slowly drift away from it due to rounding errors.
	// We can use this to put it back on the hyperboloid.
	// The corrected point is not the closest to the given one, but
	// that shouldn't matter much (this should all be of the order of machine
	// precision anyway), and it's simple.
	glm::vec4 correct_point(glm::vec4 p)
	{
		return glm::vec4(p.x, p.y, p.z, sqrt(1+p.x*p.x+p.y*p.y+p.z*p.z));
	}
}