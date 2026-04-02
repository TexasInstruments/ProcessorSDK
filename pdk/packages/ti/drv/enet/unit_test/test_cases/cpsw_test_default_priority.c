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
 * \file     cpsw_test_default_priority.c
 *
 * \brief    This file contains the cpsw_test_default_priority test implementation.
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
#include <ti/drv/ipc/ipc.h>

#include "enet_test_base.h"
#include "enet_test_entry.h"
#include "cpsw_test_default_priority.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define ETH_TEST_MAC_PORT_VLAN_ID_BASE            (100U)
#define ETH_TEST_HOST_PORT_VLAN_ID                (200U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static int32_t EnetTestDefaultPriority_setPolicerAleEntry(EnetTestTaskObj *taskObj,
                                                          uint32_t rxFlowId);

static int32_t EnetTestDefaultPriority_setRxDscpPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestDefaultPriority_setRxPriority(EnetTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetTestDefaultPriority_Run(EnetTestTaskObj *taskObj)
{
    int32_t status      = ENET_SOK;
    uint32_t priority[] = {ENETTESTDEFAULTPRIORITY1_RXFLOWID,
                           ENETTESTDEFAULTPRIORITY2_RXFLOWID,
                           ENETTESTDEFAULTPRIORITY3_RXFLOWID};
    int32_t i;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    Enet_IoctlPrms prms;
    CpswAle_DfltThreadCfg setInArgs;
    CpswAle_DfltThreadCfg getOutArgs;

    ENET_IOCTL_SET_OUT_ARGS(&prms, &getOutArgs);
    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId,
                        CPSW_ALE_IOCTL_GET_DEFAULT_THREADCFG,
                        &prms);
    if (status == ENET_SOK)
    {
        setInArgs.dfltThreadEn         = BTRUE;
        setInArgs.threadId             = taskObj->stateObj.rxFlowObj[ENETTESTDEFAULTPRIORITY_DEFAULTFLOWID].rxFlowIdx;
        setInArgs.priorityOrEn         = BTRUE;
        setInArgs.macPortDfltThreadDis = BFALSE;

        ENET_IOCTL_SET_IN_ARGS(&prms, &setInArgs);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId,
                            CPSW_ALE_IOCTL_SET_DEFAULT_THREADCFG,
                            &prms);
        if (status == ENET_SOK)
        {
            status = EnetTestDefaultPriority_setRxPriority(taskObj);
        }

        if (status == ENET_SOK)
        {
            status = EnetTestDefaultPriority_setRxDscpPriority(taskObj);
        }

        if (status == ENET_SOK)
        {
            if (ENET_CPSW_2G == taskObj->taskCfg->enetType)
            {
                for (i = 0; i < ENET_ARRAYSIZE(priority); i++)
                {
                    status = EnetTestDefaultPriority_setPolicerAleEntry(taskObj, priority[i]);
                   EnetAppUtils_assert(status == ENET_SOK);
                }
            }

            status = EnetTestCommon_Run(taskObj);
        }
    }

    return status;
}

