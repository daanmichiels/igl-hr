
#include "riftmanager.h"
#include "../logmanager/logmanager.h"
#include "../configuration/configuration.h"
#include "OVR/OVR.h"

bool RiftManager::rift_active = false;
ovrHmd RiftManager::rift = NULL;

bool RiftManager::startup() {
	if(Configuration::rift_input == OnOffAuto::off) {
		rift_active = false;
	} else {
		ovrHmd rift = ovrHmd_Create(0);
		if(!rift) {
			if(Configuration::rift_input == OnOffAuto::on) {
				LogManager::log_warning("No Rift detected. Using debug.", 1);
				rift = ovrHmd_CreateDebug(ovrHmd_DK1);
				rift_active = true;
			} else {
				LogManager::log_info("No Rift detected.", 1);
				rift_active = false;
			}
		} else {
			LogManager::log_info("Rift detected.", 1);
			rift_active = true;
		}
	}

    LogManager::log_info("RiftManager started.", 2);
    return true;
}

void RiftManager::shutdown() {
    LogManager::log_info("RiftManager stopped.", 2);
}

