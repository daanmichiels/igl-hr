#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "../data/scene.h"

class LevelManager {
public:
    static bool startup();
    static void shutdown();

    static Scene scene;

private:
    static void load_initial_level();
    static void load_level_2();
};

#endif

