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
 * \file     cpsw_test_base.c
 *
 * \brief    This file contains the common code test implementation.
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
#include <ti/drv/enet/include/core/enet_dma.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_apprm.h>
#include <ti/drv/enet/examples/utils/include/enet_ethutils.h>

#include <ti/osal/osal.h>

#include <ti/board/board.h>

#include "enet_test_entry.h"
#include "enet_test_base.h"
#include "enet_test_board.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ENET_TEST_COMMON_TX_TASK_STACK_SIZE (10U * 1024U)
#define ENET_TEST_COMMON_TX_TASK_PRI (7)
#define ENET_TEST_COMMON_TX_TEST_STATUS_COUNT  (4)

#define ENET_TEST_COMMON_RX_TASK_STACK_SIZE (10U * 1024U)
#define ENET_TEST_COMMON_RX_TASK_PRI (8)
#define ENET_TEST_COMMON_RX_TEST_STATUS_COUNT  (4)

#define ENET_TEST_COMMON_MAIN_TASK_PRI (9)
#define ENET_TEST_COMMON_PHYLINK_STATE_POLL_INTERVAL_MS   (1U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */
typedef struct EnetTestCommon_TxTestStatus_s
{
    uint32_t testStatus;
    uint32_t stackUsed;
} EnetTestCommon_TxTestStatus_t;

typedef struct EnetTestCommon_RxTestStatus_s
{
    uint32_t testStatus;
    uint32_t stackUsed;
} EnetTestCommon_RxTestStatus_t;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

static void EnetTestCommon_InitUdmaDrv(EnetTestTaskObj *taskObj);

static void EnetTestCommon_DeInitUdmaDrv(EnetTestTaskObj *taskObj);

static void     EnetTestCommon_showStats(EnetTestTaskObj *taskObj);

static int32_t EnetTestCommon_openCpsw(EnetTestTaskObj *taskObj,
                                       bool *pDisableHostAddrSecureFlag);

static void     EnetTestCommon_closeCpsw(EnetTestTaskObj *taskObj);

static int32_t EnetTestCommon_openDma(EnetTestTaskObj *taskObj,
                                      bool disableHostAddrSecureFlag);

static void     EnetTestCommon_closeDma(EnetTestTaskObj *taskObj);

static int32_t EnetTestCommon_setRxflowPrms(EnetUdma_OpenRxFlowPrms *pRxFlowPrms,
                                            EnetTestTaskObj *taskObj,
                                            uint32_t rxFlowCfgIdx);
static void EnetTestCommon_recvTask(void *a0, void *a1);
static void  EnetTestCommon_createTxSem(EnetTestTaskObj *taskObj,
                                        uint32_t txCfgIndex);

static uint32_t EnetTestCommon_waitForXmit(EnetTestTaskObj *taskObj,
                                           uint32_t txCfgIndex);

static void EnetTestCommon_deleteRxSem(EnetTestTaskObj *taskObj,
                                       uint32_t rxCfgIndex);

static void EnetTestCommon_deleteTxSem(EnetTestTaskObj *taskObj,
                                       uint32_t txCfgIndex);

static void EnetTestCommon_setPortLinkPrms(EnetPer_PortLinkCfg *linkArgs,
                                           Enet_MacPort portNum,
                                           EnetTestTaskObj *taskObj);

static int32_t EnetTestCommon_setTxChPrms(EnetUdma_OpenTxChPrms *pTxChPrms,
                                   EnetTestTaskObj *taskObj,
                                   uint32_t txPSILThreadId,
                                   uint32_t txChCfgIdx);

static int32_t EnetTestCommon_addDefaultHostPortEntry(EnetTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
static uint8_t gDefaultHostMacAddr[] = {0x02, 0x01, 0x02, 0x03, 0x04, 0x5};

static uint8_t gTxMbxBuf[sizeof(EnetTestCommon_TxTestStatus_t) * ENET_TEST_COMMON_TX_TEST_STATUS_COUNT] __attribute__ ((aligned(32)));
static uint8_t gRxMbxBuf[sizeof(EnetTestCommon_RxTestStatus_t) * ENET_TEST_COMMON_RX_TEST_STATUS_COUNT] __attribute__ ((aligned(32)));

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* PDK-4761 - TheEnetAppUtils_enableClocks /EnetAppUtils_disableClocks sequence causes
 * core to lock up on accessing CPSW register after clock enable when executed
 * mutliple iterations.
 * To avoid CPU lockup disabling clock disable/enable during each test execution
 * TODO: Fix theEnetAppUtils_enableClocks/EnetAppUtils_disableClocks and remove
 * the workaround
 */
bool gDisableClockDisable = BTRUE;

int32_t EnetTestCommon_Init(EnetTestTaskObj *taskObj)
{
    int32_t status;
    EnetTestStateObj *stateObj     = &taskObj->stateObj;
    static bool clockInitDone      = BFALSE;
    bool disableHostAddrSecureFlag = BFALSE;

    /* App should open UDMA first as UDMA handle is needed to initialize
     * CPSW RX channel */
    EnetTestCommon_InitUdmaDrv(taskObj);

    if ((gDisableClockDisable == BFALSE) || (!clockInitDone))
    {
       EnetAppUtils_enableClocks(taskObj->taskCfg->enetType, taskObj->taskCfg->instId);
        clockInitDone = BTRUE;
    }

    /* Open the CPSW */
    status = EnetTestCommon_openCpsw(taskObj, &disableHostAddrSecureFlag);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("Failed to open CPSW: %d\n", status);
    }

    /* Open legacy DMA driver */
    if (status == ENET_SOK)
    {
        status = EnetTestCommon_openDma(taskObj, disableHostAddrSecureFlag);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("Failed to open legacy DMA: %d\n", status);
        }
    }

    /* Enable host port */
    if (status == ENET_SOK)
    {
        Enet_IoctlPrms prms;
        CpswAle_SetPortStateInArgs setPortStateInArgs;

        setPortStateInArgs.portNum   = CPSW_ALE_HOST_PORT_NUM;
        setPortStateInArgs.portState = CPSW_ALE_PORTSTATE_FORWARD;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setPortStateInArgs);
        prms.outArgs = NULL;
        status       = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_PORT_STATE, &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestCommon_openCpsw() failed CPSW_ALE_IOCTL_SET_PORT_STATE: %d\n", status);
        }

        if (status == ENET_SOK)
        {
            ENET_IOCTL_SET_NO_ARGS(&prms);
            status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_HOSTPORT_IOCTL_ENABLE, &prms);
            if (status != ENET_SOK)
            {
               EnetAppUtils_print("Failed to enable host port: %d\n", status);
            }
        }
    }

    /* Show alive PHYs */
    if (status == ENET_SOK)
    {
        Enet_IoctlPrms prms;
        bool alive;
        int8_t i;

        for (i = 0U; i < MDIO_MAX_PHY_CNT; i++)
        {
            ENET_IOCTL_SET_INOUT_ARGS(&prms, &i, &alive);
            status = Enet_ioctl(stateObj->hEnet, stateObj->coreId,
                                ENET_MDIO_IOCTL_IS_ALIVE,
                                &prms);
            if (status == ENET_SOK)
            {
                if (alive == BTRUE)
                {
                   EnetAppUtils_print("PHY %d is alive\n", i);
                }
            }
            else
            {
               EnetAppUtils_print("Failed to get PHY %d alive status: %d\n", i,
                                   status);
            }
        }
    }

    if (status == ENET_SOK)
    {
        status = EnetTestCommon_addDefaultHostPortEntry(taskObj);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("Failed to add default host port address: %d\n", status);
        }
    }

    return status;
}

void  EnetTestCommon_getMacPortList(uint32_t macPortBitMask,
                                    EnetTestMacPortList_t *macPortList)
{
    Enet_MacPort portIndex;
    uint32_t numMacPorts = 0U;

    for (portIndex = ENET_MAC_PORT_FIRST; portIndex <= ENET_MAC_PORT_NUM; portIndex++)
    {
        if (ENET_BIT(ENET_MACPORT_NORM(portIndex)) & macPortBitMask)
        {
           EnetAppUtils_assert(numMacPorts < ENET_ARRAYSIZE(macPortList->macPortList));
            macPortList->macPortList[numMacPorts] = portIndex;
            numMacPorts++;
        }
    }

    macPortList->numMacPorts = numMacPorts;
}

static void EnetTestCommon_waitForPhyLinkUp(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint32_t phyPortMask       = taskObj->taskCfg->macPortMask;

    phyPortMask &= ~(stateObj->noPhyPortMask);
    EnetAppUtils_print("Waiting for PHY link on enabled ports mask:0x%X \n", phyPortMask);
    EnetAppUtils_assert(stateObj->phyEvent != NULL);

    /* Wait on event only if phyPortMask is non-zero */
    if (phyPortMask)
    {
        EventP_wait(stateObj->phyEvent, phyPortMask, EventP_WaitMode_ALL, EventP_WAIT_FOREVER);
    }
}

void EnetTestCommon_waitForPortLink(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    EnetTestCommon_waitForPhyLinkUp(taskObj);

    EnetAppUtils_assert(stateObj->portEvent != NULL);
    EnetAppUtils_print("Waiting for PORT link on enabled ports \n");
    EventP_wait(stateObj->portEvent, taskObj->taskCfg->macPortMask, EventP_WaitMode_ALL, EventP_WAIT_FOREVER);
}

int32_t EnetTestCommon_Run(EnetTestTaskObj *taskObj)
{
    int32_t status;

    EnetTestCommon_waitForPortLink(taskObj);
    status = EnetTestCommon_pktTxRx(taskObj);

    return status;
}

