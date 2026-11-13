/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
#include <syscfg/syscfg.h>
#include "cosa_webconfig_api.h"
#include "webconfig_framework.h"
#include <trower-base64/base64.h>
#include "safec_lib_common.h"
#include <pthread.h>

t_cache pf_cache[PORTMAP_CACHE_SIZE];
t_cache pf_cache_bkup[PORTMAP_CACHE_SIZE];

t_cache dmz_cache[DMZ_CACHE_SIZE];
t_cache dmz_cache_bkup[DMZ_CACHE_SIZE];

static int pf_cache_size = 0;
static int pf_cache_size_bkup = 0;

static int gpfEnable = 0;
static t_one_to_one_nat_cache one_to_one_nat_cache;
static t_one_to_one_nat_cache one_to_one_nat_cache_bkup;
static pthread_mutex_t one_to_one_nat_cache_lock = PTHREAD_MUTEX_INITIALIZER;
static int restore_one_to_one_nat_to_DB(const t_one_to_one_nat_cache *nat);

int  get_base64_decodedbuffer(char *pString, char **buffer, int *size)
{
    struct timespec start,end,*startPtr,*endPtr;
    int decodeMsgSize = 0;
    char *decodeMsg = NULL;
    if (buffer == NULL || size == NULL || pString == NULL)
        return -1;

    startPtr = &start;
    endPtr = &end;


    getCurrentTime(startPtr);
    decodeMsgSize = b64_get_decoded_buffer_size(strlen(pString));

    decodeMsg = (char *) malloc(sizeof(char) * decodeMsgSize);

    if (!decodeMsg)
        return -1;

    *size = b64_decode( (const uint8_t*)pString, strlen(pString), (uint8_t *)decodeMsg );
    CcspTraceWarning(("base64 decoded data contains %d bytes\n",*size));

    getCurrentTime(endPtr);
    CcspTraceWarning(("Base64 decode Elapsed time : %ld ms\n", timeValDiff(startPtr, endPtr)));

    *buffer = decodeMsg;
    return 0;
}

msgpack_unpack_return get_msgpack_unpack_status(char *decodedbuf, int size)
{

    msgpack_zone mempool;
    msgpack_object deserialized;
    msgpack_unpack_return unpack_ret;

    if (decodedbuf == NULL || !size)
        return MSGPACK_UNPACK_NOMEM_ERROR;

    msgpack_zone_init(&mempool, 2048);
    unpack_ret = msgpack_unpack(decodedbuf, size, NULL, &mempool, &deserialized);

    switch(unpack_ret)
    {
        case MSGPACK_UNPACK_SUCCESS:
            CcspTraceWarning(("MSGPACK_UNPACK_SUCCESS :%d\n",unpack_ret));
            break;
        case MSGPACK_UNPACK_EXTRA_BYTES:
            CcspTraceWarning(("MSGPACK_UNPACK_EXTRA_BYTES :%d\n",unpack_ret));
            break;
        case MSGPACK_UNPACK_CONTINUE:
            CcspTraceWarning(("MSGPACK_UNPACK_CONTINUE :%d\n",unpack_ret));
            break;
        case MSGPACK_UNPACK_PARSE_ERROR:
            CcspTraceWarning(("MSGPACK_UNPACK_PARSE_ERROR :%d\n",unpack_ret));
            break;
        case MSGPACK_UNPACK_NOMEM_ERROR:
            CcspTraceWarning(("MSGPACK_UNPACK_NOMEM_ERROR :%d\n",unpack_ret));
            break;
        default:
            CcspTraceWarning(("Message Pack decode failed with error: %d\n", unpack_ret));
    }

    msgpack_zone_destroy(&mempool);
    //End of msgpack decoding

    return unpack_ret;
}
int CheckIfIpIsValid( char *ipAddress )
{

    CcspTraceInfo(("%s:IpAddressReceivedIs:%s\n",__FUNCTION__,ipAddress));

    struct sockaddr_in sa;
    struct sockaddr_in6 sa6;

    if ( (inet_pton(AF_INET, ipAddress, &(sa.sin_addr)) == 1 ) || (inet_pton(AF_INET6, ipAddress, &(sa6.sin6_addr)) == 1 )) 
    {
        return VALID_IP;
    }

    return INVALID_IP;
}

int CheckIfPortsAreValid( char *port, char *port_end_range )
{

    CcspTraceInfo(("%s:ExternalPortEndRangeReceivedIs:%s\n",__FUNCTION__,port));
    CcspTraceInfo(("%s:ExternalPortEndRangeReceivedIs:%s\n",__FUNCTION__,port_end_range));


    int iPort = atoi(port);

    int iPort_end_range = atoi(port_end_range);

    while (*port) 
    {
        if (isdigit(*port++) == 0) 
        {
            return INVALID_PORT ;
        }
    }

    while (*port_end_range) 
    {
        if (isdigit(*port_end_range++) == 0) 
        {
            return INVALID_PORT ;
        }
    }

    if ( iPort <= 0 || iPort > 65535 || iPort_end_range <=0 || iPort_end_range > 65535  || iPort > iPort_end_range)
    {
        return INVALID_PORT ;
    }


    return 0;
}

/* API to get the subdoc version */
uint32_t getBlobVersion (char *subdoc)
{
    char buf[64 + 8];
    char subdoc_ver[12];

    snprintf(buf, sizeof(buf), "%s_version", subdoc);

    if (syscfg_get(NULL, buf, subdoc_ver, sizeof(subdoc_ver)) == 0)
    {
        return (uint32_t) atoi(subdoc_ver);
    }

    return 0;
}

