/*
 *  Copyright (c) Texas Instruments Incorporated 2024
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
 *  \file udma_test_csl_ringacc.c
 *
 *  \brief File containing test cases for UDMA Csl Ringacc related APIs.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <udma_test.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

static int32_t UdmaTestInitRingAcc(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle, uint32_t elemCnt);
static int32_t UdmaTestDeInitRingAcc(Udma_ChHandle chHandle);
static void memOpsFxn(void *addr, uint32_t size, uint32_t type);

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void memOpsFxn(void *addr, uint32_t size, uint32_t type)
{
    return;
}

/*
 * Test Case Description: Verifies the csl ring functions with below test scenario's
 * 1)Test scenario 1: Test CSL_ringaccGetRevision.
 * 2)Test scenario 2: Test CSL_ringaccInitRingCfg.
 * 3)Test scenario 3: Test CSL_ringaccGetRingNum.
 * 4)Test scenario 4: Test CSL_ringaccGetCmdRingPtr
 * 5)Test scenario 5: Test CSL_ringaccAckRspRing when cnt is 0
 * 6)Test scenario 6: Test CSL_ringaccGetRingIdx
 * 7)Test scenario 7: Test CSL_ringaccGetRingHwIdx
 * 8)Test scenario 8: Test CSL_ringaccGetRingOcc
 * 9)Test scenario 9: Test CSL_ringaccGetRingHwOcc
 * 10)Test scenario 10: Test CSL_ringaccPush64 CSL_ringaccPeek64 CSL_ringaccPop64
 * 11)Test scenario 11: Test CSL_ringaccPeekData CSL_ringaccWrData CSL_ringaccRdData
 * 12)Test scenario 12: Test CSL_ringaccWrData CSL_ringaccRdData passing pfMemOps as NULL
 */
