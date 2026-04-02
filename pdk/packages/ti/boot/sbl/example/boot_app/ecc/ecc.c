/*
 *  Copyright (c) Texas Instruments Incorporated 2025
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
 *  \file     ecc.c
 *
 *  \brief    This file implements ECC task function
 */

/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/

#include"ecc.h"
#include <soc.h>
#include <ti/drv/uart/UART_stdio.h>
#include <osal/sdl_osal.h>
#include <sdl_esm.h>
#include <sdl_exception.h>
#include <sdl_arm_r5_pmu.h>
#include <test/ecc/common/ecc_test_main.h>
#include <test/r5f_startup/interrupt.h>

#if !defined(SOC_J721S2) && !defined(SOC_J784S4)
#include <ti/csl/csl_serdes_pcie.h>
#include <ti/csl/cslr_pcie.h>
#endif

#if defined (SOC_J721E)
#include "test/ecc/ecc_sdl_funcTest/soc/j721e/ecc_func.h"
#include <ti/csl/soc/j721e/src/cslr_soc_ctrl_mmr.h>
#include <ti/drv/sciclient/soc/sysfw/include/j721e/tisci_devices.h>
#endif /* SOC_J721E */

#if defined (SOC_J7200)
#include "test/ecc/ecc_sdl_funcTest/soc/j7200/ecc_func.h"
#include <ti/csl/soc/j7200/src/cslr_soc_ctrl_mmr.h>
#include <ti/drv/sciclient/soc/sysfw/include/j7200/tisci_devices.h>
#endif /* SOC_J7200 */

#if defined (SOC_J721S2)
#include <ti/drv/sciclient/soc/sysfw/include/j721s2/tisci_devices.h>
#include <include/soc/j721s2/sdlr_mcu_r5fss0_baseaddress.h>
#include <src/sdl/ecc/soc/j721s2/sdl_ecc_soc.h>
#endif /* SOC_J721S2 */

#if defined (SOC_J784S4)
#include <ti/drv/sciclient/soc/sysfw/include/j784s4/tisci_devices.h>
#include <include/soc/j784s4/sdlr_mcu_r5fss0_baseaddress.h>
#include <src/sdl/ecc/soc/j784s4/sdl_ecc_soc.h>
#endif /* SOC_J784S4 */

#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

#include <ti/csl/csl_cbass.h>

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

#define ATCM_BASE_ADDR                    (0x00000000U)
#define ATCM_SIZE                         (0x00008000U)
#define TEST_AREA_SIZE                    (0x00000400U)
#define ATCM_FILL_PATTERN                 (0xffffffffU)
#define FIELDVALUE                        (0x00000001U)
#define VECTOR_SIZE                       (0x100U)

/* delay for 1us*/
#define DELAY                             (0x00000001U)

/* define the unlock and lock values */
#define KICK0_UNLOCK_VAL                  (0x68EF3490U)
#define KICK1_UNLOCK_VAL                  (0xD172BC5AU)
#define KICK_LOCK_VAL                     (0x00000000U)
#define MAIN_MMR_BASE_ADDRESS             CSL_CTRL_MMR0_CFG0_BASE
#define MCU_MMR_BASE_ADDRESS              CSL_MCU_CTRL_MMR0_CFG0_BASE
#define WKUP_MMR_BASE_ADDRESS             CSL_WKUP_CTRL_MMR0_CFG0_BASE
#define MAIN_PLL_BASE_ADDRESS             CSL_PLL0_CFG_BASE
#define MCU_PLL_BASE_ADDRESS              CSL_MCU_PLL0_CFG_BASE
#define SERDES_DIAG_TEST_REF_CLOCK        CSL_SERDES_REF_CLOCK_100M /* Ref clock of serdes */
#define SERDES_DIAG_TEST_SSC_MODE         CSL_SERDES_NO_SSC
#define SERDES_DIAG_TEST_LANE_MASK        (0x3U) /* All lanes are set */
#define SERDES_DIAG_TEST_PHY_TYPE         CSL_SERDES_PHY_TYPE_PCIe /* For running PCIe tests */
#define SERDES_DIAG_TEST_PRBS_PATTERN     SERDES_PRBS_7 /* prbs7 pattern */
#define SERDES_DIAG_TEST_LANE_RATE        CSL_SERDES_LANE_FULL_RATE /* Set to run at full rate of the link rate */
#define SERDES_DIAG_TEST_LOOPBACK_MODE    CSL_SERDES_LOOPBACK_DISABLED /* For internal near end serial loopback tests */
#define SERDES_DIAG_TEST_OPERATING_MODE   CSL_SERDES_FUNCTIONAL_MODE /* Should always be set to Diagnostic Mode for BER, EYE and PRBS tests */
#define SERDES_DIAG_TEST_FORCEATTBOOST    CSL_SERDES_FORCE_ATT_BOOST_DISABLED
#define SERDES_DIAG_TEST_REF_CLOCK_SRC    CSL_SERDES_REF_CLOCK_INT /* Internally sourced ref clk */
#define SERDES_DIAG_TEST_NUM_LANES        (0x2U) /* Number of lanes to be tested */
#define SERDES_DIAG_TEST_LINK_RATE        CSL_SERDES_LINK_RATE_5G
#define SERDES_DIAG_TEST_PCIE_GEN_TYPE    SERDES_DIAG_PCIE_GEN3 /* Generation to be tested */
#define SBL_MCU1_CPU0_FREQ_HZ             (1000000000)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile bool esmError = false;
SDL_ESM_config ECC_Test_esmInitConfig_MCU =
{
    .esmErrorConfig = {0u, 3u}, /* Self test error config */
    .enableBitmap = {0xffffffffu, 0xfe3fffffu, 0x7fffffffu, 0x00000007u,
                },
     /**< All events enable: except timer and self test  events, and Main ESM output */
    /* Temporarily disabling vim compare error as well*/
    .priorityBitmap = {0xffffffffu, 0xff3fffffu, 0x7fffffffu, 0x00000007u,
                        },
    /**< All events high priority: except timer, selftest error events, and Main ESM output */
    .errorpinBitmap = {0xffffffffu, 0xff3fffffu, 0x7fffffffu, 0x00000007u,
                      },
    /**< All events high priority: except timer, selftest error events, and Main ESM output */
};

SDL_ESM_config ECC_Test_esmInitConfig_WKUP =
{
    .esmErrorConfig = {0u, 8u}, /* Self test error config */
    .enableBitmap = {0xffffffffu, 0x00180003u, 0xffffffffu,
                },
     /**< All events enable: except clkstop events for unused clocks */
    .priorityBitmap = {0xffffffffu, 0x00180003u, 0xffffffffu,
                        },
    /**< All events high priority: except clkstop events for unused clocks */
    .errorpinBitmap = {0xffffffffu, 0x00180003u, 0xffffffffu,
                      },
    /**< All events high priority: except clkstop for unused clocks
     *   and selftest error events */
};

#if defined (SOC_J721E) || defined (SOC_J7200)
SDL_ESM_config ECC_Test_esmInitConfig_MAIN =
{
    .esmErrorConfig = {1u, 8u}, /* Self test error config */
    .enableBitmap = {0x00000000u, 0xfffffffbu, 0x7fffffffu, 0xffffffffu,
                 0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                 0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                 0xffffffffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00003c0fu,
                 0xffffffffu,
                },
     /**< All events enable: except clkstop events for unused clocks
      *   and PCIE events */
    .priorityBitmap = {0x00000000u, 0xfffffffbu, 0x7fffffffu, 0xffffffffu,
                         0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                         0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                         0xffffffffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0xffffffffu,
                        },
    /**< All events high priority: except clkstop events for unused clocks
     *   and PCIE events */
    .errorpinBitmap = {0x00000000u, 0xfffffffbu, 0x7fffffffu, 0xffffffffu,
                       0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                       0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                       0xffffffffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0xffffffffu,
                      },
    /**< All events high priority: except clkstop for unused clocks
     *   and PCIE events */
};
#endif

#if defined (SOC_J721S2)
SDL_ESM_config ECC_Test_esmInitConfig_MAIN =
{
    .esmErrorConfig = {1u, 8u}, /* Self test error config */
    .enableBitmap = {0x00000000u, 0xffffffcbu, 0x7fffffffu, 0xffffffffu,
                 0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                 0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                 0xffffffffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00003c0fu,
                },
     /**< All events enable: except clkstop events for unused clocks
      *   and PCIE events */
    .priorityBitmap = {0x00000000u, 0xfffffffbu, 0x7fffffffu, 0xffffffffu,
                         0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                         0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                         0xffffffffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00003c0fu,
                        },
    /**< All events high priority: except clkstop events for unused clocks
     *   and PCIE events */
    .errorpinBitmap = {0x00000000u, 0xffffffdbu, 0x7fffffffu, 0xffffffffu,
                       0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                       0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                       0xffffffffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00003c0fu,
                      },
    /**< All events high priority: except clkstop for unused clocks
     *   and PCIE events */
};

#endif

#if defined (SOC_J784S4)
SDL_ESM_config ECC_Test_esmInitConfig_MAIN =
{
    .esmErrorConfig = {1u, 8u}, /* Self test error config */
    .enableBitmap = {0x00000000u, 0xffffffdbu, 0x7fffffffu, 0xffffffffu,
                 0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                 0xffffffffu, 0xfffffffbu, 0xffffffffu, 0xffffffffu,
                 0xffff7fffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00003c0fu, 0x0000000fu,
                },
     /**< All events enable: except clkstop events for unused clocks
      *   and PCIE events */
    .priorityBitmap = {0x00000000u, 0xffffffdbu, 0x7fffffffu, 0xffffffffu,
                         0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                         0xffffffffu, 0xfffffffbu, 0xffffffffu, 0xffffffffu,
                         0xffff7fffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00003c0fu, 0x0000000au,
                        },
    /**< All events high priority: except clkstop events for unused clocks
     *   and PCIE events */
    .errorpinBitmap = {0x00000000u, 0xffffffdbu, 0x7fffffffu, 0xffffffffu,
                       0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                       0xffffffffu, 0xfffffffbu, 0xffffffffu, 0xffffffffu,
                       0xffff7fffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00003c0fu, 0x0000000au,
                      },
    /**< All events high priority: except clkstop for unused clocks
     *   and PCIE events */
};
#endif

