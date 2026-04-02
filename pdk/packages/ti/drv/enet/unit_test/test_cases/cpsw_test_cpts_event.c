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
 * \file     cpsw_test_cpts_event.c
 *
 * \brief    This file contains the cpts event test implementation.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <ti/drv/uart/UART_stdio.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_ethutils.h>

#include <ti/osal/osal.h>

#include <ti/board/board.h>

#include "enet_test_entry.h"
#include "enet_test_base.h"
#include "cpsw_test_cpts_event.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define ENET_TEST_CPTS_EVENT_MEM_COUNT                          (16U)
#define ENET_TEST_CPTS_EVENT_TS_LOAD_VAL                        (90000000000U)
#define ENET_TEST_CPTS_EVENT_TS_NUDGE                           (3U)
#define ENET_TEST_CPTS_EVENT_ETH_TX_SEQ_ID                      (7600U)
#define ENET_TEST_CPTS_EVENT_ETH_RX_SEQ_ID                      (7606U)
#define ENET_TEST_CPTS_EVENT_HOST_TX_SEQ_ID                     (100U)
#define ENET_TEST_CPTS_EVENT_HOST_TX_MSG_TYPE                   (1U)
#define ENET_TEST_CPTS_EVENT_HOST_TX_DOMAIN                     (5U)
#define ENET_TEST_CPTS_EVENT_GENF_COMPARE_VAL                   (95000000000U)
#define ENET_TEST_CPTS_EVENT_GENF_COMPARE_VAL_2                 (98000000000U)
#define ENET_TEST_CPTS_EVENT_GENF_LENGTH_VAL                    (1000000000U)
#define ENET_TEST_CPTS_EVENT_HW_PUSH_INSTANCE_NUM               (CPSW_CPTS_HWPUSH_1)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                         Function Declarations                              */
/* ========================================================================== */

void CptsTestCptsEvent_HwPushNotifyCallback(void *cbArg,
                                            CpswCpts_HwPush hwPushNum);

void Cpts_EventNotifyCallback(void *handle,
                              CpswCpts_Event *eventInfo);

static void EnetTestCptsEvent_setDefaultPortEventPrms(CpswMacPort_TsEventCfg *tsPortEventCfg);

static int32_t EnetTestCptsEvent_setTimeSyncRouter(Enet_Type enetType);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

CpswCpts_Event cptsEventMem[ENET_TEST_CPTS_EVENT_MEM_COUNT];

uint8_t cptsPtpPkt[] = {0x00U, 0x02U, 0x00U, 0x2cU, 0x00U, 0x00U, 0x00U, 0x00U,
                        0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
                        0x00U, 0x00U, 0x00U, 0x00U, 0xfcU, 0xafU, 0x6aU, 0xffU,
                        0xfeU, 0x02U, 0x52U, 0x15U, 0x00U, 0x01U, 0x1dU, 0xb0U,
                        0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x06U, 0x94U, 0x01U,
                        0x02U, 0xc7U, 0x55U, 0xb7U};

SemaphoreP_Handle hPktRxSem;
volatile uint8_t hwPushCount[8U] = { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
void EnetTestCptsEvent_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                         Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg      = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn           = BFALSE;
}

void EnetTestCptsEvent_setOpenPrms(EnetTestTaskObj *taskObj,
                                   Cpsw_Cfg *pCpswCfg,
                                   EnetUtils_Cfg *pUtilsPrms)
{
    pCpswCfg->cptsCfg.hostRxTsEn = BTRUE;
    pCpswCfg->cptsCfg.cptsRftClkFreq = CPSW_CPTS_RFTCLK_FREQ_500MHZ;
}

