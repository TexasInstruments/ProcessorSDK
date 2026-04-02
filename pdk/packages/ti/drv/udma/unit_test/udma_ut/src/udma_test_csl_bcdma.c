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
 *  \file udma_test_csl_bcdma.c
 *
 *  \brief File containing test cases for csl bcdma APIs.
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
 * Test Case Description: Verifies the functions of csl_bcdma.c
 * Test scenario 1: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_DEC_STATS
 * Test scenario 2: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_TRIGGER
 * Test scenario 3: Validate CSL_bcdmaClearTxChanError API
 * Test scenario 4: Validate CSL_bcdmaTriggerTxChan API
 * Test scenario 5: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_ENABLE
 * Test scenario 6: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_DISABLE
 * Test scenario 7: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_CLEAR_ERROR
 * Test scenario 8: Validate CSL_bcdmaEnableLink API when chanDir is CSL_BCDMA_CHAN_DIR_TX
 * Test scenario 9: Validate CSL_bcdmaPauseTxChan API
 * Test scenario 10: Validate CSL_bcdmaUnpauseTxChan API
 * Test scenario 11: Validate CSL_bcdmaDisableTxChan API when channel is enabled
 * Test scenario 12: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_PAUSE
 *                   and channel is not enabled
 * Test scenario 13: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_TEARDOWN
 * Test scenario 14: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_TEARDOWN
 *                   and channel is torn down already
 * Test scenario 15: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_TEARDOWN
 *                   and pOpData is NULL
 * Test scenario 16: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_DEC_STATS
 * Test scenario 17: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_TRIGGER
 * Test scenario 18: Validate CSL_bcdmaTriggerRxChan API
 * Test scenario 19: Validate CSL_bcdmaClearRxChanError API
 * Test scenario 20: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_ENABLE
 * Test scenario 21: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_PAUSE
 * Test scenario 22: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_RESUME
 * Test scenario 23: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_DISABLE
 * Test scenario 24: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_CLEAR_ERROR
 * Test scenario 25: Validate CSL_bcdmaEnableLink API when chanDir is CSL_BCDMA_CHAN_DIR_RX
 * Test scenario 26: Validate CSL_bcdmaDisableRxChan API when chanDir is CSL_BCDMA_CHAN_DIR_RX
 * Test scenario 27: Validate CSL_bcdmaPauseRxChan API when channel is enabled
 * Test scenario 28: Validate CSL_bcdmaUnpauseRxChan API when channel is paused
 * Test scenario 29: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_TEARDOWN
 * Test scenario 30: Validate CSL_bcdmaGetRevision API
 * Test scenario 31: Validate CSL_bcdmaGetRevisionInfo API
 */
