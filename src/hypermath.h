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
		// is this numerically stable enough?
		glm::vec4 u = target - ((float)cosh(r)) * basepoint;
		return ((float)(r/sinh(r))) * u;
	}

	// translation (I suppose I should call this a boost?; I mean
	// inverse of exponential at starting point, parallel transport
	// along geodesic, exponential map at target)
	// this one is a translation starting at (0,0,0,1)
	glm::mat4 translation0(glm::vec4 target)
	{
		glm::mat4 result = glm::mat4(); // start with id
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
			{
				result[i][j] += target[i]*target[j]/(target.w + 1);
			}
		}
		result[3][0] = target.x;
		result[3][1] = target.y;
		result[3][2] = target.z;
		result[0][3] = target.x;
		result[1][3] = target.y;
		result[2][3] = target.z;
		result[3][3] = target.w;

		return result;
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