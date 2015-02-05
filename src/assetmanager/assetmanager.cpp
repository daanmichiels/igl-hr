
#include "../logmanager/logmanager.h"
#include "assetmanager.h"

bool AssetManager::startup() {
    LogManager::log_info("AssetManager started.", 2);
    return true;
}

void AssetManager::shutdown() {
    LogManager::log_info("AssetManager stopped.", 2);
}

