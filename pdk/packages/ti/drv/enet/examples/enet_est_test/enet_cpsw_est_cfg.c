/*
 *  Copyright (c) Texas Instruments Incorporated 2023
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
 * \file  enet_cpsw_est_cfg.c
 *
 * \brief This file contains the implementation of the APIs for peripheral
 *        configuration for the CPSW EST example app.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "enet_cpsw_est_common.h"
#include "enet_cpsw_est_cfg.h"
#include "enet_cpsw_est_dataflow.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* First admin will have AdminBaseTime = CurrentTime + ENET_APP_EST_ADMIN_LIST_DELAY */
#define ENET_APP_EST_ADMIN_LIST_DELAY             (2000000000ULL)

/* Maximum number of times to check the oper list update status */
#define ENET_APP_OPER_LIST_UPDATE_CHECK_RETRY_MAX (10U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void EnetApp_initLinkArgs(EnetPer_PortLinkCfg *portLinkCfg,
                          Enet_MacPort macPort);

static int32_t EnetApp_waitForLinkUp(Enet_MacPort macPort);

static uint64_t EnetApp_getTestAdminBaseTime(Enet_MacPort macPort,
                                             EnetTas_TasState state);

static int32_t EnetApp_setupEst(Enet_MacPort macPort,
                                EnetTas_ControlList *controlList);

void EnetApp_updateCpswInitCfg(Enet_Type enetType, uint32_t instId, Cpsw_Cfg *cpswCfg);

int32_t EnetApp_driverOpen(Enet_Type enetType, uint32_t instId);