/* API to update the subdoc version */
int setBlobVersion (char *subdoc, uint32_t version)
{
    char buf[256];
    char subdoc_ver[12];

    snprintf(subdoc_ver, sizeof(subdoc_ver), "%u", version);

    if (strcmp(subdoc, "hotspot") == 0)
    {
		if (version == 0) {
			snprintf(buf, sizeof(buf), "rm %s", HOTSPOT_BLOB_FILE);
			CcspTraceInfo(("%s : cmd to remove filename is %s\n", __FUNCTION__, buf));
		}
		else {
			snprintf(buf, sizeof(buf), "mv /tmp/.%s%s %s", subdoc, subdoc_ver, HOTSPOT_BLOB_FILE);
			CcspTraceInfo(("%s : cmd to move filename is %s\n", __FUNCTION__, buf));
		}
        system(buf);
    }

    snprintf(buf, sizeof(buf), "%s_version", subdoc);

    if (syscfg_set_commit(NULL, buf, subdoc_ver) != 0)
    {
        CcspTraceError(("syscfg_set failed\n"));
        return -1;
    }

    return 0;
}

/* API to register all the supported subdocs , versionGet and versionSet are callback functions to get and set the subdoc versions in db */

void webConfigFrameworkInit()
{
#if defined (FEATURE_RDKB_DHCP_MANAGER)
	#if defined(SPEED_BOOST_SUPPORTED)
        char *sub_docs[]= {"portforwarding","wan","macbinding","lan","hotspot","connectedbuilding","xmspeedboost","webui",(char *) 0 };
	#else
        char *sub_docs[]= {"portforwarding","wan","macbinding","lan","hotspot","connectedbuilding","webui",(char *) 0 };
	#endif
#else
	#if defined(SPEED_BOOST_SUPPORTED)
		char *sub_docs[]= {"portforwarding","wan","macbinding","lan","hotspot","connectedbuilding","xmspeedboost","webui",(char *) 0 };
	#else
		char *sub_docs[]= {"portforwarding","wan","macbinding","lan","hotspot","connectedbuilding","webui",(char *) 0 };
	#endif
#endif
	int SUBDOC_COUNT = sizeof(sub_docs)/sizeof(sub_docs[0]) -1;

    	blobRegInfo *blobData;

    	blobData = (blobRegInfo*) malloc(SUBDOC_COUNT * sizeof(blobRegInfo));

    	int i;
    	memset(blobData, 0, SUBDOC_COUNT * sizeof(blobRegInfo));

    	blobRegInfo *blobDataPointer = blobData;


    	for (i=0 ; i < SUBDOC_COUNT ; i++ )
    	{
        	strncpy( blobDataPointer->subdoc_name, sub_docs[i], sizeof(blobDataPointer->subdoc_name)-1);

        	blobDataPointer++;
    	}

   	    blobDataPointer = blobData ;

    	getVersion versionGet = getBlobVersion;

    	setVersion versionSet = setBlobVersion;

    	register_sub_docs(blobData,SUBDOC_COUNT,versionGet,versionSet);
	initMultiCompMaster(); 
#ifndef NO_WIFI_FEATURE_SUPPORT
        //Multi Comp
        wbInitializeHotspot();
#endif
}

/* API to clear the buffer */
void clear_pf_cache(t_cache *tmp_pf_cache)
{
    memset (tmp_pf_cache, 0, sizeof(t_cache) * PORTMAP_CACHE_SIZE);
}

/* API to print cache */
void print_pf_cache(t_cache *tmp_pf_cache)
{
	int i = 0;
    	for(i = 0;i <pf_cache_size ; i++)
    	{
        	printf("pf_cache[%d].cmd - %s\n",i,tmp_pf_cache[i].cmd);
        	printf("pf_cache[%d].val - %s\n",i,tmp_pf_cache[i].val);
    	}	

}

/* API to unset the entries from DB */
int clear_pf_cache_DB(t_cache *tmp_pf_cache)
{
	int i = 0;
    	for(i = 0; i < pf_cache_size; i++)
    	{

        	if (syscfg_unset(NULL, tmp_pf_cache[i].cmd) != 0)
        	{
               		CcspTraceError(("syscfg_set failed for %s %s\n",tmp_pf_cache[i].cmd,tmp_pf_cache[i].val));
                    return SYSCFG_FAILURE;

               		//break;
        	}
        	memset(tmp_pf_cache[i].cmd,0,BLOCK_SIZE);
        	memset(tmp_pf_cache[i].val,0,VAL_BLOCK_SIZE);
    	}

        if (syscfg_commit() != 0)
        {
                CcspTraceError(("syscfg_commit failed apply_cache_ToDB\n"));
                return SYSCFG_FAILURE;
        }

    	system("sysevent set firewall-restart");

    return 0;

}

/* API to back up the cache */
void backup_pf_cache(t_cache *tmp_pf_cache,t_cache *tmp_pf_cache_bkup,int cache_size)
{
    UNREFERENCED_PARAMETER(cache_size);
	int i = 0;
    errno_t rc = -1;
    	for(i = 0; i < pf_cache_size; i++)
    	{

			memset(tmp_pf_cache_bkup[i].cmd,0,BLOCK_SIZE);
       		memset(tmp_pf_cache_bkup[i].val,0,VAL_BLOCK_SIZE);

			rc = strcpy_s(tmp_pf_cache_bkup[i].cmd, BLOCK_SIZE,tmp_pf_cache[i].cmd);
			if(rc != EOK)
			{
				ERR_CHK(rc);
			}

			rc = strcpy_s(tmp_pf_cache_bkup[i].val, VAL_BLOCK_SIZE,tmp_pf_cache[i].val);
			if(rc != EOK)
			{
				ERR_CHK(rc);
			}
   	 } 
}

