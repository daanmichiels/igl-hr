
#include "configuration.h"
#include <string>
#include <algorithm>

// Sets up the configuration of the program
// given command line arguments.
// Whatever is not supplied on the command line is read from a file.
// The parameters are the ones supplied to main.

int width = 800;
int height = 600;
bool Configuration::fullscreen = false;
OnOffAuto Configuration::rift_input = OnOffAuto::on;
OnOffAuto Configuration::rift_output = OnOffAuto::automatic;
std::vector<std::string> Configuration::filenames = std::vector<std::string>();

void Configuration::configure(int argc, const char* argv[]) {

    for(int i=0; i<argc; ++i) {
        if(strcmp(argv[i],"--fullscreen") == 0) {
            Configuration::fullscreen = true;
        } else if(strcmp(argv[i],"--rift-input") == 0) {
            Configuration::rift_input = OnOffAuto::on;
        } else if(strcmp(argv[i],"--no-rift-input") == 0) {
            Configuration::rift_input = OnOffAuto::off;
        } else if(strcmp(argv[i],"--rift-output") == 0) {
            Configuration::rift_output = OnOffAuto::on;
        } else if(strcmp(argv[i],"--no-rift-output") == 0) {
            Configuration::rift_output = OnOffAuto::off;
        } else if(strcmp(argv[i],"--rift") == 0) {
            Configuration::rift_input = OnOffAuto::on;
            Configuration::rift_output = OnOffAuto::on;
        } else if(strcmp(argv[i],"--no-rift") == 0) {
            Configuration::rift_input = OnOffAuto::off;
            Configuration::rift_output = OnOffAuto::off;
        } else {
            Configuration::filenames.push_back(std::string(argv[i]));
        }
    }

    return;
}
