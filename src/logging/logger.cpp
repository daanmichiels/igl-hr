
#include <iostream>
#include "logger.h"

#ifdef WIN32
#include <windows.h>
#include <winnt.h>

// TODO: store hConsole handle during initialization

void Logger::log_fatal_error(std::string message) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 0x0C);
    std::cout << "Fatal error: ";
    std::cout << message << std::endl;
    SetConsoleTextAttribute(hConsole, 0x07);
}

void Logger::log_error(std::string message) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 0x0C);
    std::cout << "Error:   ";
    SetConsoleTextAttribute(hConsole, 0x07);
    std::cout << message << std::endl;
}

void Logger::log_warning(std::string message) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 0x0E);
    std::cout << "Warning: ";
    SetConsoleTextAttribute(hConsole, 0x07);
    std::cout << message << std::endl;
}

void Logger::log_info(std::string message) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 0x0A);
    std::cout << "Info:    ";
    SetConsoleTextAttribute(hConsole, 0x07);
    std::cout << message << std::endl;
}

#else

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
#endif

