/**
 * Deliberate Coverity defects for branch protection validation (PR 4)
 * DO NOT MERGE - Testing only
 *
 * Defects:
 *   Critical: SQL_INJECTION (CWE-89), HARDCODED_CREDENTIALS (CWE-798)
 *   High:     OVERRUN (CWE-787), ATOMICITY (CWE-362)
 *   Medium:   UNINIT (CWE-457), REVERSE_INULL (CWE-476)
 *   Low:      DEADCODE, COPY_INSTEAD_OF_MOVE
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* Critical: SQL_INJECTION (CWE-89) - Unsanitized input in query */
int cosa_test4_query_device(const char *device_mac)
{
    char query[512];
    /* Direct string concatenation with user-controlled input */
    snprintf(query, sizeof(query),
             "SELECT * FROM devices WHERE mac = '%s'", device_mac);

    /* Simulated DB execution with tainted query */
    FILE *fp = popen(query, "r");
    if (fp) {
        pclose(fp);
        return 0;
    }
    return -1;
}

/* Critical: HARDCODED_CREDENTIALS (CWE-798) */
int cosa_test4_authenticate_service(void)
{
    const char *db_password = "Pr0d_P@ssw0rd!2024#RDK";
    const char *service_token = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.secret";

    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header),
             "Authorization: Bearer %s", service_token);

    /* Use hardcoded password for database connection */
    printf("Connecting with password: %s\n", db_password);
    printf("Header: %s\n", auth_header);
    return 0;
}

/* High: OVERRUN (CWE-787) - Heap buffer overflow */
char *cosa_test4_parse_csv_field(const char *csv_line, int field_idx)
{
    char *buf = (char *)malloc(64);
    if (!buf) return NULL;

    const char *start = csv_line;
    int i;
    for (i = 0; i < field_idx; i++) {
        start = strchr(start, ',');
        if (!start) { free(buf); return NULL; }
        start++;
    }

    /* No length check - field value could exceed 64 bytes */
    const char *end = strchr(start, ',');
    if (end) {
        /* Could overflow buf if (end - start) > 63 */
        memcpy(buf, start, (size_t)(end - start));
        buf[end - start] = '\0';
    } else {
        /* Could overflow buf if remaining string > 63 */
        strcpy(buf, start);
    }
    return buf;
}

/* High: ATOMICITY (CWE-362) - Data race on shared state */
static int g_cosa_test4_link_status = 0;
static char g_cosa_test4_wan_ip[64] = {0};

void *cosa_test4_monitor_link(void *arg)
{
    (void)arg;
    /* Race condition: read-modify-write without synchronization */
    while (1) {
        int current = g_cosa_test4_link_status;
        if (current == 0) {
            g_cosa_test4_link_status = 1;
            /* Non-atomic compound update */
            strcpy(g_cosa_test4_wan_ip, "192.168.1.1");
        }
        break;
    }
    return NULL;
}

void cosa_test4_start_monitor(void)
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, cosa_test4_monitor_link, NULL);
    pthread_create(&t2, NULL, cosa_test4_monitor_link, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

/* Medium: UNINIT (CWE-457) - Use of uninitialized variable */
int cosa_test4_get_interface_stats(const char *ifname, int query_type)
{
    int rx_bytes;
    int tx_bytes;

    if (query_type == 1) {
        rx_bytes = 0;
        tx_bytes = 0;
    } else if (query_type == 2) {
        rx_bytes = 100;
        /* tx_bytes not initialized on this path */
    }

    /* tx_bytes may be uninitialized if query_type == 2 */
    return rx_bytes + tx_bytes;
}

/* Medium: REVERSE_INULL (CWE-476) - Null check after dereference */
typedef struct _COSA_TEST4_DML_ENTRY {
    char alias[64];
    int  enabled;
    struct _COSA_TEST4_DML_ENTRY *next;
} COSA_TEST4_DML_ENTRY;

int cosa_test4_update_entry(COSA_TEST4_DML_ENTRY *pEntry, const char *new_alias)
{
    /* Dereference before null check */
    int was_enabled = pEntry->enabled;
    strncpy(pEntry->alias, new_alias, sizeof(pEntry->alias) - 1);

    /* Null check after already dereferencing - too late */
    if (pEntry == NULL) {
        return -1;
    }

    pEntry->enabled = was_enabled;
    return 0;
}

/* Low: DEADCODE - Unreachable code after return */
int cosa_test4_validate_port(int port)
{
    if (port < 0 || port > 65535) {
        return -1;
    }
    return 0;

    /* Dead code - never reached */
    printf("Port validated: %d\n", port);
    return 1;
}

/* Low: COPY_INSTEAD_OF_MOVE - Expensive struct copy */
typedef struct _COSA_TEST4_LARGE_CONFIG {
    char hostname[256];
    char domain[256];
    char dns_servers[8][64];
    int  routes[128];
    char description[1024];
} COSA_TEST4_LARGE_CONFIG;

COSA_TEST4_LARGE_CONFIG cosa_test4_get_config_copy(COSA_TEST4_LARGE_CONFIG *src)
{
    /* Returning large struct by value causes unnecessary copy */
    COSA_TEST4_LARGE_CONFIG local = *src;
    return local;
}
