/**
 *   @file  mcasp_types.h
 *
 *   @brief   
 *      This is a wrapper header file which includes the standard types
 *      which are used by the MCASP Driver.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2012 Texas Instruments, Inc.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  \par
*/
#ifndef MCASP_TYPES_H
#define MCASP_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <ti/csl/tistdtypes.h>

/* In direction for IOM layer */
#ifdef USE_BIOS

#define mcasp_type_Ptr Ptr
#define mcasp_type_IOM_Packet IOM_Packet
#define mcasp_type_IOM_TiomCallback IOM_TiomCallback

#define mcasp_type_IOM_TmdBindDev IOM_TmdBindDev
#define mcasp_type_IOM_TmdUnBindDev IOM_TmdUnBindDev
#define mcasp_type_IOM_TmdControlChan IOM_TmdControlChan
#define mcasp_type_IOM_TmdCreateChan IOM_TmdCreateChan
#define mcasp_type_IOM_TmdDeleteChan IOM_TmdDeleteChan
#define mcasp_type_IOM_TmdDeleteChan IOM_TmdDeleteChan
#define mcasp_type_IOM_TmdSubmitChan IOM_TmdSubmitChan


#define mcasp_type_IOM_Fxns IOM_Fxns

#endif

#if defined(FREERTOS) || defined(SAFERTOS)

#define mcasp_type_Ptr void *

typedef struct mcasp_type_IOM_Packet {      /* frame object */
    void *              link;   /* queue link of type Queue_Elem - not used in this implementation */
    void *              addr;   /* buffer address */
    uint32_t            size;   /* buffer size */
    void *              arg;    /* arg to be used by end app */

    uint32_t            cmd;    /* command for mini-driver */
    int32_t             status; /* status of command */

    void *              misc;   /* reserved for driver */
} mcasp_type_IOM_Packet;


typedef void    (*mcasp_type_IOM_TiomCallback)(mcasp_type_Ptr arg, mcasp_type_IOM_Packet *packet);

/*
 *  The following function prototypes define the mini-driver functions.
 */
typedef int32_t  (*mcasp_type_IOM_TmdBindDev)(mcasp_type_Ptr *devp, int32_t devid, mcasp_type_Ptr devParams);
typedef int32_t  (*mcasp_type_IOM_TmdUnBindDev)(mcasp_type_Ptr devp);
typedef int32_t  (*mcasp_type_IOM_TmdControlChan)(mcasp_type_Ptr chanp, uint32_t cmd, mcasp_type_Ptr arg);
typedef int32_t  (*mcasp_type_IOM_TmdCreateChan)(mcasp_type_Ptr *chanp, mcasp_type_Ptr devp, String name, int32_t mode,
                      mcasp_type_Ptr chanParams, mcasp_type_IOM_TiomCallback cbFxn, mcasp_type_Ptr cbArg);
typedef int32_t  (*mcasp_type_IOM_TmdDeleteChan)(mcasp_type_Ptr chanp);
typedef int32_t  (*mcasp_type_IOM_TmdSubmitChan)(mcasp_type_Ptr chanp, mcasp_type_IOM_Packet *packet);


typedef struct mcasp_type_IOM_Fxns
{
    mcasp_type_IOM_TmdBindDev       mdBindDev;
    mcasp_type_IOM_TmdUnBindDev     mdUnBindDev;
    mcasp_type_IOM_TmdControlChan   mdControlChan;
    mcasp_type_IOM_TmdCreateChan    mdCreateChan;
    mcasp_type_IOM_TmdDeleteChan    mdDeleteChan;
    mcasp_type_IOM_TmdSubmitChan    mdSubmitChan;

} mcasp_type_IOM_Fxns;

#endif

#endif	/* MCASP_TYPES_H */