int32_t EnetTestCommon_DeInit(EnetTestTaskObj *taskObj)
{
    int32_t status;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    Enet_IoctlPrms prms;
    uint32_t i;

    /* Print CPSW statistics of all ports */
    EnetTestCommon_showStats(taskObj);

    /* DeInit Portion */
    /* Disable host port */
    ENET_IOCTL_SET_NO_ARGS(&prms);
    status = Enet_ioctl(stateObj->hEnet,
                        stateObj->coreId,
                        ENET_HOSTPORT_IOCTL_DISABLE,
                        &prms);
    /* Host port disable should not fail */
    EnetAppUtils_assert(status == ENET_SOK);

    /* Close legacy DMA driver */
    EnetTestCommon_closeDma(taskObj);

    /* Close CPSW */
    EnetTestCommon_closeCpsw(taskObj);

    /* Delete the Tx and Rx semaphore */
    for (i = 0U; i < taskObj->taskCfg->numTxCh; i++)
    {
        EnetTestCommon_deleteTxSem(taskObj, i);
    }

    /* Delete the Tx and Rx semaphore */
    for (i = 0U; i < taskObj->taskCfg->numRxFlow; i++)
    {
        EnetTestCommon_deleteRxSem(taskObj, i);
    }

    EnetTestCommon_DeInitUdmaDrv(taskObj);

    if (!gDisableClockDisable)
    {
       EnetAppUtils_disableClocks(taskObj->taskCfg->enetType, taskObj->taskCfg->instId);
    }

    return ENET_SOK;
}

void EnetTestCommon_initAlePrms(CpswAle_Cfg *aleCfg,
                                EnetTestTaskObj *taskObj)
{
    aleCfg->modeFlags =
        (CPSW_ALE_CFG_MODULE_EN);
    aleCfg->agingCfg.autoAgingEn               = BTRUE;
    aleCfg->agingCfg.agingPeriodInMs           = 1000;
    aleCfg->nwSecCfg.vid0ModeEn                = BTRUE;
    aleCfg->vlanCfg.aleVlanAwareMode           = BFALSE;
    aleCfg->vlanCfg.cpswVlanAwareMode          = BFALSE;
    aleCfg->vlanCfg.unknownUnregMcastFloodMask = CPSW_ALE_ALL_PORTS_MASK;
    aleCfg->vlanCfg.unknownRegMcastFloodMask   = CPSW_ALE_ALL_PORTS_MASK;
    aleCfg->vlanCfg.unknownVlanMemberListMask  = CPSW_ALE_ALL_PORTS_MASK;
}

void EnetTestCommon_initDmaPrms(EnetDma_Cfg *dmaCfg,
                                EnetTestTaskObj *taskObj)
{
    dmaCfg->rxChInitPrms.dmaPriority = UDMA_DEFAULT_RX_CH_DMA_PRIORITY;

    dmaCfg->hUdmaDrv = taskObj->stateObj.hUdmaDrv;
}

static void EnetTestCommon_initHostPortPrms(CpswHostPort_Cfg *hostPrms)
{
    hostPrms->removeCrc      = BTRUE;
    hostPrms->padShortPacket = BTRUE;
    hostPrms->passCrcErrors  = BTRUE;
}

static void EnetTestCommon_initCpswVlanPrms(Cpsw_VlanCfg *vlanPrms)
{
    vlanPrms->vlanAware = BFALSE;
}

static void EnetTestCommon_initCptsPrms(CpswCpts_Cfg *cptsPrms)
{
    cptsPrms->hostRxTsEn = BFALSE;
}

static Enet_MacPort EnetTestCommon_mapPhyAddr2MacPort(EnetTestTaskObj *taskObj,
                                                      uint32_t matchPhyAddr)
{
    EnetTestMacPortList_t enabledPorts;
    uint32_t i;

    /* Initialized to avoid warning from GCC about uninitialized variable.
     * However if matching port is not found it will assert and abort execution
     * rather than return MAC_PORT_0
     */
    Enet_MacPort matchPort = ENET_MAC_PORT_1;

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
    for (i = 0U; i < enabledPorts.numMacPorts; i++)
    {
        uint32_t portPhyAddr = EnetTestBoard_getPhyAddr(taskObj->taskCfg->enetType,
                                                        enabledPorts.macPortList[i]);
        if (portPhyAddr == matchPhyAddr)
        {
            break;
        }
    }

    if (i < enabledPorts.numMacPorts)
    {
        matchPort = enabledPorts.macPortList[i];
    }
    else
    {
       EnetAppUtils_assert(BFALSE);
    }

    return matchPort;
}

static uint32_t EnetTestCommon_getEnablePhyMask(EnetTestTaskObj *taskObj)
{
    EnetTestMacPortList_t enabledPorts;
    uint32_t i;
    uint32_t enabledPhyMask;

    enabledPhyMask = 0U;
    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
    for (i = 0U; i < enabledPorts.numMacPorts; i++)
    {
        uint32_t portPhyAddr = EnetTestBoard_getPhyAddr(taskObj->taskCfg->enetType,
                                                        enabledPorts.macPortList[i]);
        enabledPhyMask |= ENET_MDIO_PHY_ADDR_MASK(portPhyAddr);
    }

    return enabledPhyMask;
}

static void EnetTestCommon_mdioLinkStatusChangeCb(Cpsw_MdioLinkStateChangeInfo *info,
                                                  void *appArg)
{
    EnetTestTaskObj *taskObj = (EnetTestTaskObj *)appArg;
    Enet_MacPort linkStateChangePort;

    linkStateChangePort = EnetTestCommon_mapPhyAddr2MacPort(taskObj, info->phyAddr);

    if ((info->linkChanged) && (info->isLinked))
    {
       EnetAppUtils_assert(taskObj->stateObj.phyEvent != NULL);
        EventP_post(taskObj->stateObj.phyEvent, ENET_BIT(ENET_MACPORT_NORM(linkStateChangePort)));
    }
}

static void EnetTestCommon_portLinkStatusChangeCb(Enet_MacPort portNum,
                                                  bool isLinkUp,
                                                  void *appArg)
{
    EnetTestTaskObj *taskObj = (EnetTestTaskObj *)appArg;

    EnetAppUtils_print("MAC Port %u: link %s\n", portNum, isLinkUp ? "up" : "down");

    if (isLinkUp)
    {
       EnetAppUtils_assert(taskObj->stateObj.portEvent != NULL);
        EventP_post(taskObj->stateObj.portEvent, ENET_BIT(ENET_MACPORT_NORM(portNum)));
    }
}

static void EnetTestCommon_initCpswLinkCbPrms(Cpsw_Cfg *cpswCfg,
                                              EnetTestTaskObj *taskObj)
{
    cpswCfg->mdioLinkStateChangeCb     = &EnetTestCommon_mdioLinkStatusChangeCb;
    cpswCfg->mdioLinkStateChangeCbArg  = taskObj;
    cpswCfg->portLinkStatusChangeCb    = &EnetTestCommon_portLinkStatusChangeCb;
    cpswCfg->portLinkStatusChangeCbArg = taskObj;
}

static void EnetTestCommon_initMdioPrms(Mdio_Cfg *mdioCfg,
                                        EnetTestTaskObj *taskObj)
{
    if (taskObj->taskCfg->enetType == ENET_CPSW_2G)
    {
        mdioCfg->mode = MDIO_MODE_NORMAL;
    }
    else if ((taskObj->taskCfg->enetType == ENET_CPSW_9G)||
             (taskObj->taskCfg->enetType == ENET_CPSW_5G))
    {
        mdioCfg->mode = MDIO_MODE_STATE_CHANGE_MON;
    }
    else
    {
       EnetAppUtils_assert(BFALSE);
    }

    mdioCfg->pollEnMask = EnetTestCommon_getEnablePhyMask(taskObj);
}

static void EnetTestCommon_setPortLinkPrms(EnetPer_PortLinkCfg *linkArgs,
                                           Enet_MacPort portNum,
                                           EnetTestTaskObj *taskObj)
{
    CpswMacPort_Cfg *macCfg = linkArgs->macCfg;
    EnetMacPort_LinkCfg *linkCfg = &linkArgs->linkCfg;
    EnetPhy_Cfg *phyCfg          = &linkArgs->phyCfg;
    EnetMacPort_Interface *interface   = &linkArgs->mii;

    CpswMacPort_initCfg(macCfg);

    EnetPhy_initCfg(phyCfg);

    interface->layerType = ENET_MAC_LAYER_GMII;

    EnetTestBoard_setPhyConfig(taskObj, portNum, macCfg, interface, phyCfg);
    macCfg->loopbackEn   = BFALSE;
    linkArgs->macPort    = portNum;
    linkCfg->speed       = ENET_SPEED_AUTO;
    linkCfg->duplexity   = ENET_DUPLEX_AUTO;
}

