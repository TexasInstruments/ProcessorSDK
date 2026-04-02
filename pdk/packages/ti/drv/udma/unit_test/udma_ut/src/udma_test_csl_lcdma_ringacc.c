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
 *  (INCLUDING PosLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *  \file udma_test_csl_ring_lcdma.c
 *
 *  \brief File containing test cases for UDMA Ring LCDMA related APIs.
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

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t UdmaTestInitLcdma(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle, uint32_t elemCnt);
static int32_t UdmaTestDeInitLcdma(Udma_ChHandle chHandle);
static void memOpsFxn(void *addr, uint32_t size, uint32_t type);

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
 * Test Case Description: Verifies the csl ring lcdma functions with below test scenario's
 * 1)Test scenario 1: Validate CSL_lcdma_ringaccInitCfg API.
 * 2)Test scenario 2: Validate CSL_lcdma_ringaccInitRingCfg API.
 * 3)Test scenario 3: Validate CSL_lcdma_ringaccGetRingNum API.
 * 4)Test scenario 4: Validate CSL_lcdma_ringaccGetForwardRingPtr API when ring is empty.
 * 5)Test scenario 5: Validate CSL_lcdma_ringaccGetForwardRingPtr API when ring is full.
 * 6)Test scenario 6: Validate CSL_lcdma_ringaccGetReverseRingPtr API when ring is empty.
 * 7)Test scenario 7: Validate CSL_lcdma_ringaccGetReverseRingPtr API when ring is full.
 * 8)Test scenario 8: Validate CSL_lcdma_ringaccPush64Multi API
 * 9)Test scenario 9: Test CSL_lcdma_ringaccPush64Multi when requested access size is less than element size
 * 10)Test scenario 10: Test CSL_lcdma_ringaccPush64Multi when element count is 0
 * 11)Test scenario 11: Test CSL_lcdma_ringaccPush64Multi to get error ring full
 * 12)Test scenario 12: Validate CSL_lcdma_ringaccPeek64 API
 * 13)Test scenario 13: Test CSL_lcdma_ringaccPeek64 when requested access size is less than element size
 * 14)Test scenario 14: Test CSL_lcdma_ringaccPeek64 when asel is not 0 and memOpsFxn is not NULL
 * 15 Test scenario 15: Test CSL_lcdma_ringaccPeek64 when asel is 0 and memOpsFxn is not NULL
 * 16) Test scenario 16: Validate CSL_lcdma_ringaccPop64Multi API when asel is not 0
 * 17)Test scenario 17: Validate CSL_lcdma_ringaccPop64Multi API
 * 18)Test scenario 18: Test CSL_lcdma_ringaccPop64Multi when requested access size is less than element size
 * 19)Test scenario 19: Validate CSL_lcdma_ringaccWrData API
 * 20)Test scenario 20: Test CSL_lcdma_ringaccRdData when asel is 0 and memOpsFxn is not NULL
 * 21)Test scenario 21: Test CSL_lcdma_ringaccRdData when asel is 0 and memOpsFxn is not NULL
 * 22)Test scenario 22: Test CSL_lcdma_ringaccRdData when asel is 0 and memOpsFxn is not NULL
 * 23)Test scenario 23: Validate CSL_lcdma_ringaccRdData API
 * 24)Test scenario 24: Test CSL_lcdma_ringaccRdData when asel is 0 and memOpsFxn is not NULL
 * 25)Test scenario 25: Test CSL_lcdma_ringaccRdData when asel is not 0 and memOpsFxn is not NULL
 * 26)Test scenario 26: Validate CSL_lcdma_ringaccPeekData API
 * 27)Test scenario 27: Validate CSL_lcdma_ringaccPush64Multi API to check numValues not equal numValuesWritten
 * 28)Test scenario 28: Validate CSL_lcdma_ringaccAckTeardown API
 * 29)Test scenario 29: Validate CSL_lcdma_ringaccIsTeardownComplete API when channel is not disabled
 * 30)Test scenario 30: Validate CSL_lcdma_ringaccIsTeardownComplete API when channel is disabled
 */
