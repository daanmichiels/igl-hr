#ifndef FLAGMANAGER_H
#define FLAGMANAGER_H
#include "object.h"
#include "cameracontrols.h"
#include "scene.h"

class FlagManager
{
    private:
        object _flag;
        int _flag_num;
        Scene* _scene;
        std::vector<object*> _flag_vec;
        CameraControls* _camera_controls;

    public:
        FlagManager(Scene s, CameraControls camc);
        FlagManager();
        void drop_flag();
        int num_flags();
        object* return_last_flag();
};

#endif