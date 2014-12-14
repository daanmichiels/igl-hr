#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    static void log_fatal_error(std::string message);
    static void log_error(std::string message);
    static void log_warning(std::string message);
    static void log_info(std::string message);
};

#endif

