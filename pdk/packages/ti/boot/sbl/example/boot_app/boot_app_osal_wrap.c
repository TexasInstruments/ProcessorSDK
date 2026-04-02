/*
 *  Copyright (c) Texas Instruments Incorporated 2024
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
 *  \file     interface.c
 *
 *  \brief    This file implements OSAL interface for SDL functions
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#if defined (SOC_J742S2)
#include <sdlr_soc.h>
#endif
#include <soc.h>
#include <ti/osal/osal.h>
#include <osal/sdl_osal.h>
#include <ti/csl/csl_rat.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define PBIST_RAT_CFG_BASE SDL_MCU_R5FSS0_RAT_CFG_BASE

#define PBIST_RAT_REGION_INDEX            0

/* ========================================================================== */
/*                         Function Declarations                              */
/* ========================================================================== */

pSDL_OSAL_hwipHandle BootApp_registerInterrupt(SDL_OSAL_hwipParams *pParams);
int32_t BootApp_enableInterrupt(uint32_t intNum);
int32_t BootApp_disableInterrupt(uint32_t intNum);
int32_t BootApp_globalDisableInterrupts(uintptr_t *key);
int32_t BootApp_globalRestoreInterrupts(uintptr_t key);
void* BootApp_addrTranslate(uint64_t addr, uint32_t size);

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

SDL_OSAL_Interface osal_interface =
{
    .enableInterrupt = (pSDL_OSAL_interruptFunction) BootApp_enableInterrupt,
    .disableInterrupt = (pSDL_OSAL_interruptFunction) BootApp_disableInterrupt,
    .registerInterrupt = (pSDL_OSAL_registerFunction) BootApp_registerInterrupt,
    .deregisterInterrupt = (pSDL_OSAL_deregisterFunction) HwiP_delete,
    .globalDisableInterrupts = (pSDL_OSAL_globalDisableInterruptsFunction) BootApp_globalDisableInterrupts,
    .globalRestoreInterrupts = (pSDL_OSAL_globalRestoreInterruptsFunction) BootApp_globalRestoreInterrupts,
    .printFxn = (pSDL_OSAL_printFunction) UART_printf,
    .delay = (pSDL_OSAL_delayFunction) Osal_delay,
    .addrTranslate = (pSDL_OSAL_addrTranslateFunction) BootApp_addrTranslate
};

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* If this test is for PSRAM starting at address 0x0, we need special
 * translation using RAT in order to access it for the self test. This is because,
 * in the application, R5F view of 0x0 is mapped to ATCM memory. This
 * flag in the test app allows the address translation function defined the test's
 * OSAL implementation to perform the RAT translation when the address is 0x0 for
 * PSRAM testing */
bool sdl_ecc_psram_test = (bool)false;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

pSDL_OSAL_hwipHandle BootApp_registerInterrupt(SDL_OSAL_hwipParams *pParams)
{
    HwiP_Params hwipParams;
    HwiP_Params_init(&hwipParams);

    hwipParams.arg = pParams->callbackArg;

    return HwiP_create(pParams->intNum, pParams->callback, &hwipParams);
}

int32_t BootApp_enableInterrupt(uint32_t intNum)
{
    HwiP_enableInterrupt(intNum);
    return SDL_PASS;
}

int32_t BootApp_disableInterrupt(uint32_t intNum)
{
    HwiP_disableInterrupt(intNum);
    return SDL_PASS;
}

int32_t BootApp_globalDisableInterrupts(uintptr_t *key)
{
    *key = HwiP_disable();
    return SDL_PASS;
}

int32_t BootApp_globalRestoreInterrupts(uintptr_t key)
{
    HwiP_restore(key);
    return SDL_PASS;
}

/* Note that this example provide a single instance of mappedEccRegs (which is RAT-mapped
 * ECC aggregator configuration registers that lie in larger address space than the 32-bit
 * address space on the MCU.  If more ECC aggregator registers need to be mapped, additional
 * global variables are needed for each set of aggregator registers, and SDL_ECC_init() needs
 * to be modified to make SDL_ECC_mapEccAggrReg() calls for each one that needs to be mapped.
 * The expectation is that this mapping will be retained in perpetuity because in order to obtain
 * information about the ECC errors, the ECC Aggregator configuration registers require to be
 * visible from the MCU. */
#if defined(SOC_J784S4) && defined (ECC_TESTS)
__attribute((section(".my_aggr_reg"))) uint8_t mappedEccRegs[0x400];
__attribute((section(".my_aggr_reg1"))) uint8_t mappedEccRegs1[0x4000]
__attribute((section(".my_aggr_reg2"))) uint8_t mappedEccRegs2[0x4000]
__attribute((section(".my_aggr_reg3"))) uint8_t mappedEccRegs3[0x2000]
__attribute((section(".my_aggr_reg4"))) uint8_t mappedEccRegs4[0x2000]
__attribute((section(".my_aggr_reg5"))) uint8_t mappedEccRegs5[0x2000]
__attribute((section(".my_aggr_reg6"))) uint8_t mappedEccRegs6[0x2000]
__attribute((section(".my_aggr_reg7"))) uint8_t mappedEccRegs7[0x2000]
__attribute((section(".my_aggr_reg8"))) uint8_t mappedEccRegs8[0x2000]
__attribute((section(".my_aggr_reg9"))) uint8_t mappedEccRegs9[0x400];
#else
__attribute((section(".my_aggr_reg"))) uint8_t mappedEccRegs[0x400];
__attribute((section(".my_aggr_reg1"))) uint8_t mappedEccRegs1[0x400];
__attribute((section(".my_aggr_reg2"))) uint8_t mappedEccRegs2[0x400];
__attribute((section(".my_aggr_reg3"))) uint8_t mappedEccRegs3[0x400];
__attribute((section(".my_aggr_reg4"))) uint8_t mappedEccRegs4[0x400];
__attribute((section(".my_aggr_reg5"))) uint8_t mappedEccRegs5[0x400];
__attribute((section(".my_aggr_reg6"))) uint8_t mappedEccRegs6[0x400];
__attribute((section(".my_aggr_reg7"))) uint8_t mappedEccRegs7[0x400];
__attribute((section(".my_aggr_reg8"))) uint8_t mappedEccRegs8[0x400];
__attribute((section(".my_aggr_reg9"))) uint8_t mappedEccRegs9[0x400];
#endif

