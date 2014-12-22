
#include "levelmanager.h"
#include "../configuration/configuration.h"
#include "../logmanager/logmanager.h"
#include "../math/primitives.h"

Scene LevelManager::scene = Scene();

bool LevelManager::startup() {

    mesh m = primitives::tetrahedron(0.04);
    //TODO: get rid of this heap allocation
    //(but we can't make it a stack variable)
    //We need a MemoryManager
    object* o = new object(m);
    scene.objects.push_back(o);

    LogManager::log_info("LevelManager started.", 2);
    return true;
}

void LevelManager::shutdown() {
    LogManager::log_info("LevelManager stopped.", 2);
}

