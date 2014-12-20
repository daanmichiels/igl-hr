#include <GL/glew.h>
#include "../strings.h"
#include "init.h"
#include "../configuration/configuration.h"
#include "../logmanager/logmanager.h"
#include <string>

void Init::welcome_message() {
    LogManager::log_general(HR_WELCOME_MESSAGE, 1);
}


