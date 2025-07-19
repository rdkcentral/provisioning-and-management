/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2024 RDK Management
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
#include "helpers.h"
#include "webui_doc.h"
#include "ansc_platform.h"

/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/
enum {
    OK                       = HELPERS_OK,
    OUT_OF_MEMORY            = HELPERS_OUT_OF_MEMORY,
    INVALID_FIRST_ELEMENT    = HELPERS_INVALID_FIRST_ELEMENT,
    MISSING_ENTRY         = HELPERS_MISSING_WRAPPER,
    INVALID_OBJECT,
    INVALID_VERSION,
};

/*----------------------------------------------------------------------------*/
/*                             Function Prototypes                            */
/*----------------------------------------------------------------------------*/
int process_webui_params( webui_param_t *e, msgpack_object_map *map );
int process_webui_doc( webui_doc_t *doc, int num, ...);

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/

webui_doc_t* webui_doc_convert( const void *buf, size_t len )
{
     return helper_convert( buf, len, sizeof(webui_doc_t), WEBUI_WEBCONFIG_SUBDOC_NAME,
                            MSGPACK_OBJECT_MAP, true,
                           (process_fn_t) process_webui_doc,
                           (destroy_fn_t) webui_doc_destroy);
}

void webui_doc_destroy( webui_doc_t *doc )
{
     if( NULL != doc )
     {
        if( NULL != doc->subdoc_name )
        {
            AnscFreeMemory ( doc->subdoc_name );
        }
        if( NULL != doc->param )
        {
            AnscFreeMemory ( doc->param );
        }
        AnscFreeMemory ( doc );
     }
}

const char* webui_doc_strerror( int errnum )
{
    struct error_map {
        int v;
        const char *txt;
    } map[] = {
        { .v = OK,                               .txt = "No errors." },
        { .v = OUT_OF_MEMORY,                    .txt = "Out of memory." },
        { .v = INVALID_FIRST_ELEMENT,            .txt = "Invalid first element." },
        { .v = INVALID_VERSION,                 .txt = "Invalid 'version' value." },
        { .v = INVALID_OBJECT,                .txt = "Invalid 'value' array." },
        { .v = 0, .txt = NULL }
    };
    int i = 0;
    while( (map[i].v != errnum) && (NULL != map[i].txt) ) { i++; }
    if( NULL == map[i].txt )
    {
        CcspTraceError(("----%s----\n", __FUNCTION__));
        return "Unknown error.";
    }
    return map[i].txt;
}

/*----------------------------------------------------------------------------*/
/*                             Internal functions                             */
/*----------------------------------------------------------------------------*/
int process_webui_params( webui_param_t *e, msgpack_object_map *map )
{
    msgpack_object_kv *p;
    p = map->ptr;

    if( MSGPACK_OBJECT_STR == p->key.type ) {
        if( 0 == match(p, "Mode") ) {
            if( MSGPACK_OBJECT_STR == p->val.type ) {
                e->mode = strndup( p->val.via.str.ptr, p->val.via.str.size );
            }
        }
        return 0;
    }

    return -1;
}

int process_webui_doc( webui_doc_t *doc,int num, ... )
{
    //To access the variable arguments use va_list
    va_list valist;
    va_start(valist, num);//start of variable argument loop

    msgpack_object *obj = va_arg(valist, msgpack_object *);//each usage of va_arg fn argument iterates by one time
    msgpack_object_map *mapobj = &obj->via.map;

    msgpack_object *obj1 = va_arg(valist, msgpack_object *);
    doc->subdoc_name = strndup( obj1->via.str.ptr, obj1->via.str.size );

    msgpack_object *obj2 = va_arg(valist, msgpack_object *);
    doc->version = (uint32_t) obj2->via.u64;

    msgpack_object *obj3 = va_arg(valist, msgpack_object *);
    doc->transaction_id = (uint16_t) obj3->via.u64;
    va_end(valist);//End of variable argument loop

    doc->param = (webui_param_t *) AnscAllocateMemory( sizeof(webui_param_t) );
    if( NULL == doc->param )
    {
        CcspTraceError(("%s entries count AnscAllocateMemory failed\n", __FUNCTION__));
        return -1;
    }
    memset( doc->param, 0, sizeof(webui_param_t));

    if( 0 != process_webui_params(doc->param, mapobj) )
    {
       CcspTraceError(("%s failed\n", __FUNCTION__));
       return -1;
    }

    return 0;
}