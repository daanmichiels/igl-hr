#ifndef RIFTMANAGER_H
#define RIFTMANAGER_H

#include "OVR.h"

class RiftManager {
public:
    static bool startup();
    static void shutdown();

	static bool rift_connected;
	static ovrHmd hmd;
};

#endif
