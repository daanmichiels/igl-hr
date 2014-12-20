
#include "../logmanager/logmanager.h"
#include "shadermanager.h"

bool ShaderManager::startup() {
    LogManager::log_info("ShaderManager started.", 2);
    return true;
}

void ShaderManager::shutdown() {
    LogManager::log_info("ShaderManager stopped.", 2);
}