int32_t EnetTestCptsEvent_setTxPktInfo(EnetTestTaskObj *taskObj,
                                       uint32_t txChIndex,
                                       uint32_t pktNum,
                                       EnetDma_Pkt *pktInfo,
                                       bool *testComplete)
{
    EthFrame *frame;
    /* Broadcast address */
    uint8_t bcastAddr[ENET_MAC_ADDR_LEN] =
    {
        0xffU, 0xffU, 0xffU, 0xffU, 0xffU, 0xffU
    };
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    frame = (EthFrame *)pktInfo->sgList.list[0].bufPtr;
    memcpy(frame->hdr.dstMac, bcastAddr, ENET_MAC_ADDR_LEN);
    memcpy(frame->hdr.srcMac, &stateObj->hostMacAddr[0U], ENET_MAC_ADDR_LEN);
    frame->hdr.etherType = Enet_htons(ENET_ETHERTYPE_PTP);
    memcpy(&frame->payload[0U], &cptsPtpPkt[0U], sizeof(cptsPtpPkt));
    /* Change Sequence Id for every packet */
    frame->payload[31U] += pktNum;
    pktInfo->sgList.list[0].segmentFilledLen  = taskObj->stateObj.txChCfgInfo[txChIndex]->maxTxPktLen +
                           sizeof(EthFrameHeader);

    if (pktNum == 2U)
    {
        memset(&frame->payload[0U], 0xABU, sizeof(cptsPtpPkt));
        pktInfo->tsInfo.enableHostTxTs = BTRUE;
        pktInfo->tsInfo.txPktSeqId = ENET_TEST_CPTS_EVENT_HOST_TX_SEQ_ID;
        pktInfo->tsInfo.txPktMsgType = ENET_TEST_CPTS_EVENT_HOST_TX_MSG_TYPE;
        pktInfo->tsInfo.txPktDomain = ENET_TEST_CPTS_EVENT_HOST_TX_DOMAIN;
    }

    pktInfo->appPriv = stateObj;
    *testComplete = BFALSE;

    return ENET_SOK;
}

