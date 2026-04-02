/*
 *  Copyright (c) Texas Instruments Incorporated 2024-2025
 *  All rights reserved.
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
 *  \file main.c
 *
 *  \brief File containing main function implementation for csl_ddr_ecc_test_app
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/osal/CacheP.h>

#include <ti/board/board.h>

#include "ecc_ddr.h"
#include <sdl_esm.h>
#include <osal_interface.h>

#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define BOARD_INIT_WAIT_DELAY        1000

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile uint32_t *gTest_Addr    = NULL;

volatile uint32_t gSecTestPass;
volatile uint32_t gDedTestPass;

int32_t gDdrEccTestResult = CSL_EFAIL;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* Unity functions */
void test_csl_ddr_ecc_test_app_runner(void);
void test_csl_ddr_ecc_test_app (void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/*
 *  ======== Unity set up and tear down ========
 */
void setUp(void)
{
    /* Do nothing */
}

void tearDown(void)
{
    /* Do nothing */
}

int32_t SDL_ESM_applicationCallbackFunction(SDL_ESM_Inst esmInst,
                                            SDL_ESM_IntType esmIntrType,
                                            uint32_t grpChannel,
                                            uint32_t index,
                                            uint32_t intSrc,
                                            uintptr_t *arg)
{

    int32_t retVal = 0;
    volatile uint32_t testVal = 0;

    UART_printf("\n  ESM Call back function called : instType 0x%x, intType 0x%x, " \
                "intSrc 0x%x \n",
                esmInst, esmIntrType, intSrc);
    UART_printf("  Take action \n");
    if(esmIntrType == 1u)
    {
        UART_printf(" High Priority Interrupt Executed\n");
    }
    else
    {
        UART_printf(" Low Priority Interrupt Executed\n");
    }

    if (intSrc == SDLR_ESM0_ESM_LVL_EVENT_DDR0_DDRSS_DRAM_ECC_CORR_ERR_LVL_0)
    {
        gSecTestPass = TRUE;
        /* Clear Specific ECC error */
        retVal = CSL_emifClearECCError((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE,
                                        CSL_EMIF_ECC_ERROR_TYPE_SINGLE_BIT);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCErrors  failed ");
        }

        /* Clear ECC interupt bits */
        retVal = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE,
                                                    CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                    | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus Failed ");
        }
    }
    else if (intSrc == SDLR_ESM0_ESM_LVL_EVENT_DDR1_DDRSS_DRAM_ECC_CORR_ERR_LVL_0)
    {
        gSecTestPass = TRUE;
        /* Clear Specific ECC error */
        retVal = CSL_emifClearECCError((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE,
                                        CSL_EMIF_ECC_ERROR_TYPE_SINGLE_BIT);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCErrors  failed ");
        }

        /* Clear ECC interupt bits */
        retVal = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE,
                                                    CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                    | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus Failed ");
        }
    }
#if defined (SOC_J784S4)
    else if (intSrc == SDLR_ESM0_ESM_LVL_EVENT_DDR2_DDRSS_DRAM_ECC_CORR_ERR_LVL_0)
    {
        gSecTestPass = TRUE;
        /* Clear Specific ECC error */
        retVal = CSL_emifClearECCError((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE,
                                        CSL_EMIF_ECC_ERROR_TYPE_SINGLE_BIT);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCErrors  failed ");
        }

        /* Clear ECC interupt bits */
        retVal = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE,
                                                    CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                    | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus Failed ");
        }
    }
    else if (intSrc == SDLR_ESM0_ESM_LVL_EVENT_DDR3_DDRSS_DRAM_ECC_CORR_ERR_LVL_0)
    {
        gSecTestPass = TRUE;
        /* Clear Specific ECC error */
        retVal = CSL_emifClearECCError((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE,
                                        CSL_EMIF_ECC_ERROR_TYPE_SINGLE_BIT);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCErrors  failed ");
        }

        /* Clear ECC interupt bits */
        retVal = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE,
                                                    CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                    | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus Failed ");
        }
    }