int32_t udmaTestCslRingAcc(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal    = UDMA_SOK;
    Udma_DrvHandle      drvHandle = &taskObj->testObj->drvObj[UDMA_TEST_DEFAULT_UDMA_INST];
    struct Udma_ChObj   chObj;
    Udma_ChHandle       chHandle  = &chObj;
    uint32_t            elemCnt   = 10U;
    uint32_t            ringMemSize = elemCnt * sizeof (uint64_t);
    uint64_t           *ringMem = Utils_memAlloc(UTILS_MEM_HEAP_ID_DDR, ringMemSize, UDMA_CACHELINE_ALIGNMENT);

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: CSL RingAcc Test ::\r\n", taskObj->taskId);

    retVal = UdmaTestInitRingAcc(drvHandle, chHandle, elemCnt);

    /* Test scenario 1: Test CSL_ringaccGetRevision */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_ringaccGetRevision Testcase ::\r\n", taskObj->taskId);
        uint32_t rev = CSL_ringaccGetRevision(&drvHandle->raRegs);
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: RING ACC VERSION: %u::\r\n", rev);
    }
    /* Test scenario 2: Test CSL_ringaccInitRingCfg */
    if(UDMA_SOK == retVal)
    {
        CSL_RingAccRingCfg ringCfg;
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_ringaccInitRingCfg Testcase ::\r\n", taskObj->taskId);
        CSL_ringaccInitRingCfg(&ringCfg);
    }
    /* Test scenario 3: Test CSL_ringaccGetRingNum */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_ringaccGetRingNum Testcase ::\r\n", taskObj->taskId);
        uint32_t ringNum = CSL_ringaccGetRingNum(&chHandle->fqRing->cfg);
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Ring Number: %u::\r\n", ringNum);
    }
    /* Test scenario 4: Test CSL_ringaccGetCmdRingPtr */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_ringaccGetCmdRingPtr Testcase ::\r\n", taskObj->taskId);
        uint32_t loopCnt = 0U;
        while((UDMA_SOK == retVal) && (loopCnt < 100))
        {
            retVal = Udma_ringQueueRaw(chHandle->fqRing, (uint64_t)ringMem);
            if(UDMA_SOK == retVal)
            {
                loopCnt += 1;
            }
        }
        if(loopCnt <= elemCnt)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: CSL_ringaccGetCmdRingPtr Testcase ::\r\n", taskObj->taskId);
        }
        loopCnt = 0U;
        while((UDMA_SOK == retVal) && (loopCnt < 100))
        {
            retVal = Udma_ringFlushRaw(chHandle->fqRing, ringMem);
            if(UDMA_SOK == retVal)
            {
                loopCnt += 1;
            }
        }
        if(loopCnt <= elemCnt)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: CSL_ringaccGetCmdRingPtr Testcase: Udma_ringFlushRaw failed ::\r\n", taskObj->taskId);
        }
    }
    /* Test scenario 5: Test CSL_ringaccAckRspRing when cnt is 0 */
    if(UDMA_SOK == retVal)
    {
        CSL_RingAccRingCfg ringCfg;
        ringCfg.ringNum = UDMA_RING_INVALID;
        ringCfg.waiting = 0U;
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_ringaccAckRspRing Testcase ::\r\n", taskObj->taskId);
        CSL_ringaccAckRspRing(&drvHandle->raRegs, &ringCfg, 0U);
        ringCfg.waiting += 1U;
        CSL_ringaccAckRspRing(&drvHandle->raRegs, &ringCfg, 0U);
    }
    /* Test scenario 6: Test CSL_ringaccGetRingIdx */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_ringaccGetRingIdx Testcase ::\r\n", taskObj->taskId);
        CSL_ringaccGetRingIdx(&drvHandle->raRegs, chHandle->fqRing->ringNum);
        CSL_ringaccGetRingIdx(&drvHandle->raRegs, UDMA_RING_INVALID);
    }
    /* Test scenario 7: Test CSL_ringaccGetRingHwIdx */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_ringaccGetRingHwIdx Testcase ::\r\n", taskObj->taskId);
        CSL_ringaccGetRingHwIdx(&drvHandle->raRegs, chHandle->fqRing->ringNum);
        CSL_ringaccGetRingHwIdx(&drvHandle->raRegs, UDMA_RING_INVALID);
    }
    /* Test scenario 8: Test CSL_ringaccGetRingOcc */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_ringaccGetRingOcc Testcase ::\r\n", taskObj->taskId);
        CSL_ringaccGetRingOcc(&drvHandle->raRegs, UDMA_RING_INVALID);
    }
    /* Test scenario 9: Test CSL_ringaccGetRingHwOcc */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_ringaccGetRingHwIdx Testcase ::\r\n", taskObj->taskId);
        CSL_ringaccGetRingHwOcc(&drvHandle->raRegs, UDMA_RING_INVALID);
    }
    /* Test scenario 10: Test CSL_ringaccPush64 CSL_ringaccPeek64 CSL_ringaccPop64*/
    if(UDMA_SOK == retVal)
    {
        CSL_RingAccRingCfg ringCfg;
        ringCfg.elSz = 0U;
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_ringaccGetRingHwIdx Testcase ::\r\n", taskObj->taskId);
        /* Test CSL_ringaccPeek64 when ring is empty */
        CSL_ringaccPeek64(&drvHandle->raRegs, &chHandle->fqRing->cfg, ringMem, NULL);
        /* Test CSL_ringaccPush64 when CSL_ringaccMemOpsFxnPtr is NULL */
        CSL_ringaccPush64(&drvHandle->raRegs, &chHandle->fqRing->cfg, ringMem[0], NULL);
        /* Test CSL_ringaccPush64 when ring elsz < sizeof(uint64_t) */
        CSL_ringaccPush64(&drvHandle->raRegs, &ringCfg, ringMem[0], NULL);
        /* Test CSL_ringaccPeek64 when CSL_ringaccMemOpsFxnPtr is NULL */
        CSL_ringaccPeek64(&drvHandle->raRegs, &chHandle->fqRing->cfg, ringMem, NULL);
        /* Test CSL_ringaccPeek64 when CSL_ringaccMemOpsFxnPtr is not NULL */
        CSL_ringaccPeek64(&drvHandle->raRegs, &chHandle->fqRing->cfg, ringMem, memOpsFxn);
        /* Test CSL_ringaccPeek64 when ring elsz < sizeof(uint64_t) */
        CSL_ringaccPeek64(&drvHandle->raRegs, &ringCfg, ringMem, NULL);
        /* Test CSL_ringaccPop64 when CSL_ringaccMemOpsFxnPtr is NULL */
        CSL_ringaccPop64(&drvHandle->raRegs, &chHandle->fqRing->cfg, ringMem, NULL);
    }

    /* Test scenario 11: Test CSL_ringaccPeekData CSL_ringaccWrData CSL_ringaccRdData */
    if(UDMA_SOK == retVal)
    {
        CSL_RingAccRingCfg ringCfg;
        ringCfg.elSz = 0U;
        ringCfg.mode = TISCI_MSG_VALUE_RM_RING_MODE_RING;
        GT_1trace(taskObj->traceMask, GT_INFO1,
        " |TEST INFO|:: Task:%d: CSL_ringaccGetRingHwIdx Testcase ::\r\n", taskObj->taskId);
        /* Test CSL_ringaccPeekData when ring is empty */
        CSL_ringaccPeekData(&drvHandle->raRegs, &chHandle->fqRing->cfg, (uint8_t *)ringMem, 1U, NULL);
        /* Test CSL_ringaccWrData when CSL_ringaccMemOpsFxnPtr is not NULL */
        CSL_ringaccWrData(&drvHandle->raRegs, &chHandle->fqRing->cfg, (uint8_t *)ringMem, 1U, memOpsFxn);
        /* Test CSL_ringaccWrData when ring elsz < sizeof(uint64_t) */
        CSL_ringaccWrData(&drvHandle->raRegs, &ringCfg, (uint8_t *)ringMem, 1U, NULL);
        /* Test CSL_ringaccPeekData when CSL_ringaccMemOpsFxnPtr is NULL */
        CSL_ringaccPeekData(&drvHandle->raRegs, &chHandle->fqRing->cfg, (uint8_t *)ringMem, 1U, NULL);
        /* Test CSL_ringaccPeekData when CSL_ringaccMemOpsFxnPtr is not NULL */
        CSL_ringaccPeekData(&drvHandle->raRegs, &chHandle->fqRing->cfg, (uint8_t *)ringMem, 1U, memOpsFxn);
        /* Test CSL_ringaccPeekData when ring elsz < sizeof(uint64_t) */
        CSL_ringaccPeekData(&drvHandle->raRegs, &ringCfg, (uint8_t *)ringMem, 1U, NULL);
        /* Test CSL_ringaccRdData when CSL_ringaccMemOpsFxnPtr is not NULL */
        CSL_ringaccRdData(&drvHandle->raRegs, &chHandle->fqRing->cfg, (uint8_t *)ringMem, 1U, memOpsFxn);
    }

    retVal = UdmaTestDeInitRingAcc(chHandle);

    /* Test scenario 12: Test CSL_ringaccWrData CSL_ringaccRdData passing pfMemOps as NULL */
    retVal = UdmaTestInitRingAcc(drvHandle, chHandle, elemCnt);
    {
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
            " |TEST INFO|:: Task:%d: CSL_ringaccWrData  CSL_ringaccRdData Testcase ::\r\n", taskObj->taskId);
            CSL_ringaccPeekData(&drvHandle->raRegs, &chHandle->fqRing->cfg, (uint8_t *)ringMem, 1U, NULL);
            /* Test CSL_ringaccWrData when CSL_ringaccMemOpsFxnPtr is NULL */
            CSL_ringaccWrData(&drvHandle->raRegs, &chHandle->fqRing->cfg, (uint8_t *)ringMem, 1U, NULL);
            /* Test CSL_ringaccRdData when CSL_ringaccMemOpsFxnPtr is NULL */
            CSL_ringaccRdData(&drvHandle->raRegs, &chHandle->fqRing->cfg, (uint8_t *)ringMem, 1U, NULL);
        }
    }
    retVal = UdmaTestDeInitRingAcc(chHandle);

    return (retVal);
}