int32_t EnetTestCptsEvent_Run(EnetTestTaskObj *taskObj)
{
    int32_t status = ENET_SOK;
    uint64_t tsVal = 0U, tsVal1 = 0U, tsVal2 = 0U;
    Enet_IoctlPrms prms;
    CpswCpts_RegisterStackInArgs regStackInArgs;
    CpswMacPort_EnableTsEventInArgs enableTsEventInArgs;
    CpswCpts_Event lookupEventInArgs;
    CpswCpts_Event lookupEventOutArgs;
    CpswCpts_RegisterHwPushCbInArgs hwPushCbInArgs;
    CpswCpts_HwPush hwPushNum;
    EnetTimeSync_TimestampAdj adjTsInArgs;
    int32_t nudge = 0;
    uint64_t loadTsVal = 0U;
    CpswCpts_OutputBitSel tsSyncOutputSel;
    CpswCpts_SetFxnGenInArgs setGenFInArgs;
    SemaphoreP_Params semParams;

    EnetTestCommon_waitForPortLink(taskObj);
   EnetAppUtils_assert(taskObj->taskCfg->numTxCh == 1U);
   EnetAppUtils_assert(taskObj->taskCfg->numRxFlow == 1U);

    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    hPktRxSem = SemaphoreP_create(0, &semParams);
   EnetAppUtils_assert(NULL != hPktRxSem);

    /* Configure Timesync Router to enable Hardware push events */
    EnetTestCptsEvent_setTimeSyncRouter(taskObj->taskCfg->enetType);

    regStackInArgs.eventNotifyCb = Cpts_EventNotifyCallback;
    regStackInArgs.eventNotifyCbArg = (void *)taskObj;
    /* Register a stack with call back */
    ENET_IOCTL_SET_IN_ARGS(&prms, &regStackInArgs);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        CPSW_CPTS_IOCTL_REGISTER_HWPUSH_CALLBACK,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);

    /* Software Time stamp Push event */
    ENET_IOCTL_SET_OUT_ARGS(&prms, &tsVal);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        ENET_TIMESYNC_IOCTL_GET_CURRENT_TIMESTAMP,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
   EnetAppUtils_print("\nTimeStamp Value before Nudge: %lld \n", tsVal);

    /* TS Nudge */
    nudge = ENET_TEST_CPTS_EVENT_TS_NUDGE;
    ENET_IOCTL_SET_IN_ARGS(&prms, &nudge);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        CPSW_CPTS_IOCTL_SET_TS_NUDGE,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
    ENET_IOCTL_SET_OUT_ARGS(&prms, &tsVal);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        ENET_TIMESYNC_IOCTL_GET_CURRENT_TIMESTAMP,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
   EnetAppUtils_print("TimeStamp Value after Nudge: %lld \n", tsVal);
   EnetAppUtils_print("TimeStamp Nudge successful \n");

    /* Load Time Stamp Test */
    loadTsVal = ENET_TEST_CPTS_EVENT_TS_LOAD_VAL;
    ENET_IOCTL_SET_IN_ARGS(&prms, &loadTsVal);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        ENET_TIMESYNC_IOCTL_SET_TIMESTAMP,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
    ENET_IOCTL_SET_OUT_ARGS(&prms, &tsVal);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        ENET_TIMESYNC_IOCTL_GET_CURRENT_TIMESTAMP,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
   EnetAppUtils_print("\nTimeStamp Value after Load TS: %lld \n", tsVal);
   EnetAppUtils_assert(tsVal > ENET_TEST_CPTS_EVENT_TS_LOAD_VAL);
   EnetAppUtils_print("Load TimeStamp Value successful \n");

    /* TS PPM test: -500 PPM (8ms adjustment in 4 secs interval) */
    adjTsInArgs.adjValInNsecs   = -8000000;
    adjTsInArgs.intervalInNsecs = 4000000000U;
    EnetAppUtils_print("\nTimeStamp adj set to %d nsecs in %u nsecs interval\n",
                       adjTsInArgs.adjValInNsecs, adjTsInArgs.intervalInNsecs);

    ENET_IOCTL_SET_IN_ARGS(&prms, &adjTsInArgs);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        ENET_TIMESYNC_IOCTL_ADJUST_TIMESTAMP,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);

    /* Check for Time stamps after enabling PPM */
    ENET_IOCTL_SET_OUT_ARGS(&prms, &tsVal1);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        ENET_TIMESYNC_IOCTL_GET_CURRENT_TIMESTAMP,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
    ENET_IOCTL_SET_OUT_ARGS(&prms, &tsVal2);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        ENET_TIMESYNC_IOCTL_GET_CURRENT_TIMESTAMP,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
   EnetAppUtils_print("TimeStamp Value 1: %lld \n", tsVal1);
   EnetAppUtils_print("TimeStamp Value 2: %lld \n", tsVal2);

    /* Set Ts PPM value back to zero */
    adjTsInArgs.adjValInNsecs   = 0;
    adjTsInArgs.intervalInNsecs = 4000000000U;

    ENET_IOCTL_SET_IN_ARGS(&prms, &adjTsInArgs);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        ENET_TIMESYNC_IOCTL_ADJUST_TIMESTAMP,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
    ENET_IOCTL_SET_OUT_ARGS(&prms, &tsVal1);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        ENET_TIMESYNC_IOCTL_GET_CURRENT_TIMESTAMP,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
    ENET_IOCTL_SET_OUT_ARGS(&prms, &tsVal2);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        ENET_TIMESYNC_IOCTL_GET_CURRENT_TIMESTAMP,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
   EnetAppUtils_print("\nTimeStamp PPM is removed \n");
   EnetAppUtils_print("TimeStamp Value 1: %lld \n", tsVal1);
   EnetAppUtils_print("TimeStamp Value 2: %lld \n", tsVal2);

    /* Enable PTP over Bare Ethernet - Annex F (IEEE802.3) */
    enableTsEventInArgs.macPort = ENET_MAC_PORT_1;
    EnetTestCptsEvent_setDefaultPortEventPrms(&enableTsEventInArgs.tsEventCfg);

    ENET_IOCTL_SET_IN_ARGS(&prms, &enableTsEventInArgs);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        CPSW_MACPORT_IOCTL_ENABLE_CPTS_EVENT,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);

    status = EnetTestCommon_pktTxRx(taskObj);
    SemaphoreP_pend(hPktRxSem, SemaphoreP_WAIT_FOREVER);

    /* Event Lookup for Tx Event */
    lookupEventInArgs.eventType = CPSW_CPTS_EVENTTYPE_ETH_TRANSMIT;
    lookupEventInArgs.hwPushNum = CPSW_CPTS_HWPUSH_INVALID;
    lookupEventInArgs.msgType = ENET_TIMESYNC_MESSAGE_SYNC;
    lookupEventInArgs.portNum = ENET_MAC_PORT_1;
    lookupEventInArgs.seqId = ENET_TEST_CPTS_EVENT_ETH_TX_SEQ_ID;
    lookupEventInArgs.domain = 0U;
   EnetAppUtils_print("\nTx Event Lookup for sequence Id: %d", lookupEventInArgs.seqId);
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &lookupEventInArgs, &lookupEventOutArgs);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        CPSW_CPTS_IOCTL_LOOKUP_EVENT,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
    /* Utilize Call back function to print Event Information */
    Cpts_EventNotifyCallback(NULL, &lookupEventOutArgs);

    /* Event Lookup for Rx Event */
    lookupEventInArgs.eventType = CPSW_CPTS_EVENTTYPE_ETH_RECEIVE;
    lookupEventInArgs.hwPushNum = CPSW_CPTS_HWPUSH_INVALID;
    lookupEventInArgs.msgType = ENET_TIMESYNC_MESSAGE_SYNC;
    lookupEventInArgs.portNum = ENET_MAC_PORT_1;
    lookupEventInArgs.seqId = ENET_TEST_CPTS_EVENT_ETH_RX_SEQ_ID;
    lookupEventInArgs.domain = 0U;
   EnetAppUtils_print("\nRx Event Lookup for sequence Id: %d", lookupEventInArgs.seqId);
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &lookupEventInArgs, &lookupEventOutArgs);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        CPSW_CPTS_IOCTL_LOOKUP_EVENT,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
    /* Utilize Call back function to print Event Information */
    Cpts_EventNotifyCallback(NULL, &lookupEventOutArgs);

    /* Register hardware push 1 callback */
    hwPushCbInArgs.hwPushNum = ENET_TEST_CPTS_EVENT_HW_PUSH_INSTANCE_NUM;
    hwPushCbInArgs.hwPushNotifyCb = CptsTestCptsEvent_HwPushNotifyCallback;
    hwPushCbInArgs.hwPushNotifyCbArg = (void *)taskObj;
    ENET_IOCTL_SET_IN_ARGS(&prms, &hwPushCbInArgs);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        CPSW_CPTS_IOCTL_REGISTER_HWPUSH_CALLBACK,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);

    /* Enable TS_OUTPUT_BIT to get Hardware push events fired */
    tsSyncOutputSel = CPSW_CPTS_TS_OUTPUT_BIT_31;
    ENET_IOCTL_SET_IN_ARGS(&prms, &tsSyncOutputSel);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        CPSW_CPTS_IOCTL_SELECT_TS_OUTPUT_BIT,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
    while (hwPushCount[0U] == 0U);
    while (hwPushCount[1U] == 0U);
    while (hwPushCount[2U] == 0U);
    while (hwPushCount[3U] == 0U);

   EnetAppUtils_print("\nHardware Push events test successful \n");

    /* Disable TS_OUTPUT_BIT to stop Hardware push events */
    tsSyncOutputSel = CPSW_CPTS_TS_OUTPUT_BIT_DISABLED;
    ENET_IOCTL_SET_IN_ARGS(&prms, &tsSyncOutputSel);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        CPSW_CPTS_IOCTL_SELECT_TS_OUTPUT_BIT,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);

    /* Enable GenF0 and GenF1 */

    /* Load Time Stamp to bring the time stamp value in the comparison value range */
    loadTsVal = ENET_TEST_CPTS_EVENT_TS_LOAD_VAL;
    ENET_IOCTL_SET_IN_ARGS(&prms, &loadTsVal);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        ENET_TIMESYNC_IOCTL_SET_TIMESTAMP,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);

    /* Configure GENF0  */
    setGenFInArgs.index   = 0U;
    setGenFInArgs.length  = ENET_TEST_CPTS_EVENT_GENF_LENGTH_VAL;
    setGenFInArgs.compare = ENET_TEST_CPTS_EVENT_GENF_COMPARE_VAL;
    setGenFInArgs.polarityInv = BTRUE;
    setGenFInArgs.ppmVal  = 0U;
    setGenFInArgs.ppmDir  = ENET_TIMESYNC_ADJDIR_INCREASE;
    setGenFInArgs.ppmMode = ENET_TIMESYNC_ADJMODE_DISABLE;
    ENET_IOCTL_SET_IN_ARGS(&prms, &setGenFInArgs);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        CPSW_CPTS_IOCTL_SET_GENF,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
    /* Configure GENF1  */
    setGenFInArgs.index   = 1U;
    setGenFInArgs.length  = ENET_TEST_CPTS_EVENT_GENF_LENGTH_VAL;
    setGenFInArgs.compare = ENET_TEST_CPTS_EVENT_GENF_COMPARE_VAL_2;
    setGenFInArgs.polarityInv = BTRUE;
    setGenFInArgs.ppmVal  = 0U;
    setGenFInArgs.ppmDir  = ENET_TIMESYNC_ADJDIR_INCREASE;
    setGenFInArgs.ppmMode = ENET_TIMESYNC_ADJMODE_DISABLE;
    ENET_IOCTL_SET_IN_ARGS(&prms, &setGenFInArgs);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        CPSW_CPTS_IOCTL_SET_GENF,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);
    while (hwPushCount[4U] == 0U);
    while (hwPushCount[5U] == 0U);
    while (hwPushCount[6U] == 0U);
    while (hwPushCount[7U] == 0U);

    /* Un-register hardware push call back */
    hwPushNum = ENET_TEST_CPTS_EVENT_HW_PUSH_INSTANCE_NUM;
    ENET_IOCTL_SET_IN_ARGS(&prms, &hwPushNum);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        CPSW_CPTS_IOCTL_UNREGISTER_HWPUSH_CALLBACK,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);

    /* Print CPTS event statistics */
    ENET_IOCTL_SET_NO_ARGS(&prms);
    status = Enet_ioctl(taskObj->stateObj.hEnet,
                        taskObj->stateObj.coreId,
                        ENET_TIMESYNC_IOCTL_PRINT_STATS,
                        &prms);
   EnetAppUtils_assert(status == ENET_SOK);

   /* Disable GenF0 and GenF1 */
   /* Disable GenF0 */
   setGenFInArgs.index  = 0U;
   setGenFInArgs.length = 0U;
   ENET_IOCTL_SET_IN_ARGS(&prms, &setGenFInArgs);
   status = Enet_ioctl(taskObj->stateObj.hEnet,
                       taskObj->stateObj.coreId,
                       CPSW_CPTS_IOCTL_SET_GENF,
                       &prms);
   /* Not supported is not an error, so set the status
    * to ENET_SOK */
   if (status == ENET_ENOTSUPPORTED)
   {
       status = ENET_SOK;
   }
  EnetAppUtils_assert(status == ENET_SOK);

   /* Disable GenF1 */
   setGenFInArgs.index  = 1U;
   setGenFInArgs.length = 0U;
   ENET_IOCTL_SET_IN_ARGS(&prms, &setGenFInArgs);
   status = Enet_ioctl(taskObj->stateObj.hEnet,
                       taskObj->stateObj.coreId,
                       CPSW_CPTS_IOCTL_SET_GENF,
                       &prms);
   /* Not supported is not an error, so set the status
    * to ENET_SOK */
   if (status == ENET_ENOTSUPPORTED)
   {
       status = ENET_SOK;
   }
  EnetAppUtils_assert(status == ENET_SOK);

    if (hPktRxSem != NULL)
    {
        SemaphoreP_delete(hPktRxSem);
    }

   return status;
}