/* API to apply port forwarding requests to DB */
int apply_pf_cache_ToDB(t_cache *tmp_pf_cache, int cache_size,int pmapEnable)
{
	    int i = 0;
        if ( gpfEnable !=  pmapEnable )
        {
                if (syscfg_set("CosaNAT", "port_forward_enabled", (pmapEnable == 1) ? "1" : "0" ) != 0)
                {
                    CcspTraceError(("syscfg_set failed to set %s::%s parameter\n", "CosaNAT", "port_forward_enabled"));
                    return SYSCFG_FAILURE;
                }    
        }

    	for(i = 0;i <cache_size ; i++)
    	{
            if (tmp_pf_cache[i].cmd[0] == '\0')
            {
                continue;
            }
        	//printf("pf_cache[%d].cmd - %s\n",i,tmp_pf_cache[i].cmd);
        	//printf("pf_cache[%d].val - %s\n",i,tmp_pf_cache[i].val);

        	if (syscfg_set(NULL, tmp_pf_cache[i].cmd, tmp_pf_cache[i].val) != 0)
        	{
               		CcspTraceError(("syscfg_set failed for %s %s\n",tmp_pf_cache[i].cmd,tmp_pf_cache[i].val));
               		return SYSCFG_FAILURE;
              		// break;
        	}
    	}
        if (restore_one_to_one_nat_to_DB(&one_to_one_nat_cache_bkup) != 0)
            return SYSCFG_FAILURE;
    	if (syscfg_commit() != 0)
    	{
        	CcspTraceError(("syscfg_commit failed apply_cache_ToDB\n"));
       		return SYSCFG_FAILURE;

    	}

        #ifdef WEBCFG_TEST_SIM
            int fd = -1;
            // to simulate NACK
            fd = access(NACK_SIMULATE_FILE, F_OK); 
            if(fd == 0)
            { 
                    unlink(NACK_SIMULATE_FILE);
                    return SYSCFG_FAILURE;

            }
            fd = -1;

            // to simulate timeout and maxtimeout cases
            FILE *fp;
            int interval = 15 ;
            fp = fopen (TIMEOUT_SIMULATE_FILE, "r");
            if( fp )
            {
                fscanf(fp, "%d", &interval);
                if ( interval <= 0 )
                    interval = 15;

                sleep(interval);
                fclose(fp);
                fp=NULL; 
                unlink(TIMEOUT_SIMULATE_FILE);

            }
        #endif 

    	system("sysevent set firewall-restart");

    return 0;
}

#if 0
void fill_portmap_entry(int count)
{
    int i = 0;
    rpm = (portmappingdoc_t *)malloc(sizeof(portmappingdoc_t));
    rpm->entries = (portdoc_t *)malloc(sizeof(portdoc_t)*count);
    rpm->entries_count = count;
    for(i = 0; i < count; i++ )
    {
        rpm->entries[i].internal_client = malloc(sizeof(char)*16);
        memset(rpm->entries[i].internal_client,0,16);
        strcpy(rpm->entries[i].internal_client,"192.168.0.21");

        rpm->entries[i].enable = malloc(sizeof(char)*16);
        memset(rpm->entries[i].enable,0,16);
        strcpy(rpm->entries[i].enable,"1");

        rpm->entries[i].external_port_end_range = malloc(sizeof(char)*16);
        memset(rpm->entries[i].external_port_end_range,0,16);
        if(i%2 == 1)
        strcpy(rpm->entries[i].external_port_end_range,"5670");
        else
            strcpy(rpm->entries[i].external_port_end_range,"5666");


        rpm->entries[i].protocol = malloc(sizeof(char)*16);
        memset(rpm->entries[i].protocol,0,16);
        strcpy(rpm->entries[i].protocol,"both");

        rpm->entries[i].description = malloc(sizeof(char)*16);
        memset(rpm->entries[i].description,0,16);
        strcpy(rpm->entries[i].description,"test");

        rpm->entries[i].external_port = malloc(sizeof(char)*16);
        memset(rpm->entries[i].external_port,0,16);
        strcpy(rpm->entries[i].external_port,"5666");
    }
}

void print_portmap_entry()
{
	int i = 0;
    	printf("entries %d\n",rpm->entries_count);

    	for(i = 0; i < rpm->entries_count; i++ )
    	{
        	printf("rpm->entries[%d].internal_client= %s\n",i,rpm->entries[i].internal_client);
        	printf("rpm->entries[%d].enable= %s\n",i,rpm->entries[i].enable);
        	printf("rpm->entries[%d].external_port_end_range= %s\n",i,rpm->entries[i].external_port_end_range);
        	printf("rpm->entries[%d].protocol= %s\n",i,rpm->entries[i].protocol);
        	printf("rpm->entries[%d].description= %s\n",i,rpm->entries[i].description);
        	printf("rpm->entries[%d].external_port= %s\n",i,rpm->entries[i].external_port);

    	}
}

#endif