static int32_t EnetTestCommon_openCpsw(EnetTestTaskObj *taskObj,
                                       bool *pDisableHostAddrSecureFlag)
{
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;
    uint32_t index;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    Cpsw_Cfg cpswCfg;
    EnetUtils_Cfg utilsPrms;
    EnetPer_PortLinkCfg linkArgs;
    CpswMacPort_Cfg macCfg;
    EnetDma_Cfg dmaCfg;

    *pDisableHostAddrSecureFlag = BFALSE;
    /* Set configuration parameters */
    Enet_initCfg(taskObj->taskCfg->enetType, 0U, &cpswCfg, sizeof(cpswCfg));
    EnetTestCommon_initCpswLinkCbPrms(&cpswCfg, taskObj);
    EnetTestCommon_initCpswVlanPrms(&cpswCfg.vlanCfg);

    EnetAppUtils_initResourceConfig(taskObj->taskCfg->enetType,
                                    taskObj->taskCfg->instId,
                                    taskObj->stateObj.coreId,
                                    &cpswCfg.resCfg);

    EnetTestCommon_initMdioPrms(&cpswCfg.mdioCfg, taskObj);
    EnetTestCommon_initHostPortPrms(&cpswCfg.hostPortCfg);
    EnetTestCommon_initAlePrms(&cpswCfg.aleCfg, taskObj);
    EnetTestCommon_initDmaPrms(&dmaCfg, taskObj);
    cpswCfg.dmaCfg = &dmaCfg;
    EnetTestCommon_initCptsPrms(&cpswCfg.cptsCfg);

    if (ENET_CPSW_9G == taskObj->taskCfg->enetType)
    {
       EnetAppUtils_print("ENET_CPSW_9G Test on MAIN NAVSS\n");
    }
    else if (ENET_CPSW_5G == taskObj->taskCfg->enetType)
    {
       EnetAppUtils_print("ENET_CPSW_5G Test on MAIN NAVSS\n");
    }
    else if (ENET_CPSW_2G == taskObj->taskCfg->enetType)
    {
       EnetAppUtils_print("ENET_CPSW_2G Test on MCU NAVSS\n");
    }

    Enet_initUtilsCfg(&utilsPrms);
    /* Initialize CPSW driver with default OSAL, utils */
    utilsPrms.print     = EnetAppUtils_print;
    Enet_init(&utilsPrms);

    /* Open the CPSW driver */
    if (taskObj->taskCfg->openPrmsUpdate != NULL)
    {
        taskObj->taskCfg->openPrmsUpdate(taskObj, &cpswCfg, &utilsPrms);
    }

    stateObj->hEnet = Enet_open(taskObj->taskCfg->enetType, taskObj->taskCfg->instId, &cpswCfg, sizeof(cpswCfg));
    if (stateObj->hEnet == NULL)
    {
       EnetAppUtils_print("EnetTestCommon_openCpsw() failed to open: %d\n",
                           status);
        status = ENET_EFAIL;
    }

    if (status == ENET_SOK)
    {
        /* memutils open should happen after Cpsw is opened as it uses CpswUtils_Q
         * functions */
        EnetMem_init();
    }

    if (status == ENET_SOK)
    {
        EnetTestMacPortList_t enabledPorts;
        EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
        for (index = 0U; index < enabledPorts.numMacPorts; index++)
        {
            linkArgs.macCfg = &macCfg;
            EnetTestCommon_setPortLinkPrms(&linkArgs,
                                           enabledPorts.macPortList[index],
                                           taskObj);
            if (taskObj->taskCfg->portLinkPrmsUpdate != NULL)
            {
                /* Override test specific port link init params */
                taskObj->taskCfg->portLinkPrmsUpdate(&linkArgs,
                                                     enabledPorts.macPortList[index]);
            }

            ENET_IOCTL_SET_IN_ARGS(&prms, &linkArgs);
            status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_PER_IOCTL_OPEN_PORT_LINK,
                                &prms);
            if (status != ENET_SOK)
            {
               EnetAppUtils_print("EnetTestCommon_openCpsw() failed to open MAC port: %d\n", status);
                break;
            }

            CpswMacPort_Cfg *cfg = (CpswMacPort_Cfg *)linkArgs.macCfg;
            if (cfg->loopbackEn)
            {
                *pDisableHostAddrSecureFlag = BTRUE;
            }

            if (ENETPHY_INVALID_PHYADDR == linkArgs.phyCfg.phyAddr)
            {
                /* If PHY is disabled for the port set the noPhyMask */
                taskObj->stateObj.noPhyPortMask |= ENET_BIT(ENET_MACPORT_NORM(linkArgs.macPort));
            }
        }
    }

    return status;
}

static void EnetTestCommon_closeCpsw(EnetTestTaskObj *taskObj)
{
    Enet_IoctlPrms prms;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint32_t index;
    int32_t status;
    EnetTestMacPortList_t enabledPorts;

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);

    for (index = 0; index < enabledPorts.numMacPorts; index++)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, &enabledPorts.macPortList[index]);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_PER_IOCTL_CLOSE_PORT_LINK, &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("close() failed to close MAC port: %d\n", status);
            /* Cannot handle port close failing */
           EnetAppUtils_assert(BFALSE);
        }
    }

    Enet_close(stateObj->hEnet);

    Enet_deinit();
}

void EnetTestCommon_showStats(EnetTestTaskObj *taskObj)
{
    Enet_IoctlPrms prms;
    Enet_MacPort inArgs;
    CpswStats_PortStats portStats;
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    ENET_IOCTL_SET_OUT_ARGS(&prms, &portStats);
    status =
        Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_STATS_IOCTL_GET_HOSTPORT_STATS,
                   &prms);
    if (status == ENET_SOK)
    {
       EnetAppUtils_print("\n Port 0 Statistics\n");
       EnetAppUtils_print("-----------------------------------------\n");
        switch (taskObj->taskCfg->enetType)
        {
            case ENET_CPSW_2G:
            {
                CpswStats_HostPort_2g *st;

                st = (CpswStats_HostPort_2g *)&portStats;
                EnetAppUtils_printHostPortStats2G(st);
                break;
            }

            case ENET_CPSW_9G:
            case ENET_CPSW_5G:
            {
                CpswStats_HostPort_Ng *st;

                st = (CpswStats_HostPort_Ng *)&portStats;
                EnetAppUtils_printHostPortStats9G(st);
                break;
            }

            default:
            {
                EnetAppUtils_print("Invalid ENET type\n");
                EnetAppUtils_assert(BFALSE);
            }
        }

       EnetAppUtils_print("\n");
    }
    else
    {
       EnetAppUtils_print("EnetTestCommon_showStats() failed to get host stats: %d\n",
                           status);
    }

    if (status == ENET_SOK)
    {
        uint32_t index;
        EnetTestMacPortList_t enabledPorts;

        EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
        for (index = 0U; index < enabledPorts.numMacPorts; index++)
        {
            inArgs = enabledPorts.macPortList[index];
            ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &portStats);
            status =
                Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_STATS_IOCTL_GET_MACPORT_STATS,
                           &prms);
            if (status == ENET_SOK)
            {
               EnetAppUtils_print("\n External Port %d Statistics\n", ENET_MACPORT_NORM(enabledPorts.macPortList[index]));
               EnetAppUtils_print("-----------------------------------------\n");
                switch (taskObj->taskCfg->enetType)
                {
                    case ENET_CPSW_2G:
                    {
                        CpswStats_MacPort_2g *st;

                        st = (CpswStats_MacPort_2g *)&portStats;
                       EnetAppUtils_printMacPortStats2G(st);
                        break;
                    }

                    case ENET_CPSW_5G:
                    case ENET_CPSW_9G:
                    {
                        CpswStats_MacPort_Ng *st;

                        st = (CpswStats_MacPort_Ng *)&portStats;
                        EnetAppUtils_printMacPortStats9G(st);
                        break;
                    }

                    default:
                    {
                        EnetAppUtils_print("Invalid ENET type\n");
                        EnetAppUtils_assert(BFALSE);
                    }

                }

               EnetAppUtils_print("\n");
            }
            else
            {
               EnetAppUtils_print("EnetTestCommon_showStats() failed to get MAC stats: %d\n",
                                   status);
            }
        }
    }
}

void EnetTestCommon_rxIsrFxn(void *appData)
{
    /* Post semaphore to tx handling task */
    EnetTestRxFlowObj *pRxFlowObj = (EnetTestRxFlowObj *)appData;

    SemaphoreP_post(pRxFlowObj->hRxCQSem);
}

void EnetTestCommon_txIsrFxn(void *appData)
{
    /* Post semaphore to tx handling task */
    EnetTestTxChObj *pTxChObj = (EnetTestTxChObj *)appData;

    SemaphoreP_post(pTxChObj->hTxCQSem);
}

void EnetTestCommon_initTxFreePktQ(EnetTestTaskObj *taskObj,
                                   uint32_t txChCfgIdx)
{
    EnetDma_Pkt *pktInfo;
    uint32_t index;
    EnetTestStateObj *stateObj       = &taskObj->stateObj;
    EnetTestTxChObj *txChObj         = &stateObj->txChObj[txChCfgIdx];
    EnetTestTxChCfgInfo *txChCfgInfo = stateObj->txChCfgInfo[txChCfgIdx];
    uint32_t maxPktsPerCh;
    uint32_t scatterSegments[1];

    /* Delibrately allocate more packets than number of descriptors to
     * exercise unused queue logic when submitting packets
     */
    maxPktsPerCh = (ENET_MEM_NUM_TX_PKTS * ENET_CFG_TX_CHANNELS_NUM) / taskObj->taskCfg->numTxCh;

    /* Initialize all queues */
    EnetQueue_initQ(&txChObj->txFreePktInfoQ);
    scatterSegments[0] = txChCfgInfo->maxTxPktLen;

    /* Initialize TX EthPkts and queue them to txFreePktInfoQ */
    for (index = 0U; index < EnetUtils_min(txChCfgInfo->pktSendCount, maxPktsPerCh); index++)
    {
        pktInfo = EnetMem_allocEthPkt(NULL, ENETDMA_CACHELINE_ALIGNMENT, ENET_ARRAYSIZE(scatterSegments), scatterSegments);
        if (NULL == pktInfo)
        {
            break;
        }

        memset(&pktInfo->node, 0U, sizeof(pktInfo->node));
        pktInfo->sgList.list[0].segmentAllocLen  = txChCfgInfo->maxTxPktLen;
        pktInfo->sgList.list[0].segmentFilledLen = txChCfgInfo->maxTxPktLen;
        pktInfo->appPriv    = &txChCfgInfo;

        EnetQueue_enq(&txChObj->txFreePktInfoQ, &pktInfo->node);
    }

    EnetAppUtils_assert(EnetQueue_getQCount(&txChObj->txFreePktInfoQ) == EnetUtils_min(txChCfgInfo->pktSendCount, maxPktsPerCh));
    EnetAppUtils_print("initQs() txFreePktInfoQ initialized with %d pkts\n", EnetQueue_getQCount(&txChObj->txFreePktInfoQ));
}

static int32_t EnetTestCommon_setTxChPrms(EnetUdma_OpenTxChPrms *pTxChPrms,
                                          EnetTestTaskObj *taskObj,
                                          uint32_t txPSILThreadId,
                                          uint32_t txChCfgIdx)
{
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    EnetAppUtils_setCommonTxChPrms(pTxChPrms);

    pTxChPrms->chNum = txPSILThreadId;

    pTxChPrms->notifyCb = &EnetTestCommon_txIsrFxn;
    pTxChPrms->cbArg   = &stateObj->txChObj[txChCfgIdx];

    pTxChPrms->hUdmaDrv = taskObj->stateObj.hUdmaDrv;

    return status;
}

static int32_t EnetTestCommon_setRxflowPrms(EnetUdma_OpenRxFlowPrms *pRxFlowPrms,
                                            EnetTestTaskObj *taskObj,
                                            uint32_t rxFlowCfgIdx)
{
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    EnetAppUtils_setCommonRxFlowPrms(pRxFlowPrms);

    pRxFlowPrms->notifyCb = &EnetTestCommon_rxIsrFxn;
    pRxFlowPrms->cbArg   = &stateObj->rxFlowObj[rxFlowCfgIdx];

    pRxFlowPrms->hUdmaDrv  = stateObj->hUdmaDrv;
    pRxFlowPrms->rxFlowMtu =
    ENET_UTILS_ALIGN((stateObj->rxFlowCfgInfo[rxFlowCfgIdx]->rxFlowMtu
                      + sizeof(EthVlanFrameHeader)), ENET_UDMA_RXMTU_ALIGN);

    return status;
}

