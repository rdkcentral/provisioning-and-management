#ifndef _PAM_REGISTER_COMPONENT_H_
#define _PAM_REGISTER_COMPONENT_H_

#include <stdbool.h>
#include <rbus/rbus.h>
#include <pthread.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

/* Register PAM RBUS events */
void registerPamEvents(rbusHandle_t handle);

/* Check WiFi readiness */
void pam_checkAndPublishWifiReady(rbusHandle_t handle);

/* Check WAN readiness */
void pam_checkAndPublishWanReady(rbusHandle_t handle);

/* Start monitoring component dependencies */
void pam_startDependencyMonitoring(rbusHandle_t handle);

#endif /* _PAM_REGISTER_COMPONENT_H_ */