int32_t UdmaTestCslBcdma(UdmaTestTaskObj *taskObj)
{
    int32_t               retVal = UDMA_SOK;
    CSL_BcdmaCfg    	 *pBcdmaRegs; 
    CSL_BcdmaChanOp       chanOp;
    CSL_BcdmaChanType     chanType;
    CSL_BcdmaChanDir      chanDir;
    uint32_t              chanIdx;
    uint32_t              chNum;
    void                 *pOpData = NULL;
    Udma_DrvHandle        drvHandle;
    uint32_t              instId;
    struct Udma_ChObj     chObj;
    Udma_ChHandle         chHandle;
    Udma_ChPrms           chPrms;
    uint32_t              chType;
    Udma_ChTxPrms         txChPrms;
    Udma_ChRxPrms         rxChPrms;
    CSL_BcdmaTxChanCfg    pTxChanCfg;
    CSL_BcdmaRxChanCfg    pRxChanCfg;
    CSL_BcdmaRevision     bcdmaRev;
    CSL_BcdmaChanStats    chStats;
    CSL_BcdmaTeardownOpts teardownOpts;
    CSL_BcdmaTeardownOpts BckUpTeardownOpts;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Csl Bcdma Testcase ::\r\n",
              taskObj->taskId);

    /* Configure TX channel */
    instId           = UDMA_TEST_INST_ID_BCDMA_0;
    drvHandle        = &taskObj->testObj->drvObj[instId];
    pBcdmaRegs       = &drvHandle->bcdmaRegs;
    CSL_bcdmaInitCfg(pBcdmaRegs);
    Udma_initDrvHandle(drvHandle);
    chHandle         = &chObj;
    chType           = UDMA_CH_TYPE_TX;
    UdmaChPrms_init(&chPrms, chType);
    chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_TX;
    retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
    UdmaChTxPrms_init(&txChPrms, chType);
    CSL_bcdmaInitTxChanCfg(&pTxChanCfg);
    if(UDMA_SOK == retVal)
    {
        retVal  = Udma_chConfigTx(chHandle, &txChPrms); 
        chanIdx = chHandle->txChNum; 
        if(UDMA_SOK == retVal)
        {
            chanOp   = CSL_BCDMA_CHAN_OP_DEC_STATS;
            chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_TX;
            pOpData  = &chStats;
            /* Test scenario 1: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_DEC_STATS*/
            retVal = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos::"
                          " Check when chanOp is CSL_BCDMA_CHAN_OP_DEC_STATS!!\n");
                retVal = UDMA_EFAIL;
            }
            /* Test scenario 2: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_TRIGGER*/
            chanOp = CSL_BCDMA_CHAN_OP_TRIGGER;
            retVal = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos:: "
                          " Check when chanOp is CSL_BCDMA_CHAN_OP_TRIGGER!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
            /* Test scenario 3: Validate CSL_bcdmaClearTxChanError API */
            chNum = chanIdx + drvHandle->txChOffset;
            CSL_bcdmaClearTxChanError(pBcdmaRegs, chNum);
            /* Test scenario 4: Validate CSL_bcdmaTriggerTxChan API */
            retVal = CSL_bcdmaTriggerTxChan(pBcdmaRegs, chNum);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaTriggerTxChan:: Pos:: "
                          " Check when pBcdmaRegs is valid!!\n");
                retVal = UDMA_EFAIL;
            }
            /* Test scenario 5: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_ENABLE*/
            chanOp = CSL_BCDMA_CHAN_OP_ENABLE;
            retVal = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos::"
                          " Check when chanOp is CSL_BCDMA_CHAN_OP_ENABLE!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
            /* Test scenario 6: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_DISABLE*/
            if(UDMA_SOK == retVal)
            {
                chanOp = CSL_BCDMA_CHAN_OP_DISABLE;
                retVal = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos::"
                              " Check when chanOp is CSL_BCDMA_CHAN_OP_DISABLE!!\n");
                    retVal = UDMA_EFAIL;
                }
            }
            /* Test scenario 7: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_CLEAR_ERROR*/
            if(UDMA_SOK == retVal)
            {
                chanOp   = CSL_BCDMA_CHAN_OP_CLEAR_ERROR;
                retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos::"
                              " Check when chanOp is CSL_BCDMA_CHAN_OP_CLEAR_ERROR!!\n");
                    retVal = UDMA_EFAIL;
                }
            }
            /* Test scenario 8: Validate CSL_bcdmaEnableLink API when chanDir is CSL_BCDMA_CHAN_DIR_TX*/
            if(UDMA_SOK == retVal)
            {
                chanDir = CSL_BCDMA_CHAN_DIR_TX;
                retVal = CSL_bcdmaEnableLink(pBcdmaRegs, chNum, chanDir);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaEnableLink:: Pos::"
                              " Check when chanDir is CSL_BCDMA_CHAN_DIR_TX!!\n");
                    retVal = UDMA_EFAIL;
                }
                /* Test scenario 9: Validate CSL_bcdmaPauseTxChan API */
                if(UDMA_SOK == retVal)
                {
                    retVal = CSL_bcdmaPauseTxChan(pBcdmaRegs, chNum);
                    /* Test scenario 10: Validate CSL_bcdmaUnpauseTxChan API */
                    if(UDMA_SOK == retVal)
                    {
                        retVal = CSL_bcdmaUnpauseTxChan(pBcdmaRegs, chNum);
                        if(UDMA_SOK != retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaUnpauseTxChan:: Pos::"
                                      " Check when chanDir is CSL_BCDMA_CHAN_DIR_TX!!\n");
                            retVal = UDMA_EFAIL;
                        }
                    }
                }
                /* Test scenario 11: Validate CSL_bcdmaDisableTxChan API when channel is enabled*/
                if(UDMA_SOK == retVal)
                {
                    retVal = CSL_bcdmaDisableTxChan(pBcdmaRegs, chNum);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaDisableTxChan:: Pos::"
                                  " Check when channel is enabled!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    /**
                     *  Test scenario 12: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_PAUSE
                     *                     and channel is not enabled 
                     **/
                    else
                    {
                        chanOp = CSL_BCDMA_CHAN_OP_PAUSE;
                        retVal = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                        if(UDMA_SOK == retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                                      " Check when chanOp is CSL_BCDMA_CHAN_OP_PAUSE and"
                                      " channel is not enabled !!\n");
                            retVal = UDMA_EFAIL;
                        }
                        else
                        {
                            retVal = UDMA_SOK;
                        }
                    }
                }
                if(UDMA_SOK == retVal)
                {
                    retVal = CSL_bcdmaEnableLink(pBcdmaRegs, chNum, chanDir);
                    /* Test scenario 13: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_TEARDOWN*/
                    if(UDMA_SOK == retVal)
                    {
                        chanOp             = CSL_BCDMA_CHAN_OP_TEARDOWN;
                        pOpData            = &teardownOpts;
                        BckUpTeardownOpts  = teardownOpts;
                        teardownOpts.force = 1U;
                        teardownOpts.wait  = 1U;
                        retVal = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                        if(UDMA_SOK != retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos::"
                                      " Check when chanOp is CSL_BCDMA_CHAN_OP_TEARDOWN!!\n");
                            retVal = UDMA_EFAIL;
                        }
                        teardownOpts = BckUpTeardownOpts;
                    }
                    /** 
                     * Test scenario 14: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_TEARDOWN
                     *                   and channel is torn down already 
                     **/
                    if(UDMA_SOK == retVal)
                    {
                        chanOp = CSL_BCDMA_CHAN_OP_TEARDOWN;
                        retVal = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, NULL);
                        if(UDMA_SOK == retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                                      " Check when chanOp is channel is CSL_BCDMA_CHAN_OP_TEARDOWN"
                                      " and channel is torn down already!!\n");
                            retVal = UDMA_EFAIL;
                        }
                        else
                        {
                            retVal = UDMA_SOK;
                        }
                    }
                    /**
                     *  Test scenario 15: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_TEARDOWN
                     *                   and pOpData is NULL 
                     **/
                    if(UDMA_SOK == retVal)
                    {
                        retVal  = CSL_bcdmaEnableLink(pBcdmaRegs, chNum, chanDir);
                        chanOp  = CSL_BCDMA_CHAN_OP_TEARDOWN;
                        retVal += CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, NULL);
                        if(UDMA_SOK != retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos::"
                                      " Check when chanOp is channel is CSL_BCDMA_CHAN_OP_TEARDOWN"
                                      " and pOpData is NULL!!\n");
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

    /* Configure RX channel*/
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChRxPrms_init(&rxChPrms, chType);
        CSL_bcdmaInitRxChanCfg(&pRxChanCfg);
        if(UDMA_SOK == retVal)
        {
            retVal   = Udma_chConfigRx(chHandle, &rxChPrms); 
            chanIdx  = chHandle->rxChNum;
            chNum    = chanIdx + drvHandle->rxChOffset;
            /* Test scenario 16: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_DEC_STATS*/
            if(UDMA_SOK == retVal)
            {
                chanOp   = CSL_BCDMA_CHAN_OP_DEC_STATS;
                chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_RX;
                pOpData  = &chStats;
                retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos::"
                              " Check when chanOp is CSL_BCDMA_CHAN_OP_DEC_STATS!!\n");
                    retVal = UDMA_EFAIL;
                }
                /* Test scenario 17: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_TRIGGER */
                chanOp   = CSL_BCDMA_CHAN_OP_TRIGGER;
                retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos::"
                              " Check when chanOp is CSL_BCDMA_CHAN_OP_TRIGGER!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = UDMA_SOK;
                }
                /* Test scenario 18: Validate CSL_bcdmaTriggerRxChan API*/
                retVal = CSL_bcdmaTriggerRxChan(pBcdmaRegs, chNum);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaTriggerRxChan:: Pos::"
                              " Check when pBcdmaRegs is valid!!\n");
                    retVal = UDMA_EFAIL;
                }
                /* Test scenario 19: Validate CSL_bcdmaClearRxChanError API*/
                CSL_bcdmaClearRxChanError(pBcdmaRegs, chNum);
                /* Test scenario 20: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_ENABLE*/
                if(UDMA_SOK == retVal)
                {
                    chanOp = CSL_BCDMA_CHAN_OP_ENABLE;
                    retVal = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos::"
                                  " Check when chanOp is CSL_BCDMA_CHAN_OP_ENABLE!!\n");
                        retVal = UDMA_EFAIL;
                    }
                }
                /* Test scenario 21: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_PAUSE*/
                if(UDMA_SOK == retVal)
                {
                    chanOp = CSL_BCDMA_CHAN_OP_PAUSE;
                    retVal = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos::"
                                  " Check when chanOp is CSL_BCDMA_CHAN_OP_PAUSE!!\n");
                        retVal = UDMA_EFAIL;
                    }
                }
                /* Test scenario 22: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_RESUME*/
                if(UDMA_SOK == retVal)
                {
                    chanOp = CSL_BCDMA_CHAN_OP_RESUME;
                    retVal = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos::"
                                  " Check when chanOp is CSL_BCDMA_CHAN_OP_RESUME!!\n");
                        retVal = UDMA_EFAIL;
                    }
                }
                /* Test scenario 23: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_DISABLE*/
                if(UDMA_SOK == retVal)
                {
                    chanOp = CSL_BCDMA_CHAN_OP_DISABLE;
                    retVal = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos::"
                                  " Check when chanOp is CSL_BCDMA_CHAN_OP_DISABLE!!\n");
                        retVal = UDMA_EFAIL;
                    }
                }
                /* Test scenario 24: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_CLEAR_ERROR*/
                if(UDMA_SOK == retVal)
                {
                    chanOp = CSL_BCDMA_CHAN_OP_CLEAR_ERROR;
                    retVal = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: POs::"
                                  " Check when chanOp is CSL_BCDMA_CHAN_OP_CLEAR_ERROR!!\n");
                        retVal = UDMA_EFAIL;
                    }
                }
                /* Test scenario 25: Validate CSL_bcdmaEnableLink API when chanDir is CSL_BCDMA_CHAN_DIR_RX*/
                if(UDMA_SOK == retVal)
                {
                    chanDir = CSL_BCDMA_CHAN_DIR_RX;
                    retVal  = CSL_bcdmaEnableLink(pBcdmaRegs, chNum, chanDir);
                    if(UDMA_SOK != retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaEnableLink:: Pos::"
                                  " Check when chanDir is CSL_BCDMA_CHAN_DIR_RX!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    /* Test scenario 26: Validate CSL_bcdmaDisableRxChan API when chanDir is CSL_BCDMA_CHAN_DIR_RX*/
                    if(UDMA_SOK == retVal)
                    {
                        retVal = CSL_bcdmaDisableRxChan(pBcdmaRegs, chNum);
                        if(UDMA_SOK != retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaDisableRxChan:: Pos::"
                                      " Check when chanDir is CSL_BCDMA_CHAN_DIR_RX!!\n");
                            retVal = UDMA_EFAIL;
                        }
                    }
                }
                if(UDMA_SOK == retVal)
                {
                    retVal = CSL_bcdmaEnableLink(pBcdmaRegs, chNum, chanDir);
                    /* Test scenario 27: Validate CSL_bcdmaPauseRxChan API when channel is enabled*/
                    if(UDMA_SOK == retVal)
                    {
                        retVal = CSL_bcdmaPauseRxChan(pBcdmaRegs, chNum);
                        /* Test scenario 28: Validate CSL_bcdmaUnpauseRxChan API when channel is paused */
                        if(UDMA_SOK == retVal)
                        {
                            retVal = CSL_bcdmaUnpauseRxChan(pBcdmaRegs, chNum);
                            if(UDMA_SOK != retVal)
                            {
                                GT_0trace(taskObj->traceMask, GT_ERR,
                                        " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaUnpauseRxChan:: Pos::"
                                        " Check when when channel is paused!!\n");
                                retVal = UDMA_EFAIL;
                            }
                        }
                        /* Test scenario 29: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_TEARDOWN*/
                        if(UDMA_SOK == retVal)
                        {
                            chanOp             = CSL_BCDMA_CHAN_OP_TEARDOWN;
                            pOpData            = &teardownOpts;
                            teardownOpts.force = 0U;
                            retVal = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                            if(UDMA_SOK != retVal)
                            {
                                GT_0trace(taskObj->traceMask, GT_ERR,
                                        " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Pos::"
                                        " Check when chanOp is CSL_BCDMA_CHAN_OP_TEARDOWN!!\n");
                                retVal = UDMA_EFAIL;
                            }
                            teardownOpts = BckUpTeardownOpts;
                        }
                    }
                }
            }
            Udma_chClose(chHandle);
        }
    }
    /* Test scenario 30: Validate CSL_bcdmaGetRevision API */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_bcdmaGetRevision ::\r\n",
                  taskObj->taskId);
        uint32_t rev = CSL_bcdmaGetRevision(pBcdmaRegs);
        if(0U != rev)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      ":: BCDMA Revision: %u ::\r\n",
                      rev);
        }
        else
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_bcdmaGetRevision ::\r\n",
                      taskObj->taskId);
            retVal = UDMA_EFAIL;
        }

    }
    
    /* Test scenario 31: Validate CSL_bcdmaGetRevisionInfo API. */
    if(UDMA_SOK == retVal)
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: Test CSL_bcdmaGetRevisionInfo ::\r\n",
                  taskObj->taskId);
        retVal = CSL_bcdmaGetRevisionInfo(pBcdmaRegs, &bcdmaRev);
        if(UDMA_SOK != retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_bcdmaGetRevisionInfo ::\r\n",
                      taskObj->taskId);
            retVal = UDMA_EFAIL;
        }
    }

    return retVal;
}

