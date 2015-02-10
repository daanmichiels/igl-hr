
#include "riftmanager.h"
#include "../logmanager/logmanager.h"
#include "../configuration/configuration.h"
#include "../charactermanager/charactermanager.h"

bool RiftManager::rift_connected = false;
ovrHmd RiftManager::rift = NULL;

bool RiftManager::startup() {
	if(Configuration::rift_input == OnOffAuto::off) {
		rift_connected = false;
	} else {
		ovr_Initialize();
		rift = ovrHmd_Create(0);
		if(!rift) {
            rift_connected = false;
			if(Configuration::rift_input == OnOffAuto::on) {
				LogManager::log_warning("No Rift detected. Using debug.", 1);
				rift = ovrHmd_CreateDebug(ovrHmd_DK1);
			} else {
				LogManager::log_info("No Rift detected.", 1);
			}
		} else {
			LogManager::log_info("Rift detected.", 1);
			rift_connected = true;
		}
	}

	if(rift_connected) {
		ovrHmd_ConfigureTracking(rift, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection, 0);
		CharacterManager::set_hmd(&rift);
	}

    LogManager::log_info("RiftManager started.", 2);
    return true;
}

void RiftManager::shutdown() {
    LogManager::log_info("RiftManager stopped.", 2);
}

