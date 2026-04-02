/*
 *  Copyright (c) Texas Instruments Incorporated 2019-2024
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
 *  \file udma_test_ch.c
 *
 *  \brief UDMA channel related test case file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <udma_test.h>
#include <udma_testconfig.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t udmaTestChPktdmaParamCheckTestLoop(UdmaTestTaskObj *taskObj);
static int32_t udmaTestChPktdmaChApiTestLoop(UdmaTestTaskObj *taskObj);
static int32_t udmaTestBcdmaParamCheckTestLoop(UdmaTestTaskObj *taskObj);
static int32_t udmaTestBcdmaChApiTestLoop(UdmaTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Global test pass/fail flag */
static volatile int32_t gUdmaTestChResult = UDMA_SOK;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


int32_t udmaTestChPktdmaParamCheckTc(UdmaTestTaskObj *taskObj)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    loopCnt = 0U;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: PKTDMA Channel Paramter Check Testcase ::\r\n", taskObj->taskId);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Loop count           : %d ::\r\n", taskObj->taskId, taskObj->loopCnt);

    gUdmaTestChResult = UDMA_SOK;
    while(loopCnt < taskObj->loopCnt)
    {
        retVal = udmaTestChPktdmaParamCheckTestLoop(taskObj);
        if(UDMA_SOK != retVal)
        {
            break;
        }

        loopCnt++;
    }

    retVal += gUdmaTestChResult;

    return (retVal);
}


int32_t udmaTestChPktdmaChApiTc(UdmaTestTaskObj *taskObj)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    loopCnt = 0U;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: PKTDMA Channel API's Testcase ::\r\n", taskObj->taskId);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Loop count           : %d ::\r\n", taskObj->taskId, taskObj->loopCnt);

    gUdmaTestChResult = UDMA_SOK;
    while(loopCnt < taskObj->loopCnt)
    {
        retVal = udmaTestChPktdmaChApiTestLoop(taskObj);
        if(UDMA_SOK != retVal)
        {
            break;
        }

        loopCnt++;
    }

    retVal += gUdmaTestChResult;

    return (retVal);
}

static int32_t udmaTestChPktdmaParamCheckTestLoop(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
#if (UDMA_SOC_CFG_RA_LCDMA_PRESENT == 1) && (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    uint32_t            elemCnt = 50U, ringMemSize;
    uint32_t            heapId = UTILS_MEM_HEAP_ID_MSMC;
    Udma_DrvHandle      drvHandle;
    uint32_t            chType;
    struct Udma_ChObj   chObj;
    Udma_ChHandle       chHandle = &chObj;
    Udma_ChPrms         chPrms;
    void               *ringMem = NULL;

    drvHandle = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_PKTDMA_0];
    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }


#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    /* PKTDMA Channel Open with no ring memory test */
    chType = UDMA_CH_TYPE_TX_MAPPED;
    UdmaChPrms_init(&chPrms, chType);
    chPrms.mappedChGrp = UDMA_MAPPED_TX_GROUP_CPSW;
    chPrms.peerChNum   = UDMA_TEST_PKTDMA_CPSW_TX_PEER_CH;
    retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK != retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
            " UDMA channel open failed!!\n");
    }
    if(UDMA_SOK == retVal)
    {
        if(chHandle->fqRing != (Udma_RingHandle) NULL_PTR)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                    " Ring allocated even when no ring memory was provided!!\n");
            retVal = UDMA_EFAIL;
        }
    }
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chClose(chHandle);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA channel close failed!!\n");
        }
    }

    /* PKTDMA Mapped Channel Open with invalid mapped channel group negative test*/
    chType = UDMA_CH_TYPE_TX_MAPPED;
    UdmaChPrms_init(&chPrms, chType);
    chPrms.fqRingPrms.ringMem       = ringMem;
    chPrms.fqRingPrms.ringMemSize   = ringMemSize;
    chPrms.fqRingPrms.elemCnt       = elemCnt;
    chPrms.mappedChGrp = UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP;
    retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
            " Channel Open did not fail for incorrect mappedChGrp check!!\n");
            retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }
    
#endif

    if(NULL != ringMem)
    {
        retVal += Utils_memFree(heapId, ringMem, ringMemSize);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
        }
    }
#endif

    return(retVal);
}

