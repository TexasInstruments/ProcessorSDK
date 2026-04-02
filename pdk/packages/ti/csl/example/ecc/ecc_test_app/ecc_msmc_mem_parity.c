/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2020
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
#include <stdint.h>
#include <ti/csl/example/utils/uart_console/inc/uartConfig.h>

#include <ti/osal/osal.h>
#include "ecc_msmc.h"
#include "ecc_msmc_mem_parity.h"

#include <ti/drv/uart/UART_stdio.h>
#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

/* ----------------- Constant definitions ----------------- */
volatile Bool   gMsmcMemParityInterrupt = FALSE;

/* -------------------------------------------------------- */

/* ----------------- Local Function prototypes ------------------ */
static int32_t msmcEccMemParityConfig(uint32_t eccMemType, CSL_ecc_aggrRegs *eccAggrRegs);

/* -------------------------------------------------------- */


/* ----------------- Global variables ----------------- */

/* Function configures MSMC ECC Mem Parity
 */
static int32_t msmcEccMemParityConfig(uint32_t eccMemType, CSL_ecc_aggrRegs *eccAggrRegs)
{
    int32_t    cslResult;
    CSL_ecc_aggrEnableCtrl memParityCtrl;

    /* Enable the parity ECC interrupts */
    memParityCtrl.validCfg               = CSL_ECC_AGGR_VALID_PARITY_ERR;
    memParityCtrl.intrEnableParityErr    = TRUE;
    cslResult = CSL_ecc_aggrIntrEnableCtrl(eccAggrRegs,
                            &memParityCtrl);

    if (cslResult == CSL_PASS)
    {
        /* Enable the DED ECC Interrupts, as Parity Errors
         * are always Non Correctable errors
         * and are reported on DED interrupt
         */
        cslResult = CSL_ecc_aggrEnableIntrs(eccAggrRegs,
                            CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT);
    }
    else
    {
        UART_printf("\r\nECC AGGR Parity Error Interrupt Enable failed...");
    }

    if (cslResult != CSL_PASS)
    {
        UART_printf("\r\nECC AGGR Double Interrupt Enable failed...");
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
        TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslResult);
#endif

    if (cslResult == CSL_PASS)
    {
        /* Enable the Single ECC Interrupts as well to track
         */
        cslResult = CSL_ecc_aggrEnableIntrs(eccAggrRegs,
                            CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT);
    }
    else
    {
        UART_printf("\r\nECC AGGR Parity Error Interrupt Enable failed...");
    }

    if (cslResult != CSL_PASS)
    {
        UART_printf("\r\nECC AGGR Single Interrupt Enable failed...");
    }
#if defined (UNITY_INCLUDE_CONFIG_H)
            TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslResult);
#endif

    return (cslResult);
}


