
#include <iostream>
#include "logmanager.h"

#ifdef WIN32
#include <windows.h>
#include <winnt.h>

// TODO: store hConsole handle during initialization

void LogManager::log_error(std::string message) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 0x0C);
    std::cout << "Error:   ";
    SetConsoleTextAttribute(hConsole, 0x07);
    std::cout << message << std::endl;
}

void LogManager::log_warning(std::string message) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 0x0E);
    std::cout << "Warning: ";
    SetConsoleTextAttribute(hConsole, 0x07);
    std::cout << message << std::endl;
}

void LogManager::log_info(std::string message) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 0x0A);
    std::cout << "Info:    ";
    SetConsoleTextAttribute(hConsole, 0x07);
    std::cout << message << std::endl;
}

#else

void LogManager::log_error(std::string message) {
    std::cout << "Error:   " << message << std::endl;
}

void LogManager::log_warning(std::string message) {
    std::cout << "Warning: " << message << std::endl;
}

void LogManager::log_info(std::string message) {
    std::cout << "Info:    " << message << std::endl;
}
#endif

