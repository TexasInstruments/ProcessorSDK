/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2018
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
 
#ifndef PMTEST_TESTSLIST_J721E_H_
#define PMTEST_TESTSLIST_J721E_H_

/* ========================================================================== */
/*                                Include Files                               */
/* ========================================================================== */

#include <stdint.h>
#include <ti/drv/sciclient/sciclient.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/**
 * \brief Structure to define an input clock to a module.
 */
typedef struct
{
    uint32_t modId;
    /**< module Id */
    uint32_t clkId;
    /**< clock Id */
    uint64_t clkRate;
    /**< Supported clock rate */
} Pmtest_inputClk_t;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Used to test all possible frequencies for all clocks and modules.*/
#if defined (SOC_J721E)
Pmtest_inputClk_t gPmtest_setFreqTestList[] = {
    /******MCU_ADC0*******/
    {
        TISCI_DEV_MCU_ADC12_16FFC0,
        TISCI_DEV_MCU_ADC12_16FFC0_SYS_CLK,
        0
    },
    {
        TISCI_DEV_MCU_ADC12_16FFC0,
        TISCI_DEV_MCU_ADC12_16FFC0_VBUS_CLK,
        0
    },
    /*HFOSC VALUES*/
    {
        TISCI_DEV_MCU_ADC12_16FFC0,
        TISCI_DEV_MCU_ADC12_16FFC0_ADC_CLK,
        20000000
    },
    {
        TISCI_DEV_MCU_ADC12_16FFC0,
        TISCI_DEV_MCU_ADC12_16FFC0_ADC_CLK,
        24000000
    },
    {
        TISCI_DEV_MCU_ADC12_16FFC0,
        TISCI_DEV_MCU_ADC12_16FFC0_ADC_CLK,
        25000000
    },
    /*MCU_PLL1_HSDIV1_CLKOUT*/
    {
        TISCI_DEV_MCU_ADC12_16FFC0,
        TISCI_DEV_MCU_ADC12_16FFC0_ADC_CLK,
        60000000
    },
    /*MCU_PLL0_HSDIV1_CLKOUT*/
    {
        TISCI_DEV_MCU_ADC12_16FFC0,
        TISCI_DEV_MCU_ADC12_16FFC0_ADC_CLK,
        60000000
    },
    /*MCU_EXT_REFCLK0*/
    {
        TISCI_DEV_MCU_ADC12_16FFC0,
        TISCI_DEV_MCU_ADC12_16FFC0_ADC_CLK,
        100000000
    },
    /******MCU_ADC1*******/
    {
        TISCI_DEV_MCU_ADC12_16FFC1,
        TISCI_DEV_MCU_ADC12_16FFC1_SYS_CLK,
        0
    },
    {
        TISCI_DEV_MCU_ADC12_16FFC1,
        TISCI_DEV_MCU_ADC12_16FFC1_VBUS_CLK,
        0
    },
    /*HFOSC VALUES*/
    {
        TISCI_DEV_MCU_ADC12_16FFC1,
        TISCI_DEV_MCU_ADC12_16FFC1_ADC_CLK,
        20000000
    },
    {
        TISCI_DEV_MCU_ADC12_16FFC1,
        TISCI_DEV_MCU_ADC12_16FFC1_ADC_CLK,
        24000000
    },
    {
        TISCI_DEV_MCU_ADC12_16FFC1,
        TISCI_DEV_MCU_ADC12_16FFC1_ADC_CLK,
        25000000
    },
    /*MCU_PLL1_HSDIV1_CLKOUT*/
    {
        TISCI_DEV_MCU_ADC12_16FFC1,
        TISCI_DEV_MCU_ADC12_16FFC1_ADC_CLK,
        60000000
    },
    /*MCU_PLL0_HSDIV1_CLKOUT*/
    {
        TISCI_DEV_MCU_ADC12_16FFC1,
        TISCI_DEV_MCU_ADC12_16FFC1_ADC_CLK,
        60000000
    },
    /*MCU_EXT_REFCLK0*/
    {
        TISCI_DEV_MCU_ADC12_16FFC1,
        TISCI_DEV_MCU_ADC12_16FFC1_ADC_CLK,
        100000000
    },
    {
        TISCI_DEV_CMPEVENT_INTRTR0,
        TISCI_DEV_CMPEVENT_INTRTR0_INTR_CLK,
        0
    },
    //MCU CPSW
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_GMII1_MR_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_RGMII_MHZ_250_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_CPTS_RFT_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_GMII1_MT_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_RGMII_MHZ_5_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_RMII_MHZ_50_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_RGMII_MHZ_50_CLK,
        0
    },
    /* TODO: This is failing */
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_RGMII_MHZ_50_CLK,
        50000000
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_GMII_RFT_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_CPPI_CLK_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_CPTS_GENF0,
        0
    }
};
#endif

