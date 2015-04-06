
#include "levelmanager.h"
#include "../configuration/configuration.h"
#include "../logmanager/logmanager.h"
#include "../math/primitives.h"
#include "../math/hypermath.h"
#include "../assetmanager/assetmanager.h"
#include "../data/object.h"
#include "../math/tilings.h"
#include <cmath>

Scene LevelManager::scene = Scene();

/** \brief Startup LevelManager. Logs LevelManager started at level 2
 * \param void
 * \return boolean true
 */
bool LevelManager::startup() {
    load_initial_level();
    LogManager::log_info("LevelManager started.", 2);
    return true;
}

/** \brief Shutdown levelmanager. Logs levelmanager stopped at level 2
 * \param void
 * \return void
 */
void LevelManager::shutdown() {
    LogManager::log_info("LevelManager stopped.", 2);
}

/** \brief Loads initial level (for example it is currently a tetrahedron)
 * \param void
 * \return void
 */
void LevelManager::load_initial_level() {
    //axes to give the user a feeling of where they are in the space.
    const double PI = 3.141592653589793238463;

    //-------------

    mesh axe_mesh = primitives::axes(10.0);
    object* axes = new object(axe_mesh);
    axes->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.0, -0.097, 0.0, 0.0))));
    scene.objects.push_back(axes);

    //-------------

    mesh tile = tilings::generate_tiling(5, 4, 5);
    object* tiling = new object(tile);
    scene.objects.push_back(tiling);

    //-----------

    object* suzy = AssetManager::load_object("resources/suzy.obj", false, .001);
    suzy->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.003, 0.003, -0.001, 0.0))));
    scene.objects.push_back(suzy);

    object* stuff = AssetManager::load_object("resources/stuff.obj", false, .001);
    stuff->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.003, 0.0, -0.019, 0.0))));
    scene.objects.push_back(stuff);
    
    object* pi = AssetManager::load_object("resources/pi.obj", false, .001);
    pi->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.003, 0.0, 0.039, 0.0))));
    scene.objects.push_back(pi);

    //------------
    
	LogManager::log_info("Enjoy Your Pi", 2);

    return;
}

void LevelManager::load_level_2(){
    LogManager::log_info("Level Suzy.", 2);
    object* suzy = AssetManager::load_object("resources/suzy.obj", false, .2);
    suzy->transform(hypermath::translation0(glm::dvec4(0,0,-0.2,sqrt(1+0.04))));
    scene.objects.push_back(suzy);
}