int32_t EnetApp_setupCpswAle(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void EnetApp_portLinkStatusChangeCb(Enet_MacPort macPort,
                                           bool isLinkUp,
                                           void *appArg)
{
    EnetAppUtils_print("MAC Port %u: link %s\r\n",
                       ENET_MACPORT_ID(macPort), isLinkUp ? "up" : "down");
}

static void EnetApp_mdioLinkStatusChange(Cpsw_MdioLinkStateChangeInfo *info,
                                         void *appArg)
{
    static uint32_t linkUpCount = 0U;

    if (info->linkChanged)
    {
        if (info->isLinked)
        {
            linkUpCount++;
        }
        else
        {
            gEnetApp.usingDfltSched = BFALSE;
        }
    }
    ENET_UNUSED(linkUpCount);
}

void EnetApp_updateCpswInitCfg(Enet_Type enetType, uint32_t instId, Cpsw_Cfg *cpswCfg)
{
    CpswAle_Cfg *aleCfg = &cpswCfg->aleCfg;
    CpswHostPort_Cfg *hostPortCfg = &cpswCfg->hostPortCfg;
    CpswCpts_Cfg *cptsCfg = &cpswCfg->cptsCfg;

    /* Peripheral-level config */

    cpswCfg->mdioLinkStateChangeCb     = EnetApp_mdioLinkStatusChange;
    cpswCfg->mdioLinkStateChangeCbArg  = &gEnetApp;


    cpswCfg->portLinkStatusChangeCb    = &EnetApp_portLinkStatusChangeCb;
    cpswCfg->portLinkStatusChangeCbArg = &gEnetApp;
    cpswCfg->vlanCfg.vlanAware         = BTRUE;

    /* ALE config */
    aleCfg->modeFlags                          = CPSW_ALE_CFG_MODULE_EN;
    aleCfg->agingCfg.autoAgingEn               = BTRUE;
    aleCfg->agingCfg.agingPeriodInMs           = 1000U;
    aleCfg->nwSecCfg.vid0ModeEn                = BTRUE;
    aleCfg->vlanCfg.aleVlanAwareMode           = BTRUE;
    aleCfg->vlanCfg.cpswVlanAwareMode          = cpswCfg->vlanCfg.vlanAware;
    aleCfg->vlanCfg.unknownUnregMcastFloodMask = CPSW_ALE_ALL_PORTS_MASK;
    aleCfg->vlanCfg.unknownRegMcastFloodMask   = CPSW_ALE_ALL_PORTS_MASK;
    aleCfg->vlanCfg.unknownVlanMemberListMask  = CPSW_ALE_ALL_PORTS_MASK;
    aleCfg->policerGlobalCfg.policingEn        = BFALSE;

    /* Host port config */
    hostPortCfg->removeCrc         = BTRUE;
    hostPortCfg->padShortPacket    = BTRUE;
    hostPortCfg->passCrcErrors     = BFALSE;
    /* Hardware switch priority is taken from packet's PCP or DSCP */
    hostPortCfg->rxVlanRemapEn     = BTRUE;
    hostPortCfg->rxDscpIPv4RemapEn = BTRUE;
    hostPortCfg->rxDscpIPv6RemapEn = BTRUE;

    /* CPTS config (CPTS_RFT_CLK = 200/500MHz) */
    cptsCfg->cptsRftClkFreq = CPSW_CPTS_RFTCLK_FREQ_500MHZ;
}

int32_t EnetApp_open(void)
{
    Enet_MacPort macPort;
    EnetTas_ControlList *controlList;
    uint32_t i;
    int32_t status = ENET_SOK;

    /* Do peripheral dependent initalization */
    EnetAppUtils_enableClocks(gEnetApp.enetType, gEnetApp.instId);

    status = EnetBoard_setupPorts(gEnetApp.enetType,
                                  gEnetApp.instId,
                                  &gEnetApp.ethPorts[0U],
                                  gEnetApp.macPortNum);

    EnetAppUtils_assert(status == ENET_SOK);

    /* Create RX task */
    EnetApp_createRxTask();

    status = EnetApp_driverOpen(gEnetApp.enetType, gEnetApp.instId);

    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to open ENET driver: %d\r\n", status);
    }

    /* Attach the core with RM */
    if (status == ENET_SOK)
    {
        Enet_IoctlPrms prms;
        EnetPer_AttachCoreOutArgs attachCoreOutArgs;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &gEnetApp.coreId, &attachCoreOutArgs);
        status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_PER_IOCTL_ATTACH_CORE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("EnetApp_MainTask failed ENET_PER_IOCTL_ATTACH_CORE: %d\n", status);
        }
        else
        {
            gEnetApp.coreKey = attachCoreOutArgs.coreKey;
        }
    }

    if (status == ENET_SOK)
    {
        /* memutils open should happen after Cpsw is opened as it uses CpswUtils_Q
         * functions */
        status = EnetMem_init();
        EnetAppUtils_assert(ENET_SOK == status);
    }

    /* Open DMA for peripheral/port */
    if (status == ENET_SOK)
    {
        status = EnetApp_openDma();
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to open DMA: %d\r\n", status);
        }
    }

    /* Enable host port */
    if (status == ENET_SOK)
    {
        status = EnetApp_setupCpswAle();
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to setup CPSW ALE: %d\n", status);
        }

        if (status == ENET_SOK)
        {
            Enet_IoctlPrms prms;

            ENET_IOCTL_SET_NO_ARGS(&prms);
            status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_HOSTPORT_IOCTL_ENABLE, &prms);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("Failed to enable host port: %d\n", status);
            }
        }
    }

    ClockP_start(gEnetApp.hTickTimer);

    /* Wait for PHY link up */
    if (status == ENET_SOK)
    {
        for (i = 0U; i < gEnetApp.macPortNum; i++)
        {
            macPort = gEnetApp.ethPorts[i].macPort;
            controlList = &gEnetApp.tasControlList[i];

            if(gEnetApp.macPort == macPort)
            {
                status = EnetApp_waitForLinkUp(macPort);
                if (status != ENET_SOK)
                {
                    EnetAppUtils_print("Port %u: Failed to wait for link up: %d\r\n", ENET_MACPORT_ID(macPort), status);
                    break;
                }

                /* Configure EST */
                status = EnetApp_setupEst(macPort, controlList);
                if (status != ENET_SOK)
                {
                    EnetAppUtils_print("Port %u: Failed to setup EST: %d\r\n", ENET_MACPORT_ID(macPort), status);
                    break;
                }
            }
        }

        gEnetApp.usingDfltSched = (status == ENET_SOK);
    }

    /* Print our address */
    if (status == ENET_SOK)
    {
        EnetAppUtils_print("MAC addr: ");
        EnetAppUtils_printMacAddr(&gEnetApp.macAddr[0U]);
    }

    return status;
}

