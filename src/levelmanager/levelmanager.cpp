
#include "levelmanager.h"
#include "../configuration/configuration.h"
#include "../logmanager/logmanager.h"
#include "../math/primitives.h"
#include "../math/hypermath.h"
#include "../assetmanager/assetmanager.h"
#include "../data/object.h"

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
    mesh axe_mesh = primitives::axes(10.0);
    object* axes = new object(axe_mesh);
    axes->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.0, -0.099, 0.0, 0.0))));
    scene.objects.push_back(axes);

    //setup the large circle and circumscribed hexagon.
    const double big_radius = 4.0;
    mesh circ_gon = primitives::circumscribed_ngon(6, big_radius, glm::dvec4(0.5,0.5,0.5,0.25));
    object* circ_gon_ob = new object(circ_gon);
    circ_gon_ob->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.0,-0.1,0.0,0.0))));
    scene.objects.push_back(circ_gon_ob);

    mesh big_circ = primitives::circumscribed_ngon(100, big_radius, glm::dvec4(1.0,0.79,0.4,1.0));
    object* big_circ_ob = new object(big_circ);
    big_circ_ob->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.0,-0.1001,0.0,0.0))));
    scene.objects.push_back(big_circ_ob);


	LogManager::log_info("Enjoy Your Pi", 2);

    return;
}

void LevelManager::load_level_2(){
    LogManager::log_info("Level Lucy.", 2);
    object* lucy = AssetManager::load_object("resources/suzy.obj", false, .2);
    lucy->transform(hypermath::translation0(glm::dvec4(0,0,-0.2,sqrt(1+0.04))));
    scene.objects.push_back(lucy);
}

