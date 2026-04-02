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
 *  \file gtc.c
 *
 *  \brief File containing the Global Time Counter APIs.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <sys/types.h>
#include <ti/drv/gtc/gtc.h>
#include <ti/csl/cslr_gtc.h>
#include <ti/osal/osal.h>
#include <ti/csl/cslr_device.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/drv/sciclient/sciclient.h>

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

CSL_gtc_cfg0Regs *gtcCfg0 = (CSL_gtc_cfg0Regs *) CSL_GTC0_GTC_CFG0_BASE;
CSL_gtc_cfg1Regs *gtcRegs = (CSL_gtc_cfg1Regs *) CSL_GTC0_GTC_CFG1_BASE;
CSL_gtc_cfg2Regs *gtcCfg2 = (CSL_gtc_cfg2Regs *) CSL_GTC0_GTC_CFG2_BASE;


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* Set the GTC FID to the required frequency */
int32_t GTC_setFID(void)
{

    int32_t retVal = CSL_EFAIL;
    uint64_t clkFreqHz;

    retVal = Sciclient_pmGetModuleClkFreq(TISCI_DEV_GTC0,
                                          TISCI_DEV_GTC0_GTC_CLK,
                                          &clkFreqHz,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if(CSL_PASS == retVal)
    {
        gtcRegs->CNTFID0 = (uint32_t)clkFreqHz;
    }
    return retVal;

}

/*-------------------------------------------------------------------------- */

/* Configure the GTC clock for 200 MHz */
int32_t GTC_configClock(void)
{
    int32_t retVal = CSL_EFAIL;
    uint64_t setClkRate;
/**
 * Change the GTC clock parent to MAIN_PLL0_HSDIV6_CLKOUT.
 *
 * Reason: The default GTC clock parent, MAIN_PLL3_HSDIV1_CLKOUT, cannot provide
 * the required frequencies for both GTC (200 MHz) and CPSW2G RGMI (250 MHz)
 * due to the same divider value for MAIN_PLL3_HSDIV1_CLKOUT and MAIN_PLL3_HSDIV0_CLKOUT.
 *
 * For J721S2:
 * - MAIN_PLL3 default frequency: 2 GHz
 * - MAIN_PLL3_HSDIV1_CLKOUT and MAIN_PLL3_HSDIV0_CLKOUT divider value: 8
 *
 * For J784S4/J742S2:
 * - MAIN_PLL3 default frequency: 2.5 GHz
 * - MAIN_PLL3_HSDIV1_CLKOUT and MAIN_PLL3_HSDIV0_CLKOUT divider value: 10
 *
 * By changing the GTC clock parent to MAIN_PLL0_HSDIV6_CLKOUT, we can meet the
 * frequency requirements for both GTC and CPSW2G RGMI.
 */
    #if defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
        retVal = Sciclient_pmSetModuleClkParent(TISCI_DEV_GTC0, 
                                            TISCI_DEV_GTC0_GTC_CLK, 
                                            TISCI_DEV_GTC0_GTC_CLK_PARENT_POSTDIV3_16FFT_MAIN_0_HSDIVOUT6_CLK,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
    #endif

    if(CSL_PASS == retVal)
    {
        /* Set the GTC clock rate to 200 MHz */
        retVal = Sciclient_pmSetModuleClkFreq(TISCI_DEV_GTC0,
                                              TISCI_DEV_GTC0_GTC_CLK,
                                              GTC_CLK_RATE_200_MHZ,
                                              TISCI_MSG_FLAG_AOP,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if(CSL_PASS == retVal)
    {
        /* Check if the set frequency is 200 MHz */
        retVal = Sciclient_pmQueryModuleClkFreq(TISCI_DEV_GTC0,
                                                TISCI_DEV_GTC0_GTC_CLK,
                                                GTC_CLK_RATE_200_MHZ,
                                                &setClkRate,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);

        if(CSL_PASS == retVal)
        {
            if(setClkRate != GTC_CLK_RATE_200_MHZ)
            {
                retVal = CSL_EFAIL;
            }
        }

    }

    return retVal;
}

/*-------------------------------------------------------------------------- */

/* Enable GTC */
void GTC_enable()
{
    uint32_t value = 0U;
    value = gtcRegs->CNTCR;

    /* Enable GTC by setting CNTCR and also set HDBG to stop clock while debug halt is asserted*/
    gtcRegs->CNTCR = (value | CSL_GTC_CFG1_CNTCR_EN_MASK | CSL_GTC_CFG1_CNTCR_HDBG_MASK);
}


/*-------------------------------------------------------------------------- */

/* Disable GTC */
void GTC_disable()
{
    uint32_t value = 0U;
    value = gtcRegs->CNTCR;
    gtcRegs->CNTCR = (value & ~CSL_GTC_CFG1_CNTCR_EN_MASK);
}


/*-------------------------------------------------------------------------- */

/* Do GTC init by configuring clock and setting FID and then enabling the module */
int32_t GTC_init(void)
{
    int32_t retVal = CSL_EFAIL;

    GTC_disable();

    retVal = GTC_configClock();

    if(CSL_PASS == retVal){
        retVal = GTC_setFID();
    }

    /* Clear GTC Counter Values */
    gtcRegs->CNTCV_LO = 0U;
    gtcRegs->CNTCV_HI = 0U;

    return retVal;
}

/*-------------------------------------------------------------------------- */

/* Read the GTC counter value */
uint64_t GTC_readCounter64()
{
    uint64_t count64 = 0U;
    uint32_t countHI = 0U;
    uint32_t countLO = 0U;

    while (true) {
        countHI = gtcRegs->CNTCV_HI;
        countLO = gtcRegs->CNTCV_LO;
        if (countHI == gtcRegs->CNTCV_HI) {
            break;
        }
    }

    count64 = ((uint64_t)countHI << 32) | (uint64_t)countLO;
    return count64;
}

/*-------------------------------------------------------------------------- */

/* Set the GTC counter value */
void GTC_setCounter64(uint64_t count64)
{
    /* GTC must be disabled before setting the counter value */
    gtcRegs->CNTCV_LO = (uint32_t)count64;
    gtcRegs->CNTCV_HI = (uint32_t)(count64 >> 32);

}

/*-------------------------------------------------------------------------- */

/* Select bit to generate Push event */
void GTC_selectPushEvent(uint32_t value)
{
    gtcCfg0->PUSHEVT = (value & CSL_GTC_CFG0_PUSHEVT_EXPBIT_SEL_MASK);
}

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */
