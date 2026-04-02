/*
 *  Copyright (c) Texas Instruments Incorporated 2023-24
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
 *  \file udma_test_csl_udmap.c
 *
 *  \brief File containing test cases for csl udmap APIs.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <udma_test.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define CSL_UDMAP_NUM_PEER_REGS                 (16U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void udmaTestEventCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Global test pass/fail flag */
static volatile int32_t gUdmaTestEventResult = UDMA_SOK;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


/*
 * Test Case Description: Verifies the functions of csl_udmap.c
 * Test scenario 1: Validate CSL_udmapDecChanStats API
 * Test scenario 2: Validate CSL_udmapTriggerTxChan API
 * Test scenario 3: Validate CSL_udmapClearTxChanError API
 * Test scenario 4: Validate CSL_udmapEnableLink API
 * Test scenario 5: Validate CSL_udmapDisableTxChan API
 * Test scenario 6: Validate CSL_udmapEnableTxChan API
 * Test scenario 7: Validate CSL_udmapTeardownTxChan API to clear the
 *                  channel's enable bit once teardown is complete
 * Test scenario 8: Validate CSL_udmapTeardownTxChan API 
 *                  to force teardown the channel
 * Test scenario 9: Validate CSL_udmapDecChanStats API
 * Test scenario 10: Validate CSL_udmapTriggerRxChan API
 * Test scenario 11: Validate CSL_udmapClearRxChanError API
 * Test scenario 12: Validate CSL_udmapEnableLink API when chanIdx is rxChNum
 * Test scenario 13: Validate CSL_udmapDisableRxChan API
 * Test scenario 14: Validate CSL_udmapEnableRxChan API
 * Test scenario 15: Validate CSL_udmapTeardownRxChan API to clear the
 *                   channel's enable bit once teardown is complete 
 * Test scenario 16: Validate CSL_udmapTeardownRxChan API 
 *                   to force teardown the channel
 * Test scenario 17: Validate CSL_udmapGetRxFlowIdFirewallStatus API
 * Test scenario 18: Validate CSL_udmapGetRevision API
 * Test scenario 19: Validate CSL_udmapGetRevisionInfo API 
 */
