
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <iostream>
#include "glm/gtx/string_cast.hpp"

/*! \file Hypermath */

namespace hypermath
{
    /** \brief Computes the inner product of two tangent vectors note that the basepoint is irrelevant
     * \param Two glm dvec4's
     * \return Inner product as a double
     */
    double dot(glm::dvec4 v, glm::dvec4 w)
    {
        return v.x*w.x + v.y*w.y + v.z*w.z - v.w*w.w;
    }

    /** \brief Computes the length of a tangent vector
     *  \param One glm dvec4
     * \return Length of the vector as a double
     */
    double length(glm::dvec4 v)
    {
        return sqrt(dot(v,v));
    }

    /** \brief Normalizes a vector, the basepoint is irrelevant
     * \param One glm dvec4
     * \return Normalized glm dvec4
     */
    glm::dvec4 normalize(glm::dvec4 v)
    {
        return v / length(v);
    }

    /** \brief Computes the distance between two points.
     * \param Two glm dvec4's
     * \return The distance between the vectors as a double.
    */
    double dist(glm::dvec4 p1, glm::dvec4 p2)
    {
        return glm::acosh(-dot(p1,p2));
    }

    /** \brief Exponential map
     * \param Basepoint and direction vectors
     * \return Exponentiated glm dvec4
     */
    glm::dvec4 exp(glm::dvec4 basepoint, glm::dvec4 direction)
    {
        double r = length(direction);
        if(r==0)
        {
            return basepoint;
        }
        // is this numerically stable enough?
        // maybe we should do something about the sinh(r)/r
        // It is perfect. See testing of sinh_over_id.
        return cosh(r) * basepoint + (sinh(r)/r) * direction;
    }

    /** \brief Inverse of the exponential map
     * \param Basepoint and target glm dvec4's
     * \return One exponentiated glm dvec4
     */
    glm::dvec4 expinv(glm::dvec4 basepoint, glm::dvec4 target)
    {
        double r = dist(basepoint, target);
        if(r == 0)
        {
            return glm::dvec4(0,0,0,0);
        }
        // is this numerically stable enough?
        // maybe we should do something about the r/sinh(r)
        // the sinh(r) can be calculated nicer, since it's
        // arccosh of something
        // same for cosh(r)
        glm::dvec4 u = target - cosh(r) * basepoint;
        return (r/sinh(r)) * u;
    }

    /** \brief Exponential map with basepoint (0,0,0,1)
     * \param One glm dvec4 for the direction
     * \return Exponentiated dvec4
     */
    glm::dvec4 exp0(const glm::dvec4 direction)
    {
        return exp(glm::dvec4(0,0,0,1), direction);
    }

    /** \brief Inverse Exponential map with basepoint (0,0,0,1)
     * \param One glm dvec4 for the direction
     * \return Exponentiated dvec4
     */
    glm::dvec4 exp0inv(const glm::dvec4 direction)
    {
        return expinv(glm::dvec4(0,0,0,1), direction);
    }

    /** \brief Computes the angle between two tangent vectors
     * \param Two glm dvec4's
     * \return The angle between the vectors as a double
     */
    double angle(glm::dvec4 v, glm::dvec4 w)
    {
        return acos(dot(normalize(v),normalize(w)));
    }

    /** \brief Computes the angle determined by three points, it's measured at the middle parameter
     * \param Three glm dvec4's. The middle one denotes the point from which we measure the angle
     * \return The angle between the vectors as a double
     */
    double angle(glm::dvec4 a, glm::dvec4 b, glm::dvec4 c)
    {
        return angle(expinv(b,a),expinv(b,c));
    }

    /** Computes a translation matrix ((wiki) moves every point by a fixed distance in the same direction) 
     * this one is a translation starting at (0,0,0,1)
     * \param One glm dvec4 for the direction of the translation.
     * \return glm 4x4 matrix of doubles
     */
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

    /** \brief This calculates the inverse matrix of the one calculated by translation0
     * \param Target of the translation
     * \return one glm 4x4 matrix
     */
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