/*
 * Test Case Description: Verifies the functions of csl_bcdma.c
 * Test scenario 1: Validate CSL_bcdmaGetRevisionInfo API when pRev is NULL
 * Test scenario 2: Validate CSL_bcdmaGetRevisionInfo API when bcdmaCfg is NULL and pRev is NULL
 * Test scenario 3: Validate CSL_bcdmaGetRevisionInfo API when pGenCfgRegs is NULL and pRev is NULL
 * Test scenario 4: Validate CSL_bcdmaGetRevisionInfo API when bcdmaCfg is NULL
 * Test scenario 5: Validate CSL_bcdmaGetRevisionInfo API when pGenCfgRegs is NULL
 * Test scenario 6: Validate CSL_bcdmaGetRevision API when pGenCfgRegs is NULL
 * Test scenario 7: Validate CSL_bcdmaGetRevision API when bcdmaCfg is NULL
 * Test scenario 8: Validate CSL_bcdmaDecChanStats API when chanDir is CSL_BCDMA_CHAN_DIR_RX
 * Test scenario 9: Validate CSL_bcdmaEnableTxChan API when channel is opened
 * Test scenario 9: Validate CSL_bcdmaEnableTxChan API when bcChanCnt is non zero,
 *                  splitTxChanCnt is OU and splitRxChanCnt is non zero
 * Test scenario 10: Validate CSL_bcdmaEnableTxChan API when bcChanCnt,
 *                  splitTxChanCnt and splitRxChanCnt are non zero
 * Test scenario 11: Validate CSL_bcdmaEnableTxChan API when bcChanCnt,
 *                  splitTxChanCnt are non zero and splitRxChanCnt is zero
 * Test scenario 12: Validate CSL_bcdmaEnableTxChan API when bcChanCnt is non zero,
 *                  splitTxChanCnt and splitRxChanCnt are zero
 * Test scenario 13: Validate CSL_bcdmaEnableRxChan API when channel is opened
 * Test scenario 14: Validate CSL_bcdmaChanOp API when chanType is greater than 
 *                   CSL_BCDMA_CHAN_TYPE_SPLIT_RX and chanIdx is greater than splitRxChanCnt
 * Test scenario 15: Validate CSL_bcdmaChanOp API when chanType is CSL_BCDMA_CHAN_TYPE_SPLIT_TX
 *                    and chanIdx is greater than splitTxChanCnt
 * Test scenario 16: Validate CSL_bcdmaChanOp API when chanType is greater than 
 *                  CSL_BCDMA_CHAN_TYPE_SPLIT_RX and chanIdx is equal to splitRxChanCnt
 * Test scenario 17: Validate CSL_bcdmaChanOp API when chanType is CSL_BCDMA_CHAN_TYPE_SPLIT_RX
 *                   and chanIdx is greater than splitRxChanCnt
 * Test scenario 18: Validate CSL_bcdmaChanOp API when chanType is CSL_BCDMA_CHAN_TYPE_SPLIT_TX
 *                   and chanIdx is greater than splitTxChanCnt
 * Test scenario 19: Validate CSL_bcdmaChanOp API when chanOp is Invalid
 * Test scenario 20: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_GET_RT 
 *                   and pOpData is NULL
 * Test scenario 21: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_SET_RT 
 *                   and pOpData is NULL
 * Test scenario 22: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_GET_STATS 
 *                   and pOpData is NULL
 * Test scenario 23: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_DEC_STATS 
 *                and pOpData is NULL
 * Test scenario 24: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_GET_REMOTE_PEER_REG 
 *                  and pOpData is NULL
 * Test scenario 25: Validate CSL_bcdmaChanOp API when regIdx is equal to 16U 
 * Test scenario 26: Validate CSL_bcdmaInitCfg API
 * Test scenario 27: Validate CSL_bcdmaGetCfg API
 * Test scenario 28: Validate CSL_bcdmaEnableTxChan API
 * Test scenario 29: Validate CSL_bcdmaEnableRxChan API 
 * Test scenario 30: Validate CSL_bcdmaGetTxRT API when pBcdmaRegs and pRT are NULL
 * Test scenario 31: Validate CSL_bcdmaGetRxRT API when pBcdmaRegs and pRT are NULL
 * Test scenario 32: Validate CSL_bcdmaSetTxRT API when pBcdmaRegs and pRT are NULL 
 * Test scenario 33: Validate CSL_bcdmaSetRxRT API when pBcdmaRegs and pRT are NULL
 * Test scenario 34: Validate CSL_bcdmaDisableTxChan API when pBcdmaRegs is NULL 
 * Test scenario 35: Validate CSL_bcdmaDisableRxChan API when pBcdmaRegs is NULL
 * Test scenario 36: Validate CSL_bcdmaTeardownTxChan API when pBcdmaRegs is NULL
 * Test scenario 37: Validate CSL_bcdmaTeardownRxChan API when pBcdmaRegs is NULL
 * Test scenario 38: Validate CSL_bcdmaPauseTxChan API when pBcdmaRegs is NULL
 * Test scenario 39: Validate CSL_bcdmaUnpauseTxChan API when pBcdmaRegs is NULL
 * Test scenario 40: Validate CSL_bcdmaTriggerTxChan API when pBcdmaRegs is NULL
 * Test scenario 41: Validate CSL_bcdmaTriggerRxChan API when pBcdmaRegs is NULL
 * Test scenario 42: Validate CSL_bcdmaGetChanPeerReg API when pBcdmaRegs is NULL
 * Test scenario 43: Validate CSL_bcdmaSetChanPeerReg API when pBcdmaRegs is NULL 
 * Test scenario 44: Validate CSL_bcdmaEnableLink API when pBcdmaRegs is NULL 
 *                   and chanDir is CSL_BCDMA_CHAN_DIR_RX
 * Test scenario 45: Validate CSL_bcdmaEnableLink API when pBcdmaRegs is NULL and 
 *                   chanDir is CSL_BCDMA_CHAN_DIR_TX 
 * Test scenario 46: Validate CSL_bcdmaChanOp API when pBcdmaRegs is NULL, chanIdx
 *                   is splitRxChanCnt and chanType is CSL_BCDMA_CHAN_TYPE_SPLIT_RX
 * Test scenario 47: Validate CSL_bcdmaChanOp API when pBcdmaRegs is NULL, 
 *                   chanIdx is splitRxChanCnt and chanType is greater 
 *                   than CSL_BCDMA_CHAN_TYPE_SPLIT_RX
 * Test scenario 48: Validate CSL_bcdmaGetChanStats API when pBcdmaRegs is NULL
 * Test scenario 49: Validate CSL_bcdmaDecChanStats API when pBcdmaRegs is NULL
 * Test scenario 50: Validate CSL_bcdmaClearTxChanError API when pBcdmaRegs is NULL
 * Test scenario 51: Validate CSL_bcdmaClearRxChanError API when pBcdmaRegs is NULL
 */