void CptsTestCptsEvent_HwPushNotifyCallback(void *cbArg, CpswCpts_HwPush hwPushNum)
{
    if (hwPushNum == ENET_TEST_CPTS_EVENT_HW_PUSH_INSTANCE_NUM)
    {
       EnetAppUtils_print("\nHardware push notify callback test passed \n");
    }
    else
    {
       EnetAppUtils_print("\nHardware push notify callback test failed \n");
    }
}

void Cpts_EventNotifyCallback(void *handle,
                              CpswCpts_Event *eventInfo)
{
    static uint32_t ptpRxCount = 0U;

    /* Print Event Information */
    if (eventInfo != NULL)
    {
       EnetAppUtils_print("\n=================================\n");
       EnetAppUtils_print("    CPTS Event Information \n");
       EnetAppUtils_print("=================================\n");

        switch (eventInfo->eventType)
        {
            case CPSW_CPTS_EVENTTYPE_TS_PUSH:
            {
               EnetAppUtils_print("Event Type          : Software Time Stamp Push \n");
            }
            break;

            case CPSW_CPTS_EVENTTYPE_HW_TS_PUSH:
            {
               EnetAppUtils_print("Event Type          : Hardware Time Stamp Push \n");
               EnetAppUtils_print("Hardware Push Number: %d \n", eventInfo->hwPushNum);
                hwPushCount[CPSW_CPTS_HWPUSH_NORM(eventInfo->hwPushNum)]++;
            }
            break;

            case CPSW_CPTS_EVENTTYPE_ETH_RECEIVE:
            {
               EnetAppUtils_print("Event Type          : Ethernet Receive \n");
               EnetAppUtils_print("Message Type        : %d \n", eventInfo->msgType);
               EnetAppUtils_print("Sequence Id         : %d \n", eventInfo->seqId);
               EnetAppUtils_print("Port Number         : %d \n", eventInfo->portNum);
               EnetAppUtils_print("Domain              : %d \n", eventInfo->domain);
                if (2U == ++ptpRxCount)
                {
                    SemaphoreP_post(hPktRxSem);
                }
            }
            break;

            case CPSW_CPTS_EVENTTYPE_ETH_TRANSMIT:
            {
               EnetAppUtils_print("Event Type          : Ethernet Transmit \n");
               EnetAppUtils_print("Message Type        : %d \n", eventInfo->msgType);
               EnetAppUtils_print("Sequence Id         : %d \n", eventInfo->seqId);
               EnetAppUtils_print("Port Number         : %d \n", eventInfo->portNum);
               EnetAppUtils_print("Domain              : %d \n", eventInfo->domain);
            }
            break;

            case CPSW_CPTS_EVENTTYPE_TS_COMP:
            {
               EnetAppUtils_print("Event Type          : Time Stamp Compare \n");
            }
            break;

            case CPSW_CPTS_EVENTTYPE_TS_HOST_TX:
            {
               EnetAppUtils_print("Event Type          : Ethernet Host Transmit \n");
               EnetAppUtils_print("Message Type        : %d \n", eventInfo->msgType);
               EnetAppUtils_print("Sequence Id         : %d \n", eventInfo->seqId);
               EnetAppUtils_print("Domain              : %d \n", eventInfo->domain);
            }
            break;

            default:
            {
               EnetAppUtils_print("Event Type          : Invalid \n");
            }
            break;
        }

        /* Print Time stamp value */
       EnetAppUtils_print("Time Stamp Value    : %lld \n", eventInfo->tsVal);
    }
}

