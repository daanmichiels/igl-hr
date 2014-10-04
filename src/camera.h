#ifndef CAMERA_H
#define CAMERA_H

#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/matrix_transform.hpp"

class Camera
{
    private:
        float _fov, _ratio, _near, _far;
        glm::mat4 _proj;
        glm::mat4 _view;
		glm::vec4 _pos;
    public:
        Camera(float fov=1.2f, float ratio=1.0f, float near=0.01f, float far=10.0f);
        glm::mat4 get_projection();
        glm::mat4 get_view();
		glm::vec4 get_pos();
        void set_ratio(float ratio);
        void transform(glm::mat4 transformationinv);
        void set_transformation(glm::mat4 transformationinv);
};

#endif
