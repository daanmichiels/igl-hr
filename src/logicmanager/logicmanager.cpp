#include "logicmanager.h"
#include "../logmanager/logmanager.h"
#include "../loopmanager/loopmanager.h"
#include "../rendermanager/rendermanager.h"
#include "../charactermanager/charactermanager.h"
#include "../data/mesh.h"
#include "../data/object.h"
#include "../math/primitives.h"
#include "glm/ext.hpp"

std::vector<object> LogicManager::flags = {};
std::vector<glm::dvec4> LogicManager::flag_locations = {};
std::vector<glm::dvec3> LogicManager::flag_highlights = {};

bool LogicManager::startup() {
    LogManager::log_info("LogicManager started.", 2);
    return true;
}

void LogicManager::shutdown() {
    LogManager::log_info("LogicManager stopped.", 2);
}

void LogicManager::add_flag() {
    LogManager::log_info("Adding flag #" + std::to_string(flags.size()) + ".", 2);

    //remove highlight from previous flag
    if(flags.size() > 0) {
        flag_highlights[flags.size() - 1] = glm::dvec3(0,0,0);
    }

    glm::dvec4 pos = CharacterManager::get_position_feet();
    object flag = object(primitives::sphere(0.1 * CharacterManager::meter, 4, glm::dvec4(0.3, 0.3, 0.3, 1.0)));
    flag.transform(hypermath::translation0(pos));
    flags.push_back(flag);
    flag_locations.push_back(pos);
    flag_highlights.push_back(glm::dvec3(0.3, 0.0, 0.0));
}

void LogicManager::remove_flag() {
    LogManager::log_info("Removing flag.", 2);
}

void LogicManager::mark_flag() {
    LogManager::log_info("Marking flag.", 2);
}

void LogicManager::handle() {
    if(flags.size() > 0) {
        flag_highlights[flags.size() - 1] = sin(4*LoopManager::t) * glm::dvec3(0.5, 0.0, 0.0);
    }
}