    /** \brief Computes the translation that maps source to target (in matrix form). This is the generalization of
     * translation0 for arbitrary starting point.
     * \param Two glm dvec4's. The first is the source, the second is the target
     * \return One glm 4x4 matrix of doubles
     */
    glm::dmat4 translation(glm::dvec4 source, glm::dvec4 target)
    {
        glm::dmat4 Q = translation0inv(source);
        glm::dvec4 intermediate_target = Q*target;
        return translation0(source) * translation0(intermediate_target) * Q;
    }

    /** \brief Rotation around a quaternion starting at origin. At origin, x,y,z have rotational symmetry
     * and we can use regular Eulerian rotation. Quaternion defines axis of rotation and degrees of rotation.
     * \param One glm double quaternion
     * \return glm 4x4 matrix
     */
    glm::dmat4 rotation0(glm::dquat rotation)
    {
        return glm::mat4_cast(rotation);
    }
    
    /** \brief Project quaternion into a rotation around z-axis
     * \param One glm double quaternion
     * \return glm 4x4 matrix
     */
    glm::dmat4 rotationz(glm::dquat rotation)
    {
        rotation[0] = 0;
        rotation[2] = 0;
        return glm::mat4_cast((glm::normalize(rotation)));
    }

    /** \brief Computes the inverse of the rotation for a given quaternion
     * \param One glm double quaternion
     * \return Inverse 4x4 double matrix of the given quaternion
     */
    glm::dmat4 rotation0inv(glm::dquat rotation)
    {
        glm::dquat rotateinv = glm::inverse(rotation);
        return rotation0(rotateinv);
    }

    /** \brief Compute the rotation matrix from a given basepoint and a quaternion
     * \param glm dvec4 for the basepoint, and a glm dquat for the rotation.
     * \return the glm 4x4 double matrix
     */
    glm::dmat4 rotation(glm::dvec4 basepoint, glm::dquat rotate)
    {
        glm::dmat4 rMatrix = rotation0(rotate);
        glm::dvec4 target = rMatrix*basepoint;
        return translation0(target)*rMatrix*translation0inv(basepoint);
    }
    /** \brief Compute the inverse rotation matrix from a given basepoint and a quaternion
     *  \param One glm dvec4 for the basepoint, and one glm dquat for the rotation
     *  \return The inverse glm 4x4 double matrix of the rotation.
     */
    glm::dmat4 rotationinv(glm::dvec4 basepoint, glm::dquat rotate)
    {
        return rotation(basepoint, glm::inverse(rotate));
    }

    /** \brief Rotates space while moving. not sure if this works completely correctly.
     * simple changes will fix. but if this works its faster than a multiplication 
     * of our rmatrix and translation functions by a couple calculations
     * \param basepoint double vector, target double vector, and the rotation quaternion
     * \return glm Double 4x4 matrix for the movement
     */
    glm::dmat4 movement(glm::dvec4 basepoint, glm::dvec4 target, glm::dquat rotate)
    {
        glm::dmat4 rMatrix = rotation0(rotate);
        glm::dvec4 newTarget = rMatrix*target;
        return translation0(newTarget)*rMatrix*translation0inv(basepoint);
    }
    /** \brief Calculates the point at double t between two vectors. (ex) use 0.5 for midpoint
     * \param two double vectors, and the double t for how far between them
     * \return Vector at given position between the two vectors
     */
    glm::dvec4 midpoint(glm::dvec4 a, glm::dvec4 b, double t)
    {
        return hypermath::exp(b,t*hypermath::expinv(b,a));
    }

    /** \brief Returns vector towards a point at (0,-Infinity,0, Infinity) TODO: fix this code, and indent it correctly
     * 
     */
    glm::dvec4 gravity(glm::dvec4 basepoint){
        double a = basepoint.x;
        double b = basepoint.y;
        double c = basepoint.z;
        double d = basepoint.w;
        double bd=b+d;
        glm::dvec4 dir = glm::dvec4(a*(bd),1+b*(bd),c*(bd),-1+d*(bd));
        double norm = -1/sqrt(2*d*d*bd*bd+3+a*a+c*c);
        return norm*dir;
    }
    /** \brief Calculates the radius of circle to inscribe an ngon which has angles alpha.
    * \param The angle in which we want to achieve
    * \return The radius to inscribe the ngon (as a double) 0 if it is not possible to make an ngon with those angles.
    */
    double radius_for_ngon(const double angle, const int sides){
        double PI = 3.1415926535897932384626;
        if(sides > 2){
            double radius = (cos(angle/2)+cos(angle/2)*cos((2*PI)/sides))/(sin(angle/2)*sin((2*PI)/sides));
            radius = glm::acosh(radius);
            return radius;
        }
        // Error (for whatever reason, currently only when the sides are less than 3)
        else{
            return 0.0;
        }
    }

