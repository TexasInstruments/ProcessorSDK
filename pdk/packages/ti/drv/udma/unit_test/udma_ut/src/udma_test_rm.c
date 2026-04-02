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
 *  \file udma_test_rm.c
 *
 *  \brief File containing test cases for UDMA rm APIs.
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

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * Test Case Description: Verifies the function Udma_chOpen 
 * Test scenario 1: Check when valid args are passed for Udma_rmFreeBlkCopyUhcCh
 * Test scenario 2: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_UHC and 
 *                  chNum is equal to startBlkCopyUhcCh 
 */
int32_t UdmaRmFreeBlkCopyUhcChTest(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    Udma_ChHandle      chHandle;
    struct Udma_ChObj  chObj;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    Udma_DrvHandle     drvHandle;
    uint32_t           instID;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA RmFreeBlkCopyUhcCh Testcase ::\r\n"
              , taskObj->taskId);

    /* Test scenario 1: Check when valid args are passed for Udma_rmFreeBlkCopyUhcCh */
    chType       = UDMA_CH_TYPE_TR_BLK_COPY_UHC;
    UdmaChPrms_init(&chPrms, chType);
    chHandle     = &chObj;
    instID       = UDMA_INST_ID_MAIN_0;
    drvHandle    = &taskObj->testObj->drvObj[instID];
    retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK != retVal)
    {  
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_rmFreeBlkCopyUhcCh:: Pos:: "
                  " valid args check!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = Udma_chClose(chHandle);
    }

    /* Test scenario 2: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_UHC and 
     *                  chNum is equal to startBlkCopyUhcCh */
    if(UDMA_SOK == retVal)
    {
        chType       = UDMA_CH_TYPE_TR_BLK_COPY_UHC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startBlkCopyUhcCh;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyUhcCh:: Pos::"
                      " Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_UHC"
                      " and chNum is equal to startBlkCopyUhcCh!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            Udma_chClose(chHandle);
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the functions 
 * Test scenario 1: Check Udma_rmFreeTxHcCh when chType is UDMA_CH_TYPE_TX_HC and instType is UDMA_INST_TYPE_NORMAL
 * Test scenario 2: Check Udma_chFreeResource when chType is UDMA_CH_TYPE_RX_HC and instType is UDMA_INST_TYPE_NORMAL
 * Test scenario 3: Check Udma_rmAllocBlkCopyHcCh when chType is UDMA_CH_TYPE_TR_BLK_COPY_HC, 
 *                  chNum is equal to startBlkCopyHcCh
 * Test scenario 4: Check Udma_chAllocResource when chType is UDMA_CH_TYPE_TR_BLK_COPY, 
 *                  chNum is equal to startBlkCopyCh
 */
int32_t UdmaRmAllocFreeTest(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    Udma_ChHandle      chHandle;
    struct Udma_ChObj  chObj;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    Udma_DrvHandle     drvHandle;
    uint32_t           instID;
    struct Udma_ChObj  backUpChObj;
    
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA rm free and alloc Ch, HcCh Testcase ::\r\n"
              , taskObj->taskId);

    /* Test scenario 1: Check when chType is UDMA_CH_TYPE_TX_HC and instType is UDMA_INST_TYPE_NORMAL */
    chType           = UDMA_CH_TYPE_TX_HC;
    UdmaChPrms_init(&chPrms, chType);
    chHandle         = &chObj;
    instID           = UDMA_INST_ID_MAIN_0;
    drvHandle        = &taskObj->testObj->drvObj[instID];
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK != retVal)
    {  
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_rmFreeTxHcCh:: Pos:: "
                  " valid args check!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = Udma_chClose(chHandle);
    }

    /* Test scenario 2: Check when chType is UDMA_CH_TYPE_RX_HC and instType is UDMA_INST_TYPE_NORMAL */
    if(UDMA_SOK == retVal)
    {
        chType                 = UDMA_CH_TYPE_RX_HC;
        drvHandle              = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum       = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal                 = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            retVal = Udma_chClose(chHandle);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: chFreeResource:: Pos:: "
                          " Check when chType is UDMA_CH_TYPE_RX_HC and instType is UDMA_INST_TYPE_NORMAL!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
        } 
    } 

    /* Test scenario 3: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_HC, 
     *                  chNum is equal to startBlkCopyHcCh */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_TR_BLK_COPY_HC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj  = chObj;
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startBlkCopyHcCh;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyHcCh:: Neg:: Check when"
                      " chType is UDMA_CH_TYPE_TR_BLK_COPY_HC, chNum is equal to startBlkCopyHcCh!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            Udma_chClose(chHandle);
        }
        chObj  = backUpChObj;
    }

     /* Test scenario 4: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY, 
     *                  chNum is equal to startBlkCopyCh */
    if(UDMA_SOK == retVal)
    {
        chType       = UDMA_CH_TYPE_TR_BLK_COPY;
        UdmaChPrms_init(&chPrms, chType);
        chHandle     = &chObj;
        backUpChObj  = chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startBlkCopyCh;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {  
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_chAllocResource:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TR_BLK_COPY"
                      " and chNum is equal to startBlkCopyCh;!!\n");
            retVal = UDMA_EFAIL;     
        }
        else
        {
            Udma_chClose(chHandle);
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_chOpen 
 * Test scenario 1: Check Udma_rmFreeTxUhcCh when valid args are passed for Udma_rmFreeTxUhcCh
 * Test scenario 2: Check Udma_rmAllocTxUhcCh when chType is UDMA_CH_TYPE_TX_UHC and  
 *                  chNum is equal to startTxUhcCh
 * Test scenario 3: Check Udma_rmAllocTxCh when chType is UDMA_CH_TYPE_TX,
 *                  chNum is equal to startTxCh
 */
int32_t UdmaRmFreeAllocTxChUhcHcChTest(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    Udma_ChHandle      chHandle;
    struct Udma_ChObj  chObj;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    Udma_DrvHandle     drvHandle;
    uint32_t           instID;
    struct Udma_ChObj  backUpChObj;
    struct Udma_DrvObj backUpDrvObj;
    
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA rm Free and alloc Tx Uhc ch, Ch Testcase ::\r\n"
              , taskObj->taskId);

    /* Test scenario 1: Check when valid args are passed for Udma_rmFreeTxUhcCh */
    chType           = UDMA_CH_TYPE_TX_UHC;
    UdmaChPrms_init(&chPrms, chType);
    chHandle         = &chObj;
    instID           = UDMA_INST_ID_MAIN_0;
    drvHandle        = &taskObj->testObj->drvObj[instID];
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK != retVal)
    {  
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_rmFreeTxUhcCh:: Pos:: "
                  " valid args check!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = Udma_chClose(chHandle);
    }

    /* Test scenario 2: Check when chType is UDMA_CH_TYPE_TX_UHC and  
     *                  chNum is equal to startTxUhcCh */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_TX_UHC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
        chPrms.chNum     = drvHandle->initPrms.rmInitPrms.startTxUhcCh;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxUhcCh:: Pos::"
                      " Check when chType is UDMA_CH_TYPE_TX_UHC"
                      " and chNum is equal to startTxUhcCh!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            Udma_chClose(chHandle);
        }
    }

    /* Test scenario 3: Check when chType is UDMA_CH_TYPE_TX and chNum is equal to startTxCh */
    if(UDMA_SOK == retVal)
    {
        chHandle         = &chObj;
        instID           = UDMA_INST_ID_MAIN_0;
        chType           = UDMA_CH_TYPE_TX;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj      = chObj;
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
        chPrms.chNum     = drvHandle->initPrms.rmInitPrms.startTxCh;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxCh:: Pos::"
                      " Check when chType is UDMA_CH_TYPE_TX_UHC"
                      " and chNum is equal to startTxUhcCh!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            Udma_chClose(chHandle);
        }
        chObj  = backUpChObj;
    }

    /* Test scenario 4: Check when chType is UDMA_CH_TYPE_TX_HC, 
     *                  chNum is equal to startTxHcCh */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_TX_HC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        backUpDrvObj     = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_TX;
        chPrms.chNum     = drvHandle->initPrms.rmInitPrms.startTxHcCh;
        drvHandle->initPrms.rmInitPrms.numTxHcCh = 1U;
        drvHandle->txHcChFlag[0]                 = 1U;
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxHcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TX_HC"
                      " and chNum is equal to startTxHcCh!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            Udma_chClose(chHandle);
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_chOpen
 * Test scenario 1: Check Udma_rmFreeRxUhcCh when chType is UDMA_CH_TYPE_RX_UHC and 
 *                  instType is UDMA_INST_TYPE_NORMAL
 * Test scenario 2: Check Udma_rmAllocRxUhcCh when chType is UDMA_CH_TYPE_RX_UHC and
 *                  chNum is equal to startRxUhcCh
 * Test scenario 3: Check Udma_rmAllocRxCh when chType is UDMA_CH_TYPE_RX, 
 *                  chNum is equal to startRxCh
 * Test scenario 4: Check Udma_rmAllocRxHcCh when chType is UDMA_CH_TYPE_RX_HC, 
 *                  chNum is equal to startRxHcCh
 */
