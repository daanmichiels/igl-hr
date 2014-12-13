#ifndef CAMERA_H
#define CAMERA_H

#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/matrix_transform.hpp"

class Camera
{
    private:
        double _fov, _ratio, _near, _far;
        glm::dmat4 _proj;
        glm::dmat4 _view;
    public:
        Camera(double fov=1.2f, double ratio=1.0f, double near=0.01f, double far=10.0f);
        glm::dmat4 get_projection();
        glm::dmat4 get_view();
        void set_ratio(double ratio);
        void transform(glm::dmat4 transformationinv);
        void set_transformation(glm::dmat4 transformationinv);
};

#endif