void* BootApp_addrTranslate(uint64_t addr, uint32_t size)
{
    void * ret = (void *)(-1);
    CSL_RatTranslationCfgInfo translationCfg;
    uint32_t transAddr = (uint32_t)(-1);
    uint32_t index = 0;
    bool result;
    uint32_t offset = 0;

#if defined(SOC_J721S2)
    if ((addr == SDL_COMPUTE_CLUSTER0_MSMC_PBIST0_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_MPU_PBIST0_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_DSP0_PBIST_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_DSP1_PBIST_BASE))
#elif defined(SOC_J784S4) || defined(SOC_J742S2)
    if ((addr == SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_MSMC_PBIST0_CFG_MSMC_PBIST0_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_VBUSP_CFG0_CFG_ARM_PBIST0_0_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_VBUSP_CFG0_CFG_ARM_PBIST0_1_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_VBUSP_CFG1_CFG_ARM_PBIST1_0_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_VBUSP_CFG1_CFG_ARM_PBIST1_1_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_DSP0_PBIST_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_DSP1_PBIST_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_DSP2_PBIST_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW7_CFG_DSP_PBIST7_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW4_CFG_MSMC1_PBIST4_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW5_CFG_MSMC1_PBIST5_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW6_CFG_MSMC1_PBIST6_BASE) ||
        (addr == SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW7_CFG_MSMC1_PBIST7_BASE))
#endif
    {
        /* Disable RAT translation */
        result = CSL_ratDisableRegionTranslation((CSL_ratRegs *)PBIST_RAT_CFG_BASE,
                                                 PBIST_RAT_REGION_INDEX);
        transAddr = (uint32_t)0x60000000;
        index = PBIST_RAT_REGION_INDEX;
    }
    else
    {
            /* Currently, below aggregators configuration registers can be
             * mapped by this code.  To expand to other aggregators, additional
             * instances of "mappedEccRegs" are needed and should be mapped to
             * those additional aggregators. */
            switch(addr)
            {
#if defined(SOC_J721S2)
                case SDL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR0_BASE:
                case SDL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR1_BASE:
                case SDL_COMPUTE_CLUSTER0_MSMC_DDR_0_ECC_AGGR2_BASE:
                    transAddr = (uint32_t)mappedEccRegs2;
                    offset = (addr - SDL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR0_BASE);
                    addr = SDL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR0_BASE;
                    size = 0x1000;
                                        index = 2;
                    break;
                case SDL_COMPUTE_CLUSTER0_ECC_AGGR_BASE:
                    transAddr = (uint32_t)mappedEccRegs;
                                        index = 0;
                    break;
                case SDL_COMPUTE_CLUSTER0_MPU0_COREPAC_ECC_AGGR_BASE:
                case SDL_COMPUTE_CLUSTER0_MPU0_CORE0_ECC_AGGR_BASE:
                case SDL_COMPUTE_CLUSTER0_MPU0_CORE1_ECC_AGGR_BASE:
                    transAddr = (uint32_t)mappedEccRegs3;
                    offset = (addr - SDL_COMPUTE_CLUSTER0_MPU0_COREPAC_ECC_AGGR_BASE);
                    addr = SDL_COMPUTE_CLUSTER0_MPU0_COREPAC_ECC_AGGR_BASE;
                    size = 0x1000;
                                        index = 3;
                    break;
                case SDL_COMPUTE_CLUSTER0_DDR0_0_ECC_AGGR_CTL_BASE:
                case SDL_COMPUTE_CLUSTER0_DDR0_0_ECC_AGGR_CFG_BASE:
                case SDL_COMPUTE_CLUSTER0_DDR0_0_ECC_AGGR_VBUS_BASE:
                case SDL_COMPUTE_CLUSTER0_DDR1_1_ECC_AGGR_CTL_BASE:
                    transAddr = (uint32_t)mappedEccRegs4;
                    offset = (addr - SDL_COMPUTE_CLUSTER0_DDR0_0_ECC_AGGR_CTL_BASE);
                    addr = SDL_COMPUTE_CLUSTER0_DDR0_0_ECC_AGGR_CTL_BASE;
                    size = 0x1000;
                                        index = 4;
                    break;
                case SDL_COMPUTE_CLUSTER0_DDR1_1_ECC_AGGR_CFG_BASE:
                case SDL_COMPUTE_CLUSTER0_DDR1_1_ECC_AGGR_VBUS_BASE:
                    transAddr = (uint32_t)mappedEccRegs5;
                    offset = (addr - SDL_COMPUTE_CLUSTER0_DDR1_1_ECC_AGGR_VBUS_BASE);
                    addr = SDL_COMPUTE_CLUSTER0_DDR1_1_ECC_AGGR_VBUS_BASE;
                    size = 0x1000;
                                        index = 5;
                    break;
                case SDL_COMPUTE_CLUSTER0_MSMC_DDR_1_ECC_AGGR3_BASE:
                    transAddr = (uint32_t)mappedEccRegs6;
                                        index = 6;
                    break;
                case SDL_COMPUTE_CLUSTER0_DSP1_ECCAGGR_BASE:
                    transAddr = (uint32_t)mappedEccRegs7;
                                        index = 7;
                    break;
                case SDL_COMPUTE_CLUSTER0_DSP0_ECC_AGGR_BASE:
                    transAddr = (uint32_t)mappedEccRegs8;
                                        index = 8;
                    break;
                case SDL_MSRAM_512K0_RAM_BASE:
                case SDL_MSRAM_512K1_RAM_BASE:
                    transAddr = (uint32_t)mappedEccRegs9;
                                        index = 9;
                    break;
                case 0x0:
                    if (sdl_ecc_psram_test == (bool)true)
                    {
                        transAddr = (uint32_t)mappedEccRegs9;
                                            index = 9;
                    }
                    break;
#elif defined(SOC_J784S4) || defined(SOC_J742S2)
                case SDL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR0_BASE:
                case SDL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR1_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_MSMC_DDR_0_ECC_AGGR_CFG_MSMC_ECC2_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_MSMC_DDR_1_ECC_AGGR_CFG_MSMC_ECC3_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_MSMC_DDR_2_ECC_AGGR_CFG_MSMC_ECC4_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_MSMC_DDR_3_ECC_AGGR_CFG_MSMC_ECC5_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_MSMC2_ECC_AGGR0_CFG_MSMC_ECC0_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_MSMC2_ECC_AGGR1_CFG_MSMC_ECC1_BASE:
                    transAddr = (uint32_t)mappedEccRegs2;
                    offset = (addr - SDL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR0_BASE);
                    addr = SDL_COMPUTE_CLUSTER0_MSMC_ECC_AGGR0_BASE;
                    size = 0x4000;
                                        index = 2;
                    break;
                case SDL_COMPUTE_CLUSTER0_VBUSP_GICSS_ECC_AGGR_GIC_ECC_AGGR_BASE:
                    transAddr = (uint32_t)mappedEccRegs;
                    offset = (addr - SDL_COMPUTE_CLUSTER0_VBUSP_GICSS_ECC_AGGR_GIC_ECC_AGGR_BASE);
                    addr = SDL_COMPUTE_CLUSTER0_VBUSP_GICSS_ECC_AGGR_GIC_ECC_AGGR_BASE;
                    size = 0x1000;
                                        index = 0;
                    break;

                case SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS0_ECC_AGGR_CTL_BASE:
		case SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS0_ECC_AGGR_CFG_BASE:
		case SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS0_ECC_AGGR_VBUS_BASE:
		case SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS1_ECC_AGGR_CTL_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS1_ECC_AGGR_VBUS_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS1_ECC_AGGR_CFG_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS2_ECC_AGGR_CTL_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS2_ECC_AGGR_VBUS_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS2_ECC_AGGR_CFG_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS3_ECC_AGGR_CTL_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS3_ECC_AGGR_VBUS_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS3_ECC_AGGR_CFG_BASE:
                    transAddr = (uint32_t)mappedEccRegs1;
                    offset = (addr - SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS0_ECC_AGGR_CTL_BASE);
                    addr = SDL_COMPUTE_CLUSTER0_VBUSP_DDRSS0_ECC_AGGR_CTL_BASE;
                    size = 0x4000;
                                        index = 1;
                    break;
                case SDL_COMPUTE_CLUSTER0_VBUSP_CFG0_CFG_ARM_ECC_COREPAC_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_CFG0_CFG_ARM_ECC_CORE0_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_CFG0_CFG_ARM_ECC_CORE1_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_CFG0_CFG_ARM_ECC_CORE2_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_CFG0_CFG_ARM_ECC_CORE3_BASE:
                    transAddr = (uint32_t)mappedEccRegs3;
                    offset = (addr - SDL_COMPUTE_CLUSTER0_VBUSP_CFG0_CFG_ARM_ECC_COREPAC_BASE);
                    addr = SDL_COMPUTE_CLUSTER0_VBUSP_CFG0_CFG_ARM_ECC_COREPAC_BASE;
                    size = 0x2000;
                                        index = 3;
                    break;
                case SDL_COMPUTE_CLUSTER0_VBUSP_CFG1_CFG_ARM_ECC_COREPAC_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_CFG1_CFG_ARM_ECC_CORE0_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_CFG1_CFG_ARM_ECC_CORE1_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_CFG1_CFG_ARM_ECC_CORE2_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP_CFG1_CFG_ARM_ECC_CORE3_BASE:
                    transAddr = (uint32_t)mappedEccRegs4;
                    offset = (addr - SDL_COMPUTE_CLUSTER0_VBUSP_CFG1_CFG_ARM_ECC_COREPAC_BASE);
                    addr = SDL_COMPUTE_CLUSTER0_VBUSP_CFG1_CFG_ARM_ECC_COREPAC_BASE;
                    size = 0x2000;
                                        index = 4;
                    break;
                case SDL_COMPUTE_CLUSTER0_DSP1_ECC_AGGR_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW5_CFG_MSMC1_ECC0_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW5_CFG_MSMC1_ECC1_BASE:
                    transAddr = (uint32_t)mappedEccRegs6;
                    offset = (addr - SDL_COMPUTE_CLUSTER0_DSP1_ECC_AGGR_BASE);
                    addr = SDL_COMPUTE_CLUSTER0_DSP1_ECC_AGGR_BASE;
                    size = 0x2000;
                                        index = 6;
                    break;
                case SDL_COMPUTE_CLUSTER0_DSP0_ECC_AGGR_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW4_CFG_MSMC1_ECC0_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW4_CFG_MSMC1_ECC1_BASE:
                    transAddr = (uint32_t)mappedEccRegs5;
                    offset = (addr - SDL_COMPUTE_CLUSTER0_DSP0_ECC_AGGR_BASE);
                    addr = SDL_COMPUTE_CLUSTER0_DSP0_ECC_AGGR_BASE;
                    size = 0x2000;
                                        index = 5;
                    break;
                case SDL_COMPUTE_CLUSTER0_DSP2_ECC_AGGR_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW6_CFG_MSMC1_ECC0_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW6_CFG_MSMC1_ECC1_BASE:
                    transAddr = (uint32_t)mappedEccRegs7;
                    offset = (addr - SDL_COMPUTE_CLUSTER0_DSP2_ECC_AGGR_BASE);
                    addr = SDL_COMPUTE_CLUSTER0_DSP2_ECC_AGGR_BASE;
                    size = 0x2000;
                                        index = 7;
                    break;
                case SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW7_CFG_DSP_ECCAGGR7_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW7_CFG_MSMC1_ECC0_BASE:
                case SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW7_CFG_MSMC1_ECC1_BASE:
                    transAddr = (uint32_t)mappedEccRegs8;
                    offset = (addr - SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW7_CFG_DSP_ECCAGGR7_BASE);
                    addr = SDL_COMPUTE_CLUSTER0_VBUSP4_CFG_AW7_CFG_DSP_ECCAGGR7_BASE;
                    size = 0x2000;
                                        index = 8;
                    break;
                case SDL_MSRAM_512K0_RAM_BASE:
                case SDL_MSRAM_512K1_RAM_BASE:
                case SDL_MSRAM_512K2_RAM_BASE:
                    transAddr = (uint32_t)mappedEccRegs9;
                                        index = 9;
                    break;
		case 0x0:
                    if (sdl_ecc_psram_test == (bool)true)
                    {
                        transAddr = (uint32_t)mappedEccRegs9;
                                            index = 9;
                    }
                    break;
#endif
                default:
                    break;
            }
    }

    if (transAddr != (uint32_t)(-1))
    {
        /* Add RAT configuration to access address > 32bit address range */
        translationCfg.translatedAddress = addr;
        translationCfg.sizeInBytes = size;
        translationCfg.baseAddress = transAddr;

        /* Set up RAT translation */
        result = CSL_ratConfigRegionTranslation((CSL_ratRegs *)PBIST_RAT_CFG_BASE,
                                                index, &translationCfg);
        if (result == (bool)true ) {
            ret = (void *)(transAddr + offset);
        }
        else {
            UART_printf("\nFailed to configure the RAT index %d address 0x%x\n", index, addr);
	}
    }
    else if (addr < 0x100000000)
    {
        ret = (void *)addr;
    }

    return ret;
}

int32_t BootApp_osalWrapper(void)
{
    SDL_ErrType_t ret = SDL_PASS;

    ret = SDL_OSAL_init(&osal_interface);

    return ret;
}