int32_t UdmaRmFreeAllocRxChUhcHcChTest(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    Udma_ChHandle      chHandle;
    struct Udma_ChObj  chObj;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    Udma_DrvHandle     drvHandle;
    uint32_t           instID; 
    struct Udma_ChObj  backUpChObj;
    struct Udma_DrvObj backUpDrvObj;
    
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Rm Alloc and free Rx Ch, Uhc and Hc Ch Testcase ::\r\n"
              , taskObj->taskId);

    /* Test scenario 1: Check when chType is UDMA_CH_TYPE_RX_UHC and 
    *                   instType is UDMA_INST_TYPE_NORMAL */
    chType             = UDMA_CH_TYPE_RX_UHC;
    UdmaChPrms_init(&chPrms, chType);
    chHandle         = &chObj;
    instID           = UDMA_INST_ID_MAIN_0;
    drvHandle        = &taskObj->testObj->drvObj[instID];
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK != retVal)
    {  
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_rmFreeRxUhcCh:: Pos:: Check when chType"
                  " is UDMA_CH_TYPE_RX_UHC and instType is UDMA_INST_TYPE_NORMAL!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = Udma_chClose(chHandle);
    }

    /* Test scenario 2: Check when chType is UDMA_CH_TYPE_RX_UHC and
     *                  chNum is equal to startRxUhcCh */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX_UHC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        chPrms.chNum     = drvHandle->initPrms.rmInitPrms.startRxUhcCh;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxUhcCh:: Pos::"
                      " Check when chType is UDMA_CH_TYPE_RX_UHC and"
                      " chNum is equal to startRxUhcCh!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = Udma_chClose(chHandle);
        }
    }

    /* Test scenario 3: Check when chType is UDMA_CH_TYPE_RX, 
     *                  chNum is equal to startRxCh */
    if(UDMA_SOK == retVal)
    {
        chHandle         = &chObj;
        instID           = UDMA_INST_ID_MAIN_0;
        chType           = UDMA_CH_TYPE_RX;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj      = chObj;
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startRxCh;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_RX"
                      " and chNum is equal to startRxCh!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            Udma_chClose(chHandle);
        }
    }

    /* Test scenario 4: Check when chType is UDMA_CH_TYPE_RX_HC, 
     *                  chNum is equal to startRxHcCh */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX_HC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        backUpDrvObj     = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj      = chObj;
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_RX;
        chPrms.chNum     = drvHandle->initPrms.rmInitPrms.startRxHcCh;
        drvHandle->initPrms.rmInitPrms.numRxHcCh = 1U;
        drvHandle->rxHcChFlag[0] = 1U;
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxHcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_RX_HC"
                      " and chNum is equal to startRxHcCh!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            Udma_chClose(chHandle);
        }
        chObj = backUpChObj;
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function UdmaRmInitPrms_init when
 * Test scenario 1: NULL check for rmInitPrms
 * Test scenario 2: Invalid args check for instId 
 */
int32_t UdmaRmInitPrmsinitTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t         retVal = UDMA_SOK;
    uint32_t        instId;
    Udma_RmInitPrms rmInitPrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA RmInitPrmsinit negative Testcase ::\r\n"
              , taskObj->taskId);

    /*Test scenario 1: NULL check for rmInitPrms*/
    instId = UDMA_TEST_INST_ID_MAIN_0;
    retVal = UdmaRmInitPrms_init(instId, NULL);
    if(UDMA_SOK == retVal)
    {  
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: RmInitPrms_init:: Neg:: "
                  " valid args check!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = UDMA_SOK;
    }

    if(UDMA_SOK == retVal)
    {  
        /*Test scenario 2: Invalid args check for instId*/
        instId = UDMA_INST_ID_MAX + 1U;
        retVal = UdmaRmInitPrms_init(instId, &rmInitPrms);
        if(UDMA_SOK == retVal)
        {  
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: RmInitPrms_init:: Neg:: "
                      " Invalid args check for instId!!\n");
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
 * Test Case Description: Verifies the function Udma_rmAllocBlkCopyHcCh when 
 * Test scenario 1: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_HC,
 *                  chNum is less than startBlkCopyHcCh 
 * Test scenario 2: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_HC and 
 *                  chNum is UDMA_DMA_CH_INVALID
 * Test scenario 3: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_UHC, 
 *                  chNum is less than startBlkCopyUhcCh
 * Test scenario 4: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_UHC and 
 *                  chNum is UDMA_DMA_CH_INVALID
 * Test scenario 5: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY, 
 *                  chNum is less than startBlkCopyCh
 * Test scenario 6: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY and 
 *                  chNum is UDMA_DMA_CH_INVALID
 * Test scenario 7: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY and 
 *                  numBlkCopyCh is 0U
 * Test scenario 8: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_UHC
 *                  and resource not available to allocate
 */
int32_t UdmaRmAllocBlkCopyChTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    Udma_ChHandle      chHandle;
    struct Udma_ChObj  chObj;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    Udma_DrvHandle     drvHandle;
    uint32_t           instID;
    struct Udma_ChObj  backUpChObj;
    struct Udma_DrvObj backUpDrvObj;
    uint32_t           i=0U;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA RmFreeBlkCopy Ch, HcCh and UhcCh Testcase ::\r\n"
              , taskObj->taskId);

    /* Test scenario 1: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_HC, 
     *                  chNum less than startBlkCopyHcCh */
    chHandle     = &chObj;
    instID       = UDMA_TEST_INST_ID_MAIN_0;
    chType       = UDMA_CH_TYPE_TR_BLK_COPY_HC;
    drvHandle    = &taskObj->testObj->drvObj[instID];
    UdmaChPrms_init(&chPrms, chType);
    backUpChObj  = chObj;
    chPrms.chNum = drvHandle->initPrms.rmInitPrms.startBlkCopyHcCh - 1U;
    retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                    " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyHcCh:: Neg::"
                    " Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_HC"
                    " and chNum is less than startBlkCopyHcCh!!\n");
        retVal = UDMA_EFAIL;
        Udma_chClose(chHandle);
    }
    else
    {
        retVal = UDMA_SOK;
        chObj  = backUpChObj;
    }
    
    /* Test scenario 2: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_HC and 
     *                   chNum is UDMA_DMA_CH_INVALID */
    if(UDMA_SOK == retVal)
    {
        chType       = UDMA_CH_TYPE_TR_BLK_COPY_HC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj  = chObj;
        chPrms.chNum = UDMA_DMA_CH_INVALID;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyHcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_HC"
                      " and chNum is UDMA_DMA_CH_INVALID!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
    }

    /* Test scenario 3: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_UHC, 
     *                   chNum is less than startBlkCopyUhcCh */
    if(UDMA_SOK == retVal)
    {
        chType       = UDMA_CH_TYPE_TR_BLK_COPY_UHC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj  = chObj;
        drvHandle->initPrms.rmInitPrms.startBlkCopyUhcCh = 1U;
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startBlkCopyUhcCh - 1U;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyUhcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_HC"
                      " and chNum is less than startBlkCopyUhcCh!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 4: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_UHC and 
     *                   chNum is UDMA_DMA_CH_INVALID */
    if(UDMA_SOK == retVal)
    {
        chType       = UDMA_CH_TYPE_TR_BLK_COPY_UHC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj  = chObj;
        chPrms.chNum = UDMA_DMA_CH_INVALID;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyUhcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_HC"
                      " and chNum is UDMA_DMA_CH_INVALID!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
    }

    /* Test scenario 5: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY, 
     *                  chNum is less than startBlkCopyCh */
    if(UDMA_SOK == retVal)
    {
        chType       = UDMA_CH_TYPE_TR_BLK_COPY;
        UdmaChPrms_init(&chPrms, chType);
        chHandle     = &chObj;
        backUpChObj  = chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        if(UDMA_SOK == retVal)
        { 
            chPrms.chNum = drvHandle->initPrms.rmInitPrms.startBlkCopyCh - 1U;
            retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
            if(UDMA_SOK == retVal)
            {  
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: Udma_chAllocResource:: Neg::"
                          " Check when chType is UDMA_CH_TYPE_TR_BLK_COPY"
                          " and chNum is less than startBlkCopyCh;!!\n");
                retVal = UDMA_EFAIL;
                Udma_chClose(chHandle);
            }
            else
            {
                retVal = UDMA_SOK;
                chObj  = backUpChObj;
            }
        }
    }

    /* Test scenario 6: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY and 
     *                   chNum is UDMA_DMA_CH_INVALID */
    if(UDMA_SOK == retVal)
    {
        chType       = UDMA_CH_TYPE_TR_BLK_COPY;
        UdmaChPrms_init(&chPrms, chType);
        chHandle     = &chObj;
        backUpChObj  = chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        chPrms.chNum = UDMA_DMA_CH_INVALID;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {  
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_chAllocResource:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TR_BLK_COPY"
                      " and chNum is UDMA_DMA_CH_INVALID!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
    }

    /* Test scenario 7: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY and 
     *                  numBlkCopyCh is 0U */
    if(UDMA_SOK == retVal)
    {
        chType       = UDMA_CH_TYPE_TR_BLK_COPY;
        UdmaChPrms_init(&chPrms, chType);
        chHandle     = &chObj;
        backUpChObj  = chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        drvHandle->initPrms.rmInitPrms.numBlkCopyCh = 0U;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {  
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TR_BLK_COPY"
                      " and numBlkCopyCh is 0U!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 8: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_UHC, 
     *                  and resource not available to allocate */
    if(UDMA_SOK == retVal)
    {
        chType       = UDMA_CH_TYPE_TR_BLK_COPY_UHC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        drvHandle->initPrms.rmInitPrms.numBlkCopyUhcCh = 1U;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numBlkCopyUhcCh; i++)
        {
            drvHandle->blkCopyUhcChFlag[i]  = 0U; //make resource not available
        }
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyUhcCh:: Pos::"
                      " Check when chType is UDMA_CH_TYPE_TR_BLK_COPY_HC,"
                      " and resource not available to allocate!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_rmAllocTxCh when 
 * Test scenario 1: Check when chType is UDMA_CH_TYPE_TX, 
 *                  chNum is less than startTxCh
 * Test scenario 2: Check when chType is UDMA_CH_TYPE_TX and 
 *                  chNum is UDMA_DMA_CH_INVALID
 * Test scenario 3: Check when chType is UDMA_CH_TYPE_TX_HC, 
 *                  chNum is less than startTxHcCh
 * Test scenario 4: Check when chType is UDMA_CH_TYPE_TX_HC and 
 *                  chNum is UDMA_DMA_CH_INVALID
 * Test scenario 5: Check when chType is UDMA_CH_TYPE_TX_UHC, 
 *                  chNum is less than startTxUhcCh
 * Test scenario 6: Check when chType is UDMA_CH_TYPE_TX_UHC and 
 *                  chNum is UDMA_DMA_CH_INVALID
 * Test scenario 7: Check when chType is UDMA_CH_TYPE_TX
 *                  and resource not available to allocate
 * Test scenario 8: Check when chType is UDMA_CH_TYPE_TX and numTxCh is 0U
 * Test scenario 9: Check when chType is UDMA_CH_TYPE_TX_UHC, 
 *                  and resource not available to allocate
 * Test scenario 10: Check when chType is UDMA_CH_TYPE_TX_UHC, 
 *                  chNum is equal to startTxUhcCh and txUhcChFlag[0] is 0U
 */
int32_t UdmaRmAllocTxChTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    Udma_ChHandle      chHandle;
    struct Udma_ChObj  chObj;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    Udma_DrvHandle     drvHandle;
    uint32_t           instID;
    struct Udma_ChObj  backUpChObj;
    struct Udma_DrvObj backUpDrvObj;
    uint32_t           i=0U;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA RmAllocTx Ch, HcCh and UhcCh Testcase ::\r\n"
              , taskObj->taskId);

    /* Test scenario 1: Check when chType is UDMA_CH_TYPE_TX,
     *                  chNum is less than startTxCh */
    chHandle         = &chObj;
    instID           = UDMA_INST_ID_MAIN_0;
    chType           = UDMA_CH_TYPE_TX;
    drvHandle        = &taskObj->testObj->drvObj[instID];
    UdmaChPrms_init(&chPrms, chType);
    backUpChObj      = chObj;
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
    chPrms.chNum = drvHandle->initPrms.rmInitPrms.startTxCh - 1U;
    retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxCh:: Neg::"
                  " Check when chType is UDMA_CH_TYPE_TX"
                  " and chNum is less than startTxCh!!\n");
        retVal = UDMA_EFAIL;
        Udma_chClose(chHandle);
    }
    else
    {
        retVal = UDMA_SOK;
        chObj  = backUpChObj;
    }

    /* Test scenario 2: Check when chType is UDMA_CH_TYPE_TX and 
     *                  chNum is UDMA_DMA_CH_INVALID */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_TX;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj      = chObj;
        chPrms.chNum     = UDMA_DMA_CH_INVALID;
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TX"
                      " and chNum is UDMA_DMA_CH_INVALID!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
    }

    /* Test scenario 3: Check when chType is UDMA_CH_TYPE_TX_HC, 
     *                  chNum is less than startTxHcCh */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_TX_HC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        backUpDrvObj     = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_TX;
        drvHandle->initPrms.rmInitPrms.numTxHcCh = 1U;
        drvHandle->initPrms.rmInitPrms.startTxHcCh = 1U;
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startTxHcCh - 1U;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxHcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TX_HC"
                      " and chNum is less than startTxHcCh!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 4: Check when chType is UDMA_CH_TYPE_TX_HC and 
     *                  chNum is UDMA_DMA_CH_INVALID */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_TX_HC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj      = chObj;
        chPrms.chNum     = UDMA_DMA_CH_INVALID;
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxHcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TX_HC"
                      " and chNum is UDMA_DMA_CH_INVALID!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
    }

    /* Test scenario 5: Check when chType is UDMA_CH_TYPE_TX_UHC, 
     *                  chNum is less than startTxUhcCh */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_TX_UHC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        backUpDrvObj     = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj      = chObj;
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
        drvHandle->initPrms.rmInitPrms.startTxUhcCh = 1U;
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startTxUhcCh - 1U;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxUhcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TX_UHC"
                      " and chNum is less than startTxUhcCh!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 6: Check when chType is UDMA_CH_TYPE_TX_UHC and 
     *                  chNum is UDMA_DMA_CH_INVALID */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_TX_UHC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj      = chObj;
        chPrms.chNum     = UDMA_DMA_CH_INVALID;
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxUhcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TX_UHC"
                      " and chNum is UDMA_DMA_CH_INVALID!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
    }

    /* Test scenario 7: Check when chType is UDMA_CH_TYPE_TX, 
     *                  and resource not available to allocate */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_TX;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        backUpDrvObj     = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj                            = chObj;
        chPrms.peerChNum                       = UDMA_PSIL_CH_MCU_CPSW0_TX;
        drvHandle->initPrms.rmInitPrms.numTxCh = 1U;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numTxCh; i++)
        {
            drvHandle->txChFlag[i] = 0U; //make resource not available
        }
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxUhcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TX,"
                      " and resource not available to allocate!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 8: Check when chType is UDMA_CH_TYPE_TX and numTxCh is 0U */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_TX;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        backUpDrvObj     = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj                            = chObj;
        chPrms.peerChNum                       = UDMA_PSIL_CH_MCU_CPSW0_TX;
        drvHandle->initPrms.rmInitPrms.numTxCh = 0U;
        retVal                                 = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxUhcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TX and numTxCh is 0U!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 9: Check when chType is UDMA_CH_TYPE_TX_UHC, 
     *                  and resource not available to allocate */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_TX_UHC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        backUpDrvObj     = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj                               = chObj;
        chPrms.peerChNum                          = UDMA_PSIL_CH_MCU_CPSW0_TX;
        drvHandle->initPrms.rmInitPrms.numTxUhcCh = 1U;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numTxUhcCh; i++)
        {
            drvHandle->txUhcChFlag[i] = 0U; //make resource not available
        }
        retVal                                    = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxUhcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TX_UHC"
                      " and resource not available to allocate!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 10: Check when chType is UDMA_CH_TYPE_TX_UHC, 
     *                  chNum is equal to startTxUhcCh and txUhcChFlag[0] is 0U */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_TX_UHC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        backUpDrvObj     = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj               = chObj;
        chPrms.peerChNum          = UDMA_PSIL_CH_MCU_CPSW0_TX;
        chPrms.chNum              = drvHandle->initPrms.rmInitPrms.startTxUhcCh;
        drvHandle->txUhcChFlag[0] = 0U;
        retVal                    = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxUhcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_TX_UHC,"
                      " chNum is equal to startTxUhcCh and txUhcChFlag[0] is 0U!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_rmAllocRxCh when 
 * Test scenario 1: Check when chType is UDMA_CH_TYPE_RX, 
 *                  chNum is less than startRxCh
 * Test scenario 2: Check when chType is UDMA_CH_TYPE_RX and 
 *                  chNum is UDMA_DMA_CH_INVALID
 * Test scenario 3: Check when chType is UDMA_CH_TYPE_RX_HC, 
 *                  chNum is less than startRxHcCh
 * Test scenario 4: Check when chType is UDMA_CH_TYPE_RX_HC and 
 *                  chNum is UDMA_DMA_CH_INVALID
 * Test scenario 5: Check when chType is UDMA_CH_TYPE_RX_UHC, 
 *                  chNum is less than startRxUhcCh
 * Test scenario 6: Check when chType is UDMA_CH_TYPE_RX_UHC and 
 *                  chNum is UDMA_DMA_CH_INVALID
 * Test scenario 7: Check when chType is UDMA_CH_TYPE_RX_HC, 
 *                  and resource not available to allocate
 * Test scenario 8: Check when chType is UDMA_CH_TYPE_RX_HC and numRxHcCh is 0U 
 * Test scenario 9: Check when chType is UDMA_CH_TYPE_RX_HC, 
 *                  chNum is equal to startRxHcCh and rxHcChFlag[0] is 0U
 */
int32_t UdmaRmAllocRxChTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    Udma_ChHandle      chHandle;
    struct Udma_ChObj  chObj;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    Udma_DrvHandle     drvHandle;
    uint32_t           instID;
    struct Udma_ChObj  backUpChObj;
    struct Udma_DrvObj backUpDrvObj;
    uint32_t           i=0U;
    
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA RmAllocRx CH, HcCh, UhcCh Testcase ::\r\n"
              , taskObj->taskId);

    /* Test scenario 1: Check when chType is UDMA_CH_TYPE_RX, 
     *                  chNum is less than startRxCh */
    chHandle         = &chObj;
    instID           = UDMA_INST_ID_MAIN_0;
    chType           = UDMA_CH_TYPE_RX;
    drvHandle        = &taskObj->testObj->drvObj[instID];
    UdmaChPrms_init(&chPrms, chType);
    backUpChObj      = chObj;
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
    chPrms.chNum = drvHandle->initPrms.rmInitPrms.startRxCh - 1U;
    retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxCh:: Neg::"
                  " Check when chType is UDMA_CH_TYPE_RX"
                  " and chNum is less than startRxCh!!\n");
        retVal = UDMA_EFAIL;
        Udma_chClose(chHandle);
    }
    else
    {
        retVal = UDMA_SOK;
        chObj  = backUpChObj;
    }

    /* Test scenario 2: Check when chType is UDMA_CH_TYPE_RX and 
     *                  chNum is UDMA_DMA_CH_INVALID */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj      = chObj;
        chPrms.chNum     = UDMA_DMA_CH_INVALID;
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_RX"
                      " and chNum is UDMA_DMA_CH_INVALID!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
    }

    /* Test scenario 3: Check when chType is UDMA_CH_TYPE_RX_HC, 
     *                  chNum is less than startRxHcCh */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX_HC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        backUpDrvObj     = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj      = chObj;
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_RX;
        drvHandle->initPrms.rmInitPrms.numRxHcCh = 1U;
        drvHandle->initPrms.rmInitPrms.startRxHcCh = 1U;
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startRxHcCh - 1U;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxHcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_RX_HC"
                      " and chNum is less than startRxHcCh!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 4: Check when chType is UDMA_CH_TYPE_RX_HC and 
     *                  chNum is UDMA_DMA_CH_INVALID */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX_HC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj      = chObj;
        chPrms.chNum     = UDMA_DMA_CH_INVALID;
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxHcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_RX_HC"
                      " and chNum is UDMA_DMA_CH_INVALID!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
    }

    /* Test scenario 5: Check when chType is UDMA_CH_TYPE_RX_UHC,
     *                  chNum is less than startRxUhcCh */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX_UHC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj      = chObj;
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        drvHandle->initPrms.rmInitPrms.startRxUhcCh = 1U;
        chPrms.chNum     = drvHandle->initPrms.rmInitPrms.startRxUhcCh - 1U;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxUhcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_RX_UHC"
                      " and chNum is less than startRxUhcCh!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
    }

    /* Test scenario 6: Check when chType is UDMA_CH_TYPE_RX_UHC and 
     *                  chNum is UDMA_DMA_CH_INVALID */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX_UHC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj      = chObj;
        chPrms.chNum     = UDMA_DMA_CH_INVALID;
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxUhcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_RX_UHC"
                      " and chNum is UDMA_DMA_CH_INVALID!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
    }

    /* Test scenario 7: Check when chType is UDMA_CH_TYPE_RX_HC, 
     *                  and resource not available to allocate */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX_HC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        backUpDrvObj     = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj                              = chObj;
        chPrms.peerChNum                         = UDMA_TEST_MAIN_PEER_CH_NUM_RX;
        drvHandle->initPrms.rmInitPrms.numRxHcCh = 1U;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numRxHcCh; i++)
        {
            drvHandle->rxHcChFlag[i] = 0U; //make resource not available
        }
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxHcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_RX_HC,"
                      " and resource not available to allocate!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 8: Check when chType is UDMA_CH_TYPE_RX_HC and numRxHcCh is 0U */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX_HC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        backUpDrvObj     = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj                              = chObj;
        chPrms.peerChNum                         = UDMA_TEST_MAIN_PEER_CH_NUM_RX;
        drvHandle->initPrms.rmInitPrms.numRxHcCh = 0U;
        retVal                                   = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxHcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_RX_HC and numRxHcCh is 0U!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 9: Check when chType is UDMA_CH_TYPE_RX_HC, 
     *                  chNum is equal to startRxHcCh and rxHcChFlag[0] is 0U */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX_HC;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        backUpDrvObj     = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        backUpChObj      = chObj;
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_RX;
        chPrms.chNum     = drvHandle->initPrms.rmInitPrms.startRxHcCh;
        drvHandle->initPrms.rmInitPrms.numRxHcCh = 1U;
        drvHandle->rxHcChFlag[0] = 0U;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxHcCh:: Neg::"
                      " Check when chType is UDMA_CH_TYPE_RX_HC,"
                      " chNum is equal to startRxHcCh and rxHcChFlag[0] is 0U!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the rmAlloc functions when
 * Test scenario 1: Check when numProxy is 0 for the function Udma_rmAllocProxy
 * Test scenario 2: Check when numFreeRing is 0 for the function Udma_rmAllocFreeRing
 * Test scenario 3: Check when ringMonNum is less than startRingMon for 
 *                  the function Udma_rmFreeRingMon
 * Test scenario 4: Check when instType is not UDMA_INST_TYPE_NORMAL for
 *                  the function Udma_rmAllocFreeRing
 * Test scenario 5: Check when preferredIrIntrNum is equal to and less than  
 *                  startIrIntr for the function Udma_rmAllocIrIntr
 * Test scenario 6: Check when preferredIrIntrNum is UDMA_INTR_INVALID for 
 *                  the function Udma_rmAllocIrIntr
 * Test scenario 7: Check when irIntrNum is UDMA_INTR_INVALID for 
 *                  the function Udma_rmTranslateIrOutput
 */
