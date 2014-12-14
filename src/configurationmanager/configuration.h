#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>

// Contains the configuration of the program
// neatly stored as static member variables.
//
// These are of two types:
// - the ones that can only be set directly in this file,
//   only for development
// - the ones that can be set in a configuration file,
//   some of which can be overridden on the command line.

struct Configuration {
    static const bool debug = true;
    static const int verbosity_level = 2;

    static bool fullscreen;
    static bool rift_input;
    static bool rift_output;
};

#endif

