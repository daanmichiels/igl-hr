
// #define GLM_FORCE_RADIANS
#include "../thirdparty/glm/glm/glm.hpp"

namespace hypermath
{
    // inner product of two tangent vectors
    // note that the basepoint is irrelevant
    float dot(glm::vec4 v, glm::vec4 w)
    {
        return v.x*w.x + v.y*w.y + v.z*w.z - v.w*w.w;
    }

    // length of a tangent vector
    float length(glm::vec4 v)
    {
        return sqrt(dot(v,v));
    }

    // normalizes a vector
    glm::vec4 normalize(glm::vec4 v)
    {
        return v / length(v);
    }

    // distance between two points
    float dist(glm::vec4 p1, glm::vec4 p2)
    {
        return acosh(-dot(p1,p2));
    }

    // exponential map
    glm::vec4 exp(glm::vec4 basepoint, glm::vec4 direction)
    {
        float r = length(direction);
        if(r==0)
            return basepoint;
        // is this numerically stable enough?
        // maybe we should do something about the sinh(r)/r
        return ((float)cosh(r)) * basepoint + ((float)(sinh(r)/r)) * direction;
    }

    // exponential map with basepoint (0,0,0,1)
    glm::vec4 exp0(glm::vec4 direction)
    {
        return exp(glm::vec4(0,0,0,1), direction);
    }

    // inverse of the exponential map
    glm::vec4 expinv(glm::vec4 basepoint, glm::vec4 target)
    {
        float r = dist(basepoint, target);
        if(r==0)
            return glm::vec4(0,0,0,0);
        // is this numerically stable enough?
        // maybe we should do something about the r/sinh(r)
        glm::vec4 u = target - ((float)cosh(r)) * basepoint;
        return ((float)(r/sinh(r))) * u;
    }

    // translation (see wiki for definition)
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

    // inverse matrix of the one calculated by
    // translation0
    glm::mat4 translation0inv(glm::vec4 target)
    {
        glm::mat4 result = glm::mat4(); // start with id
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                result[i][j] += target[i]*target[j]/(target.w + 1);
            }
        }
        result[3][0] = -target.x;
        result[3][1] = -target.y;
        result[3][2] = -target.z;
        result[0][3] = -target.x;
        result[1][3] = -target.y;
        result[2][3] = -target.z;
        result[3][3] = target.w;

        return result;
    }

    // translation that maps source to target
    // (in matrix form). This is the generalization of translation0
    // for arbitrary starting point.
    glm::mat4 translation(glm::vec4 source, glm::vec4 target)
    {
        glm::mat4 Q = translation0inv(source);
        glm::vec4 intermediate_target = Q*target;
        return translation0(source) * translation0(intermediate_target) * Q;
    }
}