int32_t UdmaRmAllocTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    uint32_t           instID;
    uint32_t           preferredIrIntrNum;
    Udma_ChHandle      chHandle;
    struct Udma_ChObj  backUpChObj;
    struct Udma_ChObj  chObj;
    uint16_t           ringMonNum;
    uint32_t           irIntrNum;
    struct Udma_DrvObj backUpDrvObj;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA rmAlloc related negative testcase ::\r\n"
              , taskObj->taskId);

    /* Test scenario 1: Check when numProxy is 0 */ 
    chHandle                                             = &chObj;
    backUpChObj                                          = chObj;
    instID                                               = UDMA_TEST_INST_ID_MAIN_0;
    backUpDrvObj                                         = taskObj->testObj->drvObj[instID];
    chHandle->drvHandle                                  = &taskObj->testObj->drvObj[instID];
    chHandle->drvHandle->initPrms.rmInitPrms.numProxy    = 0U;
    retVal                                               = Udma_rmAllocProxy(chHandle->drvHandle);
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocProxy:: Neg::"
                  " Check when numFreeRing is 0!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        /* Test scenario 2: Check when numFreeRing is 0 */ 
        retVal                                               = UDMA_SOK;
        chHandle->drvHandle->initPrms.rmInitPrms.numFreeRing = 0U;
        retVal                                               = Udma_rmAllocFreeRing(chHandle->drvHandle);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                    " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocFreeRing:: Neg::"
                    " Check when numProxy is 0!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            /* Test scenario 3: Check when ringMonNum is less than startRingMon */ 
            retVal     = UDMA_SOK;  
            ringMonNum = chHandle->drvHandle->initPrms.rmInitPrms.startRingMon - 1U;
            Udma_rmFreeRingMon(ringMonNum, chHandle->drvHandle);
            chObj      = backUpChObj;
        }
    }

    /* Test scenario 4: Check when instType is not UDMA_INST_TYPE_NORMAL */ 
    if(UDMA_SOK == retVal)
    {
        instID              = UDMA_TEST_INST_ID_BCDMA_0;
        chHandle->drvHandle = &taskObj->testObj->drvObj[instID];
        retVal              = Udma_rmAllocFreeRing(chHandle->drvHandle);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocFreeRing:: Neg::"
                      " Check when instType is not UDMA_INST_TYPE_NORMAL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 5: Check when preferredIrIntrNum is equal to and 
     *                  less than startIrIntr */ 
    instID              = UDMA_TEST_INST_ID_MAIN_0;
    chHandle->drvHandle = &taskObj->testObj->drvObj[instID];
    backUpChObj         = chObj;
    preferredIrIntrNum  = chHandle->drvHandle->initPrms.rmInitPrms.startIrIntr;
    chHandle->drvHandle->irIntrFlag[0] = 1;
    retVal              = Udma_rmAllocIrIntr(preferredIrIntrNum, chHandle->drvHandle);
    if(UDMA_INTR_INVALID != retVal)
    {
        preferredIrIntrNum = chHandle->drvHandle->initPrms.rmInitPrms.startIrIntr - 1U;
        retVal             = Udma_rmAllocIrIntr(preferredIrIntrNum, chHandle->drvHandle);
        if(UDMA_INTR_INVALID != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocIrIntr:: Neg::"
                      " Check when preferredIrIntrNum is less than startIrIntr!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
            chObj  = backUpChObj;
        }
    }

    /* Test scenario 6: Check when preferredIrIntrNum is UDMA_INTR_INVALID */
    instID              = UDMA_TEST_INST_ID_MAIN_0;
    chHandle->drvHandle = &taskObj->testObj->drvObj[instID];
    backUpChObj         = chObj;
    preferredIrIntrNum  = UDMA_INTR_INVALID;
    retVal              = Udma_rmAllocIrIntr(preferredIrIntrNum, chHandle->drvHandle);
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocIrIntr:: Neg::"
                  " Check when preferredIrIntrNum is UDMA_INTR_INVALID!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        /* Test scenario 7: Check when irIntrNum is UDMA_INTR_INVALID */
        retVal    = UDMA_SOK;
        {
            irIntrNum = preferredIrIntrNum;
        #if defined(BUILD_C66X)
            chHandle->drvHandle->initPrms.rmInitPrms.startIrIntr       = 0U;
            chHandle->drvHandle->initPrms.rmInitPrms.startC66xCoreIntr = 0U;
        #else
            chHandle->drvHandle->devIdIr = 0U;
        #endif
            retVal = Udma_rmTranslateIrOutput(chHandle->drvHandle, irIntrNum);
            if(UDMA_INTR_INVALID != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: Udma_rmTranslateIrOutput:: Neg::"
                          " Check when irIntrNum is UDMA_INTR_INVALID!!\n");
                retVal = UDMA_EFAIL;
            }
            else 
            {
                retVal = UDMA_SOK;
                chObj  = backUpChObj;
            }
        }
    }
    taskObj->testObj->drvObj[instID] = backUpDrvObj;

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_rmTranslateCoreIntrInput when
 * Test scenario 1: Check when valid coreIntrNum is not passed
 */
