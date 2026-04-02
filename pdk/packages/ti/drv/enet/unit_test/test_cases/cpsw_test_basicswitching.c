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
#include "enet_test_entry.h"
#include "cpsw_test_basicswitching.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
#define ENET_TEST_SWITCHING_DEFAULT_SHORTIPG_THRESHOLD                    (11)

static int32_t EnetTestSwitching_setShortIPG(EnetTestTaskObj *taskObj)
{
    Enet_IoctlPrms prms;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    Cpsw_SetTxShortIpgCfgInArgs setShortIPGInArgs;
    EnetTestMacPortList_t enabledPorts;
    int32_t status;

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);

   EnetAppUtils_print("Enabling IPG Config - Ingress port %u, Egress port %u \n",
                       enabledPorts.macPortList[0U],
                       enabledPorts.macPortList[1U]);

    ENET_IOCTL_SET_IN_ARGS(&prms, &setShortIPGInArgs);
    setShortIPGInArgs.configureGapThresh                               = BTRUE;
    setShortIPGInArgs.ipgTriggerThreshBlkCnt                           = 0U;
    setShortIPGInArgs.numMacPorts                                      = 2U;
    setShortIPGInArgs.portShortIpgCfg[0].macPort                       = enabledPorts.macPortList[0U];
    setShortIPGInArgs.portShortIpgCfg[0].shortIpgCfg.txShortGapEn      = BTRUE;
    setShortIPGInArgs.portShortIpgCfg[0].shortIpgCfg.txShortGapLimitEn = BFALSE;

    setShortIPGInArgs.portShortIpgCfg[1].macPort                           = enabledPorts.macPortList[1U];
    setShortIPGInArgs.portShortIpgCfg[1].shortIpgCfg.txShortGapEn      = BTRUE;
    setShortIPGInArgs.portShortIpgCfg[1].shortIpgCfg.txShortGapLimitEn = BFALSE;

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_PER_IOCTL_SET_SHORT_IPG_CFG,
                        &prms);
    if (ENET_SOK == status)
    {
        Cpsw_TxShortIpgCfg getShortIPGOutArgs;

        ENET_IOCTL_SET_OUT_ARGS(&prms, &getShortIPGOutArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_PER_IOCTL_GET_SHORT_IPG_CFG,
                            &prms);

        if (ENET_SOK == status)
        {
            CpswMacPort_PortTxShortIpgCfg *ipgCfg;
            uint32_t i;

            EnetAppUtils_assert(getShortIPGOutArgs.ipgTriggerThreshBlkCnt == 0U);

            for (i = 0U; i < getShortIPGOutArgs.numMacPorts; i++)
            {
                ipgCfg = &getShortIPGOutArgs.portShortIpgCfg[i];
                if ((ipgCfg->macPort == enabledPorts.macPortList[0U]) ||
                    (ipgCfg->macPort == enabledPorts.macPortList[1U]))
                {
                    EnetAppUtils_assert(ipgCfg->shortIpgCfg.txShortGapEn == BTRUE);
                    EnetAppUtils_assert(ipgCfg->shortIpgCfg.txShortGapLimitEn == BFALSE);
                }
            }
        }
    }

    return status;
}

int32_t EnetTestSwitching_RunWithIPG(EnetTestTaskObj *taskObj)
{
    int32_t status;

    EnetTestSwitching_setShortIPG(taskObj);

    status = EnetTestCommon_Run(taskObj);
    return status;
}

int32_t EnetTestSwitching_Run(EnetTestTaskObj *taskObj)
{
    int32_t status;

    status = EnetTestCommon_Run(taskObj);

    return status;
}
