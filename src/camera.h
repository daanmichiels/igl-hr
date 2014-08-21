#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/matrix_transform.hpp"

class Camera
{
    private:
        float _fov, _ratio, _near, _far;
        glm::mat4 _proj;
        glm::mat4 _view;
    public:
        Camera(float fov, float ratio, float near, float far);
        glm::mat4 get_projection();
        glm::mat4 get_view();
        void set_ratio(float ratio);
        void transform(glm::mat4 transforminv);
};

#endif