static int read_one_to_one_nat_cache(t_one_to_one_nat_cache *nat_cache)
{
    static const char *suffixes[] = {
        ALIAS_POS_ENABLE, ALIAS_POS_NAME, ALIAS_POS_PROTO, ALIAS_POS_IP,
        ALIAS_POS_INT_PORT, ALIAS_POS_IPV6, ALIAS_POS_PREV_STATE,
        ALIAS_POS_EXT_PORT_RANGE, ALIAS_PFR_PUBLIC_IP, ALIAS_PFR_INT_RANGE
    };
    char key[BLOCK_SIZE], alias[VAL_BLOCK_SIZE], value[VAL_BLOCK_SIZE], *end;
    int index, field, len;
    long count;
    struct in_addr public_ip;

    memset(nat_cache, 0, sizeof(*nat_cache));
    if (!(syscfg_get(NULL, "PortRangeForwardCount", value, sizeof(value)) == 0 && value[0] != '\0'))
    {
        CcspTraceInfo(("%s: PortRangeForwardCount is unavailable or empty\n", __FUNCTION__));
        return 0;
    }
    count = strtol(value, &end, 10);
    if (end == value || *end != '\0' || count < 0 || count > PORTMAP_CACHE_SIZE)
    {
        CcspTraceInfo(("%s: invalid PortRangeForwardCount value '%s'\n", __FUNCTION__, value));
        return SYSCFG_FAILURE;
    }
    for (index = 1; index <= count; index++)
    {
        snprintf(key, sizeof(key), ALIAS_PFR "%d", index);
        if (syscfg_get(NULL, key, alias, sizeof(alias)) != 0 || alias[0] == '\0')
        {
            CcspTraceInfo(("%s: failed to read %s or alias is empty\n", __FUNCTION__, key));
            return SYSCFG_FAILURE;
        }
        len = snprintf(key, sizeof(key), "%s" ALIAS_PFR_PUBLIC_IP, alias);
        if (len < 0 || len >= (int)sizeof(key))
        {
            CcspTraceInfo(("%s: failed to format public IP key within buffer for alias %s\n", __FUNCTION__, alias));
            return SYSCFG_FAILURE;
        }
        if (syscfg_get(NULL, key, value, sizeof(value)) != 0 ||
            inet_pton(AF_INET, value, &public_ip) != 1 ||
            public_ip.s_addr == INADDR_ANY || public_ip.s_addr == INADDR_NONE)
            continue;

        if (nat_cache->cache_size >= PORTMAP_CACHE_SIZE)
        {
            CcspTraceInfo(("%s: NAT cache is full while caching alias %s\n", __FUNCTION__, alias));
            return SYSCFG_FAILURE;
        }
        snprintf(nat_cache->entries[nat_cache->cache_size].cmd, BLOCK_SIZE,
                 ALIAS_PFR "%d", index);
        snprintf(nat_cache->entries[nat_cache->cache_size++].val, VAL_BLOCK_SIZE, "%s", alias);

        for (field = 0; field < (int)(sizeof(suffixes) / sizeof(suffixes[0])); field++)
        {
            len = snprintf(key, sizeof(key), "%s%s", alias, suffixes[field]);
            if (len < 0 || len >= (int)sizeof(key))
            {
                CcspTraceInfo(("%s: failed to format key within buffer for alias %s, suffix %s\n",
                               __FUNCTION__, alias, suffixes[field]));
                return SYSCFG_FAILURE;
            }
            if (syscfg_get(NULL, key, value, sizeof(value)) != 0)
                continue;
            if (nat_cache->cache_size >= PORTMAP_CACHE_SIZE)
            {
                CcspTraceInfo(("%s: NAT cache is full while caching key %s\n", __FUNCTION__, key));
                return SYSCFG_FAILURE;
            }
            snprintf(nat_cache->entries[nat_cache->cache_size].cmd, BLOCK_SIZE, "%s", key);
            snprintf(nat_cache->entries[nat_cache->cache_size++].val, VAL_BLOCK_SIZE, "%s", value);
        }
        nat_cache->mapping_count++;
    }
    return 0;
}

int update_one_to_one_nat_cache(void)
{
    t_one_to_one_nat_cache *nat = calloc(1, sizeof(*nat));
    int ret = SYSCFG_FAILURE;

    if (nat != NULL)
        ret = read_one_to_one_nat_cache(nat);
    if (ret == 0)
    {
        pthread_mutex_lock(&one_to_one_nat_cache_lock);
        one_to_one_nat_cache = *nat;
        pthread_mutex_unlock(&one_to_one_nat_cache_lock);
    }
    else
    {
        CcspTraceError(("%s: failed to refresh 1:1 NAT cache from syscfg\n", __FUNCTION__));
    }
    free(nat);
    return ret;
}

static int nat_rule_id(const char *alias, unsigned long *id)
{
    char *end;
    if (strncmp(alias, ALIAS_PRE_PFR, strlen(ALIAS_PRE_PFR)) != 0)
        return SYSCFG_FAILURE;
    *id = strtoul(alias + strlen(ALIAS_PRE_PFR), &end, 10);
    if (end == alias + strlen(ALIAS_PRE_PFR) || *end != '\0' ||
        *id == 0 || *id > PORTMAP_CACHE_SIZE)
        return SYSCFG_FAILURE;
    return 0;
}

