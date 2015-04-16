
#include "riftmanager.h"
#include "../logmanager/logmanager.h"
#include "../configuration/configuration.h"
#include "OVR.h"
#include "../charactermanager/charactermanager.h"
#include "../rendermanager/rendermanager.h"

bool RiftManager::rift_connected = false;
ovrHmd RiftManager::hmd = NULL;

/** \brief Startup the rift manager. Logs whether or not rift is detected at level 1, then logs RiftManager Started at level 2
 * \param void
 * \return True if rift is detected, false if the rift is not detected
 */
bool RiftManager::startup() {
	if(Configuration::rift_input == OnOffAuto::off && !Configuration::stereo) {
		// we need an hmd is we're doing anything with the rift, or stereo rendering
		rift_connected = false;
	} else {
		ovr_Initialize();
		hmd = ovrHmd_Create(0);
		if(!hmd) {
      rift_connected = false;
			if(Configuration::rift_input == OnOffAuto::on || Configuration::rift_output == OnOffAuto::on || Configuration::stereo) {
				LogManager::log_warning("No Rift detected. Using debug.", 1);
				hmd = ovrHmd_CreateDebug(ovrHmd_DK1);
			} else {
				LogManager::log_info("No Rift detected.", 1);
			}
		} else {
			LogManager::log_info("Rift detected.", 1);
			rift_connected = true;
		}
	}

	if(rift_connected) {
		ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection, 0);
	}

	if (hmd != NULL) {
		CharacterManager::set_hmd(hmd);
		RenderManager::set_hmd(hmd);

		// save window size requested by the rift if doing rift output
		if (Configuration::rift_output != OnOffAuto::off) {
			Configuration::width = hmd->Resolution.w;
			Configuration::height = hmd->Resolution.h;
		}

	}

    LogManager::log_info("RiftManager started.", 2);
    return true;
}
/** \brief Shutdown the rift manager. Logs RiftManager stopped at level 2
 * \param  void
 * \return void
 */
void RiftManager::shutdown() {
    LogManager::log_info("RiftManager stopped.", 2);
}