/* -----------Function executes memory parity ECC Test ----------- */
int32_t msmcEccMemParityTest(void)
{
    int32_t                 retVal = CSL_PASS;
    bool                    ratRetVal;
    CSL_ecc_aggrRegs        *pEccAggrRegs;
    CSL_Ecc_AggrEDCInterconnectErrorInfo forceErr;
    uint32_t                ramId = CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID;
    uint32_t                firstBitLocation = MSMC_ECC_ERR_BIT_1;
    CSL_esm_app_R5_cfg      cfg;
    CSL_RatTranslationCfgInfo translationCfg;
    CSL_Ecc_AggrEDCInterconnectErrorStatusInfo errStatusInfo;
    uint32_t                maxTimeOutMilliSeconds = 3000;

    gMsmcMemParityInterrupt = FALSE;
    UART_printf("\r\n\r\n**** MSMC Memory Parity TEST ****");

    /* Add RAT configuration to access address > 32bit address range */
    translationCfg.translatedAddress = CSL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR0_BASE;
    translationCfg.sizeInBytes       = CSL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR0_SIZE;
    translationCfg.baseAddress       = (uint32_t)MSMC_PARITY_ECC_AGGR_REGION_LOCAL_BASE;

    /* Set up RAT translation */
    ratRetVal = CSL_ratConfigRegionTranslation((CSL_ratRegs *)MSMC_PARITY_ECC_AGGR_RAT_CFG_BASE,
                                            MSMC_PARITY_ECC_AGGR_RAT_REGION_INDEX, &translationCfg);
    if (ratRetVal == false) {
        retVal = CSL_EFAIL;
    }
    else
    {
        pEccAggrRegs  = (CSL_ecc_aggrRegs*) MSMC_PARITY_ECC_AGGR_REGION_LOCAL_BASE;
        retVal = CSL_PASS;
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, retVal);
#endif

    /* Config Parity Intr */
    if (retVal == CSL_PASS)
    {
        retVal = msmcEccMemParityConfig(APP_ECC_MEMTYPE_MSMC, pEccAggrRegs);
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, retVal);
#endif

    /* ESM Setup */
    if (retVal == CSL_PASS)
    {
        cfg.hi_pri_evt = MSMC_ECC_AGGR0_DED_ERR_EVENT;
        cfg.lo_pri_evt = MSMC_ECC_AGGR0_SEC_ERR_EVENT;
        retVal = cslAppEsmSetup((uint32_t) ESM_CFG_BASE, &cfg );
    }
/*
    1.  Write the ecc vector register (aggregator address 0x8) with the
        index of the safety controller endpoint, make sure trigger_read=0. 
    2.  Write to error1 register (addres 0x18) of the
        safety controller to program:
        2a.  the ecc_grp (group of checker to inject).
        2b.  The bit location that will be flipped. If the signal is N bits,
        then bit[N:N+p] is the parity bits. Bit location that can be
        injected with error is 0...N+p-1.
    3.  Write to the control register (address 0x14) of the
        safety controller to start the injection:
          3a.  Make sure ecc_check field is enabled
          3b.  Set force_se for single bit error
*/

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, retVal);
#endif

    if (retVal == CSL_PASS)
    {
        bool    error_check = true;
        retVal = CSL_ecc_aggrConfigEDCInterconnect(pEccAggrRegs, ramId, error_check);
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, retVal);
#endif

    if (retVal == CSL_PASS)
    {
        forceErr.intrSrc    = CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT;
        forceErr.eccGroup   = MSMC_ECC_PARITY_ECC_GROUP_ACCESS_SEL; /* This group covers the Parity Check for 16 Bits */
        forceErr.eccBit1    = firstBitLocation;
        forceErr.eccBit2    = firstBitLocation + 3U;
        forceErr.bNextBit   = FALSE;
        forceErr.eccPattern = CSL_ECC_AGGR_INJECT_PATTERN_A;
        retVal = CSL_ecc_aggrForceEDCInterconnectError(pEccAggrRegs,
                                                       ramId,
                                                       &forceErr);
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, retVal);
#endif

    /* Wait for parity error triggered interrupt */
    if (retVal == CSL_PASS)
    {
        UART_printf("\r\n\r\n Waiting for Parity Error Generated Interrupt ");

        do 
        {
           uint32_t timeOutCnt = 0;
            /* dummy wait for the interrupt */
            Osal_delay(10);
            timeOutCnt += 10;
            if (timeOutCnt > maxTimeOutMilliSeconds)
            {
                retVal = CSL_EFAIL;
                break;
            }
        } while (gMsmcMemParityInterrupt == FALSE);

        if (retVal == CSL_PASS)
        {
            UART_printf("\r\n\r\n  Got it");
        }
        else
        {
            UART_printf("\r\n\r\n  Timeout waiting for the interrupt");
        }

        UART_printf("\r\n\r\n Checking for the Parity Error Group ");

        if (retVal == CSL_PASS)
        {
            retVal = CSL_ecc_aggrGetEDCInterconnectErrorStatus(pEccAggrRegs,ramId,  &errStatusInfo);
#if defined (UNITY_INCLUDE_CONFIG_H)
            TEST_ASSERT_EQUAL_INT32(MSMC_ECC_PARITY_ECC_GROUP_ACCESS_SEL, errStatusInfo.eccGroup);
#endif

            if (errStatusInfo.eccGroup != MSMC_ECC_PARITY_ECC_GROUP_ACCESS_SEL)
            {
                retVal = CSL_EFAIL;
            }
            UART_printf("\r\n\r\n DONE");
        }
#if defined (UNITY_INCLUDE_CONFIG_H)
        TEST_ASSERT_EQUAL_INT32(CSL_PASS, retVal);
#endif

        UART_printf("\r\n\r\n**** MSMC Memory Parity Error Test Complete ****");
        UART_printf("\r\n\r\n");

    }

    /* Catch up check */
#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, retVal);
#endif

    return (retVal);
}

/* Nothing past this point */