void EnetTestCommon_initRxReadyPktQ(EnetTestTaskObj *taskObj,
                                    uint32_t rxFlowCfgIdx)
{
    EnetDma_PktQ rxReadyQ;
    int32_t status;
    uint32_t i;
    EnetDma_Pkt *pPktInfo;
    EnetTestStateObj *stateObj           = &taskObj->stateObj;
    EnetTestRxFlowObj *rxFlowObj         = &stateObj->rxFlowObj[rxFlowCfgIdx];
    EnetTestRxFlowCfgInfo *rxFlowCfgInfo = stateObj->rxFlowCfgInfo[rxFlowCfgIdx];

    uint32_t maxPkts;
    uint32_t scatterSegments[1];

    /* Delibrately allocate more packets than number of descriptors to
     * exercise unused queue logic when submitting packets
     */
    maxPkts = (ENET_MEM_NUM_RX_PKTS * ENET_CFG_RX_FLOWS_NUM) / taskObj->taskCfg->numRxFlow;

    EnetQueue_initQ(&rxFlowObj->rxFreeQ);
    EnetQueue_initQ(&rxReadyQ);
    scatterSegments[0] = rxFlowCfgInfo->rxFlowMtu;

    for (i = 0U; i < EnetUtils_min(rxFlowCfgInfo->pktRecvCount, maxPkts); i++)
    {
        pPktInfo = EnetMem_allocEthPkt(&stateObj,
                                       ENETDMA_CACHELINE_ALIGNMENT,
                                       ENET_ARRAYSIZE(scatterSegments),
                                       scatterSegments);
        if (pPktInfo == NULL)
        {
            break;
        }

        EnetQueue_enq(&rxFlowObj->rxFreeQ, &pPktInfo->node);
    }

    EnetAppUtils_assert(EnetQueue_getQCount(&rxFlowObj->rxFreeQ) ==
                        EnetUtils_min(rxFlowCfgInfo->pktRecvCount, maxPkts));

    /* Retrieve any CPSW packets which are ready */
    status = EnetDma_retrieveRxPktQ(rxFlowObj->hRxFlow, &rxReadyQ);
    EnetAppUtils_assert(status == ENET_SOK);
    /* There should not be any packet with DMA during init */
    EnetAppUtils_assert(EnetQueue_getQCount(&rxReadyQ) == 0U);

    EnetDma_submitRxPktQ(rxFlowObj->hRxFlow, &rxFlowObj->rxFreeQ);
}

static int32_t EnetTestCommon_openDma(EnetTestTaskObj *taskObj,
                                      bool disableHostAddrSecureFlag)
{
    int32_t status = ENET_SOK;
    int32_t i;
    Enet_IoctlPrms prms;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    EnetDma_Handle hDma;
    EnetUdma_OpenTxChPrms cpswTxChCfg;
    EnetUdma_OpenRxFlowPrms cpswRxFlowCfg;
    EnetPer_AttachCoreOutArgs attachCoreOutArgs;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &stateObj->coreId, &attachCoreOutArgs);
    status = Enet_ioctl(stateObj->hEnet,
                        stateObj->coreId,
                        ENET_PER_IOCTL_ATTACH_CORE,
                        &prms);

    EnetAppUtils_assert(status == ENET_SOK);

    stateObj->coreKey = attachCoreOutArgs.coreKey;

    for (i = 0U; (i < taskObj->taskCfg->numTxCh) && (i < ENET_TEST_CFG_MAX_NUM_TX_CH); i++)
    {
        EnetTestCommon_initTxFreePktQ(taskObj, i);

        /* Set configuration parameters */
        EnetUdma_initTxChParams(&cpswTxChCfg);

        EnetTestCommon_createTxSem(taskObj, i);

        status =EnetAppUtils_allocTxCh(stateObj->hEnet,
                                        stateObj->coreKey,
                                        stateObj->coreId,
                                        &stateObj->txChObj[i].txPSILThreadId);
       EnetAppUtils_assert(status == ENET_SOK);

        EnetTestCommon_setTxChPrms(&cpswTxChCfg,
                                   taskObj,
                                   stateObj->txChObj[i].txPSILThreadId,
                                   i);
        if (stateObj->txChCfgInfo[i]->updateTxChPrms != NULL)
        {
            stateObj->txChCfgInfo[i]->updateTxChPrms(&cpswTxChCfg,
                                                     taskObj,
                                                     stateObj->txChObj[i].txPSILThreadId,
                                                     i);
        }

        hDma = Enet_getDmaHandle(stateObj->hEnet);
        EnetAppUtils_assert(hDma != NULL);

        stateObj->txChObj[i].hTxCh = EnetUdma_openTxCh(hDma, &cpswTxChCfg);
        if (NULL != stateObj->txChObj[i].hTxCh)
        {
            status = EnetDma_enableTxEvent(stateObj->txChObj[i].hTxCh);
            if (ENET_SOK != status)
            {
               EnetAppUtils_print("EnetDma_startTxCh() failed: %d\n", status);
                status = ENET_EFAIL;
            }
        }
        else
        {
           EnetAppUtils_print("EnetUdma_openTxCh() failed to open: %d\n",
                               status);
            status = ENET_EFAIL;
        }

       EnetAppUtils_assert(status == ENET_SOK);
    }

    for (i = 0U; (i < taskObj->taskCfg->numRxFlow) && (i < ENET_TEST_CFG_MAX_NUM_RX_FLOWS); i++)
    {
        /* Open the CPSW RX flow  */
        if (status == ENET_SOK)
        {
            EnetUdma_initRxFlowParams(&cpswRxFlowCfg);

            EnetTestCommon_createRxSem(taskObj, i);

            status =EnetAppUtils_allocRxFlow(stateObj->hEnet,
                                              stateObj->coreKey,
                                              stateObj->coreId,
                                              &cpswRxFlowCfg.startIdx,
                                              &cpswRxFlowCfg.flowIdx);

            EnetTestCommon_setRxflowPrms(&cpswRxFlowCfg, taskObj, i);
            if (stateObj->rxFlowCfgInfo[i]->updateRxFlowPrms != NULL)
            {
                stateObj->rxFlowCfgInfo[i]->updateRxFlowPrms(&cpswRxFlowCfg, taskObj, i);
            }

            EnetAppUtils_assert(status == ENET_SOK);

            hDma = Enet_getDmaHandle(stateObj->hEnet);
            EnetAppUtils_assert(hDma != NULL);

            stateObj->rxFlowObj[i].hRxFlow = EnetUdma_openRxFlow(hDma, &cpswRxFlowCfg);

            if (NULL == stateObj->rxFlowObj[i].hRxFlow)
            {
               EnetAppUtils_print("EnetUdma_openRxFlow() failed to open: %d\n", status);
               EnetAppUtils_freeRxFlow(stateObj->hEnet,
                                        stateObj->coreKey,
                                        stateObj->coreId,
                                        cpswRxFlowCfg.flowIdx);
               EnetAppUtils_assert(NULL != stateObj->rxFlowObj[i].hRxFlow);
            }
            else
            {
                /* save flow idx */
                stateObj->rxFlowObj[i].rxFlowIdx      = cpswRxFlowCfg.flowIdx;
                stateObj->rxFlowObj[i].rxFlowStartIdx = cpswRxFlowCfg.startIdx;
                /* Submit all ready RX buffers to DMA.*/
                EnetTestCommon_initRxReadyPktQ(taskObj, i);
            }

            if (stateObj->rxFlowCfgInfo[i]->useDefaultFlow)
            {
                status =EnetAppUtils_regDfltRxFlow(stateObj->hEnet,
                                                            stateObj->coreKey,
                                                            stateObj->coreId,
                                                            stateObj->rxFlowObj[i].rxFlowStartIdx,
                                                            stateObj->rxFlowObj[i].rxFlowIdx);
               EnetAppUtils_assert(status == ENET_SOK);
            }
        }

       EnetAppUtils_assert(status == ENET_SOK);
    }

    EnetAppUtils_allocMac(stateObj->hEnet, stateObj->coreKey, stateObj->coreId, &stateObj->hostMacAddr[0U]);
    EnetAppUtils_print("Host MAC address: ");
    EnetAppUtils_printMacAddr(&stateObj->hostMacAddr[0U]);
    EnetAppUtils_addHostPortEntry(stateObj->hEnet,
                                  stateObj->coreId,
                                  stateObj->hostMacAddr);
    if ((disableHostAddrSecureFlag) && (status == ENET_SOK))
    {
        /* For mac loopback disable secure flag for the host port entry */
        EnetTestCommon_disableHostAleEntrySecure(taskObj);
    }

    return status;
}

static void EnetTestCommon_freePkts(EnetDma_PktQ *pPktQ)
{
    uint32_t i;
    uint32_t pktQCnt;
    EnetDma_Pkt *pktInfo;

    pktQCnt = EnetQueue_getQCount(pPktQ);
    for (i = 0U; i < pktQCnt; i++)
    {
        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(pPktQ);
        EnetMem_freeEthPkt(pktInfo);
    }
}

static void EnetTestCommon_unregisterFlows(EnetTestTaskObj *taskObj,
                                           uint32_t rxFlowCfgIndex)
{
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    Enet_IoctlPrms prms;
    int32_t status;

    if (stateObj->rxFlowCfgInfo[rxFlowCfgIndex]->useDefaultFlow)
    {
        status =EnetAppUtils_unregDfltRxFlow(stateObj->hEnet,
                                                      stateObj->coreKey,
                                                      stateObj->coreId,
                                                      stateObj->rxFlowObj[rxFlowCfgIndex].rxFlowStartIdx,
                                                      stateObj->rxFlowObj[rxFlowCfgIndex].rxFlowIdx);
    }
    else
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, &stateObj->rxFlowObj[rxFlowCfgIndex].rxFlowIdx);

        status = Enet_ioctl(stateObj->hEnet,
                            stateObj->coreId,
                            CPSW_ALE_IOCTL_DEL_ALL_POLICER_THREADID,
                            &prms);
    }

   EnetAppUtils_assert(status == ENET_SOK);
}

