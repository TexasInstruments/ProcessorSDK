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
 * \file     cpsw_test_nway.c
 *
 * \brief    This file contains the CPSW auto-negotiation test implementation.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <ti/csl/csl_cpswitch.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/osal/osal.h>
#include <ti/board/board.h>

#include "enet_test_entry.h"
#include "enet_test_base.h"
#include "cpsw_test_nway.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

static void EnetTestNway_printLinkConfig(EnetMacPort_LinkCfg *linkCfg);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetTestNway_Run(EnetTestTaskObj *taskObj)
{
    EnetTestMacPortList_t enabledPorts;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    EnetPhy_GenericInArgs phyInArgs;
    EnetMacPort_LinkCfg phyOutArgs;
    EnetMacPort_LinkCfg macOutArgs;
    EnetMacPort_LinkCfg cpswOutArgs;
    Enet_IoctlPrms prms;
    Enet_MacPort portNum;
    int32_t phyStatus;
    int32_t macStatus = ENET_EFAIL;
    int32_t cpswStatus;
    int32_t status = ENET_SOK;

    EnetTestCommon_waitForPortLink(taskObj);
    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);

    /* Single port test, ignore others */
    portNum = enabledPorts.macPortList[0];
   EnetAppUtils_print("Port %u\n", portNum);

    /* Get port link state (speed & duplexity) from PHY */
    phyInArgs.macPort = (Enet_MacPort)portNum;
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &phyInArgs, &phyOutArgs);

    phyStatus = Enet_ioctl(stateObj->hEnet,
                           stateObj->coreId,
                           ENET_PHY_IOCTL_GET_LINK_MODE,
                           &prms);
    if (phyStatus != ENET_SOK)
    {
       EnetAppUtils_print("Failed to get link mode from PHY driver: %d\n", phyStatus);
        status = phyStatus;
    }

    EnetMacPort_GenericInArgs macInArgs;
    /* Get port link state (speed & duplexity) from MAC port */
    macInArgs.macPort = (Enet_MacPort)portNum;
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &macInArgs, &macOutArgs);

    macStatus = Enet_ioctl(stateObj->hEnet,
                           stateObj->coreId,
                           ENET_MACPORT_IOCTL_GET_LINK_CFG,
                           &prms);
    if (macStatus != ENET_SOK)
    {
       EnetAppUtils_print("Failed to get link mode from MAC port: %d\n", macStatus);
       status = macStatus;
    }

    /* Get port link state (speed & duplexity) from CPSW top level */
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &portNum, &cpswOutArgs);

    cpswStatus = Enet_ioctl(stateObj->hEnet,
                            stateObj->coreId,
                            ENET_PER_IOCTL_GET_PORT_LINK_CFG,
                            &prms);
    if (cpswStatus != ENET_SOK)
    {
       EnetAppUtils_print("Failed to get link mode from CPSW layer: %d\n", cpswStatus);
        status = cpswStatus;
    }

    /* Print all link configs read from different layers */
    if (phyStatus == ENET_SOK)
    {
       EnetAppUtils_print("PHY link config: ");
        EnetTestNway_printLinkConfig(&phyOutArgs);
    }

    if (macStatus == ENET_SOK)
    {
       EnetAppUtils_print("MAC link config: ");
       EnetTestNway_printLinkConfig(&macOutArgs);
    }

    if (cpswStatus == ENET_SOK)
    {
       EnetAppUtils_print("CPSW link config: ");
        EnetTestNway_printLinkConfig(&cpswOutArgs);
    }

    return status;
}

void EnetTestNway_updatePortLinkCfg1GFd(EnetPer_PortLinkCfg *pLinkArgs,
                                        Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg     = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    EnetPhy_Cfg *phyCfg         = &pLinkArgs->phyCfg;

    macCfg->loopbackEn = BFALSE;
    pLinkArgs->linkCfg.speed = ENET_SPEED_AUTO;
    pLinkArgs->linkCfg.duplexity = ENET_DUPLEX_AUTO;


    phyCfg->nwayCaps       = ENETPHY_LINK_CAP_FD1000;
}

void EnetTestNway_updatePortLinkCfg100MFd(EnetPer_PortLinkCfg *pLinkArgs,
                                          Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg     = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    EnetPhy_Cfg *phyCfg         = &pLinkArgs->phyCfg;

    macCfg->loopbackEn = BFALSE;
    pLinkArgs->linkCfg.speed = ENET_SPEED_AUTO;
    pLinkArgs->linkCfg.duplexity = ENET_DUPLEX_AUTO;

    phyCfg->nwayCaps      = ENETPHY_LINK_CAP_FD100;
}


void EnetTestNway_updatePortLinkCfg100MHd(EnetPer_PortLinkCfg *pLinkArgs,
                                          Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg     = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    EnetPhy_Cfg *phyCfg         = &pLinkArgs->phyCfg;

    macCfg->loopbackEn = BFALSE;
    pLinkArgs->linkCfg.speed = ENET_SPEED_AUTO;
    pLinkArgs->linkCfg.duplexity = ENET_DUPLEX_AUTO;

    phyCfg->nwayCaps      = ENETPHY_LINK_CAP_HD100;
}

static void EnetTestNway_printLinkConfig(EnetMacPort_LinkCfg *linkCfg)
{
    const char *speedStr;
    const char *duplexStr;
    bool invalid = BFALSE;

    switch (linkCfg->speed)
    {
        case ENET_SPEED_10MBIT:
            speedStr = "10-Mbps";
            break;

        case ENET_SPEED_100MBIT:
            speedStr = "100-Mbps";
            break;

        case ENET_SPEED_1GBIT:
            speedStr = "1-Gbps";
            break;

        default:
            speedStr = "invalid speed";
            invalid  = BTRUE;
            break;
    }

    switch (linkCfg->duplexity)
    {
        case ENET_DUPLEX_HALF:
            duplexStr = "half-duplex";
            break;

        case ENET_DUPLEX_FULL:
            duplexStr = "full-duplex";
            break;

        default:
            duplexStr = "invalid duplexity";
            invalid   = BTRUE;
            break;
    }

   EnetAppUtils_print("%s%s %s\n", speedStr, invalid ? ", " : "", duplexStr);
}