static int32_t udmaTestChPktdmaChApiTestLoop(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
#if (UDMA_SOC_CFG_RA_LCDMA_PRESENT == 1) && (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
    uint32_t            elemCnt = 50U, ringMemSize;
    uint32_t            heapId = UTILS_MEM_HEAP_ID_MSMC;
    Udma_DrvHandle      drvHandle;
    uint32_t            chType;
    struct Udma_ChObj   chObj;
    Udma_ChHandle       chHandle = &chObj;
    Udma_ChPrms         chPrms;
    Udma_ChTxPrms       txPrms;
    Udma_ChRxPrms       rxPrms;
    uint32_t            chGrpIdx;
    void               *ringMem = NULL;
    Udma_RmInitPrms    *rmInitPrms;
    char *pktdmaChGrpStr[] = { "Unmapped TX", "CPSW TX", "SAUL TX", "ICSSG_0 TX", "ICSSG_1_TX",
                               "Unmapped RX", "CPSW RX", "SAUL RX", "ICSSG_0 RX", "ICSSG_1_RX"};
    const UdmaTestPktdmaChPrm  *pktdmaChPrms = NULL;

    drvHandle = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_PKTDMA_0];
    rmInitPrms = &drvHandle->initPrms.rmInitPrms;
    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }
    
    /* Test for each config in PKTMA Channel param table */
    for(chGrpIdx = 0U; chGrpIdx < UDMA_TEST_NUM_PKTDMA_CH_PRM; chGrpIdx++)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " Testing for PKTDMA %s Channel Group  ...\r\n",
                  pktdmaChGrpStr[chGrpIdx]);

        if(((UDMA_TEST_PKTDMA_CH_PRMID_UNMAPPED_TX == chGrpIdx) && (0U == rmInitPrms->numTxCh)) ||
           ((UDMA_TEST_PKTDMA_CH_PRMID_CPSW_TX == chGrpIdx) && (0U == rmInitPrms->numMappedTxCh[UDMA_MAPPED_TX_GROUP_CPSW])) ||
           ((UDMA_TEST_PKTDMA_CH_PRMID_SAUL_TX == chGrpIdx) && (0U == rmInitPrms->numMappedTxCh[UDMA_MAPPED_TX_GROUP_SAUL])) ||
           ((UDMA_TEST_PKTDMA_CH_PRMID_ICSSG_0_TX == chGrpIdx) && (0U == rmInitPrms->numMappedTxCh[UDMA_MAPPED_TX_GROUP_ICSSG_0])) ||
           ((UDMA_TEST_PKTDMA_CH_PRMID_ICSSG_1_TX == chGrpIdx) && (0U == rmInitPrms->numMappedTxCh[UDMA_MAPPED_TX_GROUP_ICSSG_1])) ||
           ((UDMA_TEST_PKTDMA_CH_PRMID_UNMAPPED_RX == chGrpIdx) && (0U == rmInitPrms->numRxCh)) ||
           ((UDMA_TEST_PKTDMA_CH_PRMID_CPSW_RX == chGrpIdx) && (0U == rmInitPrms->numMappedRxCh[UDMA_MAPPED_RX_GROUP_CPSW - UDMA_NUM_MAPPED_TX_GROUP])) ||
           ((UDMA_TEST_PKTDMA_CH_PRMID_SAUL_RX == chGrpIdx) && (0U == rmInitPrms->numMappedRxCh[UDMA_MAPPED_RX_GROUP_SAUL - UDMA_NUM_MAPPED_TX_GROUP])) ||
           ((UDMA_TEST_PKTDMA_CH_PRMID_ICSSG_0_RX == chGrpIdx) && (0U == rmInitPrms->numMappedRxCh[UDMA_MAPPED_RX_GROUP_ICSSG_0 - UDMA_NUM_MAPPED_TX_GROUP])) ||
           ((UDMA_TEST_PKTDMA_CH_PRMID_ICSSG_1_RX == chGrpIdx) && (0U == rmInitPrms->numMappedRxCh[UDMA_MAPPED_RX_GROUP_ICSSG_1 - UDMA_NUM_MAPPED_TX_GROUP])))
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " Skipping the Test for PKTDMA %s Channel Group, since no channels are reserved!!\r\n",
                      pktdmaChGrpStr[chGrpIdx]);
            continue;
        }

        pktdmaChPrms = &gUdmaTestPktdmaChPrm[chGrpIdx];

        chType = pktdmaChPrms->chType;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.fqRingPrms.ringMem       = ringMem;
        chPrms.fqRingPrms.ringMemSize   = ringMemSize;
        chPrms.fqRingPrms.elemCnt       = elemCnt;
        chPrms.mappedChGrp = pktdmaChPrms->mappedChGrp;
        chPrms.peerChNum   = pktdmaChPrms->peerChNum;

        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA channel open failed!!\n");
            break;
        }
        else
        {
            GT_2trace(taskObj->traceMask, GT_INFO1,
                        " |TEST INFO|:: Task:%d: Allocated Ch   : %d ::\r\n",
                        taskObj->taskId, Udma_chGetNum(chHandle));
        }

        if((chType & UDMA_CH_FLAG_TX) == UDMA_CH_FLAG_TX)
        {
            /* Config TX channel */
            UdmaChTxPrms_init(&txPrms, chType);
            retVal = Udma_chConfigTx(chHandle, &txPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                    " UDMA TX channel config failed!!\n");
                break;
            }
        }
        else
        {
            /* Config RX channel */
            UdmaChRxPrms_init(&rxPrms, chType);
            retVal = Udma_chConfigRx(chHandle, &rxPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                    " UDMA RX channel config failed!!\n");
                break;
            }
        }
        
        retVal = Udma_chEnable(chHandle);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA channel enable failed!!\n");
            break;
        }

        retVal = Udma_chDisable(chHandle, UDMA_DEFAULT_CH_DISABLE_TIMEOUT);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA channel disable failed!!\n");
            break;
        }

        retVal = Udma_chClose(chHandle);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA channel close failed!!\n");
            break;
        }
        
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " Testing for PKTDMA %s Channel Group passed!!\r\n",
                  pktdmaChGrpStr[chGrpIdx]);
    }
#endif

    return(retVal);
}

int32_t udmaTestBcdmaParamCheckTc(UdmaTestTaskObj *taskObj)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    loopCnt = 0U;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: BCDMA Channel Paramter Check Testcase ::\r\n", taskObj->taskId);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Loop count           : %d ::\r\n", taskObj->taskId, taskObj->loopCnt);

    gUdmaTestChResult = UDMA_SOK;
    while(loopCnt < taskObj->loopCnt)
    {
        retVal = udmaTestBcdmaParamCheckTestLoop(taskObj);
        if(UDMA_SOK != retVal)
        {
            break;
        }

        loopCnt++;
    }

    retVal += gUdmaTestChResult;

    return (retVal);
}

int32_t udmaTestBcdmaChApiTc(UdmaTestTaskObj *taskObj)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    loopCnt = 0U;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: BCDMA Channel API's Testcase ::\r\n", taskObj->taskId);
    GT_2trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: Loop count           : %d ::\r\n", taskObj->taskId, taskObj->loopCnt);

    gUdmaTestChResult = UDMA_SOK;
    while(loopCnt < taskObj->loopCnt)
    {
        retVal = udmaTestBcdmaChApiTestLoop(taskObj);
        if(UDMA_SOK != retVal)
        {
            break;
        }

        loopCnt++;
    }

    retVal += gUdmaTestChResult;

    return (retVal);
}

static int32_t udmaTestBcdmaParamCheckTestLoop(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
#if (UDMA_SOC_CFG_RA_LCDMA_PRESENT == 1) && (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    uint32_t            elemCnt = 50U, ringMemSize;
    uint32_t            heapId = UTILS_MEM_HEAP_ID_MSMC;
    Udma_DrvHandle      drvHandle;
    uint32_t            chType[UDMA_TEST_BCDMA_CH_NUM_CH_TYPE] = {UDMA_CH_TYPE_TX, UDMA_CH_TYPE_RX};
    uint32_t            peerCh[UDMA_TEST_BCDMA_CH_NUM_CH_TYPE] = {UDMA_TEST_BCDMA_PEER_CH_NUM_TX, UDMA_TEST_BCDMA_PEER_CH_NUM_RX};
    struct Udma_ChObj   chObj;
    Udma_ChHandle       chHandle = &chObj;
    Udma_ChPrms         chPrms;
    uint32_t            i;
    void               *ringMem = NULL;

    drvHandle = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_BCDMA_0];

    for (i = 0; i < UDMA_TEST_BCDMA_CH_NUM_CH_TYPE; i++)
    {
        /* BCDMA Channel Open with no ring memory test */
        memset(&chPrms, 0u, sizeof(Udma_ChPrms));
        UdmaChPrms_init(&chPrms, chType[i]);
        chPrms.peerChNum   = peerCh[i];
        retVal = Udma_chOpen(drvHandle, chHandle, chType[i], &chPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA channel open failed!!\n");
        }
        if(UDMA_SOK == retVal)
        {
            if(chHandle->fqRing != (Udma_RingHandle) NULL_PTR)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                        " Ring allocated even when no ring memory was provided!!\n");
                retVal = UDMA_EFAIL;
            }
        }
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chClose(chHandle);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                    " UDMA channel close failed!!\n");
            }
        }

        ringMemSize = elemCnt * sizeof (uint64_t);
        ringMem = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        if(NULL == ringMem)
        {
            retVal = UDMA_EALLOC;
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
        }


        /* BCDMA Channel Open with ring memory test */
        UdmaChPrms_init(&chPrms, chType[i]);
        chPrms.peerChNum   = peerCh[i];
        chPrms.fqRingPrms.ringMem       = ringMem;
        chPrms.fqRingPrms.ringMemSize   = ringMemSize;
        chPrms.fqRingPrms.elemCnt       = elemCnt;
        retVal = Udma_chOpen(drvHandle, chHandle, chType[i], &chPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA channel open failed!!\n");
        }
        if(UDMA_SOK == retVal)
        {
            if(chHandle->fqRing == (Udma_RingHandle) NULL_PTR)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                        " Ring allocation failed even ring memory was provided!!\n");
                retVal = UDMA_EFAIL;
            }
        }
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chClose(chHandle);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                    " UDMA channel close failed!!\n");
            }
        }

        if(NULL != ringMem)
        {
            retVal += Utils_memFree(heapId, ringMem, ringMemSize);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
            }
        }
    }
