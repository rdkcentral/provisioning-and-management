#ifndef _PAM_REGISTER_COMPONENT_H_
#define _PAM_REGISTER_COMPONENT_H_

#include <stdbool.h>
#include <rbus/rbus.h>

/**
 * @brief Register PAM RBUS events
 *
 * Registers:
 *  - wifi_ready_to_go
 *  - wan_ready_to_go
 *
 * Must be called after rbus_open() in ssp_main.
 *
 * @param handle RBUS handle
 */
void registerPamEvents(rbusHandle_t handle);

/**
 * @brief Check WiFi dependencies and publish wifi_ready_to_go
 */
void pam_checkAndPublishWifiReady(rbusHandle_t handle);

/**
 * @brief Check WAN dependencies and publish wan_ready_to_go
 */
void pam_checkAndPublishWanReady(rbusHandle_t handle);

#endif /* _PAM_REGISTER_COMPONENT_H_ */