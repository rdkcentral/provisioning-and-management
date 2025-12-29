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
#include <errno.h>
#include <string.h>
#include <msgpack.h>
#include <stdarg.h>
#include "ccsp_trace.h"
#include "managedwifi_helpers.h"
#include "managedwifidoc.h"


/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/
/* none */
/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/
enum {
    OK                       = MW_HELPERS_OK,
    OUT_OF_MEMORY            = MW_HELPERS_OUT_OF_MEMORY,
    INVALID_FIRST_ELEMENT    = MW_HELPERS_INVALID_FIRST_ELEMENT,
    MISSING_ENTRY            = MW_HELPERS_MISSING_WRAPPER,
    INVALID_DATATYPE,
    INVALID_OBJECT,
    INVALID_VERSION,
};
/*----------------------------------------------------------------------------*/
/*                            File Scoped Variables                           */
/*----------------------------------------------------------------------------*/
/* none */
/*----------------------------------------------------------------------------*/
/*                             Function Prototypes                            */
/*----------------------------------------------------------------------------*/
int process_lanConfigParams( lanconfig_t *e, msgpack_object_map *map );
int process_lanConfigDoc( lanconfigdoc_t *hd, int num, ...);
int process_connectedbuilding( managedwifidoc_t *hd, int num, ...);
int process_connectedbuildingparams( mwparam_t *e, msgpack_object_map *map );

#if defined (AMENITIES_NETWORK_ENABLED)
static int processTunnelLanCfg( lanconfigTunnelInfo_t *pTunnelLanCfg, int iNum, ...);
#endif
/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
/* See managedwifidoc.h for details. */
managedwifidoc_t* managedwifidoc_convert( const void *buf, size_t len )
{
    return comp_mw_helper_convert( buf, len, sizeof(managedwifidoc_t), "Parameters",
                            MSGPACK_OBJECT_ARRAY, true,
                           (process_fn_t) process_connectedbuilding,
                           (destroy_fn_t) connectedbuilding_destroy);
}