#endif

    return(retVal);
}

static int32_t udmaTestBcdmaChApiTestLoop(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
#if (UDMA_SOC_CFG_RA_LCDMA_PRESENT == 1) && (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    uint32_t            elemCnt = 50U, ringMemSize;
    uint32_t            heapId = UTILS_MEM_HEAP_ID_MSMC;
    Udma_DrvHandle      drvHandle;
    uint32_t            chType[UDMA_TEST_BCDMA_CH_NUM_CH_TYPE] = {UDMA_CH_TYPE_TX, UDMA_CH_TYPE_RX};
    uint32_t            peerCh[UDMA_TEST_BCDMA_CH_NUM_CH_TYPE] = {UDMA_TEST_BCDMA_PEER_CH_NUM_TX, UDMA_TEST_BCDMA_PEER_CH_NUM_RX};
    struct Udma_ChObj   chObj;
    Udma_ChHandle       chHandle = &chObj;
    Udma_ChPrms         chPrms;
    Udma_ChTxPrms       txPrms;
    Udma_ChRxPrms       rxPrms;
    uint32_t            i;
    void               *ringMem = NULL;

    drvHandle = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_BCDMA_0];

    for (i = 0; i < UDMA_TEST_BCDMA_CH_NUM_CH_TYPE; i++)
    {
        ringMemSize = elemCnt * sizeof (uint64_t);
        ringMem = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        if(NULL == ringMem)
        {
            retVal = UDMA_EALLOC;
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
        }
        memset(&chPrms, 0u, sizeof(Udma_ChPrms));
        UdmaChPrms_init(&chPrms, chType[i]);
        chPrms.peerChNum   = peerCh[i];
        chPrms.fqRingPrms.ringMem       = ringMem;
        chPrms.fqRingPrms.ringMemSize   = ringMemSize;
        chPrms.fqRingPrms.elemCnt       = elemCnt;
        retVal = Udma_chOpen(drvHandle, chHandle, chType[i], &chPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA channel open failed!!\n");
        }
        else
        {
            GT_2trace(taskObj->traceMask, GT_INFO1,
                        " |TEST INFO|:: Task:%d: Allocated Ch   : %d ::\r\n",
                        taskObj->taskId, Udma_chGetNum(chHandle));
        }

        if((chType[i] & UDMA_CH_FLAG_TX) == UDMA_CH_FLAG_TX)
        {
            /* Config TX channel */
            UdmaChTxPrms_init(&txPrms, chType[i]);
            retVal = Udma_chConfigTx(chHandle, &txPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                    " UDMA TX channel config failed!!\n");
            }
        }

        if((chType[i] & UDMA_CH_FLAG_RX) == UDMA_CH_FLAG_RX)
        {
            /* Config RX channel */
            UdmaChRxPrms_init(&rxPrms, chType[i]);
            retVal = Udma_chConfigRx(chHandle, &rxPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                    " UDMA RX channel config failed!!\n");
            }
        }
        
        retVal = Udma_chEnable(chHandle);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA channel enable failed!!\n");
        }

        retVal = Udma_chDisable(chHandle, UDMA_DEFAULT_CH_DISABLE_TIMEOUT);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA channel disable failed!!\n");
        }

        retVal = Udma_chClose(chHandle);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                " UDMA channel close failed!!\n");
        }

        if(NULL != ringMem)
        {
            retVal += Utils_memFree(heapId, ringMem, ringMemSize);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
            }
        }
    }

#endif

    return(retVal);
}

/*
 * Test Case Description: Verifies the function Udma_chPause 
 * Test scenario 1: Check when instType is UDMA_INST_TYPE_LCDMA_BCDMA 
 *                  and chType is UDMA_CH_TYPE_TX
 * Test scenario 2: Check when chType is UDMA_CH_TYPE_RX
 */
int32_t UdmaTestChPause(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    #if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    struct Udma_ChObj  chObj;
    Udma_ChHandle      chHandle;
    struct Udma_DrvObj backUpDrvObj;
    Udma_DrvHandle     drvHandle;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    uint32_t           timeout;
    uint32_t           instID; 

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChPause positive Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when instType is UDMA_INST_TYPE_LCDMA_BCDMA 
    *                   and chType is UDMA_CH_TYPE_TX 
    */
    chHandle         = &chObj;
    timeout          = 0U;
    instID           = UDMA_TEST_INST_ID_BCDMA_0;
    chType           = UDMA_CH_TYPE_TX;
    backUpDrvObj     = taskObj->testObj->drvObj[instID];
    drvHandle        = &taskObj->testObj->drvObj[instID];
    UdmaChPrms_init(&chPrms, chType);
    chPrms.peerChNum = UDMA_PSIL_CH_MAIN_CSI_TX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    Udma_ChTxPrms txChPrms;
    UdmaChTxPrms_init(&txChPrms, chType);
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigTx(chHandle, &txChPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(chHandle);
            if(UDMA_SOK == retVal)
            {
                retVal = Udma_chPause(chHandle);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: Udma_chPauseTxLocal:: "
                              " Pos:: Check when instType is UDMA_INST_TYPE_LCDMA_BCDMA "
                              " and chType is UDMA_CH_TYPE_TX!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = Udma_chResume(chHandle);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: Udma_chUnpauseTxLocal::"
                                  " Pos:: Check when instType is UDMA_INST_TYPE_LCDMA_BCDMA"
                                  " and chType is UDMA_CH_TYPE_TX!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                    }
                }
                Udma_chDisable(chHandle, timeout);
            }
        }
        Udma_chClose(chHandle);
    }
    taskObj->testObj->drvObj[instID] = backUpDrvObj;


    /* Test scenario 2: Check when chType is UDMA_CH_TYPE_RX */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX;
        backUpDrvObj     = taskObj->testObj->drvObj[instID];
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        Udma_ChRxPrms rxPrms;
        UdmaChRxPrms_init(&rxPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigRx(chHandle, &rxPrms);
            if(UDMA_SOK == retVal)
            {
                retVal = Udma_chEnable(chHandle);
                if(UDMA_SOK == retVal)
                {
                    retVal = Udma_chPause(chHandle);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: Udma_chPauseRxLocal:: "
                                  " Pos:: Check when chType is UDMA_CH_TYPE_RX!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = Udma_chResume(chHandle);
                        if(UDMA_SOK != retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: Udma_chUnpauseRxLocal:: "
                                      " Pos:: Check when chType is UDMA_CH_TYPE_RX!!\n");
                            retVal = UDMA_EFAIL;
                        }
                        else
                        {
                            retVal = UDMA_SOK;
                        }
                    }
                    Udma_chDisable(chHandle, timeout);
                }
            }
            Udma_chClose(chHandle);
        }
    }
    taskObj->testObj->drvObj[instID] = backUpDrvObj;
    #endif

    return retVal;
}

