
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

void shutdown_managers(std::vector<bool (*) ()> shutdowns) {
    for(int j=shutdowns.size()-1; j>=0; j--) {
        shutdowns[j]();
    }
}

int main(int argc, const char* argv[]) {

    // list of calls that are needed to shut down everything we started
    // vector of function pointers (for some reason, the compiler requires
    // a cast)
    std::vector<bool (*) ()> shutdowns = {};

    // first thing we need
    // all other managers log errors using this
    if(LogManager::startup()) {
        shutdowns.push_back((bool (*) ()) &(LogManager::shutdown));
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
        shutdowns.push_back((bool (*) ()) &(RiftManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // start opengl, open a window
    if(RenderManager::startup()) {
        shutdowns.push_back((bool (*) ()) &(RenderManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // handle input
    if(InputManager::startup()) {
        shutdowns.push_back((bool (*) ()) &(InputManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // load and compile shaders
    if(ShaderManager::startup()) {
        shutdowns.push_back((bool (*) ()) &(ShaderManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // load the world
    if(LevelManager::startup()) {
        shutdowns.push_back((bool (*) ()) &(LevelManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // start handling characters
    if(CharacterManager::startup()) {
        shutdowns.push_back((bool (*) ()) &(CharacterManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // set up game loop
    if(LoopManager::startup()) {
        shutdowns.push_back((bool (*) ()) &(LoopManager::shutdown));
    } else {
        shutdown_managers(shutdowns);
        return 0;
    }

    // Run! (forrest?)
    LoopManager::run();

    // shut down everything
    shutdown_managers(shutdowns);
    return 0;
}
