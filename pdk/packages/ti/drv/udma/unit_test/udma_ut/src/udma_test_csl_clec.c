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
 *  \file udma_test_csl_clec.c
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

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

static int32_t UdmaTestInitClec(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle, Udma_EventHandle eventHandle, uint32_t elemCnt);
static int32_t UdmaTestDeInitClec(Udma_ChHandle chHandle, Udma_EventHandle eventHandle);

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

int32_t udmaTestCslClec(UdmaTestTaskObj *taskObj)
{
    int32_t                     retVal    = UDMA_SOK;
    Udma_DrvHandle              drvHandle = &taskObj->testObj->drvObj[UDMA_TEST_DEFAULT_UDMA_INST];
    struct Udma_ChObj           chObj;
    Udma_ChHandle               chHandle  = &chObj;
    uint32_t                    elemCnt   = 10U;
    struct Udma_EventObj        eventObj;
    Udma_EventHandle            eventHandle = &eventObj;
    CSL_ClecEventConfig         evtCfg;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: CSL Clec Test ::\r\n", taskObj->taskId);

    retVal = UdmaTestInitClec(drvHandle, chHandle, eventHandle, elemCnt);

    /** 
     * Test CSL_clecConfigEvent when
     * CSL_CLEC_EVTRegs    : NULL
     * evtNum              : INVALID
     * CSL_ClecEventConfig : NULL
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        retVal = CSL_clecConfigEvent(NULL, UDMA_INTR_INVALID, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /** 
     * Test CSL_clecConfigEvent when
     * CSL_CLEC_EVTRegs    : NULL
     * evtNum              : INVALID
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        retVal = CSL_clecConfigEvent(NULL, UDMA_INTR_INVALID, &evtCfg);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecConfigEvent when
     * evtNum              : INVALID
     * CSL_ClecEventConfig : NULL
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        retVal = CSL_clecConfigEvent(drvHandle->clecRegs, UDMA_INTR_INVALID, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecConfigEvent when
     * evtNum              : INVALID
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        retVal = CSL_clecConfigEvent(drvHandle->clecRegs, UDMA_INTR_INVALID, &evtCfg);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecConfigEvent when
     * extEvtNum              : INVALID
     * c7xEvtNum              : INVALID
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        evtCfg.extEvtNum = UDMA_INTR_INVALID;
        evtCfg.c7xEvtNum = UDMA_INTR_INVALID;
        retVal = CSL_clecConfigEvent(drvHandle->clecRegs, 0U, &evtCfg);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecConfigEvent when
     * extEvtNum              : INVALID
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        evtCfg.extEvtNum = UDMA_INTR_INVALID;
        evtCfg.c7xEvtNum = 0U;
        retVal = CSL_clecConfigEvent(drvHandle->clecRegs, 0U, &evtCfg);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecConfigEvent when
     * c7xEvtNum              : INVALID
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        evtCfg.extEvtNum = 0U;
        evtCfg.c7xEvtNum = UDMA_INTR_INVALID;
        retVal = CSL_clecConfigEvent(drvHandle->clecRegs, 0U, &evtCfg);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecConfigEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecConfigEventLevel when
     * CSL_CLEC_EVTRegs    : NULL
     * evtNum              : INVALID
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecConfigEventLevel Testcase ::\r\n", taskObj->taskId);
        retVal = CSL_clecConfigEventLevel(NULL, UDMA_INTR_INVALID, 1U);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecConfigEventLevel Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecConfigEventLevel when
     * evtNum              : INVALID
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecConfigEventLevel Testcase ::\r\n", taskObj->taskId);
        retVal = CSL_clecConfigEventLevel(drvHandle->clecRegs, UDMA_INTR_INVALID, 1U);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecConfigEventLevel Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecConfigEventLevel when
     * CSL_CLEC_EVTRegs    : NULL
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecConfigEventLevel Testcase ::\r\n", taskObj->taskId);
        retVal = CSL_clecConfigEventLevel(NULL, 0U, 0U);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecConfigEventLevel Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecConfigEventLevel when
     * is_level              : 1U
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecConfigEventLevel Testcase ::\r\n", taskObj->taskId);
        uint32_t regVal = CSL_REG32_RD(drvHandle->clecRegs->CFG[0U].MRR);
        uint32_t bkIslevel = CSL_FEXT(regVal, CLEC_EVT_CFG_MRR_IS_LVL);
        retVal = CSL_clecConfigEventLevel(drvHandle->clecRegs, 0U, 1U);
        retVal = CSL_clecConfigEventLevel(drvHandle->clecRegs, 0U, bkIslevel);
    }
    /** 
     * Test CSL_clecClearEvent when
     * CSL_CLEC_EVTRegs    : NULL
     * evtNum              : INVALID
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecClearEvent Testcase ::\r\n", taskObj->taskId);
        retVal = CSL_clecClearEvent(NULL, UDMA_INTR_INVALID);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecClearEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecClearEvent when
     * CSL_CLEC_EVTRegs    : NULL
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecClearEvent Testcase ::\r\n", taskObj->taskId);
        retVal = CSL_clecClearEvent(NULL, 0U);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecClearEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecClearEvent when
     * evtNum              : INVALID
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecClearEvent Testcase ::\r\n", taskObj->taskId);
        retVal = CSL_clecClearEvent(drvHandle->clecRegs, UDMA_INTR_INVALID);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecClearEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecGetSecureClaimStatus when
     * CSL_CLEC_EVTRegs    : NULL
     * evtNum              : INVALID
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecGetSecureClaimStatus Testcase ::\r\n", taskObj->taskId);
        uint32_t secureClaim;
        retVal = CSL_clecGetSecureClaimStatus(NULL, UDMA_INTR_INVALID, &secureClaim);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecGetSecureClaimStatus Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecGetSecureClaimStatus when
     * CSL_CLEC_EVTRegs    : NULL
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecGetSecureClaimStatus Testcase ::\r\n", taskObj->taskId);
        uint32_t secureClaim;
        retVal = CSL_clecGetSecureClaimStatus(NULL, 0U, &secureClaim);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecGetSecureClaimStatus Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecGetSecureClaimStatus when
     * evtNum              : CSL_CLEC_MAX_EVT_IN
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecGetSecureClaimStatus Testcase ::\r\n", taskObj->taskId);
        uint32_t secureClaim;
        retVal = CSL_clecGetSecureClaimStatus(drvHandle->clecRegs, CSL_CLEC_MAX_EVT_IN, &secureClaim);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecGetSecureClaimStatus Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecGetSecureClaimStatus when
     *  evtNum              : 0U
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecGetSecureClaimStatus Testcase ::\r\n", taskObj->taskId);
        uint32_t secureClaim;
        retVal = CSL_clecGetSecureClaimStatus(drvHandle->clecRegs, 0U, &secureClaim);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecGetSecureClaimStatus Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecSendEvent when
     * CSL_CLEC_EVTRegs    : NULL
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecSendEvent Testcase ::\r\n", taskObj->taskId);
        
        retVal = CSL_clecSendEvent(NULL, UDMA_INTR_INVALID);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecSendEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecSendEvent when
     * CSL_CLEC_EVTRegs    : NULL
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecSendEvent Testcase ::\r\n", taskObj->taskId);
        
        retVal = CSL_clecSendEvent(drvHandle->clecRegs, UDMA_INTR_INVALID);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecSendEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecSendEvent when
     * CSL_CLEC_EVTRegs    : NULL
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecSendEvent Testcase ::\r\n", taskObj->taskId);
        
        retVal = CSL_clecSendEvent(NULL, 0U);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecSendEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /** 
     * Test CSL_clecSendEvent when
     * CSL_CLEC_EVTRegs    : NULL
    */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                " |TEST INFO|:: Task:%d: CSL_clecSendEvent Testcase ::\r\n", taskObj->taskId);
        
        retVal = CSL_clecSendEvent(drvHandle->clecRegs, 0U);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                    " |TEST INFO|:: Task:%d: FAIL: CSL_clecSendEvent Testcase ::\r\n", taskObj->taskId);
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    retVal = UdmaTestDeInitClec(chHandle, eventHandle);

    return (retVal);
}


static int32_t UdmaTestInitClec(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle, Udma_EventHandle eventHandle, uint32_t elemCnt)
{
    int32_t           retVal  = UDMA_SOK;
    uint32_t          heapId  = UTILS_MEM_HEAP_ID_DDR;
    void             *ringMem = NULL;
    uint32_t          chType;
    Udma_ChPrms       chPrms;
    uint32_t          ringMemSize;
    uint32_t          ringMode;
    Udma_ChTxPrms     txChPrms;
    Udma_EventPrms    eventPrms;

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
        if(UDMA_SOK == retVal)
        {
            UdmaEventPrms_init(&eventPrms);
            eventPrms.eventType         = UDMA_EVENT_TYPE_ERR_OUT_OF_RANGE_FLOW;
            eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
            eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
            retVal = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        }
    }

    return retVal;
}

static int32_t UdmaTestDeInitClec(Udma_ChHandle chHandle, Udma_EventHandle eventHandle)
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
    /* Close Channel */
    if(UDMA_SOK == retVal)
    {
        retVal = Udma_eventUnRegister(eventHandle);
    }

    return retVal;
}