/* 
 * Test Case Description: Verifies the function Udma_chGetDefaultFlowHandle and Udma_chGetStats
 * Test scenario 1: Check when instType is UDMA_INST_TYPE_LCDMA_BCDMA and chType is UDMA_CH_TYPE_RX
 */
int32_t UdmaTestChGetDefaultFlowHandle(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    struct Udma_ChObj  chObj;
    struct Udma_DrvObj backUpDrvObj;
    Udma_ChHandle      chHandle;
    Udma_ChPrms        chPrms;
    Udma_DrvHandle     drvHandle;
    uint32_t           chType;
    uint32_t           instID;
    Udma_ChRxPrms      rxChPrms;
    Udma_ChStats       chStats;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChGetDefaultFlowHandle positive Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check for Valid args */
    chHandle         = &chObj;
    instID           = UDMA_TEST_INST_ID_BCDMA_0;
    chType           = UDMA_CH_TYPE_RX;
    UdmaChPrms_init(&chPrms, chType);
    backUpDrvObj     = taskObj->testObj->drvObj[instID];
    drvHandle        = &taskObj->testObj->drvObj[instID];
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        if(Udma_chGetDefaultFlowHandle(chHandle) == NULL_PTR)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: chGetDefaultFlowHandle:: "
                      " Pos:: Check for Valid args!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            UdmaChRxPrms_init(&rxChPrms, chType);
            retVal = Udma_chConfigRx(chHandle, &rxChPrms); 
            if(UDMA_SOK == retVal)
            {
                retVal = Udma_chGetStats(chHandle, &chStats);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: chGetStats:: Pos:: when instType"
                              " is UDMA_INST_TYPE_LCDMA_BCDMA and chType is UDMA_CH_TYPE_RX!!\n");
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
    taskObj->testObj->drvObj[instID] = backUpDrvObj;
    
    return retVal;
}

/* 
 * Test Case Description: Verifies the function Udma_chDisable when
 * Test scenario 1: Check when chType is UDMA_CH_TYPE_TX and instType is UDMA_INST_TYPE_NORMAL
 * Test scenario 2: Check when chType is UDMA_CH_TYPE_RX and instType is UDMA_INST_TYPE_NORMAL 
 */ 
int32_t UdmaChDisableTest(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    struct Udma_ChObj  chObj;
    Udma_ChHandle      chHandle;
    uint32_t           instID;
    struct Udma_DrvObj backUpDrvObj;
    Udma_DrvHandle     drvHandle;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    uint32_t           trigger;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChDisable Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when chType is UDMA_CH_TYPE_TX and instType is UDMA_INST_TYPE_NORMAL */
    chHandle         = &chObj;
    chType           = UDMA_CH_TYPE_TX;
    UdmaChPrms_init(&chPrms, chType);
    instID           = UDMA_TEST_DEFAULT_UDMA_INST;
    backUpDrvObj     = taskObj->testObj->drvObj[instID];
    drvHandle        = &taskObj->testObj->drvObj[instID];
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    Udma_ChTxPrms txChPrms;
    UdmaChTxPrms_init(&txChPrms, chType);
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigTx(chHandle, &txChPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(chHandle);
            if(UDMA_SOK == retVal)
            {
                Udma_chPause(chHandle);
                Udma_chResume(chHandle);
                trigger = CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL1;
                retVal  = Udma_chSetChaining(chHandle, chHandle, trigger);
                if(UDMA_SOK == retVal)
                {
                    retVal = Udma_chDisable(chHandle, UDMA_DEFAULT_CH_DISABLE_TIMEOUT);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: UdmachDisableTxChan:: Pos:: "
                                  " Check when chType is UDMA_CH_TYPE_TX and instType is UDMA_INST_TYPE_NORMAL!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                        retVal = Udma_chBreakChaining(chHandle, chHandle);
                        if(UDMA_SOK != retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: Udma_chBreakChaining:: Pos::"
                                      " Check when chType is UDMA_CH_TYPE_RX and instType is "
                                      " UDMA_INST_TYPE_NORMAL!!\n");
                            retVal = UDMA_EFAIL;
                        }
                        else
                        {
                            retVal = UDMA_SOK;
                        }
                    }
                }
            }
        }
        Udma_chClose(chHandle);
    }
    taskObj->testObj->drvObj[instID] = backUpDrvObj;

    /* Test scenario 2: Check when chType is UDMA_CH_TYPE_RX and instType is UDMA_INST_TYPE_NORMAL */
    chType           = UDMA_CH_TYPE_RX;
    UdmaChPrms_init(&chPrms, chType);
    instID           = UDMA_TEST_DEFAULT_UDMA_INST;
    backUpDrvObj     = taskObj->testObj->drvObj[instID];
    drvHandle        = &taskObj->testObj->drvObj[instID];
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    Udma_ChRxPrms rxChPrms;
    UdmaChRxPrms_init(&rxChPrms, chType);
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigRx(chHandle, &rxChPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(chHandle);
            if(UDMA_SOK == retVal)
            {
                Udma_chPause(chHandle);
                Udma_chResume(chHandle);
                trigger = CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL1;
                retVal  = Udma_chSetChaining(chHandle, chHandle, trigger);
                if(UDMA_SOK == retVal)
                {
                    retVal = Udma_chDisable(chHandle, UDMA_DEFAULT_CH_DISABLE_TIMEOUT);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: UdmachDisableTxChan:: Pos:: "
                                  " Check when chType is UDMA_CH_TYPE_RX and instType is UDMA_INST_TYPE_NORMAL!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                        retVal = Udma_chBreakChaining(chHandle, chHandle);
                        if(UDMA_SOK != retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: Udma_chBreakChaining:: Pos:: "
                                      " Check when chType is UDMA_CH_TYPE_RX and instType is "
                                      " UDMA_INST_TYPE_NORMAL!!\n");
                            retVal = UDMA_EFAIL;
                        }
                        else
                        {
                            retVal = UDMA_SOK;
                        }
                    }
                }
            }
        }
        Udma_chClose(chHandle);
    }
    taskObj->testObj->drvObj[instID] = backUpDrvObj;
   
    return retVal;
}