void EnetApp_close(void)
{
    int32_t status = ENET_SOK;

    /* Disable host port */
    if (status == ENET_SOK)
    {
        Enet_IoctlPrms prms;

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_HOSTPORT_IOCTL_DISABLE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to disable host port: %d\n", status);
        }
    }

    /* Stop periodic tick timer */
    ClockP_stop(gEnetApp.hTickTimer);

    /* Close DMA */
    EnetApp_closeDma();

    /* Close port link */
    if (status == ENET_SOK)
    {
        Enet_IoctlPrms prms;

        ENET_IOCTL_SET_IN_ARGS(&prms, &gEnetApp.macPort);
        status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_PER_IOCTL_CLOSE_PORT_LINK, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to close port link: %d\n", status);
        }
    }

    /* Delete RX tasks created for all peripherals */
    EnetApp_destroyRxTask();

    /*Detach Core*/
    Enet_IoctlPrms prms;
    ENET_IOCTL_SET_IN_ARGS(&prms, &gEnetApp.coreKey);

    status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_PER_IOCTL_DETACH_CORE, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to detach core key %u: %d\n", gEnetApp.coreKey, status);
    }

    /* Close UDMA */
    EnetAppUtils_udmaclose(gEnetApp.hMainUdmaDrv);

    gEnetApp.hEnet = NULL;

    /* Do peripheral dependent deinitalization */
    EnetAppUtils_disableClocks(gEnetApp.enetType, gEnetApp.instId);
}

void EnetApp_initLinkArgs(EnetPer_PortLinkCfg *linkArgs,
                          Enet_MacPort macPort)
{
    EnetPhy_Cfg *phyCfg = &linkArgs->phyCfg;
    EnetMacPort_LinkCfg *linkCfg = &linkArgs->linkCfg;
    EnetMacPort_Interface *mii = &linkArgs->mii;
    const EnetBoard_EthPort *ethPort = NULL;
    const EnetBoard_PortCfg *portCfg;
    CpswMacPort_Cfg macCfg;
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;
    uint32_t i;

    /* Setup board for requested Ethernet port */
    for (i = 0U; i < gEnetApp.macPortNum; i++)
    {
        if (gEnetApp.ethPorts[i].macPort == macPort)
        {
            ethPort = &gEnetApp.ethPorts[i];
        }
    }

    if (ethPort == NULL)
    {
        EnetAppUtils_print("MAC port %u is not enabled in the test\n", ENET_MACPORT_ID(macPort));
        EnetAppUtils_assert(BFALSE);
    }

    linkArgs->macCfg = &macCfg;
    CpswMacPort_initCfg(&macCfg);

    portCfg = EnetBoard_getPortCfg(gEnetApp.enetType, gEnetApp.instId, ethPort);
    if (portCfg != NULL)
    {
        EnetPhy_initCfg(phyCfg);
        phyCfg->phyAddr     = portCfg->phyCfg.phyAddr;
        phyCfg->isStrapped  = portCfg->phyCfg.isStrapped;
        phyCfg->loopbackEn  = BFALSE;
        phyCfg->skipExtendedCfg = portCfg->phyCfg.skipExtendedCfg;
        phyCfg->extendedCfgSize = portCfg->phyCfg.extendedCfgSize;
        memcpy(phyCfg->extendedCfg, portCfg->phyCfg.extendedCfg, phyCfg->extendedCfgSize);

        *linkCfg = portCfg->linkCfg;
    }
    else
    {
        EnetAppUtils_print("No port configuration found for MAC port %u\n",
                           ENET_MACPORT_ID(macPort));
        EnetAppUtils_assert(BFALSE);
    }

    mii->layerType     = ethPort->mii.layerType;
    mii->sublayerType  = ethPort->mii.sublayerType;
    mii->variantType   = ENET_MAC_VARIANT_FORCED;


    macCfg.sgmiiMode = portCfg->sgmiiMode;

     /* Open port link */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, linkArgs);

        status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_PER_IOCTL_OPEN_PORT_LINK, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to open port link: %d\n", status);
        }
    }

}

