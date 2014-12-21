#ifndef RIFTMANAGER_H
#define RIFTMANAGER_H

#include "OVR/OVR.h"

class RiftManager {
public:
    static bool startup();
    static void shutdown();

	static bool rift_active;
	static ovrHmd rift;
};

#endif