static void EnetTestCommon_closeDma(EnetTestTaskObj *taskObj)
{
    EnetDma_PktQ rxReadyQ;
    EnetDma_PktQ teardownFQ, teardownCQ;
    int32_t status = ENET_SOK;
    uint32_t i;
    Enet_IoctlPrms prms;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    /* Retrieve any CPSW packets which are ready */
    for (i = 0U; i < taskObj->taskCfg->numRxFlow; i++)
    {
        EnetQueue_initQ(&rxReadyQ);
        status = EnetDma_retrieveRxPktQ(stateObj->rxFlowObj[i].hRxFlow,
                                           &rxReadyQ);
        EnetAppUtils_assert(status == ENET_SOK);
        EnetTestCommon_freePkts(&rxReadyQ);
    }

    /* Close RX channel */
    for (i = 0U; i < taskObj->taskCfg->numRxFlow; i++)
    {
        EnetQueue_initQ(&teardownFQ);
        EnetQueue_initQ(&teardownCQ);
        EnetTestCommon_unregisterFlows(taskObj, i);

        status = EnetUdma_closeRxFlow(stateObj->rxFlowObj[i].hRxFlow,
                                     &teardownFQ,
                                     &teardownCQ);
        EnetAppUtils_assert(status == ENET_SOK);
        EnetTestCommon_freePkts(&teardownFQ);
        EnetTestCommon_freePkts(&teardownCQ);
        status =EnetAppUtils_freeRxFlow(stateObj->hEnet, stateObj->coreKey, stateObj->coreId, stateObj->rxFlowObj[i].rxFlowIdx);
        EnetAppUtils_assert(status == ENET_SOK);
        EnetTestCommon_freePkts(&stateObj->rxFlowObj[i].rxFreeQ);
        EnetTestCommon_freePkts(&stateObj->rxFlowObj[i].rxReadyQ);
    }

    /* Close TX channel */
    for (i = 0U; i < taskObj->taskCfg->numTxCh; i++)
    {
        status = EnetDma_disableTxEvent(stateObj->txChObj[i].hTxCh);
        EnetAppUtils_assert(status == ENET_SOK);
        EnetQueue_initQ(&teardownFQ);
        EnetQueue_initQ(&teardownCQ);
        status += EnetUdma_closeTxCh(stateObj->txChObj[i].hTxCh, &teardownFQ,
                                    &teardownCQ);
        EnetAppUtils_assert(status == ENET_SOK);
        EnetTestCommon_freePkts(&teardownFQ);
        EnetTestCommon_freePkts(&teardownCQ);

        status =EnetAppUtils_freeTxCh(stateObj->hEnet,
                                       stateObj->coreKey,
                                       stateObj->coreId,
                                       stateObj->txChObj[i].txPSILThreadId);
        EnetAppUtils_assert(status == ENET_SOK);
        EnetTestCommon_freePkts(&stateObj->txChObj[i].txFreePktInfoQ);
    }

    ENET_IOCTL_SET_IN_ARGS(&prms, &stateObj->coreKey);

    status = Enet_ioctl(stateObj->hEnet,
                        stateObj->coreId,
                        ENET_PER_IOCTL_DETACH_CORE,
                        &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("close() failed ENET_PER_IOCTL_DETACH_CORE: %d\n", status);
    }

    if (status != ENET_SOK)
    {
        EnetAppUtils_print("EnetTestCommon_closeDma() failed: %d\n", status);
    }

    EnetMem_deInit();
}

void EnetTestCommon_disableHostAleEntrySecure(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setUcastOutArgs;
    CpswAle_SetUcastEntryInArgs setUcastInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memcpy(&setUcastInArgs.addr.addr[0U], &stateObj->hostMacAddr[0U],
           sizeof(setUcastInArgs.addr.addr));
    setUcastInArgs.addr.vlanId  = 0U;
    setUcastInArgs.info.portNum = 0U;
    setUcastInArgs.info.blocked = BFALSE;
    setUcastInArgs.info.secure  = BFALSE;
    setUcastInArgs.info.super   = BFALSE;
    setUcastInArgs.info.ageable = BFALSE;
    setUcastInArgs.info.trunk   = BFALSE;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &setUcastOutArgs);

    status = Enet_ioctl(stateObj->hEnet,
                        stateObj->coreId,
                        CPSW_ALE_IOCTL_ADD_UCAST,
                        &prms);

    if (status != ENET_SOK)
    {
        EnetAppUtils_print("EnetTestCommon_openCpsw() failed CPSW_ALE_IOCTL_SET_PORT_STATE: %d\n", status);
        EnetAppUtils_assert(BFALSE);
    }
}

int32_t EnetTestCommon_setTxPktInfo(EnetTestTaskObj *taskObj,
                                    uint32_t         txChIndex,
                                    uint32_t         pktNum,
                                    EnetDma_Pkt *pktInfo,
                                    bool            *testComplete)
{
    EthFrame *frame;
    /* Broadcast address */
    uint8_t bcastAddr[ENET_MAC_ADDR_LEN] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};
    EnetTestStateObj *stateObj          = &taskObj->stateObj;

    frame = (EthFrame *)pktInfo->sgList.list[0].bufPtr;

    memcpy(frame->hdr.dstMac, bcastAddr, ENET_MAC_ADDR_LEN);
    memcpy(frame->hdr.srcMac, &stateObj->hostMacAddr[0U], ENET_MAC_ADDR_LEN);
    frame->hdr.etherType = Enet_htons(ETHERTYPE_EXPERIMENTAL1);

    memset(&frame->payload[0U],
           (uint8_t)(0xA5 + pktNum),
           stateObj->txChCfgInfo[txChIndex]->maxTxPktLen);
    pktInfo->sgList.list[0].segmentFilledLen = stateObj->txChCfgInfo[txChIndex]->maxTxPktLen +
                                               sizeof(EthFrameHeader);
    pktInfo->appPriv = stateObj;
    *testComplete    = BFALSE;

    return ENET_SOK;
}

int32_t EnetTestCommon_processRxPkt(EnetTestTaskObj *taskObj,
                                    uint32_t rxFlowId,
                                    uint32_t pktNum,
                                    EnetDma_Pkt *pktInfo,
                                    bool *testComplete)
{
#ifdef ENABLE_PRINTFRAME
    EthFrame *frame;

    /* Consume the packet by just printing its content */
    frame = (EthFrame *)pktInfo->sgList.list[0].bufPtr;
    EnetAppUtils_printFrame(frame, (pktInfo->sgList.list[0].segmentFilledLen - sizeof(EthFrameHeader)));
#endif
    *testComplete = BFALSE;

    return ENET_SOK;
}

uint32_t EnetTestCommon_retrieveFreeTxPkts(EnetTestTaskObj *taskObj,
                                           uint32_t txCfgIndex)
{
    EnetDma_PktQ txFreeQ;
    EnetDma_Pkt *pktInfo;
    int32_t status;
    uint32_t txFreeQCnt        = 0U;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    EnetQueue_initQ(&txFreeQ);

    /* Retrieve any CPSW packets that may be free now */
    status = EnetDma_retrieveTxPktQ(stateObj->txChObj[txCfgIndex].hTxCh,
                                           &txFreeQ);
    if (status == ENET_SOK)
    {
        txFreeQCnt = EnetQueue_getQCount(&txFreeQ);

        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFreeQ);
        while (NULL != pktInfo)
        {
            EnetQueue_enq(&stateObj->txChObj[txCfgIndex].txFreePktInfoQ,
                          &pktInfo->node);

            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFreeQ);
        }
    }
    else
    {
        EnetAppUtils_print("retrieveFreeTxPkts() failed to retrieve pkts: %d\n",
                           status);
    }

    return txFreeQCnt;
}

uint32_t EnetTestCommon_receivePkts(EnetTestTaskObj *taskObj,
                                    uint32_t rxCfgIndex)
{
    EnetDma_PktQ rxReadyQ;
    int32_t status;
    EnetTestStateObj *stateObj   = &taskObj->stateObj;
    EnetTestRxFlowObj *rxFlowObj = &stateObj->rxFlowObj[rxCfgIndex];

    EnetQueue_initQ(&rxReadyQ);

    /* Retrieve any CPSW packets which are ready */
    status = EnetDma_retrieveRxPktQ(rxFlowObj->hRxFlow,
                                       &rxReadyQ);
    if (status == ENET_SOK)
    {
        EnetQueue_append(&rxFlowObj->rxReadyQ, &rxReadyQ);
    }
    else
    {
        EnetAppUtils_print("receivePkts() failed to retrieve pkts: %d\n",
                           status);
    }

    return EnetQueue_getQCount(&rxFlowObj->rxReadyQ);
}

Enet_Handle EnetTestCommon_getCpswHandle(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    EnetAppUtils_assert(stateObj->hEnet != NULL);
    return stateObj->hEnet;
}

static void EnetTestCommon_setUdmaRmConfig(Udma_RmInitPrms *rmInitPrms)
{
    uint32_t defaultVintr = rmInitPrms->numVintr;

    if (defaultVintr < ENET_TEST_VINTR_MAX)
    {
        EnetAppUtils_print("[Error] Insuffient no. of VINTR as per defaultBoardCfg!! \n");
        EnetAppUtils_print("Available no. of VINTR: %d!! \n",
                            defaultVintr);
    }
}

static void EnetTestCommon_InitUdmaDrv(EnetTestTaskObj *taskObj)
{
    Udma_InitPrms initPrms, *pInitPrms = NULL;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint32_t instId;

    if ((taskObj->taskCfg->enetType == ENET_CPSW_9G)||
        (taskObj->taskCfg->enetType == ENET_CPSW_5G))
    {
        instId =EnetAppUtils_getNavSSInstanceId(taskObj->taskCfg->enetType);
        /* Initialize the UDMA driver based on NAVSS instance */
        UdmaInitPrms_init(instId, &initPrms);
        initPrms.printFxn = (Udma_PrintFxn) &EnetAppUtils_print;

        /* Need custom Udma open function for J721E as the default RM config
         * is not sufficient to allocates more than total of 5 Rx+Tx which is
         * not sufficient for testcases like policer test
         */
        EnetTestCommon_setUdmaRmConfig(&initPrms.rmInitPrms);

        pInitPrms = &initPrms;
    }

    stateObj->hUdmaDrv =EnetAppUtils_udmaOpen(taskObj->taskCfg->enetType, pInitPrms);
   EnetAppUtils_assert(NULL != stateObj->hUdmaDrv);
}

