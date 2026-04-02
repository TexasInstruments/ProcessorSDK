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

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* UDMA driver object */
struct Udma_DrvObj      gECCDDRUdmaDrvObj;

volatile uint32_t gSecTestPass;
volatile uint32_t gDedTestPass;

volatile uint32_t testVal;

SDL_ESM_config ECC_Test_esmInitConfig_MAIN =
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

static void DDREccInit();
static int32_t DDREccConfig();
static int32_t DDRSecErrTest(uint32_t test_address);
static int32_t DDRDedErrTest(uint32_t test_address);
int32_t DDRUdmaPrimeECC();
int32_t DDREnableECC(CSL_emif_sscfgRegs *ddrCfgAddr);
int32_t DDRDisableECC(CSL_emif_sscfgRegs *ddrCfgAddr);
int32_t SDL_ESM_applicationCallbackFunction(SDL_ESM_Inst esmInstType,
                                                   SDL_ESM_IntType esmIntType,
                                                   uint32_t grpChannel,
                                                   uint32_t index,
                                                   uint32_t intSrc,
                                                   void *arg);

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

static int32_t DDREccHighMemRatConfig(uint32_t ratRegion)
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

int32_t DDREccTest(void)
{
    int32_t retVal = CSL_PASS;
    SDL_ErrType_t result = 0;

    /* Initialize Test variables */
    gSecTestPass   = FALSE;
    gDedTestPass   = FALSE;

    retVal = DDREccHighMemRatConfig(DDR_HIMEM_RAT_REGION);
    if (retVal != CSL_PASS)
    {
        /* print error and quit */
        UART_printf("DDR ECC: RAT configuration failed\n");
    }

    /* Initialize MAIN ESM module */
    retVal = SDL_ESM_init(SDL_ESM_INST_MAIN_ESM0, &ECC_Test_esmInitConfig_MAIN, SDL_ESM_applicationCallbackFunction, NULL);
    if (retVal != CSL_PASS)
    {
        /* print error and quit */
        UART_printf("SDL_ESM_init: Error initializing MAIN ESM: result = %d\n", result);
    }
    else
    {
        UART_printf("\nSDL_ESM_init: Init MAIN ESM complete \n");
    }

    if (retVal == CSL_PASS)
    {
        DDREccInit();
        //Injecting within DDRSS0
        if(DDRSecErrTest(DDRSS0_TEST_ADDRESS) != CSL_PASS)
        {
            retVal = CSL_EFAIL;
        }
        if(DDRDedErrTest(DDRSS0_TEST_ADDRESS) != CSL_PASS)
        {
            retVal = CSL_EFAIL;
        }

        //Injecting within DDRSS1
        if(DDRSecErrTest(DDRSS1_TEST_ADDRESS)!= CSL_PASS)
        {
            retVal = CSL_EFAIL;
        }
        if(DDRDedErrTest(DDRSS1_TEST_ADDRESS)!= CSL_PASS)
        {
            retVal = CSL_EFAIL;
        }

#if defined (SOC_J784S4)
        //Injecting within DDRSS2
        if(DDRSecErrTest(DDRSS2_TEST_ADDRESS)!= CSL_PASS)
        {
            retVal = CSL_EFAIL;
        }
        if(DDRDedErrTest(DDRSS2_TEST_ADDRESS)!= CSL_PASS)
        {
            retVal = CSL_EFAIL;
        }

        //Injecting within DDRSS3
        if(DDRSecErrTest(DDRSS3_TEST_ADDRESS)!= CSL_PASS)
        {
            retVal = CSL_EFAIL;
        }
        if(DDRDedErrTest(DDRSS3_TEST_ADDRESS)!= CSL_PASS)
        {
            retVal = CSL_EFAIL;
        }
#endif

        //Injecting in Himem region within DDRSS0
        if(DDRSecErrTest(DDRSS0_HIMEM_TEST_ADDRESS) != CSL_PASS)
        {
            retVal = CSL_EFAIL;
        }
        //Injecting in Himem region within DDRSS1
        if(DDRSecErrTest(DDRSS1_HIMEM_TEST_ADDRESS) != CSL_PASS)
        {
            retVal = CSL_EFAIL;
        }
#if defined (SOC_J784S4)
        //Injecting in Himem region within DDRSS2
        if(DDRSecErrTest(DDRSS2_HIMEM_TEST_ADDRESS) != CSL_PASS)
        {
            retVal = CSL_EFAIL;
        }
        //Injecting in Himem region within DDRSS3
        if(DDRSecErrTest(DDRSS3_HIMEM_TEST_ADDRESS) != CSL_PASS)
        {
            retVal = CSL_EFAIL;
        }
#endif

        UART_printf( "\r\nDDR ECC test exiting...");
    }

    return retVal;
}

/* Function initialized DDR ECC */
static void DDREccInit()
{
    int32_t cslRet;

    /* Ecc config */
    cslRet = DDREccConfig();

    if (cslRet != CSL_PASS)
    {
        UART_printf( "\r\nECC Config failed...");
    }
 
    return;
}

int32_t DDRDisableECC(CSL_emif_sscfgRegs *ddrCfgAddr)
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
    if (cslRet != CSL_PASS)
    {
        UART_printf( "\r\n CSL_emifConfig failed");
        retVal = -1;
    }
    return retVal;
}

int32_t DDREnableECC(CSL_emif_sscfgRegs *ddrCfgAddr)
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
    if (cslRet != CSL_PASS)
    {
        UART_printf( "\r\n CSL_emifConfig failed");
        retVal = -1;
    }
    return retVal;
}

