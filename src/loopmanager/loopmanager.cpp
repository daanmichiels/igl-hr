
#include "../logmanager/logmanager.h"
#include "loopmanager.h"

void LoopManager::startup() {
    LogManager::log_info("LoopManager started.", 2);
}

void LoopManager::shutdown() {
    LogManager::log_info("LoopManager stopped.", 2);
}