#endif
    else if (intSrc == SDLR_ESM0_ESM_LVL_EVENT_DDR0_DDRSS_DRAM_ECC_UNCORR_ERR_LVL_0)
    {
        gDedTestPass = TRUE;

        /* This section corrects the ECC error simulated */
        /* In a real application the user must take necessary corrective action */
        /******************************************************************/

        /* Temporarily disable ECC */
        DDRDisableECC((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);

        /* Now replace location with original value as 2b errors are not corrected */
        *(gTest_Addr) = testVal;

        /* Write back any pending writes */
        CacheP_wbInv((const void *)gTest_Addr, 4);

        /* Enable back ECC */
        DDREnableECC((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);

        /******************************************************************/
        /* Clear specific error */
        retVal = CSL_emifClearECCError((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE,
                                        CSL_EMIF_ECC_ERROR_TYPE_DOUBLE_BIT);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCErrors  failed ");
        }

        /* Clear ECC interrupt bits */
        retVal = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE,
                                                    CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus Failed ");
        }
    }
    else if (intSrc == SDLR_ESM0_ESM_LVL_EVENT_DDR1_DDRSS_DRAM_ECC_UNCORR_ERR_LVL_0)
    {
        gDedTestPass = TRUE;

        /* This section corrects the ECC error simulated */
        /* In a real application the user must take necessary corrective action */
        /******************************************************************/

        /* Temporarily disable ECC */
        DDRDisableECC((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);

        /* Now replace location with original value as 2b errors are not corrected */
        *(gTest_Addr) = testVal;
    
        /* Write back any pending writes */
        CacheP_wbInv((const void *)gTest_Addr, 4);

        /* Enable back ECC */
        DDREnableECC((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);

        /******************************************************************/
        /* Clear specific error */
        retVal = CSL_emifClearECCError((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE,
                                        CSL_EMIF_ECC_ERROR_TYPE_DOUBLE_BIT);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCErrors  failed ");
        }

        /* Clear ECC interrupt bits */
        retVal = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE,
                                                    CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus Failed ");
        }
    }
#if defined (SOC_J784S4)
    else if (intSrc == SDLR_ESM0_ESM_LVL_EVENT_DDR2_DDRSS_DRAM_ECC_UNCORR_ERR_LVL_0)
    {
        gDedTestPass = TRUE;

        /* This section corrects the ECC error simulated */
        /* In a real application the user must take necessary corrective action */
        /******************************************************************/

        /* Temporarily disable ECC */
        DDRDisableECC((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);

        /* Now replace location with original value as 2b errors are not corrected */
        *(gTest_Addr) = testVal;

        /* Write back any pending writes */
        CacheP_wbInv((const void *)gTest_Addr, 4);

        /* Enable back ECC */
        DDREnableECC((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);

        /******************************************************************/
        /* Clear specific error */
        retVal = CSL_emifClearECCError((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE,
                                        CSL_EMIF_ECC_ERROR_TYPE_DOUBLE_BIT);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCErrors  failed ");
        }

        /* Clear ECC interrupt bits */
        retVal = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE,
                                                    CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus Failed ");
        }
    }
    else if (intSrc == SDLR_ESM0_ESM_LVL_EVENT_DDR3_DDRSS_DRAM_ECC_UNCORR_ERR_LVL_0)
    {
        gDedTestPass = TRUE;

        /* This section corrects the ECC error simulated */
        /* In a real application the user must take necessary corrective action */
        /******************************************************************/

        /* Temporarily disable ECC */
        DDRDisableECC((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);

        /* Now replace location with original value as 2b errors are not corrected */
        *(gTest_Addr) = testVal;

        /* Write back any pending writes */
        CacheP_wbInv((const void *)gTest_Addr, 4);

        /* Enable back ECC */
        DDREnableECC((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);

        /******************************************************************/
        /* Clear specific error */
        retVal = CSL_emifClearECCError((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE,
                                        CSL_EMIF_ECC_ERROR_TYPE_DOUBLE_BIT);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCErrors  failed ");
        }

        /* Clear ECC interrupt bits */
        retVal = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE,
                                                    CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (retVal != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus Failed ");
        }
    }
#endif
    SDL_ESM_clrNError(esmInst);
    return retVal;
}

static int32_t sdlApp_osalInit(void)
{
    SDL_ErrType_t ret = SDL_PASS;

    ret = SDL_TEST_osalInit();
    if (ret != SDL_PASS)
    {
        UART_printf("Error: Init Failed\n");
    }

    return ret;
}

void test_csl_ddr_ecc_test_app(void)
{

    gDdrEccTestResult = DDREccTest();
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, gDdrEccTestResult);
    if (gDdrEccTestResult != CSL_PASS)
    {
        UART_printf("\r\n DDR ECC test failed \n");
    }
    return;
}

void test_csl_ddr_ecc_test_app_runner(void)
{
    UNITY_BEGIN();
    /* DDR ECC test for R5F */
    RUN_TEST (test_csl_ddr_ecc_test_app);

    UNITY_END();
    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
    return;
}

int main (void)
{
    Board_initCfg     boardCfg;
    Board_STATUS  status = BOARD_SOK;

    boardCfg = BOARD_INIT_MODULE_CLOCK | BOARD_INIT_UART_STDIO |BOARD_INIT_PINMUX_CONFIG;
    status = Board_init(boardCfg);
    TEST_ASSERT_EQUAL_INT32(BOARD_SOK, status);
    if (status != BOARD_SOK)
    {
        UART_printf("\n Board Init failed \n");
        return 0;
    }

    UART_printf("\n Board init complete \n");
    
    sdlApp_osalInit();

    test_csl_ddr_ecc_test_app_runner();
    if (gDdrEccTestResult == CSL_PASS)
    {
        {
           UART_printf("\r\nAll tests have passed.");
        }
    }
    while (1);
}