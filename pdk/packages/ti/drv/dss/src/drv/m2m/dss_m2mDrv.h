/*
 *  Copyright (c) Texas Instruments Incorporated 2021
 *  All rights reserved.
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
 */

/**
 *  \file dss_m2mDrv.h
 *
 *  \brief DSS m2m driver internal interface file.
 */

#ifndef DSS_M2MDRV_H_
#define DSS_M2MDRV_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/drv/dss/src/drv/m2m/dss_m2mPriv.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/**
 *  \brief DSS M2M Driver initialization parameters.
 */
typedef struct
{
    uint32_t drvInstId;
    /**< Driver Instance Id */
} Dss_M2MDrvInitParams;

/* ========================================================================== */
/*                  Internal/Private Function Declarations                   */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 *  \brief DSS display driver initialization function.
 *   This function gets called as a part of Dss_init(). It Initializes internal
 *   data structures and common system level resources related to display.
 *
 *  \param initParams     Pointer to a #Dss_InitParams structure
 *                        containing the driver init configuration
 *
 *  \return FVID2_SOK if successful, else suitable error code
 */
int32_t Dss_m2mDrvInit(const Dss_InitParams *initParams);

/**
 *  \brief DSS display driver de-initialization function.
 *   This function gets called as a part of Dss_deInit(). It de-Initializes
 *   internal data structures and common system level resources
 *   related to display.
 *
 *  \param None
 *
 *  \return FVID2_SOK if successful, else suitable error code
 */
int32_t Dss_m2mDrvDeInit(void);

/*
 * FVID2 DSS M2M Driver API's
 */
Fdrv_Handle Dss_m2mCreate(uint32_t drvId,
                          uint32_t instId,
                          void *createArgs,
                          void *createStatusArgs,
                          const Fvid2_DrvCbParams *fdmCbParams);

int32_t Dss_m2mDelete(Fdrv_Handle handle, void *reserved);

int32_t Dss_m2mProcessRequest(Fdrv_Handle handle,
                              Fvid2_FrameList *inProcessList,
                              Fvid2_FrameList *outProcessList,
                              uint32_t timeout);

int32_t Dss_m2mGetProcessedRequest(Fdrv_Handle handle,
                                   Fvid2_FrameList *inProcessList,
                                   Fvid2_FrameList *outProcessList,
                                   uint32_t timeout);

int32_t Dss_m2mControl(Fdrv_Handle handle,
                       uint32_t cmd,
                       void *cmdArgs,
                       void *cmdStatusArgs);
/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSS_M2MDRV_H_ */