static int32_t UdmaTestInitRingAcc(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle, uint32_t elemCnt)
{
    int32_t           retVal  = UDMA_SOK;
    uint32_t          heapId  = UTILS_MEM_HEAP_ID_DDR;
    void             *ringMem = NULL;
    uint32_t          chType;
    Udma_ChPrms       chPrms;
    uint32_t          ringMemSize;
    uint32_t          ringMode;
    Udma_ChTxPrms     txChPrms;

    /* Allocate Ring Memory in heap */
    ringMode    = TISCI_MSG_VALUE_RM_RING_MODE_RING;
    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem     = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
    }

    /* Open Channel */
    if(UDMA_SOK == retVal)
    {
        chType                        = UDMA_CH_TYPE_TX;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum              = UDMA_PSIL_CH_MCU_CPSW0_TX;
        chPrms.fqRingPrms.ringMem     = ringMem;
        chPrms.fqRingPrms.ringMemSize = ringMemSize;
        chPrms.fqRingPrms.elemCnt     = elemCnt;
        chPrms.fqRingPrms.mode        = ringMode;
        retVal                        = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            UdmaChTxPrms_init(&txChPrms, chType);
            retVal = Udma_chConfigTx(chHandle, &txChPrms);
        }
    }

    return retVal;
}

static int32_t UdmaTestDeInitRingAcc(Udma_ChHandle chHandle)
{
    int32_t           retVal  = UDMA_SOK;
    uint32_t          heapId  = UTILS_MEM_HEAP_ID_DDR;

    /* Free Ring Memory in heap */
    retVal = Utils_memFree(heapId, chHandle->chPrms.fqRingPrms.ringMem, chHandle->chPrms.fqRingPrms.ringMemSize);

    /* Close Channel */
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chClose(chHandle);
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the csl ring functions with below test scenario's
 * 1)Test scenario 1: Check CSL_ringaccPeek64 mode is not CSL_RINGACC_RING_MODE_RING
 * 2)Test scenario 2: Check CSL_ringaccPeek64 to get pRingEntry equal to NULL
 * 3)Test scenario 3: Check CSL_ringaccPop64 mode is not CSL_RINGACC_RING_MODE_RING
 * 4)Test scenario 4: Check CSL_ringaccWrData mode is not CSL_RINGACC_RING_MODE_RING
 * 5)Test scenario 5: Check CSL_ringaccReadRingMonitor mode is not CSL_RINGACC_RING_MODE_RING
 * 6)Test scenario 6: Check CSL_ringaccPeek64 to get pRingEntry equal to NULL
 * 7)Test scenario 7: Check CSL_ringaccRdData when elsz count is more than numBytes
 * 8)Test scenario 8: Check CSL_ringaccRdData to get pRingEntry is NULL
 * 9)Test scenario 9: Check CSL_ringaccPeekData to get pRingEntry is NULL
 * 10)Test scenario 10: Check CSL_ringaccWrData to get pRingEntry is NULL
 * 11)Test scenario 11: Check CSL_ringaccPop64 mode is not CSL_RINGACC_RING_MODE_RING
 */