#if defined (SOC_J7200)
Pmtest_inputClk_t gPmtest_setFreqTestList[] = {
    /******MCU_ADC0*******/
    {
        TISCI_DEV_MCU_ADC0,
        TISCI_DEV_MCU_ADC0_SYS_CLK,
        0
    },
    {
        TISCI_DEV_MCU_ADC0,
        TISCI_DEV_MCU_ADC0_VBUS_CLK,
        0
    },
    /*HFOSC VALUES*/
    {
        TISCI_DEV_MCU_ADC0,
        TISCI_DEV_MCU_ADC0_ADC_CLK,
        20000000
    },
    {
        TISCI_DEV_MCU_ADC0,
        TISCI_DEV_MCU_ADC0_ADC_CLK,
        24000000
    },
    {
        TISCI_DEV_MCU_ADC0,
        TISCI_DEV_MCU_ADC0_ADC_CLK,
        25000000
    },
    /*MCU_PLL1_HSDIV1_CLKOUT*/
    {
        TISCI_DEV_MCU_ADC0,
        TISCI_DEV_MCU_ADC0_ADC_CLK,
        60000000
    },
    /*MCU_PLL0_HSDIV1_CLKOUT*/
    {
        TISCI_DEV_MCU_ADC0,
        TISCI_DEV_MCU_ADC0_ADC_CLK,
        60000000
    },
    /*MCU_EXT_REFCLK0*/
    {
        TISCI_DEV_MCU_ADC0,
        TISCI_DEV_MCU_ADC0_ADC_CLK,
        100000000
    },
    /******MCU_ADC1*******/
    {
        TISCI_DEV_MCU_ADC1,
        TISCI_DEV_MCU_ADC1_SYS_CLK,
        0
    },
    {
        TISCI_DEV_MCU_ADC1,
        TISCI_DEV_MCU_ADC1_VBUS_CLK,
        0
    },
    /*HFOSC VALUES*/
    {
        TISCI_DEV_MCU_ADC1,
        TISCI_DEV_MCU_ADC1_ADC_CLK,
        20000000
    },
    {
        TISCI_DEV_MCU_ADC1,
        TISCI_DEV_MCU_ADC1_ADC_CLK,
        24000000
    },
    {
        TISCI_DEV_MCU_ADC1,
        TISCI_DEV_MCU_ADC1_ADC_CLK,
        25000000
    },
    /*MCU_PLL1_HSDIV1_CLKOUT*/
    {
        TISCI_DEV_MCU_ADC1,
        TISCI_DEV_MCU_ADC1_ADC_CLK,
        60000000
    },
    /*MCU_PLL0_HSDIV1_CLKOUT*/
    {
        TISCI_DEV_MCU_ADC1,
        TISCI_DEV_MCU_ADC1_ADC_CLK,
        60000000
    },
    /*MCU_EXT_REFCLK0*/
    {
        TISCI_DEV_MCU_ADC1,
        TISCI_DEV_MCU_ADC1_ADC_CLK,
        100000000
    },
    {
        TISCI_DEV_CMPEVENT_INTRTR0,
        TISCI_DEV_CMPEVENT_INTRTR0_INTR_CLK,
        0
    },
    //MCU CPSW
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_GMII1_MR_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_RGMII_MHZ_250_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_CPTS_RFT_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_GMII1_MT_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_RGMII_MHZ_5_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_RMII_MHZ_50_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_RGMII_MHZ_50_CLK,
        0
    },
    /* TODO: This is failing */
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_RGMII_MHZ_50_CLK,
        50000000
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_GMII_RFT_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_CPPI_CLK_CLK,
        0
    },
    {
        TISCI_DEV_MCU_CPSW0,
        TISCI_DEV_MCU_CPSW0_CPTS_GENF0,
        0
    }
};
#endif

#ifdef __cplusplus
}
#endif
#endif /* PMTEST_TESTSLIST_J721E_H_ */

