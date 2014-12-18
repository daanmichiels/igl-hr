#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <vector>

// Contains the configuration of the program
// neatly stored as static member variables.
//
// These are of two types:
// - the ones that can only be set directly in this file,
//   only for development
// - the ones that can be set in a configuration file,
//   some of which can be overridden on the command line.

enum class OnOffAuto { on, off, automatic };

class Configuration {
public:
    static const bool debug = true;
    static const int verbosity = 2;

    static int width;
    static int height;
    static bool fullscreen;
    static OnOffAuto rift_input;
    static OnOffAuto rift_output;
    static std::vector<std::string> filenames;

    static void configure(int argc, const char* argv[]);
};

#endif