int32_t UdmaTestCslRing(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal  = UDMA_SOK;
    uint32_t             elemCnt = 50U;
    uint32_t             heapId  = UTILS_MEM_HEAP_ID_MSMC;
    uint64_t            *ringMem = NULL;
    Udma_DrvHandle       drvHandle;
    Udma_RingPrms        ringPrms;
    struct Udma_RingObj  ringObj;
    Udma_RingHandle      ringHandle;
    uint32_t             ringMemSize;
    ringHandle  = &ringObj;
    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem     = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }

    if(UDMA_SOK == retVal)
    {
        drvHandle  = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
        UdmaRingPrms_init(&ringPrms);
        ringPrms.ringMem     = ringMem;
        ringPrms.ringMemSize = ringMemSize;
        ringPrms.mode        = CSL_RINGACC_RING_MODE_MESSAGE;
        ringPrms.elemCnt     = elemCnt;
        ringPrms.elemSize    = UDMA_RING_ES_8BYTES;

        /* Allocate a free ring */
        retVal = Udma_ringAlloc(drvHandle, ringHandle, UDMA_RING_ANY, &ringPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring alloc failed!!\n");
        }

        /* Test scenario 1: Check CSL_ringaccPeek64 mode is not CSL_RINGACC_RING_MODE_RING */
        retVal = CSL_ringaccPeek64(&drvHandle->raRegs, &ringHandle->cfg, ringMem, NULL);
        if(UDMA_SOK == retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccPeek64 ::"
                      " Check CSL_ringaccPeek64 mode is not CSL_RINGACC_RING_MODE_RING \r\n",
                      taskObj->taskId);
            retVal = UDMA_EFAIL;
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 2: Check CSL_ringaccPeekData mode is not CSL_RINGACC_RING_MODE_RING */
            retVal = CSL_ringaccPeekData(&drvHandle->raRegs, &ringHandle->cfg, (uint8_t *)ringMem, 1U, NULL);
            if(UDMA_SOK != retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccPeekData \r\n",
                          taskObj->taskId);
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 3: Check CSL_ringaccRdData mode is not CSL_RINGACC_RING_MODE_RING */
            retVal = CSL_ringaccRdData(&drvHandle->raRegs, &ringHandle->cfg, (uint8_t *)ringMem, 1U, NULL);
            if(UDMA_SOK != retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccRdData \r\n",
                          taskObj->taskId);
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 4: Check CSL_ringaccWrData mode is not CSL_RINGACC_RING_MODE_RING */
            retVal = CSL_ringaccWrData(&drvHandle->raRegs, &ringHandle->cfg, (uint8_t *)ringMem, 1U, NULL);
            if(UDMA_SOK != retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccWrData \r\n",
                          taskObj->taskId);
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 5: Check CSL_ringaccReadRingMonitor mode is not CSL_RINGACC_RING_MODE_RING */
            retVal = CSL_ringaccReadRingMonitor(&drvHandle->raRegs, UDMA_RING_ORDERID_MAX, NULL, NULL);
            if(UDMA_SOK != retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccReadRingMonitor \r\n",
                          taskObj->taskId);
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 6: Check CSL_ringaccPeek64 to get pRingEntry equal to NULL */
            ringHandle->cfg.mode = CSL_RINGACC_RING_MODE_RING;
            retVal = CSL_ringaccPeek64(&drvHandle->raRegs, &ringHandle->cfg, ringMem, NULL);
            if(UDMA_SOK != retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccPeek64 ::"
                          " Check CSL_ringaccPeek64 to get pRingEntry equal to NULL \r\n",
                          taskObj->taskId);
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 7: Check CSL_ringaccRdData when elsz count is more than numBytes */
            ringHandle->cfg.mode = CSL_RINGACC_RING_MODE_RING;
            retVal = CSL_ringaccRdData(&drvHandle->raRegs, &ringHandle->cfg, (uint8_t *)ringMem, UDMA_RING_ANY, NULL);
            if(UDMA_SOK != retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccRdData"
                          " Check CSL_ringaccRdData when elsz count is more than numBytes \r\n",
                          taskObj->taskId);
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 8: Check CSL_ringaccRdData to get pRingEntry is NULL */
            ringHandle->cfg.mode = CSL_RINGACC_RING_MODE_RING;
            retVal = CSL_ringaccRdData(&drvHandle->raRegs, &ringHandle->cfg, (uint8_t *)ringMem, 1U, NULL);
            if(UDMA_SOK != retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccRdData \r\n",
                          taskObj->taskId);
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 9: Check CSL_ringaccPeekData to get pRingEntry is NULL */
            ringHandle->cfg.mode     = CSL_RINGACC_RING_MODE_RING;
            ringHandle->cfg.rwIdx    = 0;
            ringHandle->cfg.virtBase = 0;
            retVal = CSL_ringaccPeekData(&drvHandle->raRegs, &ringHandle->cfg, (uint8_t *)ringMem, 1U, NULL);
            if(UDMA_SOK == retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccPeekData \r\n",
                          taskObj->taskId);
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 10: Check CSL_ringaccWrData to get pRingEntry is NULL */
            ringHandle->cfg.mode  = CSL_RINGACC_RING_MODE_RING;
            retVal = CSL_ringaccWrData(&drvHandle->raRegs, &ringHandle->cfg, (uint8_t *)ringMem, 1U, NULL);
            if(UDMA_SOK != retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccWrData \r\n",
                          taskObj->taskId);
                retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 11: Check CSL_ringaccPop64 mode is not CSL_RINGACC_RING_MODE_RING */
            ringHandle->cfg.mode = CSL_RINGACC_RING_MODE_MESSAGE;
            retVal = CSL_ringaccPop64(&drvHandle->raRegs, &ringHandle->cfg, ringMem, NULL);
            if(UDMA_SOK == retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccPop64 ::"
                          " mode is not CSL_RINGACC_RING_MODE_RING \r\n",
                          taskObj->taskId);
                retVal = UDMA_EFAIL;
            }
        }
    }
    Udma_ringFree(ringHandle);

    return retVal;
}