/* If this test is for PSRAM starting at address 0x0, we need special
 * translation using RAT in order to access it for the self test. This is because,
 * in the application, R5F view of 0x0 is mapped to ATCM memory. This
 * flag in the test app allows the address translation function defined the test's
 * OSAL implementation to perform the RAT translation when the address is 0x0 for
 * PSRAM testing */
extern bool sdl_ecc_psram_test;
#if defined(SOC_J721E)
uint32_t sdl_ecc_psram_mem = SDL_ECC_MEMTYPE_MCU_PSRAM0;
#elif defined(SOC_J7200)
uint32_t sdl_ecc_psram_mem = SDL_ECC_PSRAM2KECC0_ECC_AGGR;
#elif defined(SOC_J721S2)
uint32_t sdl_ecc_psram_mem = SDL_PSRAM2KECC0_ECC_AGGR;
#elif defined(SOC_J784S4)
uint32_t sdl_ecc_psram_mem = SDL_PSRAM2KECC0_ECC_AGGR;
#endif

/* This is the list of exception handle and the parameters */
const SDL_R5ExptnHandlers ECC_Test_R5ExptnHandlers =
{
    .udefExptnHandler = &SDL_EXCEPTION_undefInstructionExptnHandler,
    .swiExptnHandler = &SDL_EXCEPTION_swIntrExptnHandler,
    .pabtExptnHandler = &SDL_EXCEPTION_prefetchAbortExptnHandler,
    .dabtExptnHandler = &SDL_EXCEPTION_dataAbortExptnHandler,
    .irqExptnHandler = &SDL_EXCEPTION_irqExptnHandler,
    .fiqExptnHandler = &SDL_EXCEPTION_fiqExptnHandler,
    .udefExptnHandlerArgs = ((void *)0u),
    .swiExptnHandlerArgs = ((void *)0u),
    .pabtExptnHandlerArgs = ((void *)0u),
    .dabtExptnHandlerArgs = ((void *)0u),
    .irqExptnHandlerArgs = ((void *)0u),
};

static uint32_t arg;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

extern int32_t BootApp_osalWrapper(void);

/* ========================================================================== */
/*                 Internal Function Definitions                              */
/* ========================================================================== */

int32_t ECC_Memory_init (void);
void ECC_Test_VIMDEDInterruptHandler(void);

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

void ECC_Test_undefInstructionExptnCallback(void)
{
    UART_printf("\n Undefined Instruction exception");
}

void ECC_Test_swIntrExptnCallback(void)
{
    UART_printf("\n Software interrupt exception");
}

void ECC_Test_prefetchAbortExptnCallback(void)
{
    UART_printf("\n Prefetch Abort exception");
}
void ECC_Test_dataAbortExptnCallback(void)
{
    UART_printf("\n Data Abort exception");
}
void ECC_Test_irqExptnCallback(void)
{
    UART_printf("\n Irq exception");
}

void ECC_Test_fiqExptnCallback(void)
{
    UART_printf("\n Fiq exception");
}

int32_t SDL_ESM_applicationCallbackFunction(SDL_ESM_Inst esmInst,
                                            SDL_ESM_IntType esmIntrType,
                                            uint32_t grpChannel,
                                            uint32_t index,
                                            uint32_t intSrc,
                                            uintptr_t *arg)
{

    SDL_ECC_MemType eccmemtype;
    SDL_Ecc_AggrIntrSrc eccIntrSrc;
    SDL_ECC_ErrorInfo_t eccErrorInfo;
    int32_t retVal;


    UART_printf("\n  ESM Call back function called : instType 0x%x, intType 0x%x, " \
                "grpChannel 0x%x, index 0x%x, intSrc 0x%x \n",
                esmInst, esmIntrType, grpChannel, index, intSrc);
    UART_printf("  Take action \n");
    if(esmIntrType == 1u){
        UART_printf(" High Priority Interrupt Executed\n");
    }
    else{
        UART_printf(" Low Priority Interrupt Executed\n");
    }
    retVal = SDL_ECC_getESMErrorInfo(esmInst, intSrc, &eccmemtype, &eccIntrSrc);
    if (retVal != SDL_PASS)
    {
        UART_printf("SDL_ECC_getESMErrorInfo failed\n");
    }
    /* Any additional customer specific actions can be added here */
    retVal = SDL_ECC_getErrorInfo(eccmemtype, eccIntrSrc, &eccErrorInfo);

    if (retVal != SDL_PASS)
    {
        UART_printf("SDL_ECC_getErrorInfo failed\n");
    }
    UART_printf("\n  ECC Error Call back function called : eccMemType %d, errorSrc 0x%x, " \
               "ramId %d, bitErrorOffset 0x%04x%04x, bitErrorGroup %d\n",
               eccmemtype, eccIntrSrc, eccErrorInfo.memSubType, (uint32_t)(eccErrorInfo.bitErrorOffset >> 32),
               (uint32_t)(eccErrorInfo.bitErrorOffset & 0x00000000FFFFFFFF), eccErrorInfo.bitErrorGroup);

    if (eccErrorInfo.injectBitErrCnt != 0)
    {
        SDL_ECC_clearNIntrPending(eccmemtype, eccErrorInfo.memSubType, eccIntrSrc, SDL_ECC_AGGR_ERROR_SUBTYPE_INJECT, eccErrorInfo.injectBitErrCnt);
    }
    else
    {
        SDL_ECC_clearNIntrPending(eccmemtype, eccErrorInfo.memSubType, eccIntrSrc, SDL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, eccErrorInfo.bitErrCnt);
    }

    retVal = SDL_ECC_ackIntr(eccmemtype, eccIntrSrc);

    esmError = true;

    return retVal;
}

void ECC_Test_exceptionInit(void)
{

    SDL_EXCEPTION_CallbackFunctions_t exceptionCallbackFunctions =
            {
             .udefExptnCallback = ECC_Test_undefInstructionExptnCallback,
             .swiExptnCallback = ECC_Test_swIntrExptnCallback,
             .pabtExptnCallback = ECC_Test_prefetchAbortExptnCallback,
             .dabtExptnCallback = ECC_Test_dataAbortExptnCallback,
             .irqExptnCallback = ECC_Test_irqExptnCallback,
             .fiqExptnCallback = ECC_Test_fiqExptnCallback,
            };

    /* Initialize SDL exception handler */
    SDL_EXCEPTION_init(&exceptionCallbackFunctions);
    /* Register SDL exception handler */
    Intc_RegisterExptnHandlers(&ECC_Test_R5ExptnHandlers);

    return;
}

int32_t eccInject_onlyTest(uint32_t mainMem, uint32_t subType, uint32_t chkGrp )
{
    SDL_ErrType_t result;
    int32_t retVal=0;
    uint32_t maxTimeOutMilliSeconds = 100000;
    uint32_t timeOutCnt = 0;

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;

    memset(&injectErrorConfig, 0, sizeof(injectErrorConfig));


    /* Run one shot test for MSMC 2 bit error */
    /* Note the address is relative to start of ram */
    injectErrorConfig.pErrMem = (uint32_t *)(0u);

    injectErrorConfig.flipBitMask = 0x1;
    injectErrorConfig.chkGrp = chkGrp;

    result = SDL_ECC_injectError(mainMem,
                                subType,
                                SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE,
                                &injectErrorConfig);

    if (result != SDL_PASS ) {
        UART_printf("\n ecc_aggr_test inject test: mainMem %d: fixed location test failed,Interconnect type RAM id = %d, checker group = %d\n",
        mainMem, subType, chkGrp);
        retVal = -1;
    }
    if (result == SDL_PASS)
    {
        UART_printf("\n\n Waiting for ESM Interrupt \n\n");
        do
            {
                /* dummy wait for the interrupt */
                SDL_OSAL_delay(DELAY);
                timeOutCnt += 10;
                if (timeOutCnt > maxTimeOutMilliSeconds)
                {
                    result = SDL_EFAIL;
                    break;
                }
            } while (esmError == false);
    }
    if(result == SDL_PASS){
        UART_printf("\n\n got ESM Interrupt \n\n");
        esmError = false;
    }

    if (result != SDL_PASS) {
        UART_printf("    ...skipped because this is Inject Only type\n");
        result = SDL_PASS;
    }

    return retVal;
}

/*********************************************************************
* @fn      ECC_Memory_init
*
* @brief   Initializes Software Diagostics Test Framework
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t ECC_Memory_init (void)
{
    int32_t retValue=0;
    void *ptr = (void *)&arg;
    SDL_ErrType_t result;

    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    /* Initialise exception handler */
    ECC_Test_exceptionInit();

    UART_printf("\nECC_Test_init: Exception init complete \n");

    if (retValue == 0) {
        /* Initialize MCU ESM module */
        result = SDL_ESM_init(SDL_ESM_INST_MCU_ESM0, &ECC_Test_esmInitConfig_MCU, (SDL_ESM_applicationCallback) SDL_ESM_applicationCallbackFunction, ptr);
        if (result != SDL_PASS) {
            /* print error and quit */
             UART_printf("ECC_Memory_init: Error initializing MCU ESM: result = %d\n", result);

            retValue = -1;
        }
    }

    if (retValue == 0) {
        /* Initialize WKUP ESM module */
        result = SDL_ESM_init(SDL_ESM_INST_WKUP_ESM0, &ECC_Test_esmInitConfig_WKUP,(SDL_ESM_applicationCallback) SDL_ESM_applicationCallbackFunction,ptr);
        if (result != SDL_PASS) {
            /* print error and quit */
             UART_printf("ECC_Memory_init: Error initializing WKUP ESM: result = %d\n", result);

            retValue = -1;
        }
    }

    if (retValue == 0) {
        /* Initialize MAIN ESM module */
        result = SDL_ESM_init(SDL_ESM_INST_MAIN_ESM0, &ECC_Test_esmInitConfig_MAIN,(SDL_ESM_applicationCallback) SDL_ESM_applicationCallbackFunction,ptr);
        if (result != SDL_PASS) {
            /* print error and quit */
            UART_printf("ECC_Memory_init: Error initializing MAIN ESM: result = %d\n", result);

            retValue = -1;
        }
    }

    if (retValue == 0) {
        /* Initialize ECC callbacks within the MCU ESM */
        result = SDL_ECC_initEsm(SDL_ESM_INST_MCU_ESM0);
        if (result != SDL_PASS) {
            /* print error and quit */
             UART_printf("ECC_Memory_init: Error initializing ECC callback for MCU ESM: result = %d\n", result);

            retValue = -1;
        }
    }

    if (retValue == 0) {
        /* Initialize ECC callbacks within the Main ESM */
        result = SDL_ECC_initEsm(SDL_ESM_INST_MAIN_ESM0);
        if (result != SDL_PASS) {
            /* print error and quit */
             UART_printf("ECC_Memory_init: Error initializing ECC callback for Main ESM: result = %d\n", result);

            retValue = -1;
        }
    }

    if (retValue == 0) {
        /* Initialize ECC callbacks within the WKUP ESM */
        result = SDL_ECC_initEsm(SDL_ESM_INST_WKUP_ESM0);
        if (result != SDL_PASS) {
            /* print error and quit */
             UART_printf("ECC_Memory_init: Error initializing ECC callback for WKUP ESM: result = %d\n", result);

            retValue = -1;
        }
    }

    if (retValue == 0) {
        /* Initialize VIM ECC memory ; This is specifically for VIM in lockstep*/
        /* All other memories are auto initialized by hardware */
        result = SDL_ECC_initMemory(SDL_ECC_MEMTYPE_MCU_R5F0_CORE, SDL_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID);
        if (result != SDL_PASS) {
             /* print error and quit */
              UART_printf("ECC_Memory_init: Error initializing ECC memory: retValue = %d\n", retValue);
             retValue = -1;;
         }
    }

    return retValue;
}
static uint32_t BootApp_getTimeInMicroSec(uint32_t pmuCntrVal)
{
    uint64_t mcu_clk_freq = SBL_MCU1_CPU0_FREQ_HZ;
    uint32_t cycles_per_usec = (mcu_clk_freq / 1000000);
    return (pmuCntrVal/cycles_per_usec);
}