#if defined (AMENITIES_NETWORK_ENABLED)
void enableAmenityDestroy( bool *pIsAmenityEnabled )
{
    if (NULL != pIsAmenityEnabled)
    {
        free(pIsAmenityEnabled);
    }
}
int proccessEnableAmenity ( bool *pIsAmenityEnabled, int iNum, ...)
{
    if ((NULL == pIsAmenityEnabled) || (0 == iNum))
    {
        CcspTraceError(("%s:%d, pIsAmenityEnabled is NULL or iNum is 0\n",__FUNCTION__,__LINE__));
        return -1;
    }

    va_list valist;
    va_start(valist, iNum);//start of variable argument loop
    msgpack_object *pMsgPackObj = va_arg(valist, msgpack_object *);//each usage of va_arg fn argument iterates by one time
    if (NULL == pMsgPackObj)
    {
        CcspTraceError(("%s:%d, pMsgPackObj is NULL\n",__FUNCTION__,__LINE__));
        va_end(valist);//End of variable argument loop
        return -1;
    }
    else if (MSGPACK_OBJECT_BOOLEAN != pMsgPackObj->type)
    {
        CcspTraceError(("%s:%d, pMsgPackObj is NULL Or\n", __FUNCTION__, __LINE__));
        CcspTraceError(("%s:%d, pMsgPackObj->type is %d Or\n", __FUNCTION__, __LINE__, pMsgPackObj->type));
        va_end(valist); // End of variable argument loop
        return -1;
    }
    else
    {
        va_end(valist);//End of variable argument loop
        *pIsAmenityEnabled = pMsgPackObj->via.boolean;
        CcspTraceInfo(("%s:%d, Enable Amenity: %s\n",__FUNCTION__,__LINE__, (*pIsAmenityEnabled) ? "true" : "false"));
        return 0;
    }
    CcspTraceError(("%s:%d, Enable Amenity is not boolean\n",__FUNCTION__,__LINE__));
    return -1;
}
lanconfigTunnelInfo_t* lanconfigdoc_convert( const void *buf, size_t len )
{
    if ((NULL == buf) || (0 == len))
    {
        CcspTraceError(("%s:%d, buf is NULL or len is zero\n",__FUNCTION__,__LINE__));
        return NULL;
    }

    bool *pIsAmenityEnabled = (bool*)comp_mw_helper_convert( buf, len, sizeof(bool), "Enable",
                            MSGPACK_OBJECT_STR, true,
                           (process_fn_t) proccessEnableAmenity,
                           (destroy_fn_t) enableAmenityDestroy );

    lanconfigTunnelInfo_t * pLanCfgTunnelInfo = (lanconfigTunnelInfo_t*) comp_mw_helper_convert( buf, len, sizeof(lanconfigTunnelInfo_t), "Tunnels_Network",
                            MSGPACK_OBJECT_ARRAY, true,
                           (process_fn_t) processTunnelLanCfg,
                           (destroy_fn_t) tunnelLanConfigDocdestroy );
    if (NULL != pLanCfgTunnelInfo)
    {
        pLanCfgTunnelInfo->pIsAmenityEnabled = pIsAmenityEnabled;
        CcspTraceInfo(("%s:%d, pIsAmenityEnabled:%s\n",__FUNCTION__,__LINE__,*(pLanCfgTunnelInfo->pIsAmenityEnabled)? "true":"false"));
    }
    else
    {
        CcspTraceError (("%s:%d, pLanCfgTunnelInfo is NULL\n",__FUNCTION__,__LINE__));
    }

    return pLanCfgTunnelInfo;
}
#else
lanconfigdoc_t* lanconfigdoc_convert( const void *buf, size_t len )
{
    CcspTraceInfo(("%s:%d, buf = %p, len = %zu\n", __FUNCTION__, __LINE__, buf, len));
    return comp_mw_helper_convert( buf, len, sizeof(lanconfigdoc_t), "lanconfig",
                            MSGPACK_OBJECT_MAP, true,
                           (process_fn_t) process_lanConfigDoc,
                           (destroy_fn_t) lanConfigDoc_destroy );
}
#endif

void lanConfigDoc_destroy( lanconfigdoc_t *lwd )
{
    if( NULL != lwd )
    {
        if( NULL != lwd->entries )
        {
            free( lwd->entries );
        }
        free( lwd );
    }
}

#if defined (AMENITIES_NETWORK_ENABLED)
void tunnelLanConfigDocdestroy( lanconfigTunnelInfo_t *pTunnelLanCfg)
{
    if(NULL != pTunnelLanCfg)
    {
        if (NULL != pTunnelLanCfg->pIsAmenityEnabled)
        {
            free(pTunnelLanCfg->pIsAmenityEnabled);
            pTunnelLanCfg->pIsAmenityEnabled = NULL;
        }
        if(NULL != pTunnelLanCfg->ppVapNames)
        {
            for (int iCount = 0; iCount < pTunnelLanCfg->iVapCount; iCount++)
            {
                if (NULL != pTunnelLanCfg->ppVapNames[iCount])
                {
                    free(pTunnelLanCfg->ppVapNames[iCount]);
                    pTunnelLanCfg->ppVapNames[iCount] = NULL;
                }
            }
            free(pTunnelLanCfg->ppVapNames);
            pTunnelLanCfg->ppVapNames = NULL;
        }
        if (NULL != pTunnelLanCfg->pVlanIds)
        {
            free(pTunnelLanCfg->pVlanIds);
            pTunnelLanCfg->pVlanIds = NULL;
        }
        free(pTunnelLanCfg); /* CID 346829 fix - Resource leak prevention */
    }
}
#endif

void connectedbuilding_destroy(managedwifidoc_t *mwd)
{
    if( NULL != mwd )
    {
        free( mwd );
    }
}