int32_t UdmaTestCslBcdmaNeg(UdmaTestTaskObj *taskObj)
{
    int32_t                 retVal = UDMA_SOK;
    CSL_BcdmaCfg    	   *pBcdmaRegs; 
    CSL_BcdmaChanOp         chanOp;
    CSL_BcdmaChanType       chanType;
    CSL_BcdmaChanDir        chanDir;
    uint32_t                chanIdx;
    uint32_t                chNum;
    void                   *pOpData;
    Udma_DrvHandle          drvHandle;
    uint32_t                instId;
    struct Udma_ChObj       chObj;
    Udma_ChHandle           chHandle;
    Udma_ChPrms             chPrms;
    uint32_t                chType;
    Udma_ChRxPrms           rxChPrms;
    uint32_t                rev;
    uint32_t               *pVal;
    CSL_BcdmaRevision       bcdmaRev;
    CSL_BcdmaRemotePeerOpts pPeerOpts;
    uint32_t                regIdx;
    CSL_BcdmaChanStats      pChanStats;
    CSL_bcdma_gcfgRegs     *bkPtrGcfgRegs;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Csl Bcdma negative Testcase ::\r\n",
              taskObj->taskId);

    chHandle         = &chObj;
    instId           = UDMA_TEST_INST_ID_BCDMA_0;
    drvHandle        = &taskObj->testObj->drvObj[instId];
    pBcdmaRegs       = &drvHandle->bcdmaRegs;
    /* Test scenario 1: Validate CSL_bcdmaGetRevisionInfo API when pRev is NULL */
    GT_1trace(taskObj->traceMask, GT_INFO1,
            " |TEST INFO|:: Task:%d: Test CSL_bcdmaGetRevisionInfo ::\r\n",
            taskObj->taskId);
    retVal = CSL_bcdmaGetRevisionInfo(pBcdmaRegs, NULL);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_1trace(taskObj->traceMask, GT_INFO1,
                  " |TEST INFO|:: Task:%d: FAIL :: Test CSL_bcdmaGetRevisionInfo when pRev is NULL ::\r\n",
                  taskObj->taskId);
        retVal = UDMA_EFAIL;
    }
    /* Test scenario 2: Validate CSL_bcdmaGetRevisionInfo API when bcdmaCfg is NULL and pRev is NULL */
    if(UDMA_SOK == retVal)
    {
        retVal = CSL_bcdmaGetRevisionInfo(NULL, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_bcdmaGetRevisionInfo API when bcdmaCfg is NULL and when pRev is NULL ::\r\n",
                      taskObj->taskId);
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 3: Validate CSL_bcdmaGetRevisionInfo API when pGenCfgRegs is NULL and pRev is NULL */
    if(UDMA_SOK == retVal)
    {
        bkPtrGcfgRegs                    = drvHandle->bcdmaRegs.pGenCfgRegs;
        drvHandle->bcdmaRegs.pGenCfgRegs = NULL;
        retVal                           = CSL_bcdmaGetRevisionInfo(pBcdmaRegs, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_bcdmaGetRevisionInfo API when pGenCfgRegs is NULL and when pRev is NULL ::\r\n",
                      taskObj->taskId);
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
        drvHandle->bcdmaRegs.pGenCfgRegs = bkPtrGcfgRegs;
    }
    /* Test scenario 4: Validate CSL_bcdmaGetRevisionInfo API when bcdmaCfg is NULL */
    if(UDMA_SOK == retVal)
    {
        retVal = CSL_bcdmaGetRevisionInfo(NULL, &bcdmaRev);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_bcdmaGetRevisionInfo API when bcdmaCfg is NULL ::\r\n",
                      taskObj->taskId);
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 5: Validate CSL_bcdmaGetRevisionInfo API when pGenCfgRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        bkPtrGcfgRegs                    = drvHandle->bcdmaRegs.pGenCfgRegs;
        drvHandle->bcdmaRegs.pGenCfgRegs = NULL;
        retVal                           = CSL_bcdmaGetRevisionInfo(pBcdmaRegs, &bcdmaRev);
        if(UDMA_SOK == retVal)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_bcdmaGetRevision API when pGenCfgRegs is NULL ::\r\n",
                      taskObj->taskId);
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
        drvHandle->bcdmaRegs.pGenCfgRegs = bkPtrGcfgRegs;
    }
    /* Test scenario 6: Validate CSL_bcdmaGetRevision API when pGenCfgRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        bkPtrGcfgRegs                    = drvHandle->bcdmaRegs.pGenCfgRegs;
        drvHandle->bcdmaRegs.pGenCfgRegs = NULL;
        rev                              = CSL_bcdmaGetRevision(pBcdmaRegs);
        if(0U != rev)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_bcdmaGetRevision API when pGenCfgRegs is NULL ::\r\n",
                      taskObj->taskId);
            retVal = UDMA_EFAIL;
        }
        drvHandle->bcdmaRegs.pGenCfgRegs = bkPtrGcfgRegs;
    }
    /* Test scenario 7: Validate CSL_bcdmaGetRevision API when bcdmaCfg is NULL */
    if(UDMA_SOK == retVal)
    {
        rev = CSL_bcdmaGetRevision(NULL);
        if(0U != rev)
        {
            GT_1trace(taskObj->traceMask, GT_INFO1,
                      " |TEST INFO|:: Task:%d: FAIL :: Test CSL_bcdmaGetRevision API when bcdmaCfg is NULL ::\r\n",
                      taskObj->taskId);
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChRxPrms_init(&rxChPrms, chType);
        /* Test scenario 8: Validate CSL_bcdmaDecChanStats API when chanDir is CSL_BCDMA_CHAN_DIR_RX */
        if(UDMA_SOK == retVal)
        {
            retVal   = Udma_chConfigRx(chHandle, &rxChPrms); 
            chanIdx  = chHandle->rxChNum;
            chNum    = chanIdx + drvHandle->rxChOffset;
            chanDir  = CSL_BCDMA_CHAN_DIR_RX;
            CSL_bcdmaDecChanStats(pBcdmaRegs, chNum, chanDir, &pChanStats);
            pBcdmaRegs->bcChanCnt      = 0U;
            pBcdmaRegs->splitTxChanCnt = 1U;
            pBcdmaRegs->splitRxChanCnt = 1U;
            /* Test scenario 9: Validate CSL_bcdmaEnableTxChan API when channel is opened*/
            retVal = CSL_bcdmaEnableTxChan(pBcdmaRegs, chNum);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaEnableTxChan:: Pos::"
                          " Check when channel is opened!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
            /* Test scenario 9: Validate CSL_bcdmaEnableTxChan API when bcChanCnt is non zero,
            *                  splitTxChanCnt is OU and splitRxChanCnt is non zero */
            pBcdmaRegs->bcChanCnt      = 1U;
            pBcdmaRegs->splitTxChanCnt = 0U;
            pBcdmaRegs->splitRxChanCnt = 1U;
            retVal                     = CSL_bcdmaEnableTxChan(pBcdmaRegs, chNum);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaEnableTxChan:: Pos::"
                          " Check when bcChanCnt is non zero, splitTxChanCnt is OU"
                          " and splitRxChanCnt is non zero!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
            /* Test scenario 10: Validate CSL_bcdmaEnableTxChan API when bcChanCnt,
            *                  splitTxChanCnt and splitRxChanCnt are non zero */
            pBcdmaRegs->bcChanCnt      = 1U;
            pBcdmaRegs->splitTxChanCnt = 1U;
            pBcdmaRegs->splitRxChanCnt = 1U;
            retVal                     = CSL_bcdmaEnableTxChan(pBcdmaRegs, chNum); 
            if(UDMA_SOK == retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaEnableTxChan:: Neg::"
                          " Check when bcChanCnt, splitTxChanCnt"
                          " and splitRxChanCnt are non zero!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
            /* Test scenario 11: Validate CSL_bcdmaEnableTxChan API when bcChanCnt,
            *                  splitTxChanCnt are non zero and splitRxChanCnt is zero */
            pBcdmaRegs->bcChanCnt      = 1U;
            pBcdmaRegs->splitTxChanCnt = 1U;
            pBcdmaRegs->splitRxChanCnt = 0U;
            retVal                     = CSL_bcdmaEnableTxChan(pBcdmaRegs, chNum); 
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaEnableTxChan:: Pos::"
                          " Check when bcChanCnt, splitTxChanCnt"
                          " are non zero and splitRxChanCnt is zero!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
            /* Test scenario 12: Validate CSL_bcdmaEnableTxChan API when bcChanCnt is non zero,
            *                  splitTxChanCnt and splitRxChanCnt are zero */
            pBcdmaRegs->bcChanCnt      = 1U;
            pBcdmaRegs->splitTxChanCnt = 0U;
            pBcdmaRegs->splitRxChanCnt = 0U;
            retVal                     = CSL_bcdmaEnableTxChan(pBcdmaRegs, chNum);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaEnableTxChan:: Pos::"
                          " Check when bcChanCnt is non zero,"
                          " splitTxChanCnt and splitRxChanCnt are zero!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
        }
        Udma_chClose(chHandle);
    }

    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChRxPrms_init(&rxChPrms, chType);
        /* Test scenario 13: Validate CSL_bcdmaEnableRxChan API when channel is opened*/
        if(UDMA_SOK == retVal)
        {
            retVal   = Udma_chConfigRx(chHandle, &rxChPrms); 
            chanIdx  = chHandle->rxChNum;
            chNum    = chanIdx + drvHandle->rxChOffset;
            retVal = CSL_bcdmaEnableRxChan(pBcdmaRegs, chNum);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaEnableRxChan:: Pos::"
                          " Check when channel is opened!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
        }
        Udma_chClose(chHandle);
    }
    /* Test scenario 14: Validate CSL_bcdmaChanOp API when chanType is greater than 
    *                   CSL_BCDMA_CHAN_TYPE_SPLIT_RX and chanIdx is greater than splitRxChanCnt */
    if(UDMA_SOK == retVal)
    {
        pOpData  = NULL;
        chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_RX + 1U;
        chanIdx  = pBcdmaRegs->splitRxChanCnt + 1U;
        chanOp   = CSL_BCDMA_CHAN_OP_ENABLE;
        retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                      " Check when chanType is greater than CSL_BCDMA_CHAN_TYPE_SPLIT_RX"
                      " and chanIdx is greater than splitRxChanCnt!!\n");
            retVal = UDMA_EFAIL;
        }
        /* Test scenario 15: Validate CSL_bcdmaChanOp API when chanType is CSL_BCDMA_CHAN_TYPE_SPLIT_TX
        *                    and chanIdx is greater than splitTxChanCnt */
        else
        {
            retVal   = UDMA_SOK;
            chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_TX;
            chanIdx  = pBcdmaRegs->splitTxChanCnt + 1U;
            retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
            if(UDMA_SOK == retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                          " Check when chanType is CSL_BCDMA_CHAN_TYPE_SPLIT_TX"
                          " and chanIdx is greater than splitTxChanCnt!!\n");
                retVal = UDMA_EFAIL;
            }
            /* Test scenario 16: Validate CSL_bcdmaChanOp API when chanType is greater than 
            *                  CSL_BCDMA_CHAN_TYPE_SPLIT_RX and chanIdx is equal to splitRxChanCnt*/
            else
            {
                retVal   = UDMA_SOK;
                chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_RX + 1U;
                chanIdx  = pBcdmaRegs->splitRxChanCnt;
                retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                              " Check when chanType is greater than CSL_BCDMA_CHAN_TYPE_SPLIT_RX"
                              " and chanIdx is equal to splitRxChanCnt!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = UDMA_SOK;
                }
            }
            /* Test scenario 17: Validate CSL_bcdmaChanOp API when chanType is CSL_BCDMA_CHAN_TYPE_SPLIT_RX
            *                   and chanIdx is greater than splitRxChanCnt */
            if(UDMA_SOK == retVal)
            {
                pOpData  = &pPeerOpts;
                chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_RX;
                chanIdx  = pBcdmaRegs->splitRxChanCnt + 1;
                chanOp   = CSL_BCDMA_CHAN_OP_CLEAR_ERROR;
                retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                              " Check when chanType is CSL_BCDMA_CHAN_TYPE_SPLIT_RX"
                              " and chanIdx is greater than splitRxChanCnt!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = UDMA_SOK;
                }
            }
            /* Test scenario 18: Validate CSL_bcdmaChanOp API when chanType is CSL_BCDMA_CHAN_TYPE_SPLIT_TX
            *                   and chanIdx is greater than splitTxChanCnt */
            if(UDMA_SOK == retVal)
            {
                pOpData  = &pPeerOpts;
                chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_TX;
                chanIdx  = pBcdmaRegs->splitTxChanCnt + 1;
                chanOp   = CSL_BCDMA_CHAN_OP_CLEAR_ERROR;
                retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                              " Check when chanType is CSL_BCDMA_CHAN_TYPE_SPLIT_TX"
                              " and chanIdx is greater than splitTxChanCnt!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = UDMA_SOK;
                }
            }
            /* Test scenario 19: Validate CSL_bcdmaChanOp API when chanOp is Invalid */
            if(UDMA_SOK == retVal)
            {
                pOpData  = &pPeerOpts;
                chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_RX;
                chanIdx  = pBcdmaRegs->splitRxChanCnt;
                chanOp   = CSL_BCDMA_CHAN_OP_CLEAR_ERROR +1U;
                retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
                if(UDMA_SOK == retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR,
                              " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                              " Check when chanOp is Invalid!!\n");
                    retVal = UDMA_EFAIL;
                }
                else
                {
                    retVal = UDMA_SOK;
                }
            }
        }
    }
    /* Test scenario 20: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_GET_RT 
    *                   and pOpData is NULL */
    if(UDMA_SOK == retVal)
    {
        chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_RX;
        chanOp   = CSL_BCDMA_CHAN_OP_GET_RT;
        retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                      " Check when chanOp is CSL_BCDMA_CHAN_OP_GET_RT"
                      " and pOpData is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 21: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_SET_RT 
    *                   and pOpData is NULL */
    if(UDMA_SOK == retVal)
    {
        chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_RX;
        chanOp   = CSL_BCDMA_CHAN_OP_SET_RT;
        retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                      " Check when chanOp is CSL_BCDMA_CHAN_OP_SET_RT"
                      " and pOpData is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 22: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_GET_STATS 
    *                   and pOpData is NULL */
    if(UDMA_SOK == retVal)
    {
        chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_RX;
        chanOp   = CSL_BCDMA_CHAN_OP_GET_STATS;
        retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                      " Check when chanOp is CSL_BCDMA_CHAN_OP_GET_STATS"
                      " and pOpData is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 23: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_DEC_STATS 
    *                   and pOpData is NULL */
    if(UDMA_SOK == retVal)
    {
        chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_RX;
        chanOp   = CSL_BCDMA_CHAN_OP_DEC_STATS;
        retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                      " Check when chanOp is CSL_BCDMA_CHAN_OP_DEC_STATS"
                      " and pOpData is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 24: Validate CSL_bcdmaChanOp API when chanOp is CSL_BCDMA_CHAN_OP_GET_REMOTE_PEER_REG 
    *                   and pOpData is NULL */
    if(UDMA_SOK == retVal)
    {
        chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_RX;
        chanOp   = CSL_BCDMA_CHAN_OP_GET_REMOTE_PEER_REG;
        retVal   = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                      " Check when chanOp is CSL_BCDMA_CHAN_OP_GET_REMOTE_PEER_REG"
                      " and pOpData is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 25: Validate CSL_bcdmaChanOp API when regIdx is equal to 16U */
    if(UDMA_SOK == retVal)
    {
        chType           = UDMA_CH_TYPE_RX;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.peerChNum = UDMA_PSIL_CH_MCU_CPSW0_RX;
        retVal           = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        UdmaChRxPrms_init(&rxChPrms, chType);
        if(UDMA_SOK == retVal)
        {
            retVal           = Udma_chConfigRx(chHandle, &rxChPrms); 
            chanIdx          = chHandle->rxChNum;
            chanType         = CSL_BCDMA_CHAN_TYPE_SPLIT_RX;
            chanOp           = CSL_BCDMA_CHAN_OP_GET_REMOTE_PEER_REG;
            pOpData          = &pPeerOpts;
            pPeerOpts.regIdx = 16U;
            retVal           = CSL_bcdmaChanOp(pBcdmaRegs, chanOp, chanType, chanIdx, pOpData);
            if(UDMA_SOK == retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR,
                          " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                          " Check when regIdx is equal to 16U!!\n");
                retVal = UDMA_EFAIL;
            }
            else
            {
                retVal = UDMA_SOK;
            }
        }
        Udma_chClose(chHandle);
    }

    /* Test scenario 26: Validate CSL_bcdmaInitCfg API */
    CSL_bcdmaInitCfg(NULL);
    /* Test scenario 27: Validate CSL_bcdmaGetCfg API */
    CSL_bcdmaGetCfg(NULL);
    /* Test scenario 28: Validate CSL_bcdmaEnableTxChan API */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->txChOffset;
        retVal = CSL_bcdmaEnableTxChan(NULL, chNum);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaEnableTxChan:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 29: Validate CSL_bcdmaEnableRxChan API */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->rxChOffset;
        retVal = CSL_bcdmaEnableRxChan(NULL, chNum);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaEnableRxChan:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 30: Validate CSL_bcdmaGetTxRT API when pBcdmaRegs and pRT are NULL */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->txChOffset;
        retVal = CSL_bcdmaGetTxRT(NULL, chNum, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaGetTxRT:: Neg::"
                      " Check when pBcdmaRegs and pRT are NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 31: Validate CSL_bcdmaGetRxRT API when pBcdmaRegs and pRT are NULL */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->rxChOffset;
        retVal = CSL_bcdmaGetRxRT(NULL, chNum, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaGetRxRT:: Neg::"
                      " Check when pBcdmaRegs and pRT are NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 32: Validate CSL_bcdmaSetTxRT API when pBcdmaRegs and pRT are NULL */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->txChOffset;
        retVal = CSL_bcdmaSetTxRT(NULL, chNum, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaSetTxRT:: Neg::"
                      " Check when pBcdmaRegs and pRT are NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 33: Validate CSL_bcdmaSetRxRT API when pBcdmaRegs and pRT are NULL */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->txChOffset;
        retVal = CSL_bcdmaSetRxRT(NULL, chNum, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaSetRxRT:: Neg::"
                      " Check when pBcdmaRegs and pRT are NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 34: Validate CSL_bcdmaDisableTxChan API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->txChOffset;
        retVal = CSL_bcdmaDisableTxChan(NULL, chNum);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaDisableTxChan:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 35: Validate CSL_bcdmaDisableRxChan API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->rxChOffset;
        retVal = CSL_bcdmaDisableRxChan(NULL, chNum);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaDisableRxChan:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 36: Validate CSL_bcdmaTeardownTxChan API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->txChOffset;
        retVal =  CSL_bcdmaTeardownTxChan(NULL, chNum, (bool)false, (bool)true);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaTeardownTxChan:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 37: Validate CSL_bcdmaTeardownRxChan API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->rxChOffset;
        retVal =  CSL_bcdmaTeardownRxChan(NULL, chNum, (bool)false, (bool)true);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaTeardownRxChan:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 38: Validate CSL_bcdmaPauseTxChan API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->txChOffset;
        retVal =  CSL_bcdmaPauseTxChan(NULL, chNum);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaPauseTxChan:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 39: Validate CSL_bcdmaUnpauseTxChan API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->txChOffset;
        retVal =  CSL_bcdmaUnpauseTxChan(NULL, chNum);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaUnpauseTxChan:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 40: Validate CSL_bcdmaTriggerTxChan API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->txChOffset;
        retVal =  CSL_bcdmaTriggerTxChan(NULL, chNum);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaTriggerTxChan:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 41: Validate CSL_bcdmaTriggerRxChan API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        chNum  = chanIdx + drvHandle->rxChOffset;
        retVal =  CSL_bcdmaTriggerRxChan(NULL, chNum);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaTriggerRxChan:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 42: Validate CSL_bcdmaGetChanPeerReg API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        pVal    = &pPeerOpts.regVal;
        chanDir = CSL_BCDMA_CHAN_DIR_RX;
        regIdx  = pPeerOpts.regIdx;
        chNum   = chanIdx + drvHandle->rxChOffset;
        retVal  =  CSL_bcdmaGetChanPeerReg(NULL, chNum, chanDir, regIdx, pVal);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaGetChanPeerReg:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 43: Validate CSL_bcdmaSetChanPeerReg API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        pVal    = &pPeerOpts.regVal;
        chanDir = CSL_BCDMA_CHAN_DIR_RX;
        chNum   = chanIdx + drvHandle->rxChOffset;
        retVal  =  CSL_bcdmaSetChanPeerReg(NULL, chNum, chanDir, regIdx, pVal);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaSetChanPeerReg:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 44: Validate CSL_bcdmaEnableLink API when pBcdmaRegs is NULL 
    *                   and chanDir is CSL_BCDMA_CHAN_DIR_RX */
    if(UDMA_SOK == retVal)
    {
        chanDir = CSL_BCDMA_CHAN_DIR_RX;
        chNum   = chanIdx + drvHandle->rxChOffset;
        retVal  =  CSL_bcdmaEnableLink(NULL, chNum, chanDir);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaEnableLink:: Neg::"
                      " Check when pBcdmaRegs is NULL"
                      " and chanDir is CSL_BCDMA_CHAN_DIR_RX!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 45: Validate CSL_bcdmaEnableLink API when pBcdmaRegs is NULL and 
    *                   chanDir is CSL_BCDMA_CHAN_DIR_TX */
    if(UDMA_SOK == retVal)
    {
        chanDir = CSL_BCDMA_CHAN_DIR_TX;
        chNum   = chanIdx + drvHandle->txChOffset;
        retVal  =  CSL_bcdmaEnableLink(NULL, chNum, chanDir);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaEnableLink:: Neg::"
                      " Check when pBcdmaRegs is NULL"
                      " and chanDir is CSL_BCDMA_CHAN_DIR_TX!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 46: Validate CSL_bcdmaChanOp API when pBcdmaRegs is NULL, chanIdx
    *                   is splitRxChanCnt and chanType is CSL_BCDMA_CHAN_TYPE_SPLIT_RX */
    if(UDMA_SOK == retVal)
    {
        chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_RX;
        chanIdx  = pBcdmaRegs->splitRxChanCnt;
        retVal   = CSL_bcdmaChanOp(NULL, chanOp, chanType, chanIdx, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                      " Check when pBcdmaRegs is NULL, chanIdx is splitRxChanCnt"
                      " and chanType is CSL_BCDMA_CHAN_TYPE_SPLIT_RX!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }
    /* Test scenario 47: Validate CSL_bcdmaChanOp API when pBcdmaRegs is NULL, 
    *                   chanIdx is splitRxChanCnt and chanType is greater 
    *                   than CSL_BCDMA_CHAN_TYPE_SPLIT_RX */
    if(UDMA_SOK == retVal)
    {
        chanType = CSL_BCDMA_CHAN_TYPE_SPLIT_RX + 1U;
        chanIdx  = pBcdmaRegs->splitRxChanCnt + 1U;
        retVal   = CSL_bcdmaChanOp(NULL, chanOp, chanType, chanIdx, NULL);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                      " Check when pBcdmaRegs is NULL, chanIdx is splitRxChanCnt and"
                      " chanType is greater than CSL_BCDMA_CHAN_TYPE_SPLIT_RX!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 48: Validate CSL_bcdmaGetChanStats API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        chanDir = CSL_BCDMA_CHAN_DIR_TX;
        retVal  = CSL_bcdmaGetChanStats(NULL, chanIdx, chanDir, &pChanStats);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaGetChanStats:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 49: Validate CSL_bcdmaDecChanStats API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        chanDir = CSL_BCDMA_CHAN_DIR_TX;
        retVal  = CSL_bcdmaDecChanStats(NULL, chanIdx, chanDir, &pChanStats);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaChanOp:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 50: Validate CSL_bcdmaClearTxChanError API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        retVal = CSL_bcdmaClearTxChanError(NULL, chanIdx);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaClearTxChanError:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    /* Test scenario 51: Validate CSL_bcdmaClearRxChanError API when pBcdmaRegs is NULL */
    if(UDMA_SOK == retVal)
    {
        retVal = CSL_bcdmaClearRxChanError(NULL, chanIdx);
        if(UDMA_SOK == retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR,
                      " |TEST INFO|:: FAIL:: UDMA:: CSL_bcdmaClearRxChanError:: Neg::"
                      " Check when pBcdmaRegs is NULL!!\n");
            retVal = UDMA_EFAIL;
        }
        else
        {
            retVal = UDMA_SOK;
        }
    }

    return retVal;
}
