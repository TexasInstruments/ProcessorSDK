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
 *  \file udma_test_utc.c
 *
 *  \brief File containing test cases for UDMA UTC related APIs.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <udma_test.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*
 * Ring parameters
 */
/** \brief Number of ring entries - we can prime this much memcpy operations */
#define UDMA_TEST_APP_RING_ENTRIES      (1U)
/** \brief Size (in bytes) of each ring entry (Size of pointer - 64-bit) */
#define UDMA_TEST_APP_RING_ENTRY_SIZE   (sizeof(uint64_t))
/** \brief Total ring memory */
#define UDMA_TEST_APP_RING_MEM_SIZE     (UDMA_TEST_APP_RING_ENTRIES * \
                                         UDMA_TEST_APP_RING_ENTRY_SIZE)
/** \brief This ensures every channel memory is aligned */
#define UDMA_TEST_APP_RING_MEM_SIZE_ALIGN ((UDMA_TEST_APP_RING_MEM_SIZE + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))

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
/* UDMA memories */
static uint8_t gUdmaTestDruRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gUdmaTestDruCompRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gUdmaTestDruTdCompRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * Test Case Description: Verifies the function Udma_chPause
 * Test scenario 1: Check to fail DRU channel pause
 * Test scenario 2: Check to fail channel enable and destination thread disable
 */
int32_t UdmaTestChPauseDruNeg(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    struct Udma_ChObj  chObj;
    Udma_ChHandle      chHandle;
    struct Udma_DrvObj backUpDrvObj;
    Udma_DrvHandle     drvHandle;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    uint32_t           timeout;
    struct Udma_ChObj  backUpChObj;
  
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChPauseDru negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check to fail DRU channel pause */
    timeout          = 0U;
    chHandle         = &chObj;
    chType           = UDMA_CH_TYPE_UTC;
    backUpDrvObj     = taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];           
    UdmaChPrms_init(&chPrms, chType);
    chPrms.utcId     = UDMA_UTC_ID_MSMC_DRU0;
    drvHandle        = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    Udma_ChUtcPrms utcPrms;
    UdmaChUtcPrms_init(&utcPrms);
    utcPrms.druOwner = CSL_DRU_OWNER_DIRECT_TR;
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigUtc(chHandle, &utcPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(chHandle);
            if(UDMA_SOK == retVal)
            {
                backUpChObj        = chObj;
                chHandle->extChNum = CSL_DRU_NUM_CH;
                retVal             = Udma_chPause(chHandle);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: chPause:: Neg:: "
                              " Check to fail DRU channel pause!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = Udma_chResume(chHandle);
                    if(UDMA_SOK == retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: ChResume:: Neg::"
                                  " Check to fail DRU channel pause!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                        chObj  = backUpChObj;
                        retVal = Udma_chPause(chHandle);
                        if(UDMA_SOK == retVal)
                        {
                            retVal = Udma_chResume(chHandle);
                            if(UDMA_SOK != retVal)
                            {
                                GT_0trace(taskObj->traceMask, GT_ERR,
                                        " |TEST INFO|:: FAIL:: UDMA:: ChResume:: Pos::"
                                        " Check when druOwner is CSL_DRU_OWNER_DIRECT_TR!!\n");
                                retVal = UDMA_EFAIL;
                            }
                        }
                        Udma_chDisable(chHandle, timeout);
                        Udma_chClose(chHandle); 
                    }
                }
            }
        }
    }
    taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0] = backUpDrvObj;

    /* Test scenario 2: Check to fail channel enable and destination thread disable*/
    if(UDMA_SOK == retVal)
    {
        UdmaChPrms_init(&chPrms, chType);
        chPrms.utcId     = UDMA_UTC_ID_MSMC_DRU0;
        drvHandle        = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        Udma_ChUtcPrms utcPrms;
        UdmaChUtcPrms_init(&utcPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigUtc(chHandle, &utcPrms);
            if(UDMA_SOK == retVal)
            {
                backUpChObj                = chObj;
                chHandle->utcPrms.druOwner = CSL_DRU_OWNER_DIRECT_TR;
                chHandle->extChNum         = CSL_DRU_NUM_CH;
                retVal = Udma_chEnable(chHandle);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: chEnable:: Neg::"
                              " Check to fail channel enable!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal                     = UDMA_SOK;
                    chObj                      = backUpChObj;
                    chHandle->utcPrms.druOwner = CSL_DRU_OWNER_UDMAC_TR;
                    chHandle->peerThreadId     = UDMA_THREAD_ID_INVALID;
                    retVal                     = Udma_chEnable(chHandle); 
                    retVal                    += Udma_chDisable(chHandle, timeout); 
                    if(UDMA_SOK == retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: chEnable:: Neg::"
                                  " Check when peerThreadId is Invalid!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                        chObj  = backUpChObj;
                        Udma_chEnable(chHandle); 
                        Udma_chDisable(chHandle, timeout);
                        chHandle->peerThreadId = UDMA_THREAD_ID_INVALID;
                        retVal = Udma_chClose(chHandle);
                        if(UDMA_SOK == retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: chUnpair:: Neg::"
                                      " Check when peerThreadId is Invalid!!\n");
                            retVal = UDMA_EFAIL;
                        }
                        else
                        {
                            retVal = UDMA_SOK;
                            chObj  = backUpChObj;
                            Udma_chClose(chHandle);
                        }
                    } 
                }
            }
        }
        taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0] = backUpDrvObj;
    }

    return retVal;
}