static int32_t EnetApp_waitForLinkUp(Enet_MacPort macPort)
{
    Enet_IoctlPrms prms;
    bool linked = BFALSE;
    int32_t status = ENET_SOK;

    EnetAppUtils_print("Port %u: Waiting for link up...\r\n", ENET_MACPORT_ID(macPort));

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &macPort, &linked);

    while (gEnetApp.run && !linked)
    {
        status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_PER_IOCTL_IS_PORT_LINK_UP, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to get link status: %d\r\n", status);
            linked = BFALSE;
            break;
        }

        if (!linked)
        {
        	EnetUtils_delay(10U);
        }
    }

    if (gEnetApp.run && (status == ENET_SOK))
    {
        EnetAppUtils_print("Port %u: Link is %s\r\n", ENET_MACPORT_ID(macPort), linked ? "up" : "down");
    }

    return status;
}

void EnetApp_printStats(void)
{
    Enet_IoctlPrms prms;
    Enet_MacPort macPort;
    uint32_t i;
    int32_t status;

    /* Get host port statistics */
    EnetAppUtils_print("\n   Host Port statistics\r\n");
    EnetAppUtils_print("--------------------------------\r\n");

    ENET_IOCTL_SET_OUT_ARGS(&prms, &gEnetApp_cpswStats);
    status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_STATS_IOCTL_GET_HOSTPORT_STATS, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to get host port stats\r\n");
    }
    else
    {
        EnetAppUtils_printHostPortStats9G((CpswStats_HostPort_Ng *)&gEnetApp_cpswStats);
        EnetAppUtils_print("\n");
    }

    /* Get MAC ports statistics */
    for (i = 0U; i < gEnetApp.macPortNum; i++)
    {
        macPort = gEnetApp.ethPorts[i].macPort;

        EnetAppUtils_print("   MAC Port %u statistics\r\n", ENET_MACPORT_ID(macPort));
        EnetAppUtils_print("--------------------------------\r\n");

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &macPort, &gEnetApp_cpswStats);
        status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_STATS_IOCTL_GET_MACPORT_STATS, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to get MAC port %u stats\r\n", ENET_MACPORT_ID(macPort));
            continue;
        }

        EnetAppUtils_printMacPortStats9G((CpswStats_MacPort_Ng *)&gEnetApp_cpswStats);
        EnetAppUtils_print("\n");
    }
}

void EnetApp_resetStats(void)
{
    Enet_IoctlPrms prms;
    Enet_MacPort macPort;
    uint32_t i;
    int32_t status;

    ENET_IOCTL_SET_NO_ARGS(&prms);
    status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_STATS_IOCTL_RESET_HOSTPORT_STATS, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to reset host port stats\r\n");
    }

    for (i = 0U; i < gEnetApp.macPortNum; i++)
    {
        macPort = gEnetApp.ethPorts[i].macPort;

        if(gEnetApp.macPort == macPort)
        {
            ENET_IOCTL_SET_IN_ARGS(&prms, &macPort);
            status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_STATS_IOCTL_RESET_MACPORT_STATS, &prms);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("Failed to reset MAC port %u stats\r\n", ENET_MACPORT_ID(macPort));
                continue;
            }
        }
    }
}

static void EnetApp_printEstList(EnetTas_ControlList *list)
{
    uint8_t gateMask = 0U;
    uint32_t start = 0U;
    uint32_t end;
    uint32_t dur;
    uint32_t i;

    for (i = 0U; i < list->listLength; i++)
    {
        gateMask = list->gateCmdList[i].gateStateMask;
        dur = list->gateCmdList[i].timeInterval;
        end = start + dur - 1U;

        /* o = Gate open, C = Gate closed */
        EnetAppUtils_print("Gate mask=%s%s%s%s%s%s%s%s (0x%02x), start=%u ns, end=%u ns, dur=%u ns\r\n",
                           ENET_IS_BIT_SET(gateMask, 7U) ? "o" : "C",
                           ENET_IS_BIT_SET(gateMask, 6U) ? "o" : "C",
                           ENET_IS_BIT_SET(gateMask, 5U) ? "o" : "C",
                           ENET_IS_BIT_SET(gateMask, 4U) ? "o" : "C",
                           ENET_IS_BIT_SET(gateMask, 3U) ? "o" : "C",
                           ENET_IS_BIT_SET(gateMask, 2U) ? "o" : "C",
                           ENET_IS_BIT_SET(gateMask, 1U) ? "o" : "C",
                           ENET_IS_BIT_SET(gateMask, 0U) ? "o" : "C",
                           gateMask,
                           start, end, dur);

        start += dur;
    }

    EnetAppUtils_print("Cycle time=%llu ns\r\n", list->cycleTime);
    EnetAppUtils_print("Base time=%llu ns\r\n", list->baseTime);
}