/* Merge saved NAT references with the table written by apply or rollback. */
static int restore_one_to_one_nat_to_DB(const t_one_to_one_nat_cache *nat)
{
    unsigned char present[PORTMAP_CACHE_SIZE + 1] = {0};
    unsigned long id;
    char key[BLOCK_SIZE], value[VAL_BLOCK_SIZE], *end;
    int total = 0, old_count = 0, i;
    long count;
    int ret = SYSCFG_FAILURE;

    if (nat->mapping_count == 0)
        return 0;
    if (syscfg_get(NULL, "PortRangeForwardCount", value, sizeof(value)) == 0)
    {
        count = strtol(value, &end, 10);
        if (end == value || *end != '\0' || count < 0 || count > PORTMAP_CACHE_SIZE)
            goto exit;
        old_count = (int)count;
    }
    for (i = 1; i <= old_count; i++)
    {
        snprintf(key, sizeof(key), ALIAS_PFR "%d", i);
        if (syscfg_get(NULL, key, value, sizeof(value)) != 0 ||
            nat_rule_id(value, &id) != 0)
            goto exit;
        present[id] = 1;
    }
    for (i = 0; i < nat->cache_size; i++)
    {
        if (strncmp(nat->entries[i].cmd, ALIAS_PFR, strlen(ALIAS_PFR)) == 0)
        {
            if (nat_rule_id(nat->entries[i].val, &id) != 0)
                goto exit;
            present[id] = 1;
            continue;
        }
        if (syscfg_set(NULL, nat->entries[i].cmd, nat->entries[i].val) != 0)
            goto exit;
    }
    /* Ascending IDs also preserve Utopia's ordering for subsequent additions. */
    for (id = 1; id <= PORTMAP_CACHE_SIZE; id++)
    {
        if (!present[id])
            continue;
        snprintf(key, sizeof(key), ALIAS_PFR "%d", ++total);
        snprintf(value, sizeof(value), ALIAS_PRE_PFR "%lu", id);
        if (syscfg_set(NULL, key, value) != 0)
            goto exit;
    }
    for (i = total + 1; i <= old_count; i++)
    {
        snprintf(key, sizeof(key), ALIAS_PFR "%d", i);
        if (syscfg_unset(NULL, key) != 0)
            goto exit;
    }
    snprintf(value, sizeof(value), "%d", total);
    if (syscfg_set(NULL, "PortRangeForwardCount", value) != 0)
        goto exit;
    ret = 0;
exit:
    if (ret != 0)
        CcspTraceError(("%s: failed to restore saved 1:1 NAT to syscfg\n", __FUNCTION__));
    return ret;
}