int32_t UdmaTestCslRingLcdma(UdmaTestTaskObj *taskObj)
{
    int32_t           retVal    = UDMA_SOK;
    Udma_DrvHandle    drvHandle = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_BCDMA_0];
    uint32_t          elemCnt   = 10U;
    uint32_t          qCnt;
    struct Udma_ChObj chObj;
    Udma_ChHandle     chHandle  = &chObj;
    uint64_t          ringData[elemCnt];
    uint32_t          backupElsz;
    uint32_t          backupAsel;

    /* Initialize CSL LCDMA test */
    retVal = UdmaTestInitLcdma(drvHandle, chHandle, elemCnt);

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 1: Validate CSL_lcdma_ringaccInitCfg API. */
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccInitCfg ::\r\n",
                taskObj->taskId);
        CSL_LcdmaRingaccCfg ringAccCfg;
        CSL_lcdma_ringaccInitCfg(&ringAccCfg);

        /* Test scenario 2: Validate CSL_lcdma_ringaccInitRingCfg API. */
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccInitRingCfg ::\r\n",
                taskObj->taskId);
        CSL_LcdmaRingaccRingCfg ringCfg;
        CSL_lcdma_ringaccInitRingCfg(&ringCfg);

        /* Test scenario 3: Validate CSL_lcdma_ringaccGetRingNum API. */
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccGetRingNum ::\r\n",
                  taskObj->taskId);
        uint32_t ringNum = CSL_lcdma_ringaccGetRingNum(&ringCfg);
        if(ringNum != 0U)
        {
            retVal = UDMA_EFAIL;
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccGetRingNum ::\r\n",
                      taskObj->taskId);
        }
    }
    else
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: FAIL :: Initialize CSL LCDMA test ::\r\n",
                  taskObj->taskId);
    }

    /* Test scenario 4: Validate CSL_lcdma_ringaccGetForwardRingPtr API when ring is empty. */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccGetForwardRingPtr when ring is empty ::\r\n",
                  taskObj->taskId);
        void *pFwdRing = CSL_lcdma_ringaccGetForwardRingPtr(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg);
        if(pFwdRing == NULL)
        {
            retVal = UDMA_EFAIL;
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccGetForwardRingPtr when ring is empty ::\r\n",
                    taskObj->taskId);
        }
    }

    /* Test scenario 5: Validate CSL_lcdma_ringaccGetForwardRingPtr API when ring is full. */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccGetForwardRingPtr when ring is full ::\r\n",
                  taskObj->taskId);
        for(qCnt = 0U; qCnt < elemCnt; qCnt++)
        {
            ringData[qCnt]  = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
            retVal += Udma_ringQueueRawLcdma(drvHandle, chHandle->fqRing, (uint64_t)&ringData[qCnt]);
        }
        if(UDMA_SOK == retVal)
        {
            void *pFwdRing = CSL_lcdma_ringaccGetForwardRingPtr(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg);
            if(pFwdRing != NULL)
            {
                retVal = UDMA_EFAIL;
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccGetForwardRingPtr when ring is full ::\r\n",
                          taskObj->taskId);
            }
            qCnt = 0U;
            int32_t tempRet = UDMA_SOK;
            while(UDMA_ETIMEOUT != tempRet)
            {
                tempRet = Udma_ringFlushRawLcdma(drvHandle, chHandle->fqRing, &ringData[qCnt++]);
            }
            tempRet = UDMA_SOK;
        }
        else
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccGetForwardRingPtr ringQueue failed ::\r\n",
                      taskObj->taskId);
        }
    }

    /* Test scenario 6: Validate CSL_lcdma_ringaccGetReverseRingPtr API when ring is empty. */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccGetReverseRingPtr when ring is empty ::\r\n",
                  taskObj->taskId);
        void *pRevRing = CSL_lcdma_ringaccGetReverseRingPtr(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg);
        if(pRevRing != NULL)
        {
            retVal = UDMA_EFAIL;
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccGetReverseRingPtr when ring is empty ::\r\n",
                      taskObj->taskId);
        }
    }

    /* Test scenario 7: Validate CSL_lcdma_ringaccGetReverseRingPtr API when ring is full. */
    if(UDMA_SOK == retVal)
    {
        Udma_chEnable(chHandle);
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccGetReverseRingPtr when ring is full ::\r\n",
                taskObj->taskId);
        for(qCnt = 0U; qCnt < elemCnt; qCnt++)
        {
            ringData[qCnt]  = ((uint64_t) qCnt | (uint64_t) 0xDEADBEEF00000000UL);
            Udma_ringPrimeLcdma(chHandle->fqRing, ringData[qCnt]);
        }

        /* Do Cache flush and commit to ring */
        Udma_appUtilsCacheWb(chHandle->chPrms.fqRingPrms.ringMem, chHandle->chPrms.fqRingPrms.ringMemSize);
        Udma_ringSetDoorBellLcdma(chHandle->fqRing, elemCnt);

        Osal_delay(100);

        if(UDMA_SOK == retVal)
        {
            void *pRevRing = CSL_lcdma_ringaccGetReverseRingPtr(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg);
            if(pRevRing == NULL)
            {
                retVal = UDMA_EFAIL;
                GT_1trace(taskObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccGetReverseRingPtr when ring is full ::\r\n",
                          taskObj->taskId);
            }
            qCnt = 0U;
            int32_t tempRet = UDMA_SOK;
            while(UDMA_ETIMEOUT != tempRet)
            {
                tempRet = Udma_ringFlushRawLcdma(drvHandle, chHandle->fqRing, &ringData[qCnt++]);
            }
            tempRet = UDMA_SOK;
        }
        else
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccGetReverseRingPtr ringQueue failed ::\r\n",
                      taskObj->taskId);
        }
        Udma_chDisable(chHandle, 0);
    }
    
    UdmaTestDeInitLcdma(chHandle);
    UdmaTestInitLcdma(drvHandle, chHandle, elemCnt);

    /* Test scenario 8: Validate CSL_lcdma_ringaccPush64Multi API */
    Udma_chEnable(chHandle);
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccPush64Multi ::\r\n",
                  taskObj->taskId);
        retVal = CSL_lcdma_ringaccPush64Multi(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, elemCnt, NULL);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPush64Multi ::\r\n",
                    taskObj->taskId);
        }
    }
    
    /* Test scenario 9: Test CSL_lcdma_ringaccPush64Multi when requested access size is less than element size */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccPush64Multi ::\r\n",
                  taskObj->taskId);
        backupElsz = chHandle->fqRing->lcdmaCfg.elSz;
        chHandle->fqRing->lcdmaCfg.elSz = 1U;
        retVal = CSL_lcdma_ringaccPush64Multi(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, elemCnt, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPush64Multi ::"
                      " when requested access size is less than element size \r\n",
                      taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        chHandle->fqRing->lcdmaCfg.elSz = backupElsz;
    }

    /* Test scenario 10: Test CSL_lcdma_ringaccPush64Multi when element count is 0 */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccPush64Multi ::\r\n",
                  taskObj->taskId);
        retVal = CSL_lcdma_ringaccPush64Multi(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, 0, NULL);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPush64Multi ::"
                      " Test CSL_lcdma_ringaccPush64Multi when element count is 0 \r\n",
                      taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 11: Test CSL_lcdma_ringaccPush64Multi to get error ring full */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccPush64Multi ::\r\n",
                  taskObj->taskId);
        retVal = CSL_lcdma_ringaccPush64Multi(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, elemCnt, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPush64Multi :: "
                    " Test CSL_lcdma_ringaccPush64Multi to get error ring full \r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 12: Validate CSL_lcdma_ringaccPeek64 API */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccPeek64 ::\r\n",
                  taskObj->taskId);
        retVal = CSL_lcdma_ringaccPeek64(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, NULL);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPeek64 ::\r\n",
                    taskObj->taskId);
        }
    }
    
    /* Test scenario 13: Test CSL_lcdma_ringaccPeek64 when requested access size is less than element size */
    if(UDMA_SOK == retVal)
    {
	chHandle->fqRing->lcdmaCfg.elSz = 1U;
	retVal = CSL_lcdma_ringaccPeek64(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPeek64 :: "
                    " Test CSL_lcdma_ringaccPeek64 when requested access size is less than "
                    " element size\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        chHandle->fqRing->lcdmaCfg.elSz = backupElsz;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 14: Test CSL_lcdma_ringaccPeek64 when asel is not 0 and memOpsFxn is not NULL */
        backupAsel = chHandle->fqRing->lcdmaCfg.asel;
        chHandle->fqRing->lcdmaCfg.asel = 1;
        retVal = CSL_lcdma_ringaccPeek64(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, &memOpsFxn);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPeek64 ::"
                    " Test CSL_lcdma_ringaccPeek64 when asel is not 0 and memOpsFxn is "
                    " not NULL\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        chHandle->fqRing->lcdmaCfg.asel = backupAsel;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 15: Test CSL_lcdma_ringaccPeek64 when asel is 0 and memOpsFxn is not NULL */
        backupAsel = chHandle->fqRing->lcdmaCfg.asel;
        chHandle->fqRing->lcdmaCfg.asel = 0;
        retVal = CSL_lcdma_ringaccPeek64(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, &memOpsFxn);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPeek64 :: "
                    " Test CSL_lcdma_ringaccPeek64 when asel is not 0 and memOpsFxn is not"
                    "  NULL\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        chHandle->fqRing->lcdmaCfg.asel = backupAsel;
    }

    /* Test scenario 16: Validate CSL_lcdma_ringaccPop64Multi API when asel is not 0 */
    if(UDMA_SOK == retVal)
    {
        backupAsel = chHandle->fqRing->lcdmaCfg.asel;
        chHandle->fqRing->lcdmaCfg.asel = 1;
	GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccPop64Multi  ::\r\n",
                  taskObj->taskId);
        CSL_lcdma_ringaccPop64Multi(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, 1U, &memOpsFxn);
        chHandle->fqRing->lcdmaCfg.elSz = backupElsz;
    }

    /* Test scenario 17: Validate CSL_lcdma_ringaccPop64Multi API */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccPop64Multi when numvalues is 0 ::\r\n",
                  taskObj->taskId);
        retVal = CSL_lcdma_ringaccPop64Multi(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, 0U, NULL);
        if(UDMA_SOK > retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPop64Multi when numvalues is 0 ::\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 18: Test CSL_lcdma_ringaccPop64Multi when requested access size is less than element size */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccPop64Multi ::\r\n",
                  taskObj->taskId);
        backupElsz = chHandle->fqRing->lcdmaCfg.elSz;
        chHandle->fqRing->lcdmaCfg.elSz = 1U;
        retVal = CSL_lcdma_ringaccPop64Multi(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, elemCnt, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPop64Multi ::\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        chHandle->fqRing->lcdmaCfg.elSz = backupElsz;
    }

    Udma_chDisable(chHandle, 0);

    /* Test scenario 19: Validate CSL_lcdma_ringaccWrData API */
    Udma_chEnable(chHandle);
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccWrData ::\r\n",
                  taskObj->taskId);
        retVal = CSL_lcdma_ringaccWrData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, (uint8_t *)ringData, sizeof(uint64_t), NULL);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccWrData::\r\n",
                    taskObj->taskId);
        }
    }

    /* Test scenario 20: Test CSL_lcdma_ringaccRdData when asel is 0 and memOpsFxn is NULL */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccWrData ::\r\n",
                  taskObj->taskId);
        backupAsel = chHandle->fqRing->lcdmaCfg.asel;
        chHandle->fqRing->lcdmaCfg.asel = 0;
        retVal = CSL_lcdma_ringaccWrData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, (uint8_t *)ringData, sizeof(uint64_t), NULL);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccRdData when asel is 0 "
                    " and memOpsFxn is not NULL::\r\n",
                    taskObj->taskId);
        }
        chHandle->fqRing->lcdmaCfg.asel = backupAsel;
    }

    /* Test scenario 21: Test CSL_lcdma_ringaccRdData when asel is 0 and memOpsFxn is not NULL */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccWrData ::\r\n",
                  taskObj->taskId);
        backupAsel = chHandle->fqRing->lcdmaCfg.asel;
        chHandle->fqRing->lcdmaCfg.asel = 0;
        retVal = CSL_lcdma_ringaccWrData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, (uint8_t *)ringData, sizeof(uint64_t), &memOpsFxn);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccRdData when asel is 0 "
                    " and memOpsFxn is not NULL::\r\n",
                    taskObj->taskId);
        }
        chHandle->fqRing->lcdmaCfg.asel = backupAsel;
    }

    /* Test scenario 22: Test CSL_lcdma_ringaccRdData when asel is 0 and memOpsFxn is not NULL */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccWrData ::\r\n",
                  taskObj->taskId);
        backupAsel = chHandle->fqRing->lcdmaCfg.asel;
        chHandle->fqRing->lcdmaCfg.asel = 1U;
        retVal = CSL_lcdma_ringaccWrData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, (uint8_t *)ringData, sizeof(uint64_t), &memOpsFxn);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccRdData when asel is "
                    " 0 and memOpsFxn is not NULL::\r\n",
                    taskObj->taskId);
        }
        chHandle->fqRing->lcdmaCfg.asel = backupAsel;
    }

    /* Test scenario 23: Validate CSL_lcdma_ringaccRdData API */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccRdData ::\r\n",
                  taskObj->taskId);
        retVal = CSL_lcdma_ringaccRdData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, (uint8_t *)ringData, sizeof(uint64_t), NULL);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccRdData ::\r\n",
                    taskObj->taskId);
        }
    }

    /* Test scenario 24: Test CSL_lcdma_ringaccRdData when asel is 0 and memOpsFxn is not NULL */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccRdData ::\r\n",
                  taskObj->taskId);
        backupAsel = chHandle->fqRing->lcdmaCfg.asel;
        chHandle->fqRing->lcdmaCfg.asel = 0;
        retVal = CSL_lcdma_ringaccRdData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, (uint8_t *)ringData, sizeof(uint64_t), &memOpsFxn);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccRdData when asel is"
                    " 0 and memOpsFxn is not NULL ::\r\n",
                    taskObj->taskId);
        }
        chHandle->fqRing->lcdmaCfg.asel = backupAsel;
    }

    /* Test scenario 25: Test CSL_lcdma_ringaccRdData when asel is not 0 and memOpsFxn is not NULL */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccRdData ::\r\n",
                  taskObj->taskId);
        backupAsel = chHandle->fqRing->lcdmaCfg.asel;
        chHandle->fqRing->lcdmaCfg.asel = 1U;
        retVal = CSL_lcdma_ringaccRdData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, (uint8_t *)ringData, sizeof(uint64_t), &memOpsFxn);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccRdData when "
                    " asel is not 0 and memOpsFxn is not NUL ::\r\n",
                    taskObj->taskId);
        }
        chHandle->fqRing->lcdmaCfg.asel = backupAsel;
    }

    /* Test scenario 26: Validate CSL_lcdma_ringaccPeekData API */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccPeekData ::\r\n",
                  taskObj->taskId);
        retVal = CSL_lcdma_ringaccPeekData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, (uint8_t *)ringData, sizeof(uint64_t), NULL);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPeekData ::\r\n",
                    taskObj->taskId);
        }
    }

    /* Test scenario 27: Validate CSL_lcdma_ringaccPush64Multi API to check numValues not equal numValuesWritten */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccPush64Multi ::\r\n",
                  taskObj->taskId);
        backupAsel = chHandle->fqRing->lcdmaCfg.asel;
        chHandle->fqRing->lcdmaCfg.asel = 1U;
        retVal = CSL_lcdma_ringaccPush64Multi(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, elemCnt, &memOpsFxn);
        if(retVal >= UDMA_SOK)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccPush64Multi ::\r\n",
                      taskObj->taskId);
            retVal = UDMA_EFAIL;
        }
        chHandle->fqRing->lcdmaCfg.asel = backupAsel;
    }

    /* Test scenario 28: Validate CSL_lcdma_ringaccAckTeardown API */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccAckTeardown ::\r\n",
                  taskObj->taskId);
        CSL_lcdma_ringaccAckTeardown(&drvHandle->lcdmaRaRegs, chHandle->fqRing->lcdmaCfg.ringNum);
    }

    /* Test scenario 29: Validate CSL_lcdma_ringaccIsTeardownComplete API when channel is not disabled */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccIsTeardownComplete ::\r\n",
                  taskObj->taskId);
        CSL_lcdma_ringaccIsTeardownComplete(&drvHandle->lcdmaRaRegs, chHandle->fqRing->lcdmaCfg.ringNum);
    }
    
    Udma_chDisable(chHandle, 0);

    /* Test scenario 30: Validate CSL_lcdma_ringaccIsTeardownComplete API when channel is disabled */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_lcdma_ringaccIsTeardownComplete ::\r\n",
                  taskObj->taskId);
        CSL_lcdma_ringaccIsTeardownComplete(&drvHandle->lcdmaRaRegs, chHandle->fqRing->lcdmaCfg.ringNum);
    }

    UdmaTestDeInitLcdma(chHandle);

    return retVal;
}

