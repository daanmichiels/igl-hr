#include "object.h"
#include "cameracontrols.h"
#include "FlagManager.h"
#include "../thirdparty/glm/glm/glm.hpp"
#include "scene.h"
#include "hypermath.h"
#include "primitives.h"
#include <iostream>

FlagManager::FlagManager(Scene* s, CameraControls camc)
{
    _flag_num = 0;
    glm::vec4 a = hypermath::exp0(glm::vec4(.1,0,0,0));
    glm::vec4 b  = hypermath::exp0(glm::vec4(0,.1,0,0));
    glm::vec4 c = hypermath::exp0(glm::vec4(0,-.1,0,0));
    mesh mesh_triangle = primitives::triangle(a,b,c);
    _flag.meshes.push_back(mesh_triangle);

    _flag_vec = std::vector<object*>();
    _scene = s;
    _camera_controls = &camc;
}
//empty constructor
FlagManager::FlagManager()
{
}

void FlagManager::drop_flag()
{
    object* new_flag = new object(_flag, _flag_num);
    new_flag->set_transformation(glm::inverse(_camera_controls->get_cam_view()));
    _flag_vec.push_back(new_flag);

    if(_scene != NULL)
    {
        _scene->objects.push_back(new_flag);
    }
    _flag_num++;
}