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
 * \file     enet_switching_logic.c
 *
 * \brief    This file contains the enet_switching_logic test implementation.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <ti/drv/uart/UART_stdio.h>
#include <ti/csl/csl_cpswitch.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_ethutils.h>

#include <ti/osal/osal.h>

#include <ti/board/board.h>

#include "enet_test_base.h"
#include "cpsw_test_autolearn.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static int32_t EnetTest_lookupAleEntry(EnetTestTaskObj *taskObj,
                                       uint8_t macAddr[]);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test ALE Entry Address */
static uint8_t testAddr[ENET_MAC_ADDR_LEN] =
{
    0x40U, 0xcdU, 0xefU, 0xfeU, 0xdcU, 0xbaU
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetTestAutoLearn_Run(EnetTestTaskObj *taskObj)
{
    int32_t status;

    status = EnetTestCommon_Run(taskObj);
    if (status == ENET_SOK)
    {
        status = EnetTest_lookupAleEntry(taskObj, &testAddr[0U]);
    }

    return status;
}

static int32_t EnetTest_lookupAleEntry(EnetTestTaskObj *taskObj,
                                      uint8_t macAddr[])
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    CpswAle_MacAddrInfo macAddrInfo;
    CpswAle_GetUcastEntryOutArgs getUcastInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memcpy(&macAddrInfo.addr[0], macAddr, sizeof(macAddrInfo.addr));
    macAddrInfo.vlanId = 0U;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &macAddrInfo, &getUcastInArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_LOOKUP_UCAST,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("ALE auto learn test pass \n");
        status = ENET_SOK;
    }

    return status;
}

void EnetTestAutoLearn_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                         Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg      = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn           = BFALSE;
}

void EnetTestAutoLearn_setOpenPrms(EnetTestTaskObj *taskObj,
                                   Cpsw_Cfg *pCpswCfg,
                                   EnetUtils_Cfg *pUtilsPrms)
{
    pCpswCfg->aleCfg.portCfg[0].learningCfg.noLearn = BTRUE;
    pCpswCfg->aleCfg.portCfg[1].learningCfg.noLearn = BTRUE;
}