void EnetTestDefaultPriority_setOpenPrms(EnetTestTaskObj *taskObj,
                                         Cpsw_Cfg *pCpswCfg,
                                         EnetUtils_Cfg *pUtilsPrms)
{
    Enet_MacPort i;

    pCpswCfg->aleCfg.policerGlobalCfg.policingEn = BTRUE;
    pCpswCfg->hostPortCfg.passPriorityTaggedUnchanged   = BTRUE;

    pCpswCfg->aleCfg.modeFlags =
        (CPSW_ALE_CFG_MODULE_EN | CPSW_ALE_CFG_UNKNOWN_UCAST_FLOOD2HOST);
    pCpswCfg->aleCfg.vlanCfg.unknownUnregMcastFloodMask = 0x0;
    pCpswCfg->aleCfg.vlanCfg.unknownRegMcastFloodMask   = 0x0;
    pCpswCfg->aleCfg.vlanCfg.unknownVlanMemberListMask  = CPSW_ALE_ALL_PORTS_MASK;
    pCpswCfg->aleCfg.vlanCfg.aleVlanAwareMode           = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.autoLearnWithVlan          = BFALSE;
    pCpswCfg->aleCfg.vlanCfg.cpswVlanAwareMode          = BTRUE;
    pCpswCfg->aleCfg.nwSecCfg.vid0ModeEn                = BTRUE;

    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].learningCfg.noLearn              = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].vlanCfg.dropUntagged             = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.unregMcastFloodMask      = 0x0;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.regMcastFloodMask        = CPSW_ALE_ALL_PORTS_MASK;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.forceUntaggedEgressMask  = 0U;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.noLearnMask              = 0x0;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.vidIngressCheck          = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.limitIPNxtHdr            = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.disallowIPFrag           = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.vlanIdInfo.vlanId        = ETH_TEST_HOST_PORT_VLAN_ID;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.vlanMemberList           = CPSW_ALE_ALL_PORTS_MASK;

    for (i = ENET_MAC_PORT_FIRST; i < CPSW_ALE_NUM_MAC_PORTS; i++)
    {
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].learningCfg.noLearn              = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].vlanCfg.dropUntagged             = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.unregMcastFloodMask      = 0x0;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.regMcastFloodMask        = CPSW_ALE_ALL_PORTS_MASK;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.forceUntaggedEgressMask  = 0U;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.noLearnMask              = 0x0;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vidIngressCheck          = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.limitIPNxtHdr            = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.disallowIPFrag           = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vlanIdInfo.vlanId        = ETH_TEST_MAC_PORT_VLAN_ID_BASE + ENET_MACPORT_NORM(i);
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vlanMemberList           = CPSW_ALE_ALL_PORTS_MASK;
    }

    pCpswCfg->hostPortCfg.vlanCfg.portPri = 7U;
    pCpswCfg->hostPortCfg.vlanCfg.portCfi = 0U;
    pCpswCfg->hostPortCfg.vlanCfg.portVID = ETH_TEST_HOST_PORT_VLAN_ID;
    pCpswCfg->vlanCfg.vlanAware           = BTRUE;

    /* Update the resource usage so that priority OR results in direct mapping
     * to rxFlowIndex allocated starting with default flow.
     * For this the default rx flow index should be 8.
     * This requires the cores MCU_2_0 rxFlowIndex start at 7 as the first
     * rxFlowId is allocated to the reserved flow
     */
   EnetAppUtils_assert(pCpswCfg->resCfg.selfCoreId == IPC_MCU2_0);
    pCpswCfg->resCfg.resPartInfo.numCores = 2;

    pCpswCfg->resCfg.resPartInfo.coreResInfo[0].numRxFlows = 7;
    pCpswCfg->resCfg.resPartInfo.coreResInfo[0].coreId     = IPC_MPU1_0;
    pCpswCfg->resCfg.resPartInfo.coreResInfo[0].numMacAddress = 1;
    pCpswCfg->resCfg.resPartInfo.coreResInfo[0].numTxCh       = 4U;

    pCpswCfg->resCfg.resPartInfo.coreResInfo[1].numRxFlows = 5;
    pCpswCfg->resCfg.resPartInfo.coreResInfo[1].coreId     = IPC_MCU2_0;
    pCpswCfg->resCfg.resPartInfo.coreResInfo[1].numMacAddress = 1;
    pCpswCfg->resCfg.resPartInfo.coreResInfo[1].numTxCh       = 4U;
}

void EnetTestDefaultPriority_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                               Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg      = (CpswMacPort_Cfg*)pLinkArgs->macCfg;

    macCfg->loopbackEn                  = BFALSE;
    macCfg->passPriorityTaggedUnchanged = BTRUE;
    macCfg->vlanCfg.portPri = 7U; // ENET_MACPORT_NORM(portNum);
    macCfg->vlanCfg.portCfi = 0U;
    macCfg->vlanCfg.portVID = ETH_TEST_MAC_PORT_VLAN_ID_BASE + ENET_MACPORT_NORM(portNum);
}

static int32_t EnetTestDefaultPriority_setPolicerAleEntry(EnetTestTaskObj *taskObj,
                                                          uint32_t rxFlowId)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_SetPolicerEntryOutArgs setPolicerOutArgs;
    CpswAle_SetPolicerEntryInArgs setPolicerInArgs;

    EnetTestStateObj *stateObj = &taskObj->stateObj;

    setPolicerInArgs.policerMatch.policerMatchEnMask = CPSW_ALE_POLICER_MATCH_PRIORITY;
    setPolicerInArgs.policerMatch.priority               = rxFlowId;
    setPolicerInArgs.threadIdEn                          = BTRUE;
    setPolicerInArgs.threadId                            = taskObj->stateObj.rxFlowObj[rxFlowId].rxFlowIdx;
    setPolicerInArgs.peakRateInBitsPerSec                = 0;
    setPolicerInArgs.commitRateInBitsPerSec              = 0;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerInArgs, &setPolicerOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_POLICER,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_openCpsw() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",
                           status);
    }

    setPolicerInArgs.policerMatch.policerMatchEnMask = CPSW_ALE_POLICER_MATCH_PRIORITY;
    setPolicerInArgs.policerMatch.priority               = rxFlowId + 4;
    setPolicerInArgs.threadIdEn                          = BTRUE;
    setPolicerInArgs.threadId                            = taskObj->stateObj.rxFlowObj[rxFlowId].rxFlowIdx;
    setPolicerInArgs.peakRateInBitsPerSec                = 0;
    setPolicerInArgs.commitRateInBitsPerSec              = 0;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerInArgs, &setPolicerOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_POLICER,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_openCpsw() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",
                           status);
    }

    return status;
}

