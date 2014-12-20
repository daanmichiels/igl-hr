
#include "charactermanager.h"
#include "../logmanager/logmanager.h"

bool CharacterManager::startup() {
    LogManager::log_info("CharacterManager started", 2);
    return true;
}

void CharacterManager::shutdown() {
    LogManager::log_info("CharacterManager stopped.", 2);
}
