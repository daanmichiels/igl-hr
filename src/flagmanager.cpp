#include "object.h"
#include "cameracontrols.h"
#include "flagmanager.h"
#include "../thirdparty/glm/glm/glm.hpp"
#include "scene.h"
#include "hypermath.h"
#include "primitives.h"
#include <iostream>

flagmanager::flagmanager(Scene* s, CameraControls camc)
{
    //setup our flag
    _flag_num = 0;
    _flag = primitives::sphere(0.01f, 4, glm::dvec4(0.0f, 1.0f, 0.0f, 0.0f));
    _flags = std::vector<object*>();
    _scene = s;
    _camera_controls = &camc;
}

void flagmanager::drop_flag()
{
    std::cout << "Dropping Flag" << std::endl;
    object* new_flag = new object();
    new_flag->name = "flag" + std::to_string(_flag_num);
    new_flag->meshes.push_back(_flag);
    new_flag->set_transformation(glm::inverse(_camera_controls->get_cam_view()));
    _flags.push_back(new_flag);

    if(_scene != NULL)
    {
        _scene->objects.push_back(new_flag);
    }
    _flag_num++;
}

