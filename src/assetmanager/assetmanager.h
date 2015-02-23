#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "../data/object.h"

class AssetManager {
public:
    static bool startup();
    static void shutdown();
	static object* load_object(std::string filename, bool converted, double scale);
};

#endif