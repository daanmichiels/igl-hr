#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "../data/scene.h"

class LevelManager {
public:
    static bool startup();
    static void shutdown();

    static Scene scene;
};

#endif