/* See mangagedwifidoc.h for details. */
const char* managedwifidoc_strerror( int errnum )
{
    struct error_map {
        int v;
        const char *txt;
    } map[] = {
        { .v = OK,                               .txt = "No errors." },
        { .v = OUT_OF_MEMORY,                    .txt = "Out of memory." },
        { .v = INVALID_FIRST_ELEMENT,            .txt = "Invalid first element." },
        { .v = INVALID_VERSION,                  .txt = "Invalid 'version' value." },
        { .v = INVALID_OBJECT,                   .txt = "Invalid 'value' array." },
        { .v = 0, .txt = NULL }
    };
    int i = 0;
    while( (map[i].v != errnum) && (NULL != map[i].txt) ) { i++; }
    if( NULL == map[i].txt )
    {
        CcspTraceWarning(("----managedwifidoc_strerror----\n"));
        return "Unknown error.";
    }
    return map[i].txt;
}
/*----------------------------------------------------------------------------*/
/*                             Internal functions                             */
/*----------------------------------------------------------------------------*/
int process_lanConfigParams( lanconfig_t *e, msgpack_object_map *map )
{
    int left = map->size;
    msgpack_object_kv *p;

    p = map->ptr;
    while( left-- >= 0 )
    {
        if( MSGPACK_OBJECT_STR == p->key.type )
        {
            if(MSGPACK_OBJECT_BOOLEAN  == p->val.type )
            {
                if( 0 == match(p, "Enable") )
                {
                    e->mwenable = p->val.via.boolean;
                }
                if( 0 == match(p, "DhcpServerEnable"))
                {
                    e->dhcp_server_enable = p->val.via.boolean;
                }
                if( 0 == match(p, "IPv6"))
                {
                    e->ipv6enable = p->val.via.boolean;
                }
            }
            else if( MSGPACK_OBJECT_STR == p->val.type )
            {
                if( 0 == match(p, "LanBridge") )
                {
                    e->alias = strndup( p->val.via.str.ptr, p->val.via.str.size );
                }

                if( 0 == match(p, "LanIPAddress") )
                {
                     e->lan_ip_address = strndup( p->val.via.str.ptr, p->val.via.str.size );
                }
                if( 0 == match(p, "LanSubnetMask") )
                {
                    e->lan_subnet_mask = strndup( p->val.via.str.ptr, p->val.via.str.size );
                }
                if( 0 == match(p, "DhcpStartIPAddress") )
                {
                    e->dhcp_start_ip_address = strndup( p->val.via.str.ptr, p->val.via.str.size );
                }
                if( 0 == match(p, "DhcpEndIPAddress") )
                {
                    e->dhcp_end_ip_address = strndup( p->val.via.str.ptr, p->val.via.str.size );
                }
                if( 0 == match(p, "Firewall") )
                {
                    e->firewall = strndup( p->val.via.str.ptr, p->val.via.str.size );
                }
                if( 0 == match(p, "LeaseTime") )
                {
                    e->leasetime = strndup( p->val.via.str.ptr, p->val.via.str.size );
                }
            }
        }
        p++;
        if(left == 0){
            break;
        }
    }
    if( 1 & left ) {
    } else {
        errno = OK;
    }
    return (0 == left) ? 0 : -1;
}

