
#include "riftmanager.h"
#include "../logmanager/logmanager.h"

bool RiftManager::startup() {
    LogManager::log_info("RiftManager started.", 2);
    return true;
}

void RiftManager::shutdown() {
    LogManager::log_info("RiftManager stopped.", 2);
}

