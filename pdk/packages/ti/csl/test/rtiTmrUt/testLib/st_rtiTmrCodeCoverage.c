/*
 *   Copyright (c) 2020 Texas Instruments Incorporated
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
 *
 */

 /**
 *  \file     st_rtiTmrCodeCoverage.c
 *
 *  \brief    This file contains RTI Timer code coverage test code.
 *
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <st_rtiTmr.h>

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
#define RTI_TMR_INVALID_BASE_ADDR           (0U)
#define RTI_TMR_INVALID_CNT_BLK_INDEX       (RTI_TMR_CNT_BLK_INDEX_MAX + 1U)
#define RTI_TMR_INVALID_CMP_BLK_INDEX       (RTI_TMR_CMP_BLK_INDEX_MAX + 1U)
#define RTI_TMR_INVALID_PRESCALER           (0U)
#define RTI_TMR_INVALID_CAPTURE_EVT         (RTI_TMR_CAPTURE_EVT_MAX + 1U)

#define RTI_TMR_CLK_SRC_DEF                 RTI_TMR_CLK_SRC_COUNTER
#define RTI_TMR_NTU_DEF                     RTI_TMR_NTU_0
#define RTI_TMR_PERIOD_DEF                  (1000U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static int32_t RTITmrConfigAPITest(uint32_t baseAddr,
                                   rtiTmrConfig_t *pConfig);
static int32_t RTITmrCaptureAPITest(uint32_t baseAddr);
static int32_t RTITmrCompareAPITest(uint32_t baseAddr);
static int32_t RTITmrAPITest(uint32_t baseAddr, rtiTmrConfig_t *pConfig);
static int32_t RTITmrNegTest(uint32_t baseAddr);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int32_t st_rtiTmrCodeCoverage_main(st_RTITmrTestcaseParams_t *testParams)
{
    uint32_t baseAddr;
    rtiTmrConfig_t tmrConfig;
    int32_t  retVal = CSL_PASS;

    /* Latch to Configure RTI parameters */
    baseAddr = testParams->rtiTmrConfigParams.rtiModule;
    tmrConfig.cntBlkIndex = testParams->rtiTmrConfigParams.rtiCntBlk;
    tmrConfig.cmpBlkIndex = testParams->rtiTmrConfigParams.rtiCmpBlk;
    tmrConfig.prescaler = testParams->rtiTmrConfigParams.rtiPrescaler;
    tmrConfig.period = testParams->rtiTmrConfigParams.rtiPrescaler;
    tmrConfig.clkSrc = RTI_TMR_CLK_SRC_DEF;
    tmrConfig.ntu    = RTI_TMR_NTU_DEF;

    /* Now Go through all the API tests to make sure the APIs have code coverage done */

    /* 1. RTI Timer configure test */
    retVal = RTITmrConfigAPITest(baseAddr, &tmrConfig);

    /* Proceed to next API test */
    if (retVal == CSL_PASS)
    {
        retVal = RTITmrCaptureAPITest(baseAddr);
    }

    /* Proceed to next API test */
    if (retVal == CSL_PASS)
    {
        retVal = RTITmrCompareAPITest(baseAddr);
    }

    /* Proceed to next API test */
    if (retVal == CSL_PASS)
    {
        retVal = RTITmrAPITest(baseAddr, &tmrConfig);
    }

    /* Proceed to next API test */
    if (retVal == CSL_PASS)
    {
        retVal = RTITmrNegTest(baseAddr);
    }

    return retVal;
}

