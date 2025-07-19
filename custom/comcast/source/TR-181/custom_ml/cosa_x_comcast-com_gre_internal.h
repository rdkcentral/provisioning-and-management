/************************************************************************************
  If not stated otherwise in this file or this component's Licenses.txt file the
  following copyright and licenses apply:

  Copyright 2018 RDK Management

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
**************************************************************************/

#ifdef CONFIG_CISCO_HOTSPOT
/**************************************************************************

    module: cosa_x_comcast-com_gre_internal.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        zhicheng_qiu@cable.comcast.com

    -------------------------------------------------------------------

    revision:

        05/18/2015    initial revision.

**************************************************************************/

#ifndef  _COSA_X_COMCAST__COM_GRE_INTERNAL_H
#define  _COSA_X_COMCAST__COM_GRE_INTERNAL_H
#include "cosa_apis.h"
#include "cosa_x_comcast-com_gre_apis.h"

typedef  struct
_COSA_DATAMODEL_GRE2
{
    COSA_BASE_CONTENT
	COSA_DML_GRE_TUNNEL  GreTu[MAX_GRE_TU];
} COSA_DATAMODEL_GRE2;

	
ANSC_HANDLE CosaGreTunnelCreate ();	//

ANSC_STATUS CosaGreTunnelInitialize  ( ANSC_HANDLE  hThisObject ); 	//
ANSC_STATUS CosaGreTunnelIfInitialize  (ULONG tuIns, ANSC_HANDLE  hThisObject );	//
	
ANSC_STATUS CosaGreTunnelRemove ( ANSC_HANDLE hThisObject );	//
	
#endif
#endif