/* 
* Test Case Description: Verifies the function Udma_chDisable 
* Test scenario 1: Check when chType is UDMA_CH_TYPE_UTC and 
                   druOwner is CSL_DRU_OWNER_DIRECT_TR
*/
int32_t UdmaTestChDisableDruNeg(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    struct Udma_ChObj  chObj;
    struct Udma_DrvObj backUpDrvObj; 
    Udma_ChHandle      chHandle;
    Udma_ChPrms        chPrms;
    Udma_DrvHandle     drvHandle;
    uint32_t           timeout;
    uint32_t           instID;
    uint32_t           chType;
    uint32_t           bkupExtCh;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChDisableDru negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when chType is UDMA_CH_TYPE_UTC and 
    *                   druOwner is CSL_DRU_OWNER_DIRECT_TR 
    */
    timeout      = 0U;
    chHandle     = &chObj;
    instID       = UDMA_TEST_INST_ID_MAIN_0;
    chType       = UDMA_CH_TYPE_UTC;
    UdmaChPrms_init(&chPrms, chType);
    backUpDrvObj = taskObj->testObj->drvObj[instID];
    chPrms.utcId = UDMA_UTC_ID_MSMC_DRU0;
    drvHandle    = &taskObj->testObj->drvObj[instID];
    retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    Udma_ChUtcPrms utcPrms;
    UdmaChUtcPrms_init(&utcPrms);
    utcPrms.druOwner = CSL_DRU_OWNER_DIRECT_TR;
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigUtc(chHandle, &utcPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(chHandle);
            if(UDMA_SOK == retVal)
            {
                bkupExtCh          = chHandle->extChNum;
                chHandle->extChNum = CSL_DRU_NUM_CH;
                retVal             = Udma_chDisable(chHandle, timeout);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: chDisable:: Pos:: "
                              " Check when chType is UDMA_CH_TYPE_UTC and "
                              " druOwner is CSL_DRU_OWNER_DIRECT_TR*/!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal             = UDMA_SOK;
                    chHandle->extChNum = bkupExtCh;
                    Udma_chDisable(chHandle, timeout);
                    Udma_chClose(chHandle);
                }
            }
        }
    }
    taskObj->testObj->drvObj[instID] = backUpDrvObj;

    return retVal;
}

/* 
 * Test Case Description: Verifies the function Udma_chConfigUtc 
 * Test scenario 1: Check when druOwner is CSL_DRU_OWNER_UDMAC_TR
 * Test scenario 2: Check when druOwner is CSL_DRU_OWNER_UDMAC_TR 
 *                  and druQueueId is CSL_DRU_NUM_QUEUE
 * Test scenario 3: Check when druOwner is CSL_DRU_OWNER_DIRECT_TR 
 *                  and extChNum is CSL_DRU_NUM_CH
 */ 
int32_t UdmaTestChConfigUtcDruTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    struct Udma_ChObj  chObj;
    Udma_ChHandle      chHandle;
    Udma_ChUtcPrms     utcPrms;
    Udma_ChPrms        chPrms;
    Udma_DrvHandle     drvHandle;
    struct Udma_DrvObj backUpDrvObj;
    uint32_t           chType;
    uint32_t           bkupExtCh;
    uint32_t           instID;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChConfigUtcDru negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when druOwner is CSL_DRU_OWNER_UDMAC_TR */
    chHandle        = &chObj;
    backUpDrvObj    = taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
    chType          = UDMA_CH_TYPE_UTC;
    drvHandle       = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
    UdmaChPrms_init(&chPrms, chType);
    chPrms.utcId    = UDMA_UTC_ID_MSMC_DRU0;
    retVal          = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        utcPrms.druOwner = CSL_DRU_OWNER_UDMAC_TR;
        retVal           = Udma_chConfigUtc(chHandle, &utcPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: chConfigUtc:: Neg:: "
                      " Check when druOwner is CSL_DRU_OWNER_UDMAC_TR!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
            Udma_chClose(chHandle);
        }
    }
    taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0] = backUpDrvObj;

    /* Test scenario 2: Check when druOwner is CSL_DRU_OWNER_UDMAC_TR 
    *                   and druQueueId is CSL_DRU_NUM_QUEUE
    */
    if(UDMA_SOK == retVal)
    {
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_UTC;
        UdmaChPrms_init(&chPrms, chType);
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        chPrms.utcId = UDMA_UTC_ID_MSMC_DRU0;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChUtcPrms_init(&utcPrms);
        if(UDMA_SOK == retVal)
        {
            utcPrms.druQueueId = CSL_DRU_NUM_QUEUE;
            retVal = Udma_chConfigUtc(chHandle, &utcPrms);
            if(UDMA_SOK == retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: chConfigUtc:: Neg::"
                          " Check when druOwner is CSL_DRU_OWNER_DIRECT_TR"
                          " and druQueueId is CSL_DRU_NUM_QUEUE!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
                Udma_chClose(chHandle);
            }
        }
    }
    taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0] = backUpDrvObj;

    /* Test scenario 3: Check when druOwner is CSL_DRU_OWNER_DIRECT_TR 
    *                   and extChNum is CSL_DRU_NUM_CH
    */
    if(UDMA_SOK == retVal)
    {
        drvHandle       = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.utcId    = UDMA_UTC_ID_MSMC_DRU0;
        retVal          = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChUtcPrms_init(&utcPrms);
        if(UDMA_SOK == retVal)
        {
            bkupExtCh          = chHandle->extChNum;
            utcPrms.druOwner   = CSL_DRU_OWNER_DIRECT_TR;
            chHandle->extChNum = CSL_DRU_NUM_CH;
            retVal           = Udma_chConfigUtc(chHandle, &utcPrms);
            if(UDMA_SOK == retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: chConfigUtc:: Neg::"
                          " Check when druOwner is CSL_DRU_OWNER_DIRECT_TR"
                          " and extChNum is CSL_DRU_NUM_CH!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal             = UDMA_SOK;
                chHandle->extChNum = bkupExtCh;
                Udma_chClose(chHandle);
            }
        }
    }
    taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0] = backUpDrvObj;

    return retVal;
}

/* 
 * Test Case Description: Verifies the function Udma_chSetSwTrigger
 * Test scenario 1: check when chType is UDMA_CH_TYPE_UTC and 
 *                  trigger is CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL1
 */ 
int32_t UdmaTestChSetSwTriggerDruNeg(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    struct Udma_ChObj  chObj;
    Udma_ChHandle      chHandle;
    Udma_ChPrms        chPrms;
    uint32_t           chType; 
    Udma_DrvHandle     drvHandle;
    struct Udma_DrvObj backUpDrvObj;
    uint32_t           trigger;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChSetSwTriggerDru negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: check when chType is UDMA_CH_TYPE_UTC and 
    *                   trigger is CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL1 
    */
    chHandle     = &chObj;
    backUpDrvObj = taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
    chType       = UDMA_CH_TYPE_UTC;
    UdmaChPrms_init(&chPrms, chType);
    chPrms.utcId = UDMA_UTC_ID_MSMC_DRU0;
    drvHandle    = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
    retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        trigger = CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL1;
        retVal  = Udma_chSetSwTrigger(chHandle, trigger);
        if(retVal == UDMA_EFAIL)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                        " |TEST INFO|:: FAIL:: UDMA:: chSetSwTrigger:: Neg::"
                        " When chType is UDMA_CH_TYPE_UTC and trigger is "
                        " CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL1!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
            Udma_chClose(chHandle);
        } 
    }
    taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0] = backUpDrvObj;

    return retVal;
}

/* Test Case Description: Verifies the function Udma_druGetNumQueue when
 * 1)Test scenario 1: Check when driver handle is NULL.
 * 2)Test scenario 2: Check when drvInitDone is UDMA_DEINIT_DONE.
 * 3)Test scenario 3: Check to get error print message as Invalid UTC ID.
 */
int32_t UdmaTestDruGetNumQueueNeg(UdmaTestTaskObj *taskObj)
{
    int32_t        retVal = UDMA_SOK;
    uint32_t       utcId  = 0U;
    Udma_DrvHandle drvHandle;
    uint32_t       numQueue;
    uint32_t       BackupdrvInitDone;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA DRU get Num negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when driver handle is NULL */
    drvHandle = (Udma_DrvHandle) NULL_PTR;
    numQueue  = Udma_druGetNumQueue(drvHandle, utcId);
    if(0U == numQueue)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: druGetNumQueue :: Neg::"
                  " Check when driver handle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when drvInitDone is UDMA_DEINIT_DONE */
        drvHandle              = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        BackupdrvInitDone      = drvHandle->drvInitDone;
        drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        numQueue               = Udma_druGetNumQueue(drvHandle, utcId);
        if(0U == numQueue)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: druGetNumQueue :: Neg::"
                      " Check when drvInitDone is UDMA_DEINIT_DONE\n");
            retVal = UDMA_EFAIL;
        }
        drvHandle->drvInitDone = BackupdrvInitDone;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check to get error print message as Invalid UTC ID */
        utcId    = UDMA_NUM_UTC_INSTANCE + 1U;
        numQueue = Udma_druGetNumQueue(drvHandle, utcId);
        if(0U == numQueue)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: druGetNumQueue :: Neg::"
                      " Check to get error print message as Invalid UTC ID!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_druQueueConfig when
 * 1)Test scenario 1: Check when driver handle and queueCfg are NULL.
 * 2)Test scenario 2: Check when driver handle is NULL.
 * 3)Test scenario 3: Check when drvInitDone is UDMA_DEINIT_DONE and
 *                    queueCfg is NULL.
 * 4)Test scenario 4: Check when drvInitDone is UDMA_DEINIT_DONE.
 * 5)Test scenario 5: Check when queueCfg is NULL.
 * 6)Test scenario 6: Check to get error print message as Invalid UTC ID.
 * 7)Test scenario 7: Check to get error print message DRU queue config failed.
 */