int32_t UdmaTestCslUdmap(UdmaTestTaskObj *taskObj)
{
    int32_t                         retVal = UDMA_SOK;
    CSL_UdmapCfg                   *pUdmapRegs;
    Udma_DrvHandle                  drvHandle;
    uint32_t                        instId;
    uint32_t                        chanIdx;
    CSL_UdmapChanDir                chanDir;
    struct Udma_ChObj               chObj;
    Udma_ChHandle                   chHandle;
    Udma_ChPrms                     chPrms;
    uint32_t                        chType;
    Udma_ChTxPrms                   txChPrms;
    Udma_ChRxPrms                   rxChPrms;
    uint32_t                        timeout=0U;
    Udma_ChStats                    chStats;
    CSL_UdmapChanStats              pChanStats;
    Udma_EventPrms                  eventPrms;
    struct Udma_EventObj            eventObj;
    Udma_EventHandle                eventHandle;
    CSL_UdmapRevision               pRev;
    CSL_UdmapRxFlowIdFirewallStatus pRxFlowIdFwStatus;
    Udma_EventRxFlowIdFwStatus      status;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA  Testcase :: Csl Udmap test cases\r\n",
              taskObj->taskId);

    instId           = UDMA_TEST_DEFAULT_UDMA_INST;
    drvHandle        = &taskObj->testObj->drvObj[instId];
    pUdmapRegs       = &drvHandle->udmapRegs;
    chHandle         = &chObj;
    chType           = UDMA_CH_TYPE_TX;
    UdmaChPrms_init(&chPrms, chType);
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    UdmaChTxPrms_init(&txChPrms, chType);
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigTx(chHandle, &txChPrms);     
        if(UDMA_SOK == retVal)
        {
            chanIdx = chHandle->txChNum;
            chanDir = CSL_UDMAP_CHAN_DIR_TX;
            retVal  = Udma_chGetStats(chHandle, &chStats);
            if(UDMA_SOK == retVal)
            {
                /* Test scenario 1: Validate CSL_udmapDecChanStats API */
                CSL_udmapDecChanStats(pUdmapRegs, chanIdx, chanDir, &pChanStats);
            }
            /* Test scenario 2: Validate CSL_udmapTriggerTxChan API */
            retVal = CSL_udmapTriggerTxChan(pUdmapRegs, chanIdx);
            if(UDMA_SOK == retVal)
            {
                /* Test scenario 3: Validate CSL_udmapClearTxChanError API */
                CSL_udmapClearTxChanError(pUdmapRegs, chanIdx);
            }
            else
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                        " |TEST INFO|:: FAIL:: UDMA:: CSL_udmapTriggerTxChan:: Pos::"
                        " Check when chanIdx is txChNum!!\n");
                retVal = UDMA_EFAIL;
            }
            retVal  = Udma_chEnable(chHandle);
            if(UDMA_SOK == retVal)
            {
                /* Test scenario 4: Validate CSL_udmapEnableLink API */
                retVal  = CSL_udmapEnableLink(pUdmapRegs, chanIdx, chanDir);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                            " |TEST INFO|:: FAIL:: UDMA:: CSL_udmapEnableLink:: Pos::"
                            " Check when chanIdx is txChNum and chanDir is CSL_UDMAP_CHAN_DIR_TX!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = UDMA_SOK;
                    /* Test scenario 5: Validate CSL_udmapDisableTxChan API */
                    retVal = CSL_udmapDisableTxChan(pUdmapRegs, chanIdx);
                    if(UDMA_SOK == retVal)
                    {
                        /* Test scenario 6: Validate CSL_udmapEnableTxChan API */
                        retVal = CSL_udmapEnableTxChan(pUdmapRegs, chanIdx);
                        if(UDMA_SOK == retVal)
                        {
                            /* Test scenario 7: Validate CSL_udmapTeardownTxChan API to clear the
                             *                  channel's enable bit once teardown is complete
                             */
                            retVal = CSL_udmapTeardownTxChan(pUdmapRegs, chanIdx, (bool)false, (bool)true);
                            if(UDMA_SOK != retVal)
                            {
                                GT_0trace(taskObj->traceMask, GT_ERR,
                                        " |TEST INFO|:: FAIL:: UDMA:: CSL_udmapTeardownTxChan:: Pos::"
                                        " Check to clear channel's enable bit once teardown is complete!!\n");
                                retVal = UDMA_EFAIL;
                            }
                        }
                        if(UDMA_SOK == retVal)
                        {
                            retVal = CSL_udmapEnableTxChan(pUdmapRegs, chanIdx);
                            if(UDMA_SOK == retVal)
                            {
                                /* Test scenario 8: Validate CSL_udmapTeardownTxChan API 
                                 *                  to force teardown the channel */
                                retVal = CSL_udmapTeardownTxChan(pUdmapRegs, chanIdx, (bool)true, (bool)false);
                                if(UDMA_SOK != retVal)
                                {
                                    GT_0trace(taskObj->traceMask, GT_ERR,
                                            " |TEST INFO|:: FAIL:: UDMA:: CSL_udmapTeardownTxChan:: Pos::"
                                            " Check to force teardown the channel!!\n");
                                    retVal = UDMA_EFAIL;
                                }
                            }
                        }
                    }
                }
            }
        
        }
        Udma_chClose(chHandle);
    }

    if(UDMA_SOK == retVal)
    {
        chHandle         = &chObj;
        chType           = UDMA_CH_TYPE_RX;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChRxPrms_init(&rxChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigRx(chHandle, &rxChPrms);     
            if(UDMA_SOK == retVal)
            {
                chanIdx = chHandle->rxChNum;
                chanDir = CSL_UDMAP_CHAN_DIR_RX;
                retVal  = Udma_chGetStats(chHandle, &chStats);
                if(UDMA_SOK == retVal)
                {
                    /* Test scenario 9: Validate CSL_udmapDecChanStats API */
                    CSL_udmapDecChanStats(pUdmapRegs, chanIdx, chanDir, &pChanStats);
                }
                /* Test scenario 10: Validate CSL_udmapTriggerRxChan API */
                retVal = CSL_udmapTriggerRxChan(pUdmapRegs, chanIdx);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: CSL_udmapTriggerRxChan:: Pos::"
                              " Check when chanIdx is rxChNum!!\n");
                    retVal = UDMA_EFAIL;   
                }   
                /* Test scenario 11: Validate CSL_udmapClearRxChanError API */
                CSL_udmapClearRxChanError(pUdmapRegs, chanIdx);
                retVal  = Udma_chEnable(chHandle);
                if(UDMA_SOK == retVal)
                {
                    /* Test scenario 12: Validate CSL_udmapEnableLink API when chanIdx is rxChNum */
                    retVal  = CSL_udmapEnableLink(pUdmapRegs, chanIdx, chanDir);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                " |TEST INFO|:: FAIL:: UDMA:: CSL_udmapEnableLink:: Pos::"
                                " Check when chanIdx is rxChNum and chanDir is CSL_UDMAP_CHAN_DIR_RX!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                        /* Test scenario 13: Validate CSL_udmapDisableRxChan API */
                        retVal = CSL_udmapDisableRxChan(pUdmapRegs, chanIdx);
                        if(UDMA_SOK == retVal)
                        {
                            /* Test scenario 14: Validate CSL_udmapEnableRxChan API */
                            retVal = CSL_udmapEnableRxChan(pUdmapRegs, chanIdx);
                            if(UDMA_SOK == retVal)
                            {
                                /* Test scenario 15: Validate CSL_udmapTeardownRxChan API to clear the
                                 *                   channel's enable bit once teardown is complete           
                                 */
                                retVal = CSL_udmapTeardownRxChan(pUdmapRegs, chanIdx, BFALSE, BTRUE);
                                if(UDMA_SOK == retVal)
                                {
                                    retVal = CSL_udmapEnableRxChan(pUdmapRegs, chanIdx);
                                    /* Test scenario 16: Validate CSL_udmapTeardownRxChan API 
                                    *                    to force teardown the channel          
                                    */
                                    retVal = CSL_udmapTeardownRxChan(pUdmapRegs, chanIdx, BTRUE, BFALSE);
                                    if(UDMA_SOK != retVal)
                                    {
                                        GT_0trace(taskObj->traceMask, GT_ERR,
                                                " |TEST INFO|:: FAIL:: UDMA:: CSL_udmapTeardownRxChan:: Pos::"
                                                " Check to force teardown the channel!!\n");
                                        retVal = UDMA_EFAIL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Udma_chClose(chHandle);
        }
    }

    if(UDMA_SOK == retVal)
    {
        Udma_initDrvHandle(drvHandle);
        chHandle         = &chObj;
        chType           = UDMA_CH_TYPE_RX;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChRxPrms_init(&rxChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigRx(chHandle, &rxChPrms);     
            if(UDMA_SOK == retVal)
            {
                retVal  = Udma_chEnable(chHandle);
                eventHandle = &eventObj;
                UdmaEventPrms_init(&eventPrms);
                eventPrms.eventType         = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
                eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
                eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
                eventPrms.eventCb           = &udmaTestEventCb;
                retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
                pUdmapRegs                  = &drvHandle->udmapRegs;
                retVal = Udma_eventGetRxFlowIdFwStatus(eventHandle, &status);
                if(UDMA_SOK == retVal)
                {
                    /* Test scenario 17: Validate CSL_udmapGetRxFlowIdFirewallStatus API */
                    retVal = CSL_udmapGetRxFlowIdFirewallStatus(pUdmapRegs, &pRxFlowIdFwStatus);
                    if((bool)true == retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                " |TEST INFO|:: FAIL:: UDMA:: CSL_udmapGetRxFlowIdFirewallStatus:: Pos::"
                                " Check when the flow id is valid!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                    }
                }
                Udma_eventUnRegister(eventHandle);
                Udma_chDisable(chHandle, timeout);
            }
            Udma_chClose(chHandle);
        }
    }

    if(UDMA_SOK == retVal)
    {
        Udma_initDrvHandle(drvHandle);
        chHandle         = &chObj;
        chType           = UDMA_CH_TYPE_RX;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChRxPrms_init(&rxChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigRx(chHandle, &rxChPrms);     
            if(UDMA_SOK == retVal)
            {
                pUdmapRegs = &drvHandle->udmapRegs;
                /* Test scenario 18: Validate CSL_udmapGetRevision API */
                CSL_udmapGetRevision(pUdmapRegs);
                /* Test scenario 19: Validate CSL_udmapGetRevisionInfo API */
                retVal = CSL_udmapGetRevisionInfo(pUdmapRegs, &pRev);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                            " |TEST INFO|:: FAIL:: UDMA:: CSL_udmapGetRevisionInfo:: Pos::"
                            " Check when pUdmapRegs is valid!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = UDMA_SOK;
                }
            }
            Udma_chClose(chHandle);
        }
    }

    return retVal;
}

static void udmaTestEventCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData)
{
    SemaphoreP_Handle   transferDoneSem = (SemaphoreP_Handle) appData;

    if(transferDoneSem != NULL)
    {
        if(UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW == eventType)
        {
            SemaphoreP_post(transferDoneSem);
        }
        else
        {
            gUdmaTestEventResult = UDMA_EFAIL;
        }
    }
    else
    {
        gUdmaTestEventResult = UDMA_EFAIL;
    }

    return;
}

/*
 * Test Case Description: Verifies the functions of Csl_udmap.c
 * Test scenario 1: Validate Udma_eventGetRxFlowIdFwStatus API
 * Test scenario 2: Validate CSL_udmapPauseRxChan API when channel is not enabled
 * Test scenario 3: Validate CSL_udmapInitTxChanCfg API
 * Test scenario 4: Validate CSL_udmapInitRxChanCfg API
 * Test scenario 5: Validate CSL_udmapInitRxFlowCfg API 
 * Test scenario 6: Validate CSL_udmapGetChanPeerReg API 
 *                  when regIdx is greater than CSL_UDMAP_NUM_PEER_REGS
 */
int32_t UdmaTestCslUdmapNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                         retVal = UDMA_SOK;
    CSL_UdmapCfg                   *pUdmapRegs;
    Udma_DrvHandle                  drvHandle;
    uint32_t                        instId;
    uint32_t                        chanIdx;
    struct Udma_ChObj               chObj;
    Udma_ChHandle                   chHandle;
    Udma_ChPrms                     chPrms;
    uint32_t                        chType;
    Udma_ChRxPrms                   rxChPrms;
    uint32_t                        timeout=0U;
    Udma_EventPrms                  eventPrms;
    struct Udma_EventObj            eventObj;
    Udma_EventHandle                eventHandle;
    CSL_UdmapTxChanCfg              pTxChanCfg;
    CSL_UdmapRxChanCfg              pRxChanCfg;
    CSL_UdmapRxFlowCfg              pFlow;
    uint32_t                        regIdx;
    uint32_t                        pVal;
    CSL_UdmapChanDir                chanDir;
    Udma_ChTxPrms                   txChPrms;
    Udma_ChRxPrms                   rxPrms;
    Udma_EventRxFlowIdFwStatus      status;
    struct Udma_DrvObj              backUpDrvObj;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Testcase:: Csl Udmap Negative test case \r\n",
              taskObj->taskId);

    instId           = UDMA_TEST_DEFAULT_UDMA_INST;
    drvHandle        = &taskObj->testObj->drvObj[instId];
    pUdmapRegs       = &drvHandle->udmapRegs;
    Udma_initDrvHandle(drvHandle);
    chHandle         = &chObj;
    chType           = UDMA_CH_TYPE_RX;
    UdmaChPrms_init(&chPrms, chType);
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    UdmaChRxPrms_init(&rxChPrms, chType);
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigRx(chHandle, &rxChPrms);     
        if(UDMA_SOK == retVal)
        {
            Udma_chEnable(chHandle);
            /* Test scenario 1: Validate Udma_eventGetRxFlowIdFwStatus API */
            eventHandle = &eventObj;
            UdmaEventPrms_init(&eventPrms);
            eventPrms.eventType         = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
            eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
            eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
            eventPrms.eventCb           = &udmaTestEventCb;
            retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
            pUdmapRegs                  = &drvHandle->udmapRegs;
            backUpDrvObj                = taskObj->testObj->drvObj[instId];
            drvHandle->devIdUdma        = TISCI_DEV_MCU_NAVSS0_UDMAP_0 + 1U;
            retVal                     += Udma_eventGetRxFlowIdFwStatus(eventHandle, &status);
            if(UDMA_SOK == retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                        " |TEST INFO|:: FAIL:: UDMA:: Udma_eventGetRxFlowIdFwStatus:: Neg::"
                        " Check when flow id is out of range!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
            taskObj->testObj->drvObj[instId] = backUpDrvObj;
            Udma_eventUnRegister(eventHandle);
            Udma_chDisable(chHandle, timeout);
        }
        Udma_chClose(chHandle);
    }

    if(UDMA_SOK == retVal)
    {
        Udma_initDrvHandle(drvHandle);
        chHandle         = &chObj;
        chType           = UDMA_CH_TYPE_RX;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChRxPrms_init(&rxChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigRx(chHandle, &rxChPrms);     
            if(UDMA_SOK == retVal)
            {
                /* Test scenario 2: Validate CSL_udmapPauseRxChan API 
                 *                  when channel is not enabled
                 */
                chanIdx = chHandle->rxChNum;
                retVal  = CSL_udmapPauseRxChan(pUdmapRegs, chanIdx);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                            " |TEST INFO|:: FAIL:: UDMA:: CSL_udmapPauseRxChan:: Neg:: "
                            " Check to pause Rx channel when it is not enabled!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = UDMA_SOK;
                }
            }
            Udma_chClose(chHandle);
        }
    }

    if(UDMA_SOK == retVal)
    {
        instId           = UDMA_TEST_DEFAULT_UDMA_INST;
        chType           = UDMA_CH_TYPE_TR_BLK_COPY;
        drvHandle        = &taskObj->testObj->drvObj[instId];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChTxPrms_init(&txChPrms, chType);
        UdmaChRxPrms_init(&rxPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigTx(chHandle, &txChPrms);
            retVal = Udma_chConfigRx(chHandle, &rxPrms);
            /* Test scenario 3: Validate CSL_udmapInitTxChanCfg API */
            CSL_udmapInitTxChanCfg(&pTxChanCfg);
            /* Test scenario 4: Validate CSL_udmapInitRxChanCfg API */
            CSL_udmapInitRxChanCfg(&pRxChanCfg);
            /* Test scenario 5: Validate CSL_udmapInitRxFlowCfg API */
            CSL_udmapInitRxFlowCfg(&pFlow);
            if(UDMA_SOK == retVal)
            {
                pUdmapRegs = &drvHandle->udmapRegs;
                chanIdx = chHandle->txChNum;
                chanDir = CSL_UDMAP_CHAN_DIR_TX;
                regIdx  = CSL_UDMAP_NUM_PEER_REGS + 1U;
                /* Test scenario 6: Validate CSL_udmapGetChanPeerReg API 
                 *                  when regIdx is greater than CSL_UDMAP_NUM_PEER_REGS
                 */
                retVal = CSL_udmapGetChanPeerReg(pUdmapRegs, chanIdx, chanDir, regIdx, &pVal);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                            " |TEST INFO|:: FAIL:: UDMA:: CSL_udmapGetChanPeerReg:: Neg::"
                            " Check when regIdx is greater than CSL_UDMAP_NUM_PEER_REGS!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = UDMA_SOK;
                }
            }
        }
        Udma_chClose(chHandle);
    }

    return retVal;
}
