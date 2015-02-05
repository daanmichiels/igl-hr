#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <string>

class LogManager {
public:
    static bool startup();
    static void log_error(std::string message);
    static void log_warning(std::string message, int level);
    static void log_info(std::string message, int level);
    static void log_general(std::string message, int level);
    static void shutdown();
};

#endif