/* Read blob entries into a cache */
int set_portmap_conf(portmappingdoc_t *rpm)
{
        unsigned long i = 0; 
        int count = 0; int j = 0;
    	int spf_count = 0;
    	int pfr_count = 0;
    	char alias_pre[8];
        errno_t  rc   = -1;
        printf("Port map entries %zu\n",rpm->entries_count);
        unsigned long first_id = 1, saved_id;
        int k;
        char longest_key[BLOCK_SIZE];
        if (rpm->entries_count > (PORTMAP_CACHE_SIZE - 2) / 11)
        {
            CcspTraceError(("%s: too many forwarding entries\n", __FUNCTION__));
            return SYSCFG_FAILURE;
        }
        for (k = 0; k < one_to_one_nat_cache_bkup.cache_size; k++)
        {
            if (strncmp(one_to_one_nat_cache_bkup.entries[k].cmd, ALIAS_PFR, strlen(ALIAS_PFR)) != 0)
                continue;
            if (nat_rule_id(one_to_one_nat_cache_bkup.entries[k].val, &saved_id) != 0)
            {
                CcspTraceError(("%s: invalid saved NAT ID\n", __FUNCTION__));
                return SYSCFG_FAILURE;
            }
            if (saved_id >= first_id)
                first_id = saved_id + 1;
        }
        int length = snprintf(longest_key, sizeof(longest_key),
                              ALIAS_PRE_PFR "%lu" ALIAS_POS_PREV_STATE,
                              first_id + (unsigned long)rpm->entries_count - 1);
        if (length < 0 || length >= BLOCK_SIZE)
        {
            CcspTraceError(("%s: forwarding key exceeds cache key size\n", __FUNCTION__));
            return SYSCFG_FAILURE;
        }
    	//printf("SinglePortForwardCount = %d\n", rpm->entries_count);
    	//count = rpm->entries_count + 1;
        for(i = first_id,j =0; j < (int)rpm->entries_count; i++, j++ )
    	{
        	memset(alias_pre,0,8);

            if ( INVALID_IP == CheckIfIpIsValid(rpm->entries[j].internal_client) )
            {
                return INVALID_IP;
            }


            if ( (rpm->entries[j].external_port[0] == '\0' ) || ( rpm->entries[j].external_port_end_range[0] == '\0' ) )
            {
                CcspTraceError(("port value is null, validation failed\n"));
                return INVALID_PORT ;

            }

            else if ( INVALID_PORT == CheckIfPortsAreValid(rpm->entries[j].external_port, rpm->entries[j].external_port_end_range ) )
            {
                return INVALID_PORT ;
            }

            if( IsPortOverlapWithManagementAccess(atoi(rpm->entries[j].external_port), atoi(rpm->entries[j].external_port_end_range)))
            {
                CcspTraceError(("%s: Port Range %d - %d is overlapping with Management Access port \n",__FUNCTION__,atoi(rpm->entries[j].external_port), atoi(rpm->entries[j].external_port_end_range)));
                return INVALID_PORT ;
            }

#if defined (SPEED_BOOST_SUPPORTED)
            if( IsPortOverlapWithSpeedboostPortRange(atoi(rpm->entries[j].external_port), atoi(rpm->entries[j].external_port_end_range) , 0, 0))
            {
                CcspTraceError(("%s:Ext Port Range %d - %d is overlapping with speedboost port range\n",__FUNCTION__,atoi(rpm->entries[j].external_port), atoi(rpm->entries[j].external_port_end_range)));
                return OVERLAPPING_SPEEDBOOST_PORT;
            }
#endif

       		if(strcmp(rpm->entries[j].external_port,rpm->entries[j].external_port_end_range) == 0)
        	{
            		spf_count++;
            		snprintf(pf_cache[count].cmd,BLOCK_SIZE,ALIAS_SPF"%d",spf_count);
            		rc = strcpy_s(alias_pre, sizeof(alias_pre),ALIAS_PRE_SPF);
			if(rc != EOK)
			{
				ERR_CHK(rc);
				return -1;
			}
			snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"%s%lu",alias_pre,i);
			rc = sprintf_s(pf_cache[count].cmd,BLOCK_SIZE,"%s%lu"ALIAS_POS_EXT_PORT,alias_pre,i);
			if(rc < EOK) ERR_CHK(rc);
            		snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"%s",rpm->entries[j].external_port);

        	}
        	else
        	{
           		pfr_count++;
           		snprintf(pf_cache[count].cmd,BLOCK_SIZE,ALIAS_PFR"%d",pfr_count);
			rc = strcpy_s(alias_pre, sizeof(alias_pre),ALIAS_PRE_PFR);
			if(rc != EOK)
			{
				ERR_CHK(rc);
				return -1;
			}
            		snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"%s%lu",alias_pre,i);
            		rc = sprintf_s(pf_cache[count].cmd,BLOCK_SIZE,"%s%lu"ALIAS_POS_EXT_PORT_RANGE,alias_pre,i);
			if(rc < EOK) ERR_CHK(rc);
            		snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"%s %s",rpm->entries[j].external_port,rpm->entries[j].external_port_end_range);
            		snprintf(pf_cache[count].cmd,BLOCK_SIZE,"%s%lu"ALIAS_PFR_PUBLIC_IP,alias_pre,i);
            		snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"%s","0.0.0.0");
            		rc = sprintf_s(pf_cache[count].cmd,BLOCK_SIZE,"%s%lu"ALIAS_PFR_INT_RANGE,alias_pre,i);
			if(rc < EOK) ERR_CHK(rc);
            		snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"%s","0");
        	}

        	snprintf(pf_cache[count].cmd,BLOCK_SIZE,"%s%lu"ALIAS_POS_IP,alias_pre,i);


      		snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"%s",rpm->entries[j].internal_client);
        	snprintf(pf_cache[count].cmd,BLOCK_SIZE,"%s%lu"ALIAS_POS_NAME,alias_pre,i);
        	snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"%s",rpm->entries[j].description);
        	snprintf(pf_cache[count].cmd,BLOCK_SIZE,"%s%lu"ALIAS_POS_ENABLE,alias_pre,i);
        	snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"%s",rpm->entries[j].enable);
        	snprintf(pf_cache[count].cmd,BLOCK_SIZE,"%s%lu"ALIAS_POS_PROTO,alias_pre,i);

            if ( ( strcasecmp(rpm->entries[j].protocol,"TCP/UDP") == 0 )|| ( strcasecmp(rpm->entries[j].protocol,"UDP/TCP") == 0 ) || ( strcasecmp(rpm->entries[j].protocol,"BOTH") == 0 )  )
            {
                    snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"both");
 
            }
            else if ( strcasecmp(rpm->entries[j].protocol,"TCP") == 0 )
            {
                snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"tcp");
 
            }
            else if ( strcasecmp(rpm->entries[j].protocol,"UDP") == 0 )
            {
                snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"udp");
 
            }
            else
            {
                return INVALID_PROTOCOL ;
            }
        	rc = sprintf_s(pf_cache[count].cmd,BLOCK_SIZE,"%s%lu"ALIAS_POS_INT_PORT,alias_pre,i);
        	if(rc < EOK) ERR_CHK(rc);
        	snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"0");
        	snprintf(pf_cache[count].cmd,BLOCK_SIZE,"%s%lu"ALIAS_POS_IPV6,alias_pre,i);
        	snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"x");
        	rc = sprintf_s(pf_cache[count].cmd,BLOCK_SIZE,"%s%lu"ALIAS_POS_PREV_STATE,alias_pre,i);
        	if(rc < EOK) ERR_CHK(rc);
        	snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"1");

    	}
        printf("SinglePortForwardCount = %d\n", spf_count);
        printf("PortRangeForwardCount = %d\n", pfr_count);
        snprintf(pf_cache[count].cmd,BLOCK_SIZE,"SinglePortForwardCount");
        snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"%d",spf_count);
        snprintf(pf_cache[count].cmd,BLOCK_SIZE,"PortRangeForwardCount");
        snprintf(pf_cache[count++].val,VAL_BLOCK_SIZE,"%d",pfr_count);
        pf_cache_size =  count;

    return 0;
}

