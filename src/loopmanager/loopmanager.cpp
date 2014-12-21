
#include "../logmanager/logmanager.h"
#include "loopmanager.h"

bool LoopManager::startup() {
    LogManager::log_info("LoopManager started.", 2);
    return true;
}

void LoopManager::shutdown() {
    LogManager::log_info("LoopManager stopped.", 2);
}

void LoopManager::run() {
    return;
}

