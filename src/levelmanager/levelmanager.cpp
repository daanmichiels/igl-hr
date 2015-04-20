
#include "levelmanager.h"
#include "../configuration/configuration.h"
#include "../logmanager/logmanager.h"
#include "../math/primitives.h"
#include "../math/hypermath.h"
#include "../assetmanager/assetmanager.h"
#include "../data/object.h"
#include "../math/tilings.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cmath>
#include "glm/glm.hpp"

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
    //-------------

    mesh axe_mesh = primitives::axes(10.0);
    object* axes = new object(axe_mesh);
    axes->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.0, -0.097, 0.0, 0.0))));
    scene.objects.push_back(axes);

    //-------------

    load_level("Level1");
    return;
}

void LevelManager::load_level_2(){
    /* This will be phased out as we can save a scene to a level */
    LogManager::log_info("No second level yet.", 2);
    const double PI = 3.141592653589793238463;
    const double big_radius = hypermath::radius_for_ngon(PI/3, 12);
    mesh circ_gon = primitives::circumscribed_ngon(12, big_radius, glm::dvec4(.761, .137, .149, 1.0));
    object* circ_gon_ob = new object(circ_gon);
    circ_gon_ob->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.0,-0.0006,0.0,0.0))));
    scene.objects.push_back(circ_gon_ob);

    mesh big_circ = primitives::circumscribed_ngon(300, big_radius, glm::dvec4(.953, .451, .22, 1.0));
    object* big_circ_ob = new object(big_circ);
    big_circ_ob->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.0,-0.0009,0.0,0.0))));
    scene.objects.push_back(big_circ_ob);

    double smallRad = hypermath::radius_for_ngon(4 * PI / 7, 7);
    double circleRad = asinh(sin(2*PI/7) * glm::sinh(smallRad));
    object* insc_gon = new object(primitives::circumscribed_ngon(7, smallRad, glm::dvec4(.992, .714, .196, 1.0)));
    insc_gon->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.0,-0.0003,0.0,0.0))));
    scene.objects.push_back(insc_gon);

    object* small_circ = new object(primitives::circumscribed_ngon(300, circleRad, glm::dvec4(.502, .086, .22, 1.0)));
    scene.objects.push_back(small_circ);

    object* eucl_circ_1 = new object(primitives::circumscribed_ngon(300, 0.003, glm::dvec4(0.2, 0.6, 1.0, 1.0)));
    eucl_circ_1->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.0, 0.000001, 0.0, 0.0))));
    scene.objects.push_back(eucl_circ_1);

    object* eucl_circ_2 = new object(primitives::circumscribed_ngon(6, 0.003, glm::dvec4(.992, .714, .196, 1.0)));
    eucl_circ_2->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.0, 0.000002, 0.0, 0.0))));
    scene.objects.push_back(eucl_circ_2);

    object* eucl_circ_3 = new object(primitives::ngon(6, atanh(tanh(0.003)/cos(PI/6))));
    eucl_circ_3->transform(hypermath::translation0(hypermath::exp0(glm::dvec4(0.0, 0.0000005, 0.0, 0.0))));
    scene.objects.push_back(eucl_circ_3);
}

void LevelManager::load_level(const char *filename){
    std::vector<object*> objects = AssetManager::parse_lev(filename);
    for(int i=0; i < objects.size(); i++){
        scene.objects.push_back(objects.at(i));
    }
}