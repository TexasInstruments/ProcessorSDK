/*
 *  Copyright (c) Texas Instruments Incorporated 2026
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
 *  \file     ecc_ddr.c
 *
 *  \brief    This file implements DDR ECC functions
 */

/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/

#include "ecc_ddr.h"
#include <sdl_esm.h>
#include <ti/osal/osal.h>
#include <ti/csl/csl_rat.h>

#include <ip_fma_ddr.h>
#include <ip_fma_ecc_aggr.h>

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/**< Timer period for test, set to 3 s  */
#define TEST_TIMER_PERIOD_3S    ((3000U) * 10U)
/**< Converts miliseconds to microseconds for TimerP configuration */
#define MSEC_TO_USEC(x)         ((uint32_t)(x) * 1000)
/**< Number of DDR ECC tests iterations performed */
#define NO_DDR_ECC_TESTS        ((uint8_t) 10U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* UDMA driver object */
struct Udma_DrvObj      gECCDDRUdmaDrvObj;

volatile uint32_t gSecTestPass;
volatile uint32_t gDedTestPass;

volatile uint32_t testVal;

/* Expected values for DDR subsystem registers, filled after init */
IpFma_DdrSubsystemRegs gDdrSubsystemRegsExpValues0;
IpFma_DdrSubsystemRegs gDdrSubsystemRegsExpValues1;
#if defined (SOC_J784S4)
IpFma_DdrSubsystemRegs gDdrSubsystemRegsExpValues2;
IpFma_DdrSubsystemRegs gDdrSubsystemRegsExpValues3;
#endif

/* Expected values for DDR ECC aggregator registers, filled after init */
IpFma_EccAggregatorRegs gDdrAggregatorRegsExpValues0;
IpFma_EccAggregatorRegs gDdrAggregatorRegsExpValues1;
#if defined (SOC_J784S4)
IpFma_EccAggregatorRegs gDdrAggregatorRegsExpValues2;
IpFma_EccAggregatorRegs gDdrAggregatorRegsExpValues3;
#endif


#if defined (FAULT_INJECTION_ENABLED)    
/* Fault injection flag */
int8_t gFaultInjection = 0;
#endif


