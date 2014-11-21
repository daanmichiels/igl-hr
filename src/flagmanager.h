#ifndef FLAGMANAGER_H
#define FLAGMANAGER_H
#include "object.h"
#include "cameracontrols.h"
#include "scene.h"
#include <vector>

class flagmanager
{
    private:
        mesh _flag;
        int _flag_num;
        Scene* _scene;
        CameraControls* _camera_controls;
        std::vector<object*> _flags;

    public:
        flagmanager(Scene* s, CameraControls camc);
        flagmanager();
        void drop_flag();
};

#endif