static int32_t UdmaTestInitLcdma(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle, uint32_t elemCnt)
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
        chPrms.peerChNum              = UDMA_PSIL_CH_MAIN_CSI_TX;
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

static int32_t UdmaTestDeInitLcdma(Udma_ChHandle chHandle)
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
 * Test Case Description: Verifies the csl ring lcdma functions with below test scenario's
 * 1)Test scenario 1:Test CSL_lcdma_ringaccPeekData when Number of bytes to peek is greater
 *                   than ring element size
 * 2)Test scenario 2:Test CSL_lcdma_ringaccPeekData Check when asel is 0 and memOpsFxn
 *                   is not NULL
 * 3)Test scenario 3:Test CSL_lcdma_ringaccPeekData when  memOpsFxn is NULL
 * 4)Test scenario 4:Test CSL_lcdma_ringaccPeekData when asel is not 0
 * 5)Test scenario 6:Test CSL_lcdma_ringaccPeekData when pRingEntry is NULL
 * 6)Test scenario 6:Test CSL_lcdma_ringaccRdData when Number of bytes to peek is greater than
 *                   ring element size
 * 7)Test scenario 7:Test CSL_lcdma_ringaccRdData when pRingEntry is NULL
 * 8)Test scenario 8:CSL_lcdma_ringaccWrData when Number of bytes to peek
 *                   is greater than ring element size
 * 9)Test scenario 9:Test CSL_lcdma_ringaccWrData when pRingEntry is NULL
 * 10)Test scenario 10: Test get return from CSL_lcdma_ringaccIsRingEmpty as false
 * 11)Test scenario 11: Test CSL_lcdma_ringaccPeek64
 * 12)Test scenario 12: Test CSL_lcdma_ringaccPeek64 when rdOcc is OU
 */
