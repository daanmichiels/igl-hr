
#include <vector>
#include "init/init.h"
#include "logmanager/logmanager.h"
#include "configuration/configuration.h"
#include "shadermanager/shadermanager.h"
#include "assetmanager/assetmanager.h"
#include "loopmanager/loopmanager.h"
#include "levelmanager/levelmanager.h"
#include "rendermanager/rendermanager.h"
#include "charactermanager/charactermanager.h"
#include "riftmanager/riftmanager.h"
#include "inputmanager/inputmanager.h"
#include "logicmanager/logicmanager.h"
//#include "math/hypermath.h"
//#include "glm/glm.hpp"
#include <string.h>

/** \brief Shutdown the various managers
 * \param vector of managers to shutdown
 * \return void
 */
void shutdown_managers(std::vector<void (*) ()> shutdowns) {
    for(int j=shutdowns.size()-1; j>=0; j--) {
        shutdowns[j]();
    }
}

int main(int argc, const char* argv[]) {
    // list of calls that are needed to shut down everything we started
    // vector of function pointers (for some reason, the compiler requires
    // a cast)
    std::vector<void (*) ()> shutdowns = {};

    // first thing we need
    // all other managers log errors using this
    if(LogManager::startup()) {
        shutdowns.push_back(&(LogManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // say hello
    Init::welcome_message();

    // read configuration from command line (and config files?)
    Configuration::configure(argc, argv);

    // detect rift, set it up
    if(RiftManager::startup()) {
        shutdowns.push_back(&(RiftManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // start opengl, open a window
    if(RenderManager::startup()) {
        shutdowns.push_back(&(RenderManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // handle input
    if(InputManager::startup()) {
        shutdowns.push_back(&(InputManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // load and compile shaders
    if(ShaderManager::startup()) {
        shutdowns.push_back(&(ShaderManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // load the world
    if(LevelManager::startup()) {
        shutdowns.push_back(&(LevelManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // start handling characters
    if(CharacterManager::startup()) {
        shutdowns.push_back(&(CharacterManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // make the game playable
    if(LogicManager::startup()) {
        shutdowns.push_back(&(LogicManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // set up game loop
    if(LoopManager::startup()) {
        shutdowns.push_back(&(LoopManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

	///*
	glm::dvec4 pos = glm::dvec4(5,12,4,0);
	glm::dvec4 dir = glm::dvec4(2,2,8,0);
	glm::dvec4 result = hypermath::d_exp0(pos, dir);
	long double x = result.x;
	long double y = result.y;
	long double z = result.z;
	long double w = result.w;
	std::string txt = std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w);
	LogManager::log_info(txt, 2);
	//*/
	
    // Run! (forrest?)
    LoopManager::run();

    // shut down everything
    shutdown_managers(shutdowns);
    return 0;
}