/* 
 * Test Case Description: Verifies the function Udma_chConfigPdma when
 * Test scenario 1: Check when instType is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_PDMA_TX
 * Test scenario 1: Check when instType is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_PDMA_RX
 */ 
int32_t UdmaChConfigPdmaTest(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal = UDMA_SOK;
    struct Udma_ChObj chObj;
    Udma_ChHandle     chHandle;
    Udma_ChTxPrms     txPrms;
    Udma_ChPdmaPrms   pdmaPrms;
    uint32_t          instID;
    Udma_DrvHandle    drvHandle;
    Udma_ChPrms       chPrms;
    uint32_t          chType;
    Udma_ChRxPrms     rxPrms;
    
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChConfigPdma Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when instType is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_PDMA_TX */
    chHandle         = &chObj;
    chType           = UDMA_CH_TYPE_PDMA_TX;
    UdmaChPrms_init(&chPrms, chType);
    instID           = UDMA_TEST_DEFAULT_UDMA_INST;
    drvHandle        = &taskObj->testObj->drvObj[instID];
    chPrms.peerChNum = CSL_PDMA_CH_MCU_MCAN0_CH0_TX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        /* Config RX channel */
        UdmaChTxPrms_init(&txPrms, UDMA_CH_TYPE_PDMA_TX);
        retVal = Udma_chConfigTx(chHandle, &txPrms);
        /* Config PDMA channel */
        UdmaChPdmaPrms_init(&pdmaPrms);
        retVal = Udma_chConfigPdma(chHandle, &pdmaPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: ChConfigPdma:: Neg:: "
                      " when instType is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_PDMA_TX!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
        Udma_chClose(chHandle);
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when instType is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_PDMA_RX */
        chHandle         = &chObj;
        chType           = UDMA_CH_TYPE_PDMA_RX;
        UdmaChPrms_init(&chPrms, chType);
        instID           = UDMA_TEST_DEFAULT_UDMA_INST;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        chPrms.peerChNum = CSL_PDMA_CH_MCU_MCAN0_CH0_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            /* Config RX channel */
            UdmaChRxPrms_init(&rxPrms, UDMA_CH_TYPE_PDMA_RX);
            retVal = Udma_chConfigRx(chHandle, &rxPrms);
            /* Config PDMA channel */
            UdmaChPdmaPrms_init(&pdmaPrms);
            retVal = Udma_chConfigPdma(chHandle, &pdmaPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                        " |TEST INFO|:: FAIL:: UDMA:: ChConfigPdma:: Neg:: "
                        " when instType is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_PDMA_RX!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
            Udma_chClose(chHandle);
        }
    }

    return retVal;
}

/* 
 * Test Case Description: Verifies the function Udma_chGetTdCqRingHandle 
 * Test scenario 1: Check for Valid args
 */
int32_t UdmaTestChGetTdCqRingHandle(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal = UDMA_SOK;
    Udma_ChHandle     chHandle;
    struct Udma_ChObj chObj;
    Udma_ChPrms       chPrms;
    uint32_t          chType;
    Udma_DrvHandle    drvHandle;
    uint32_t          instID;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChGetTdCqRingHandle Testcase ::\r\n",
              taskObj->taskId);

    /*Test scenario 1: Check for Valid args*/
    chHandle         = &chObj;
    instID           = UDMA_TEST_DEFAULT_UDMA_INST;
    chType           = UDMA_CH_TYPE_TX;
    UdmaChPrms_init(&chPrms, chType);
    drvHandle        = &taskObj->testObj->drvObj[instID];
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        if(Udma_chGetTdCqRingHandle(chHandle) != NULL_PTR)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: chGetTdCqRingHandle:: Pos::"
                      " Check for Valid args!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
        Udma_chClose(chHandle);
    }

    return retVal;
}

/* 
 * Test Case Description: Verifies the function Udma_chGetFqRingNum
 * Test scenario 1: Check Udma_chGetFqRingNum when valid args are passed
 * Test scenario 2: Check Udma_chOpen when chType is UDMA_CH_TYPE_RX 
 */
int32_t UdmaTestChFqRing(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal = UDMA_SOK;
    uint32_t          heapId = UTILS_MEM_HEAP_ID_MSMC;
    uint32_t          instID;
    Udma_ChHandle     chHandle;
    struct Udma_ChObj chObj;
    Udma_ChPrms       chPrms;
    uint32_t          elemCnt = 50U, ringMemSize;
    uint32_t          chType;
    Udma_DrvHandle    drvHandle;
    void              *ringMem = NULL;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChGetFqRingNum Testcase ::\r\n",
              taskObj->taskId);

    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }

    if(UDMA_SOK == retVal)
    {
        /*Test scenario 1: Check for Valid args*/
        chHandle                      = &chObj;
        instID                        = UDMA_TEST_DEFAULT_UDMA_INST;
        chType                        = UDMA_CH_TYPE_TX;
        UdmaChPrms_init(&chPrms, chType);
        drvHandle                     = &taskObj->testObj->drvObj[instID];
        chPrms.peerChNum              = UDMA_PSIL_CH_MCU_CPSW0_TX;
        chPrms.fqRingPrms.ringMem     = ringMem;
        chPrms.fqRingPrms.ringMemSize = ringMemSize;
        chPrms.fqRingPrms.elemCnt     = elemCnt;
        retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chGetFqRingNum(chHandle);
            if(retVal == UDMA_RING_INVALID)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: chGetFqRingNum:: Pos::"
                          " Check for Valid args!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
        }
        Udma_chClose(chHandle);
    }

    if(NULL != ringMem)
    {
        retVal += Utils_memFree(heapId, ringMem, ringMemSize);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
        }
    }

    /* Test scenario 2: Check Udma_chOpen when chType is UDMA_CH_TYPE_RX */
    ringMem = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }

    if(UDMA_SOK == retVal)
    {
        chHandle                      = &chObj;
        instID                        = UDMA_TEST_DEFAULT_UDMA_INST;
        chType                        = UDMA_CH_TYPE_RX;
        UdmaChPrms_init(&chPrms, chType);
        drvHandle                     = &taskObj->testObj->drvObj[instID];
        chPrms.peerChNum              = UDMA_PSIL_CH_MCU_CPSW0_RX;
        chPrms.fqRingPrms.ringMem     = ringMem;
        chPrms.fqRingPrms.ringMemSize = ringMemSize;
        chPrms.fqRingPrms.elemCnt     = elemCnt;
        retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                        " |TEST INFO|:: FAIL:: UDMA:: Udma_chOpen:: Pos::"
                        " Check when chType is UDMA_CH_TYPE_RX !!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
            Udma_chClose(chHandle);
        }
    }

    if(NULL != ringMem)
    {
        retVal += Utils_memFree(heapId, ringMem, ringMemSize);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
        }
    }

    return (retVal);
}

