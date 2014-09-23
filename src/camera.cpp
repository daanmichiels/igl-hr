#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/matrix_transform.hpp"

#include <GL/glew.h>
#include "camera.h"
using namespace glm;
// By default, the camera is at (0,0,0,1) and looking in the
// negative z-direction (0,0,-1,0).
Camera::Camera(float fov, float ratio, float near, float far)
{
    _fov = fov;
    _ratio = ratio;
    _near = near;
    _far = far;
    _proj = glm::perspective(fov, ratio, near, far);
    _view = glm::mat4(); //defaults to identity matrix
}

// Return the camera's current projection matrix.
glm::mat4 Camera::get_projection()
{
    return _proj;
}

// Return the camera's current view matrix.
glm::mat4 Camera::get_view()
{
    return _view;
}

// Change the camera's aspect ratio. Useful when resizing
// the window.
void Camera::set_ratio(float ratio)
{
    if(_ratio == ratio)
        return;

    _ratio = ratio;
    _proj = glm::perspective(_fov, _ratio, _near, _far);
}

// Applies a transformation to the camera, like moving
// or rotating.
// The matrix supplied must be the *inverse* of the trans-
// formation you want to apply. This avoids explicitly inverting
// the matrix, which is inferior to generating the inverse from the
// start.
void Camera::transform(glm::mat4 transformationinv)
{
    _view = _view * transformationinv;
}

// Sets the transformation of the camera.
// The matrix supplied must be the *inverse* of the trans-
// formation you want.
void Camera::set_transformation(glm::mat4 transformationinv)
{
    _view = transformationinv;
}
void Camera::rotx(double xang){
mat4 rxI;
double hsin=sinh(xang);
double hcos=cosh(xang);
rxI[0][0]=1.0;
rxI[0][1]=0.0;
rxI[0][2]=0.0;
rxI[0][3]=0.0;
rxI[1][0]=0.0;
rxI[1][1]=hcos;
rxI[1][2]=0.0-hsin;
rxI[1][3]=0.0;
rxI[2][0]=0.0;
rxI[2][1]=hsin;
rxI[2][2]=hcos;
rxI[2][3]=0.0;
rxI[3][0]=0.0;
rxI[3][1]=0.0;
rxI[3][2]=0.0;
rxI[3][3]=1.0;
Camera::transform(rxI);
}
void Camera::roty(double yang){
mat4 ryI;
double hsin=sinh(yang);
double hcos=cosh(yang);
ryI[0][0]=hcos;
ryI[0][1]=0.0;
ryI[0][2]=hsin;
ryI[0][3]=0.0;
ryI[1][0]=0.0;
ryI[1][1]=1.0;
ryI[1][2]=0.0;
ryI[1][3]=0.0;
ryI[2][0]=-1*hsin;
ryI[2][1]=0.0;
ryI[2][2]=hcos;
ryI[2][3]=0.0;
ryI[3][0]=0.0;
ryI[3][1]=0.0;
ryI[3][2]=0.0;
ryI[3][3]=1.0;
Camera::transform(ryI);
}
void Camera::rotz(double zang){
mat4 rzI;
double hsin=sinh(zang);
double hcos=cosh(zang);
rzI[0][0]=hcos;
rzI[0][1]=-1*hsin;
rzI[0][2]=0.0;
rzI[0][3]=0.0;
rzI[1][0]=hsin;
rzI[1][1]=hcos;
rzI[1][2]=0.0;
rzI[1][3]=0.0;
rzI[2][0]=0.0;
rzI[2][1]=0.0;
rzI[2][2]=1.0;
rzI[2][3]=0.0;
rzI[3][0]=0.0;
rzI[3][1]=0.0;
rzI[3][2]=0.0;
rzI[3][3]=1.0;
Camera::transform(rzI);
}