#if defined (AMENITIES_NETWORK_ENABLED)
int processTunnelMapInfo (lanconfigTunnelInfo_t *pTunnelLanCfg, msgpack_object_map *pMsgPackObjMap, int iMapCount)
{
    if ((NULL == pTunnelLanCfg) || (NULL == pMsgPackObjMap) || (0 == pMsgPackObjMap->size) || (NULL == pMsgPackObjMap->ptr))
    {
        CcspTraceError(("%s:%d, pTunnelLanCfg or pMsgPackObjMap is NULL or size is 0\n",__FUNCTION__,__LINE__));
        return -1;
    }

    int iMsgObjMapSize = pMsgPackObjMap->size;
    CcspTraceInfo(("%s:%d, iMsgObjMapSize = %d\n",__FUNCTION__,__LINE__, iMsgObjMapSize));
    CcspTraceInfo(("%s:%d, iMapCount = %d\n",__FUNCTION__,__LINE__, iMapCount));
    msgpack_object_kv *pMsgPackObjKv = pMsgPackObjMap->ptr;
    int iFound = -1;

    for (int iCount = 0; iCount < iMsgObjMapSize ; iCount++)
    {
        if ((NULL == pMsgPackObjKv) || ((NULL == pMsgPackObjKv->key.via.str.ptr) && (NULL == pMsgPackObjKv->val.via.str.ptr)))
        {
            CcspTraceError(("%s:%d, Value is NULL\n",__FUNCTION__,__LINE__));
            return -1;
        }

        if ((MSGPACK_OBJECT_STR == pMsgPackObjKv->key.type) && (NULL != pMsgPackObjKv->key.via.str.ptr))
        {
            if ((0 == match (pMsgPackObjKv, "VapName")) && (MSGPACK_OBJECT_STR == pMsgPackObjKv->val.type))
            {
                pTunnelLanCfg->ppVapNames[iMapCount] = strndup(pMsgPackObjKv->val.via.str.ptr, pMsgPackObjKv->val.via.str.size);
                if (NULL == pTunnelLanCfg->ppVapNames[iMapCount])
                {
                    CcspTraceError (("%s:%d, ppVapNames[%d] strndup failed\n",__FUNCTION__,__LINE__, iMapCount));
                    return -1;
                }
                iFound = 0;
                CcspTraceInfo(("%s:%d, pTunnelLanCfg->ppVapNames[%d]: %s\n",__FUNCTION__,__LINE__, iMapCount, pTunnelLanCfg->ppVapNames[iMapCount]));
            }
            else if ((0 == match (pMsgPackObjKv, "VLANID")) && (MSGPACK_OBJECT_POSITIVE_INTEGER == pMsgPackObjKv->val.type))
            {
                pTunnelLanCfg->pVlanIds[iMapCount] = (int)pMsgPackObjKv->val.via.u64;
                iFound = 0;
                CcspTraceInfo(("%s:%d, pTunnelLanCfg->pVlanIds[%d]: %d\n",__FUNCTION__,__LINE__, iMapCount, pTunnelLanCfg->pVlanIds[iMapCount]));
            }
        }
        pMsgPackObjKv++;
    }
    if (-1 == iFound)
    {
        CcspTraceError(("%s:%d, Not found any VapName's or VLANID's \n",__FUNCTION__,__LINE__));
        return -1;
    }
    return 0;
}

