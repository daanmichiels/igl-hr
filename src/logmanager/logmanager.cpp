
#include <iostream>
#include "logmanager.h"
#include "../configuration/configuration.h"

#ifdef WIN32
#include <windows.h>
#include <winnt.h>

namespace {
    HANDLE hConsole;
}

bool LogManager::startup() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    log_info("LogManager started.", 2);
    return true;
}

void LogManager::shutdown() {
    // should we unset hConsole?
    log_info("LogManager stopped.", 2);
}

void LogManager::log_error(std::string message) {
    SetConsoleTextAttribute(hConsole, 0x0C);
    std::cout << "Error:   ";
    SetConsoleTextAttribute(hConsole, 0x07);
    std::cout << message << std::endl;
}

void LogManager::log_warning(std::string message, int level) {
    if(Configuration::verbosity < level) {
        return;
    }
    SetConsoleTextAttribute(hConsole, 0x0E);
    std::cout << "Warning: ";
    SetConsoleTextAttribute(hConsole, 0x07);
    std::cout << message << std::endl;
}

void LogManager::log_info(std::string message, int level) {
    if(Configuration::verbosity < level) {
        return;
    }
    SetConsoleTextAttribute(hConsole, 0x0A);
    std::cout << "Info:    ";
    SetConsoleTextAttribute(hConsole, 0x07);
    std::cout << message << std::endl;
}

void LogManager::log_general(std::string message, int level) {
    if(Configuration::verbosity < level) {
        return;
    }
    std::cout << message << std::endl;
}

#else

void LogManager::startup() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void LogManager::shutdown() {
}

void LogManager::log_error(std::string message) {
    std::cout << "Error:   " << message << std::endl;
}

void LogManager::log_warning(std::string message, int level) {
    if(Configuration::verbosity < level) {
        return;
    }
    std::cout << "Warning: " << message << std::endl;
}

void LogManager::log_info(std::string message, int level) {
    if(Configuration::verbosity < level) {
        return;
    }
    std::cout << "Info:    " << message << std::endl;
}

void LogManager::log_general(std::string message, int level) {
    if(Configuration::verbosity < level) {
        return;
    }
    std::cout << message << std::endl;
}

#endif