int32_t SDL_dummyWaitForInterrupt (uint32_t maxTimeOutMilliSeconds)
{
    SDL_ErrType_t result = SDL_PASS;
    uint32_t timeOutCnt = 0;
    do
    {
        /* dummy wait for the interrupt */
        SDL_OSAL_delay(DELAY);
        timeOutCnt += 10;
        if (timeOutCnt > maxTimeOutMilliSeconds)
        {
            result = SDL_EFAIL;
            break;
        }
    } while (esmError == false);
    
    return result;
}

/********************************************************************
 * @fn      ecc_aggr_test
 *
 * @brief   Execute ECC single bit error self test on ecc_aggr_test ECC aggregator
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t ecc_aggr_test(void)
{

    SDL_ErrType_t result;
    int32_t retVal=0;
    uint32_t j,i,k,mainMem;
    SDL_ecc_aggrRegs *pEccAggr;
    uint32_t maxTimeOutMilliSeconds = 3000;
    uint32_t timeOutCnt = 0;
    SDL_ECC_InjectErrorType    intsrc;
    uint32_t errSrc;
    uint32_t errorInjectionErrSrc;
    bool ecc_test_status=true;

#ifdef DEBUG
    int32_t selectedIndex = -1;
    bool exit = (bool)false;
#endif

    SDL_ECC_InjectErrorConfig_t injectErrorConfig;
#ifdef DEBUG
    while (exit != (bool)true)
    {
    	UART_printf("\n Enter number between range 0 to %d",SDL_ECC_MEMTYPE_MAX);
        UART_printf("\n Select the memory to test...");
        UART_scanFmt("%d", &selectedIndex);
        ecc_test_status=true;
        if (selectedIndex == -1)
        {
            UART_printf("\n Exiting the test on request \n");
            exit = (bool)true;
            continue;
        }
        UART_printf("...selected %d\n", selectedIndex);
        if (selectedIndex >= SDL_ECC_MEMTYPE_MAX)
        {
            UART_printf("\necc_aggr_test: selection [%d] is not a valid memory id\n", selectedIndex);
            continue;
        }
#endif

        memset(&injectErrorConfig, 0, sizeof(injectErrorConfig));
        /* Run one shot test for ecc_aggr_test 1 bit error */
        /* Note the address is relative to start of ram */
        injectErrorConfig.pErrMem = (uint32_t *)(0u);

        injectErrorConfig.flipBitMask = 0x3;
#ifdef DEBUG
        mainMem = selectedIndex;
