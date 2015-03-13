#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "../data/object.h"

class AssetManager {
public:
    static bool startup();
    static void shutdown();
	static object* load_object(std::string name);

private:
    static mesh mesh_from_data(float* data, int data_size, bool has_pos, bool has_col, bool has_nor);
};

#endif
