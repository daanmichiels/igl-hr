#include <GL/glew.h>
#include "../strings.h"
#include "init.h"
#include "../configuration/configuration.h"
#include "../logmanager/logmanager.h"
#include <string>

// TODO: Is this worth a class?
//       No. Move it to main.
void Init::welcome_message() {
    LogManager::log_general(HR_WELCOME_MESSAGE, 1);
}


