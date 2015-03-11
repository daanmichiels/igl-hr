#ifndef LOGICMANAGER_H
#define LOGICMANAGER_H

#include <vector>
#include "glm/glm.hpp"
#include "../data/object.h"

class LogicManager {
public:
    static bool startup();
    static void shutdown();
    static void add_flag();
    static void remove_flag();
    static void mark_flag();
    static void handle();
    static std::vector<object> flags;
    static std::vector<glm::dvec4> flag_locations;
    static std::vector<glm::dvec3> flag_highlights;
};

#endif /* LOGICMANAGER_H */

