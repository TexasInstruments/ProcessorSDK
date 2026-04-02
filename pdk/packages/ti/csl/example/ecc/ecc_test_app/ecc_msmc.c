/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2019-2020
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
#include <ti/drv/uart/UART_stdio.h>

#include <ti/osal/osal.h>
#include "ecc_msmc.h"

#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

/* ----------------- Constant definitions ----------------- */


/* -------------------------------------------------------- */

/* ----------------- Function prototypes ------------------ */
static void msmcEccInit(char option);
static int32_t msmcEccConfig(uint32_t eccMemType);
static int32_t msmcSecErrTest(CSL_ecc_aggrRegs *eccAggrRegs, uint32_t ramId);
static int32_t msmcDedErrTest(CSL_ecc_aggrRegs *eccAggrRegs, uint32_t ramId);
static int32_t msmcECCInjectError(CSL_ecc_aggrRegs *pEccAggrRegs, uint32_t ramId, uint32_t err_pat,
                                  uint32_t err_chk, uint32_t isCorrectableError);
/* -------------------------------------------------------- */


/* ----------------- Global variables ----------------- */
CSL_ecc_aggrRegs * const cslAppEccAggrAddrTable[APP_ECC_AGGREGATOR_MAX_ENTRIES] =
{
    ((CSL_ecc_aggrRegs *) ((uintptr_t) CSL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR0_BASE)),
};

uint32_t const cslAppEccAggrTestRAMIdTable[APP_ECC_AGGREGATOR_MAX_ENTRIES] =
{
    CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID,
};

extern volatile uint32_t gSecTestPass;
extern volatile uint32_t gDedTestPass;
extern volatile char     gOption;


/* Function handles Emif test sub menu */
void mainSubMenuMsmcEccTest(char *pOption)
{
    char option;
    while (1)
    {
        UART_printf("\r\n\r\n**** MSMC ECC TEST ****");
        UART_printf("\r\nMenu:");
        UART_printf("\r\n1. MSMC SEC ERR ECC test");
        UART_printf("\r\n2. MSMC DED ERR ECC test");
        UART_printf("\r\nx. Exit");
        UART_printf("\r\nSelect ECC TEST : ");
#if defined (UNITY_INCLUDE_CONFIG_H)

        if (gOption == '0')
        {
            gOption = '1';
        }
        else if (gOption == '1')
        {
            gOption = '2';
        }
        else
        {
            gOption = 'x';
        }
        *pOption = gOption;
#else
        /* Get option */
        UARTConfigGets(gUartBaseAddr, pOption, 1);
#endif
        option = *pOption;
        if ( (MSMC_ECC_SEC_ERR_TEST == option) ||
             (MSMC_ECC_DED_ERR_TEST == option) ||
             (MSMC_ECC_EXIT         == option) )
        {
            break;
        }
        else
        {
            UART_printf("\r\nEnter Valid option\r\n");
        }
    }
}

