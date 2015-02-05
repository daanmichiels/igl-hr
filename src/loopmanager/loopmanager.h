#include "fpscounter.h"

class LoopManager {
public:
    static bool startup();
    static void shutdown();
    static void run();

    static FpsCounter fpscounter;
};