int32_t UdmaTestDruQueueConfigNeg(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal   = UDMA_SOK;
    uint32_t           utcId    = 0U;
    uint32_t           queId    = 1U;
    uint32_t           DruUtcId = 0U;
    Udma_DrvHandle     drvHandle;
    uint32_t           BackupdrvInitDone;
    CSL_DruQueueConfig DruqueueCfg;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA DRU Queue Config negative Testcase ::\r\n"
              , taskObj->taskId);

    /* Test scenario 1: Check when driver handle and queueCfg are NULL */
    drvHandle = (Udma_DrvHandle) NULL_PTR;
    retVal    = Udma_druQueueConfig(drvHandle, utcId, queId, NULL);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: druQueueConfig :: Neg:: "
                  " Check when driver handle and queueCfg are NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when driver handle is NULL */
        drvHandle = (Udma_DrvHandle) NULL_PTR;
        retVal    = Udma_druQueueConfig(drvHandle, utcId, queId, &DruqueueCfg);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: druQueueConfig :: Neg:: "
                      " Check when driver handle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when drvInitDone is UDMA_DEINIT_DONE and
         *                  queueCfg is NULL
         */
        drvHandle              = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        BackupdrvInitDone      = drvHandle->drvInitDone;
        drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                 = Udma_druQueueConfig(drvHandle, utcId, queId, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: druQueueConfig :: Neg::"
                      " Check for drvInitDone is UDMA_DEINIT_DONE and "
                      " queueCfg is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        drvHandle->drvInitDone = BackupdrvInitDone;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check for drvInitDone is UDMA_DEINIT_DONE */
        drvHandle              = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        BackupdrvInitDone      = drvHandle->drvInitDone;
        drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                 = Udma_druQueueConfig(drvHandle, utcId, queId, &DruqueueCfg);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: druQueueConfig :: Neg::"
                      " Check for drvInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        drvHandle->drvInitDone = BackupdrvInitDone;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when queueCfg is NULL */
        retVal = Udma_druQueueConfig(drvHandle, utcId, queId, NULL);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: druQueueConfig :: Neg::"
                      " Check when queueCfg is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Check to get error print message as Invalid UTC ID */
        UdmaDruQueueConfig_init(&DruqueueCfg);
        utcId  = UDMA_NUM_UTC_INSTANCE + 1U;
        retVal = Udma_druQueueConfig(drvHandle, utcId, queId, &DruqueueCfg);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: druQueueConfig :: Neg::"
                      " Check to get error print message as Invalid UTC ID !!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 7: Check to get error print message DRU queue config failed */
        queId  = CSL_DRU_NUM_QUEUE + 1U;
        retVal = Udma_druQueueConfig(drvHandle, DruUtcId, queId, &DruqueueCfg);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: druQueueConfig :: Neg::"
                      " Check to get error print message DRU queue config failed!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_druGetTriggerRegAddr when
 * 1)Test scenario 1: Check when Channel handle is NULL.
 * 2)Test scenario 2: Check when chInitDone is not UDMA_DEINIT_DONE
 *                    and chType is not UDMA_CH_FLAG_UTC.
 * 3)Test scenario 3: Check when chInitDone is UDMA_DEINIT_DONE.
 * 4)Test scenario 4: Check when chType is not UDMA_CH_FLAG_UTC.
 * 5)Test scenario 5: Check when driver handle is NULL.
 * 6)Test scenario 6: Check when drvInitDone is UDMA_DEINIT_DONE.
 */