#else
        for (mainMem = SDL_ECC_MEMTYPE_MCU_R5F0_CORE; mainMem < SDL_ECC_MEMTYPE_MAX; mainMem++)
        {
            static uint64_t gTimeStart, gTimeFinish;
#if defined (SOC_J721S2)
            /*
             * The following ECC aggregators have been excluded from the ECC test:
             * 1. SDL Known issues: SDL_PCIE1_0_ECC_AGGR, SDL_PCIE1_1_ECC_AGGR, SDL_WKUP_SMS0_TIFS_ECC_AGGR_0_ECC_AGGR,SDL_WKUP_SMS0_HSM_ECC_AGGR_0_ECC_AGGR, SDL_DSS_EDP0_0_ECC_AGGR, SDL_DSS_EDP0_1_ECC_AGGR, SDL_CSI_TX_IF_V2_1_0_ECC_AGGR, SDL_CSI_TX_IF_V2_1_1_ECC_AGGR, SDL_CSI_TX_IF_V2_0_0_ECC_AGGR, SDL_CSI_TX_IF_V2_0_1_ECC_AGGR
             * 2. SDL_ECC_MEMTYPE_MCU_R5F1_CORE can be tested when an application is loaded and executed on MCU0_1, please refer to Documentation for more details.
             * 3. SDL_ECC_MEMTYPE_A72_1, SDL_ECC_MEMTYPE_A72_0, SDL_ECC_MEMTYPE_A72_COREPAC are known issues with QNX Boot, can be tested without HLOS Boot. 
             */
            if(mainMem == SDL_ECC_MEMTYPE_MCU_R5F1_CORE  || mainMem == SDL_ECC_MEMTYPE_A72_1 || mainMem == SDL_ECC_MEMTYPE_A72_0 || mainMem == SDL_ECC_MEMTYPE_A72_COREPAC || mainMem == SDL_PCIE1_0_ECC_AGGR || mainMem == SDL_PCIE1_1_ECC_AGGR || mainMem == SDL_WKUP_SMS0_TIFS_ECC_AGGR_0_ECC_AGGR || mainMem == SDL_WKUP_SMS0_HSM_ECC_AGGR_0_ECC_AGGR || mainMem == SDL_DSS_EDP0_0_ECC_AGGR || mainMem == SDL_DSS_EDP0_1_ECC_AGGR || mainMem == SDL_CSI_TX_IF_V2_1_0_ECC_AGGR || mainMem == SDL_CSI_TX_IF_V2_1_1_ECC_AGGR || mainMem == SDL_CSI_TX_IF_V2_0_0_ECC_AGGR || mainMem == SDL_CSI_TX_IF_V2_0_1_ECC_AGGR )
            {
                continue;
            }    
#endif  
#if defined (SOC_J784S4)
            /*
             * The following ECC aggregators have been excluded from the ECC test:
             * 1. SDL Known issues: SDL_PCIE0_0_ECC_AGGR, SDL_PCIE0_1_ECC_AGGR, SDL_PCIE3_0_ECC_AGGR,SDL_PCIE3_1_ECC_AGGR, SDL_PCIE2_0_ECC_AGGR, SDL_PCIE2_1_ECC_AGGR, SDL_WKUP_SMS0_TIFS_ECC_AGGR_0_ECC_AGGR, SDL_WKUP_SMS0_HSM_ECC_AGGR_0_ECC_AGGR, SDL_PCIE1_0_ECC_AGGR, SDL_PCIE1_1_ECC_AGGR, SDL_MCU_SA3_SS0_0_ECC_AGGR, SDL_DSS_EDP0_0_ECC_AGGR,SDL_CSI_TX_IF_V2_1_1_ECC_AGGR, SDL_CSI_TX_IF_V2_0_1_ECC_AGGR
             * 2. SDL_ECC_MEMTYPE_MCU_R5F1_CORE and SDL_R5FSS2_1_ECC_AGGR can be tested when an application is loaded and executed on MCU0_1 and MCU2_1, please refer to Documentation for more details.
             * 3. SDL_COMPUTE_CLUSTER0_CFG0_ARM_CORE0_ECC_AGGR, SDL_COMPUTE_CLUSTER0_CFG0_ARM_CORE1_ECC_AGGR, SDL_COMPUTE_CLUSTER0_CFG0_ARM_CORE2_ECC_AGGR, SDL_COMPUTE_CLUSTER0_CFG0_ARM_CORE3_ECC_AGGR, SDL_COMPUTE_CLUSTER0_CFG1_ARM_COREPAC_ECC_AGGR, SDL_COMPUTE_CLUSTER0_CFG1_ARM_CORE0_ECC_AGGR, SDL_COMPUTE_CLUSTER0_CFG1_ARM_CORE1_ECC_AGGR, SDL_COMPUTE_CLUSTER0_CFG1_ARM_CORE2_ECC_AGGR and SDL_COMPUTE_CLUSTER0_CFG1_ARM_CORE3_ECC_AGGR are known issues with QNX Boot, can be tested without HLOS Boot.
             * 4. SDL_COMPUTE_CLUSTER0_CFG0_ARM_COREPAC_ECC_AGGR and SDL_ECC_MEMTYPE_MCU_R5F0_CORE are pending debug. 
             */
            if(mainMem == SDL_ECC_MEMTYPE_MCU_R5F0_CORE || mainMem == SDL_ECC_MEMTYPE_MCU_R5F1_CORE || mainMem == SDL_PCIE0_0_ECC_AGGR || mainMem == SDL_PCIE0_1_ECC_AGGR || mainMem == SDL_PCIE3_0_ECC_AGGR || mainMem == SDL_PCIE3_1_ECC_AGGR || mainMem == SDL_PCIE2_0_ECC_AGGR || mainMem == SDL_PCIE2_1_ECC_AGGR || mainMem == SDL_WKUP_SMS0_TIFS_ECC_AGGR_0_ECC_AGGR || mainMem == SDL_WKUP_SMS0_HSM_ECC_AGGR_0_ECC_AGGR || mainMem == SDL_PCIE1_0_ECC_AGGR || mainMem == SDL_PCIE1_1_ECC_AGGR || mainMem == SDL_MCU_SA3_SS0_0_ECC_AGGR || mainMem == SDL_DSS_EDP0_0_ECC_AGGR || mainMem == SDL_CSI_TX_IF_V2_1_1_ECC_AGGR || mainMem == SDL_CSI_TX_IF_V2_0_1_ECC_AGGR || mainMem == SDL_COMPUTE_CLUSTER0_CFG0_ARM_COREPAC_ECC_AGGR || mainMem == SDL_COMPUTE_CLUSTER0_CFG0_ARM_CORE0_ECC_AGGR || mainMem == SDL_COMPUTE_CLUSTER0_CFG0_ARM_CORE1_ECC_AGGR || mainMem == SDL_COMPUTE_CLUSTER0_CFG0_ARM_CORE2_ECC_AGGR || mainMem == SDL_COMPUTE_CLUSTER0_CFG0_ARM_CORE3_ECC_AGGR || mainMem == SDL_COMPUTE_CLUSTER0_CFG1_ARM_COREPAC_ECC_AGGR || mainMem == SDL_COMPUTE_CLUSTER0_CFG1_ARM_CORE0_ECC_AGGR || mainMem == SDL_COMPUTE_CLUSTER0_CFG1_ARM_CORE1_ECC_AGGR || mainMem == SDL_COMPUTE_CLUSTER0_CFG1_ARM_CORE2_ECC_AGGR || mainMem == SDL_COMPUTE_CLUSTER0_CFG1_ARM_CORE3_ECC_AGGR)
            {
                continue;
            }    
#endif  
#endif
#ifndef DEBUG
            if (mainMem >= SDL_ECC_MEMTYPE_MAX)
            {
                UART_printf("\necc_aggr_test: selection [%d] is not a valid memory id\n", mainMem);
                continue;
            }
            else if (ECC_Test_config[mainMem].initConfig == NULL)
            {
                UART_printf("\necc_aggr_test: [%d] Skipping %s due to missing information\n", mainMem, ECC_Test_config[mainMem].aggrName);
                continue;
            }
#endif
            UART_printf("\n ecc_aggrtest: [%d]  self test: %s starting \n\n", mainMem, ECC_Test_config[mainMem].aggrName);
            /* Sub memory list  entered in the for loop for perticular mem type*/
            gTimeStart = BootApp_getTimeInMicroSec(SDL_R5PMU_readCntr(SDL_ARM_R5_PMU_CYCLE_COUNTER_NUM));

            /* Call any device init that is specific to the selected aggregator (ones that cannot be called during the generic init) */
            ECC_Test_lateDeviceInit(mainMem);

            if (ECC_Test_config[mainMem].initConfig != NULL)
            {

                /* Initialize ECC */
                result = SDL_ECC_init(mainMem, ECC_Test_config[mainMem].initConfig);
                if (result != SDL_PASS)
                {
                    /* print error and quit */
                    UART_printf("\nECC_Memory_init: [%d] Error initializing %s: result = %d\n", mainMem, ECC_Test_config[mainMem].aggrName, result);

                    result = -1;
                }
                else
                {
                    UART_printf("\nECC_Memory_init: [%d] %s ECC Init complete \n", mainMem, ECC_Test_config[mainMem].aggrName);
                }
            }
            else
            {
                UART_printf("\nECC_Memory_init: [%d] Skipping %s due to missing data\n", mainMem, ECC_Test_config[mainMem].aggrName);
            }

            for (i=0; i< SDL_ECC_aggrTable[mainMem].numRams; i++)
            {
#if defined (SOC_J721E)
                if (((mainMem == SDL_ECC_MEMTYPE_MCU_R5F0_CORE) && (i == SDL_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID)) ||
                    ((mainMem == SDL_R5FSS0_CORE0_ECC_AGGR) && (i == SDL_R5FSS0_CORE0_ECC_AGGR_CPU0_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_ECC_MEMTYPE_MCU_R5F1_CORE) && (i == SDL_MCU_R5FSS0_CORE1_ECC_AGGR_CPU1_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_R5FSS0_CORE1_ECC_AGGR) && (i == SDL_R5FSS0_CORE1_ECC_AGGR_CPU1_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_R5FSS1_CORE0_ECC_AGGR) && (i == SDL_R5FSS1_CORE0_ECC_AGGR_CPU0_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_R5FSS1_CORE1_ECC_AGGR) && (i == SDL_R5FSS1_CORE1_ECC_AGGR_CPU1_KS_VIM_RAMECC_RAM_ID)))
#elif (SOC_J7200)
                if (((mainMem == SDL_ECC_MEMTYPE_MCU_R5F0_CORE) && (i == SDL_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID)) ||
                    ((mainMem == SDL_R5FSS0_CORE0_ECC_AGGR) && (i == SDL_R5FSS0_CPU0_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_ECC_MEMTYPE_MCU_R5F1_CORE) && (i == SDL_MCU_R5FSS0_CPU1_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_R5FSS0_CORE1_ECC_AGGR) && (i == SDL_R5FSS0_CPU1_KS_VIM_RAMECC_RAM_ID)) )
#elif (SOC_J721S2)
                if (((mainMem == SDL_ECC_MEMTYPE_MCU_R5F0_CORE) && (i == SDL_MCU_R5FSS0_PULSAR_SL_CPU0_ECC_AGGR_CPU0_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_ECC_MEMTYPE_MCU_R5F1_CORE) && (i == SDL_MCU_R5FSS0_PULSAR_SL_CPU1_ECC_AGGR_CPU1_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_R5FSS0_0_ECC_AGGR) && (i == SDL_R5FSS0_PULSAR_SL_CPU0_ECC_AGGR_CPU0_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_R5FSS0_1_ECC_AGGR) && (i == SDL_R5FSS0_PULSAR_SL_CPU1_ECC_AGGR_CPU1_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_R5FSS1_0_ECC_AGGR) && (i == SDL_R5FSS1_PULSAR_SL_CPU0_ECC_AGGR_CPU0_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_R5FSS1_1_ECC_AGGR) && (i == SDL_R5FSS1_PULSAR_SL_CPU1_ECC_AGGR_CPU1_KS_VIM_RAMECC_RAM_ID)))
#elif (SOC_J784S4)
                if (((mainMem == SDL_ECC_MEMTYPE_MCU_R5F0_CORE) && (i == SDL_MCU_R5FSS0_PULSAR_SL_CPU0_ECC_AGGR_CPU0_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_ECC_MEMTYPE_MCU_R5F1_CORE) && (i == SDL_MCU_R5FSS0_PULSAR_SL_CPU1_ECC_AGGR_CPU1_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_R5FSS0_0_ECC_AGGR) && (i == SDL_R5FSS0_PULSAR_SL_CPU0_ECC_AGGR_CPU0_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_R5FSS0_1_ECC_AGGR) && (i == SDL_R5FSS0_PULSAR_SL_CPU1_ECC_AGGR_CPU1_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_R5FSS1_0_ECC_AGGR) && (i == SDL_R5FSS1_PULSAR_SL_CPU0_ECC_AGGR_CPU0_KS_VIM_RAMECC_RAM_ID)) ||
                    ((mainMem == SDL_R5FSS1_1_ECC_AGGR) && (i == SDL_R5FSS1_PULSAR_SL_CPU1_ECC_AGGR_CPU1_KS_VIM_RAMECC_RAM_ID)))
#endif
                {
                    UART_printf("skipping VIM because it is destructive\n");
                }
#if defined (SOC_J721S2)
                else if ((mainMem == SDL_ECC_MEMTYPE_MAIN_MSMC_AGGR0) && (i == SDL_COMPUTE_CLUSTER0_COMPUTE_CLUSTER_J7AE_MSMC_EN_DRU_MSMC_EN_4MB_WRAP_MSMC_4MB_MSMC_WRAP_ECC_AGGR0_EN_MSMC_P0_MMR_FW_EDC_CTL_RAM_ID))
                {
                    UART_printf("skipping MSMC endpoint 85 - it fails\n");
                }
#endif
                else if ((SDL_ECC_aggrTable[mainMem].ramTable[i].RAMId) != SDL_ECC_RAMID_INVALID)
                {
                    if ((SDL_ECC_aggrTable[mainMem].ramTable[i].ramIdType) != SDL_ECC_AGGR_ECC_TYPE_ECC_WRAPPER)
                    {
                        UART_printf("self test started RamId %d  starting \n",i );

                        /* This for loop provide interconnect checkers group */
                        for (j=0; j< SDL_ECC_aggrTable[mainMem].ramTable[i].numCheckers; j++)
                        {
                            injectErrorConfig.chkGrp = j;
                            /* Below mainMem are third parties ECC Aggregator, It can be tested by creating traffics.
                            SDL supports inject only because out of scope for SDL implementation.  */
#if defined (SOC_J721E)
                            if( (mainMem == SDL_CSI_RX_IF0_ECC_AGGR_0) || (mainMem == SDL_CSI_RX_IF1_ECC_AGGR_0 ) ||
                                (mainMem == SDL_CSI_TX_IF0_CSI_TX_IF_ECC_AGGR) || (mainMem == SDL_CSI_TX_IF0_CSI_TX_IF_ECC_AGGR_BYTE) ||
                                (mainMem == SDL_DSS_EDP0_K3_DSS_EDP_MHDPTX_WRAPPER_ECC_AGGR_PHY) || (mainMem == SDL_DSS_EDP0_K3_DSS_EDP_MHDPTX_WRAPPER_ECC_AGGR_DSC) ||
                                (mainMem == SDL_DSS_EDP0_K3_DSS_EDP_MHDPTX_WRAPPER_ECC_AGGR_CORE) || (mainMem == SDL_DSS_DSI0_K3_DSS_DSI_TOP_ECC_AGGR_SYS) ||
                                (mainMem == SDL_I3C0_I3C_P_ECC_AGGR) || (mainMem == SDL_MCU_I3C0_I3C_P_ECC_AGGR) || (mainMem == SDL_MCU_I3C1_I3C_P_ECC_AGGR))
#elif (SOC_J7200)
                            if( (mainMem == SDL_ECC_MCU_I3C0_0_ECC_AGGR) || (mainMem == SDL_ECC_MCU_I3C0_1_ECC_AGGR)
#elif (SOC_J721S2)
                            if(0)
#elif (SOC_J784S4)
							if( (mainMem == SDL_DSS_DSI0_ECC_AGGR) || (mainMem == SDL_DSS_EDP0_0_ECC_AGGR) || (mainMem == SDL_DSS_EDP0_2_ECC_AGGR) ||
								(mainMem == SDL_DSS_DSI1_ECC_AGGR) || (mainMem == SDL_PCIE0_0_ECC_AGGR) || (mainMem == SDL_PCIE0_1_ECC_AGGR)||
								(mainMem == SDL_PCIE3_0_ECC_AGGR) || (mainMem == SDL_PCIE3_1_ECC_AGGR) || (mainMem == SDL_PCIE2_0_ECC_AGGR) ||
								(mainMem == SDL_PCIE2_1_ECC_AGGR) || (mainMem == SDL_PCIE1_0_ECC_AGGR) || (mainMem == SDL_PCIE1_1_ECC_AGGR) ||
								(mainMem == SDL_CSI_RX_IF2_ECC_AGGR) ||
                                (mainMem == SDL_CSI_RX_IF1_ECC_AGGR) || (mainMem == SDL_CSI_RX_IF0_ECC_AGGR) || (mainMem == SDL_CSI_TX_IF_V2_1_0_ECC_AGGR) ||
                                (mainMem == SDL_CSI_TX_IF_V2_1_1_ECC_AGGR) || (mainMem == SDL_CSI_TX_IF_V2_0_0_ECC_AGGR) || (mainMem == SDL_CSI_TX_IF_V2_0_1_ECC_AGGR) ||
                                (mainMem == SDL_MCU_I3C0_1_ECC_AGGR) )
#endif                                      
                            {
                                result = eccInject_onlyTest(mainMem, i, j);
                                if(result == -1)
                                {
                                    ecc_test_status = false;
                                }
                            }
                            else
                            {
                                if(SDL_ECC_aggrTable[mainMem].esmIntSEC != 0u)
                                {
                                    intsrc = SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE;
                                    result = SDL_ECC_selfTest(mainMem,
                                                            i,
                                                            intsrc,
                                                            &injectErrorConfig,
                                                            100000);
                                        
                                    if (result != SDL_PASS)
                                    {
                                        UART_printf("\n ecc_aggr_test self test SEC : mainMem %d: fixed location test failed,Interconnect type RAM id = %d, checker group = %d\n",
                                        mainMem, i, j);
                                        retVal = -1;
                                        ecc_test_status = false;

                                    }
                                }
                                /*
                                * C7x, EDC type checker-groups, ECC errors after error injection is not getting cleared even after we clear the flags. 
                                * It is sticky and require either functional access or sec/ded injection to get clear.
                                * Hence did functional access of L2 SRAM because C7x L2 should be SRAM only by default after reset,
                                * So we need to just do memory mapped access to the addresses corresponding to the bank we are 
                                * running error injection on. Banking arrangement is shown below.
                                * Base address for L2 SRAM is : 0x64800000 
                                * For SOCs with more than one C7x cluster. Base address is : 0x6N800000 where N=4,5,6,7 
                                */
#if defined (SOC_J784S4)
                                if (mainMem == SDL_COMPUTE_CLUSTER0_DSP0_ECC_AGGR && i == 5U)
                                {
                                    SDL_REG32_RD(0x64800000);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP0_ECC_AGGR && i == 7U)
                                {
                                    SDL_REG32_RD(0x64800080);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP0_ECC_AGGR && i == 9U)
                                {
                                    SDL_REG32_RD(0x64800100);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP0_ECC_AGGR && i == 11U)
                                {
                                    SDL_REG32_RD(0x64800180);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP1_ECC_AGGR && i == 5U)
                                {
                                    SDL_REG32_RD(0x65800000);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP1_ECC_AGGR && i == 7U)
                                {
                                    SDL_REG32_RD(0x65800080);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP1_ECC_AGGR && i == 9U)
                                {
                                    SDL_REG32_RD(0x65800100);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP1_ECC_AGGR && i == 11U)
                                {
                                    SDL_REG32_RD(0x65800180);
                                }
                                if (mainMem == SDL_COMPUTE_CLUSTER0_DSP2_ECC_AGGR && i == 5U)
                                {
                                    SDL_REG32_RD(0x66800000);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP2_ECC_AGGR && i == 7U)
                                {
                                    SDL_REG32_RD(0x66800080);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP2_ECC_AGGR && i == 9U)
                                {
                                    SDL_REG32_RD(0x66800100);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP2_ECC_AGGR && i == 11u)
                                {
                                    SDL_REG32_RD(0x66800180);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP3_ECC_AGGR && i == 5U)
                                {
                                    SDL_REG32_RD(0x67800000);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP3_ECC_AGGR && i == 7u)
                                {
                                    SDL_REG32_RD(0x67800080);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP3_ECC_AGGR && i == 9U)
                                {
                                    SDL_REG32_RD(0x67800100);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_DSP3_ECC_AGGR && i == 11U)
                                {
                                    SDL_REG32_RD(0x67800180);
                                }
#endif
#if defined (SOC_J721S2)
                                if (mainMem == SDL_ECC_MEMTYPE_C7X_1 && i == 5U)
                                {
                                    SDL_REG32_RD(0x65800000);
                                }
                                else if (mainMem == SDL_ECC_MEMTYPE_C7X_1 && i == 7U)
                                {
                                    SDL_REG32_RD(0x65800080);
                                }
                                else if (mainMem == SDL_ECC_MEMTYPE_C7X_1 && i == 9U)
                                {
                                    SDL_REG32_RD(0x65800100);
                                }
                                else if (mainMem == SDL_ECC_MEMTYPE_C7X_1 && i == 11U)
                                {
                                    SDL_REG32_RD(0x65800180);
                                }
                                else if (mainMem == SDL_ECC_MEMTYPE_C7X_0 && i == 5U)
                                {
                                    SDL_REG32_RD(0x64800000);
                                }
                                else if (mainMem == SDL_ECC_MEMTYPE_C7X_0 && i == 7U)
                                {
                                    SDL_REG32_RD(0x64800080);
                                }
                                else if (mainMem == SDL_ECC_MEMTYPE_C7X_0 && i == 9U)
                                {
                                    SDL_REG32_RD(0x64800100);
                                }
                                else if (mainMem == SDL_ECC_MEMTYPE_C7X_0 && i == 11U)
                                {
                                    SDL_REG32_RD(0x64800180);
                                }
#endif
#if defined (SOC_J721E)
                                if (mainMem == SDL_COMPUTE_CLUSTER0_C71SS0_ECC_AGGR && i == 6U)
                                {
                                    SDL_REG32_RD(0x64800000);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_C71SS0_ECC_AGGR && i == 8U)
                                {
                                    SDL_REG32_RD(0x64800080);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_C71SS0_ECC_AGGR && i == 10U)
                                {
                                    SDL_REG32_RD(0x64800100);
                                }
                                else if (mainMem == SDL_COMPUTE_CLUSTER0_C71SS0_ECC_AGGR && i == 12U)
                                {
                                    SDL_REG32_RD(0x64800180);
                                }                         
#endif
                                /* DED is supported only for EDC checker type groups of interconnect RAM id's */
                                intsrc = SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE;
                                result = SDL_ECC_selfTest(mainMem,
                                                        i,
                                                        intsrc,
                                                        &injectErrorConfig,
                                                        100000);

                                /* SDL_EUNSUPPORTED_OPS is only returned in the case of DED test for invalid checker group type */
                                if (result != SDL_PASS && result != SDL_EUNSUPPORTED_OPS)
                                {
                                    UART_printf("\n ecc_aggr_test self test DED : mainMem %d: fixed location test failed,Interconnect type RAM id = %d, checker group = %d\n",
                                    mainMem, i, j);
                                    retVal = -1;
                                    ecc_test_status = false;
                                }
                            }
                        }
                    }
                    else
                    {
                        for (k=0; k< SDL_ECC_aggrTable[mainMem].numMemEntries; k++)
                        {
#if defined(SOC_J7200)
                            if ((SDL_ECC_aggrTable[mainMem].memConfigTable[k].memSubType) == (SDL_ECC_aggrTable[mainMem].ramTable[i].RAMId))
                            {
                                if ((mainMem == SDL_ECC_MSRAM_512K0_ECC_AGGR) && (SDL_ECC_aggrTable[mainMem].ramTable[i].RAMId == 0U))
                                {
                                    SDL_ratTranslationCfgInfo translationCfg;
                                    /* Add RAT configuration to access address > 32bit address range */
                                    translationCfg.translatedAddress = SDL_MSRAM_512K0_RAM_BASE;
                                    translationCfg.sizeInBytes = 528U;
                                    translationCfg.baseAddress = (uint32_t)0x60000000U;
                                    uint32_t index = 0U;

                                    /* Set up RAT translation */
                                    result = SDL_RAT_configRegionTranslation((SDL_ratRegs *)SDL_MCU_R5FSS0_RAT_CFG_BASE,
                                                                            index, &translationCfg);
                                    if (result == SDL_PASS)
                                    {
                                        injectErrorConfig.pErrMem = (uint32_t *)0x60000000U;
                                    }
                                    else
                                    {
                                        UART_printf("RAT translation is failed" );
                                    }
                                    break;
                                }
                                else
                                {
                                    injectErrorConfig.pErrMem =((uint32_t *)SDL_ECC_aggrTable[mainMem].memConfigTable[k].memStartAddr);
                                    break;
                                }
                            }
#elif defined (SOC_J721E) || defined (SOC_J721S2) || defined (SOC_J784S4)
                            if ((SDL_ECC_aggrTable[mainMem].memConfigTable[k].memSubType) == (SDL_ECC_aggrTable[mainMem].ramTable[i].RAMId))
                            {
                                if (SDL_ECC_aggrTable[mainMem].memConfigTable[k].memStartAddr < 0x100000000)
                                {
                                    injectErrorConfig.pErrMem = ((uint32_t *)SDL_ECC_aggrTable[mainMem].memConfigTable[k].memStartAddr);
                                }
                                else
                                {
                                    injectErrorConfig.errMem64 = (uint64_t)SDL_ECC_aggrTable[mainMem].memConfigTable[k].memStartAddr;
                                }

                                break;
                            }
#endif
                        }

 /* Below mainMem are third parties ECC Aggregator, It can be tested by creating traffics.
                   SDL supports inject only because out of scope for SDL implementation.  */
#if defined (SOC_J721E)
                        if( (mainMem == SDL_CSI_RX_IF0_ECC_AGGR_0) || (mainMem == SDL_CSI_RX_IF1_ECC_AGGR_0 ) ||
                            (mainMem == SDL_CSI_TX_IF0_CSI_TX_IF_ECC_AGGR) || (mainMem == SDL_CSI_TX_IF0_CSI_TX_IF_ECC_AGGR_BYTE) ||
                            (mainMem == SDL_DSS_EDP0_K3_DSS_EDP_MHDPTX_WRAPPER_ECC_AGGR_PHY) || (mainMem == SDL_DSS_EDP0_K3_DSS_EDP_MHDPTX_WRAPPER_ECC_AGGR_DSC) ||
                            (mainMem == SDL_DSS_EDP0_K3_DSS_EDP_MHDPTX_WRAPPER_ECC_AGGR_CORE) || (mainMem == SDL_DSS_DSI0_K3_DSS_DSI_TOP_ECC_AGGR_SYS) ||
                            (mainMem == SDL_I3C0_I3C_P_ECC_AGGR) || (mainMem == SDL_MCU_I3C0_I3C_P_ECC_AGGR) || (mainMem == SDL_MCU_I3C1_I3C_P_ECC_AGGR))
#elif (SOC_J7200)
                        if( (mainMem == SDL_ECC_MCU_I3C0_0_ECC_AGGR) || (mainMem == SDL_ECC_MCU_I3C0_1_ECC_AGGR)
#elif (SOC_J721S2)
                        if(0)
#elif (SOC_J784S4)
						if( (mainMem == SDL_DSS_DSI0_ECC_AGGR) || (mainMem == SDL_DSS_EDP0_0_ECC_AGGR) || (mainMem == SDL_DSS_EDP0_2_ECC_AGGR) ||
							(mainMem == SDL_DSS_DSI1_ECC_AGGR) || (mainMem == SDL_PCIE0_0_ECC_AGGR) || (mainMem == SDL_PCIE0_1_ECC_AGGR)||
							(mainMem == SDL_PCIE3_0_ECC_AGGR) || (mainMem == SDL_PCIE3_1_ECC_AGGR) || (mainMem == SDL_PCIE2_0_ECC_AGGR) ||
							(mainMem == SDL_PCIE2_1_ECC_AGGR) || (mainMem == SDL_PCIE1_0_ECC_AGGR) || (mainMem == SDL_PCIE1_1_ECC_AGGR) ||
							(mainMem == SDL_CSI_RX_IF2_ECC_AGGR) ||
                            (mainMem == SDL_CSI_RX_IF1_ECC_AGGR) || (mainMem == SDL_CSI_RX_IF0_ECC_AGGR) || (mainMem == SDL_CSI_TX_IF_V2_1_0_ECC_AGGR) ||
                            (mainMem == SDL_CSI_TX_IF_V2_1_1_ECC_AGGR) || (mainMem == SDL_CSI_TX_IF_V2_0_0_ECC_AGGR) || (mainMem == SDL_CSI_TX_IF_V2_0_1_ECC_AGGR) ||
                            (mainMem == SDL_MCU_I3C0_1_ECC_AGGR) )
#endif
                        {
                            result = eccInject_onlyTest(mainMem, i, j);
                            if(result == -1)
                            {
                                ecc_test_status = false;
                            }
                        }
                        else
                        {

                            if (SDL_ECC_aggrTable[mainMem].memConfigTable[k].readable == ((bool)true))
                            {
                                UART_printf("self test started accessable RamId %d  starting \n",i );
                                for (errorInjectionErrSrc=SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE; errorInjectionErrSrc<=(uint32_t)SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE; errorInjectionErrSrc++)
                                {
                                    if((errorInjectionErrSrc == SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE) && (SDL_ECC_aggrTable[mainMem].esmIntSEC != 0u))
                                    {
                                        intsrc = SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE;

                                    }
                                    else if (errorInjectionErrSrc == SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE){
                                        intsrc = SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE;
                                    }
                                    else {
                                        continue;
                                    }

                                    if (SDL_ECC_aggrTable[mainMem].ramTable[i].aggregatorTypeInjectOnly == 1U)
                                    {
                                        result = SDL_ECC_injectError(mainMem,i,intsrc,&injectErrorConfig);
                                        if (result == SDL_PASS )
                                        {
                                            if (mainMem < SDL_ECC_MEMTYPE_MAX)
                                            {
                                                if (mainMem <= (SDL_ECC_AGGREGATOR_MAX_LOW_ENTRIES-1U))
                                                {
                                                    /* Note in the SDL_ECC_aggrBaseAddressTable only the above are
                                                    * supported currently
                                                    */
                                                    pEccAggr = SDL_ECC_aggrBaseAddressTable[mainMem];
                                                }
                                                else if (mainMem >= SDL_ECC_MEMTYPE_MAIN_MSMC_AGGR0)
                                                {
                                                    pEccAggr = SDL_ECC_aggrHighBaseAddressTableTrans[mainMem - \
                                                            SDL_ECC_MEMTYPE_MAIN_MSMC_AGGR0];
                                                }
                                                SDL_ecc_aggrSetEccRamIntrPending(pEccAggr, i, errSrc);
                                            }

                                            do
                                            {
                                                timeOutCnt += 10;
                                                if (timeOutCnt > maxTimeOutMilliSeconds)
                                                {
                                                    result = SDL_EFAIL;
                                                    break;
                                                }
                                            } while (esmError == false);
                                            UART_printf("    ...skipped because this is Inject Only type\n");
                                            result = SDL_PASS;
                                        }
                                    }
                                    else
                                    {
                                        /* If this test is for PSRAM starting at address 0x0, we need special
                                        * translation using RAT in order to access it for the self test. This is because,
                                        * in the application, R5F view of 0x0 is mapped to ATCM memory. This
                                        * flag in the test app allows the address translation function defined the test's
                                        * OSAL implementation to perform the RAT translation when the address is 0x0 for
                                        * PSRAM testing */
                                        if (mainMem == sdl_ecc_psram_mem)
                                        {
                                            sdl_ecc_psram_test = (bool)true;
                                        }

                                        /* Added to demonstrate testing of MSRAM when memory is Cacheable
                                        This method helps to invalidate cache just before triggering ECC Error
                                        MSRAM MPU config is set as Cacheable in this case. SDL_ECC_selfTest can 
                                        be used for MSRAM when MPU config non- cacheable which is the other case */
                                        #if defined (SOC_J721S2) || defined (SOC_J784S4)
                                        if (mainMem == SDL_MCU_MSRAM_1MB0_ECC_AGGR && 
                                            i == SDL_MCU_MSRAM_1MB0_MSRAM128KX64E_ECC_AGGR_MSRAM128KX64E_MSRAM0_ECC0_RAM_ID)
                                        {
                                            uint32_t *  localaddr;
                                            localaddr =injectErrorConfig.pErrMem;
                                            /* Inject error */
                                            retVal = SDL_ECC_injectError(mainMem,
                                                                        i, intsrc,&injectErrorConfig);
                                            
                                            /* invalidate cache */
                                            CacheP_wbInv((uint32_t *)localaddr, VECTOR_SIZE);

                                            /* Read value to trigger ECC error injection */
                                            SDL_REG32_RD(localaddr);
                                            if (retVal == SDL_PASS)
                                            {
                                                UART_printf("\n\n Waiting for ESM Interrupt \n\n");
                                                /* Real wait for the interrupt , just reusing the existing wait function */
                                                result = SDL_dummyWaitForInterrupt(maxTimeOutMilliSeconds);
                                            }
                                        }
                                        else 
                                        {
                                        #endif
                                        result = SDL_ECC_selfTest(mainMem,
                                                                i,
                                                                intsrc,
                                                                &injectErrorConfig,
                                                                100000);
                                        # if defined (SOC_J721S2) || defined (SOC_J784S4)
                                        }
                                        #endif
                                        /* SDL_EUNSUPPORTED_OPS is only returned in the case of DED test for invalid checker group type */
                                        if (result != SDL_PASS  && result != SDL_EUNSUPPORTED_OPS)
                                        {
                                            UART_printf("\n ecc_aggr_test self test for intSrc = %d : mainMem %d: accessable mem type test failed, Wrapper type RAM id = %d\n",intsrc,
                                                        mainMem, i);
                                            retVal = -1;
                                            ecc_test_status=false;
                                        }
                                        /* set sdl_ecc_psram_test back to false to indicate test is done */
                                        sdl_ecc_psram_test = (bool)false;
                                    }
                                }
                            }
                            else
                            {
                                UART_printf("self test started not accessable RamId %d  starting \n",i );

                                if(SDL_ECC_aggrTable[mainMem].esmIntSEC != 0u)
                                {
                                    intsrc = SDL_INJECT_ECC_ERROR_FORCING_1BIT_ONCE;
                                    errSrc = SDL_ECC_AGGR_INTR_SRC_SINGLE_BIT;
                                    esmError = false;
                                    result = SDL_ECC_injectError(mainMem,i,intsrc,&injectErrorConfig);
                                    if (result == SDL_PASS )
                                    {
                                        if (SDL_ECC_aggrTable[mainMem].ramTable[i].aggregatorTypeInjectOnly != 1)
                                        {
                                            if (mainMem < SDL_ECC_MEMTYPE_MAX)
                                            {
                                                if (mainMem <= (SDL_ECC_AGGREGATOR_MAX_LOW_ENTRIES-1U))
                                                {
                                                    /* Note in the SDL_ECC_aggrBaseAddressTable only the above are
                                                    * supported currently
                                                    */
                                                    pEccAggr = SDL_ECC_aggrBaseAddressTable[mainMem];
                                                } 
                                                else if (mainMem >= SDL_ECC_MEMTYPE_MAIN_MSMC_AGGR0)
                                                {
                                                    pEccAggr = SDL_ECC_aggrHighBaseAddressTableTrans[mainMem - \
                                                            SDL_ECC_MEMTYPE_MAIN_MSMC_AGGR0];
                                                }
                                                SDL_ecc_aggrSetEccRamIntrPending(pEccAggr, i, errSrc);
                                            }
                                            timeOutCnt = 0;
                                            do
                                            {
                                                /* dummy wait for the interrupt */
                                                SDL_OSAL_delay(DELAY);;
                                                timeOutCnt += 10;
                                                if (timeOutCnt > maxTimeOutMilliSeconds)
                                                {
                                                    result = SDL_EFAIL;
                                                    break;
                                                }
                                            } while (esmError == false);

                                            timeOutCnt = 0;
                                            esmError = false;
                                            if (result == SDL_PASS)
                                            {
                                                UART_printf("\n\n  Got it\n");
                                            }
                                            else
                                            {
                                                UART_printf("\n Failed\n");
                                                ecc_test_status=false;
                                            }
                                        }
                                        else
                                        {
                                            do
                                            {
                                                timeOutCnt += 10;
                                                if (timeOutCnt > maxTimeOutMilliSeconds)
                                                {
                                                    result = SDL_EFAIL;
                                                    break;
                                                }
                                            } while (esmError == false);
                                            UART_printf("    ...skipped because this is Inject Only type\n");
                                            result = SDL_PASS;
                                        }
                                    }
                                    else
                                    {
                                        UART_printf("    Inject error failed!\n");
                                    }
                                    if (result != SDL_PASS )
                                    {
                                        UART_printf("\n ecc_aggr_test self test SEC: mainMem %d: fixed location test failed, Wrapper type RAM id = %d\n",
                                                mainMem, i);
                                        retVal = -1;
                                        ecc_test_status=false;
                                    }
                                }
                                intsrc = SDL_INJECT_ECC_ERROR_FORCING_2BIT_ONCE;
                                errSrc = SDL_ECC_AGGR_INTR_SRC_DOUBLE_BIT;
                                esmError = false;
                                result = SDL_ECC_injectError(mainMem,i,intsrc,&injectErrorConfig);
                                if (result == SDL_PASS )
                                {
                                    if (SDL_ECC_aggrTable[mainMem].ramTable[i].aggregatorTypeInjectOnly != 1)
                                    {
                                        if (mainMem < SDL_ECC_MEMTYPE_MAX)
                                        {
                                            if (mainMem <= (SDL_ECC_AGGREGATOR_MAX_LOW_ENTRIES-1U))
                                            {
                                                /* Note in the SDL_ECC_aggrBaseAddressTable only the above are
                                                * supported currently
                                                */
                                                pEccAggr = SDL_ECC_aggrBaseAddressTable[mainMem];
                                            } 
                                            else if (mainMem >= SDL_ECC_MEMTYPE_MAIN_MSMC_AGGR0)
                                            {
                                                pEccAggr = SDL_ECC_aggrHighBaseAddressTableTrans[mainMem - \
                                                        SDL_ECC_MEMTYPE_MAIN_MSMC_AGGR0];
                                            }
                                            SDL_ecc_aggrSetEccRamIntrPending(pEccAggr, i, errSrc);
                                        }
                                        timeOutCnt = 0;
                                        do
                                        {
                                            /* dummy wait for the interrupt */
                                            SDL_OSAL_delay(DELAY);;
                                            timeOutCnt += 10;
                                            if (timeOutCnt > maxTimeOutMilliSeconds)
                                            {
                                                result = SDL_EFAIL;
                                                break;
                                            }
                                        } while (esmError == false);

                                        timeOutCnt = 0;
                                        esmError = false;
                                        if (result == SDL_PASS)
                                        {
                                            UART_printf("\n\n  Got it\n");
                                        }
                                        else
                                        {
                                            UART_printf("\n Failed\n");
                                            ecc_test_status=false;
                                        }
                                    }
                                    else
                                    {
                                        do
                                        {
                                            timeOutCnt += 10;
                                            if (timeOutCnt > maxTimeOutMilliSeconds)
                                            {
                                                result = SDL_EFAIL;
                                                break;
                                            }
                                        } while (esmError == false);
                                        UART_printf("    ...skipped because this is Inject Only type\n");
                                        result = SDL_PASS;
                                    }
                                }
                                else
                                {
                                    UART_printf("    Inject error failed!\n");
                                }
                                /* SDL_EUNSUPPORTED_OPS is only returned in the case of DED test for invalid checker group type */
                                if (result != SDL_PASS  && result != SDL_EUNSUPPORTED_OPS)
                                {
                                    UART_printf("\n ecc_aggr_test self test DED: mainMem %d: fixed location test failed, Wrapper type RAM id = %d\n",
                                            mainMem, i);
                                    retVal = -1;
                                    ecc_test_status=false;
                                }
                            }
                        }
                    }
                }
        }
        if(ecc_test_status)
        {
            UART_printf("All tests have passed");   
        }
        else
        {
            UART_printf("some memories have failed");
            ecc_test_status = true;
        }
        gTimeFinish = BootApp_getTimeInMicroSec(SDL_R5PMU_readCntr(SDL_ARM_R5_PMU_CYCLE_COUNTER_NUM));

        UART_printf("ECC for memtype %d started at %d usecs and finished at %d usecs\r\n", mainMem, (uint32_t)gTimeStart, (uint32_t)gTimeFinish);
    }
    return retVal;
}

static int32_t ECC_sdlFuncTest(void)
{
    int32_t result;
    int32_t retVal = 0;

    UART_printf("\n ECC SDL API tests: starting \n\n");
    if (retVal == 0) {
       result = ecc_aggr_test();
       if (result != SDL_PASS) {
           retVal = -1;
            UART_printf("\n ecc_aggr test has failed...");
       }
    }
    return retVal;
}

/* ECC Function module test */
int32_t ECC_funcTest(void)
{
    int32_t testResult;

    testResult = ECC_Memory_init();
    if (testResult != 0)
    {
        UART_printf("\n ECC func tests: unsuccessful");
        return SDL_EFAIL;
    }

    testResult = ECC_sdlFuncTest();

    return (testResult);
}

/* SDL_ECC_applicationCallbackFunction is expected to be defined by the application. It is
 * required by the SDL ECC module. It is called by the SDL ECC module to notify the
 * application of certain ECC errors that are reported as Exception events.
 * Note, however, that it is not executed in this example */
void SDL_ECC_applicationCallbackFunction(SDL_ECC_MemType eccMemType,
                                         uint32_t errorSrc,
                                         uint32_t address,
                                         uint32_t ramId,
                                         uint64_t bitErrorOffset,
                                         uint32_t bitErrorGroup){

    UART_printf("\n  ECC Error Call back function called : eccMemType %d, errorSrc 0x%x, " \
                "address 0x%x, ramId %d, bitErrorOffset 0x%04x%04x, bitErrorGroup %d\n",
                eccMemType, errorSrc, address, ramId, (uint32_t)(bitErrorOffset >> 32),
                (uint32_t)(bitErrorOffset & 0x00000000FFFFFFFF), bitErrorGroup);
    UART_printf("  Take action \n");

    /* Any additional customer specific actions can be added here */

}

uint32_t MMR_unlock_one(uint32_t * kick0, uint32_t * kick1)
{
    /* initialize the status variable */
    uint32_t status = 1;

    /* if either of the kick lock registers are locked */
    if (!(*kick0 & 0x1) | !(*kick1 & 0x1))
    {
        /* unlock the partition by writing the unlock values to the kick lock registers */
        *kick0 = KICK0_UNLOCK_VAL;
        *kick1 = KICK1_UNLOCK_VAL;
    }

    /* check to see if either of the kick registers are unlocked. */
    if (!(*kick0 & 0x1))
    {
        status = 0;
    }

    /* return the status to the calling program */
    return status;

}

#if defined (SOC_J721E) || defined (SOC_J721S2) || defined (SOC_J784S4)
uint32_t MAIN_CTRL_MMR_unlock_all()
{
    /* initialize the status variable */
    uint32_t status = 1;
    /* Unlock the 0th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK0_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK0_KICK1));
    /* Unlock the 1st partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK1_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK1_KICK1));
    /* Unlock the 2nd partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK2_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK2_KICK1));
    /* Unlock the 3rd partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK1));
    /* Unlock the 4th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK4_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK4_KICK1));
/* These two not in the CSL yet
   status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK1));
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK6_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK6_KICK1));
*/
    /* Unlock the 7th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK7_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK7_KICK1));
    /* Return status to calling program */
    return status;
}
#endif

typedef enum /* updated for J7 */
{
    SERDES_DIAG_PCIE_GEN1 = 1,

    SERDES_DIAG_PCIE_GEN2 = 2,

    SERDES_DIAG_PCIE_GEN3 = 3,

    SERDES_DIAG_PCIE_GEN4 = 4
} SERDES_DIAG_PCIE_TYPE;

#if defined (SOC_J721E)
uint32_t serdes_init(uint8_t serdesInstance, CSL_SerdesSSCMode SSC_Mode){
    CSL_SerdesResult status;
    uint32_t i, laneNum;
    CSL_SerdesLaneEnableParams serdesLaneEnableParams;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    /* SERDES_DIAG_PCIE_TYPE pcie_gen_type;  We can use use link rate to determine the gen type in code */

    if(serdesInstance > 3){
        UART_printf("Invalid Serdes!\n");
        return 1;
    }

    memset(&serdesLaneEnableParams, 0, sizeof(serdesLaneEnableParams));
    serdesLaneEnableParams.serdesInstance = (CSL_SerdesInstance)serdesInstance;

    switch(serdesInstance){
        case 0:
            serdesLaneEnableParams.baseAddr = CSL_SERDES_16G0_BASE;
            break;
        case 1:
            serdesLaneEnableParams.baseAddr = CSL_SERDES_16G1_BASE;
            break;
        case 2:
            serdesLaneEnableParams.baseAddr = CSL_SERDES_16G2_BASE;
            break;
        case 3:
            serdesLaneEnableParams.baseAddr = CSL_SERDES_16G3_BASE;
            break;
    }

    serdesLaneEnableParams.refClock         = SERDES_DIAG_TEST_REF_CLOCK;
    serdesLaneEnableParams.refClkSrc        = SERDES_DIAG_TEST_REF_CLOCK_SRC;
    serdesLaneEnableParams.linkRate         = SERDES_DIAG_TEST_LINK_RATE;
    serdesLaneEnableParams.numLanes         = SERDES_DIAG_TEST_NUM_LANES;
    serdesLaneEnableParams.laneMask         = SERDES_DIAG_TEST_LANE_MASK;
    serdesLaneEnableParams.SSC_mode         = SSC_Mode;
    serdesLaneEnableParams.phyType          = SERDES_DIAG_TEST_PHY_TYPE;
    serdesLaneEnableParams.pcieGenType      = SERDES_DIAG_TEST_PCIE_GEN_TYPE;
    serdesLaneEnableParams.operatingMode    = SERDES_DIAG_TEST_OPERATING_MODE;
    serdesLaneEnableParams.phyInstanceNum   = serdesInstance;
    for(i=0; i< serdesLaneEnableParams.numLanes; i++){
      serdesLaneEnableParams.laneCtrlRate[i] = SERDES_DIAG_TEST_LANE_RATE;
      serdesLaneEnableParams.loopbackMode[i] = SERDES_DIAG_TEST_LOOPBACK_MODE; /* still have to change to correct loopback mode */
    }
    /* pcie_gen_type = SERDES_DIAG_TEST_PCIE_GEN_TYPE; */

    CSL_serdesPorReset(serdesLaneEnableParams.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    for (laneNum=0; laneNum < serdesLaneEnableParams.numLanes; laneNum++){
        CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                           serdesLaneEnableParams.phyType,
                           serdesLaneEnableParams.phyInstanceNum,
                           serdesLaneEnableParams.serdesInstance,
                           laneNum);
    }

    /* selects the appropriate clocks for all serdes based on the protocol chosen */
     status = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                  serdesLaneEnableParams.baseAddr,
                                  serdesLaneEnableParams.refClock,
                                  serdesLaneEnableParams.refClkSrc,
                                  serdesLaneEnableParams.serdesInstance,
                                  serdesLaneEnableParams.phyType);

     /* Assert PHY reset and disable all lanes */
     CSL_serdesDisablePllAndLanes(serdesLaneEnableParams.baseAddr, serdesLaneEnableParams.numLanes, serdesLaneEnableParams.laneMask);

    /*Load the Serdes Config File */
    status = CSL_serdesPCIeInit(&serdesLaneEnableParams); /* Use this for PCIe serdes config load */

    /* Return error if input params are invalid */
    if (status != CSL_SERDES_NO_ERR){
        UART_printf("Invalid SERDES Init Params \n");
    }

    /* Set this to standard mode defined by Cadence */
    for (laneNum=0; laneNum < serdesLaneEnableParams.numLanes; laneNum++){
        CSL_serdesPCIeModeSelect(serdesLaneEnableParams.baseAddr, serdesLaneEnableParams.pcieGenType, laneNum);
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    laneRetVal = CSL_serdesLaneEnable(&serdesLaneEnableParams);

    if (laneRetVal != 0){
        UART_printf("Invalid Serdes Lane Enable\n");
        return 2;
    }

    UART_printf("Serdes %d Init Complete\n", serdesInstance);
    return 0;
}

void ecc_aggr_test_pcie_init()
{
  MAIN_CTRL_MMR_unlock_all();
  serdes_init(0x0, CSL_SERDES_NO_SSC);
  serdes_init(0x1, CSL_SERDES_NO_SSC);
  serdes_init(0x2, CSL_SERDES_NO_SSC);
  serdes_init(0x3, CSL_SERDES_NO_SSC);

  UART_printf("Serdes Init Complete\n");
}

void edp_init()
{
    SDL_REG32_WR(((uintptr_t)SDL_CTRL_MMR0_CFG0_BASE) + ((uintptr_t)SDL_MAIN_CTRL_MMR_CFG0_EDP_PHY0_CLKSEL), 0x1);
    SDL_REG32_WR(((uintptr_t)SDL_DSS_EDP0_INTG_CFG_VP_BASE) + ((uintptr_t)0x8), 0x1);
    SDL_REG32_WR(((uintptr_t)SDL_DSS_EDP0_INTG_CFG_VP_BASE) + ((uintptr_t)0x10), 0x1F);

    SDL_REG32_WR(((uintptr_t)SDL_WKUP_CTRL_MMR0_CFG0_BASE) + ((uintptr_t)SDL_WKUP_CTRL_MMR_CFG0_HFOSC1_CTRL), 0x0);

    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x408), 0xA0000000);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x40c), 0x39000000);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x480), 0x80000000);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x4c0), 0x80000000);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x500), 0x80000000);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x540), 0x80000000);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x484), 0x10001);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x4c4), 0x10001);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x504), 0x10001);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x544), 0x10001);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x5fc), 0x00000);

}
#endif
#if defined (SOC_J784S4)
void edp_init()
{
    SDL_REG32_WR(((uintptr_t)SDL_CTRL_MMR0_CFG0_BASE) + ((uintptr_t)SDL_MAIN_CTRL_MMR_CFG0_EDP_PHY0_CLKSEL), 0x1);
    SDL_REG32_WR(((uintptr_t)SDL_DSS_EDP0_INTG_CFG_VP_BASE) + ((uintptr_t)0x8), 0x1);
    SDL_REG32_WR(((uintptr_t)SDL_DSS_EDP0_INTG_CFG_VP_BASE) + ((uintptr_t)0x10), 0x1F);

    SDL_REG32_WR(((uintptr_t)SDL_WKUP_CTRL_MMR0_CFG0_BASE) + ((uintptr_t)SDL_WKUP_CTRL_MMR_CFG0_HFOSC1_CTRL), 0x0);

    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x408), 0xA0000000);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x40c), 0x39000000);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x480), 0x80000000);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x4c0), 0x80000000);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x500), 0x80000000);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x540), 0x80000000);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x484), 0x10001);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x4c4), 0x10001);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x504), 0x10001);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x544), 0x10001);
    SDL_REG32_WR(((uintptr_t)0x05050000) + ((uintptr_t)0x5fc), 0x00000);

}
#endif