static void EnetTestCptsEvent_setDefaultPortEventPrms(CpswMacPort_TsEventCfg *tsPortEventCfg)
{
    tsPortEventCfg->commonPortIpCfg.ttlNonzeroEn = BTRUE;
    tsPortEventCfg->commonPortIpCfg.tsIp107En = BFALSE;
    tsPortEventCfg->commonPortIpCfg.tsIp129En = BFALSE;
    tsPortEventCfg->commonPortIpCfg.tsIp130En = BFALSE;
    tsPortEventCfg->commonPortIpCfg.tsIp131En = BFALSE;
    tsPortEventCfg->commonPortIpCfg.tsIp132En = BFALSE;
    tsPortEventCfg->commonPortIpCfg.tsIp132En = BFALSE; // can we remove this as - Duplicate
    tsPortEventCfg->commonPortIpCfg.tsPort320En = BFALSE;
    tsPortEventCfg->commonPortIpCfg.unicastEn = BTRUE;
    tsPortEventCfg->domainOffset = 4U;
    tsPortEventCfg->ltype2En = BFALSE;
    tsPortEventCfg->rxAnnexDEn = BTRUE;
    tsPortEventCfg->rxAnnexEEn = BTRUE;
    tsPortEventCfg->rxAnnexFEn = BTRUE;
    tsPortEventCfg->txAnnexDEn = BTRUE;
    tsPortEventCfg->txAnnexEEn = BTRUE;
    tsPortEventCfg->txAnnexFEn = BTRUE;
    tsPortEventCfg->txHostTsEn = BTRUE;
    tsPortEventCfg->mcastType = 0U;
    tsPortEventCfg->messageType = 0xFFFFU;
    tsPortEventCfg->rxVlanType  = ENET_MACPORT_VLAN_TYPE_NONE;
    tsPortEventCfg->seqIdOffset = 30U;
    tsPortEventCfg->txVlanType  = ENET_MACPORT_VLAN_TYPE_NONE;
    tsPortEventCfg->vlanLType1  = 0U;
    tsPortEventCfg->vlanLType2  = 0U;
}

