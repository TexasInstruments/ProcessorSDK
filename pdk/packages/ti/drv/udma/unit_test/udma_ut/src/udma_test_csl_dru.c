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
 *  \file udma_test_csl_dru.c
 *
 *  \brief File containing test cases for CSL DRU APIs.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <udma_test.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
  #define UDMA_UTC_BASE_DRU0   (CSL_COMPUTE_CLUSTER0_MMR_DRU0_MMR_CFG_DRU_BASE)
#else
  #define UDMA_UTC_BASE_DRU0   (CSL_COMPUTE_CLUSTER0_DRU_BASE)
#endif 

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

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

/*
 * Test Case Description: Verifies the function CSL_druGetCapabilities
 * Test scenario 1: Check when queueCfg is NULL
 * Test scenario 2: NULL check for druCap
 * Test scenario 3: Check with CSL_druGetCapabilities valid arguments
 */
int32_t UdmaTestCSLDruGetCapabilities(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal = UDMA_SOK;
    CSL_DRU_t           *pRegs  = ((CSL_DRU_t *) UDMA_UTC_BASE_DRU0);
    CSL_DruCapabilities  druCap;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: CSL_druGetCapabilities Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when queueCfg is NULL */
    UdmaDruQueueConfig_init(NULL);

    /* Test scenario 2: NULL check for druCap */
    retVal = CSL_druGetCapabilities(pRegs, NULL);
    if(CSL_PASS == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: CSL_druGetCapabilities:: Neg:: "
                  " NULL check for druCap!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }

    /* Test scenario 3: Check with CSL_druGetCapabilities valid arguments */
    if(UDMA_SOK == retVal)
    {
        retVal = CSL_druGetCapabilities(pRegs, &druCap);
        if(CSL_PASS != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_druGetCapabilities:: Pos:: "
                      " Check with CSL_druGetCapabilities valid arguments!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function CSL_druChSetGlobalTrigger1
 * Test scenario 1: Check when DRU register base is NULL
 * Test scenario 2: Check CSL_druChSetGlobalTrigger1 valid arguments
 */
int32_t UdmaTestCSLDruChSetGlobalTrigger1(UdmaTestTaskObj *taskObj)
{
    int32_t    retVal = UDMA_SOK;
    CSL_DRU_t *pRegs  = ((CSL_DRU_t *) UDMA_UTC_BASE_DRU0);
    uint32_t   startChId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: CSL_druChSetGlobalTrigger1 Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when DRU register base is NULL */
    startChId = 0U;
    retVal    = CSL_druChSetGlobalTrigger1(NULL, startChId);
    if(CSL_PASS == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: CSL_druChSetGlobalTrigger1:: Neg:: "
                  " Check when DRU register base is NULL!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }

    /* Test scenario 2: Check CSL_druChSetGlobalTrigger1 valid arguments */
    if(UDMA_SOK == retVal)
    {
        retVal = CSL_druChSetGlobalTrigger1(pRegs, startChId);
        if(CSL_PASS != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_druChSetGlobalTrigger1:: Pos:: "
                      " Check CSL_druChSetGlobalTrigger1 valid arguments!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function CSL_druChSetGlobalTrigger0
 * Test scenario 1: Check when DRU register base is NULL
 * Test scenario 2: Check CSL_druChSetGlobalTrigger0 valid arguments
 */
int32_t UdmaTestCSLDruChSetGlobalTrigger0(UdmaTestTaskObj *taskObj)
{
    int32_t    retVal = UDMA_SOK;
    CSL_DRU_t *pRegs  = ((CSL_DRU_t *) UDMA_UTC_BASE_DRU0);
    uint32_t   startChId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: CSL_druChSetGlobalTrigger0 Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when DRU register base is NULL */
    startChId = 0U;
    retVal    = CSL_druChSetGlobalTrigger0(NULL, startChId);
    if(CSL_PASS == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: CSL_druChSetGlobalTrigger0:: Neg:: "
                  " Check when DRU register base is NULL!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }

    /* Test scenario 2: Check CSL_druChSetGlobalTrigger0 valid arguments */
    if(UDMA_SOK == retVal)
    {
        retVal = CSL_druChSetGlobalTrigger0(pRegs, startChId);
        if(CSL_PASS != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_druChSetGlobalTrigger0:: Pos:: "
                      " Check CSL_druChSetGlobalTrigger0 valid arguments!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function CSL_druChSetLocalTrigger0
 * Test scenario 1: Check when DRU register base is NULL
 * Test scenario 2: Check CSL_druChSetLocalTrigger0 valid arguments
 */
int32_t UdmaTestCSLDruChSetLocalTrigger0(UdmaTestTaskObj *taskObj)
{
    int32_t    retVal = UDMA_SOK;
    CSL_DRU_t *pRegs  = ((CSL_DRU_t *) UDMA_UTC_BASE_DRU0);
    uint32_t   startChId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: CSL_druChSetLocalTrigger0 Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when DRU register base is NULL */
    startChId = 0U;
    retVal    = CSL_druChSetLocalTrigger0(NULL, startChId);
    if(CSL_PASS == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: CSL_druChSetLocalTrigger0:: Neg:: "
                  " Check when DRU register base is NULL!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }

    /* Test scenario 2: Check CSL_druChSetLocalTrigger0 valid arguments */
    if(UDMA_SOK == retVal)
    {
        retVal = CSL_druChSetLocalTrigger0(pRegs, startChId);
        if(CSL_PASS != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_druChSetLocalTrigger0:: Pos:: "
                      " Check CSL_druChSetLocalTrigger0 valid arguments!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function CSL_druGetQueueStatus
 * Test scenario 1: Check when queueId is greater than CSL_DRU_NUM_QUEUE and
 *                  status is NULL
 * Test scenario 2: Check when status is NULL
 * Test scenario 3: Check when queueId is greater than CSL_DRU_NUM_QUEUE
 * Test scenario 4: Check CSL_druGetQueueStatus with valid arguments
 */
int32_t UdmaTestCSLDruChGetQueueStatus(UdmaTestTaskObj *taskObj)
{
    int32_t             retVal = UDMA_SOK;
    CSL_DRU_t          *pRegs  = ((CSL_DRU_t *) UDMA_UTC_BASE_DRU0);
    CSL_DruQueueStatus  status;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: CSL_druGetQueueStatus Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when queueId is greater than CSL_DRU_NUM_QUEUE and
     *                  status is NULL
     */
    retVal = CSL_druGetQueueStatus(pRegs, CSL_DRU_MAX_QUEUE , NULL);
    if(CSL_PASS == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: CSL_druGetQueueStatus:: Neg:: "
                  " Check when queueId is greater than CSL_DRU_NUM_QUEUE and "
                  " status is NULL!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }

    /* Test scenario 2: Check when status is NULL */
    if(UDMA_SOK == retVal)
    {
        retVal = CSL_druGetQueueStatus(pRegs, CSL_DRU_QUEUE_ID_0 , NULL);
        if(CSL_PASS == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_druGetQueueStatus:: Neg:: "
                      " Check when status is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 3: Check when queueId is greater than CSL_DRU_NUM_QUEUE */
    if(UDMA_SOK == retVal)
    {

        retVal = CSL_druGetQueueStatus(pRegs, CSL_DRU_MAX_QUEUE , &status);
        if(CSL_PASS == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_druGetQueueStatus:: Neg:: "
                      " Check when queueId is greater than CSL_DRU_NUM_QUEUE!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 4: Check CSL_druGetQueueStatus with valid arguments */
    if(UDMA_SOK == retVal)
    {
        retVal = CSL_druGetQueueStatus(pRegs, CSL_DRU_QUEUE_ID_0 , &status);
        if(CSL_PASS != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: chPause:: Pos:: "
                      " Check CSL_druGetQueueStatus with valid arguments!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function CSL_druChResume
 * Test scenario 1: Check when DRU register base is NULL
 */
int32_t UdmaTestCSLDruChResume(UdmaTestTaskObj *taskObj)
{
    int32_t    retVal = UDMA_SOK;
    CSL_DRU_t *pRegs  = ((CSL_DRU_t *) UDMA_UTC_BASE_DRU0);
    uint32_t   startChId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: CSL_druChResume Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when chId is UDMA_CH_FLAG_UTC*/
    startChId = 0U;
    retVal    = CSL_druChResume(pRegs, startChId);
    if(CSL_PASS != retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: CSL_druChResume:: Neg:: "
                  " Check when DRU register base is NULL!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function CSL_druChPause
 * Test scenario 1: Check when DRU register base is NULL
 */
int32_t UdmaTestCSLDruChPause(UdmaTestTaskObj *taskObj)
{
    int32_t    retVal = UDMA_SOK;
    CSL_DRU_t *pRegs  = ((CSL_DRU_t *) UDMA_UTC_BASE_DRU0);
    uint32_t   startChId;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: CSL_druChPause Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when DRU register base is NULL*/
    startChId = 0U;
    retVal    = CSL_druChPause(pRegs, startChId);
    if(CSL_PASS != retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: CSL_druChPause:: Neg:: "
                  " Check when DRU register base is NULL!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function CSL_druChIsTeardownComplete
 * Test scenario 1: Check when DRU register base is NULL
 */
int32_t UdmaTestCSLDruChIsTeardownComplete(UdmaTestTaskObj *taskObj)
{
    int32_t    retVal = UDMA_SOK;
    CSL_DRU_t *pRegs  = ((CSL_DRU_t *) UDMA_UTC_BASE_DRU0);
    uint32_t   startChId;
    uint32_t   status;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: CSL_druChIsTeardownComplete Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when DRU register base is NULL */
    startChId = CSL_DRU_MAX_CH;
    status    = CSL_druChIsTeardownComplete(pRegs, startChId);
    if(FALSE != status)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: CSL_druChIsTeardownComplete:: Neg:: "
                  " Check when DRU register base is NULL!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }

    if(UDMA_SOK == retVal)
    {
        retVal = CSL_druChEnable(pRegs, CSL_DRU_NUM_QUEUE);
        if(UDMA_SOK == retVal)
        {
            CSL_druChIsTeardownComplete(pRegs, CSL_DRU_NUM_QUEUE);
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function CSL_druChSetEvent
 * Test scenario 1: Check when DRU register base is NULL
 * Test scenario 2: Check CSL_druChSetEvent valid arguments
 */
int32_t UdmaTestCSLDruChSetEvent(UdmaTestTaskObj *taskObj)
{
    int32_t    retVal = UDMA_SOK;
    CSL_DRU_t *pRegs  = ((CSL_DRU_t *) UDMA_UTC_BASE_DRU0);
    uint32_t   startChId;
    uint64_t   evtNum;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: CSL_druChSetEvent Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when DRU register base is NULL */
    startChId = CSL_DRU_MAX_CH;
    evtNum    = UDMA_EVENT_INVALID;
    retVal    = CSL_druChSetEvent(pRegs, startChId, evtNum);
    if(CSL_PASS == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: CSL_druChSetEvent:: Neg:: "
                  " Check when DRU register base is NULL!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }

    /* Test scenario 2: Check CSL_druChSetEvent valid arguments */
    if(UDMA_SOK == retVal)
    {
        startChId = 0U;
        retVal    = CSL_druChSetEvent(pRegs, startChId, evtNum);
        if(CSL_PASS != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_druChSetEvent:: Pos:: "
                      " Check CSL_druChSetEvent valid arguments!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function CSL_druChConfig
 * Test scenario 1: Check when chId is greater than or equal to CSL_DRU_NUM_CH,
 *                  when queueId is greater than equal to CSL_DRU_NUM_QUEUE and chCfg
 *                  is NULL
 * Test scenario 2: Check when queueId is greater than equal to CSL_DRU_NUM_QUEUE and
 *                  chCfg is NULL
 * Test scenario 3: Check when chId is greater than or equal to CSL_DRU_NUM_CH
 * Test scenario 4: Check when chId is greater than or equal to CSL_DRU_NUM_CH and
 *                  when queueId is greater than equal to CSL_DRU_NUM_QUEUE
 * Test scenario 5: Check when queueId is greater than or equal to CSL_DRU_NUM_QUEUE
 */
int32_t UdmaTestCSLDruChConfig(UdmaTestTaskObj *taskObj)
{
    int32_t          retVal = UDMA_SOK;
    CSL_DRU_t       *pRegs  = ((CSL_DRU_t *) UDMA_UTC_BASE_DRU0);
    uint32_t         chId;
    CSL_DruChConfig  chCfg;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: CSL_druChConfig Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when chId is greater than or equal to CSL_DRU_NUM_CH,
     *                  when queueId is greater than equal to CSL_DRU_NUM_QUEUE and
     *                  chCfg is NULL
     */
    chId          = CSL_DRU_NUM_CH;
    chCfg.queueId = CSL_DRU_NUM_QUEUE;
    retVal        = CSL_druChConfig(pRegs, chId, NULL);
    if(CSL_PASS == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: CSL_druChConfig:: Neg:: "
                  " Check when chId is greater than or equal to CSL_DRU_NUM_CH,"
                  " when queueId is greater than equal to CSL_DRU_NUM_QUEUE and "
                  " chCfg is NULL!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check when queueId is greater than equal to CSL_DRU_NUM_QUEUE
         *                  and chCfg is NULL
         */
        chId          = CSL_DRU_NUM_CORE;
        chCfg.queueId = CSL_DRU_NUM_QUEUE;
        retVal        = CSL_druChConfig(pRegs, chId, NULL);
        if(CSL_PASS == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_druChConfig:: Neg:: "
                      " Check when queueId is greater than equal to CSL_DRU_NUM_QUEUE and "
                      " chCfg is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check when chId is greater than or equal to CSL_DRU_NUM_CH */
        chId          = CSL_DRU_NUM_CH;
        chCfg.queueId = CSL_DRU_NUM_QUEUE - 1U;
        retVal        = CSL_druChConfig(pRegs, chId, &chCfg);
        if(CSL_PASS == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_druChConfig:: Neg:: "
                      " Check when chId is greater than or equal to CSL_DRU_NUM_CH !!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check when chId is greater than or equal to CSL_DRU_NUM_CH and
         *                  when queueId is greater than equal to CSL_DRU_NUM_QUEUE
         */
        chId          = CSL_DRU_NUM_CH;
        chCfg.queueId = CSL_DRU_NUM_QUEUE;
        retVal        = CSL_druChConfig(pRegs, chId, &chCfg);
        if(CSL_PASS == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_druChConfig:: Neg:: "
                      " Check when chId is greater than or equal to CSL_DRU_NUM_CH and"
                      " when queueId is greater than equal to CSL_DRU_NUM_QUEUE!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check when queueId is greater than or equal to CSL_DRU_NUM_QUEUE */
        chId          = CSL_DRU_NUM_CORE;
        chCfg.queueId = CSL_DRU_NUM_QUEUE;
        retVal        = CSL_druChConfig(pRegs, chId, &chCfg);
        if(CSL_PASS == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_druChSetEvent:: Neg:: "
                      " Check when queueId is greater than or equal to CSL_DRU_NUM_QUEUE!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function CSL_druQueueConfig and CSL_druChGetTriggerRegAddr
 * Test scenario 1: Check when DRU register base is NULL
 * Test scenario 2: Check CSL_druChGetTriggerRegAddr when  DRU register base is NULL
 * Test scenario 3: Check CSL_druChGetTriggerRegAddr when ChId is invalid
 */
int32_t UdmaTestCSLDruQueueConfig(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal  = UDMA_SOK;
    CSL_DRU_t         *pRegs   = ((CSL_DRU_t *) UDMA_UTC_BASE_DRU0);
    uint32_t           queueId = CSL_DRU_QUEUE_ID_0;
    volatile uint64_t *regAddr = NULL;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: CSL_druQueueConfig Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check when DRU register base is NULL */
    retVal = CSL_druQueueConfig(pRegs, queueId, NULL);
    if(CSL_PASS == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: CSL_druQueueConfig:: Neg:: "
                  " Check when DRU register base is NULL!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check CSL_druChGetTriggerRegAddr when  DRU register base is NULL */
        regAddr = CSL_druChGetTriggerRegAddr(NULL, CSL_DRU_MAX_CH);
        if(NULL != regAddr)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_druChSetEvent:: Neg:: "
                      " Check CSL_druChGetTriggerRegAddr when  DRU register base is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: Check CSL_druChGetTriggerRegAddr when ChId is invalid */
        regAddr = CSL_druChGetTriggerRegAddr(pRegs, CSL_DRU_MAX_CH);
        if(NULL != regAddr)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_druChSetEvent:: Neg:: "
                      " Check CSL_druChGetTriggerRegAddr when ChId is invalid!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    return retVal;
}