int processLanCfgTunnelInfo (lanconfigTunnelInfo_t *pTunnelLanCfg, msgpack_object_array *pMsgPackObjArray)
{
    if ((NULL == pTunnelLanCfg) || (NULL == pMsgPackObjArray) || (0 == pMsgPackObjArray->size))
    {
        CcspTraceError(("%s:%d, pTunnelLanCfg or pMsgPackObjArray is NULL or size is 0\n",__FUNCTION__,__LINE__));
        return -1;
    }
    if ( NULL == pMsgPackObjArray->ptr )
    {
        CcspTraceError(("%s:%d, pMsgPackObjArray->ptr is NULL\n",__FUNCTION__,__LINE__));
        return -1;
    }

    int iMsgObjArraySize = pMsgPackObjArray->size;
    CcspTraceInfo(("%s:%d, iMsgObjArraySize = %d\n",__FUNCTION__,__LINE__, iMsgObjArraySize));
    pTunnelLanCfg->ppVapNames = (char **)malloc(sizeof(char *) * iMsgObjArraySize);
    if (NULL == pTunnelLanCfg->ppVapNames)
    {
        CcspTraceError (("%s:%d, ppVapNames malloc failed\n",__FUNCTION__,__LINE__));
        return -1;
    }
    memset(pTunnelLanCfg->ppVapNames, 0, sizeof(char *) * iMsgObjArraySize);
    pTunnelLanCfg->pVlanIds = (int *)malloc(sizeof(int) * iMsgObjArraySize);
    if (NULL == pTunnelLanCfg->pVlanIds)
    {
        CcspTraceError (("%s:%d, pVlanIds malloc failed\n",__FUNCTION__,__LINE__));
        free(pTunnelLanCfg->ppVapNames);
        return -1;
    }
    memset(pTunnelLanCfg->pVlanIds, 0, sizeof(int) * iMsgObjArraySize);
    pTunnelLanCfg->iVapCount = iMsgObjArraySize;
    msgpack_object *pMsgPackObj = pMsgPackObjArray->ptr;
    int iMapCount = 0;

    for (int iCount = 0; iCount < iMsgObjArraySize; iCount++)
    {
        if (MSGPACK_OBJECT_MAP == pMsgPackObj[iCount].type)
        {
            msgpack_object_kv *pMsgPackObjKv = pMsgPackObj[iCount].via.map.ptr;
            if ((NULL == pMsgPackObjKv) || (0 == pMsgPackObj[iCount].via.map.size))
            {
                CcspTraceError(("%s:%d, pMsgPackObjKv is NULL or size is 0\n",__FUNCTION__,__LINE__));
                free(pTunnelLanCfg->ppVapNames);
                free(pTunnelLanCfg->pVlanIds);
                return -1;
            }
            CcspTraceInfo(("%s:%d, iMapCount = %d\n",__FUNCTION__,__LINE__, iMapCount));
            if (0 != processTunnelMapInfo(pTunnelLanCfg, &pMsgPackObj[iCount].via.map, iMapCount))
            {
                CcspTraceError(("%s:%d, processTunnelMapInfo failed\n",__FUNCTION__,__LINE__));
                free(pTunnelLanCfg->ppVapNames);
                free(pTunnelLanCfg->pVlanIds);
                return -1;
            }
            iMapCount++;
        }
        else
        {
            CcspTraceWarning(("%s:%d, pMsgPackObj->type is %d\n",__FUNCTION__,__LINE__, pMsgPackObj->type));
        }
    }
    return 0;
}
static int processTunnelLanCfg( lanconfigTunnelInfo_t *pTunnelLanCfg, int iNum, ...)
{
    CcspTraceInfo(("%s:%d, iNum = %d\n", __FUNCTION__, __LINE__, iNum));
    if ((NULL == pTunnelLanCfg) || (0 == iNum))
    {
        CcspTraceError(("%s:%d, pTunnelLanCfg is NULL or iNum is 0\n",__FUNCTION__,__LINE__));
        return -1;
    }

    va_list valist;
    va_start(valist, iNum);//start of variable argument loop
    msgpack_object *pMsgPackObj = va_arg(valist, msgpack_object *);//each usage of va_arg fn argument iterates by one time
    if (NULL == pMsgPackObj)
    {
        CcspTraceError(("%s:%d, pMsgPackObj is NULL\n",__FUNCTION__,__LINE__));
        va_end(valist);//End of variable argument loop
        return -1;
    }
    else if ((MSGPACK_OBJECT_ARRAY != pMsgPackObj->type) || (0 == pMsgPackObj->via.array.size))
    {
        CcspTraceError(("%s:%d, pMsgPackObj is NULL Or\n", __FUNCTION__, __LINE__));
        CcspTraceError(("%s:%d, pMsgPackObj->type is %d Or\n", __FUNCTION__, __LINE__, pMsgPackObj->type));
        CcspTraceError(("%s:%d, pMsgPackObj->via.array.size is %u\n", __FUNCTION__, __LINE__, (unsigned int)pMsgPackObj->via.array.size));
        va_end(valist); // End of variable argument loop
        return -1;
    }

    msgpack_object_array *pMsgPackObjArray = &pMsgPackObj->via.array;
    va_end(valist);//End of variable argument loop

    return processLanCfgTunnelInfo(pTunnelLanCfg, pMsgPackObjArray);
}
#endif

//process lanconfig docs params
int process_lanConfigDoc( lanconfigdoc_t *mwd,int num, ... )
{
    //To access the variable arguments use va_list
    va_list valist;
    va_start(valist, num);//start of variable argument loop
    msgpack_object *obj = va_arg(valist, msgpack_object *);//each usage of va_arg fn argument iterates by one time
    msgpack_object_map *mapobj = &obj->via.map;

    va_end(valist);//End of variable argument loop
    mwd->entries = (lanconfig_t *) malloc( sizeof(lanconfig_t) );
    if( NULL == mwd->entries )
    {
        CcspTraceWarning(("entries count malloc failed\n"));
        return -1;
    }
    memset( mwd->entries, 0, sizeof(lanconfig_t));
    if( 0 != process_lanConfigParams(mwd->entries, mapobj))
    {
        CcspTraceWarning(("process_managedwifidoc failed\n"));
        return -1;
    }
    return 0;
}

//process connected building message pack
int process_connectedbuilding( managedwifidoc_t *md, int num, ...)
{
    //To access the variable arguments use va_list
    va_list valist;
    va_start(valist, num);//start of variable argument loop

    msgpack_object *obj = va_arg(valist, msgpack_object *);//each usage of va_arg fn argument iterates by one time
    msgpack_object_array *array = &obj->via.array;

    msgpack_object *obj1 = va_arg(valist, msgpack_object *);
    md->subdoc_name = strndup( obj1->via.str.ptr, obj1->via.str.size );

    msgpack_object *obj2 = va_arg(valist, msgpack_object *);
    md->version = (uint32_t) obj2->via.u64;

    msgpack_object *obj3 = va_arg(valist, msgpack_object *);
    md->transaction_id = (uint16_t) obj3->via.u64;

    va_end(valist);//End of variable argument loop

    if( 0 < array->size )
    {
        size_t i;

        md->entries_count = array->size;
        md->entries = (mwparam_t *) malloc( sizeof(mwparam_t) * md->entries_count );
        if( NULL == md->entries )
        {
            md->entries_count = 0;
            return -1;
        }

        memset( md->entries, 0, sizeof(mwparam_t) * md->entries_count );
        for( i = 0; i < md->entries_count; i++ )
        {
            if( MSGPACK_OBJECT_MAP != array->ptr[i].type )
            {
                errno = INVALID_OBJECT;
                CcspTraceWarning(("Invalid object\n"));
                return -1;
            }
            if( 0 != process_connectedbuildingparams(&md->entries[i], &array->ptr[i].via.map))
            {
                CcspTraceWarning(("process_connectedbuilding failed\n"));
                return -1;
            }
        }
    }
    return 0;
}

//process connected building params
int process_connectedbuildingparams( mwparam_t *e, msgpack_object_map *map )
{
    int left = map->size;
    uint8_t objects_left = 0x02;
    msgpack_object_kv *p;

    p = map->ptr;
    while( (0 < objects_left) && (0 < left--) )
    {
        if( MSGPACK_OBJECT_STR == p->key.type )
        {
            if( MSGPACK_OBJECT_STR == p->val.type )
            {
                if( 0 == match(p, "Name") )
                {
                    e->name = strndup( p->val.via.str.ptr, p->val.via.str.size );
                    objects_left &= ~(1 << 0);
                }
                if( 0 == match(p, "Value"))
                {
                    e->value = malloc(sizeof(char) * p->val.via.str.size+1 );
                    memset( e->value, 0, sizeof(char) * p->val.via.str.size+1);
                    e->value = memcpy(e->value, p->val.via.str.ptr, p->val.via.str.size+1 );
                    e->value[p->val.via.str.size] = '\0';
                    e->value_size =(uint32_t) p->val.via.str.size;
                    CcspTraceWarning(("e->value_size is %lu\n", (long)e->value_size));
                    objects_left &= ~(1 << 1);
                }
            }
        }
        p++;
    }

    if( 1 & objects_left ) {
    } else {
        errno = OK;
    }
    return (0 == objects_left) ? 0 : -1;
}