int32_t DDRUdmaPrimeECC()
{
    int32_t         retVal;
    Udma_DrvHandle  drvHandle = &gECCDDRUdmaDrvObj;
    retVal = ECC_DDR_udmaInit(drvHandle);
    if(retVal != 0 )
    {
        UART_printf("[DMA] BOARD UDMA init failed!!\n");
    }

    /* Start DDR primeing */
    ECC_DDR_udmaPrimeDDR(EMIF_DDR_START_ADDR, EMIF_DDR_LOW_MEM_SIZE);
    CacheP_Inv((void *)EMIF_DDR_START_ADDR, EMIF_DDR_LOW_MEM_SIZE);
    UART_printf( "\nPrime DDR Low Mem Done");
    ECC_DDR_udmaPrimeDDR(EMIF_DDR_HIMEM_START_ADDR, EMIF_DDR_HIMEM_SIZE);
    UART_printf( "\nPrime DDR High Mem Done");

    retVal += ECC_DDR_udmaDeInit(drvHandle);
    if(retVal != 0)
    {
        UART_printf("[DMA] BOARD UDMA deinit failed!!\n");
    }

    return 0;
}

/* Function configures DDR ECC and also primes the memory area */
int32_t DDREccConfig()
{
    int32_t    cslResult = CSL_PASS;
    int32_t    retVal = 0;

    UART_printf( "\r\n Enabling ECC for DDRSS0 ");
    retVal = DDREnableECC((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
    UART_printf( "\r\n Enabling ECC for DDRSS1 ");
    retVal = DDREnableECC((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
#if defined (SOC_J784S4)
    UART_printf( "\r\n Enabling ECC for DDRSS2 ");
    retVal = DDREnableECC((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
    UART_printf( "\r\n Enabling ECC for DDRSS3 ");
    retVal = DDREnableECC((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
#endif

    if ( retVal == 0 )
    {
        /* Prime DDR memory using UDMA */
        DDRUdmaPrimeECC();

        /* Clear any residual ECC errors */
        cslResult = CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
        if (cslResult != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCErrors failed for DDRSS0 ");
            retVal = -1;
        }
        cslResult = CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
        if (cslResult != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCErrors failed for DDRSS1");
            retVal = -1;
        }
#if defined (SOC_J784S4)
        cslResult = CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
        if (cslResult != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCErrors failed for DDRSS2 ");
            retVal = -1;
        }
        cslResult = CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
        if (cslResult != CSL_PASS)
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
        if (cslResult != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus failed for DDRSS0");
            retVal = -1;
        }
        /* clear interrupt status */
        cslResult = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (cslResult != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus failed for DDRSS1");
            retVal = -1;
        }
#if defined (SOC_J784S4)
        cslResult = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (cslResult != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifClearECCInterruptStatus failed for DDRSS2");
            retVal = -1;
        }
        /* clear interrupt status */
        cslResult = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (cslResult != CSL_PASS)
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
        if (cslResult != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifEnableECCInterrupts for DDRSS0 ");
            retVal = -1;
        }
        /* Configure ECC interrupts : 1 bit error , 1 bit error multiple and 2 bit errors */
        cslResult = CSL_emifEnableECCInterrupts((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (cslResult != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifEnableECCInterrupts for DDRSS1");
            retVal = -1;
        }
#if defined (SOC_J784S4)
        cslResult = CSL_emifEnableECCInterrupts((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (cslResult != CSL_PASS)
        {
            UART_printf( "\r\n CSL_emifEnableECCInterrupts for DDRSS2 ");
            retVal = -1;
        }
        /* Configure ECC interrupts : 1 bit error , 1 bit error multiple and 2 bit errors */
        cslResult = CSL_emifEnableECCInterrupts((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE,
                                                CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK
                                                | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
        if (cslResult != CSL_PASS)
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
int32_t DDRSecErrTest(uint32_t test_address)
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
    retVal = DDRDisableECC((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
    retVal = DDRDisableECC((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
#if defined (SOC_J784S4)
    retVal = DDRDisableECC((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
    retVal = DDRDisableECC((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
#endif

    /* Now corrupt the value */
    *(gTest_Addr) = testVal2;
    CacheP_wbInv((const void *)gTest_Addr, 4);

    /* Enable back ECC */
    retVal = DDREnableECC((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
    retVal = DDREnableECC((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
#if defined (SOC_J784S4)
    retVal = DDREnableECC((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
    retVal = DDREnableECC((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
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
static int32_t DDRDedErrTest(uint32_t test_address)
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
    retVal = DDRDisableECC((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
    retVal = DDRDisableECC((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
#if defined (SOC_J784S4)
    retVal = DDRDisableECC((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
    retVal = DDRDisableECC((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
#endif

    /* Now corrupt the value */
    *(gTest_Addr) = testVal2;

    /* Make sure the values are written back */
    CacheP_wbInv((const void *)gTest_Addr, 4);

    /* Enable back ECC */
    retVal = DDREnableECC((CSL_emif_sscfgRegs *)DDRSS0_CFG_BASE);
    retVal = DDREnableECC((CSL_emif_sscfgRegs *)DDRSS1_CFG_BASE);
#if defined (SOC_J784S4)
    retVal = DDREnableECC((CSL_emif_sscfgRegs *)DDRSS2_CFG_BASE);
    retVal = DDREnableECC((CSL_emif_sscfgRegs *)DDRSS3_CFG_BASE);
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