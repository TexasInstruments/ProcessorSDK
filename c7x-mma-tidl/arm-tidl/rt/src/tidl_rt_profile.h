/*
*
* Copyright (c) {2015 - 2024} Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

/**
@file      itidl_rt_profile.h
@brief     This file defines an interface for profiling the TIDL Runtime API. \n
           This interface is targeted for profiling within the TIDL RT stack 

@version 0.1 April 2024 : Initial Code

*/

#ifndef ITIDL_RT_PROFILE_H_
#define ITIDL_RT_PROFILE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TIDLRT_PROFILE_TIDLRT_CREATE               (0) 
#define TIDLRT_PROFILE_TIDL_RT_OVX_INIT            (1) 
#define TIDLRT_PROFILE_VXCREATECONTEXT             (2) 
#define TIDLRT_PROFILE_INIT_TIDL_TIOVX             (3) 
#define TIDLRT_PROFILE_CREATE_GRAPH_TIDL_TIOVX     (4) 
#define TIDLRT_PROFILE_VERIFY_GRAPH_TIDL_TIOVX     (5)
#define TIDLRT_PROFILE_TIVX_TIDL_LOAD_KERNEL       (6) 
#define TIDLRT_PROFILE_MAPCONFIG                   (7) 
#define TIDLRT_PROFILE_TIVXADDKERNELTIDL           (8) 
#define TIDLRT_PROFILE_MAPNETWORK                  (9) 
#define TIDLRT_PROFILE_SETCREATEPARAMS             (10)
#define TIDLRT_PROFILE_SETARGS                     (11)
#define TIDLRT_PROFILE_VX_CREATE_USER_DATA_OBJECT  (12)
#define TIDLRT_PROFILE_VX_MAP_USER_DATA_OBJECT     (13)
#define TIDLRT_PROFILE_MEMCOPY_NETWORK_BUFFER      (14)
#define TIDLRT_PROFILE_VX_UNMAP_USER_DATA_OBJECT   (15)
#define TIDLRT_PROFILE_MAX_POINTS                  (16)

typedef struct
{
  char   string[64];
  int32_t profileIdx;
} sTIDLProfilePrintInfo_t ;

typedef struct{
  uint64_t startTime;
  uint64_t totalTime;
} sProfileInfo_t;

typedef struct{
    uint32_t enable;
    sProfileInfo_t points[TIDLRT_PROFILE_MAX_POINTS];
} sProfilePoints_t;

void init_rt_profile(sProfilePoints_t *profiler, int32_t traceLogLevel);

void printProfileInfo(sProfilePoints_t* profile_points);

void TIDLRT_profileStart(sProfilePoints_t*  profile_points, int32_t profileName);

void TIDLRT_profileEnd(sProfilePoints_t*  profile_points, int32_t profileName);

#ifdef __cplusplus
}
#endif

/*@}*/
/* ITIDL_RT_PROFILE */

#endif /*ITIDL_RT_PROFILE_H_ */