static void EnetApp_showAdminList(Enet_MacPort macPort)
{
    Enet_IoctlPrms prms;
    EnetTas_GenericInArgs inArgs;
    EnetTas_ControlList adminList;
    int32_t status;

    inArgs.macPort = macPort;
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &adminList);
    status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_TAS_IOCTL_GET_ADMIN_LIST, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to get TAS admin list: %d\r\n", status);
    }
    else
    {
        EnetAppUtils_print("\r\nMAC %u: Admin List\r\n", ENET_MACPORT_ID(macPort));
        EnetAppUtils_print("-------------------------------------------\r\n");
        EnetApp_printEstList(&adminList);
    }
}

static void EnetApp_showOperList(Enet_MacPort macPort)
{
    Enet_IoctlPrms prms;
    EnetTas_GenericInArgs inArgs;
    EnetTas_ControlList operList;
    int32_t status;

    inArgs.macPort = macPort;
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &operList);
    status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_TAS_IOCTL_GET_OPER_LIST, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to get TAS oper list: %d\r\n", status);
    }
    else
    {
        EnetAppUtils_print("\r\nMAC %u: Oper List\r\n", ENET_MACPORT_ID(macPort));
        EnetAppUtils_print("-------------------------------------------\r\n");
        EnetApp_printEstList(&operList);
    }
}

static uint64_t EnetApp_getTestAdminBaseTime(Enet_MacPort macPort,
                                             EnetTas_TasState state)
{
    uint64_t tsVal;
    uint32_t i;

    /* If EST is not enabled (reset or disabled), CPSW driver allows setting an
     * admin basetime in the future.
     * If EST is already enabled, CPSW driver only allows setting an admin time
     * in the past, so setting it to 0. */
    if ((state == ENET_TAS_DISABLE) ||
        (state == ENET_TAS_RESET))
    {
        tsVal = EnetApp_getCurrentTime() + ENET_APP_EST_ADMIN_LIST_DELAY;
        EnetAppUtils_print("MAC Port %u: suggested start time: %llu\r\n", ENET_MACPORT_ID(macPort), tsVal);

        /* Save the ESTF start time in order to compute the normalized timestamp of EST
         * events at a later point.  This is needed solely for test verification purpose */
        for (i = 0U; i < gEnetApp.macPortNum; i++)
        {
            if (gEnetApp.ethPorts[i].macPort == macPort)
            {
                gEnetApp.estfStartTime[i] = tsVal;
            }
        }
    }
    else
    {
        tsVal = 0ULL;
    }

    return tsVal;
}

