
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/quaternion.hpp"
#include <iostream>

namespace hypermath
{
    // inner product of two tangent vectors
    // note that the basepoint is irrelevant
    double dot(glm::dvec4 v, glm::dvec4 w)
    {
        return v.x*w.x + v.y*w.y + v.z*w.z - v.w*w.w;
    }

    // length of a tangent vector
    double length(glm::dvec4 v)
    {
        return sqrt(dot(v,v));
    }

    // normalizes a vector
    // basepoint is irrelevant
    glm::dvec4 normalize(glm::dvec4 v)
    {
        return v / length(v);
    }

    // distance between two points
    double dist(glm::dvec4 p1, glm::dvec4 p2)
    {
        return acosh(-dot(p1,p2));
    }

    // exponential map
    glm::dvec4 exp(glm::dvec4 basepoint, glm::dvec4 direction)
    {
        double r = length(direction);
        if(r==0)
        {
            return basepoint;
        }
        // is this numerically stable enough?
        // maybe we should do something about the sinh(r)/r
        return ((double)cosh(r)) * basepoint + ((double)(sinh(r)/r)) * direction;
    }

    // exponential map with basepoint (0,0,0,1)
    glm::dvec4 exp0(glm::dvec4 direction)
    {
        return exp(glm::dvec4(0,0,0,1), direction);
    }

    // inverse of the exponential map
    glm::dvec4 expinv(glm::dvec4 basepoint, glm::dvec4 target)
    {
        double r = dist(basepoint, target);
        if(r==0)
        {
            return glm::dvec4(0,0,0,0);
        }
        // is this numerically stable enough?
        // maybe we should do something about the r/sinh(r)
        // the sinh(r) can be calculated nicer, since it's
        // arccosh of something
        // same for cosh(r)
        glm::dvec4 u = target - ((double)cosh(r)) * basepoint;
        return ((double)(r/sinh(r))) * u;
    }

    // the angle between two tangent vectors
    double angle(glm::dvec4 v, glm::dvec4 w)
    {
        return acos(dot(normalize(v),normalize(w)));
    }

    // the angle determined by three points
    // it's measured at the point b
    double angle(glm::dvec4 a, glm::dvec4 b, glm::dvec4 c)
    {
        return angle(expinv(b,a),expinv(b,c));
    }

    // translation (see wiki for definition)
    // this one is a translation starting at (0,0,0,1)
    glm::dmat4 translation0(glm::dvec4 target)
    {
        glm::dmat4 result = glm::dmat4(); // start with id
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
    glm::dmat4 translation0inv(glm::dvec4 target)
    {
        glm::dmat4 result = glm::dmat4(); // start with id
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
    glm::dmat4 translation(glm::dvec4 source, glm::dvec4 target)
    {
        glm::dmat4 Q = translation0inv(source);
        glm::dvec4 intermediate_target = Q*target;
        return translation0(source) * translation0(intermediate_target) * Q;
    }

    // rotation around a quaternion starting at origin. 
    // At origin, x,y,z have rotational symmetry and we can use regular Eulerian rotation.
    // Quaternion defines axis of rotation and degrees of rotation.
    glm::dmat4 rotation0(glm::quat rotation)
    {
        return glm::dmat4_cast(rotation);
    }
    
    //project quaternion into a rotation around z-axis
    glm::dmat4 rotationz(glm::quat rotation)
    {
        rotation[0] = 0;
        rotation[2] = 0;
        return glm::dmat4_cast((glm::normalize(rotation)));
    }

    //just invert the quaternion to get the inverse rotation.
    glm::dmat4 rotation0inv(glm::quat rotation)
    {
        glm::quat rotateinv = glm::inverse(rotation);
        return rotation0(rotateinv);
    }

    //need to rotate our final position since we're also rotating space
    glm::dmat4 rotation(glm::dvec4 basepoint, glm::quat rotate)
    {
        glm::dmat4 rMatrix = rotation0(rotate);
        glm::dvec4 target = rMatrix*basepoint;
        return translation0(target)*rMatrix*translation0inv(basepoint);
    }

    glm::dmat4 rotationinv(glm::dvec4 basepoint, glm::quat rotate)
    {
        return rotation(basepoint, glm::inverse(rotate));
    }

    //now allows to rotate space while moving.
    //not sure if this works completely correctly.
    //simple changes will fix. but if this works its faster than a multiplication 
    //of our rmatrix and translation functions by a couple calculations
    glm::dmat4 movement(glm::dvec4 basepoint, glm::dvec4 target, glm::quat rotate)
    {
        glm::dmat4 rMatrix = rotation0(rotate);
        glm::dvec4 newTarget = rMatrix*target;
        return translation0(newTarget)*rMatrix*translation0inv(basepoint);
    }

    glm::dvec4 midpoint(glm::dvec4 a, glm::dvec4 b, double t)
    {
        return hypermath::exp(b,t*hypermath::expinv(b,a));
    }

    // Returns vector towards a point at (0,-Infinity,0, Infinity)
    glm::dvec4 gravity(glm::dvec4 basepoint) 
    {
    double a = basepoint.x;
    double b = basepoint.y;
    double c = basepoint.z;
    double d = basepoint.w;
    double bd=b+d;
    glm::dvec4 dir = glm::dvec4(a*(bd),1+b*(bd),c*(bd),-1+d*(bd));
    double norm = -1/sqrt(2*d*d*bd*bd+3+a*a+c*c);
    return norm*dir;
    }
}