static int32_t EnetTestDefaultPriority_setRxDscpPriority(EnetTestTaskObj *taskObj)
{
    EnetPort_DscpPriorityMap setHostInArgs;
    EnetPort_DscpPriorityMap getHostOutArgs;
    Enet_IoctlPrms hostPrms;
    EnetMacPort_SetIngressDscpPriorityMapInArgs setMacInArgs;
    EnetMacPort_GenericInArgs getMacInArgs;
    EnetPort_DscpPriorityMap getMacOutArgs;
    Enet_IoctlPrms macPrms;
    uint32_t i;
    int32_t status;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint32_t portIter;
    EnetTestMacPortList_t enabledPorts;

    setHostInArgs.dscpIPv4En = BTRUE;
    setHostInArgs.dscpIPv6En = BTRUE;
    for (i = 0U; i < ENET_ARRAYSIZE(setHostInArgs.tosMap); i++)
    {
        setHostInArgs.tosMap[i] = (i % 8U);
    }

    ENET_IOCTL_SET_IN_ARGS(&hostPrms, &setHostInArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_HOSTPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP,
                        &hostPrms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_openCpsw() failed ENET_HOSTPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP: %d\n",
                           status);
    }

    ENET_IOCTL_SET_OUT_ARGS(&hostPrms, &getHostOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_HOSTPORT_IOCTL_GET_INGRESS_DSCP_PRI_MAP,
                        &hostPrms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_openCpsw() failed ENET_HOSTPORT_IOCTL_GET_INGRESS_DSCP_PRI_MAP: %d\n",
                           status);
    }
    else
    {
       EnetAppUtils_assert(setHostInArgs.dscpIPv4En ==
                            getHostOutArgs.dscpIPv4En);
       EnetAppUtils_assert(setHostInArgs.dscpIPv6En ==
                            getHostOutArgs.dscpIPv6En);
        for (i = 0U; i < ENET_ARRAYSIZE(setHostInArgs.tosMap); i++)
        {
           EnetAppUtils_assert(setHostInArgs.tosMap[i] ==
                                getHostOutArgs.tosMap[i]);
        }
    }

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
    /* Set Mac Port DSCP priority mapping for all mac ports */
    for (portIter = 0; portIter < enabledPorts.numMacPorts; portIter++)
    {
        setMacInArgs.macPort                     = enabledPorts.macPortList[portIter];
        setMacInArgs.dscpPriorityMap.dscpIPv4En = BTRUE;
        setMacInArgs.dscpPriorityMap.dscpIPv6En = BTRUE;
        for (i = 0U; i < ENET_ARRAYSIZE(setMacInArgs.dscpPriorityMap.tosMap); i++)
        {
            setMacInArgs.dscpPriorityMap.tosMap[i] = (i % taskObj->taskCfg->numRxFlow);
        }

        ENET_IOCTL_SET_IN_ARGS(&macPrms, &setMacInArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_MACPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP,
                            &macPrms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTest_openCpsw() failed ENET_MACPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP: %d\n",
                               status);
        }

        getMacInArgs.macPort = enabledPorts.macPortList[portIter];
        ENET_IOCTL_SET_INOUT_ARGS(&macPrms, &getMacInArgs, &getMacOutArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_MACPORT_IOCTL_GET_INGRESS_DSCP_PRI_MAP,
                            &macPrms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTest_openCpsw() failed ENET_MACPORT_IOCTL_GET_INGRESS_DSCP_PRI_MAP: %d\n",
                               status);
        }
        else
        {
           EnetAppUtils_assert(setMacInArgs.dscpPriorityMap.dscpIPv4En ==
                                getMacOutArgs.dscpIPv4En);
           EnetAppUtils_assert(setMacInArgs.dscpPriorityMap.dscpIPv6En ==
                                getMacOutArgs.dscpIPv6En);
            for (i = 0U; i < ENET_ARRAYSIZE(setMacInArgs.dscpPriorityMap.tosMap); i++)
            {
               EnetAppUtils_assert(setMacInArgs.dscpPriorityMap.tosMap[i] ==
                                    getMacOutArgs.tosMap[i]);
            }
        }
    }

    return status;
}

