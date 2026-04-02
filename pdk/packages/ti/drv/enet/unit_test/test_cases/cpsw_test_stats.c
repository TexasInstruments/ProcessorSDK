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
 * \file     cpsw_test_stats.c
 *
 * \brief    This file contains the cpsw stats test implementation.
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

#include "enet_test_entry.h"
#include "enet_test_base.h"
#include "cpsw_test_stats.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

static void EnetTestStats_testStatsShowStats(EnetTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetTestStats_PostProcessRxPkt(EnetTestTaskObj *taskObj,
                                       uint32_t rxCfgIndex)
{
    /* Print CPSW statistics of all ports */
    EnetTestStats_testStatsShowStats(taskObj);
    return ENET_SOK;
}

static void EnetTestStats_testStatsShowStats(EnetTestTaskObj *taskObj)
{
    Enet_IoctlPrms prms;
    Enet_MacPort inArgs;
    CpswStats_PortStats portStats;
    CpswStats_HostPort_2g *hostPortStats = (CpswStats_HostPort_2g *)&portStats;
    CpswStats_MacPort_2g *macPortStats = (CpswStats_MacPort_2g *)&portStats;
    int32_t status = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    ENET_IOCTL_SET_OUT_ARGS(&prms, &portStats);
    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_STATS_IOCTL_GET_HOSTPORT_STATS, &prms);
    if (status == ENET_SOK)
    {
       EnetAppUtils_print("\n Port 0 Statistics\n");
       EnetAppUtils_print("-----------------------------------------\n");
       EnetAppUtils_printHostPortStats2G(hostPortStats);
       EnetAppUtils_print("\n");

       EnetAppUtils_print("netOctets check: %s (got %llu)\n",
                           (hostPortStats->netOctets <
                            0xFFFFFFFFLLU) ? "FAILED" : "PASSED",
                           hostPortStats->netOctets);
    }
    else
    {
       EnetAppUtils_print("EnetTest_showStats() failed to get host stats: %d\n",
                           status);
    }

    if (status == ENET_SOK)
    {
        EnetTestMacPortList_t enabledPorts;

        EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
        /* Take any port from enabled ports */
        inArgs = enabledPorts.macPortList[0];
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, macPortStats);
        status =
            Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_STATS_IOCTL_GET_MACPORT_STATS,
                       &prms);
        if (status == ENET_SOK)
        {
           EnetAppUtils_print("\n Port 1 Statistics\n");
           EnetAppUtils_print("-----------------------------------------\n");
           EnetAppUtils_printMacPortStats2G(macPortStats);
           EnetAppUtils_print("\n");

           EnetAppUtils_print("netOctets check: %s (got %llu)\n",
                               (macPortStats->netOctets <
                                0xFFFFFFFFLLU) ? "FAILED" : "PASSED",
                               macPortStats->netOctets);
        }
        else
        {
           EnetAppUtils_print("%s: failed to get MAC stats: %d\n",  __func__,
                               status);
        }
    }
}

void EnetTestStats_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                     Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg     = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn          = BTRUE;
    pLinkArgs->mii.variantType  = ENET_MAC_VARIANT_FORCED;
}