int32_t EnetApp_setAdminList(Enet_MacPort macPort,
                             EnetTas_ControlList *controlList)
{
    Enet_IoctlPrms prms;
    EnetTas_SetAdminListInArgs adminListInArgs;
    EnetTas_TasState state;
    EnetTas_OperStatus operStatus = ENET_TAS_OPER_LIST_NOT_YET_UPDATED;
    uint32_t i;
    int32_t status = ENET_SOK;

    /* Get the current EST state */
    state = EnetApp_getEstState(macPort);

    /* Setup new TAS admin list */
    adminListInArgs.macPort = macPort;
    memcpy(&adminListInArgs.adminList, controlList, sizeof(EnetTas_ControlList));
    /* Update baseTime to future time if EST is not enabled */
    if (controlList->baseTime == 0ULL)
    {
        adminListInArgs.adminList.baseTime = EnetApp_getTestAdminBaseTime(macPort, state);
        controlList->baseTime = adminListInArgs.adminList.baseTime;
    }
    ENET_IOCTL_SET_IN_ARGS(&prms, &adminListInArgs);

    status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_TAS_IOCTL_SET_ADMIN_LIST, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to set TAS admin list: %d\r\n", status);
    }

    /* Print admin list */
    if (status == ENET_SOK)
    {
        EnetApp_showAdminList(macPort);
    }

    /* Wait until the operational list is updated */
    if (status == ENET_SOK)
    {
        for (i = 0U; i < ENET_APP_OPER_LIST_UPDATE_CHECK_RETRY_MAX; i++)
        {
            ENET_IOCTL_SET_INOUT_ARGS(&prms, &macPort, &operStatus);

            status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_TAS_IOCTL_GET_OPER_LIST_STATUS, &prms);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("Failed to check TAS operational list update status: %d\r\n", status);
                break;
            }

            if (operStatus == ENET_TAS_OPER_LIST_UPDATED)
            {
                break;
            }
        }
    }

    return status;
}

int32_t EnetApp_setEstState(Enet_MacPort macPort,
                            EnetTas_TasState state)
{
    Enet_IoctlPrms prms;
    EnetTas_SetStateInArgs setStateInArgs;
    int32_t status;

    /* Set TAS state to requested state */
    setStateInArgs.macPort = macPort;
    setStateInArgs.state   = state;
    ENET_IOCTL_SET_IN_ARGS(&prms, &setStateInArgs);

    status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_TAS_IOCTL_SET_STATE, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to set TAS state %u: %d\r\n", (uint32_t)state, status);
    }
    else
    {
        EnetAppUtils_print("TAS state is set to %u\r\n", (uint32_t)state);
    }

    return status;
}

EnetTas_TasState EnetApp_getEstState(Enet_MacPort macPort)
{
    Enet_IoctlPrms prms;
    EnetTas_GenericInArgs stateInArgs;
    EnetTas_TasState state = ENET_TAS_RESET;
    int32_t status;

    /* Get the current EST state */
    stateInArgs.macPort = macPort;
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &stateInArgs, &state);

    status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, ENET_TAS_IOCTL_GET_STATE, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to get TAS state: %d\r\n", status);
    }

    return state;
}

void EnetApp_printEstStatus(Enet_MacPort macPort)
{
    Enet_IoctlPrms prms;
    EnetMacPort_GenericInArgs inArgs;
    CpswMacPort_EstStatus estStatus;
    int32_t status;

    /* Get EST status */
    inArgs.macPort = macPort;
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &estStatus);
    status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, CPSW_MACPORT_IOCTL_EST_GET_STATUS, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to get EST status\r\n");
    }
    else
    {
        EnetAppUtils_print("\nMAC Port %u EST status\r\n", ENET_MACPORT_ID(macPort));
        EnetAppUtils_print("-------------------------\r\n");
        EnetAppUtils_print("Active buffer     : %s\n", (estStatus.activeBuffer == 1U) ? "Upper" : "Lower");
        EnetAppUtils_print("Address error     : %s\n", (estStatus.addrErr == 1U) ? "Yes" : "No");
        EnetAppUtils_print("Fetch count error : %s\n", (estStatus.fetchCntErr == 1U) ? "Yes" : "No");
        EnetAppUtils_print("TX MAC allow      : 0x%02x\n", estStatus.macAllow);
        EnetAppUtils_print("TX priority active: 0x%02x\n", estStatus.txPriorityActive);
    }
}

static int32_t EnetApp_setupEst(Enet_MacPort macPort,
                                EnetTas_ControlList *controlList)
{
    int32_t status;

    /* Set TAS state to 'RESET' */
    status = EnetApp_setEstState(macPort, ENET_TAS_RESET);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to set TAS state to 'RESET': %d\r\n", status);
    }

    /* Setup TAS admin list */
    if (status == ENET_SOK)
    {
        status = EnetApp_setAdminList(macPort, controlList);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to set admin list: %d\r\n", status);
        }
    }

    /* Set TAS state to 'ENABLE' */
    if (status == ENET_SOK)
    {
        status = EnetApp_setEstState(macPort, ENET_TAS_ENABLE);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to set TAS state to 'ENABLE': %d\r\n", status);
        }
    }

    /* Print operational list */
    if (status == ENET_SOK)
    {
        EnetApp_showOperList(macPort);
    }

    return status;
}