/* Initialize cache , this API will be called once in boot up */
void init_pf_cache(t_cache *tmp_pf_cache)
{
    int i, j;
    int spf_count;
    int pfr_count;
    bool commit = false;
    /*CID: 144097 fix*/
    errno_t rc = -1;

    syscfg_get("CosaNAT", "port_forward_enabled", tmp_pf_cache[0].val, sizeof(tmp_pf_cache[0].val));

    gpfEnable = atoi(tmp_pf_cache[0].val);

    if (syscfg_get(NULL, "SinglePortForwardCount", tmp_pf_cache[0].val, VAL_BLOCK_SIZE) != 0)
    {
        rc = strcpy_s(tmp_pf_cache[0].val, sizeof(tmp_pf_cache[0].val), "0");
        if (rc != EOK)
        {
            ERR_CHK(rc);
        }
    }

    if (syscfg_get(NULL, "PortRangeForwardCount", tmp_pf_cache[1].val, VAL_BLOCK_SIZE) != 0)
    {
        rc = strcpy_s(tmp_pf_cache[1].val, sizeof(tmp_pf_cache[1].val), "0");
        if (rc != EOK)
        {
            ERR_CHK(rc);
        }
    }

    i = 2;

    spf_count = atoi(tmp_pf_cache[0].val);

    for (j = 1; j < (spf_count + 1); j++)
    {
        if (i + 9 > PORTMAP_CACHE_SIZE)
        {
            spf_count = j - 1;
            snprintf(tmp_pf_cache[0].val, VAL_BLOCK_SIZE, "%d", spf_count);
            syscfg_set(NULL, "SinglePortForwardCount", tmp_pf_cache[0].val);
            commit = true;
            break;
        }

        snprintf(tmp_pf_cache[i].cmd, BLOCK_SIZE, "SinglePortForward_%d", j);
        syscfg_get(NULL, tmp_pf_cache[i].cmd, tmp_pf_cache[i].val, VAL_BLOCK_SIZE);

        printf("alias_pre = %s\n", tmp_pf_cache[i].val);

        snprintf(tmp_pf_cache[i + 1].cmd, BLOCK_SIZE, "%s" ALIAS_POS_EXT_PORT,   tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 2].cmd, BLOCK_SIZE, "%s" ALIAS_POS_IP,         tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 3].cmd, BLOCK_SIZE, "%s" ALIAS_POS_NAME,       tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 4].cmd, BLOCK_SIZE, "%s" ALIAS_POS_ENABLE,     tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 5].cmd, BLOCK_SIZE, "%s" ALIAS_POS_PROTO,      tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 6].cmd, BLOCK_SIZE, "%s" ALIAS_POS_INT_PORT,   tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 7].cmd, BLOCK_SIZE, "%s" ALIAS_POS_IPV6,       tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 8].cmd, BLOCK_SIZE, "%s" ALIAS_POS_PREV_STATE, tmp_pf_cache[i].val);

        i += 9;
    }

    pfr_count = atoi(tmp_pf_cache[1].val);

    for (j = 1; j < (pfr_count + 1); j++)
    {
        if (i + 11 > PORTMAP_CACHE_SIZE)
        {
            pfr_count = j - 1;
            snprintf(tmp_pf_cache[1].val, VAL_BLOCK_SIZE, "%d", pfr_count);
            syscfg_set(NULL, "PortRangeForwardCount", tmp_pf_cache[1].val);
            commit = true;
            break;
        }

        snprintf(tmp_pf_cache[i].cmd, BLOCK_SIZE, "PortRangeForward_%d", j);
        syscfg_get(NULL, tmp_pf_cache[i].cmd, tmp_pf_cache[i].val, VAL_BLOCK_SIZE);

        printf("alias_pre = %s\n", tmp_pf_cache[i].val);

        snprintf(tmp_pf_cache[i + 1].cmd, BLOCK_SIZE, "%s" ALIAS_POS_EXT_PORT_RANGE, tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 2].cmd, BLOCK_SIZE, "%s" ALIAS_PFR_PUBLIC_IP,      tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 3].cmd, BLOCK_SIZE, "%s" ALIAS_PFR_INT_RANGE,      tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 4].cmd, BLOCK_SIZE, "%s" ALIAS_POS_IP,             tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 5].cmd, BLOCK_SIZE, "%s" ALIAS_POS_NAME,           tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 6].cmd, BLOCK_SIZE, "%s" ALIAS_POS_ENABLE,         tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 7].cmd, BLOCK_SIZE, "%s" ALIAS_POS_PROTO,          tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 8].cmd, BLOCK_SIZE, "%s" ALIAS_POS_INT_PORT,       tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 9].cmd, BLOCK_SIZE, "%s" ALIAS_POS_IPV6,           tmp_pf_cache[i].val);
        snprintf(tmp_pf_cache[i + 10].cmd, BLOCK_SIZE, "%s" ALIAS_POS_PREV_STATE,    tmp_pf_cache[i].val);

        i += 11;
    }

    pf_cache_size = i;

    if (commit)
    {
        syscfg_commit();
    }
}

