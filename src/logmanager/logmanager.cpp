
#include <iostream>
#include "logmanager.h"
#include "../configuration/configuration.h"

#ifdef WIN32
#include <windows.h>
#include <winnt.h>

namespace {
    HANDLE hConsole;
}

/** \brief Start Logmanager logs LogManager Started at level 2 (for windows systems)
 * \param void
 * \return boolean true
 */
bool LogManager::startup() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    log_info("LogManager started.", 2);
    return true;
}

/** \brief Shutdown Logmanager. Logs LogManager stopped at level 2 (for windows systems)
 * \param void
 * \return void
 */
void LogManager::shutdown() {
    // should we unset hConsole?
    log_info("LogManager stopped.", 2);
}

/** \brief Logs an error to logmanager. (for windows systems)
 * \param String for the error message
 * \return void
 */
void LogManager::log_error(std::string message) {
    SetConsoleTextAttribute(hConsole, 0x0C);
    std::cout << "Error:   ";
    SetConsoleTextAttribute(hConsole, 0x07);
    std::cout << message << std::endl;
}

/** \brief Logs a warning at a particular level. (for windows systems)
 * \param String for the error message, and an integer for the level of warning
 * \return void
 */
void LogManager::log_warning(std::string message, int level) {
    if(Configuration::verbosity < level) {
        return;
    }
    SetConsoleTextAttribute(hConsole, 0x0E);
    std::cout << "Warning: ";
    SetConsoleTextAttribute(hConsole, 0x07);
    std::cout << message << std::endl;
}

/** \brief Logs information of a particular level (for windows systems)
 * \param Information as a string, and an integer for the level of the information
 * \return void
 */
void LogManager::log_info(std::string message, int level) {
    if(Configuration::verbosity < level) {
        return;
    }
    SetConsoleTextAttribute(hConsole, 0x0A);
    std::cout << "Info:    ";
    SetConsoleTextAttribute(hConsole, 0x07);
    std::cout << message << std::endl;
}

/** \brief Log a general message of a particular level (for windows systems)
 * \param Message as a string, and an integer for the level of the message
 * \return void
 */
void LogManager::log_general(std::string message, int level) {
    if(Configuration::verbosity < level) {
        return;
    }
    std::cout << message << std::endl;
}

#else

/** \brief Startup logmanager for non-windows systems
 * \param void
 * \return true
 */
bool LogManager::startup() {
    return true;
}

/** \brief Shutdown logmanager (empty function) for non-windows systems
 * \param void
 * \return void
 */
void LogManager::shutdown() {
}

/** \brief Log an error for a non-windows system
 * \param Error message as a string
 * \return void
 */
void LogManager::log_error(std::string message) {
    std::cout << "Error:   " << message << std::endl;
}

/** \brief Log a warning of a particular level for non-windows systems
 * \param Warning as a string, and the level as an integer
 * \return void
 */
void LogManager::log_warning(std::string message, int level) {
    if(Configuration::verbosity < level) {
        return;
    }
    std::cout << "Warning: " << message << std::endl;
}

/** \brief Log info of a particular level for non-windows systems
 * \param Info as a string, and the level as an integer
 * \return void
 */
void LogManager::log_info(std::string message, int level) {
    if(Configuration::verbosity < level) {
        return;
    }
    std::cout << "Info:    " << message << std::endl;
}

/** \brief Log a general message of a particular level for non-windows systems
 * \param Message as a string, and the integer value of the level
 * \return void
 */
void LogManager::log_general(std::string message, int level) {
    if(Configuration::verbosity < level) {
        return;
    }
    std::cout << message << std::endl;
}

#endif