int32_t BootApp_initDevices(void)
{
    int32_t status = SDL_PASS;
    uint32_t i;
#if defined (SOC_J721E) || defined (SOC_J721S2) || defined (SOC_J784S4)
    uint32_t read_val;
#endif

    for (i = 0; i < AUX_NUM_DEVICES; i++)
    {
        /* Power up RTI */
        status = Sciclient_pmSetModuleState(aux_devices[i],
                                            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                            TISCI_MSG_FLAG_AOP,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);

        if (status != SDL_PASS)
        {
            UART_printf("   Sciclient_pmSetModuleState 0x%x ...FAILED: retValue %d\n",
                        aux_devices[i], status);
        }
    }
#if defined (SOC_J721E) || defined (SOC_J784S4)
    // UFS init clocks - SDL_UFS0_P2A_WRAP_CFG_VBP_UFSHCI_BASE
    read_val = SDL_REG32_RD(((uintptr_t)SDL_UFS0_P2A_WRAP_CFG_VBP_UFSHCI_BASE)+((uintptr_t)0x34));
    SDL_REG32_WR(((uintptr_t)SDL_UFS0_P2A_WRAP_CFG_VBP_UFSHCI_BASE)+ ((uintptr_t)0x34), read_val | 0x1);
    read_val = SDL_REG32_RD(((uintptr_t)SDL_UFS0_P2A_WRAP_CFG_VBP_UFSHCI_BASE)+((uintptr_t)0x34));
    while(read_val!= 0x1) {
        read_val = SDL_REG32_RD(((uintptr_t)SDL_UFS0_P2A_WRAP_CFG_VBP_UFSHCI_BASE)+((uintptr_t)0x34));
    }
    read_val = SDL_REG32_RD(((uintptr_t)SDL_UFS0_P2A_WRAP_CFG_VBP_UFSHCI_BASE)+((uintptr_t)0xc8));
    SDL_REG32_WR(((uintptr_t)SDL_UFS0_P2A_WRAP_CFG_VBP_UFSHCI_BASE)+ ((uintptr_t)0xc8), read_val | 0xF8);

    // SA2UL init
    read_val = SDL_REG32_RD(((uintptr_t)SDL_SA2_UL0_BASE)+((uintptr_t)0x1000));
    SDL_REG32_WR(((uintptr_t)SDL_SA2_UL0_BASE)+ ((uintptr_t)0x1000), read_val | 0x10);
#if !defined(SOC_J784S4)
    // PCIE init
    ecc_aggr_test_pcie_init();
#endif
    // EDP init
    edp_init();
#elif defined (SOC_J721S2)
    // SA2UL init
    read_val = SDL_REG32_RD(((uintptr_t)SDL_SA2_UL0_BASE)+((uintptr_t)0x1000));
    SDL_REG32_WR(((uintptr_t)SDL_SA2_UL0_BASE)+ ((uintptr_t)0x1000), read_val | 0x10);
#endif
    return status;
}