/*
 * Test Case Description: Verifies the function Udma_chGetSwTriggerRegister and Udma_chGetStats
 * Test scenario 1: Check when instType is chType is UDMA_CH_TYPE_TX
 */
int32_t UdmaTestChSetSwTriggerRegister(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    struct Udma_ChObj  chObj;
    Udma_ChHandle      chHandle;
    struct Udma_DrvObj backUpDrvObj;
    Udma_DrvHandle     drvHandle;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    uint32_t           timeout;
    uint32_t           trigger;
    Udma_ChStats       chStats;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChSetSwTriggerRegister positive Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when instType is chType is UDMA_CH_TYPE_TX */
    chHandle         = &chObj;
    timeout          = 0U;
    chType           = UDMA_CH_TYPE_TX;
    backUpDrvObj     = taskObj->testObj->drvObj[taskObj->chObj[0]->instId];
    drvHandle        = &taskObj->testObj->drvObj[taskObj->chObj[0]->instId];
    UdmaChPrms_init(&chPrms, chType);
    chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_TX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    trigger          = CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL1;
    Udma_ChTxPrms txChPrms;
    UdmaChTxPrms_init(&txChPrms, chType);
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigTx(chHandle, &txChPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chGetStats(chHandle, &chStats);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: chGetStats:: Pos:: when instType"
                          " is UDMA_INST_TYPE_LCDMA_BCDMA and chType is UDMA_CH_TYPE_TX!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = Udma_chEnable(chHandle);
                if(UDMA_SOK == retVal)
                {
                    retVal = Udma_chSetSwTrigger(chHandle, trigger);
                    if(UDMA_SOK != retVal)
                    {                
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: ChSetSwTriggerRegister::"
                                  " Pos:: Check when instType is UDMA_INST_TYPE_LCDMA_BCDMA "
                                  " and chType is UDMA_CH_TYPE_TX!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                    }
                    Udma_chDisable(chHandle, timeout);
                }
            }
        }
        Udma_chClose(chHandle);
    }
    taskObj->testObj->drvObj[taskObj->chObj[0]->instId] = backUpDrvObj;

    return retVal;
}

/* 
 * Test Case Description: Verifies the function Udma_chGetStats when
 * Test scenario 1: Check when instType is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_TX
 * Test scenario 2: Check Udma_chGetDefaultFlowHandle when instType is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_RX
 * Test scenario 3: Check Udma_chGetStats when instType is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_RX
 */
int32_t UdmaChGetStatsTest(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    struct Udma_ChObj  chObj;
    Udma_ChHandle      chHandle;
    Udma_ChStats       chStats;
    uint32_t           instID;
    Udma_DrvHandle     drvHandle;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    Udma_ChTxPrms      txChPrms;
    Udma_ChRxPrms      rxChPrms;

    /* Test scenario 1: Check when instType is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_TX */
    chHandle         = &chObj;
    chType           = UDMA_CH_TYPE_TX;
    UdmaChPrms_init(&chPrms, chType);
    instID           = UDMA_TEST_DEFAULT_UDMA_INST;
    drvHandle        = &taskObj->testObj->drvObj[instID];
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    UdmaChTxPrms_init(&txChPrms, chType);
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigTx(chHandle, &txChPrms);     
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chGetStats(chHandle, &chStats);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: chGetStats:: Pos:: when instType"
                          " is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_TX!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
        }
        Udma_chClose(chHandle);
    }

    /* Test scenario 2: Check Udma_chGetDefaultFlowHandle when instType is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_RX */
    chType           = UDMA_CH_TYPE_RX;
    UdmaChPrms_init(&chPrms, chType);
    drvHandle        = &taskObj->testObj->drvObj[instID];
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    UdmaChRxPrms_init(&rxChPrms, chType);
    if(UDMA_SOK == retVal)
    {
        if(Udma_chGetDefaultFlowHandle(chHandle) == NULL_PTR)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: chGetDefaultFlowHandle:: "
                      " Pos:: Check when instType is UDMA_INST_TYPE_NORMAL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = Udma_chConfigRx(chHandle, &rxChPrms); 
            if(UDMA_SOK == retVal)
            {
                /* Test scenario 3: Check Udma_chGetStats when instType is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_RX */
                retVal = Udma_chGetStats(chHandle, &chStats);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: chGetStats:: Pos:: when instType"
                              " is UDMA_INST_TYPE_NORMAL and chType is UDMA_CH_TYPE_RX!!\n");
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

/*
 * Test Case Description: Verifies the function Udma_chGetTriggerEvent when
 * Test scenario 1: Check when instType is UDMA_INST_TYPE_LCDMA_BCDMA
 */
int32_t UdmaChGetTriggerEventTest(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    struct Udma_ChObj  chObj;
    Udma_ChHandle      chHandle;
    uint32_t           instID;
    Udma_DrvHandle     drvHandle;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    Udma_ChRxPrms      rxChPrms;
    uint32_t           timeout = 0U;
    uint32_t           trigger;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChGetTriggerEvent Testcase ::\r\n",
              taskObj->taskId);

    chHandle = &chObj;
    chType   = UDMA_CH_TYPE_RX;
    UdmaChPrms_init(&chPrms, chType);
    instID           = UDMA_TEST_INST_ID_BCDMA_0;
    drvHandle        = &taskObj->testObj->drvObj[instID];
    /* CSI Rx module is not power-on state */
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    UdmaChRxPrms_init(&rxChPrms, chType);
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigRx(chHandle, &rxChPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(chHandle);
            if(UDMA_SOK == retVal)
            {
                trigger = CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL1;
                retVal  = Udma_chSetChaining(chHandle, chHandle, trigger);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: Udma_chGetTriggerEvent:: Pos::"
                              " Check when instType is UDMA_INST_TYPE_LCDMA_BCDMA!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = UDMA_SOK;
                }
                Udma_chDisable(chHandle, timeout);
                Udma_chBreakChaining(chHandle, chHandle);
            }
        }
        Udma_chClose(chHandle);
    }
    
    return retVal;
}

/* 
 * Test Case Description: Verifies the function Udma_chConfigRx
 * Test scenario 1: Check Udma_chConfigRx when ring is allocated 
 */