static void EnetTestCommon_DeInitUdmaDrv(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    EnetAppUtils_udmaclose(stateObj->hUdmaDrv);
}

static int32_t EnetTestCommon_transmitPkts(EnetTestTaskObj *taskObj,
                                           uint32_t txCfgIndex,
                                           EnetDma_PktQ *pTxSubmitQ,
                                           uint32_t *pCurRetrieveCnt)
{
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    *pCurRetrieveCnt = 0U;
    while (0U != EnetQueue_getQCount(pTxSubmitQ))
    {
        status = EnetDma_submitTxPktQ(stateObj->txChObj[txCfgIndex].hTxCh,
                                                pTxSubmitQ);
        if (status != ENET_SOK)
        {
            break;
        }

        if (0U != EnetQueue_getQCount(pTxSubmitQ))
        {
            *pCurRetrieveCnt += EnetTestCommon_waitForXmit(taskObj, txCfgIndex);
        }
    }

    return status;
}

static uint32_t EnetTestCommon_waitForXmit(EnetTestTaskObj *taskObj,
                                           uint32_t txCfgIndex)
{
    SemaphoreP_Status semStatus;
    uint32_t txFreeQCnt;
    EnetTestTxChObj *txChObj;

    txChObj    = &taskObj->stateObj.txChObj[txCfgIndex];
    txFreeQCnt = EnetTestCommon_retrieveFreeTxPkts(taskObj, txCfgIndex);
    while (txFreeQCnt == 0U)
    {
        semStatus = SemaphoreP_pend(txChObj->hTxCQSem, SemaphoreP_WAIT_FOREVER);
        EnetAppUtils_assert(semStatus == SemaphoreP_OK);
        txFreeQCnt = EnetTestCommon_retrieveFreeTxPkts(taskObj, txCfgIndex);
    }

    return txFreeQCnt;
}

static void EnetTestCommon_xmitTask(void *a0,
                                    void *a1)
{
    int32_t retVal           = ENET_SOK;
    EnetTestTaskObj *taskObj = (EnetTestTaskObj *)a0;
    uint32_t txCfgIndex      = (uint32_t)(uintptr_t)a1;
    EnetDma_Pkt *pktInfo;
    EnetDma_PktQ txSubmitQ;
    EnetTestTxChCfgInfo *txChCfgInfo;
    EnetTestTxChObj *txChObj;
    EnetTestCommon_TxTestStatus_t testStatusMsg;
    EnetTest_SetTxPktInfoFxn setTxPktInfo;
    bool mbxStatus;
    bool testComplete;
    uint32_t startTime;
    int32_t byteCount;

    txChCfgInfo = taskObj->stateObj.txChCfgInfo[txCfgIndex];
    txChObj     = &taskObj->stateObj.txChObj[txCfgIndex];

    /* Open the CPSW TX channel  */
    if (txChCfgInfo->setTxPktInfo == NULL)
    {
        setTxPktInfo = &EnetTestCommon_setTxPktInfo;
    }
    else
    {
        setTxPktInfo = txChCfgInfo->setTxPktInfo;
    }

    if (txChCfgInfo->preTxSend != NULL)
    {
        retVal = txChCfgInfo->preTxSend(taskObj, txCfgIndex);
    }

    EnetDma_enableTxEvent(txChObj->hTxCh);
    txChObj->processCount = 0U;
    txChObj->retrieveCnt  = 0U;
    byteCount             = 0;
    startTime             = 0U;
    if (ENET_SOK == retVal)
    {
        testComplete = BFALSE;
        while ((txChObj->processCount < txChCfgInfo->pktSendCount)
               &&
               (ENET_SOK == retVal)
               &&
               (testComplete == BFALSE))
        {
            EnetQueue_initQ(&txSubmitQ);

            /* Dequeue one free TX Eth packet */
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txChObj->txFreePktInfoQ);
            while (NULL != pktInfo)
            {
                retVal = setTxPktInfo(taskObj,
                                      txCfgIndex,
                                      txChObj->processCount,
                                      pktInfo,
                                      &testComplete);
                if (txChObj->processCount == 0U)
                {
                    startTime = AppUtils_getCurTimeInMsec();
                }

                txChObj->processCount++;
                byteCount += pktInfo->sgList.list[0].segmentFilledLen;

                /* Enqueue the packet for later transmission */
                EnetQueue_enq(&txSubmitQ, &pktInfo->node);

                if (txChObj->processCount >= txChCfgInfo->pktSendCount)
                {
                    break;
                }

                if (testComplete)
                {
                    EnetAppUtils_print("Test Completed:%s,Id:%u,Frame:%u\n", __func__, txCfgIndex, txChObj->processCount);
                    break;
                }

                if (retVal != ENET_SOK)
                {
                    EnetAppUtils_print("Test Fail:%s,Id:%u,Frame:%u\n", __func__, txCfgIndex, txChObj->processCount);
                    break;
                }

                /* Dequeue one free TX Eth packet */
                pktInfo =
                    (EnetDma_Pkt *)EnetQueue_deq(&txChObj->txFreePktInfoQ);
            }

            if (EnetQueue_getQCount(&txSubmitQ) == 0U)
            {
                txChObj->retrieveCnt += EnetTestCommon_waitForXmit(taskObj, txCfgIndex);
            }
            else
            {
                uint32_t curRetrieveCnt = 0U;

                retVal = EnetTestCommon_transmitPkts(taskObj, txCfgIndex,
                                                     &txSubmitQ,
                                                     &curRetrieveCnt);
                txChObj->retrieveCnt += curRetrieveCnt;
            }

            if (retVal != ENET_SOK)
            {
                break;
            }
        }
    }

    if (ENET_SOK == retVal)
    {
        while (txChObj->retrieveCnt < txChCfgInfo->pktSendCount)
        {
            txChObj->retrieveCnt += EnetTestCommon_waitForXmit(taskObj, txCfgIndex);
        }
    }

    if (ENET_SOK == retVal)
    {
        uint32_t elapsedMSecs = AppUtils_getCurTimeInMsec() - startTime;
        txChObj->transmitBitRate = (byteCount * 8) / elapsedMSecs;

        if (txChCfgInfo->postTxSend != NULL)
        {
            retVal = txChCfgInfo->postTxSend(taskObj, txCfgIndex);
        }
    }

    EnetAppUtils_print("Transmit Completed:%s,Id:%u,Frame:%u\n", __func__, txCfgIndex, txChObj->retrieveCnt);
    EnetDma_disableTxEvent(txChObj->hTxCh);
    // TODO: OSAL do not support TaskP_stat call
    testStatusMsg.stackUsed  = 0U;
    testStatusMsg.testStatus = retVal;
    mbxStatus                =
        MailboxP_post(taskObj->stateObj.txChObj[txCfgIndex].hTxTestStatusMbx,
                     &testStatusMsg,
                     MailboxP_WAIT_FOREVER);
    EnetAppUtils_assert(mbxStatus == MailboxP_OK);
}

static void EnetTestCommon_deleteTxTask(EnetTestTaskObj *taskObj,
                                        uint32_t txCfgIndex)
{
    TaskP_Params *pTaskParams = &taskObj->stateObj.txChObj[txCfgIndex].txTaskParams;

    TaskP_delete(&taskObj->stateObj.txChObj[txCfgIndex].hTxTask);

    Utils_memFree(UTILS_MEM_HEAP_ID_DDR, pTaskParams->stack, pTaskParams->stacksize);
}

static void EnetTestCommon_deleteTxSem(EnetTestTaskObj *taskObj,
                                       uint32_t txCfgIndex)
{
    if (taskObj->stateObj.txChObj[txCfgIndex].hTxCQSem != NULL)
    {
        SemaphoreP_delete(taskObj->stateObj.txChObj[txCfgIndex].hTxCQSem);
    }
}

static void EnetTestCommon_deleteTxMbx(EnetTestTaskObj *taskObj,
                                       uint32_t txCfgIndex)
{
    MailboxP_delete(taskObj->stateObj.txChObj[txCfgIndex].hTxTestStatusMbx);
}

static void  EnetTestCommon_createTxTask(EnetTestTaskObj *taskObj,
                                         uint32_t txCfgIndex)
{
    TaskP_Params *pTaskParams = &taskObj->stateObj.txChObj[txCfgIndex].txTaskParams;
    TaskP_Params_init(pTaskParams);

    pTaskParams->priority = ENET_TEST_COMMON_TX_TASK_PRI;
    pTaskParams->stack    = Utils_memAlloc(UTILS_MEM_HEAP_ID_DDR,
                                           ENET_TEST_COMMON_TX_TASK_STACK_SIZE,
                                           sizeof(uint64_t));
    EnetAppUtils_assert(pTaskParams->stack != NULL);
    pTaskParams->stacksize      = ENET_TEST_COMMON_TX_TASK_STACK_SIZE;
    pTaskParams->arg0           = (void *)taskObj;
    pTaskParams->arg1           = (void *)(uintptr_t)txCfgIndex;
    pTaskParams->name           = (char *)"Test common Transmit task";

    taskObj->stateObj.txChObj[txCfgIndex].hTxTask = TaskP_create(&EnetTestCommon_xmitTask,
                                                                 pTaskParams);
    EnetAppUtils_assert(taskObj->stateObj.txChObj[txCfgIndex].hTxTask != NULL);
}

static void  EnetTestCommon_createTxSem(EnetTestTaskObj *taskObj,
                                        uint32_t txCfgIndex)
{
    SemaphoreP_Params semParams;

    SemaphoreP_Params_init(&semParams);
    semParams.mode                                 = SemaphoreP_Mode_BINARY;
    taskObj->stateObj.txChObj[txCfgIndex].hTxCQSem = SemaphoreP_create(0, &semParams);
    EnetAppUtils_assert(NULL != taskObj->stateObj.txChObj[txCfgIndex].hTxCQSem);
}

