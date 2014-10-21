
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/quaternion.hpp"
#include <iostream>

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
    // basepoint is irrelevant
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
        {
            return basepoint;
        }
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
        {
            return glm::vec4(0,0,0,0);
        }
        // is this numerically stable enough?
        // maybe we should do something about the r/sinh(r)
        // the sinh(r) can be calculated nicer, since it's
        // arccosh of something
        // same for cosh(r)
        glm::vec4 u = target - ((float)cosh(r)) * basepoint;
        return ((float)(r/sinh(r))) * u;
    }

    // the angle between two tangent vectors
    float angle(glm::vec4 v, glm::vec4 w)
    {
        return acos(dot(normalize(v),normalize(w)));
    }

    // the angle determined by three points
    // it's measured at the point b
    float angle(glm::vec4 a, glm::vec4 b, glm::vec4 c)
    {
        return angle(expinv(b,a),expinv(b,c));
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
                result[i][j] += target[i]*target[j]/(target.w+1);
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

    // rotation around a quaternion starting at origin. 
    // At origin, x,y,z have rotational symmetry and we can use regular Eulerian rotation.
    // Quaternion defines axis of rotation and degrees of rotation.
    glm::mat4 rotation0(glm::quat rotation)
    {
        glm::mat3 rotate3d = glm::mat3_cast(rotation);
        glm::mat4 result = glm::mat4();
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                result[i][j] = rotate3d[i][j];
            }
        }
        return result;
    }

    //just invert the quaternion to get the inverse rotation.
    glm::mat4 rotation0inv(glm::quat rotation)
    {
        glm::quat rotateinv = glm::inverse(rotation);
        return rotation0(rotateinv);
    }

    //need to rotate our final position since we're also rotating space
    glm::mat4 rotation(glm::vec4 basepoint, glm::quat rotate)
    {
        glm::mat4 rMatrix = rotation0(rotate);
        glm::vec4 target = rMatrix*basepoint;
        return translation0(target)*rMatrix*translation0inv(basepoint);
    }

    glm::mat4 rotationinv(glm::vec4 basepoint, glm::quat rotate)
    {
        return rotation(basepoint, glm::inverse(rotate));
    }

    //now allows to rotate space while moving.
    //not sure if this works completely correctly.
    //simple changes will fix. but if this works its faster than a multiplication 
    //of our rmatrix and translation functions by a couple calculations
    glm::mat4 movement(glm::vec4 basepoint, glm::vec4 target, glm::quat rotate)
    {
        glm::mat4 rMatrix = rotation0(rotate);
        glm::vec4 newTarget = rMatrix*target;
        return translation0(newTarget)*rMatrix*translation0inv(basepoint);
    }
}

