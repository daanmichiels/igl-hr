
#include <iostream>
#include "logger.h"

void Logger::log_fatal_error(std::string message) {
    std::cout << "Fatal error: " << message << std::endl;
}

void Logger::log_error(std::string message) {
    std::cout << "Error:   " << message << std::endl;
}

void Logger::log_warning(std::string message) {
    std::cout << "Warning: " << message << std::endl;
}

void Logger::log_info(std::string message) {
    std::cout << "Info:    " << message << std::endl;
}