static int32_t EnetTestDefaultPriority_setRxPriority(EnetTestTaskObj *taskObj)
{
    EnetPort_PriorityMap setHostInArgs;
    Enet_IoctlPrms hostPrms;
    EnetMacPort_SetPriorityRegenMapInArgs setMacInArgs;
    Enet_IoctlPrms macPrms;
    uint32_t i;
    int32_t status;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint32_t portIter;
    EnetTestMacPortList_t enabledPorts;

    for (i = 0; i < ENET_ARRAYSIZE(setHostInArgs.priorityMap); i++)
    {
        setHostInArgs.priorityMap[i] = i;
    }

    ENET_IOCTL_SET_IN_ARGS(&hostPrms, &setHostInArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_HOSTPORT_IOCTL_SET_PRI_REGEN_MAP,
                        &hostPrms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_openCpsw() failed ENET_HOSTPORT_IOCTL_SET_PRI_REGEN_MAP: %d\n",
                           status);
    }

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
    /* Set Mac Port DSCP priority mapping for all mac ports */
    for (portIter = 0; portIter < enabledPorts.numMacPorts; portIter++)
    {
        setMacInArgs.macPort = enabledPorts.macPortList[portIter];
        for (i = 0; i < ENET_ARRAYSIZE(setMacInArgs.priorityRegenMap.priorityMap); i++)
        {
            setMacInArgs.priorityRegenMap.priorityMap[i] = i;
        }

        ENET_IOCTL_SET_IN_ARGS(&macPrms, &setMacInArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_MACPORT_IOCTL_SET_PRI_REGEN_MAP,
                            &macPrms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTest_openCpsw() failed ENET_MACPORT_IOCTL_SET_PRI_REGEN_MAP: %d\n",
                               status);
        }
    }

    return status;
}

int32_t EnetTestDefaultPriority_processRxPkt(EnetTestTaskObj *taskObj,
                                             uint32_t rxFlowCfgId,
                                             uint32_t pktNum,
                                             EnetDma_Pkt *pktInfo,
                                             bool *testComplete)
{
    int32_t status = ENET_SOK;
    uint32_t rxFlowId;
    EthVlanFrame *frame;
    uint32_t pcp;

    EnetAppUtils_assert(pktInfo->sgList.numScatterSegments == 1U);
    /* Consume the packet by just printing its content */
    frame = (EthVlanFrame *)pktInfo->sgList.list[0].bufPtr;

    pcp = (((Enet_htons(frame->hdr.tci)) >> 13) & 0x7);

    rxFlowId = taskObj->stateObj.rxFlowObj[rxFlowCfgId].rxFlowIdx;
    if ((rxFlowId & (ENET_PRI_NUM - 1)) == pcp)
    {
        status = ENET_SOK;
    }
    else
    {
        status = ENET_EFAIL;
    }

    *testComplete = BFALSE;
    return status;
}

const uint8_t gEnetTestDefaultPriorityIPPkt[] =
{
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe8, 0x84, 0x4c, 0xeb, 0x64, 0x93, 0x08, 0x00, 0x45, 0x04,
    0x00, 0x28, 0x12, 0x34, 0x40, 0x00, 0xff, 0x06, 0x94, 0x97, 0xac, 0x18, 0xbe, 0x5e, 0xac, 0x18,
    0xbe, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x64, 0x50, 0x00,
    0x0f, 0xa0, 0xca, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#define ENET_TEST_DEFAULTPRIORITY_DSCP_OFFSET  (15)
#define ENET_TEST_DEFAULTPRIORITY_TOS2DSCP(x)  ((x) << 2)
#define ENET_TEST_DEFAULTPRIORITY_MAC_OFFSET   (11)
int32_t EnetTestDefaultPriority_setTxPktInfo(EnetTestTaskObj *taskObj,
                                             uint32_t txChIndex,
                                             uint32_t pktNum,
                                             EnetDma_Pkt *pktInfo,
                                             bool *testComplete)
{
    memcpy(pktInfo->sgList.list[0].bufPtr, gEnetTestDefaultPriorityIPPkt, sizeof(gEnetTestDefaultPriorityIPPkt));
    pktInfo->sgList.list[0].bufPtr[ENET_TEST_DEFAULTPRIORITY_DSCP_OFFSET] = ENET_TEST_DEFAULTPRIORITY_TOS2DSCP(txChIndex);
    pktInfo->sgList.list[0].bufPtr[ENET_TEST_DEFAULTPRIORITY_MAC_OFFSET]  = gEnetTestDefaultPriorityIPPkt[ENET_TEST_DEFAULTPRIORITY_MAC_OFFSET] + txChIndex;

    pktInfo->sgList.list[0].segmentFilledLen = sizeof(gEnetTestDefaultPriorityIPPkt);
    *testComplete       = BFALSE;
    return ENET_SOK;
}