int32_t UdmaTestDruGetTriggerRegAddrNeg(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal     = UDMA_SOK;
    volatile uint64_t *pSwTrigReg = (volatile uint64_t *) NULL_PTR;
    struct Udma_ChObj  chObj;
    Udma_ChHandle      channelHandle;
    uint32_t           BackupdrvInitDone;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA DRU Get Trigger Reg Addr negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when Channel handle is NULL */
    channelHandle = (Udma_ChHandle) NULL_PTR;
    pSwTrigReg    = Udma_druGetTriggerRegAddr(channelHandle);
    if(NULL_PTR == pSwTrigReg)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|::FAIL:: UDMA:: druGetTriggerRegAddr :: Neg:: "
                  " Check when Channel handle is NULL!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when chInitDone is not UDMA_DEINIT_DONE
         *                  and chType is not UDMA_CH_FLAG_UTC
         */
        channelHandle             = &chObj;
        channelHandle->chInitDone = UDMA_DEINIT_DONE;
        channelHandle->chType     = UDMA_CH_FLAG_BLK_COPY;
        pSwTrigReg                = Udma_druGetTriggerRegAddr(channelHandle);
        if(NULL_PTR == pSwTrigReg)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: druGetTriggerRegAddr :: Neg:: "
                      " Check when chInitDone is not UDMA_DEINIT_DONE and "
                      " chType is not UDMA_CH_FLAG_UTC!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when chInitDone is UDMA_DEINIT_DONE */
        channelHandle->chInitDone = UDMA_DEINIT_DONE;
        channelHandle->chType     = UDMA_CH_FLAG_UTC;
        pSwTrigReg                = Udma_druGetTriggerRegAddr(channelHandle);
        if(NULL_PTR == pSwTrigReg)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: druGetTriggerRegAddr :: Neg:: "
                      " Check when chInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when chType is not UDMA_CH_FLAG_UTC */
        channelHandle->chInitDone = UDMA_INIT_DONE;
        channelHandle->chType     = UDMA_CH_FLAG_BLK_COPY;
        pSwTrigReg                = Udma_druGetTriggerRegAddr(channelHandle);
        if(NULL_PTR == pSwTrigReg)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: druGetTriggerRegAddr :: Neg:: "
                      " Check for chType when it is not equal to UDMA_CH_FLAG_UTC!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when driver handle is NULL */
        channelHandle->chType    = UDMA_CH_TYPE_UTC;
        channelHandle->drvHandle = NULL_PTR;
        pSwTrigReg               = Udma_druGetTriggerRegAddr(channelHandle);
        if(NULL_PTR == pSwTrigReg)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: druGetTriggerRegAddr :: Neg:: "
                      " Check when driver handle is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Check when drvInitDone is UDMA_DEINIT_DONE */
        channelHandle->drvHandle              = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        BackupdrvInitDone                     = channelHandle->drvHandle->drvInitDone;
        channelHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        pSwTrigReg                            = Udma_druGetTriggerRegAddr(channelHandle);
        if(NULL_PTR == pSwTrigReg)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: druGetTriggerRegAddr :: Neg:: "
                      " Check when drvInitDone is UDMA_DEINIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        channelHandle->drvHandle->drvInitDone = BackupdrvInitDone;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_druGetTriggerRegAddr when
 * 1)Test scenario 1: Check when utcType is UDMA_UTC_TYPE_DRU
 */
int32_t UdmaTestDruGetTriggerRegAddr(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal     = UDMA_SOK;
    volatile uint64_t *pSwTrigReg = (volatile uint64_t *) NULL_PTR;
    struct Udma_ChObj  chObj;
    Udma_ChHandle      channelHandle;
    uint32_t           chType;
    Udma_ChPrms        chPrms;
    Udma_DrvHandle     drvHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA DRU Get Trigger Reg Addr positive Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when utcType is UDMA_UTC_TYPE_DRU */
    channelHandle = &chObj;
    chType        = UDMA_CH_TYPE_UTC;
    UdmaChPrms_init(&chPrms, chType);
    drvHandle     = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
    chPrms.utcId  = UDMA_UTC_ID_MSMC_DRU0;
    retVal        = Udma_chOpen(drvHandle, channelHandle, chType, &chPrms);
    if(UDMA_SOK != retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|::FAIL:: UDMA:: druGetTriggerRegAddr :: "
                  " Channel open failed!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        pSwTrigReg = Udma_druGetTriggerRegAddr(channelHandle);
        if(NULL_PTR != pSwTrigReg)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|::FAIL:: UDMA:: druGetTriggerRegAddr :: "
                      " Check for utcType is UDMA_UTC_TYPE_DRU!!\n");
            retVal = UDMA_EFAIL;
        }
    }
    Udma_chClose(channelHandle);

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_chPause
 * Test scenario 1: Check when druOwner is CSL_DRU_OWNER_UDMAC_TR
 */
