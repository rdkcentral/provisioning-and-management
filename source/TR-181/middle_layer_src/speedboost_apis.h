#ifndef _COSA_SPEEDBOOST_DML_H
#define _COSA_SPEEDBOOST_DML_H


#include <pthread.h>
#include <stdbool.h>

#define WAIT_TIME_TO_TRIGGER_RA_RESTART 15

#define MAC_ADDR_SIZE               18
#define MAX_MAC_ADDR_COUNT          10

typedef struct _ClientInfoOfSpeedBoost_
{
    int iNumOfConfigOfClients;
    int iNumOfActiveClients;
    char cCurrentDeviceList [MAX_MAC_ADDR_COUNT][MAC_ADDR_SIZE];
}ClientInfoOfSpeedBoost;

/**
 * @brief Triggers a restart of the Router Advertisement (RA) service.
 *
 * This function initiates a restart of the RA service to apply SpeedBoost configuration changes.
 *
 * @return void
 */
void trigger_ra_service_restart();
/**
 * @brief Monitor thread function for delayed RA service restart.
 *
 * This function runs in a separate thread to monitor and trigger RA service restart
 * after a delay period for SpeedBoost configuration.
 *
 * @param[in] arg Thread argument.
 *
 * @return NULL (thread return value).
 */
void* monitor_thread_for_ra_restart(void* arg);

typedef struct SpeedBoost
{
    bool pvd_enabled;
    bool pvd_h_flag ;
    int pvd_delay;
    int pvd_seqNum;
    char pvd_fqdn[256];
    ClientInfoOfSpeedBoost sClientInfoSpeedBoost;
}SpeedBoost;

typedef enum {
    CURRENT_SPEEDBOOST_VALUE = 0,
    CURRENT_NORMAL_SPEEDBOOST_VALUE
} SpeedBoostType;

/**
 * @brief Gets the number of configured SpeedBoost clients.
 *
 * This function returns the count of clients configured for SpeedBoost service.
 *
 * @return Number of configured SpeedBoost clients.
 */
int getNumOfConfiguredClients(void);

/**
 * @brief Gets the number of active SpeedBoost clients.
 *
 * This function returns the count of currently active clients using SpeedBoost service.
 *
 * @return Number of active SpeedBoost clients.
 */
int getNumOfActiveClients(void);

/**
 * @brief Retrieves the list of active SpeedBoost devices.
 *
 * This function populates a buffer with the MAC addresses of devices currently using SpeedBoost.
 *
 * @param[out] pActiveDevicesList Buffer to store comma-separated list of active device MAC addresses.
 * @param[in] iLen Size of the pActiveDevicesList buffer in bytes.
 *
 * @return list active SpeedBoost devices.
 * @retval true if list was retrieved successfully.
 * @retval false otherwise.
 */
bool getActiveDevicesListOfSpeedBoost(char * pActiveDevicesList, int iLen);


/**
 * @brief Initializes the SpeedBoost structure with default values.
 *
 * This function sets up the SpeedBoost configuration structure with initial/default values.
 *
 * @return void
 */
void initializeSpeedBoostStructVal();

/**
 * @brief Processes and validates a SpeedBoost configuration value.
 *
 * This function parses and validates SpeedBoost configuration.
 *
 * @param[in] str New SpeedBoost configuration value string to process.
 * @param[in] prev_val Previous SpeedBoost configuration value for comparison.
 * @param[out] target_value Buffer to store the processed value.
 * @param[in] target_size Size of the target_value buffer in bytes.
 * @param[in] boostType Type of SpeedBoost value.
 *
 * @return The status of the operation.
 * @return 0 on success.
 * @retval non-zero error code on failure.
 */
int processSpeedBoostValue(const char* str, const char* prev_val, char* target_value, size_t target_size, SpeedBoostType boostType);
#endif