int32_t BootApp_eccFxn(void)
{
/* Declaration of variables */
    int32_t  testResult = 0;

    /* Init the modules */
    BootApp_initDevices();

#if defined (SOC_J721S2) || defined (SOC_J784S4)
    /* Power on CPSW1 explicitly */
    Sciclient_pmSetModuleState(TISCI_DEV_CPSW1, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON, TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
    UART_printf("Powering on CPSW1\r\n");

    /* Power on ADC12FC_16FFC1 explicitly */
    Sciclient_pmSetModuleState(TISCI_DEV_MCU_ADC12FC_16FFC1, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON, TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
    UART_printf("Powering on ADC12FC_16FFC1\r\n");

    /* Power on ADC12FC_16FFC0 explicitly */
    Sciclient_pmSetModuleState(TISCI_DEV_MCU_ADC12FC_16FFC0, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON, TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
    UART_printf("Powering on ADC12FC_16FFC0\r\n");
#endif
#if defined (SOC_J784S4)
 /* Power on CPSW_9XUSS_J7AM0 explicitly */
    Sciclient_pmSetModuleState(TISCI_DEV_CPSW_9XUSS_J7AM0, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON, TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
    UART_printf("Powering on CPSW_9XUSS_J7AM0\r\n");
#endif

    if (testResult == SDL_PASS)
    {
        /* SDL osal wrapper */
        BootApp_osalWrapper();

        UART_printf("\nECC test Application\r\n");
        testResult = ECC_funcTest();
        UART_printf("\n ECC func Test");
        if (testResult == SDL_PASS)
        {
        UART_printStatus("\r\nAll test have passed. \r\n");
        }
        else
        {
        UART_printStatus("\r\nSome test have failed. \r\n");
        }

    }
    else
    {
        UART_printStatus("\r\nBoard Init failed. Exiting the app.\r\n");
    }

    return (0);
}