int32_t UdmaChConfigRx(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal = UDMA_SOK;
    uint32_t               instID;
    Udma_DrvHandle         drvHandle;
    struct Udma_ChObj      chObj;
    Udma_ChHandle          chHandle;
    Udma_ChPrms            chPrms;
    uint32_t               chType;
    uint32_t               heapIdSrc = UTILS_MEM_HEAP_ID_MSMC;
    uint32_t               elemCnt = 50U;

    /* Test scenario 1: Check Udma_chConfigRx when ring is allocated */
    chHandle                      = &chObj;
    chType                        = UDMA_CH_TYPE_RX;
    instID                        = UDMA_TEST_DEFAULT_UDMA_INST;
    drvHandle                     = &taskObj->testObj->drvObj[instID];
    UdmaChPrms_init(&chPrms, chType);
    chPrms.fqRingPrms.elemCnt     = elemCnt;
    chPrms.fqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
    chPrms.fqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.fqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    chPrms.cqRingPrms.elemCnt     = elemCnt;
    chPrms.cqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
    chPrms.cqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.cqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    chPrms.peerChNum              = UDMA_PSIL_CH_MCU_CPSW0_RX;
    retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    Udma_ChRxPrms rxChPrms;
    UdmaChRxPrms_init(&rxChPrms, chType);
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigRx(chHandle, &rxChPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chConfigRx failed!!\n");
        }
        Udma_chClose(chHandle);
    }
    if(NULL != chPrms.cqRingPrms.ringMem)
    {
        retVal  = Utils_memFree(heapIdSrc, chPrms.fqRingPrms.ringMem, chPrms.fqRingPrms.ringMemSize);
        retVal += Utils_memFree(heapIdSrc, chPrms.cqRingPrms.ringMem, chPrms.cqRingPrms.ringMemSize);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
        }
    }

    return retVal;
}

/**
 * Test Case Description: Verfies UDMA Channel Disable Polling
 * 1. Instance Type UDMA - Channel Type UTC
 * 2. Instance Type UDMA - Channel Type Rx
 * 3. Instance Type BCDMA - Channel Type Rx
 * 4. Instance Type UDMA - Channel Type Tx
 * 5. Instance Type BCDMA - Channel Type Tx
*/
int32_t UdmaTestChDisablePolling(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal = UDMA_SOK;
    uint32_t               instID;
    Udma_DrvHandle         drvHandle;
    struct Udma_ChObj      chObj;
    Udma_ChHandle          chHandle;
    Udma_ChPrms            chPrms;
    uint32_t               chType;
    uint32_t               heapIdSrc = UTILS_MEM_HEAP_ID_MSMC;
    uint32_t               elemCnt = 50U;
    uint32_t               timeOut = 0xFFFFU;
    Udma_ChRxPrms          rxChPrms;
    Udma_ChTxPrms          txChPrms;

#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
#if (UDMA_NUM_UTC_INSTANCE > 0)
    if(UDMA_SOK == retVal)
    {
        /* 1. Instance Type UDMA - Channel Type UTC */
        Udma_ChUtcPrms         utcPrms;
        GT_0trace(taskObj->traceMask, GT_INFO, "Test Udma_chDisablePolling: Instance Type UDMA - Channel Type UTC \n");
        chHandle                      = &chObj;
        chType                        = UDMA_CH_TYPE_UTC;
        instID                        = UDMA_INST_ID_MAIN_0;
        drvHandle                     = &taskObj->testObj->drvObj[instID];
        /* Init channel parameters */
        chType = UDMA_CH_TYPE_UTC;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.utcId                = UDMA_UTC_ID_MSMC_DRU0;
        /* Ring not used in direct TR submission via DRU */
        chPrms.fqRingPrms.ringMem   = NULL;
        chPrms.cqRingPrms.ringMem   = NULL;
        chPrms.tdCqRingPrms.ringMem = NULL;
        chPrms.fqRingPrms.elemCnt   = 0U;
        chPrms.cqRingPrms.elemCnt   = 0U;
        chPrms.tdCqRingPrms.elemCnt = 0U;
        retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            /* Config UTC channel */
            UdmaChUtcPrms_init(&utcPrms);
            utcPrms.druOwner    = CSL_DRU_OWNER_DIRECT_TR;
            utcPrms.druQueueId  = CSL_DRU_QUEUE_ID_3;
            retVal = Udma_chConfigUtc(chHandle, &utcPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, "[Error] UDMA UTC channel config failed!!\n");
            }
            else
            {
                retVal = Udma_chEnable(chHandle);

                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chEnable failed!!\n");
                }
                else
                {
                    retVal = Udma_chDisablePolling(chHandle, timeOut);
                }

                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chDisablePolling failed!!\n");
                }
            }
            Udma_chClose(chHandle);
        }
    }
#endif
#endif

#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    if(UDMA_SOK == retVal)
    {
        /* 2. Instance Type UDMA - Channel Type Rx */
        GT_0trace(taskObj->traceMask, GT_INFO, "Test Udma_chDisablePolling: Instance Type UDMA - Channel Type Rx \n");
        chHandle                      = &chObj;
        chType                        = UDMA_CH_TYPE_RX;
        instID                        = UDMA_TEST_DEFAULT_UDMA_INST;
        drvHandle                     = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.fqRingPrms.elemCnt     = elemCnt;
        chPrms.fqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
        chPrms.fqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.fqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        chPrms.cqRingPrms.elemCnt     = elemCnt;
        chPrms.cqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
        chPrms.cqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.cqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        chPrms.peerChNum              = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChRxPrms_init(&rxChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigRx(chHandle, &rxChPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chConfigRx failed!!\n");
            }
            else
            {
                retVal = Udma_chEnable(chHandle);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chEnable failed!!\n");
                }
                else
                {
                    retVal = Udma_chDisablePolling(chHandle, timeOut);
                }

                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chDisablePolling failed!!\n");
                }
            }
            Udma_chClose(chHandle);
        }
            if(NULL != chPrms.cqRingPrms.ringMem)
            {
                retVal += Utils_memFree(heapIdSrc, chPrms.fqRingPrms.ringMem, chPrms.fqRingPrms.ringMemSize);
                retVal += Utils_memFree(heapIdSrc, chPrms.cqRingPrms.ringMem, chPrms.cqRingPrms.ringMemSize);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
                }
            }
    }
#endif

#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1) && defined (BUILD_MCU2_0)
    if(UDMA_SOK == retVal)
    {
        /* 3. Instance Type BCDMA - Channel Type Rx */
        GT_0trace(taskObj->traceMask, GT_INFO, "Test Udma_chDisablePolling: Instance Type BCDMA - Channel Type Rx \n");
        chHandle                      = &chObj;
        chType                        = UDMA_CH_TYPE_RX;
        instID                        = UDMA_INST_ID_BCDMA_0;
        drvHandle                     = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.fqRingPrms.elemCnt     = elemCnt;
        chPrms.fqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
        chPrms.fqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.fqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        chPrms.cqRingPrms.elemCnt     = elemCnt;
        chPrms.cqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
        chPrms.cqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.cqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        chPrms.peerChNum              = CSL_PSIL_CSI_RX0_CH0;
        retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            UdmaChRxPrms_init(&rxChPrms, chType);
            retVal = Udma_chConfigRx(chHandle, &rxChPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chConfigRx failed!!\n");
            }
            else
            {
                retVal = Udma_chEnable(chHandle);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chEnable failed!!\n");
                }
                else
                {
                    retVal = Udma_chDisablePolling(chHandle, timeOut);
                }

                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chDisablePolling failed!!\n");
                }
            }
            Udma_chClose(chHandle);
        }
        if(NULL != chPrms.cqRingPrms.ringMem)
        {
            retVal += Utils_memFree(heapIdSrc, chPrms.fqRingPrms.ringMem, chPrms.fqRingPrms.ringMemSize);
            retVal += Utils_memFree(heapIdSrc, chPrms.cqRingPrms.ringMem, chPrms.cqRingPrms.ringMemSize);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
            }
        }
    }
