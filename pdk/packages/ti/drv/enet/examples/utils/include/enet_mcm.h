/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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

/*!
 * \file     enet_mcm.h
 *
 * \brief    This file contains the structure definitions and function
 *           prototypes of the Multi-client manager.
 */

/*!
 * \addtogroup ENET_MCM_API
 * @{
 */

#ifndef ENET_MCM_H_
#define ENET_MCM_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/osal/osal.h>
#include <ti/osal/MailboxP.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/include/per/cpsw.h>
#include <ti/drv/enet/include/core/enet_dma.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */
typedef void (*EnetMcm_setPortLinkCfg)(EnetPer_PortLinkCfg *linkArgs,
                                       Enet_MacPort macPort);

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */
typedef struct EnetMcm_InitConfig_s
{
    Enet_Type enetType;

    uint32_t instId;

    void *perCfg;

    EnetMcm_setPortLinkCfg setPortLinkCfg;

    Enet_MacPort macPortList[ENET_MAC_PORT_NUM];

    uint8_t numMacPorts;

    uint32_t periodicTaskPeriod;

    Enet_Print print;

    /*! Trace timestamp provider */
    EnetTrace_TraceTsFunc traceTsFunc;

    /*! Extended trace function */
    EnetTrace_ExtTraceFunc extTraceFunc;
}EnetMcm_InitConfig;

typedef struct EnetMcm_HandleInfo_s
{
    Enet_Handle hEnet;

    Udma_DrvHandle hUdmaDrv;
} EnetMcm_HandleInfo;

typedef struct EnetMcm_CmdIf_s
{
    MailboxP_Handle hMboxCmd;
    MailboxP_Handle hMboxResponse;
}  EnetMcm_CmdIf;

/* ========================================================================== */
/*                         Global Variables Declarations                      */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t  EnetMcm_init(const EnetMcm_InitConfig *pMcmInitCfg);

void  EnetMcm_getCmdIf(Enet_Type enetType,
                       EnetMcm_CmdIf *hMcmCmdIf);

void  EnetMcm_releaseCmdIf(Enet_Type enetType,
                           EnetMcm_CmdIf *hMcmCmdIf);

void     EnetMcm_acquireHandleInfo(const EnetMcm_CmdIf *hMcmCmdIf,
                                   EnetMcm_HandleInfo *handleInfo);

void     EnetMcm_coreAttach(const EnetMcm_CmdIf *hMcmCmdIf,
                            uint32_t coreId,
                            EnetPer_AttachCoreOutArgs *attachInfo);

void     EnetMcm_coreDetach(const EnetMcm_CmdIf *hMcmCmdIf,
                            uint32_t coreId,
                            uint32_t coreKey);

int32_t EnetMcm_ioctl(const EnetMcm_CmdIf *hMcmCmdIf,
                      uint32_t cmd,
                      Enet_IoctlPrms *prms);

void EnetMcm_releaseHandleInfo(const EnetMcm_CmdIf *hMcmCmdIf);

void EnetMcm_deInit(Enet_Type enetType);

void EnetMcm_saveCtxt(const EnetMcm_CmdIf *hMcmCmdIf);

int32_t EnetMcm_restoreCtxt(const EnetMcm_CmdIf *hMcmCmdIf);

int32_t EnetMcm_closeMacPorts(const EnetMcm_CmdIf *hMcmCmdIf);

int32_t EnetMcm_openMacPorts(const EnetMcm_CmdIf *hMcmCmdIf);

void EnetMcm_stopPeriodicTick(const EnetMcm_CmdIf *hMcmCmdIf);

void EnetMcm_startPeriodicTick(const EnetMcm_CmdIf *hMcmCmdIf);

#ifdef __cplusplus
}
#endif

#endif /* ENET_MCM_H_ */

/*! @} */
