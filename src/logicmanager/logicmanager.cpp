#include "logicmanager.h"
#include "../logmanager/logmanager.h"
#include "../data/mesh.h"
#include "../math/primitives.h"

std::vector<glm::dvec4> LogicManager::flags = std::vector<glm::dvec4>();

bool LogicManager::startup() {
    LogManager::log_info("LogicManager started.", 2);
    mesh flag_mesh = primitives::sphere(0.1, 2, glm::dvec4(1.0, 1.0, 0.0, 1.0));
    return true;
}

void LogicManager::shutdown() {
    LogManager::log_info("LogicManager stopped.", 2);
}

void LogicManager::add_flag() {
    LogManager::log_info("Adding flag.", 2);
}

void LogicManager::remove_flag() {
    LogManager::log_info("Removing flag.", 2);
}

void LogicManager::mark_flag() {
    LogManager::log_info("Marking flag.", 2);
}
