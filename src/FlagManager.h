#ifndef FLAGMANAGER_H
#define FLAGMANAGER_H
#include "object.h"
#include "cameracontrols.h"
#include "scene.h"
#include <vector>

class FlagManager
{
    private:
        object _flag;
        int _flag_num;
        Scene* _scene;
        CameraControls* _camera_controls;
        std::vector<object*> _flag_vec;


    public:
        FlagManager(Scene* s, CameraControls camc);
        FlagManager();
        void drop_flag();
};

#endif