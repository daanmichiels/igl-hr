
#include "../logmanager/logmanager.h"
#include "shadermanager.h"

void ShaderManager::startup() {
    LogManager::log_info("ShaderManager started.", 2);
}

void ShaderManager::shutdown() {
    LogManager::log_info("ShaderManager stopped.", 2);
}