/* CallBack API to execute Portforwarding Blob request */
pErr Process_PF_WebConfigRequest(void *Data)
{

    	pErr execRetVal = NULL;
        int lpfEnable = 0;
    	execRetVal = (pErr) malloc (sizeof(Err));
    	if (execRetVal == NULL )
    	{
        	CcspTraceError(("%s : malloc failed\n",__FUNCTION__));
        	return execRetVal;
    	}

    	memset(execRetVal,0,sizeof(Err));

    	execRetVal->ErrorCode = BLOB_EXEC_SUCCESS;

    	portmappingdoc_t *rpm = (portmappingdoc_t *) Data ;


        CcspTraceWarning(("rpm->entries_count is %zu\n", rpm->entries_count));
    	CcspTraceWarning(("Portmap configurartion recieved\n"));


       backup_pf_cache(pf_cache,pf_cache_bkup,pf_cache_size);
       pf_cache_size_bkup = pf_cache_size;

        pthread_mutex_lock(&one_to_one_nat_cache_lock);
        one_to_one_nat_cache_bkup = one_to_one_nat_cache;
        pthread_mutex_unlock(&one_to_one_nat_cache_lock);

        if ( SYSCFG_FAILURE == clear_pf_cache_DB(pf_cache) )
        {
                CcspTraceError(("%s: Failed to clear_pf_cache_DB\n",__FUNCTION__));
             //   portmappingdoc_destroy( rpm ); 

                execRetVal->ErrorCode = SYSCFG_FAILURE;

                strncpy(execRetVal->ErrorMsg,"sysconfig failure while clearing old entries",sizeof(execRetVal->ErrorMsg)-1);

                return execRetVal;

        }

        int ret  = set_portmap_conf(rpm) ; 
    	if ( 0 != ret )
        {
            if ( INVALID_IP == ret )
            {
                CcspTraceError(("%s : Invalid IP\n",__FUNCTION__));
                execRetVal->ErrorCode = INVALID_IP;

                strncpy(execRetVal->ErrorMsg,"Invalid IP",sizeof(execRetVal->ErrorMsg)-1);

            }
            else if ( INVALID_PORT == ret )
            {
                CcspTraceError(("%s : Invalid Port\n",__FUNCTION__));
                execRetVal->ErrorCode = INVALID_PORT;

                strncpy(execRetVal->ErrorMsg,"Invalid Port",sizeof(execRetVal->ErrorMsg)-1);

            }

            else if ( INVALID_PROTOCOL == ret )
            {
                CcspTraceError(("%s : Invalid Protocol\n",__FUNCTION__));
                execRetVal->ErrorCode = INVALID_PROTOCOL;

                strncpy(execRetVal->ErrorMsg,"Invalid Protocol",sizeof(execRetVal->ErrorMsg)-1);

            }
#if defined (SPEED_BOOST_SUPPORTED)
            else if ( OVERLAPPING_SPEEDBOOST_PORT == ret )
            {
                CcspTraceError(("%s : Ports overlap with Speedboost ports\n",__FUNCTION__));
                execRetVal->ErrorCode = OVERLAPPING_SPEEDBOOST_PORT;

                strncpy(execRetVal->ErrorMsg,"Ports overlap with Speedboost ports",sizeof(execRetVal->ErrorMsg)-1);

            }
#endif
            else
            {
                execRetVal->ErrorCode = SYSCFG_FAILURE;
                strncpy(execRetVal->ErrorMsg, "Failed to build port mapping configuration",
                        sizeof(execRetVal->ErrorMsg)-1);
            }

            //portmappingdoc_destroy( rpm ); 


            return execRetVal;
        }
        if ( rpm->entries_count > 0 )
                lpfEnable = 1 ;
        
    	if ( SYSCFG_FAILURE == apply_pf_cache_ToDB(pf_cache,pf_cache_size, lpfEnable ) )
    	{
            	CcspTraceError(("Failed to apply Portmap configurartion\n"));
            	//portmappingdoc_destroy( rpm ); 

            	execRetVal->ErrorCode = SYSCFG_FAILURE;

            	strncpy(execRetVal->ErrorMsg,"sysconfig failure while applying",sizeof(execRetVal->ErrorMsg)-1);

            	return execRetVal;

    	}	

        pthread_mutex_lock(&one_to_one_nat_cache_lock);
        one_to_one_nat_cache = one_to_one_nat_cache_bkup;
        pthread_mutex_unlock(&one_to_one_nat_cache_lock);
     	CcspTraceWarning(("Portmap configurartion applied\n"));
        gpfEnable = lpfEnable;
    	int i;                  
    	for(i = 0; i < (int)rpm->entries_count ; i++)
    	{
            CcspTraceWarning(("rpm->entries[%d].InternalClient %s\n", i, rpm->entries[i].internal_client));
            CcspTraceWarning(("rpm->entries[%d].ExternalPortEndRange %s\n" , i, rpm->entries[i].external_port_end_range));
            CcspTraceWarning(("rpm->entries[%d].Enable %s\n", i, rpm->entries[i].enable?"true":"false"));
            CcspTraceWarning(("rpm->entries[%d].Protocol %s\n", i, rpm->entries[i].protocol));
            CcspTraceWarning(("rpm->entries[%d].Description %s\n", i, rpm->entries[i].description));
            CcspTraceWarning(("rpm->entries[%d].external_port %s\n", i, rpm->entries[i].external_port));
    	}

      //  portmappingdoc_destroy( rpm );  

    	return execRetVal;

}

/* Callback function to rollback when port forwarding blob execution fails */
int rollback_PortForwarding()
{
    // return 0 to notify framework when rollback is success
    CcspTraceInfo((" Entering %s \n",__FUNCTION__));

    int ret = 0;
    ret = apply_pf_cache_ToDB(pf_cache_bkup,pf_cache_size_bkup,  gpfEnable ) ;

    backup_pf_cache(pf_cache_bkup,pf_cache,pf_cache_size_bkup);

    pf_cache_size = pf_cache_size_bkup;
    pthread_mutex_lock(&one_to_one_nat_cache_lock);
    one_to_one_nat_cache = one_to_one_nat_cache_bkup;
    pthread_mutex_unlock(&one_to_one_nat_cache_lock);


    return ret ;
}


void freeResources_PortForwarding(void *arg)
{

    CcspTraceInfo((" Entering %s \n",__FUNCTION__));
    execData *blob_exec_data  = (execData*) arg;
    /* CID: 158629 Dereference before null check*/   
    if(!blob_exec_data)
        return;

    portmappingdoc_t *rpm = (portmappingdoc_t *) blob_exec_data->user_data ;

    if ( rpm != NULL )
    {
        portmappingdoc_destroy( rpm );  
        rpm = NULL;
    }

    if ( blob_exec_data != NULL )
    {
        free(blob_exec_data);
        blob_exec_data = NULL ;
    }
}