static int32_t EnetTestCptsEvent_setTimeSyncRouter(Enet_Type enetType)
{
    int32_t status = ENET_SOK;

    status =EnetAppUtils_setTimeSyncRouter(enetType,
                                            0,
                                            CSLR_TIMESYNC_INTRTR0_IN_CPSW0_CPTS_SYNC_0,
                                            ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW1_PUSH);
    EnetAppUtils_assert(status == ENET_SOK);

    status =EnetAppUtils_setTimeSyncRouter(enetType,
                                            0,
                                            CSLR_TIMESYNC_INTRTR0_IN_CPSW0_CPTS_SYNC_0,
                                            ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW2_PUSH);
    EnetAppUtils_assert(status == ENET_SOK);

    status =EnetAppUtils_setTimeSyncRouter(enetType,
                                            0,
                                            CSLR_TIMESYNC_INTRTR0_IN_CPSW0_CPTS_SYNC_0,
                                            ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW3_PUSH);
    EnetAppUtils_assert(status == ENET_SOK);

    status =EnetAppUtils_setTimeSyncRouter(enetType,
                                            0,
                                            CSLR_TIMESYNC_INTRTR0_IN_CPSW0_CPTS_SYNC_0,
                                            ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW4_PUSH);
    EnetAppUtils_assert(status == ENET_SOK);

    status =EnetAppUtils_setTimeSyncRouter(enetType,
                                            0,
                                            CSLR_TIMESYNC_INTRTR0_IN_CPSW0_CPTS_GENF0_0,
                                            ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW5_PUSH);
    EnetAppUtils_assert(status == ENET_SOK);

    status =EnetAppUtils_setTimeSyncRouter(enetType,
                                            0,
                                            CSLR_TIMESYNC_INTRTR0_IN_CPSW0_CPTS_GENF0_0,
                                            ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW6_PUSH);
    EnetAppUtils_assert(status == ENET_SOK);

    status =EnetAppUtils_setTimeSyncRouter(enetType,
                                            0,
                                            CSLR_TIMESYNC_INTRTR0_IN_CPSW0_CPTS_GENF1_0,
                                            ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW7_PUSH);
   EnetAppUtils_assert(status == ENET_SOK);

    status =EnetAppUtils_setTimeSyncRouter(enetType,
                                            0,
                                            CSLR_TIMESYNC_INTRTR0_IN_CPSW0_CPTS_GENF1_0,
                                            ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW8_PUSH);
    EnetAppUtils_assert(status == ENET_SOK);

    return status;
}