#endif

#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    if(UDMA_SOK == retVal)
    {
        /* 4. Instance Type UDMA - Channel Type Tx */
        GT_0trace(taskObj->traceMask, GT_INFO, "Test Udma_chDisablePolling: Instance Type UDMA - Channel Type Tx \n");
        chHandle                      = &chObj;
        chType                        = UDMA_CH_TYPE_TX;
        instID                        = UDMA_TEST_DEFAULT_UDMA_INST;
        drvHandle                     = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.fqRingPrms.elemCnt     = elemCnt;
        chPrms.fqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
        chPrms.fqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.fqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        chPrms.cqRingPrms.elemCnt     = elemCnt;
        chPrms.cqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
        chPrms.cqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.cqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        chPrms.peerChNum              = UDMA_PSIL_CH_MCU_CPSW0_TX;
        retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChTxPrms_init(&txChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigTx(chHandle, &txChPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chConfigRx failed!!\n");
            }
            else
            {
                retVal = Udma_chEnable(chHandle);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chEnable failed!!\n");
                }
                else
                {
                    retVal = Udma_chDisablePolling(chHandle, timeOut);
                }

                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chDisablePolling failed!!\n");
                }
            }
            Udma_chClose(chHandle);
        }
            if(NULL != chPrms.cqRingPrms.ringMem)
            {
                retVal += Utils_memFree(heapIdSrc, chPrms.fqRingPrms.ringMem, chPrms.fqRingPrms.ringMemSize);
                retVal += Utils_memFree(heapIdSrc, chPrms.cqRingPrms.ringMem, chPrms.cqRingPrms.ringMemSize);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
                }
            }
    }
#endif

#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1) && defined (BUILD_MCU2_0)
    if(UDMA_SOK == retVal)
    {
        /* 5. Instance Type BCDMA - Channel Type Tx */
        GT_0trace(taskObj->traceMask, GT_INFO, "Test Udma_chDisablePolling: Instance Type BCDMA - Channel Type Tx \n");
        chHandle                      = &chObj;
        chType                        = UDMA_CH_TYPE_TX;
        instID                        = UDMA_INST_ID_BCDMA_0;
        drvHandle                     = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.fqRingPrms.elemCnt     = elemCnt;
        chPrms.fqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
        chPrms.fqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.fqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        chPrms.cqRingPrms.elemCnt     = elemCnt;
        chPrms.cqRingPrms.ringMemSize = elemCnt * sizeof(uint64_t);
        chPrms.cqRingPrms.ringMem     = Utils_memAlloc(heapIdSrc, chPrms.cqRingPrms.ringMemSize, UDMA_CACHELINE_ALIGNMENT);
        chPrms.peerChNum              = UDMA_PSIL_CH_MAIN_CSI_TX;
        retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChTxPrms_init(&txChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigTx(chHandle, &txChPrms);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chConfigRx failed!!\n");
            }
            else
            {
                retVal = Udma_chEnable(chHandle);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chEnable failed!!\n");
                }
                else
                {
                    retVal = Udma_chDisablePolling(chHandle, timeOut);
                }

                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Udma_chDisablePolling failed!!\n");
                }
            }
            Udma_chClose(chHandle);
        }
            if(NULL != chPrms.cqRingPrms.ringMem)
            {
                retVal += Utils_memFree(heapIdSrc, chPrms.fqRingPrms.ringMem, chPrms.fqRingPrms.ringMemSize);
                retVal += Utils_memFree(heapIdSrc, chPrms.cqRingPrms.ringMem, chPrms.cqRingPrms.ringMemSize);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Ring free failed!!\n");
                }
            }
    }
#endif

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_chDisablePolling when
 * Test scenario 1: Null check for chHandle
 * Test scenario 2: Check when chInitDone is not UDMA_INIT_DONE
 * Test scenario 3: Null check for drvHandle
 * Test scenario 4: Check when drvInitDone is not UDMA_INIT_DONE
*/
int32_t UdmaTestChDisablePollingNeg(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    struct Udma_ChObj  chObj;
    Udma_ChHandle      chHandle;
    uint32_t           timeout = 0U;

    GT_0trace(taskObj->traceMask, GT_INFO, "Neg Test: Udma_chDisablePolling: Null check for chHandle \n");

    /* Test scenario 1: Null check for chHandle */
    retVal = Udma_chDisablePolling(NULL, timeout);
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                    " |TEST INFO|:: FAIL:: UDMA:: chDisablePolling:: Neg:: "
                    " Null check for chHandle!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }

    /* Test scenario 2: Check when chInitDone is not UDMA_INIT_DONE */
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_INFO, "Neg Test: Udma_chDisablePolling: Check when chInitDone is not UDMA_INIT_DONE \n");
        chHandle             = &chObj;
        chHandle->chInitDone = UDMA_DEINIT_DONE;
        retVal = Udma_chDisablePolling(chHandle, timeout);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                        " |TEST INFO|:: FAIL:: UDMA:: chDisablePolling:: Neg:: "
                        " Check when chInitDone is not UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 3: Null check for drvHandle */
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_INFO, "Neg Test: Udma_chDisablePolling: Null check for drvHandle \n");
        chHandle->drvHandle = NULL;
        retVal = Udma_chDisablePolling(chHandle, timeout);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                        " |TEST INFO|:: FAIL:: UDMA:: chDisablePolling:: Neg:: "
                        " Null check for drvHandle!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 4: Check when drvInitDone is not UDMA_INIT_DONE */
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_INFO, "Neg Test: Udma_chDisablePolling: Check when drvInitDone is not UDMA_INIT_DONE \n");
        chHandle->drvHandle = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        uint32_t drvInitDone = chHandle->drvHandle->drvInitDone;
        chHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal = Udma_chDisablePolling(chHandle, timeout);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                        " |TEST INFO|:: FAIL:: UDMA:: chDisablePolling:: Neg:: "
                        " Check when drvInitDone is not UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
        chHandle->drvHandle->drvInitDone = drvInitDone;
    }

    return retVal;
}