int32_t EnetApp_driverOpen(Enet_Type enetType, uint32_t instId)
{
    EnetUtils_Cfg utilsCfg;
    Cpsw_Cfg cpswCfg;
    EnetDma_Cfg dmaCfg;
    int32_t status = ENET_SOK;
    EnetPer_PortLinkCfg linkArgs;
    uint32_t i;

    /* Initialize Enet driver (use default utils) */
    Enet_initUtilsCfg(&utilsCfg);
    Enet_init(&utilsCfg);

    cpswCfg.dmaCfg = &dmaCfg;

    /* Set initial config */
    Enet_initCfg(gEnetApp.enetType, gEnetApp.instId, &cpswCfg, sizeof(cpswCfg));

    EnetApp_updateCpswInitCfg(gEnetApp.enetType, gEnetApp.instId, &cpswCfg);

    EnetAppUtils_initResourceConfig(gEnetApp.enetType, gEnetApp.instId, gEnetApp.coreId, &cpswCfg.resCfg);

    /* Open Enet driver */
    if (gEnetApp.enetType == ENET_CPSW_9G)
    {
        EnetAppUtils_print("CPSW_9G Test on MAIN NAVSS\n");
    }
    else if (gEnetApp.enetType == ENET_CPSW_5G)
    {
        EnetAppUtils_print("CPSW_5G Test on MAIN NAVSS\n");
    }
    else if (gEnetApp.enetType == ENET_CPSW_2G)
    {
        if (gEnetApp.instId == 0)
        {
            EnetAppUtils_print("CPSW_2G Test on MCU NAVSS\n");
        }
        else if (gEnetApp.instId == 1)
        {
            EnetAppUtils_print("CPSW_2G Test on MAIN NAVSS\n");
        }
    }

    dmaCfg.rxChInitPrms.dmaPriority = UDMA_DEFAULT_RX_CH_DMA_PRIORITY;

    /* App should open UDMA first as UDMA handle is needed to initialize
     * CPSW RX channel */
    gEnetApp.hMainUdmaDrv = EnetAppUtils_udmaOpen(gEnetApp.enetType, NULL);
    EnetAppUtils_assert(NULL != gEnetApp.hMainUdmaDrv);

    dmaCfg.hUdmaDrv = gEnetApp.hMainUdmaDrv;

    /* Set Enet global runtime log level */
    Enet_setTraceLevel(ENET_TRACE_DEBUG);

    /* Open the Enet driver */
    gEnetApp.hEnet = Enet_open(gEnetApp.enetType, gEnetApp.instId, &cpswCfg, sizeof(cpswCfg));
    if (gEnetApp.hEnet == NULL)
    {
        EnetAppUtils_print("Failed to open Enet driver\n");
        status = ENET_EFAIL;
    }


    for (i = 0U; i < gEnetApp.macPortNum; i++)
    {
        if(gEnetApp.macPort == gEnetApp.ethPorts[i].macPort)
        {
            linkArgs.macPort = gEnetApp.ethPorts[i].macPort;
            EnetApp_initLinkArgs(&linkArgs, gEnetApp.ethPorts[i].macPort);
        }
    }

    return status;
}


void EnetApp_timerCallback(void* arg)
{
    SemaphoreP_Handle hSem = (SemaphoreP_Handle)arg;

    /* Tick! */
    SemaphoreP_post(hSem);
}