int32_t UdmaTestChPauseDru(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    struct Udma_ChObj  chObj;
    Udma_ChHandle      chHandle;
    struct Udma_DrvObj backUpDrvObj;
    Udma_DrvHandle     drvHandle;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    uint32_t           timeout;
    Udma_ChStats       chStats;
  
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChPauseDru Testcase ::\r\n",
              taskObj->taskId);
            
    /* Test scenario 1: Check when druOwner is CSL_DRU_OWNER_UDMAC_TR */
    timeout          = 0U;
    chHandle         = &chObj;
    chType           = UDMA_CH_TYPE_UTC;
    backUpDrvObj     = taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];  
    UdmaChPrms_init(&chPrms, chType);
    chPrms.utcId     = UDMA_UTC_ID_MSMC_DRU0;
    drvHandle        = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    Udma_ChUtcPrms utcPrms;
    UdmaChUtcPrms_init(&utcPrms);
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigUtc(chHandle, &utcPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chEnable(chHandle);
            if(UDMA_SOK == retVal)
            {
                retVal = Udma_chPause(chHandle);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: chPause:: Pos::"
                              " Check when druOwner is CSL_DRU_OWNER_UDMAC_TR!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = Udma_chResume(chHandle);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: ChResume:: Pos::"
                                  " Check when druOwner is CSL_DRU_OWNER_UDMAC_TR!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = Udma_chGetStats(chHandle, &chStats);
                        if(UDMA_SOK != retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: ChGetStats:: Pos::"
                                      " Check when chType is UDMA_CH_TYPE_UTC!!\n");
                            retVal = UDMA_EFAIL;
                        }
                        else
                        {
                            Udma_chDisable(chHandle, timeout);
                            Udma_chClose(chHandle); 
                        } 
                    }
                }
            }
        }
    }
    taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0] = backUpDrvObj;

    return retVal;
}

/* 
 * Test Case Description: Verifies the function Udma_chConfigUtc and Udma_chSetSwTrigger
 * Test scenario 1: Check Udma_chConfigUtc when utcPrms are not initialized and utcId is UDMA_UTC_ID_VPAC_TC0
 * Test scenario 2: Check Udma_chSetSwTrigger when utcId is not UDMA_UTC_ID_MSMC_DRU0 
 * Test scenario 3: Check Udma_chDisable when utcId is not UDMA_UTC_ID_MSMC_DRU0 
 * Test scenario 4: Check Udma_druGetTriggerRegAddr when utcId is not UDMA_UTC_ID_MSMC_DRU0 
 * Test scenario 5: Check Udma_eventProgramSteering when utcId is not UDMA_UTC_ID_MSMC_DRU0 
 */ 
