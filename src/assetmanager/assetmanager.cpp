
#include "../logmanager/logmanager.h"
#include "assetmanager.h"

void AssetManager::startup() {
    LogManager::log_info("AssetManager started.", 2);
}

void AssetManager::shutdown() {
    LogManager::log_info("AssetManager stopped.", 2);
}

