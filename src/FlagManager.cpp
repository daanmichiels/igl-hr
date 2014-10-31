#include "object.h"
#include "cameracontrols.h"
#include "FlagManager.h"
#include "../thirdparty/glm/glm/glm.hpp"
#include "scene.h"
#include "hypermath.h"
#include "primitives.h"

FlagManager::FlagManager(Scene s, CameraControls camc)
{
    _flag_num = 0;
    glm::vec4 a = hypermath::exp0(glm::vec4(.2,0,0,0));
    glm::vec4 b  = hypermath::exp0(glm::vec4(0,.2,0,0));
    glm::vec4 c = hypermath::exp0(glm::vec4(0,-.2,0,0));
    mesh mesh_triangle = primitives::triangle(a,b,c);
    _flag.meshes.push_back(mesh_triangle);

    _scene = &s;
    _camera_controls = &camc;
}
//empty constructor
FlagManager::FlagManager()
{
}

void FlagManager::drop_flag()
{
    object new_flag = object(_flag, _flag_num);
    new_flag.set_transformation(hypermath::translation0inv(_camera_controls->get_forward()));
    _flag_vec.push_back(&new_flag);

    //THIS LINE CAUSES SEGFAULT
//    _scene->objects.push_back(&new_flag);
    _flag_num++;
}
int FlagManager::num_flags()
{
    return _flag_num;
}
object* FlagManager::return_last_flag()
{
    object* last_flag = NULL;
    if(_flag_vec[_flag_num] != NULL)
    {
        last_flag = _flag_vec[_flag_num];
    }
    return last_flag;
}