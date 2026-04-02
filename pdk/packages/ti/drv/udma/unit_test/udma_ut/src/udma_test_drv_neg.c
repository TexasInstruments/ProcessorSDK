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

/**
 *  \file udma_test_drv_neg.c
 *
 *  \brief UDMA Driver Init and Deinit Negative Test Cases.
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

static void *udmaTestRmLockMutexCreate(void);
static void *udmaTestPrintLockMutexCreate(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static uint32_t gMutexCnt = 0U;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * Test Case Description: Verifies Udma_init function when drvHandle 
 * and initPrms are NULL/Invalid arguments
 */
int32_t udmaTestUdmaInitNegTc(UdmaTestTaskObj *taskObj)
{
    int32_t retVal       = UDMA_SOK;
    int32_t instId       = 0U;
    uint8_t  tcIdx       = 0U; /* Test Case Index */
    UdmaTestObj *testObj = taskObj->testObj;
    Udma_InitPrms  initPrmsTemp;
    Udma_DrvHandle drvHandle[3] = {(Udma_DrvHandle) NULL_PTR, 
                                   (Udma_DrvHandle) NULL_PTR, 
                                   &testObj->drvObj[instId]};

    Udma_InitPrms *initPrms[3]  = {(Udma_InitPrms *) NULL_PTR, 
                                   &initPrmsTemp,
                                   (Udma_InitPrms *) NULL_PTR};

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d:: Udma_Init function verification testcase for invalid drvHandle and initPrms\r\n", taskObj->taskId);
    
    /* Deinit the driver */
    retVal = udmaTestDeinitDriver(testObj);
    if(UDMA_SOK == retVal)
    {
        for(tcIdx = 0U; tcIdx < 3U; tcIdx++)
        {
            retVal = Udma_init(drvHandle[tcIdx], initPrms[tcIdx]);
            if(UDMA_SOK != retVal)
            {
                GT_1trace(testObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Test%d:: Pass\n", tcIdx);
            }
            else
            {
                GT_1trace(testObj->traceMask, GT_ERR,
                          " |TEST INFO|:: Test%d:: Failed!!\n", tcIdx);
            }
        }
    }
    /* Re-init driver with default params */
    retVal = udmaTestInitDriver(testObj);
    return (retVal);
}

/*
 * Test Case Description: Verifies Udma_init function for an 
 * invalid rmLock
 */
int32_t udmaTestUdmaInitRmLockNegTc(UdmaTestTaskObj *taskObj)
{
    int32_t retVal       = UDMA_SOK;
    int32_t instId       = 0U;
    UdmaTestObj *testObj = taskObj->testObj;
    Udma_DrvHandle drvHandle; 
    Udma_InitPrms initPrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d:: Verify Udma_Init function for invalid RmLock\r\n", taskObj->taskId);
    
    /* Deinit the driver */
    retVal = udmaTestDeinitDriver(testObj);
    if(UDMA_SOK == retVal)
    {
        for(instId = UDMA_INST_ID_START; instId <= UDMA_TEST_INST_ID_MAX; instId++)
        {
            drvHandle = &testObj->drvObj[instId];
            UdmaInitPrms_init(instId, &initPrms);
            initPrms.osalPrms.createMutex = &udmaTestRmLockMutexCreate;

            retVal = Udma_init(drvHandle, &initPrms);
            if(UDMA_SOK != retVal)
            {
                GT_1trace(testObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Test%d:: Pass\n", instId);
            }
            else
            {
                GT_1trace(testObj->traceMask, GT_ERR,
                          " |TEST INFO|:: Test%d:: Failed!!\n", instId);
            }
        }
    }
    /* Re-init driver with default params */
    retVal = udmaTestInitDriver(testObj);
    return (retVal);
}

/*
 * Test Case Description: Function reproduces Udma_Init 
 * failure when PrintLock is null pointer
 */
int32_t udmaTestUdmaInitPrintLockNegTc(UdmaTestTaskObj *taskObj)
{
    int32_t retVal       = UDMA_SOK;
    int32_t instId       = 0U;
    UdmaTestObj *testObj = taskObj->testObj;
    Udma_DrvHandle drvHandle; 
    Udma_InitPrms initPrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d:: Udma_Init verification testcase for an invalid printLock\r\n", taskObj->taskId);
    
    /* Deinit the driver */
    retVal = udmaTestDeinitDriver(testObj);
    if(UDMA_SOK == retVal)
    {
        for(instId = UDMA_INST_ID_START; instId <= UDMA_TEST_INST_ID_MAX; instId++)
        {
            drvHandle = &testObj->drvObj[instId];
            UdmaInitPrms_init(instId, &initPrms);
            initPrms.osalPrms.createMutex = &udmaTestPrintLockMutexCreate;

            retVal = Udma_init(drvHandle, &initPrms);
            if(UDMA_SOK != retVal)
            {
                GT_1trace(testObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Test%d:: Pass\n", instId);
            }
            else
            {
                GT_1trace(testObj->traceMask, GT_ERR,
                          " |TEST INFO|:: Test%d:: Failed!!\n", instId);
            }
        }
    }
    /* Re-init driver with default params */
    retVal = udmaTestInitDriver(testObj);
    return (retVal);
}

/*
 * Test Case Description: This function fails Udma_Init 
 * for an invalid sciclient proxy configuration
 */
int32_t udmaTestUdmaInitRmSetProxyNegTc(UdmaTestTaskObj *taskObj)
{
    int32_t  retVal          = UDMA_SOK;
    uint32_t instId          = 0U;
    UdmaTestObj *testObj     = taskObj->testObj;
    Udma_DrvHandle drvHandle = (Udma_DrvHandle) NULL_PTR;
    Udma_InitPrms initPrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d:: Udma_init testcase for an invalid sciclient proxy configuration\r\n", taskObj->taskId);

    /* Deinit the driver */
    retVal = udmaTestDeinitDriver(testObj);
    if(UDMA_SOK == retVal)
    {
        for(instId = UDMA_INST_ID_START; instId < UDMA_NUM_UDMAP_INST_ID ; instId++)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Test%d\r\n", instId);

            drvHandle = &testObj->drvObj[instId];
            UdmaInitPrms_init(instId, &initPrms);

            drvHandle->initPrms.rmInitPrms.proxyThreadNum = 0U;
            initPrms.rmInitPrms.proxyThreadNum            = 0U;
            drvHandle->initPrms.printFxn                  = &udmaDrvPrint;

            retVal = Udma_init(drvHandle, &initPrms);
            if(UDMA_SOK != retVal)
            {
                GT_1trace(testObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Test%d:: Pass\n", instId);
            }
            else
            {
                GT_1trace(testObj->traceMask, GT_ERR,
                          " |TEST INFO|:: Test%d:: Failed!!\n", instId);
            }
        }
    }

    /* Re-init driver with default params */
    retVal = udmaTestInitDriver(testObj);
    return retVal;
}

/*
 * Test Case Description: Testcase to verify the Udma_PrmsInit
 * function with input parameters as null
 */
int32_t udmaTestUdmaPrmsInitNegTc(UdmaTestTaskObj *taskObj)
{
    int32_t  retVal      = UDMA_SOK;
    uint32_t tcIdx       = 0U;
    uint32_t instId[2]   = {0U, 5U};
    UdmaTestObj *testObj = taskObj->testObj;
    Udma_InitPrms  tempPrms;
    Udma_InitPrms *initPrms[2] ={(Udma_InitPrms*) NULL_PTR, &tempPrms};

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d:: Testcase which verifies Udma_PrmsInit function for NULL parameters\r\n", taskObj->taskId);

    /* Deinit the driver */
    retVal = udmaTestDeinitDriver(testObj);
    if(UDMA_SOK == retVal)
    {
        for(tcIdx = 0U; tcIdx < 2U; tcIdx++)
        {
            retVal = UdmaInitPrms_init(instId[tcIdx], initPrms[tcIdx]);
            if(UDMA_SOK != retVal)
            {
                GT_1trace(testObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Test%d:: Pass\n", tcIdx);
            }
            else
            {
                GT_1trace(testObj->traceMask, GT_ERR,
                          " |TEST INFO|:: Test%d:: Failed!!\n", tcIdx);
            }
        }
    }

    /* Re-init driver with default params */
    retVal = udmaTestInitDriver(testObj);                                                                                            
    return (retVal);
}

/*
 * Function Description: Verifies Udma_deinit for 
 * an incorrect input parameters 
 */
int32_t udmaTestUdmaDeinitNegTc(UdmaTestTaskObj *taskObj)
{
    int32_t  retVal          = UDMA_SOK;
    uint32_t instId          = 0U;
    uint32_t tcIdx           = 0U;
    UdmaTestObj *testObj     = taskObj->testObj;
    Udma_DrvHandle drvHandle = (Udma_DrvHandle) NULL_PTR;
    Udma_EventHandle eventHandleObj;
    Udma_InitPrms initPrms;
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Test%d:: Testcase to verify Udma_init for an invalid drvHandle\r\n", taskObj->taskId);
    
    /* Deinit the driver */
    retVal = udmaTestDeinitDriver(testObj);
    if(UDMA_SOK == retVal)
    {
       for(tcIdx = 0U; tcIdx < 3U ; tcIdx++)
        {
            /* Test Case: Udma_deinit fails when drvHandle is NULL_PTR */
            if(0U == tcIdx)
            {
                drvHandle = (Udma_DrvHandle) NULL_PTR;
            }
            else if(1U == tcIdx)
            {
                drvHandle = &testObj->drvObj[instId];
                drvHandle->drvInitDone = UDMA_DEINIT_DONE;
            }
            /* Test Case: Udma_deinit fails due to invalid drvHandle in globalEventHandle */
            else
            {
                drvHandle = &testObj->drvObj[instId];
                UdmaInitPrms_init(instId, &initPrms);
                drvHandle->globalEventHandle            = (Udma_EventHandle) &eventHandleObj;
                drvHandle->globalEventHandle->drvHandle = (Udma_DrvHandle) NULL_PTR;
                drvHandle->drvInitDone = UDMA_INIT_DONE;
            }

            retVal = Udma_deinit(drvHandle);
            if(UDMA_SOK != retVal)
            {
                GT_1trace(testObj->traceMask, GT_INFO1,
                          " |TEST INFO|:: Test%d:: Pass\n", tcIdx);
            }
            else
            {
                GT_1trace(testObj->traceMask, GT_ERR,
                          " |TEST INFO|:: Test%d:: Failed!!\n", tcIdx);
            }
        }
    }

    /* Re-init driver with default params */
    retVal = udmaTestInitDriver(testObj);
    return (retVal);
}

/*
 * Function Description: Function which fails Udma_Init from registering
 * global event
 */
int32_t udmaTestUdmaInitEventRegTc(UdmaTestTaskObj *taskObj)
{
    int32_t  retVal           = UDMA_SOK;
    uint32_t instId           = 0U;
    uint16_t dst_Idx          = 0U;
    uint16_t dst_Id           = 0U;
    UdmaTestObj *testObj      = taskObj->testObj;
    Udma_DrvHandle drvHandle  = (Udma_DrvHandle) NULL_PTR;
    Udma_InitPrms initPrms;
    struct tisci_msg_rm_irq_set_req rmIrqReq;
    struct tisci_msg_rm_irq_set_resp rmIrqResp;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d:: Testcase to verify Udma_init from registering global event for an invalid configuration\r\n", taskObj->taskId);

    /* Deinit the driver */
    retVal = udmaTestDeinitDriver(testObj);
    if(UDMA_SOK == retVal)
    {
        for(instId = UDMA_INST_ID_START; instId <= UDMA_TEST_INST_ID_MAX ; instId++)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1, " |TEST INFO|:: Test%d\r\n", instId);

            drvHandle = &testObj->drvObj[instId];
            UdmaInitPrms_init(instId, &initPrms);
            Udma_initDrvHandle(drvHandle);
            dst_Id  = Udma_getCoreSciDevId();
#if defined (_TMS320C6X)
            /*
             * For C66x Sciclient translates NAVSS IR Idx to corresponding C66 IR Idx,
             * Not the Core Interrupt Idx.
             */
            dst_Idx = initPrms.rmInitPrms.startC66xCoreIntr;
#else
            uint16_t irIntrNum = initPrms.rmInitPrms.startIrIntr;
            retVal = Sciclient_rmIrqTranslateIrOutput(drvHandle->devIdIr, irIntrNum, dst_Id, &dst_Idx);
            if(UDMA_SOK != retVal)
            {
                GT_1trace(testObj->traceMask, GT_ERR, "Sciclient RmIrq failed for an instId %d!!!\n", instId);
            }
            else
#endif
            {
                rmIrqReq.valid_params           = 0U;
                rmIrqReq.valid_params          |= TISCI_MSG_VALUE_RM_DST_ID_VALID;
                rmIrqReq.valid_params          |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
                rmIrqReq.valid_params          |= TISCI_MSG_VALUE_RM_IA_ID_VALID;
                rmIrqReq.valid_params          |= TISCI_MSG_VALUE_RM_VINT_VALID;
                rmIrqReq.global_event           = 0U;
                rmIrqReq.src_id                 = 0U;
                rmIrqReq.src_index              = 0U;
                rmIrqReq.dst_id                 = dst_Id;
                rmIrqReq.dst_host_irq           = dst_Idx;
                rmIrqReq.ia_id                  = drvHandle->devIdIa;
                rmIrqReq.vint                   = initPrms.rmInitPrms.startVintr;
                rmIrqReq.vint_status_bit_index  = 0U;
                rmIrqReq.secondary_host         = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

                /* Get Resource */
                retVal = Sciclient_rmIrqSet(&rmIrqReq, &rmIrqResp, UDMA_SCICLIENT_TIMEOUT);
                if(UDMA_SOK != retVal)
                {
                    GT_1trace(testObj->traceMask, GT_ERR, "Sciclient Irq Set failed for an instId %d!!!\n", instId);
                }
                else
                {
                    drvHandle->initPrms.printFxn = &udmaDrvPrint;
                    initPrms.printFxn            = &udmaDrvPrint;
                    retVal = Udma_init(drvHandle, &initPrms);
                    if(UDMA_SOK != retVal)
                    {
                        GT_1trace(testObj->traceMask, GT_INFO1, " |TEST INFO|:: Test%d:: Pass\r\n", instId);
                    }
                    else
                    {
                        GT_1trace(testObj->traceMask, GT_ERR, " |TEST INFO|:: Test%d:: Failed!!!\r\n", instId);
                    }

                    retVal = Sciclient_rmIrqRelease((struct tisci_msg_rm_irq_release_req*)&rmIrqReq, UDMA_SCICLIENT_TIMEOUT);
                    if(UDMA_SOK != retVal)
                    {
                        GT_1trace(testObj->traceMask, GT_ERR, " Sciclient Irq Release failed for an instId %d!!!\n", instId);
                    }
                }
            }
        }
    }

    /* Re-init driver with default params */
    retVal = udmaTestInitDriver(testObj);
    return (retVal);
}

/*
 * Function Description: Returns MutexHandle as null pointer 
 */
static void *udmaTestRmLockMutexCreate(void)
{
    SemaphoreP_Handle mutexHandle = NULL_PTR;
    return (mutexHandle);
}

/*
 * Function Description: Helper function which fails Udma_Init 
 * by assigning PrintLock as null pointer.
 * This functions returns valid mutexHandler when gMutexCnt is 0U 
 * and null pointer when gMutexCnt is not equal to 0U. It 
 * returns Mutex handler for RmLock when gMutexCnt is 0U and returns null pointer 
 * for PrintLock when gMutexCnt is not equal to 0U.
 */
static void *udmaTestPrintLockMutexCreate(void)
{
    SemaphoreP_Params semPrms;
    SemaphoreP_Handle mutexHandle;

    SemaphoreP_Params_init(&semPrms);
    semPrms.mode = SemaphoreP_Mode_BINARY;
    mutexHandle = (void *) SemaphoreP_create(1U, &semPrms);

    if(0U == gMutexCnt)
    {
        gMutexCnt = gMutexCnt + 1;
    }
    else
    {
        mutexHandle = NULL_PTR;
    }

    return (mutexHandle);
}

/*
 * Function Description: Function to test Udma_Init and Udma_deinit when Global Event is disabled
 */
int32_t udmaTestUdmaInitSkipEventRegTc(UdmaTestTaskObj *taskObj)
{
    int32_t  retVal           = UDMA_SOK;
    uint32_t instId           = 0U;
    UdmaTestObj *testObj      = taskObj->testObj;
    Udma_DrvHandle drvHandle  = (Udma_DrvHandle) NULL_PTR;
    Udma_InitPrms initPrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d:: Testcase to verify Udma_init and Udma_deinit when GlobalEventReg value is True\r\n", taskObj->taskId);

    /* Deinit the driver */
    retVal = udmaTestDeinitDriver(testObj);
    if(UDMA_SOK == retVal)
    {
        for(instId = UDMA_INST_ID_START; instId <= UDMA_TEST_INST_ID_MAX ; instId++)
        {
            drvHandle = &testObj->drvObj[instId];
            UdmaInitPrms_init(instId, &initPrms);
            initPrms.skipGlobalEventReg = UTRUE;
            retVal = Udma_init(drvHandle, &initPrms);
            if(UDMA_SOK != retVal)
            {
                GT_1trace(testObj->traceMask, GT_ERR, " |TEST INFO|:: UDMA Init Instance %d:: Failed!!!\r\n", instId);
            }
            else
            {
                GT_1trace(testObj->traceMask, GT_INFO1, " |TEST INFO|:: UDMA Init Instance %d:: Pass\r\n", instId);
            }
            retVal = Udma_deinit(drvHandle);
            if(UDMA_SOK != retVal)
            {
                GT_1trace(testObj->traceMask, GT_ERR, " |TEST INFO|:: UDMA Denit Instance %d:: Failed!!!\r\n", instId);
            }
            else
            {
                GT_1trace(testObj->traceMask, GT_INFO1, " |TEST INFO|:: UDMA Deinit Instance %d:: Pass\r\n", instId);
            }
            initPrms.skipGlobalEventReg = UFALSE;
        }
    }

    /* Re-init driver with default params */
    retVal = udmaTestInitDriver(testObj);
    return (retVal);
}

/* 
 * Test Case Description: Verifies the init functions when
 * Test scenario 1: Check when txPrms is Null
 * Test scenario 2: Check when rxPrms is Null
 * Test scenario 3: Check when utcPrms is Null
 * Test scenario 4: Check when chPrms is Null
 * Test scenario 5: Check when pdmaPrms is Null
 * Test scenario 6: Check when eventPrms is Null
 * Test scenario 7: Check when osalPrms is NULL
 * Test scenario 8: Check when cachePrms is NULL
 */ 
int32_t UdmaInitPrmsNeg(UdmaTestTaskObj *taskObj)
{
    int32_t  retVal = UDMA_SOK;
    uint32_t chType = UDMA_CH_TYPE_TX;

    /* Test scenario 1: Check when txPrms is Null */
    UdmaChTxPrms_init(NULL, chType);
    /* Test scenario 2: Check when rxPrms is Null */
    UdmaChRxPrms_init(NULL, chType);
    /* Test scenario 3: Check when utcPrms is Null */
    UdmaChUtcPrms_init(NULL);
    /* Test scenario 4: Check when chPrms is Null */
    UdmaChPrms_init(NULL, chType);
    /* Test scenario 5: Check when pdmaPrms is Null */
    UdmaChPdmaPrms_init(NULL);
    /* Test scenario 6: Check when eventPrms is Null */
    UdmaEventPrms_init(NULL);
    /* Test scenario 7: Check when osalPrms is NULL */
    UdmaOsalPrms_init(NULL);
    /* Test scenario 8: Check when cachePrms is NULL */
    UdmaOsalCachePrms_init(NULL);

    return retVal;
}