int32_t UdmaDruVhwaTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal = UDMA_SOK;
    struct Udma_ChObj      chObj;
    Udma_ChHandle          chHandle;
    Udma_ChUtcPrms         utcPrms;
    Udma_ChPrms            chPrms;
    Udma_DrvHandle         drvHandle;
    struct Udma_DrvObj     backUpDrvObj;
    uint32_t               chType;
    uint32_t               trigger;
    uint32_t               timeout = 0U;
    uint16_t               backUpDevIdPsil;
    uint32_t               backUpUtcId;
    volatile uint64_t     *pSwTrigReg = (volatile uint64_t *) NULL_PTR;
    struct Udma_EventObj   eventObj;
    Udma_EventHandle       eventHandle;
    Udma_EventPrms         eventPrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA ChConfigUtc Dru Vhwa negative Testcase ::\r\n",
              taskObj->taskId);

    retVal = Sciclient_pmSetModuleState(TISCI_DEV_VPAC0,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER); 
    if(UDMA_SOK != retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: Sciclient_pmSetModuleState failed"
                  " to power ON VPAC0!!\n");
        retVal = UDMA_EFAIL;
    }

    /* Test scenario 1: Check Udma_chConfigUtc when utcPrms are not initialized and utcId is UDMA_UTC_ID_VPAC_TC0 */
    chHandle        = &chObj;
    backUpDrvObj    = taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
    chType          = UDMA_CH_TYPE_UTC;
    drvHandle       = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
    UdmaChPrms_init(&chPrms, chType);
    backUpUtcId     = chPrms.utcId;    
    chPrms.utcId    = UDMA_UTC_ID_VPAC_TC0;
    retVal          = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigUtc(chHandle, &utcPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: chConfigUtc:: Neg::"
                      " Check when utcPrms are not initialized and utcId is UDMA_UTC_ID_VPAC_TC0!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal       = UDMA_SOK;
            chPrms.utcId = backUpUtcId;
        }
        Udma_chClose(chHandle);
    }
    taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0] = backUpDrvObj;

    /* Test scenario 2: Check Udma_chSetSwTrigger when utcId is not UDMA_UTC_ID_MSMC_DRU0 */
    if(UDMA_SOK == retVal)
    {
        trigger   = CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL1;
        drvHandle = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.utcId                    = UDMA_UTC_ID_VPAC_TC0;
        chPrms.fqRingPrms.ringMem       = &gUdmaTestDruRingMem[0U];
        chPrms.cqRingPrms.ringMem       = &gUdmaTestDruCompRingMem[0U];
        chPrms.tdCqRingPrms.ringMem     = &gUdmaTestDruTdCompRingMem[0U];
        chPrms.fqRingPrms.ringMemSize   = UDMA_TEST_APP_RING_MEM_SIZE;
        chPrms.cqRingPrms.ringMemSize   = UDMA_TEST_APP_RING_MEM_SIZE;
        chPrms.tdCqRingPrms.ringMemSize = UDMA_TEST_APP_RING_MEM_SIZE;
        chPrms.fqRingPrms.elemCnt       = UDMA_TEST_APP_RING_ENTRIES;
        chPrms.cqRingPrms.elemCnt       = UDMA_TEST_APP_RING_ENTRIES;
        chPrms.tdCqRingPrms.elemCnt     = UDMA_TEST_APP_RING_ENTRIES;
        retVal                          = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChUtcPrms_init(&utcPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigUtc(chHandle, &utcPrms);
            if(UDMA_SOK == retVal)
            {
                retVal = Udma_chSetSwTrigger(chHandle, trigger);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: ChGetSwTriggerRegister:: Neg::"
                              " Check when utcId is not UDMA_UTC_ID_MSMC_DRU0!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = UDMA_SOK;
                    retVal = Udma_chEnable(chHandle);
                    /* Test scenario 3: Check Udma_chDisable when utcId is not UDMA_UTC_ID_MSMC_DRU0 */
                    if(UDMA_SOK == retVal)
                    {
                        backUpDevIdPsil                = chHandle->drvHandle->devIdPsil;
                        chHandle->drvHandle->devIdPsil = TISCI_DEV_NAVSS0 - 1U;
                        retVal                         = Udma_chDisable(chHandle, timeout);
                        if(UDMA_SOK == retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: chDisableExtChan:: Neg::"
                                      " Check when utcId is not UDMA_UTC_ID_MSMC_DRU0!!\n");
                            retVal = UDMA_EFAIL;
                        }
                        else
                        {
                            retVal = UDMA_SOK;
                            chHandle->drvHandle->devIdPsil = backUpDevIdPsil;
                            retVal = Udma_chPause(chHandle);
                            if(UDMA_SOK == retVal)
                            {
                                retVal = Udma_chResume(chHandle);
                                if(UDMA_SOK != retVal)
                                {
                                    GT_0trace(taskObj->traceMask, GT_ERR,
                                            " |TEST INFO|:: FAIL:: UDMA:: Udma_chResume:: Pos::"
                                            " Check when utcId is not UDMA_UTC_ID_MSMC_DRU0!!\n");
                                    retVal = UDMA_EFAIL;
                                }
                            }
                        }
                        Udma_chDisable(chHandle, timeout);
                    }
                }
            }
            Udma_chClose(chHandle);
        }
        taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0] = backUpDrvObj;
    }

    /* Test scenario 4: Check Udma_druGetTriggerRegAddr when utcId is not UDMA_UTC_ID_MSMC_DRU0 */
    if(UDMA_SOK == retVal)
    {
        drvHandle    = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.utcId = UDMA_UTC_ID_VPAC_TC0;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChUtcPrms_init(&utcPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigUtc(chHandle, &utcPrms);
            if(UDMA_SOK == retVal)
            {
                pSwTrigReg = Udma_druGetTriggerRegAddr(chHandle);
                if(NULL_PTR != pSwTrigReg)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|::FAIL:: UDMA:: Udma_druGetTriggerRegAddr :: Neg::"
                              " Check when utcId is not UDMA_UTC_ID_MSMC_DRU0!!\n");
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

    /* Test scenario 5: Check Udma_eventRegister when eventType is UDMA_EVENT_TYPE_TR and utcId is not UDMA_UTC_ID_MSMC_DRU0 */
    if(UDMA_SOK == retVal)
    {
        drvHandle    = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.utcId = UDMA_UTC_ID_VPAC_TC0;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChUtcPrms_init(&utcPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chConfigUtc(chHandle, &utcPrms);
            if(UDMA_SOK == retVal)
            {
                eventHandle = &eventObj;
                UdmaEventPrms_init(&eventPrms);
                eventPrms.eventMode         = UDMA_EVENT_MODE_EXCLUSIVE;
                eventPrms.eventType         = UDMA_EVENT_TYPE_TR;
                eventPrms.masterEventHandle = NULL_PTR;;
                eventPrms.chHandle          = chHandle;
                eventPrms.eventCb           = &udmaTestEventCb; 
                retVal = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|::FAIL:: UDMA:: Udma_eventProgramSteering :: Neg:: "
                              " Check when utcId is not UDMA_UTC_ID_MSMC_DRU0!!\n");
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
 * Test Case Description: Verifies the function Udma_eventRegister
 * Test scenario 1: Check Udma_eventProgramSteering when chType is UDMA_CH_TYPE_UTC 
 *                  and extChNum is CSL_DRU_NUM_CH chType
 */
int32_t UdmaEventProgramSteeringTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                retVal = UDMA_SOK;
    Udma_DrvHandle         drvHandle;
    struct Udma_EventObj   eventObj;
    Udma_EventHandle       eventHandle;
    Udma_EventPrms         eventPrms;
    struct Udma_DrvObj     backUpDrvObj;
    Udma_RingMonHandle     monHandle;
    struct Udma_RingMonObj monObj;
    Udma_ChHandle          chHandle;
    struct Udma_ChObj      chObj;
    Udma_ChPrms            chPrms;
    uint32_t               chType;
    uint32_t               bkupExtCh;

    chHandle         = &chObj;
    chType           = UDMA_CH_TYPE_UTC;
    backUpDrvObj     = taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];  
    UdmaChPrms_init(&chPrms, chType);
    chPrms.utcId     = UDMA_UTC_ID_MSMC_DRU0;
    drvHandle        = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    Udma_ChUtcPrms utcPrms;
    UdmaChUtcPrms_init(&utcPrms);
    utcPrms.druOwner = CSL_DRU_OWNER_DIRECT_TR;
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_chConfigUtc(chHandle, &utcPrms);
        if(UDMA_SOK == retVal)
        {
            eventHandle                 = &eventObj;
            monHandle                   = &monObj;
            UdmaEventPrms_init(&eventPrms);
            eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
            eventPrms.eventType         = UDMA_EVENT_TYPE_TR;
            eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);;
            eventPrms.monHandle         = monHandle;
            eventPrms.chHandle          = chHandle;
            eventPrms.eventCb           = &udmaTestEventCb; 
            bkupExtCh                   = chHandle->extChNum;
            chHandle->extChNum          = CSL_DRU_NUM_CH;
            retVal                      = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
            if(UDMA_SOK == retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                            " |TEST INFO|:: FAIL:: UDMA:: Udma_eventProgramSteering::Neg:: Check when" 
                            " chType is UDMA_CH_TYPE_UTC and extChNum is CSL_DRU_NUM_CH chType!!\n");
                retVal = UDMA_EFAIL;
                Udma_eventUnRegister(eventHandle);
            }
            else 
            {
                retVal             = UDMA_SOK;
                chHandle->extChNum = bkupExtCh;
            }
        }
        Udma_chClose(chHandle);
    }
    taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0] = backUpDrvObj;

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_rmAllocExtCh
 * Test scenario 1: Check when chNum is less than numUtcCh
 * Test scenario 2: Check Udma_rmAllocExtCh when no resource is available 
 *                  and chNum is less than numUtcCh
 * Test scenario 3: Check Udma_rmAllocExtCh when no resource is available
 * Test scenario 4: Check Udma_rmAllocExtCh and chNum is equal to numUtcCh
 */