static void  EnetTestCommon_createTxMbx(EnetTestTaskObj *taskObj,
                                        uint32_t txCfgIndex)
{
    MailboxP_Params mbxParams;

    MailboxP_Params_init(&mbxParams);
    mbxParams.name = (uint8_t *)"TxMbx";
    mbxParams.buf = (void *)gTxMbxBuf;
    mbxParams.size =  sizeof(EnetTestCommon_TxTestStatus_t);
    mbxParams.count = ENET_TEST_COMMON_TX_TEST_STATUS_COUNT;
    mbxParams.bufsize = sizeof(EnetTestCommon_TxTestStatus_t) * ENET_TEST_COMMON_TX_TEST_STATUS_COUNT;
    taskObj->stateObj.txChObj[txCfgIndex].hTxTestStatusMbx =MailboxP_create(&mbxParams);
    EnetAppUtils_assert(taskObj->stateObj.txChObj[txCfgIndex].hTxTestStatusMbx != NULL);
}

static void EnetTestCommon_waitTxTaskTermination(EnetTestTaskObj *taskObj,
                                                 uint32_t txCfgIndex)
{
    EnetTestCommon_TxTestStatus_t testStatusMsg;
    bool mbxStatus;

    mbxStatus =
        MailboxP_pend(taskObj->stateObj.txChObj[txCfgIndex].hTxTestStatusMbx,
                     &testStatusMsg,
                     MailboxP_WAIT_FOREVER);
    EnetAppUtils_assert(mbxStatus == MailboxP_OK);

    taskObj->stateObj.txChObj[txCfgIndex].txTaskStatus =
        testStatusMsg.testStatus;
    taskObj->stateObj.txChObj[txCfgIndex].stackUsed =
        testStatusMsg.stackUsed;

    while (TaskP_isTerminated(taskObj->stateObj.txChObj[txCfgIndex].hTxTask) != 1)
    {
        TaskP_sleep(1U);
    }
}

static void EnetTestCommon_spawnTxChTasks(EnetTestTaskObj *taskObj)
{
    uint32_t i;

    for (i = 0U; i < taskObj->taskCfg->numTxCh; i++)
    {
        EnetTestCommon_createTxMbx(taskObj, i);
        EnetTestCommon_createTxTask(taskObj, i);
    }
}

static void EnetTestCommon_terminateTxChTasks(EnetTestTaskObj *taskObj)
{
    uint32_t i;

    for (i = 0U; i < taskObj->taskCfg->numTxCh; i++)
    {
        EnetTestCommon_waitTxTaskTermination(taskObj, i);
        EnetTestCommon_deleteTxTask(taskObj, i);
        EnetTestCommon_deleteTxMbx(taskObj, i);
    }
}

static void EnetTestCommon_deleteRxTask(EnetTestTaskObj *taskObj,
                                        uint32_t rxCfgIndex)
{
    TaskP_Params *pTaskParams = &taskObj->stateObj.rxFlowObj[rxCfgIndex].rxTaskParams;

    TaskP_delete(&taskObj->stateObj.rxFlowObj[rxCfgIndex].hRxTask);

    Utils_memFree(UTILS_MEM_HEAP_ID_DDR, pTaskParams->stack, pTaskParams->stacksize);
}

static void EnetTestCommon_deleteRxSem(EnetTestTaskObj *taskObj,
                                       uint32_t rxCfgIndex)
{
    if (taskObj->stateObj.rxFlowObj[rxCfgIndex].hRxCQSem != NULL)
    {
        SemaphoreP_delete(taskObj->stateObj.rxFlowObj[rxCfgIndex].hRxCQSem);
    }
}

static void EnetTestCommon_deleteRxMbx(EnetTestTaskObj *taskObj,
                                       uint32_t rxCfgIndex)
{
    MailboxP_delete(taskObj->stateObj.rxFlowObj[rxCfgIndex].hRxTestStatusMbx);
}

static void  EnetTestCommon_createRxTask(EnetTestTaskObj *taskObj,
                                         uint32_t rxCfgIndex)
{
    TaskP_Params *pTaskParams = &taskObj->stateObj.rxFlowObj[rxCfgIndex].rxTaskParams;

    TaskP_Params_init(pTaskParams);

    pTaskParams->priority = ENET_TEST_COMMON_RX_TASK_PRI;
    pTaskParams->stack    = Utils_memAlloc(UTILS_MEM_HEAP_ID_DDR,
                                           ENET_TEST_COMMON_RX_TASK_STACK_SIZE,
                                           sizeof(uint64_t));
    EnetAppUtils_assert(pTaskParams->stack != NULL);

    pTaskParams->stacksize      = ENET_TEST_COMMON_RX_TASK_STACK_SIZE;
    pTaskParams->arg0           = (void *)taskObj;
    pTaskParams->arg1           = (void *)(uintptr_t)rxCfgIndex;
    pTaskParams->name           = (char *)"Test common Receive task";

    taskObj->stateObj.rxFlowObj[rxCfgIndex].hRxTask = TaskP_create(&EnetTestCommon_recvTask,
                                                                  pTaskParams);
    EnetAppUtils_assert(taskObj->stateObj.rxFlowObj[rxCfgIndex].hRxTask != NULL);
}

void  EnetTestCommon_createRxSem(EnetTestTaskObj *taskObj,
                                 uint32_t rxCfgIndex)
{
    SemaphoreP_Params semParams;

    SemaphoreP_Params_init(&semParams);
    semParams.mode                                   = SemaphoreP_Mode_BINARY;
    taskObj->stateObj.rxFlowObj[rxCfgIndex].hRxCQSem = SemaphoreP_create(0, &semParams);
    EnetAppUtils_assert(NULL != taskObj->stateObj.rxFlowObj[rxCfgIndex].hRxCQSem);
}

static void  EnetTestCommon_createRxMbx(EnetTestTaskObj *taskObj,
                                        uint32_t rxCfgIndex)
{
    MailboxP_Params mbxParams;

    MailboxP_Params_init(&mbxParams);
    mbxParams.name = (uint8_t *)"RxMbx";
    mbxParams.buf = (void *)gRxMbxBuf;
    mbxParams.size =  sizeof(EnetTestCommon_RxTestStatus_t);
    mbxParams.count = ENET_TEST_COMMON_RX_TEST_STATUS_COUNT;
    mbxParams.bufsize = sizeof(EnetTestCommon_RxTestStatus_t) * ENET_TEST_COMMON_RX_TEST_STATUS_COUNT;
    taskObj->stateObj.rxFlowObj[rxCfgIndex].hRxTestStatusMbx = MailboxP_create(&mbxParams);
    EnetAppUtils_assert(taskObj->stateObj.rxFlowObj[rxCfgIndex].hRxTestStatusMbx != NULL);
}

static void EnetTestCommon_waitRxTaskTermination(EnetTestTaskObj *taskObj,
                                                 uint32_t rxCfgIndex)
{
    EnetTestCommon_RxTestStatus_t testStatusMsg;
    bool mbxStatus;

    mbxStatus = MailboxP_pend(taskObj->stateObj.rxFlowObj[rxCfgIndex].hRxTestStatusMbx,
                             &testStatusMsg,
                             MailboxP_WAIT_FOREVER);
    EnetAppUtils_assert(mbxStatus == MailboxP_OK);

    taskObj->stateObj.rxFlowObj[rxCfgIndex].rxTaskStatus = testStatusMsg.testStatus;
    taskObj->stateObj.rxFlowObj[rxCfgIndex].stackUsed    = testStatusMsg.stackUsed;

    while (TaskP_isTerminated(taskObj->stateObj.rxFlowObj[rxCfgIndex].hRxTask) != 1)
    {
        TaskP_sleep(1U);
    }
}

void EnetTestCommon_spawnRxFlowTasks(EnetTestTaskObj *taskObj)
{
    uint32_t i;

    for (i = 0U; i < taskObj->taskCfg->numRxFlow; i++)
    {
        EnetTestCommon_createRxMbx(taskObj, i);
        EnetTestCommon_createRxTask(taskObj, i);
    }
}

void EnetTestCommon_terminateRxFlowTasks(EnetTestTaskObj *taskObj)
{
    uint32_t i;

    for (i = 0U; i < taskObj->taskCfg->numRxFlow; i++)
    {
        EnetTestCommon_waitRxTaskTermination(taskObj, i);
        EnetTestCommon_deleteRxTask(taskObj, i);
        EnetTestCommon_deleteRxMbx(taskObj, i);
    }
}