/*
 * Test Case Description: Verifies the csl ring monitor functions with below test scenario's
 * 1)Test scenario 1: Check Check CSL_ringaccReadRingMonitor API
 * 2)Test scenario 2: Check maxMonitors is zero
 * 3)Test scenario 3: Check CSL_ringaccReadRingMonitor for invalid monNum
 * 4)Test scenario 4: Check CSL_ringaccReadRingMonitor when pData0 and pData1 are NULL
 */
int32_t UdmaTestCslRingMonitor(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal  = UDMA_SOK;
    uint32_t               elemCnt = 500U;
    uint32_t               heapId  = UTILS_MEM_HEAP_ID_MSMC;
    void                  *ringMem = NULL;
    uint32_t               instId,ringMode;
    uint32_t               ringMemSize;
    Udma_DrvHandle         drvHandle;
    Udma_RingPrms          ringPrms;
    struct Udma_RingObj    ringObj;
    Udma_RingHandle        ringHandle = &ringObj;
    struct Udma_RingMonObj ringMonObj;
    Udma_RingMonHandle     monHandle = &ringMonObj;
    Udma_RingMonPrms       monPrms;
    uint32_t               backUpMaxmonitors;
    uint32_t               data0, data1;

    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }

    if(UDMA_SOK == retVal)
    {
        instId = UDMA_TEST_DEFAULT_UDMA_INST;
        drvHandle = &taskObj->testObj->drvObj[instId];
        ringMode = TISCI_MSG_VALUE_RM_RING_MODE_RING;

        UdmaRingPrms_init(&ringPrms);
        ringPrms.ringMem     = ringMem;
        ringPrms.ringMemSize = ringMemSize;
        ringPrms.mode        = ringMode;
        ringPrms.elemCnt     = elemCnt;

        /* Allocate a free ring */
        retVal = Udma_ringAlloc(drvHandle, ringHandle, UDMA_RING_ANY, &ringPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring alloc failed!!\n");
        }

        /* Allocate and configure a ring monitor */
        retVal = Udma_ringMonAlloc(drvHandle, monHandle, UDMA_RING_MON_ANY);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring monitor alloc failed!!\n");
        }
        UdmaRingMonPrms_init(&monPrms);
        monPrms.source  = TISCI_MSG_VALUE_RM_MON_SRC_ELEM_CNT;
        monPrms.mode    = TISCI_MSG_VALUE_RM_MON_MODE_PUSH_POP;
        monPrms.ringNum = Udma_ringGetNum(ringHandle);
        retVal = Udma_ringMonConfig(monHandle, &monPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring monitor config failed!!\n");
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 1: Check CSL_ringaccReadRingMonitor API */
            retVal = CSL_ringaccReadRingMonitor(&drvHandle->raRegs, Udma_ringMonGetNum(monHandle),
                                                &data0, &data1);
            if(0 == retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                 GT_1trace(taskObj->traceMask, GT_INFO1,
                           " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccReadRingMonitor ::\r\n",
                           taskObj->taskId);
                 retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 2: Check maxMonitors is zero */
            backUpMaxmonitors             = drvHandle->raRegs.maxMonitors;
            drvHandle->raRegs.maxMonitors = 0U;
            retVal = CSL_ringaccReadRingMonitor(&drvHandle->raRegs, Udma_ringMonGetNum(monHandle),
                                                &data0, &data1);
            if(0 != retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                 GT_1trace(taskObj->traceMask, GT_INFO1,
                           " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccReadRingMonitor::"
                           "  \r\n",
                           taskObj->taskId);
                 retVal = UDMA_EFAIL;
            }
            drvHandle->raRegs.maxMonitors = backUpMaxmonitors;
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 3: Check CSL_ringaccReadRingMonitor for invalid monNum */
            retVal = CSL_ringaccReadRingMonitor(&drvHandle->raRegs, UDMA_RING_ANY,
                                                &data0, &data1);
            if(0 != retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                 GT_1trace(taskObj->traceMask, GT_INFO1,
                           " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccReadRingMonitor \r\n",
                           taskObj->taskId);
                 retVal = UDMA_EFAIL;
            }
        }

        if(UDMA_SOK == retVal)
        {
            /* Test scenario 4: Check CSL_ringaccReadRingMonitor when pData0 and pData1 are NULL */
            retVal = CSL_ringaccReadRingMonitor(&drvHandle->raRegs, Udma_ringMonGetNum(monHandle),
                                                NULL, NULL);
            if(0 == retVal)
            {
                retVal = UDMA_SOK;
            }
            else
            {
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Task:%d: FAIL :: Test CSL_ringaccReadRingMonitor \r\n",
                          taskObj->taskId);
                retVal = UDMA_EFAIL;
            }
        }
    }
    Udma_ringFree(ringHandle);
    Udma_ringMonFree(monHandle);

    return retVal;
}