int32_t UdmaTestCSL_lcdma_ringaccPeekData(UdmaTestTaskObj *taskObj)
{

    int32_t           retVal    = UDMA_SOK;
    Udma_DrvHandle    drvHandle = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_BCDMA_0];
    uint32_t          elemCnt   = 10U;
    struct Udma_ChObj chObj;
    Udma_ChHandle     chHandle  = &chObj;
    uint64_t          ringData[elemCnt];
    uint32_t          backupAsel;

    retVal = UdmaTestInitLcdma(drvHandle, chHandle, elemCnt);

    CSL_lcdma_ringaccGetRevision(&drvHandle->lcdmaRaRegs);

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 1: Test CSL_lcdma_ringaccPeekData when Number of bytes to peek is greater
         *                  than ring element size
         */
        retVal = CSL_lcdma_ringaccPeekData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg,
        	                          (uint8_t *)ringData, UDMA_RING_ANY , NULL);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPeekData Number of "
                    " bytes to peek is greater than ring element size::\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    if(UDMA_SOK == retVal)
    {
       /* Test scenario 2: Test CSL_lcdma_ringaccPeekData Check when asel is 0 and memOpsFxn
        *                  is not NULL
        */
        backupAsel = chHandle->fqRing->lcdmaCfg.asel;
        chHandle->fqRing->lcdmaCfg.asel = 0U;
        retVal = CSL_lcdma_ringaccPeekData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg,
                                    (uint8_t *)ringData, sizeof(uint64_t), &memOpsFxn);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPeekData when Check "
                    " when asel is 0 and memOpsFxn is not NULL::\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        chHandle->fqRing->lcdmaCfg.asel = backupAsel;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Test CSL_lcdma_ringaccPeekData when  memOpsFxn is NULL */
        backupAsel = chHandle->fqRing->lcdmaCfg.asel;
        chHandle->fqRing->lcdmaCfg.asel = 1U;
        retVal = CSL_lcdma_ringaccPeekData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg,
                                    (uint8_t *)ringData, sizeof(uint64_t), NULL);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPeekData when  "
                    " memOpsFxn is NULL ::\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        chHandle->fqRing->lcdmaCfg.asel = backupAsel;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Test CSL_lcdma_ringaccPeekData when asel is not 0 */
        backupAsel = chHandle->fqRing->lcdmaCfg.asel;
        chHandle->fqRing->lcdmaCfg.asel = 1U;
        retVal = CSL_lcdma_ringaccPeekData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg,
                                           (uint8_t *)ringData, sizeof(uint64_t), &memOpsFxn);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPeekData when  asel is not 0 ::\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        chHandle->fqRing->lcdmaCfg.asel = backupAsel;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Test CSL_lcdma_ringaccPeekData when pRingEntry is NULL */
        chHandle->fqRing->lcdmaCfg.rdIdx    = 0U;
        chHandle->fqRing->lcdmaCfg.virtBase = 0U;
        retVal = CSL_lcdma_ringaccPeekData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg,
                                           (uint8_t *)ringData, sizeof(uint64_t), NULL);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPeekData when pRingEntry is NULL::\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Test CSL_lcdma_ringaccRdData when Number of bytes to peek is greater than
         *                  ring element size
         */
        retVal = CSL_lcdma_ringaccRdData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg,
                                         (uint8_t *)ringData, UDMA_RING_ANY , NULL);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccRdData when Number of bytes "
                    " to peek is greater than ring element size::\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 7: Test CSL_lcdma_ringaccRdData when pRingEntry is NULL */
        retVal = CSL_lcdma_ringaccRdData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg,
                                         (uint8_t *)ringData, sizeof(uint64_t), &memOpsFxn);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccRdData when pRingEntry is NULL::\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 8: Test CSL_lcdma_ringaccWrData when Number of bytes to peek
         *                  is greater than ring element size
         */
        retVal = CSL_lcdma_ringaccWrData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg,
                                         (uint8_t *)ringData, UDMA_RING_ANY , NULL);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccWrData when Number of bytes to "
                    " peek is greater than ring element size::\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 9: Test CSL_lcdma_ringaccWrData when pRingEntry is NULL */
        retVal = CSL_lcdma_ringaccWrData(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg,
                                         (uint8_t *)ringData, sizeof(uint64_t), &memOpsFxn);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccWrData when pRingEntry is NULL ::\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }


    if(UDMA_SOK == retVal)
    {
        /* Test scenario 10: Test get return from CSL_lcdma_ringaccIsRingEmpty as false*/
        void* ptr;
        chHandle->fqRing->lcdmaCfg.rdOcc = 1U;
        ptr = CSL_lcdma_ringaccGetReverseRingPtr(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg);
        if(NULL != ptr)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccGetReverseRingPtr ::\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 11: Test CSL_lcdma_ringaccPeek64 */
        retVal = CSL_lcdma_ringaccPeek64(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, &memOpsFxn);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPeek64 ::"
                    " Test CSL_lcdma_ringaccPeek64\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 12: Test CSL_lcdma_ringaccPeek64 when rdOcc is OU */
        chHandle->fqRing->lcdmaCfg.elSz = sizeof(uint64_t);
        chHandle->fqRing->lcdmaCfg.rdOcc = 0U;
        retVal = CSL_lcdma_ringaccPeek64(&drvHandle->lcdmaRaRegs, &chHandle->fqRing->lcdmaCfg, ringData, &memOpsFxn);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL :: Test CSL_lcdma_ringaccPeek64 ::"
                    " Test CSL_lcdma_ringaccPeek64\r\n",
                    taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    UdmaTestDeInitLcdma(chHandle);

    return retVal;
}

