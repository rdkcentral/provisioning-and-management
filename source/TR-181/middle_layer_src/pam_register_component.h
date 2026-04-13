#ifndef _PAM_REGISTER_COMPONENT_H_
#define _PAM_REGISTER_COMPONENT_H_

#include <stdbool.h>
#include <rbus/rbus.h>



/**
 * @brief Check WiFi dependencies and publish wifi_ready_to_go
 *
 * This API checks the hardcoded WiFi dependencies:
 *  - com.cisco.spvtg.ccsp.psm
 *  - com.cisco.spvtg.ccsp.pam
 *
 * If all dependencies are up (discovered via RBUS),
 * a wifi_ready_to_go event is published using RBUS.
 *
 * @param handle  RBUS handle obtained by PAM
 */
void pam_checkAndPublishWifiReady(rbusHandle_t handle);

/**
 * @brief Check WAN Manager dependencies and publish wan_ready_to_go
 *
 * This API checks the hardcoded WAN dependencies:
 *  - com.cisco.spvtg.ccsp.psm
 *  - com.cisco.spvtg.ccsp.pam
 *  - com.cisco.spvtg.ccsp.ledmanager
 *
 * If all dependencies are up (discovered via RBUS),
 * a wan_ready_to_go event is published using RBUS.
 *
 * @param handle RBUS handle obtained by PAM
 */
void pam_checkAndPublishWanReady(rbusHandle_t handle);



#endif /* _PAM_REGISTER_COMPONENT_H_ */