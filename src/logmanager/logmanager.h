#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <string>

class LogManager {
public:
    static void startup();
    static void log_error(std::string message);
    static void log_warning(std::string message);
    static void log_info(std::string message);
    static void shutdown();
};

#endif

