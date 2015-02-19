
#include "levelmanager.h"
#include "../configuration/configuration.h"
#include "../logmanager/logmanager.h"
#include "../math/primitives.h"
#include "../math/hypermath.h"
#include "../assetmanager/assetmanager.h"
#include "../data/object.h"

Scene LevelManager::scene = Scene();

bool LevelManager::startup() {
    load_initial_level();
    LogManager::log_info("LevelManager started.", 2);
    return true;
}

void LevelManager::shutdown() {
    LogManager::log_info("LevelManager stopped.", 2);
}

void LevelManager::load_initial_level() {
    mesh m = primitives::tetrahedron(0.04);
    //TODO: get rid of this heap allocation
    //(but we can't make it a stack variable)
    //We need a MemoryManager
    object* o = new object(m);
    o->transform(hypermath::translation0(glm::dvec4(0,0,-0.2,sqrt(1+0.04))));
    scene.objects.push_back(o);
	LogManager::log_info("Level Lucy.", 2);
	object* lucy = AssetManager::load_object("lucy.obj", false, 1);
	lucy->transform(hypermath::translation0(glm::dvec4(0,0,-0.2,sqrt(1+0.04))));
	scene.objects.push_back(lucy);
	LogManager::log_info("Level Lucy Done.", 2);

    return;
}