int32_t UdmaRmTranslateCoreIntrInputTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t        retVal = UDMA_SOK;
    Udma_DrvHandle drvHandle;
    uint32_t       coreIntrNum;
    uint32_t       instID;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Rm Translate Core Intr Input Negative Testcase ::\r\n"
              , taskObj->taskId);

    /* Test scenario 1: Check when valid coreIntrNum is not passed */
    instID      = UDMA_TEST_INST_ID_MAIN_0;
    drvHandle   = &taskObj->testObj->drvObj[instID];
    coreIntrNum = UDMA_INTR_INVALID;
    retVal      = Udma_rmTranslateCoreIntrInput(drvHandle, coreIntrNum);
    if(UDMA_INTR_INVALID != retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_rmTranslateCoreIntrInput::" 
                  " Neg:: Check when valid coreIntrNum is not passed!!\n");
        retVal = UDMA_EFAIL;
    }
    else 
    {
        retVal = UDMA_SOK;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the function Udma_rmSetSharedResRmInitPrms
 * Test scenario 1: Check when sumInstShare is greater than numUnresvRes
 * Test scenario 2: Check when instId is invalid
 * Test scenario 3: Check when minReq is 0U and numInst is greater than UDMA_RM_SHARED_RES_MAX_INST 
 * Test scenario 4: Check Udma_rmGetSharedResPrms when resId is Invalid
 * Test scenario 5: Check Udma_rmGetLocalBoardCfgResp when resId is Invalid
 */
int32_t UdmaRmSetSharedResRmInitPrmsTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal = UDMA_SOK;
    uint32_t             instId;
    int32_t              i;
    Udma_RmInitPrms      rmInitPrms;
    instId               = UDMA_TEST_INST_ID_MAIN_0;
    Udma_RmSharedResPrms *rmSharedResPrms;
    Udma_RmSharedResPrms backUpRmSharedResPrms;

    uint32_t shareRes[UDMA_TEST_NUM_RES_OVERWRITE] = {UDMA_RM_RES_ID_GLOBAL_EVENT, 
                                                        UDMA_RM_RES_ID_VINTR,
#if (UDMA_SOC_CFG_INTR_ROUTER_PRESENT == 1)
                                                        UDMA_RM_RES_ID_IR_INTR
#endif
                                                        };

    for(i = 0 ; i < UDMA_TEST_NUM_RES_OVERWRITE ; i++)
    {
        rmSharedResPrms = Udma_rmGetSharedResPrms(shareRes[i]);

        if(NULL_PTR != rmSharedResPrms)
        {
            rmSharedResPrms->startResrvCnt = 0U;
            rmSharedResPrms->endResrvCnt = 0U;
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU1_0)
            rmSharedResPrms->instShare[UDMA_INST_ID_MAIN_0] = UDMA_RM_SHARED_RES_CNT_MIN;
            rmSharedResPrms->instShare[UDMA_INST_ID_MCU_0] = UDMA_RM_SHARED_RES_CNT_REST;
#else
            rmSharedResPrms->instShare[UDMA_INST_ID_MAIN_0] = UDMA_RM_SHARED_RES_CNT_REST;
            rmSharedResPrms->instShare[UDMA_INST_ID_MCU_0] = UDMA_RM_SHARED_RES_CNT_MIN;
#endif
#endif
        }
    }
    
    backUpRmSharedResPrms = *rmSharedResPrms;
    /* Test scenario 1: Check when sumInstShare is greater than numUnresvRes */
    rmSharedResPrms->instShare[instId] = UDMA_RM_SHARED_RES_CNT_REST + 1U;
    retVal                             = UdmaRmInitPrms_init(instId, &rmInitPrms);
    if(UDMA_SOK == retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_rmSetSharedResRmInitPrms::" 
                  " Neg:: Check when sumInstShare is greater than numUnresvRes!!\n");
        retVal = UDMA_EFAIL;
    }
    else 
    {
        retVal = UDMA_SOK;
    }

    /* Test scenario 2: Check when instId is invalid */
    if(UDMA_SOK == retVal)
    {  
        instId = UDMA_CORE_ID_INVALID;
        retVal = UdmaRmInitPrms_init(instId, &rmInitPrms);
        if(UDMA_SOK == retVal)
        {  
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: RmInitPrms_init:: Neg::"
                      " Check when instId is invalid!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 3: Check when minReq is 0U and numInst is greater than UDMA_RM_SHARED_RES_MAX_INST */
    if(UDMA_SOK == retVal)
    {  
        instId = UDMA_TEST_INST_ID_MAIN_0;
        rmSharedResPrms->numInst = UDMA_RM_SHARED_RES_MAX_INST + 1U;
        rmSharedResPrms->minReq  = 0U;
        retVal = UdmaRmInitPrms_init(instId, &rmInitPrms);
        if(UDMA_SOK == retVal)
        {  
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: RmInitPrms_init:: Neg::"
                      " Check when minReq is 0U and numInst is greater than UDMA_RM_SHARED_RES_MAX_INST!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    *rmSharedResPrms = backUpRmSharedResPrms;

    /* Test scenario 4: Check Udma_rmGetSharedResPrms when resId is Invalid*/
    if(UDMA_SOK == retVal)
    {  
        rmSharedResPrms = Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_INVALID);
        if(NULL != rmSharedResPrms)
        {  
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmGetSharedResPrms:: Neg::"
                      " Check when resId is Invalid!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    #if defined (BUILD_C7X) && (UDMA_LOCAL_C7X_DRU_PRESENT == 1)
    /* Test scenario 5: Check Udma_rmGetLocalBoardCfgResp when resId is Invalid*/
    if(UDMA_SOK == retVal)
    { 
        const Udma_RmDefBoardCfgResp *resp = Udma_rmGetLocalBoardCfgResp(UDMA_RM_RES_ID_INVALID);
        if(NULL != resp)
        {  
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmGetLocalBoardCfgResp:: Neg::"
                      " Check when resId is Invalid!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    #endif

    return retVal;
}

/*
 * Test Case Description: Verifies the functions in Udma_rm.c
 * Test scenario 1: Check when minimum required number of resources are reserved for instances 
 * Test scenario 2: Check Udma_rmAllocBlkCopyCh when chNum is equal to startBlkCopyCh
 *                  and resource is not available
 * Test scenario 3: Check Udma_rmAllocBlkCopyHcCh when numBlkCopyHcCh is zero
 * Test scenario 4: Check Udma_rmAllocBlkCopyHcCh when numBlkCopyHcCh is greater than zero,
 *                  chNum is equal to startBlkCopyHcCh and resource is not available
 * Test scenario 5: Check Udma_rmAllocBlkCopyUhcCh when chNum is equal to startBlkCopyUhcCh
 *                  and resource is not available
 * Test scenario 6: Check Udma_rmAllocTxCh when chNum is equal to startTxCh and 
 *                  resource is not available
 * Test scenario 7: Check when Udma_rmAllocRxCh chNum is equal to startRxCh and 
 *                  resource is not available
 * Test scenario 8: Check when Udma_rmAllocRxCh resource is not available
 * Test scenario 9: Check Udma_rmAllocRxCh when numRxCh is zero
 * Test scenario 10: Check Udma_rmAllocTxHcCh when chNum is equal to startTxHcCh 
 *                  and resource is not available
 * Test scenario 11: Check Udma_rmAllocTxHcCh when resource is not available
 * Test scenario 12: Check Udma_rmAllocTxHcCh when numTxHcCh is zero 
 * Test scenario 13: Check Udma_rmAllocRxUhcCh when chNum is equal to startRxUhcCh
 *                   and no resource is available
 * Test scenario 14: Check Udma_rmAllocRxUhcCh when resource is not available
 * Test scenario 15: Check Udma_rmAllocRxUhcCh when numRxUhcCh is zero 
 * Test scenario 16: Check Udma_rmAllocIrIntr when preferredIrIntrNum is equal to 
 *                   startIrIntr and no resource is available 
 * Test scenario 17: Check Udma_rmAllocRingMon when numRingMon is greater than zero 
 *                   and no resource is availble 
 * Test scenario 18: Check Udma_rmAllocBlkCopyHcCh when numBlkCopyHcCh is greater than zero
 *                  and resource is not available
 */
int32_t UdmaRmTestNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal = UDMA_SOK;
    Udma_ChHandle        chHandle;
    struct Udma_ChObj    chObj;
    Udma_ChPrms          chPrms;
    uint32_t             chType;
    Udma_DrvHandle       drvHandle;
    uint32_t             instID;
    uint32_t             i=0U;
    uint32_t             preferredIrIntrNum;
    struct Udma_DrvObj   backUpDrvObj;
    Udma_RmSharedResPrms *rmSharedResPrms;
    Udma_RmInitPrms      rmInitPrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA  Testcase ::\r\n"
              , taskObj->taskId);

    uint32_t shareRes[UDMA_TEST_NUM_RES_OVERWRITE] = {UDMA_RM_RES_ID_GLOBAL_EVENT, 
                                                        UDMA_RM_RES_ID_VINTR,
#if (UDMA_SOC_CFG_INTR_ROUTER_PRESENT == 1)
                                                        UDMA_RM_RES_ID_IR_INTR
#endif
                                                        };

    for(i = 0 ; i < UDMA_TEST_NUM_RES_OVERWRITE ; i++)
    {
        rmSharedResPrms = Udma_rmGetSharedResPrms(shareRes[i]);

        if(NULL_PTR != rmSharedResPrms)
        {
            rmSharedResPrms->startResrvCnt = 0U;
            rmSharedResPrms->endResrvCnt = 0U;
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU1_0)
            rmSharedResPrms->instShare[UDMA_INST_ID_MAIN_0] = UDMA_RM_SHARED_RES_CNT_MIN;
            rmSharedResPrms->instShare[UDMA_INST_ID_MCU_0] = UDMA_RM_SHARED_RES_CNT_MIN;
#else
            rmSharedResPrms->instShare[UDMA_INST_ID_MAIN_0] = UDMA_RM_SHARED_RES_CNT_MIN;
            rmSharedResPrms->instShare[UDMA_INST_ID_MCU_0] = UDMA_RM_SHARED_RES_CNT_MIN;
#endif
#endif
        }
    }

    /* Test scenario 1: Check when minimum required number of resources are reserved for instances */
    instID = UDMA_TEST_DEFAULT_UDMA_INST;
    retVal = UdmaRmInitPrms_init(instID, &rmInitPrms);
    if(UDMA_SOK != retVal)
    {
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_rmSetSharedResRmInitPrms::Pos:: Check when" 
                  " minimum required number of resources are reserved for instances!!\n");
        retVal = UDMA_EFAIL;
    }
    else 
    {
        retVal = UDMA_SOK;
    }

    /* Test scenario 2: Check Udma_rmAllocBlkCopyCh when chNum is equal to startBlkCopyCh
     *                  and resource is not available */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_TR_BLK_COPY;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startBlkCopyCh;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numBlkCopyCh; i++)
        {
            drvHandle->blkCopyChFlag[i] = 0U; //make resource not available
        }
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyCh:: Neg:: Check when"
                      " chNum is equal to startBlkCopyCh and resource is not available!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 3: Check Udma_rmAllocBlkCopyHcCh when numBlkCopyHcCh is zero */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_TR_BLK_COPY_HC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        drvHandle->initPrms.rmInitPrms.numBlkCopyHcCh = 0U;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyHcCh:: Neg::"
                      " Check when numBlkCopyHcCh is zero!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 4: Check Udma_rmAllocBlkCopyHcCh when numBlkCopyHcCh is greater than zero,
     *                  chNum is equal to startBlkCopyHcCh and resource is not available */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_TR_BLK_COPY_HC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startBlkCopyHcCh;
        drvHandle->initPrms.rmInitPrms.numBlkCopyHcCh = 1U;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numBlkCopyHcCh; i++)
        {
            drvHandle->blkCopyHcChFlag[i]  = 0U; //make resource not available
        }
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyHcCh:: Neg:: Check when numBlkCopyHcCh"
                      " is greater than zero, chNum is equal to startBlkCopyHcCh and resource is not available!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 5: Check Udma_rmAllocBlkCopyUhcCh when chNum is equal to startBlkCopyUhcCh
     *                  and resource is not available */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_TR_BLK_COPY_UHC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startBlkCopyUhcCh;
        drvHandle->initPrms.rmInitPrms.numBlkCopyUhcCh = 1U;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numBlkCopyUhcCh; i++)
        {
            drvHandle->blkCopyUhcChFlag[i]  = 0U; //make resource not available
        }
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyUhcCh:: Neg:: Check when"
                      " chNum is equal to startBlkCopyUhcCh and resource is not available!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 6: Check Udma_rmAllocTxCh when chNum is equal to startTxCh and 
     *                  resource is not available */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_TX;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_TX;
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startTxCh;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numTxCh; i++)
        {
            drvHandle->txChFlag[i]  = 0U; //make resource not available
        }
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxCh:: Neg:: Check when"
                      " chNum is equal to startTxCh and resource is not available!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 7: Check when Udma_rmAllocRxCh chNum is equal to startRxCh and 
     *                  resource is not available */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_RX;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_RX;
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startRxCh;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numRxCh; i++)
        {
            drvHandle->rxChFlag[i]  = 0U; //make resource not available
        }
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxCh:: Neg:: Check when"
                      " chNum is equal to startRxCh and resource is not available!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 8: Check when Udma_rmAllocRxCh resource is not available */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_RX;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_RX;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numRxCh; i++)
        {
            drvHandle->rxChFlag[i]  = 0U; //make resource not available
        }
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                        " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxCh:: Neg::"
                        " Check when resource is not available!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 9: Check Udma_rmAllocRxCh when numRxCh is zero */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_RX;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_RX;
        drvHandle->initPrms.rmInitPrms.numRxCh = 0U;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxCh:: Neg::"
                      " Check when numRxCh is zero!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 10: Check Udma_rmAllocTxHcCh when chNum is equal to startTxHcCh 
     *                  and resource is not available */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_TX_HC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_TX;
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startTxHcCh;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numTxHcCh; i++)
        {
            drvHandle->txHcChFlag[i]  = 0U; //make resource not available
        }
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxHcCh:: Neg:: Check when"
                      " chNum is equal to startTxHcCh and resource is not available!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 11: Check Udma_rmAllocTxHcCh when resource is not available */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_TX_HC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_TX;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numTxHcCh; i++)
        {
            drvHandle->txHcChFlag[i]  = 0U; //make resource not available
        }
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxHcCh:: Neg::"
                      " Check when resource is not available!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 12: Check Udma_rmAllocTxHcCh when numTxHcCh is zero */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_TX_HC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_TX;
        drvHandle->initPrms.rmInitPrms.numTxHcCh = 0U;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocTxHcCh:: Neg::"
                      " Check when numTxHcCh is zero!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 13: Check Udma_rmAllocRxUhcCh when chNum is equal to startRxUhcCh
     *                   and no resource is available */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_RX_UHC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_RX;
        chPrms.chNum = drvHandle->initPrms.rmInitPrms.startRxUhcCh;
        drvHandle->initPrms.rmInitPrms.numRxUhcCh = 1U;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numRxUhcCh; i++)
        {
            drvHandle->rxUhcChFlag[i]  = 0U; //make resource not available
        }
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxUhcCh:: Neg:: Check when"
                      " chNum is equal to startRxUhcCh and no resource is available!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 14: Check Udma_rmAllocRxUhcCh when resource is not available */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_RX_UHC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_RX;
        drvHandle->initPrms.rmInitPrms.numRxUhcCh = 1U;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numRxUhcCh; i++)
        {
            drvHandle->rxUhcChFlag[i]  = 0U; //make resource not available
        }
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxUhcCh:: Neg::"
                      " Check when resource is not available !!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 15: Check Udma_rmAllocRxUhcCh when numRxUhcCh is zero */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_RX_UHC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_TEST_MAIN_PEER_CH_NUM_RX;
        drvHandle->initPrms.rmInitPrms.numRxUhcCh = 0U;
        retVal       = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRxUhcCh:: Neg::"
                      " Check when numRxUhcCh is zero!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 16: Check Udma_rmAllocIrIntr when preferredIrIntrNum is equal to 
     *                   startIrIntr and no resource is available */
    if(UDMA_SOK == retVal)
    {
        instID              = UDMA_TEST_INST_ID_MAIN_0;
        chHandle->drvHandle = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        preferredIrIntrNum  = chHandle->drvHandle->initPrms.rmInitPrms.startIrIntr;
        chHandle->drvHandle->irIntrFlag[0] = 0U;
        retVal              = Udma_rmAllocIrIntr(preferredIrIntrNum, chHandle->drvHandle);
        if(UDMA_INTR_INVALID != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocIrIntr:: Neg:: Check when"
                      " preferredIrIntrNum is equal to startIrIntr and no resource is available!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 17: Check Udma_rmAllocRingMon when numRingMon is greater than zero 
     *                   and no resource is availble */
    if(UDMA_SOK == retVal)
    {
        instID              = UDMA_TEST_INST_ID_MAIN_0;
        chHandle->drvHandle = &taskObj->testObj->drvObj[instID];
        backUpDrvObj        = taskObj->testObj->drvObj[instID];
        chHandle->drvHandle->initPrms.rmInitPrms.numRingMon = 1U;
        chHandle->drvHandle->ringMonFlag[0] = 0U;
        retVal = Udma_rmAllocRingMon(chHandle->drvHandle);
        if(UDMA_RING_MON_INVALID != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocRingMon:: Neg:: Check when"
                      " numRingMon is greater than zero and no resource is availble!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    /* Test scenario 18: Check Udma_rmAllocBlkCopyHcCh when numBlkCopyHcCh is greater than zero
     *                  and resource is not available */
    if(UDMA_SOK == retVal)
    {
        chHandle     = &chObj;
        instID       = UDMA_TEST_INST_ID_MAIN_0;
        chType       = UDMA_CH_TYPE_TR_BLK_COPY_HC;
        drvHandle    = &taskObj->testObj->drvObj[instID];
        backUpDrvObj = taskObj->testObj->drvObj[instID];
        UdmaChPrms_init(&chPrms, chType);
        drvHandle->initPrms.rmInitPrms.numBlkCopyHcCh = 1U;
        for(i=0U; i<drvHandle->initPrms.rmInitPrms.numBlkCopyHcCh; i++)
        {
            drvHandle->blkCopyHcChFlag[i]  = 0U; //make resource not available
        }
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyHcCh:: Neg:: Check when"
                      " numBlkCopyHcCh is greater than zero and resource is not available!!\n");
            retVal = UDMA_EFAIL;
            Udma_chClose(chHandle);
        }
        else
        {
            retVal = UDMA_SOK;
        }
        taskObj->testObj->drvObj[instID] = backUpDrvObj;
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the functions in Udma_rm.c
 * Test scenario 1: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY and chNum is startBlkCopyCh 
 * Test scenario 2: Check when chType is UDMA_CH_TYPE_TX and chNum is startTxCh
 * Test scenario 3: Check when chType is UDMA_CH_TYPE_RX and chNum is startRxCh
 */ 
int32_t UdmaRmAllocTest(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    Udma_ChHandle      chHandle;
    struct Udma_ChObj  chObj;
    Udma_ChPrms        chPrms;
    uint32_t           chType;
    Udma_DrvHandle     drvHandle;
    uint32_t           instID;
    struct Udma_ChObj  backUpChObj;
    
    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA rm alloc Ch Testcase ::\r\n"
              , taskObj->taskId);

    /* Test scenario 1: Check when chType is UDMA_CH_TYPE_TR_BLK_COPY and chNum is startBlkCopyCh */
    chType           = UDMA_CH_TYPE_TR_BLK_COPY;
    UdmaChPrms_init(&chPrms, chType);
    chHandle         = &chObj;
    backUpChObj      = chObj;
    instID           = UDMA_INST_ID_MAIN_0;
    drvHandle        = &taskObj->testObj->drvObj[instID];
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
    chPrms.chNum     = drvHandle->initPrms.rmInitPrms.startBlkCopyCh;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    if(UDMA_SOK != retVal)
    {  
        GT_0trace(taskObj->traceMask, GT_ERR,
                  " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyCh:: Pos:: "
                  " valid args check!!\n");
        retVal = UDMA_EFAIL;
    }
    else
    {
        retVal = Udma_chClose(chHandle);
        chObj = backUpChObj;
    }

    /* Test scenario 2: Check when chType is UDMA_CH_TYPE_TX and chNum is startTxCh */
    if(UDMA_SOK == retVal)
    { 
        chType           = UDMA_CH_TYPE_TX;
        UdmaChPrms_init(&chPrms, chType);
        chHandle         = &chObj;
        backUpChObj      = chObj;
        instID           = UDMA_INST_ID_MAIN_0;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
        chPrms.chNum     = drvHandle->initPrms.rmInitPrms.startTxCh;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {  
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyCh:: Pos:: "
                      " valid args check!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = Udma_chClose(chHandle);
            chObj = backUpChObj;
        }
    }

    /* Test scenario 3: Check when chType is UDMA_CH_TYPE_RX and chNum is startRxCh */
    if(UDMA_SOK == retVal)
    { 
        chType           = UDMA_CH_TYPE_RX;
        UdmaChPrms_init(&chPrms, chType);
        chHandle         = &chObj;
        backUpChObj      = chObj;
        instID           = UDMA_INST_ID_MAIN_0;
        drvHandle        = &taskObj->testObj->drvObj[instID];
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        chPrms.chNum     = drvHandle->initPrms.rmInitPrms.startRxCh;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {  
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: Udma_rmAllocBlkCopyCh:: Pos:: "
                      " valid args check!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = Udma_chClose(chHandle);
            chObj = backUpChObj;
        }
    }

    return retVal;
}
