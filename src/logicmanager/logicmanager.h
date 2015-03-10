#ifndef LOGICMANAGER_H
#define LOGICMANAGER_H

#include <vector>
#include "glm/glm.hpp"

class LogicManager {
public:
    static bool startup();
    static void shutdown();
    static void add_flag();
    static void remove_flag();
    static void mark_flag();

private:
    static std::vector<glm::dvec4> flags;
};

#endif /* LOGICMANAGER_H */