void EnetApp_createClock(void)
{
    TaskP_Params taskParams;
    SemaphoreP_Params semParams;
    ClockP_Params clkParams;

    /* Initialize timer semaphore params */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_COUNTING;

    /* Create timer semaphore */
    gEnetApp.hTimerSem = SemaphoreP_create(0, &semParams);

    /* Reset the exitFlag */
    gEnetApp.exitFlag = BFALSE;

    /* Initialize the periodic tick task params */
    TaskP_Params_init(&taskParams);
    taskParams.priority       = 7U;
    taskParams.stack          = gEnetAppTaskStackTick;
    taskParams.stacksize      = sizeof(gEnetAppTaskStackTick);
    taskParams.arg0           = (void*)gEnetApp.hTimerSem;
    taskParams.name           = (const char *)"Periodic tick task";

    /* Create periodic tick task */
    gEnetApp.hTickTask = TaskP_create(&EnetApp_tickTask, &taskParams);
    if (gEnetApp.hTickTask == NULL)
    {
        EnetAppUtils_print("EnetLpbk_createClock() failed to create tick task\n");
        OS_stop();
    }

    ClockP_Params_init(&clkParams);
    clkParams.startMode = ClockP_StartMode_USER;
    clkParams.period    = ENETAPP_PERIODIC_TICK_MS;
    clkParams.runMode   = ClockP_RunMode_CONTINUOUS;
    clkParams.arg       = (void*)gEnetApp.hTimerSem;

    /* Creating timer and setting timer callback function*/
    gEnetApp.hTickTimer = ClockP_create(EnetApp_timerCallback, &clkParams);
    if (gEnetApp.hTickTimer == NULL)
    {
        EnetAppUtils_print("EnetLpbk_createClock() failed to create clock\n");
        OS_stop();
    }
}

void EnetApp_tickTask(void* a0,
                      void* a1)
{
    SemaphoreP_Handle hSem = (SemaphoreP_Handle)a0;

    while (!gEnetApp.exitFlag)
    {
        SemaphoreP_pend(hSem, SemaphoreP_WAIT_FOREVER);

        /* PeriodicTick should be called from non-ISR context */
        Enet_periodicTick(gEnetApp.hEnet);
    }
    EnetAppUtils_print("EnetLpbk_tickTask() exiting..\n");
}


void EnetApp_deleteClock(void)
{
	gEnetApp.exitFlag = BTRUE;

    /* Delete periodic tick timer */
    if (gEnetApp.hTickTimer != NULL)
    {
        ClockP_delete(gEnetApp.hTickTimer);
        gEnetApp.hTickTimer = NULL;
    }

    /* Delete periodic tick task */
    if (gEnetApp.hTickTask != NULL)
    {
#if !defined (FREERTOS)
        TaskP_delete(&gEnetApp.hTickTask);
#endif
        gEnetApp.hTickTask = NULL;
    }

    /* Delete periodic tick timer */
    if (gEnetApp.hTimerSem != NULL)
    {
        SemaphoreP_delete(gEnetApp.hTimerSem);
        gEnetApp.hTimerSem = NULL;
    }
}

int32_t EnetApp_setupCpswAle(void)
{
    Enet_IoctlPrms prms;
    CpswAle_SetPortStateInArgs setPortStateInArgs;
    CpswAle_SetUcastEntryInArgs setUcastInArgs;
    uint32_t entryIdx;
    int32_t status;

    /* ALE entry with "secure" bit cleared is required for loopback */
    setUcastInArgs.addr.vlanId  = 0U;
    setUcastInArgs.info.portNum = CPSW_ALE_HOST_PORT_NUM;
    setUcastInArgs.info.blocked = BFALSE;
    setUcastInArgs.info.secure  = BFALSE;
    setUcastInArgs.info.super   = BFALSE;
    setUcastInArgs.info.ageable = BFALSE;
    setUcastInArgs.info.trunk   = BFALSE;
    EnetUtils_copyMacAddr(&setUcastInArgs.addr.addr[0U], gEnetApp.macAddr);
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &entryIdx);

    status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, CPSW_ALE_IOCTL_ADD_UCAST, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to add ucast entry: %d\n", status);
    }

    /* Set host port to 'forwarding' state */
    if (status == ENET_SOK)
    {
        setPortStateInArgs.portNum   = CPSW_ALE_HOST_PORT_NUM;
        setPortStateInArgs.portState = CPSW_ALE_PORTSTATE_FORWARD;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setPortStateInArgs);

        status = Enet_ioctl(gEnetApp.hEnet, gEnetApp.coreId, CPSW_ALE_IOCTL_SET_PORT_STATE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to set ALE port state: %d\n", status);
        }
    }

    return status;
}