static void EnetTestCommon_recvTask(void *a0,
                                    void *a1)
{
    EnetTestTaskObj *taskObj = (EnetTestTaskObj *)a0;
    uint32_t rxCfgIndex      = (uint32_t)(uintptr_t)a1;
    EnetDma_Pkt *pktInfo;
    uint32_t rxReadyCnt;
    EnetTestRxFlowCfgInfo *rxFlowCfgInfo;
    EnetTestRxFlowObj *rxFlowObj;
    SemaphoreP_Status semStatus;
    bool testComplete;
    int32_t retVal;
    EnetTest_ProcessRxPktFxn processRxPkt;
    EnetTestCommon_RxTestStatus_t testStatusMsg;
    Bool mbxStatus;
    uint32_t startTime;
    int32_t byteCount;

    rxFlowCfgInfo = taskObj->stateObj.rxFlowCfgInfo[rxCfgIndex];
    rxFlowObj     = &taskObj->stateObj.rxFlowObj[rxCfgIndex];
    retVal        = ENET_SOK;
    byteCount     = 0;
    startTime     = 0U;

    /* Set process packet function to common function if application passes null */
    if (rxFlowCfgInfo->processRxPkt == NULL)
    {
        processRxPkt = &EnetTestCommon_processRxPkt;
    }
    else
    {
        processRxPkt = rxFlowCfgInfo->processRxPkt;
    }

    if (rxFlowCfgInfo->preRxProcess != NULL)
    {
        retVal = rxFlowCfgInfo->preRxProcess(taskObj, rxCfgIndex);
    }

    if (ENET_SOK == retVal)
    {
        rxFlowObj->processCount = 0U;
        testComplete            = BFALSE;
        while ((rxFlowObj->processCount < rxFlowCfgInfo->pktRecvCount)
               &&
               (ENET_SOK == retVal)
               &&
               (testComplete == BFALSE))
        {
            /* Get the packets received so far */
            rxReadyCnt = EnetTestCommon_receivePkts(taskObj, rxCfgIndex);
            if (rxReadyCnt > 0U)
            {
                /* Consume the received packets and release them */
                pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&rxFlowObj->rxReadyQ);
                while (NULL != pktInfo)
                {
                    if (rxFlowObj->processCount == 0U)
                    {
                        startTime = AppUtils_getCurTimeInMsec();
                    }

                    byteCount += pktInfo->sgList.list[0].segmentFilledLen;

                    retVal = processRxPkt(taskObj,
                                          rxCfgIndex,
                                          rxFlowObj->processCount,
                                          pktInfo,
                                          &testComplete);
                    if (retVal != ENET_SOK)
                    {
                       EnetAppUtils_print("Receive Fail:%s,Id:%u,Frame:%u\n",
                                           __func__, rxCfgIndex, rxFlowObj->processCount);
                        EnetQueue_enq(&rxFlowObj->rxFreeQ, &pktInfo->node);
                        break;
                    }

                    EnetQueue_enq(&rxFlowObj->rxFreeQ, &pktInfo->node);

                    rxFlowObj->processCount++;
                    if (!testComplete)
                    {
                        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&rxFlowObj->rxReadyQ);
                    }
                    else
                    {
                        pktInfo = NULL;
                    }
                }

                EnetDma_submitRxPktQ(rxFlowObj->hRxFlow, &rxFlowObj->rxFreeQ);
            }
            else
            {
                semStatus = SemaphoreP_pend(rxFlowObj->hRxCQSem, SemaphoreP_WAIT_FOREVER);
                EnetAppUtils_assert(semStatus == SemaphoreP_OK);
            }
        }
    }

    if (ENET_SOK == retVal)
    {
        uint32_t elapsedMSecs = AppUtils_getCurTimeInMsec() - startTime;
        rxFlowObj->receivedBitRate = (byteCount / elapsedMSecs) * 8 * 1000;

        if (rxFlowCfgInfo->postRxProcess != NULL)
        {
            retVal = rxFlowCfgInfo->postRxProcess(taskObj, rxCfgIndex);
        }
    }

   EnetAppUtils_print("Receive Complete:%s,Id:%u,Frame:%u\n", __func__, rxCfgIndex, rxFlowObj->processCount);
    // TODO: OSAL do not support TaskP_stat call
    testStatusMsg.stackUsed  = 0U;
    testStatusMsg.testStatus = retVal;
    mbxStatus                = MailboxP_post(rxFlowObj->hRxTestStatusMbx,
                                            &testStatusMsg,
                                            MailboxP_WAIT_FOREVER);
   EnetAppUtils_assert(mbxStatus == MailboxP_OK);
}

static bool EnetTestCommon_checkTxError(EnetTestTaskObj *taskObj)
{
    uint32_t i;
    bool error;

    for (i = 0U; i < taskObj->taskCfg->numTxCh; i++)
    {
        if (taskObj->stateObj.txChObj[i].txTaskStatus != ENET_SOK)
        {
            break;
        }
    }

    if (i < taskObj->taskCfg->numTxCh)
    {
        error = BTRUE;
    }
    else
    {
        error = BFALSE;
    }

    return error;
}

static bool EnetTestCommon_checkRxError(EnetTestTaskObj *taskObj)
{
    uint32_t i;
    bool error;

    for (i = 0U; i < taskObj->taskCfg->numRxFlow; i++)
    {
        if (taskObj->stateObj.rxFlowObj[i].rxTaskStatus != ENET_SOK)
        {
            break;
        }
    }

    if (i < taskObj->taskCfg->numRxFlow)
    {
        error = BTRUE;
    }
    else
    {
        error = BFALSE;
    }

    return error;
}

static void EnetTestCommon_printTxStackUsage(EnetTestTaskObj *taskObj)
{
    uint32_t i;

    EnetAppUtils_print("\r\n");
    for (i = 0U; i < taskObj->taskCfg->numTxCh; i++)
    {
        EnetAppUtils_print("Tx stack usage.Tx Cfg Id:%d, Stack Used:%d\n",
                           i, taskObj->stateObj.txChObj[i].stackUsed);
    }
}

static void EnetTestCommon_printRxStackUsage(EnetTestTaskObj *taskObj)
{
    uint32_t i;

    EnetAppUtils_print("\r\n");
    for (i = 0U; i < taskObj->taskCfg->numRxFlow; i++)
    {
        EnetAppUtils_print("Rx stack usage.Rx Cfg Id:%d, Stack Used:%d\n",
                           i, taskObj->stateObj.rxFlowObj[i].stackUsed);
    }
}



int32_t EnetTestCommon_pktTxRx(EnetTestTaskObj *taskObj)
{
    int32_t retVal;
    // TODO: OSAL API do not support return of current priority

    EnetTestCommon_spawnTxChTasks(taskObj);
    EnetTestCommon_spawnRxFlowTasks(taskObj);

    EnetTestCommon_terminateTxChTasks(taskObj);
    EnetTestCommon_terminateRxFlowTasks(taskObj);

    retVal = EnetTestCommon_checkTxError(taskObj);
    if (ENET_SOK == retVal)
    {
        retVal = EnetTestCommon_checkRxError(taskObj);
    }

    EnetTestCommon_printTxStackUsage(taskObj);
    EnetTestCommon_printRxStackUsage(taskObj);

    return retVal;
}

int32_t EnetTestCommon_setAleMulticastEntry(EnetTestTaskObj *taskObj,
                                            uint8_t* macAddr,
                                            uint32_t vlanId,
                                            uint32_t numIgnBits,
                                            uint32_t portMask)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setMcastOutArgs;
    CpswAle_SetMcastEntryInArgs setMcastInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memcpy(&setMcastInArgs.addr.addr[0], macAddr,
           sizeof(setMcastInArgs.addr.addr));
    setMcastInArgs.addr.vlanId = vlanId;

    setMcastInArgs.info.super      = BFALSE;
    setMcastInArgs.info.fwdState   = CPSW_ALE_FWDSTLVL_FWD;
    setMcastInArgs.info.portMask   = portMask;
    setMcastInArgs.info.numIgnBits = numIgnBits;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setMcastInArgs, &setMcastOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_MCAST,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("%s: failed CPSW_ALE_IOCTL_ADD_MCAST: %d\n",  __func__,
                           status);
    }

    return status;
}

int32_t EnetTestCommon_getAleMulticastEntry(EnetTestTaskObj *taskObj,
                                            uint8_t* macAddr,
                                            uint32_t vlanId,
                                            uint32_t numIgnBits,
                                            uint32_t *pPortMask)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_GetMcastEntryInArgs inArgs;
    CpswAle_GetMcastEntryOutArgs getMcastOutArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memcpy(&inArgs.addr.addr[0], macAddr, sizeof(inArgs.addr.addr));
    inArgs.addr.vlanId = vlanId;
    inArgs.numIgnBits  = numIgnBits;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &getMcastOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_LOOKUP_MCAST,
                        &prms);
    if (status == ENET_SOK)
    {
        //EnetAppUtils_print(ALE ENTRY INDEX: %d\n", getMcastOutArgs.aleIndex);
        //EnetAppUtils_print("portMask: %d\n", getMcastOutArgs.info.portMask);
        //EnetAppUtils_print("numIgnBits: %d\n", getMcastOutArgs.info.numIgnBits);
       EnetAppUtils_assert(numIgnBits == getMcastOutArgs.info.numIgnBits);
    }
    else
    {
       EnetAppUtils_print("CPSW_ALE_IOCTL_LOOKUP_MCAST IOCTL Failed: %d\n",
                           status);
    }

    *pPortMask = getMcastOutArgs.info.portMask;
    return status;
}

void EnetTestCommon_discardRxPkts(EnetTestTaskObj *taskObj,
                                  uint32_t rxCfgIndex)
{
    EnetTestStateObj *stateObj   = &taskObj->stateObj;
    EnetTestRxFlowObj *rxFlowObj = &stateObj->rxFlowObj[rxCfgIndex];

    EnetQueue_append(&rxFlowObj->rxFreeQ, &rxFlowObj->rxReadyQ);
    EnetQueue_initQ(&rxFlowObj->rxReadyQ);
    while (EnetTestCommon_receivePkts(taskObj, rxCfgIndex))
    {
        EnetQueue_append(&rxFlowObj->rxFreeQ, &rxFlowObj->rxReadyQ);
        EnetQueue_initQ(&rxFlowObj->rxReadyQ);
    }

    EnetDma_submitRxPktQ(rxFlowObj->hRxFlow, &rxFlowObj->rxFreeQ);
}

uint32_t EnetTestCommon_getMacPortAleMask(EnetTestTaskObj *taskObj)
{
    EnetTestMacPortList_t enabledPorts;
    uint32_t i;
    uint32_t alePortMask;

    alePortMask = 0U;
    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
    for (i = 0U; i < enabledPorts.numMacPorts; i++)
    {
        alePortMask |=
            ENET_BIT(CPSW_ALE_MACPORT_TO_ALEPORT(enabledPorts.macPortList[i]));
    }

    return alePortMask;
}

static int32_t EnetTestCommon_addDefaultHostPortEntry(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setUcastOutArgs;
    CpswAle_SetUcastEntryInArgs setUcastInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memset(&setUcastInArgs, 0, sizeof(setUcastInArgs));
    memcpy(&setUcastInArgs.addr.addr[0U], gDefaultHostMacAddr,
           sizeof(setUcastInArgs.addr.addr));
    setUcastInArgs.addr.vlanId  = 0U;
    setUcastInArgs.info.trunk   = BFALSE;
    setUcastInArgs.info.portNum = 0U;
    setUcastInArgs.info.blocked = BFALSE;
    setUcastInArgs.info.secure  = BFALSE;
    setUcastInArgs.info.super   = BFALSE;
    setUcastInArgs.info.ageable = BFALSE;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &setUcastOutArgs);

    status = Enet_ioctl(stateObj->hEnet,
                        stateObj->coreId,
                        CPSW_ALE_IOCTL_ADD_UCAST,
                        &prms);

    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTestCommon_addDefaultHostPortEntry() failed CPSW_ALE_IOCTL_ADD_UCAST: %d\n", status);
       EnetAppUtils_assert(BFALSE);
    }

    return status;
}


void EnetTestCommon_getDefaultHostMacAddr(uint8_t *macAddr)
{
    memcpy(macAddr, gDefaultHostMacAddr, sizeof(gDefaultHostMacAddr));
}


/* end of file */