static int32_t RTITmrConfigAPITest(uint32_t baseAddr, rtiTmrConfig_t *pConfig)
{
    uint32_t    index;
    rtiTmrConfig_t cfg;
    uint32_t ntu[4] = {RTI_TMR_NTU_0, RTI_TMR_NTU_1, RTI_TMR_NTU_2, RTI_TMR_NTU_3};

    /* Verify all error cases */
    /* Check with null base addr and expect CSL_FAIL */
    if (RTITmrConfigure(RTI_TMR_INVALID_BASE_ADDR, pConfig) != CSL_EFAIL)
    {
        UART_printf("RTITmrConfigure: failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with null config pointer and expect CSL_FAIL */
    if (RTITmrConfigure(baseAddr, NULL) != CSL_EFAIL)
    {
        UART_printf("RTITmrConfigure: failure on line no. %d with null pointer\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with invalid counter block and expect CSL_FAIL */
    cfg = *pConfig;
    cfg.cntBlkIndex = RTI_TMR_INVALID_CNT_BLK_INDEX;
    if (RTITmrConfigure(baseAddr, &cfg) != CSL_EFAIL)
    {
        UART_printf("RTITmrConfigure: failure on line no. %d with invalid counter block \n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with invalid compare block and expect CSL_FAIL */
    cfg = *pConfig;
    cfg.cmpBlkIndex = RTI_TMR_INVALID_CMP_BLK_INDEX;
    if (RTITmrConfigure(baseAddr, &cfg) != CSL_EFAIL)
    {
        UART_printf("RTITmrConfigure: failure on line no. %d with invalid compare block \n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with invalid prescaler and expect CSL_FAIL */
    cfg = *pConfig;
    cfg.prescaler = RTI_TMR_INVALID_PRESCALER;
    if (RTITmrConfigure(baseAddr, &cfg) != CSL_EFAIL)
    {
        UART_printf("RTITmrConfigure: failure on line no. %d with invalid prescale \n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Verify valid configurations */
    /* Check with all valid counter blocks */
    for (index = 0; index <= RTI_TMR_CNT_BLK_INDEX_MAX; index++)
    {
        cfg = *pConfig;
        cfg.cntBlkIndex = index;
        if (RTITmrConfigure(baseAddr, &cfg) != CSL_PASS)
        {
            UART_printf("RTITmrConfigure: failure on line no. %d with counter block %u \n", __LINE__, index);
            return (CSL_EFAIL);
        }
    }

    /* Check with all valid compare blocks */
    for (index = 0; index <= RTI_TMR_CMP_BLK_INDEX_MAX; index++)
    {
        cfg = *pConfig;
        cfg.cmpBlkIndex = index;
        if (RTITmrConfigure(baseAddr, &cfg) != CSL_PASS)
        {
            UART_printf("RTITmrConfigure: failure on line no. %d with compare block %u \n", __LINE__, index);
            return (CSL_EFAIL);
        }
    }

    /* Check with all valid NTU with counter block 0 */
    cfg = *pConfig;
    cfg.cntBlkIndex = RTI_TMR_CNT_BLK_INDEX_0;
    for (index = 0; index <= RTI_TMR_CMP_BLK_INDEX_MAX; index++)
    {
        cfg.ntu = ntu[index];
        if (RTITmrConfigure(baseAddr, &cfg) != CSL_PASS)
        {
            UART_printf("RTITmrConfigure: failure on line no. %d with NTU %u \n", __LINE__, ntu[index]);
            return (CSL_EFAIL);
        }
    }

    /* Return the API test restuls */
    UART_printf("\n RTI Timer Configure API Test : pass \n");
    return (CSL_PASS);
}

static int32_t RTITmrCompareAPITest(uint32_t baseAddr)
{
    uint32_t index;
    rtiTmrCmpCtrl_t  ctrl = {RTI_TMR_CNT_BLK_INDEX_0,  /* Counter block */
                             RTI_TMR_PERIOD_DEF,       /* init value */
                             RTI_TMR_PERIOD_DEF};      /* period */

    /* Verify all error cases */
    /* Check with null base addr and expect CSL_FAIL */
    if (RTITmrCompareConfig(RTI_TMR_INVALID_BASE_ADDR, RTI_TMR_CMP_BLK_INDEX_0, &ctrl) != CSL_EFAIL)
    {
        UART_printf("RTITmrCompareConfig: failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with invalid compare block and expect CSL_FAIL */
    if (RTITmrCompareConfig(baseAddr, RTI_TMR_INVALID_CMP_BLK_INDEX, &ctrl) != CSL_EFAIL)
    {
        UART_printf("RTITmrCompareConfig: failure on line no. %d with invalid compare block\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with null ctrl pointer and expect CSL_FAIL */
    if (RTITmrCompareConfig(baseAddr, RTI_TMR_CMP_BLK_INDEX_0, NULL) != CSL_EFAIL)
    {
        UART_printf("RTITmrCompareConfig: failure on line no. %d with null control pointer\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with invalid counter block and expect CSL_FAIL */
    ctrl.cntBlkIndex = RTI_TMR_INVALID_CNT_BLK_INDEX;
    if (RTITmrCompareConfig(baseAddr, RTI_TMR_CMP_BLK_INDEX_0, &ctrl) != CSL_EFAIL)
    {
        UART_printf("RTITmrCompareConfig: failure on line no. %d with invalid counter block\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with null base addr and expect 0 */
    if (RTITmrCompareGet(RTI_TMR_INVALID_BASE_ADDR, RTI_TMR_CMP_BLK_INDEX_0) != 0U)
    {
        UART_printf("RTITmrCompareGet: failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with invalid compare block and expect 0 */
    if (RTITmrCompareGet(baseAddr, RTI_TMR_INVALID_CMP_BLK_INDEX) != 0U)
    {
        UART_printf("RTITmrCompareGet: failure on line no. %d with invalid compare block\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check all valid configurations */
    /* Check with all valid counter blocks */
    for (index = 0; index <= RTI_TMR_CNT_BLK_INDEX_MAX; index++)
    {
        ctrl.cntBlkIndex = index;
        if (RTITmrCompareConfig(baseAddr, RTI_TMR_CMP_BLK_INDEX_0, &ctrl) != CSL_PASS)
        {
            UART_printf("RTITmrCompareConfig: failure on line no. %d with counter block %u \n", __LINE__, index);
            return (CSL_EFAIL);
        }
    }

    /* Check with all valid compare blocks */
    for (index = 0; index <= RTI_TMR_CMP_BLK_INDEX_MAX; index++)
    {
        if (RTITmrCompareConfig(baseAddr, index, &ctrl) != CSL_PASS)
        {
            UART_printf("RTITmrCompareConfig: failure on line no. %d with compare block %u \n", __LINE__, index);
            return (CSL_EFAIL);
        }
        (void) RTITmrCompareGet(baseAddr, index);
    }

    /* Return the API test restuls */
    UART_printf("\n RTI Timer Compare API Test : pass \n");
    return (CSL_PASS);
}

static int32_t RTITmrCaptureAPITest(uint32_t baseAddr)
{
    uint32_t   index, event;
    rtiTmrCnts_t cnts;
    rtiTmrCapCtrl_t  ctrl = {RTI_TMR_CAPTURE_EVT_0,  /* Source event of counter 0 */
                             RTI_TMR_CAPTURE_EVT_0}; /* Source event of counter 1 */

    /* Verify all error cases */
    /* Check with null base addr and expect CSL_FAIL */
    if (RTITmrCaptureConfig(RTI_TMR_INVALID_BASE_ADDR, &ctrl) != CSL_EFAIL)
    {
        UART_printf("RTITmrCaptureConfig failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with null ctrl pointer and expect CSL_FAIL */
    if (RTITmrCaptureConfig(baseAddr, NULL) != CSL_EFAIL)
    {
        UART_printf("RTITmrCaptureConfig: failure on line no. %d with null control pointer\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with invalid source event and expect CSL_FAIL */
    ctrl.capSrcCnt0 = RTI_TMR_INVALID_CAPTURE_EVT;
    if (RTITmrCaptureConfig(baseAddr, &ctrl) != CSL_EFAIL)
    {
        UART_printf("RTITmrCaptureConfig: failure on line no. %d with invalid source event\n", __LINE__);
        return (CSL_EFAIL);
    }

    ctrl.capSrcCnt0 = RTI_TMR_CAPTURE_EVT_0;
    ctrl.capSrcCnt1 = RTI_TMR_INVALID_CAPTURE_EVT;
    if (RTITmrCaptureConfig(baseAddr, &ctrl) != CSL_EFAIL)
    {
        UART_printf("RTITmrCaptureConfig: failure on line no. %d with invalid source event\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with null base addr and expect CSL_FAIL */
    if (RTITmrCaptureCounterGet(RTI_TMR_INVALID_BASE_ADDR, RTI_TMR_CNT_BLK_INDEX_0, &cnts) != CSL_EFAIL)
    {
        UART_printf("RTITmrCaptureCounterGet failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with invalid counter block and expect CSL_FAIL */
    if (RTITmrCaptureCounterGet(baseAddr, RTI_TMR_INVALID_CNT_BLK_INDEX, &cnts) != CSL_EFAIL)
    {
        UART_printf("RTITmrCaptureCounterGet failure on line no. %d with invalid counter block\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with null counter pointer and expect CSL_FAIL */
    if (RTITmrCaptureCounterGet(baseAddr, RTI_TMR_CNT_BLK_INDEX_0, NULL) != CSL_EFAIL)
    {
        UART_printf("RTITmrCaptureCounterGet failure on line no. %d with null cou nter pointer\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check valid operations */
    /* check valid source events */
    for (event = RTI_TMR_CAPTURE_EVT_0; event <= RTI_TMR_CAPTURE_EVT_MAX; event++)
    {
        ctrl.capSrcCnt0 = event;
        ctrl.capSrcCnt1 = (event == RTI_TMR_CAPTURE_EVT_0)? RTI_TMR_CAPTURE_EVT_1:
                                                            RTI_TMR_CAPTURE_EVT_0;
        if (RTITmrCaptureConfig(baseAddr, &ctrl) != CSL_PASS)
        {
            UART_printf("RTITmrCaptureConfig: failure on line no. %d with source event (%u, %u)\n",
                        __LINE__, ctrl.capSrcCnt0, ctrl.capSrcCnt1);
            return (CSL_EFAIL);
        }
    }

    /* Check all vaild counter blocks */
    for (index = RTI_TMR_CNT_BLK_INDEX_0; index <= RTI_TMR_CNT_BLK_INDEX_MAX; index++)
    {
        if (RTITmrCaptureCounterGet(baseAddr, index, &cnts) != CSL_PASS)
        {
            UART_printf("RTITmrCaptureCounterGet failure on line no. %d with index = %u\n", __LINE__, index);
            return (CSL_EFAIL);
        }
    }

    /* Return the API test restuls */
    UART_printf("\n RTI Timer Capture API Test : pass \n");
    return (CSL_PASS);
}

static int32_t RTITmrAPITest(uint32_t baseAddr, rtiTmrConfig_t *pConfig)
{

    rtiTmrCnts_t cnts;
    uint32_t flags = RTI_TMR_INT_REQ_ALL;
    uint32_t status, index;

    /* Check valid operations of other APIs */
    /* Clear counter API */
    for(index = RTI_TMR_CNT_BLK_INDEX_0; index <= RTI_TMR_CNT_BLK_INDEX_MAX; index++)
    {
        if (RTITmrClear(baseAddr, index, RTI_TMR_CMP_BLK_INDEX_0) != CSL_PASS)
        {
            UART_printf("RTITmrClear failure on line no. %d with counter index = %u\n", __LINE__, index);
            return (CSL_EFAIL);
        }
    }

    for(index = RTI_TMR_CMP_BLK_INDEX_0; index <= RTI_TMR_CMP_BLK_INDEX_MAX; index++)
    {
        if (RTITmrClear(baseAddr, RTI_TMR_CNT_BLK_INDEX_0, index) != CSL_PASS)
        {
            UART_printf("RTITmrClear failure on line no. %d with compare block index = %u\n", __LINE__, index);
            return (CSL_EFAIL);
        }
    }

    /* Test specific configuration */
    if (RTITmrConfigure(baseAddr, pConfig) != CSL_PASS)
    {
        UART_printf("RTITmrConfigure: failure on line no. %d\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Configure the stall mode */
    if (RTIGcSetStallMode(baseAddr, RTI_GC_STALL_MODE_ON) != CSL_PASS)
    {
        UART_printf("RTIGcSetStallMode: failure on line no. %d\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Enable */
    for(index = RTI_TMR_CNT_BLK_INDEX_0; index <= RTI_TMR_CNT_BLK_INDEX_MAX; index++)
    {
        if (RTITmrEnable(baseAddr, index) != CSL_PASS)
        {
            UART_printf("RTITmrEnable failure on line no. %d with counter index = %u\n", __LINE__, index);
            return (CSL_EFAIL);
        }
    }

    /* Get Counter */
    for(index = RTI_TMR_CNT_BLK_INDEX_0; index <= RTI_TMR_CNT_BLK_INDEX_MAX; index++)
    {
        if (RTITmrCounterGet(baseAddr, index, &cnts) != CSL_PASS)
        {
            UART_printf("RTITmrCounterGet failure on line no. %d with counter index = %u\n", __LINE__, index);
            return (CSL_EFAIL);
        }
    }

    /* Interrupt Enable */
    if (RTITmrIntEnable(baseAddr, flags) != CSL_PASS)
    {
        UART_printf("RTITmrIntEnable failure on line no. %d with flags = 0x%x\n", __LINE__, flags);
        return (CSL_EFAIL);
    }

    /* Interrupt Sttaus query and clear */
    status = RTITmrIntStatusGet(baseAddr);
    if (RTITmrIntStatusClear(baseAddr, status) != CSL_PASS)
    {
        UART_printf("RTITmrIntStatusClear failure on line no. %d with flags = 0x%x\n", __LINE__, status);
        return (CSL_EFAIL);
    }

    /* Interrupt Disable */
    if (RTITmrIntDisable(baseAddr, flags) != CSL_PASS)
    {
        UART_printf("RTITmrIntDisable failure on line no. %d with flags = 0x%x\n", __LINE__, flags);
        return (CSL_EFAIL);
    }

    /* Disable */
    for(index = RTI_TMR_CNT_BLK_INDEX_0; index <= RTI_TMR_CNT_BLK_INDEX_MAX; index++)
    {
        if (RTITmrDisable(baseAddr, index) != CSL_PASS)
        {
            UART_printf("RTITmrDisable failure on line no. %d with counter index = %u\n", __LINE__, index);
            return (CSL_EFAIL);
        }
    }

    UART_printf("\n RTI Timer API Test : pass \n");
    return (CSL_PASS);
}

static int32_t RTITmrNegTest(uint32_t baseAddr)
{
    rtiTmrCnts_t cnts;
    uint32_t flags = RTI_TMR_INT_REQ_ALL;

    /* Check valid operations of other APIs */
    /* Clear counter API */
    /* Check with null base addr and expect CSL_FAIL */
    if (RTITmrClear(RTI_TMR_INVALID_BASE_ADDR, RTI_TMR_CNT_BLK_INDEX_0, RTI_TMR_CMP_BLK_INDEX_0) != CSL_EFAIL)
    {
        UART_printf("RTITmrClear: failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with invalid counter block and expect CSL_FAIL */
    if (RTITmrClear(baseAddr, RTI_TMR_INVALID_CNT_BLK_INDEX, RTI_TMR_CMP_BLK_INDEX_1) != CSL_EFAIL)
    {
        UART_printf("RTITmrClear: failure on line no. %d with invalid counter block\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with invalid compare block and expect CSL_FAIL */
    if (RTITmrClear(baseAddr, RTI_TMR_CNT_BLK_INDEX_0, RTI_TMR_INVALID_CMP_BLK_INDEX) != CSL_EFAIL)
    {
        UART_printf("RTITmrClear: failure on line no. %d with invalid compare block\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Configure the stall mode */
    /* Check with null base addr and expect CSL_FAIL */
    if (RTIGcSetStallMode(RTI_TMR_INVALID_BASE_ADDR, RTI_GC_STALL_MODE_ON) != CSL_EFAIL)
    {
        UART_printf("RTIGcSetStallMode: failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Enable */
    /* Check with null base addr and expect CSL_FAIL */
    if (RTITmrEnable(RTI_TMR_INVALID_BASE_ADDR, RTI_TMR_CNT_BLK_INDEX_0) != CSL_EFAIL)
    {
        UART_printf("RTITmrEnable: failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with invalid counter block and expect CSL_FAIL */
    if (RTITmrEnable(baseAddr, RTI_TMR_INVALID_CNT_BLK_INDEX) != CSL_EFAIL)
    {
        UART_printf("RTITmrEnable: failure on line no. %d with invalid counter block\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Get Counter */
    /* Check with null base addr and expect CSL_FAIL */
    if (RTITmrCounterGet(RTI_TMR_INVALID_BASE_ADDR, RTI_TMR_CNT_BLK_INDEX_0, &cnts) != CSL_EFAIL)
    {
        UART_printf("RTITmrCounterGet: failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with invalid counter block and expect CSL_FAIL */
    if (RTITmrCounterGet(baseAddr, RTI_TMR_INVALID_CNT_BLK_INDEX, &cnts) != CSL_EFAIL)
    {
        UART_printf("RTITmrCounterGet: failure on line no. %d with invalid counter block\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with null cnts pointer and expect CSL_FAIL */
    if (RTITmrCounterGet(baseAddr, RTI_TMR_CNT_BLK_INDEX_0, NULL) != CSL_EFAIL)
    {
        UART_printf("RTITmrCounterGet: failure on line no. %d with null cnts pointer\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Interrupt Enable */
    /* Check with null base addr and expect CSL_FAIL */
    if (RTITmrIntEnable(RTI_TMR_INVALID_BASE_ADDR, flags) != CSL_EFAIL)
    {
        UART_printf("RTITmrIntEnable: failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Interrupt Sttaus query  and clear */
    /* Check with null base addr and expect 0 */
    if (RTITmrIntStatusGet(RTI_TMR_INVALID_BASE_ADDR) != 0U)
    {
        UART_printf("RTITmrIntStatusGet: failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with null base addr and expect CSL_FAIL */
    if (RTITmrIntStatusClear(RTI_TMR_INVALID_BASE_ADDR, flags) != CSL_EFAIL)
    {
        UART_printf("RTITmrIntClear: failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Interrupt Disable */
    /* Check with null base addr and expect CSL_FAIL */
    if (RTITmrIntDisable(RTI_TMR_INVALID_BASE_ADDR, flags) != CSL_EFAIL)
    {
        UART_printf("RTITmrIntDisable: failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Disable */
    /* Check with null base addr and expect CSL_FAIL */
    if (RTITmrDisable(RTI_TMR_INVALID_BASE_ADDR, RTI_TMR_CNT_BLK_INDEX_0) != CSL_EFAIL)
    {
        UART_printf("RTITmrDisable: failure on line no. %d with invalid base address\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Check with invalid counter block and expect CSL_FAIL */
    if (RTITmrDisable(baseAddr, RTI_TMR_INVALID_CNT_BLK_INDEX) != CSL_EFAIL)
    {
        UART_printf("RTITmrDisable: failure on line no. %d with invalid counter block\n", __LINE__);
        return (CSL_EFAIL);
    }

    UART_printf("\n RTI Timer Negatove Test : pass \n");

    return (CSL_PASS);

}

/********************************* End of file ******************************/