int32_t UdmaTestRmAllocExtChNeg(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    struct Udma_ChObj  chObj;
    Udma_ChHandle      chHandle;
    struct Udma_DrvObj backUpDrvObj;
    Udma_DrvHandle     drvHandle;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
  
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA RmAllocExtCh negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when chNum is less than numUtcCh */
    chHandle     = &chObj;
    chType       = UDMA_CH_TYPE_UTC;
    backUpDrvObj = taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];           
    UdmaChPrms_init(&chPrms, chType);
    chPrms.utcId = UDMA_UTC_ID_MSMC_DRU0;
    drvHandle    = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
    chPrms.chNum = drvHandle->initPrms.rmInitPrms.numUtcCh[chPrms.utcId] - 1U;
    retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK != retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocExtCh::Pos::" 
                  " Check when chNum is less than numUtcCh!!\n");
        retVal = UDMA_EFAIL;
    }
    else 
    {
        retVal = UDMA_SOK;
        Udma_chClose(chHandle);
    }
    taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0] = backUpDrvObj; 

    /* Test scenario 2: Check Udma_rmAllocExtCh when no resource is available 
                        and chNum is less than numUtcCh */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        chType       = UDMA_CH_TYPE_UTC;
        backUpDrvObj = taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];           
        UdmaChPrms_init(&chPrms, chType);
        chPrms.utcId = UDMA_UTC_ID_MSMC_DRU0;
        drvHandle    = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.numUtcCh[chPrms.utcId] - 1U;
        drvHandle->utcChFlag[chPrms.utcId][0] = 0U;
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocExtCh::Neg:: Check when" 
                      " no resource is available and chNum is less than numUtcCh!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else 
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0] = backUpDrvObj; 
    }

    /* Test scenario 3: Check Udma_rmAllocExtCh when no resource is available */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        chType       = UDMA_CH_TYPE_UTC;
        backUpDrvObj = taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];           
        UdmaChPrms_init(&chPrms, chType);
        chPrms.utcId = UDMA_UTC_ID_MSMC_DRU0;
        drvHandle    = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
        drvHandle->initPrms.rmInitPrms.numUtcCh[chPrms.utcId] = 0U;
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocExtCh::Neg::" 
                      " Check when no resource is available!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else 
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0] = backUpDrvObj; 
    }

    /* Test scenario 4: Check Udma_rmAllocExtCh and chNum is equal to numUtcCh */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        chType       = UDMA_CH_TYPE_UTC;       
        UdmaChPrms_init(&chPrms, chType);
        chPrms.utcId = UDMA_UTC_ID_MSMC_DRU0;
        drvHandle    = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.numUtcCh[chPrms.utcId];
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocExtCh::Neg:: Check when" 
                      " chNum is equal to numUtcCh!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else 
        {
            retVal = UDMA_SOK;
        }
    }

    return retVal;
}