void cslAppChkIsExpectedRamId(uint32_t testId)
{
    uint32_t    expectedRamId;
    CSL_ecc_aggrRegs *eccAggrRegs;
    int32_t cslRet;
    bool isPend;
    CSL_Ecc_AggrIntrSrc intrSrc;
 
    eccAggrRegs = cslAppEccAggrAddrTable[APP_ECC_MEMTYPE_MSMC];
    expectedRamId = cslAppEccAggrTestRAMIdTable[APP_ECC_MEMTYPE_MSMC];

    if (testId == MSMC_ECC_AGGR0_DED_ERR_EVENT)
    {
        intrSrc = CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT;
    }
    else if (testId == MSMC_ECC_AGGR0_SEC_ERR_EVENT)
    {
        intrSrc = CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT;
    } else
    {
        /* Unexpected test Id */
        return;
    }

    /* Check if the Interrupt caused by the RAM ID expected  */
    cslRet = CSL_ecc_aggrIsEDCInterconnectIntrPending(eccAggrRegs,
                 expectedRamId, intrSrc, &isPend);
    if (cslRet == CSL_PASS)
    {
        if (isPend)
        {
            cslRet = CSL_ecc_aggrClrEDCInterconnectNIntrPending(eccAggrRegs,
                         expectedRamId, intrSrc,
                         CSL_ECC_AGGR_ERROR_SUBTYPE_INJECT, 1U);
            if (cslRet == CSL_PASS)
            {
                if (testId == MSMC_ECC_AGGR0_DED_ERR_EVENT)
                {
                    gDedTestPass = TRUE;
                }
                else if (testId == MSMC_ECC_AGGR0_SEC_ERR_EVENT)
                {
                    gSecTestPass = TRUE;
                }              
            }
            /* Write end of interrupt */
            CSL_ecc_aggrAckIntr(eccAggrRegs, intrSrc);
        }
    }

    return;
}
/* Function executes Emif ECC Test */
int32_t msmcEccTest(void)
{
    int32_t retVal;
    char    mainSubMenuOption;
    CSL_ecc_aggrRegs *eccAggrRegs;
    uint32_t skipMsmcTest;
    uint32_t   ramId = ECC_AGGR_UNKNOWN_RAM_ID;

    gSecTestPass   = FALSE;
    gDedTestPass   = FALSE;
    gOption        = '0';

    /* initialize the ESM */
    retVal = cslAppEsmSetup((uint32_t) ESM_CFG_BASE );

    if (retVal == CSL_PASS)
    {
        skipMsmcTest = FALSE;
    }
    else
    {
        /* Skip the test as ESM setup Failed */
        skipMsmcTest = TRUE;
        UART_printf("\r\n ESM Setup Failed...");
    }


    while (1)
    {
        if (skipMsmcTest == FALSE)
        {
            mainSubMenuMsmcEccTest(&mainSubMenuOption);
            msmcEccInit(mainSubMenuOption);
        }
        else
        {
            /* select exit option */
            mainSubMenuOption = 'x';
        }

        if (MSMC_ECC_SEC_ERR_TEST == mainSubMenuOption)
        {
            ramId = CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID;
            eccAggrRegs = cslAppEccAggrAddrTable[APP_ECC_MEMTYPE_MSMC];
            retVal = msmcSecErrTest(eccAggrRegs, ramId);
        }
        else if (MSMC_ECC_DED_ERR_TEST == mainSubMenuOption)
        {
            ramId = CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID;
            eccAggrRegs = cslAppEccAggrAddrTable[APP_ECC_MEMTYPE_MSMC];
            retVal = msmcDedErrTest(eccAggrRegs, ramId);
        }
        else
        {
            UART_printf("\r\nMSMC ECC test exiting...");
            break;
        }
    }

    if (gSecTestPass)
    {
        UART_printf("\r\nMSMC ECC SEC ECC TESTS PASSED");
    }
    else
    {
        UART_printf("\r\nMSMC ECC SEC ECC TESTS FAILED");
    }

    if (gDedTestPass)
    {
        UART_printf("\r\nMSMC ECC DED ECC TESTS PASSED");
    }
    else
    {
        UART_printf("\r\nMSMC ECC DED ECC TESTS FAILED");
    }

    if ((retVal == CSL_PASS) && (gDedTestPass & gSecTestPass))
    {
        UART_printf("\r\nALL MSMC ECC TESTS PASS");
    }
    else
    {
        UART_printf("\r\nFEW MSMC ECC TESTS FAILED");
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_TRUE(gDedTestPass & gSecTestPass);
#endif

    return retVal;
}

/* Function initialized MSMC ECC */
static void msmcEccInit(char option)
{
    int32_t cslRet;

    /* Ecc config */
    cslRet = msmcEccConfig(APP_ECC_MEMTYPE_MSMC);

    if (cslRet != CSL_PASS)
    {
        UART_printf("\r\nECC Config failed...");
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
        TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslRet);
#endif

    return;
}

/* Enable ECC and */
static int32_t msmcECCInjectError(CSL_ecc_aggrRegs *pEccAggrRegs, uint32_t ramId, uint32_t err_pat,
                                  uint32_t err_chk, uint32_t isCorrectableError)
{
    int32_t    retVal;

    CSL_Ecc_AggrEDCInterconnectErrorInfo ECCForceError;

    /* Set parameters for error injection */
    if ( isCorrectableError )
    {
        ECCForceError.intrSrc = CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT;
        ECCForceError.eccBit1 = MSMC_ECC_ERR_BIT_1;
        ECCForceError.eccBit2 = 0U;
    }
    else
    {
        ECCForceError.intrSrc = CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT;
        ECCForceError.eccBit1 = MSMC_ECC_ERR_BIT_1;
        ECCForceError.eccBit2 = MSMC_ECC_ERR_BIT_2;
    }
    ECCForceError.bNextBit = FALSE;
    ECCForceError.eccGroup = 0;
    ECCForceError.eccPattern = err_pat;

    /* Call CSL API to force error with parameters */
    retVal = CSL_ecc_aggrForceEDCInterconnectError(pEccAggrRegs, ramId,
                 &ECCForceError);

    return retVal;
}



/* Function configures MSMC ECC
 * Also primes the memory area
 */
static int32_t msmcEccConfig(uint32_t eccMemType)
{
    int32_t    cslResult;
    CSL_ecc_aggrRegs *eccAggrRegs;

    eccAggrRegs = cslAppEccAggrAddrTable[APP_ECC_MEMTYPE_MSMC];

    /* Enable the single bit ECC interrupts */
    /* Note: The following statement enables interrupts for all RAMs */
    cslResult = CSL_ecc_aggrEnableIntrs(eccAggrRegs,
                            CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT);

    if (cslResult == CSL_PASS)
    {
        /* Enable the Double bit ECC Interrupts */
        cslResult = CSL_ecc_aggrEnableIntrs(eccAggrRegs,
                            CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT);
    }
    else
    {
        UART_printf("\r\nECC AGGR SINGLE BIT Error Interrupt Enable failed...");
    }

    /* Configure ECC check */
    if (cslResult == CSL_PASS)
    {
        cslResult = CSL_ecc_aggrConfigEDCInterconnect(eccAggrRegs,
                        CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID,
                        TRUE);
    }

    /* Verify configuration */
    if (cslResult == CSL_PASS)
    {
        cslResult = CSL_ecc_aggrVerifyConfigEDCInterconnect(eccAggrRegs,
                        CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID,
                        TRUE);
    }

    if (cslResult != CSL_PASS)
    {
        UART_printf("\r\nECC AGGR Configuration failed...");
    }

#if defined (UNITY_INCLUDE_CONFIG_H)
        TEST_ASSERT_EQUAL_INT32(CSL_PASS, cslResult);
#endif
    return (cslResult);
}

/* Function performs MSMC single bit error test
 * Configures ECC, Inserts single bit error
 * and waits for handler to finish
 */
static int32_t msmcSecErrTest(CSL_ecc_aggrRegs *eccAggrRegs, uint32_t ramId)
{
    int32_t  retVal = CSL_EFAIL;

    /* set the test status to false */
    gSecTestPass = FALSE;

    /* Inject SEC Error */
    retVal = msmcECCInjectError(eccAggrRegs,
                                CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID, 2u,
                                1u, 1u);

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, retVal);
#endif

    /* wait until the test passes */
    UART_printf("\r\n Waiting on SEC Interrupt...");
    while (gSecTestPass == FALSE);
    UART_printf("\r\n Got it (test pass)...");
    /* return the test status */
    return retVal;
}

/* Function performs MSMC double bit error test
 * Inserts double bit error and waits for handler to finish
 */
static int32_t msmcDedErrTest(CSL_ecc_aggrRegs *eccAggrRegs, uint32_t ramId)
{
    int32_t  retVal = CSL_EFAIL;

    gDedTestPass = FALSE;

     /* Inject the DED errors */
     retVal = msmcECCInjectError(eccAggrRegs,
                                 CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID, 2u, 1u, 0u);

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, retVal);
#endif

    /* wait until the test passes */
    UART_printf("\r\n Waiting on DED interrupt...");
    while (gDedTestPass == FALSE);
    UART_printf("\r\n Got it...(test pass) ");
    /* return the test status */
    return retVal;
}

/* Nothing past this point */