    /** \brief Reflects a point about a line in the hyperbolic plane
     * \param point p of the form (x,0,z,w), and line designeted by the endpoints line_Q and line_R all as dvec4s
     * \return reflected point as a dvec4. If this function returns (0,0,0,0) then an error has occured. Either the 
     * point p was not on the correct plane, or Q was unable to be translated to origin
     */
    glm::dvec4 reflect_planar_point(glm::dvec4 p, glm::dvec4 line_Q, glm::dvec4 line_R){
        //first generate translations before modifying line_Q
        glm::dmat4 q_origin = translation(line_Q, glm::dvec4(0.0,0.0,0.0,1.0));
        glm::dmat4 q_origin_inv = translation(glm::dvec4(0.0,0.0,0.0,1.0), line_Q);

        //if point is not on the hyperbolic plane we simply return our "bad" value
        if(p[1]!=0){
            return glm::dvec4(0.0, 0.0, 0.0, 0.0);
        }

        //First we translate Q, P, and R to the origin.
        p = q_origin * p;
        line_Q = q_origin * line_Q;
        line_R = q_origin * line_R;
        //Next take the inverse exponential map
        p = exp0inv(p);
        line_Q = exp0inv(line_Q);
        line_R = exp0inv(line_R);
        //reflect about the line
        glm::dvec4 reflected_point = 2*(dot(p,line_R)/dot(line_R, line_R))*line_R - p;
        //put the new point back where it belongs
        reflected_point = exp0(reflected_point);
        reflected_point = q_origin_inv * reflected_point;
        return reflected_point;
    }

    /** \brief Differential of exp0
     * \param The first parameter is a tangent vector at origin (w coordinate should be zero)
     * \param The second parameter is the tangent vector (at v) to the tangent space at origin 
     * \return dvec4 of the differential of exp0 multiplied by n
     */
    glm::dvec4 d_exp0(const glm::dvec4 v, const glm::dvec4 n){
        glm::dmat4 output;

        //set first column according to mathematica notebook
        output[0][0] = sin(1.0) * (pow(v.w, 2) - pow(v.y,2) - pow(v.z,2)) - pow(v.x,2) * cos(1.0);
        output[1][0] = v.x * v.y * (sin(1.0) - cos(1.0));
        output[2][0] = v.x * v.z * (sin(1.0) - cos(1.0));
        output[3][0] = v.x * (v.w * (sin(1.0)-cos(1.0)) + sin(1.0));

        //set second column similarly
        output[0][1] = v.x * v.y * (sin(1.0) - cos(1.0));
        output[1][1] = sin(1.0) * (pow(v.w, 2) - pow(v.x, 2) - pow(v.z, 2)) - pow(v.y, 2)*cos(1.0);
        output[2][1] = v.y * v.z * (sin(1.0) - cos(1.0));
        output[3][1] = v.y * (v.w * (sin(1.0) - cos(1.0)) + sin(1.0));

        //set third column
        output[0][2] = v.x * v.z * (sin(1.0) - cos(1.0));
        output[1][2] = v.y * v.z * (sin(1.0) - cos(1.0));
        output[2][2] = sin(1.0) * (pow(v.w, 2) - pow(v.x, 2) - pow(v.y, 2)) - pow(v.z, 2)*cos(1.0);
        output[3][2] = v.z * (v.w * (sin(1.0) - cos(1.0)) + sin(1.0));

        //set fourth column
        output[0][3] = v.w * v.x * (cos(1.0)-sin(1.0));
        output[1][3] = v.w * v.y * (cos(1.0)-sin(1.0));
        output[2][3] = v.w * v.z * (cos(1.0)-sin(1.0));
        output[3][3] = -1 * sin(1.0) * (pow(v.w,3) - v.w * (pow(v.x,2) + pow(v.y, 2) + pow(v.z,2)) + pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) ) + pow(v.w, 2) * cos(1.0);
        return output * n;
    }
}

