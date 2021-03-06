
#include "configuration.h"
#include <string.h>
#include <algorithm>

int Configuration::width = 1200;
int Configuration::height = 800;
bool Configuration::fullscreen = false;
OnOffAuto Configuration::rift_input = OnOffAuto::automatic;
OnOffAuto Configuration::rift_output = OnOffAuto::automatic;
std::vector<std::string> Configuration::filenames = std::vector<std::string>();

const double Configuration::mouse_speed = 0.002; //in rad/pix
const double Configuration::walking_speed = 1.8; //in m/s
const double Configuration::running_speed = 4.8; //in m/s
const double Configuration::ipd = 0.065; //interpupillary distance in meters
const double Configuration::eye_height = 1.60; //in m

/** \brief Sets up the configuration of the program given command line args.  TODO: Whatever is not supplied
 *  on the command line is read from a file. 
 *  \param argc and argv passed from main
 *  \return void
 */
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