SDL_ESM_config DdrApp_EccTestEsmInitConfigMAIN =
{
    .esmErrorConfig = {1u, 8u}, /* Self test error config */
    .enableBitmap = {0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x0000C000u, 0x00000C00u, 0xC0000000u, 0x000000C0u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u,
                },
     /* Only DDR Inline ECC ESM events are enabled */
    .priorityBitmap = {0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x0000C000u, 0x00000C00u, 0xC0000000u, 0x000000C0u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u,
                },
    /* Only DDR Inline ECC ESM events are set to high priority */
    .errorpinBitmap = {0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x0000C000u, 0x00000C00u, 0xC0000000u, 0x000000C0u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u,
                },
    /* Only DDR Inline ECC ESM events are set to trigger error pin */
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void DdrApp_DdrEccInit();
static int32_t DdrApp_DdrEccConfig();
static int32_t DdrApp_DdrSecErrTest(uint32_t test_address);
static int32_t DdrApp_DdrDedErrTest(uint32_t test_address);
int32_t DdrApp_DdrUdmaPrimeECC();
int32_t DdrApp_DdrEnableECC(CSL_emif_sscfgRegs *ddrCfgAddr);
int32_t DdrApp_DdrDisableECC(CSL_emif_sscfgRegs *ddrCfgAddr);
int32_t DdrApp_SdlEsmApplicationCallbackFunction(SDL_ESM_Inst esmInstType,
                                                   SDL_ESM_IntType esmIntType,
                                                   uint32_t grpChannel,
                                                   uint32_t index,
                                                   uint32_t intSrc,
                                                   void *arg);

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

static int32_t DdrApp_DdrEccHighMemRatConfig(uint32_t ratRegion)
{
    int32_t status = CSL_PASS;
    CSL_ratRegs *pRatRegs = (CSL_ratRegs *)CSL_MCU_R5FSS0_RAT_CFG_BASE;
    CSL_RatTranslationCfgInfo TranslationCfg;

    TranslationCfg.sizeInBytes = EMIF_DDR_HIMEM_SIZE;
    TranslationCfg.baseAddress = DDR_HIMEM_R5F_ADDRESS;
    TranslationCfg.translatedAddress = EMIF_DDR_HIMEM_START_ADDR;

    if (ratRegion < CSL_ratGetMaxRegions(pRatRegs)) {
        if (CSL_ratIsRegionTranslationEnabled(pRatRegs, ratRegion) == (bool)false) {
            CSL_ratEnableRegionTranslation(pRatRegs, ratRegion);
            CSL_ratConfigRegionTranslation(pRatRegs, ratRegion, &TranslationCfg);
        }
        else
        {
            status = CSL_EFAIL;
        }
    }
    else
    {
        status = CSL_EBADARGS;
    }

    return status;
}

static int32_t DdrApp_EccAggrRatConfig(uint32_t ratRegion)
{
    int32_t status = CSL_PASS;
    CSL_ratRegs *pRatRegs = (CSL_ratRegs *)CSL_MCU_R5FSS0_RAT_CFG_BASE;
    CSL_RatTranslationCfgInfo cfg;

    cfg.sizeInBytes       = ECC_AGGR_SPAN_BYTES;
    cfg.baseAddress       = ECC_AGGR_LOCAL_BASE;
    cfg.translatedAddress = ECC_AGGR_PHYS_BASE;

    if (ratRegion < CSL_ratGetMaxRegions(pRatRegs)) 
    {
        if (CSL_ratIsRegionTranslationEnabled(pRatRegs, ratRegion) == (bool)false) 
        {
            CSL_ratEnableRegionTranslation(pRatRegs, ratRegion);
            CSL_ratConfigRegionTranslation(pRatRegs, ratRegion, &cfg);
        } 
        else 
        {
            status = CSL_EFAIL;
        }
    } 
    else 
    {
        status = CSL_EBADARGS;
    }

    return status;
}

static void DdrApp_printRbCheckLog(IpFma_Status statusRb)
{
    if (IPFMA_OK == statusRb)
    {
        UART_printf(
            "   values MATCH!\n"
            "---------------------------------------------------------------\n"
        );
    }
    else
    {
        UART_printf(
            "   values MISMATCH!\n"
            "---------------------------------------------------------------\n"
        );
    }
}

static void DdrApp_registerCheck(void *arg)
{
    IpFma_Status statusRb = IPFMA_OK;

    /* Check the register values for DDRSS0*/
    IpFma_DdrSubsystemRegs ddrSubsystemRegsActualValue0;
    statusRb = IpFma_Ddr_GetSubsystemRegs(DDRSS0_CFG_BASE, &ddrSubsystemRegsActualValue0);

#if defined (FAULT_INJECTION_ENABLED)    
    /* False values injection scenario */
    if (1 == gFaultInjection)
    {
        ddrSubsystemRegsActualValue0.ecc_ctrl_reg = 0xFFFF;
        gFaultInjection = 0;
    }
#endif

    if (IPFMA_OK == statusRb)
    {
        UART_printf(
            "\n---------------------------------------------------------------"
            "\nREGCHECK: Comparing expected-actual ddr subsystem register values DDRSS0...\n"
        );
        statusRb = IpFma_Ddr_CompareSubsystemRegs(&gDdrSubsystemRegsExpValues0, &ddrSubsystemRegsActualValue0);
        DdrApp_printRbCheckLog(statusRb);
    }

    IpFma_EccAggregatorRegs ddrEccAggregatorActualValue0;
    statusRb = IpFma_EccAggr_GetRegs(DDRSS0_ECC_AGGR_LOCAL, &ddrEccAggregatorActualValue0);
    if (IPFMA_OK == statusRb)
    {
        UART_printf(
            "\n---------------------------------------------------------------"
            "\nREGCHECK: Comparing expected-actual ddr aggregator register values DDRSS0...\n"
        );
        statusRb = IpFma_EccAggr_CompareRegs(&gDdrAggregatorRegsExpValues0, &ddrEccAggregatorActualValue0);
        DdrApp_printRbCheckLog(statusRb);
    }



    
    /* Check the register values DDRSS1*/
    IpFma_DdrSubsystemRegs ddrSubsystemRegsActualValue1;
    statusRb = IpFma_Ddr_GetSubsystemRegs(DDRSS1_CFG_BASE, &ddrSubsystemRegsActualValue1);
    if (IPFMA_OK == statusRb)
    {
        UART_printf(
            "\n---------------------------------------------------------------"
            "\nREGCHECK: Comparing expected-actual ddr subsystem register values DDRSS1...\n"
        );
        statusRb = IpFma_Ddr_CompareSubsystemRegs(&gDdrSubsystemRegsExpValues1, &ddrSubsystemRegsActualValue1);
        DdrApp_printRbCheckLog(statusRb);
    }


    IpFma_EccAggregatorRegs ddrEccAggregatorActualValue1;
    statusRb = IpFma_EccAggr_GetRegs(DDRSS1_ECC_AGGR_LOCAL, &ddrEccAggregatorActualValue1);
    if (IPFMA_OK == statusRb)
    {
        UART_printf(
            "\n---------------------------------------------------------------"
            "\nREGCHECK: Comparing expected-actual ddr aggregator register values DDRSS1...\n"
        );
        statusRb = IpFma_EccAggr_CompareRegs(&gDdrAggregatorRegsExpValues1, &ddrEccAggregatorActualValue1);
        DdrApp_printRbCheckLog(statusRb);
    }


#if defined (SOC_J784S4)
    /* Check the register values DDRSS2 */
    IpFma_DdrSubsystemRegs ddrSubsystemRegsActualValue2;
    statusRb = IpFma_Ddr_GetSubsystemRegs(DDRSS2_CFG_BASE, &ddrSubsystemRegsActualValue2);
    if (IPFMA_OK == statusRb)
    {
        UART_printf(
            "\n---------------------------------------------------------------"
            "\nREGCHECK: Comparing expected-actual ddr subsystem register values DDRSS2...\n"
        );
        statusRb = IpFma_Ddr_CompareSubsystemRegs(&gDdrSubsystemRegsExpValues2, &ddrSubsystemRegsActualValue2);
        DdrApp_printRbCheckLog(statusRb);
    }

    IpFma_EccAggregatorRegs ddrEccAggregatorActualValue2;
    statusRb = IpFma_EccAggr_GetRegs(DDRSS2_ECC_AGGR_LOCAL, &ddrEccAggregatorActualValue2);
    if (IPFMA_OK == statusRb)
    {
        UART_printf(
            "\n---------------------------------------------------------------"
            "\nREGCHECK: Comparing expected-actual ddr aggregator register values DDRSS2...\n"
        );
        statusRb = IpFma_EccAggr_CompareRegs(&gDdrAggregatorRegsExpValues2, &ddrEccAggregatorActualValue2);
        DdrApp_printRbCheckLog(statusRb);
    }



    /* Check the register values DDRSS3*/
    IpFma_DdrSubsystemRegs ddrSubsystemRegsActualValue3;
    statusRb = IpFma_Ddr_GetSubsystemRegs(DDRSS3_CFG_BASE, &ddrSubsystemRegsActualValue3);
    if (IPFMA_OK == statusRb)
    {
        UART_printf(
            "\n---------------------------------------------------------------"
            "\nREGCHECK: Comparing expected-actual ddr subsystem register values DDRSS3...\n"
        );
        statusRb = IpFma_Ddr_CompareSubsystemRegs(&gDdrSubsystemRegsExpValues3, &ddrSubsystemRegsActualValue3);
        DdrApp_printRbCheckLog(statusRb);
    }

    IpFma_EccAggregatorRegs ddrEccAggregatorActualValue3;
    statusRb = IpFma_EccAggr_GetRegs(DDRSS3_ECC_AGGR_LOCAL, &ddrEccAggregatorActualValue3);
    if (IPFMA_OK == statusRb)
    {
        UART_printf(
            "\n---------------------------------------------------------------"
            "\nREGCHECK: Comparing expected-actual ddr aggregator register values DDRSS3...\n"
        );
        statusRb = IpFma_EccAggr_CompareRegs(&gDdrAggregatorRegsExpValues3, &ddrEccAggregatorActualValue3);
        DdrApp_printRbCheckLog(statusRb);
    }
#endif
}

static TimerP_Handle DdrApp_startPeriodicTimer(const uint32_t periodMs, TimerP_Fxn callback, void *arg)
{
    TimerP_Params timerParams;
    TimerP_Handle timerHandle;

    if (NULL == callback)
    {
        UART_printf("TIMER: DdrApp_startPeriodicTimer: NULL callback provided.\n");
        return NULL;
    }

    (void)TimerP_Params_init(&timerParams);
    timerParams.runMode = TimerP_RunMode_CONTINUOUS;
    timerParams.startMode = TimerP_StartMode_AUTO;
    timerParams.periodType = TimerP_PeriodType_MICROSECS;
    timerParams.period = ((MSEC_TO_USEC(periodMs)));
    timerParams.arg = (void *)arg;

    timerHandle = TimerP_create(TimerP_ANY, callback, &timerParams);
    if (NULL == timerHandle)
    {
        UART_printf("TIMER: Failed to create timer (period = %u μs).\n", timerParams.period);
        return NULL;
    }

    UART_printf("TIMER: Timer started successfully (period = %u μs).\n", timerParams.period);
    return timerHandle;
}

int32_t DdrApp_DdrEccTest(void)
{
    int32_t retVal = CSL_PASS;
    SDL_ErrType_t result = 0;

    /* Initialize Test variables */
    gSecTestPass   = FALSE;
    gDedTestPass   = FALSE;

    retVal = DdrApp_DdrEccHighMemRatConfig(DDR_HIMEM_RAT_REGION);
    if (CSL_PASS != retVal)
    {
        /* print error and quit */
        UART_printf("DDR ECC: RAT configuration failed\n");
    }

    /* Initialize RAT region for ECC Aggregator registers */
    retVal = DdrApp_EccAggrRatConfig(ECC_AGGR_RAT_REGION);
    if (CSL_PASS != retVal)
    {
        /* print error and quit */
        UART_printf("DDR ECC AGG: RAT configuration failed\n");
    }

    /* Initialize MAIN ESM module */
    retVal = SDL_ESM_init(SDL_ESM_INST_MAIN_ESM0, &DdrApp_EccTestEsmInitConfigMAIN, DdrApp_SdlEsmApplicationCallbackFunction, NULL);
    if (CSL_PASS != retVal)
    {
        /* print error and quit */
        UART_printf("SDL_ESM_init: Error initializing MAIN ESM: result = %d\n", result);
    }
    else
    {
        UART_printf("\nSDL_ESM_init: Init MAIN ESM complete \n");
    }

    if (CSL_PASS == retVal)
    {
        DdrApp_DdrEccInit();

        UART_printf("\nDDR ECC: Init phase is done\n");
        /* Init phase is done. Periodic checks can now be performed. */

        /* DDRSS0 */
        /* Get the expected register values for DDRSS0 after init phase */
        IpFma_Status statusRb = IPFMA_OK;
        statusRb = IpFma_Ddr_GetSubsystemRegs(DDRSS0_CFG_BASE, &gDdrSubsystemRegsExpValues0);

        if (IPFMA_OK == statusRb)
        {
            UART_printf("\nREGCHECK: Successful DDR subsystem register read of DDRSS0 for expected values!\n");
        }

        statusRb = IpFma_EccAggr_GetRegs(DDRSS0_ECC_AGGR_LOCAL, &gDdrAggregatorRegsExpValues0);

        if (IPFMA_OK == statusRb)
        {
            UART_printf("\nREGCHECK: Successful DDR ECC aggregator register read of DDRSS0 for expected values!\n");
        }
        
        /* DDRSS1 */
        /* Get the expected register values for DDRSS1 after init phase */
        statusRb = IpFma_Ddr_GetSubsystemRegs(DDRSS1_CFG_BASE, &gDdrSubsystemRegsExpValues1);

        if (IPFMA_OK == statusRb)
        {
            UART_printf("\nREGCHECK: Successful DDR subsystem register read of DDRSS1 for expected values!\n");
        }

        statusRb = IpFma_EccAggr_GetRegs(DDRSS1_ECC_AGGR_LOCAL, &gDdrAggregatorRegsExpValues1);

        if (IPFMA_OK == statusRb)
        {
            UART_printf("\nREGCHECK: Successful DDR ECC aggregator register read of DDRSS1 for expected values!\n");
        }

#if defined (SOC_J784S4)
        /* DDRSS2 */
        /* Get the expected register values for DDRSS2 after init phase */
        statusRb = IpFma_Ddr_GetSubsystemRegs(DDRSS2_CFG_BASE, &gDdrSubsystemRegsExpValues2);

        if (IPFMA_OK == statusRb)
        {
            UART_printf("\nREGCHECK: Successful DDR subsystem register read of DDRSS2 for expected values!\n");
        }

        statusRb = IpFma_EccAggr_GetRegs(DDRSS2_ECC_AGGR_LOCAL, &gDdrAggregatorRegsExpValues2);

        if (IPFMA_OK == statusRb)
        {
            UART_printf("\nREGCHECK: Successful DDR ECC aggregator register read of DDRSS2 for expected values!\n");
        }

        /* DDRSS3 */
        /* Get the expected register values for DDRSS3 after init phase */
        statusRb = IpFma_Ddr_GetSubsystemRegs(DDRSS3_CFG_BASE, &gDdrSubsystemRegsExpValues3);

        if (IPFMA_OK == statusRb)
        {
            UART_printf("\nREGCHECK: Successful subsystem register read of DDRSS3 for expected values!\n");
        }

        statusRb = IpFma_EccAggr_GetRegs(DDRSS3_ECC_AGGR_LOCAL, &gDdrAggregatorRegsExpValues3);

        if (IPFMA_OK == statusRb)
        {
            UART_printf("\nREGCHECK: Successful DDR ECC aggregator register read of DDRSS3 for expected values!\n");
        }
#endif

        /* Start a periodic timer with callback function that does register check */
        TimerP_Handle timer = DdrApp_startPeriodicTimer(TEST_TIMER_PERIOD_3S, (TimerP_Fxn)&DdrApp_registerCheck, NULL);

        if (NULL == timer)
        {
            UART_printf("\nTIMER: Unable to start periodic timer.\n");
        }


        int8_t i = NO_DDR_ECC_TESTS;
        while (i > 0)
        {
            /* Simulate the read-back fail scenario in the last iteration*/               
#if defined (FAULT_INJECTION_ENABLED)    
            if (NO_DDR_ECC_TESTS == i)
            {
                gFaultInjection = 1;
            }
            UART_printf("FAULT injection enabled\n");
#endif
            /* Do the ECC tests */
            UART_printf("\nDDR ECC: Test iteration no %d\n", i);

            //Injecting within DDRSS0
            if (DdrApp_DdrSecErrTest(DDRSS0_TEST_ADDRESS) != CSL_PASS)
            {
                retVal = CSL_EFAIL;
            }
            if (DdrApp_DdrDedErrTest(DDRSS0_TEST_ADDRESS) != CSL_PASS)
            {
                retVal = CSL_EFAIL;
            }

            //Injecting within DDRSS1
            if (DdrApp_DdrSecErrTest(DDRSS1_TEST_ADDRESS)!= CSL_PASS)
            {
                retVal = CSL_EFAIL;
            }
            if (DdrApp_DdrDedErrTest(DDRSS1_TEST_ADDRESS)!= CSL_PASS)
            {
                retVal = CSL_EFAIL;
            }

#if defined (SOC_J784S4)
            //Injecting within DDRSS2
            if (DdrApp_DdrSecErrTest(DDRSS2_TEST_ADDRESS)!= CSL_PASS)
            {
                retVal = CSL_EFAIL;
            }
            if (DdrApp_DdrDedErrTest(DDRSS2_TEST_ADDRESS)!= CSL_PASS)
            {
                retVal = CSL_EFAIL;
            }

            //Injecting within DDRSS3
            if (DdrApp_DdrSecErrTest(DDRSS3_TEST_ADDRESS)!= CSL_PASS)
            {
                retVal = CSL_EFAIL;
            }
            if (DdrApp_DdrDedErrTest(DDRSS3_TEST_ADDRESS)!= CSL_PASS)
            {
                retVal = CSL_EFAIL;
            }
#endif

            //Injecting in Himem region within DDRSS0
            if (DdrApp_DdrSecErrTest(DDRSS0_HIMEM_TEST_ADDRESS) != CSL_PASS)
            {
                retVal = CSL_EFAIL;
            }
            //Injecting in Himem region within DDRSS1
            if (DdrApp_DdrSecErrTest(DDRSS1_HIMEM_TEST_ADDRESS) != CSL_PASS)
            {
                retVal = CSL_EFAIL;
            }

#if defined (SOC_J784S4)
            //Injecting in Himem region within DDRSS2
            if (DdrApp_DdrSecErrTest(DDRSS2_HIMEM_TEST_ADDRESS) != CSL_PASS)
            {
                retVal = CSL_EFAIL;
            }
            //Injecting in Himem region within DDRSS3
            if (DdrApp_DdrSecErrTest(DDRSS3_HIMEM_TEST_ADDRESS) != CSL_PASS)
            {
                retVal = CSL_EFAIL;
            }
#endif

            UART_printf( "\r\nDDR ECC test exiting...");
            UART_printf("\n ========================================================== \n");
            i--;
        }

    }

    return retVal;
}

/* Function initialized DDR ECC */
static void DdrApp_DdrEccInit()
{
    int32_t cslRet;

    /* Ecc config */
    cslRet = DdrApp_DdrEccConfig();

    if (CSL_PASS != cslRet)
    {
        UART_printf( "\r\nECC Config failed...");
    }
 
    return;
}

int32_t DdrApp_DdrDisableECC(CSL_emif_sscfgRegs *ddrCfgAddr)
{
    CSL_EmifConfig emifCfg;
    int32_t cslRet;
    int32_t retVal =0;
    /* Configure ECC addresses and other configuration */
    emifCfg.bEnableMemoryECC = true;
    /* Switched the start and end address to disable the region, keeping enable bit true to avoid address translaion */
    emifCfg.bReadModifyWriteEnable = true;
    emifCfg.bECCCheck = true;
    emifCfg.bWriteAlloc = true;
    emifCfg.ECCThreshold = 1U;
    emifCfg.pMemEccCfg.endAddr[0] = (DDR_ECC_START_ADDR-EMIF_DDR_START_ADDR)/NUM_DDR_SUBSYSTEMS;
    emifCfg.pMemEccCfg.startAddr[0] = (DDR_ECC_END_ADDR-EMIF_DDR_START_ADDR)/NUM_DDR_SUBSYSTEMS;
    emifCfg.pMemEccCfg.endAddr[1] = (DDR_ECC_START_ADDR1-EMIF_DDR_START_ADDR)/NUM_DDR_SUBSYSTEMS;
    emifCfg.pMemEccCfg.startAddr[1] = (DDR_ECC_END_ADDR1-EMIF_DDR_START_ADDR)/NUM_DDR_SUBSYSTEMS;
    emifCfg.pMemEccCfg.endAddr[2] = (DDR_ECC_START_ADDR2-EMIF_DDR_HIMEM_START_ADDR+EMIF_DDR_LOW_MEM_SIZE)/NUM_DDR_SUBSYSTEMS;
    emifCfg.pMemEccCfg.startAddr[2] = (DDR_ECC_END_ADDR2-EMIF_DDR_HIMEM_START_ADDR+EMIF_DDR_LOW_MEM_SIZE)/NUM_DDR_SUBSYSTEMS;

    cslRet = CSL_emifConfig(ddrCfgAddr, &emifCfg);
    if (CSL_PASS != cslRet)
    {
        UART_printf( "\r\n CSL_emifConfig failed");
        retVal = -1;
    }
    return retVal;
}

int32_t DdrApp_DdrEnableECC(CSL_emif_sscfgRegs *ddrCfgAddr)
{
    CSL_EmifConfig emifCfg;
    int32_t cslRet;
    int32_t retVal =0;
    /* Configure ECC addresses and other configuration */
    emifCfg.bEnableMemoryECC = true;
    emifCfg.bReadModifyWriteEnable = true;
    emifCfg.bECCCheck = true;
    emifCfg.bWriteAlloc = true;
    emifCfg.ECCThreshold = 1U;
    emifCfg.pMemEccCfg.startAddr[0] = (DDR_ECC_START_ADDR-EMIF_DDR_START_ADDR)/NUM_DDR_SUBSYSTEMS;
    emifCfg.pMemEccCfg.endAddr[0] = (DDR_ECC_END_ADDR-EMIF_DDR_START_ADDR)/NUM_DDR_SUBSYSTEMS;
    emifCfg.pMemEccCfg.startAddr[1] = (DDR_ECC_START_ADDR1-EMIF_DDR_START_ADDR)/NUM_DDR_SUBSYSTEMS;
    emifCfg.pMemEccCfg.endAddr[1] = (DDR_ECC_END_ADDR1-EMIF_DDR_START_ADDR)/NUM_DDR_SUBSYSTEMS;
    emifCfg.pMemEccCfg.startAddr[2] = (DDR_ECC_START_ADDR2-EMIF_DDR_HIMEM_START_ADDR+EMIF_DDR_LOW_MEM_SIZE)/NUM_DDR_SUBSYSTEMS;
    emifCfg.pMemEccCfg.endAddr[2] = (DDR_ECC_END_ADDR2-EMIF_DDR_HIMEM_START_ADDR+EMIF_DDR_LOW_MEM_SIZE)/NUM_DDR_SUBSYSTEMS;

    cslRet = CSL_emifConfig(ddrCfgAddr, &emifCfg);
    if (CSL_PASS != cslRet)
    {
        UART_printf( "\r\n CSL_emifConfig failed");
        retVal = -1;
    }
    return retVal;
}

int32_t DdrApp_DdrUdmaPrimeECC()
{
    int32_t         retVal;
    Udma_DrvHandle  drvHandle = &gECCDDRUdmaDrvObj;
    retVal = DdrApp_EccDdrUdmaInit(drvHandle);
    if (retVal != 0 )
    {
        UART_printf("[DMA] BOARD UDMA init failed!!\n");
    }

    /* Start DDR primeing */
    DdrApp_EccDdrUdmaPrimeDDR(EMIF_DDR_START_ADDR, EMIF_DDR_LOW_MEM_SIZE);
    CacheP_Inv((void *)EMIF_DDR_START_ADDR, EMIF_DDR_LOW_MEM_SIZE);
    UART_printf( "\nPrime DDR Low Mem Done");
    DdrApp_EccDdrUdmaPrimeDDR(EMIF_DDR_HIMEM_START_ADDR, EMIF_DDR_HIMEM_SIZE);
    UART_printf( "\nPrime DDR High Mem Done");

    retVal += DdrApp_EccDdrUdmaDeInit(drvHandle);
    if (retVal != 0)
    {
        UART_printf("[DMA] BOARD UDMA deinit failed!!\n");
    }

    return 0;
}

/* Function configures DDR ECC and also primes the memory area */
int32_t DdrApp_DdrEccConfig()
{
    int32_t    cslResult = CSL_PASS;
    int32_t    retVal = 0;

    UART_printf( "\r\n Enabling ECC for DDRSS0 ");
    retVal = DdrApp_DdrEnableECC((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
    UART_printf( "\r\n Enabling ECC for DDRSS1 ");
    retVal = DdrApp_DdrEnableECC((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
#if defined (SOC_J784S4)
    UART_printf( "\r\n Enabling ECC for DDRSS2 ");
    retVal = DdrApp_DdrEnableECC((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
    UART_printf( "\r\n Enabling ECC for DDRSS3 ");
    retVal = DdrApp_DdrEnableECC((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
#endif

    if ( retVal == 0 )
    {
        /* Prime DDR memory using UDMA */
        DdrApp_DdrUdmaPrimeECC();

        /* Clear any residual ECC errors */
        cslResult = CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
        if (CSL_PASS != cslResult )
        {
            UART_printf( "\r\n CSL_emifClearECCErrors failed for DDRSS0 ");
            retVal = -1;
        }
        cslResult = CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
        if (CSL_PASS != cslResult )
        {
            UART_printf( "\r\n CSL_emifClearECCErrors failed for DDRSS1");
            retVal = -1;
        }
#if defined (SOC_J784S4)
        cslResult = CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
        if (CSL_PASS != cslResult )
        {
            UART_printf( "\r\n CSL_emifClearECCErrors failed for DDRSS2 ");
            retVal = -1;
        }
        cslResult = CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
        if (CSL_PASS != cslResult )
        {
            UART_printf( "\r\n CSL_emifClearECCErrors failed for DDRSS3");
            retVal = -1;
        }
#endif
    }

    if ( retVal == 0 )
    {
        /* clear interrupt status */
        cslResult = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (CSL_PASS != cslResult )
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus failed for DDRSS0");
            retVal = -1;
        }
        /* clear interrupt status */
        cslResult = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (CSL_PASS != cslResult )
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus failed for DDRSS1");
            retVal = -1;
        }
#if defined (SOC_J784S4)
        cslResult = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (CSL_PASS != cslResult )
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus failed for DDRSS2");
            retVal = -1;
        }
        /* clear interrupt status */
        cslResult = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (CSL_PASS != cslResult )
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus failed for DDRSS3");
            retVal = -1;
        }
#endif
    }

    if ( retVal == 0 )
    {
        /* Configure ECC interrupts : 1 bit error , 1 bit error multiple and 2 bit errors */
        cslResult = CSL_emifEnableECCInterrupts((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (CSL_PASS != cslResult )
        {
            UART_printf( "\r\n CSL_emifEnableECCInterrupts for DDRSS0 ");
            retVal = -1;
        }
        /* Configure ECC interrupts : 1 bit error , 1 bit error multiple and 2 bit errors */
        cslResult = CSL_emifEnableECCInterrupts((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (CSL_PASS != cslResult )
        {
            UART_printf( "\r\n CSL_emifEnableECCInterrupts for DDRSS1");
            retVal = -1;
        }
#if defined (SOC_J784S4)
        cslResult = CSL_emifEnableECCInterrupts((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (CSL_PASS != cslResult )
        {
            UART_printf( "\r\n CSL_emifEnableECCInterrupts for DDRSS2 ");
            retVal = -1;
        }
        /* Configure ECC interrupts : 1 bit error , 1 bit error multiple and 2 bit errors */
        cslResult = CSL_emifEnableECCInterrupts((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (CSL_PASS != cslResult )
        {
            UART_printf( "\r\n CSL_emifEnableECCInterrupts for DDRSS3");
            retVal = -1;
        }
#endif
    }

    if ( retVal != 0 )
    {
        UART_printf( "\r\nDDR ECC Configuration failed...");
    }

    return (retVal);
}

/* Function performs DDR single bit error test
 * Configures ECC, Inserts single bit error
 * and waits for handler to finish
 */
int32_t DdrApp_DdrSecErrTest(uint32_t test_address)
{
    int32_t  retVal = CSL_EFAIL;
    volatile uint32_t testVal2;
    uint32_t waitCount = 0;

    UART_printf( "\r\n Test Address: 0x%x", test_address);

    /* set the test status to false */
    gSecTestPass = FALSE;

    /* Clear any residual ECC errors */
    CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
    CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
#if defined (SOC_J784S4)
    CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
    CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
#endif

    /* Inject error */
    gTest_Addr = (uint32_t *) (test_address);
    /* Write back any pending writes */
    CacheP_wbInv((const void *)gTest_Addr, 4);

    /* Read value from test location */
    testVal = gTest_Addr[0];

    /* Flip one bit to introduce error */
    testVal2       = testVal ^ 0x00010000u;

    /* No need to translate the address */

    /* NOTE: The following section is test code only cannot be used in real application */
    /* ================================================================================ */

    /* Temporarily disable ECC */
    retVal = DdrApp_DdrDisableECC((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
    retVal = DdrApp_DdrDisableECC((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
#if defined (SOC_J784S4)
    retVal = DdrApp_DdrDisableECC((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
    retVal = DdrApp_DdrDisableECC((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
#endif

    /* Now corrupt the value */
    *(gTest_Addr) = testVal2;
    CacheP_wbInv((const void *)gTest_Addr, 4);

    /* Enable back ECC */
    retVal = DdrApp_DdrEnableECC((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
    retVal = DdrApp_DdrEnableECC((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
#if defined (SOC_J784S4)
    retVal = DdrApp_DdrEnableECC((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
    retVal = DdrApp_DdrEnableECC((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
#endif

    /* ================================================================================ */

    /* Invalidate cache */
    CacheP_Inv((const void *)gTest_Addr, 4);

    /* Read value to trigger error */
    testVal2 = gTest_Addr[0];

    /* wait until the test passes */
    UART_printf( "\r\n Waiting on SEC Interrupt...");

    while ((gSecTestPass == FALSE) && (waitCount++ < 100u)) {
        Osal_delay(10);
    }
    if (gSecTestPass == TRUE) {
        UART_printf( "\r\n Got it. DDR ECC SEC TEST PASSED");
        retVal = CSL_PASS;
    }
    else
    {
        UART_printf( "\r\n DDR ECC SEC TEST FAILED.");
        retVal = CSL_EFAIL;
    }

    /* Restore original value */
    gTest_Addr[0] = testVal;

    /* Write back any pending writes */
    CacheP_wbInv((const void *)gTest_Addr, 4);

    TEST_ASSERT_EQUAL_INT32(CSL_PASS, retVal);
    /* return the test status */
    return retVal;
}

/* Function performs DDR double bit error test
 * Inserts double bit error and waits for handler to finish
 */
static int32_t DdrApp_DdrDedErrTest(uint32_t test_address)
{
    int32_t  retVal = CSL_EFAIL;
    volatile uint32_t testVal;
    volatile uint32_t testVal2;    
    uint32_t waitCount = 0;

    gDedTestPass = FALSE;

    UART_printf( "\r\n Test Address: 0x%x", test_address);

    /* Clear any residual ECC errors */
    CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
    CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
#if defined (SOC_J784S4)
    CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
    CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
#endif

    gTest_Addr = (uint32_t *) (test_address);
        
    CacheP_wbInv((const void *)gTest_Addr, 4);
     /* Read reference value */
    testVal       = gTest_Addr[0];
    /* flip 2 bits */
    testVal2       = testVal ^ 0x00101000u;

    /* No need to translate the address */

    /* NOTE: The following section is test code only cannot be used in real application */
    /* ================================================================================ */

    /* Temporarily disable ECC */
    retVal = DdrApp_DdrDisableECC((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
    retVal = DdrApp_DdrDisableECC((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
#if defined (SOC_J784S4)
    retVal = DdrApp_DdrDisableECC((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
    retVal = DdrApp_DdrDisableECC((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
#endif

    /* Now corrupt the value */
    *(gTest_Addr) = testVal2;

    /* Make sure the values are written back */
    CacheP_wbInv((const void *)gTest_Addr, 4);

    /* Enable back ECC */
    retVal = DdrApp_DdrEnableECC((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
    retVal = DdrApp_DdrEnableECC((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
#if defined (SOC_J784S4)
    retVal = DdrApp_DdrEnableECC((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
    retVal = DdrApp_DdrEnableECC((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
#endif

    /* ================================================================================ */

    /* Invalidate cache */
    CacheP_Inv((const void *)gTest_Addr, 4);

    /* Read value to trigger error */
    testVal2 = gTest_Addr[0];

    /* wait until the test passes */
    UART_printf( "\r\n Waiting on DED interrupt...");
    while ((gDedTestPass == FALSE) && (waitCount++ < 10u)) {
        Osal_delay(1000);
    }

    if (gDedTestPass == TRUE) {
        UART_printf( "\r\n Got it. DDR ECC DED TEST PASSED");
        retVal = CSL_PASS;
    }
    else
    {
        UART_printf( "\r\n DDR ECC DED TEST FAILED.");
        retVal = CSL_EFAIL;
    }

    /* Restore original value */
    gTest_Addr[0] = testVal;
    
    /* return the test status */
    return retVal;
}

/* Nothing past this point */