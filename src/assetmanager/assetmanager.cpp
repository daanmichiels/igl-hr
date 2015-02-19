
#include "../logmanager/logmanager.h"
#include "../data/object.h"
#include "assetmanager.h"

bool AssetManager::startup() {
    LogManager::log_info("AssetManager started.", 2);
    return true;
}

object* AssetManager::load_object(std::string filename, bool converted, double scale) {
	const char * char_filename = filename.c_str();
	object* o = new object(char_filename, converted, scale);
	return o;
}

void AssetManager::shutdown() {
    LogManager::log_info("AssetManager stopped.", 2);
}

