
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
Configuration::samples = 4; //multisample default here
//NOTE: The logic down below sets this default to 4 if an invalid value is passed through the command line. If this default value is changed, then so should the value down below.

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
		} else if(strstr(argv[i],"--multisample")) {
			/*
			if(!RenderManager::rift_render) {
				char[] keys = "0123456789";
				int n = strcspn(argv[i], keys);
				char[] nums = "";
				for(int i=0; i<strlen(argv[i])-n; i++)
					nums[i]=argv[i][n+i];
				int samples = std::atoi(nums);
				RenderManager::samples = samples;
			}
			*/
			if(i+1>=argc)
				LogManager::log_warning("No multisample value given. Using default (" + Configuration::samples + ")", 0);
			else{
				Configuration::samples = std::atoi(argv[i+1]);
				if(!Configuration::samples){
					Configuration::samples = 4; //multisample default here
					LogManager::log_warning("No multisample value given. Using default (" + Configuration::samples + ")", 0);
				}
				++i;
			}
        } else {
            Configuration::filenames.push_back(std::string(argv[i]));
        }
    }

    return;
}
