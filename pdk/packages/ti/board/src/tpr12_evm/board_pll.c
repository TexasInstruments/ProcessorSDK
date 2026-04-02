/******************************************************************************
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
 *****************************************************************************/

/**
 *
 * \file   board_pll.c
 *
 * \brief  PLL configuration structures
 *
 * This file provides the PLL base addressess and configurations
 * for PLL initialization
 *
 */

#include "board_pll.h"

/**
 *
 * \brief  PLL base address
 *
 * This array gives the  PLL base addressess i.e clock control register
 * for different PLLs.
 *
 */
uint32_t PllRegs[BOARD_PLL_COUNT] = {
    BOARD_MSS_TOPRCM_U_BASE + BOARD_PLL_CORE_CLKCTRL,
    BOARD_MSS_TOPRCM_U_BASE + BOARD_PLL_DSP_CLKCTRL,
    BOARD_MSS_TOPRCM_U_BASE + BOARD_PLL_PER_CLKCTRL
};
/**
 *
 * \brief  PLL configurations
 *
 * This structure gives the different PLL controller configurations
 *
 */
const Board_Pll_config_t pllConfig[BOARD_PLL_COUNT] = {
    /* Board_Pll_type , Board_Pll_clkout_type clkOut,
       mIntMult, nDiv, m2Div, n2Div, fracM,
       hsDiv3, hsDiv2, hsDiv1, hsDiv0 */
    {BOARD_CORE_PLL, BOARD_PLL_CLKDCOLDO,
     2000, 39,  1, 0, 0,
     9, 4, 3, 1},  /* 2000MHz */
    {BOARD_DSS_PLL, BOARD_PLL_CLKDCOLDO,
     900, 19, 1, 0, 0,
     0, 0, 1, 0},  /*900MHz */
    {BOARD_PER_PLL, BOARD_PLL_CLKDCOLDO,
     1728, 39, 1, 0, 0,
     9, 17, 8, 0}  /*1728MHz */
};

/**
 * \brief  PLL programming function
 *
 * This function configures the multiplier/divider values to PLL registers
 * based on PLL type and programs the PLL registers
 *  
 * \param  pllConfig *data [IN] PLL config structure pointer
 *
 * \return None
 */
static void Board_PLLProgram (const Board_Pll_config_t *data)
{
    uint32_t pll_base_addr;

    /**
     *  Initalize variables
     *  All clock values in MHz
     *  Assume some dividers are implemented in reality as +1
     *  (prevents dividing by zero)
     *
     */

    pll_base_addr = PllRegs[data->pll];

    /* write multiplier/divider values to registers independent of PLL type */

    /* bits 11:0 (M_FRAC_MULT) of MSS_TOPRCM_PLL_CORE_MN2DIV */
    CSL_FINS((*(volatile uint32_t *)(pll_base_addr + BOARD_PLL_MN2DIV_OFFSET)),
            MSS_TOPRCM_PLL_CORE_MN2DIV_PLL_CORE_MN2DIV_M, data->mIntMult);

    /* bits 19:16 (N2_div) of MSS_TOPRCM_PLL_CORE_MN2DIV */
    CSL_FINS((*(volatile uint32_t *)(pll_base_addr + BOARD_PLL_MN2DIV_OFFSET)),
            MSS_TOPRCM_PLL_CORE_MN2DIV_PLL_CORE_MN2DIV_N2, data->n2Div);

    /* bits 22:16 (M2_DIV) of MSS_TOPRCM_PLL_CORE_M2NDIV */
    CSL_FINS((*(volatile uint32_t *)(pll_base_addr + BOARD_PLL_M2NDIV_OFFSET)),
            MSS_TOPRCM_PLL_CORE_M2NDIV_PLL_CORE_M2NDIV_M2, data->m2Div);

    /* bits 7:0 (N_div) of MSS_TOPRCM_PLL_CORE_M2NDIV */
    CSL_FINS((*(volatile uint32_t *)(pll_base_addr + BOARD_PLL_M2NDIV_OFFSET)),
            MSS_TOPRCM_PLL_CORE_M2NDIV_PLL_CORE_M2NDIV_N, data->nDiv);

    /* NWELLTRIM[28:24] = 9 IDLE[23] = 0 CLKDCOLDOPWDNZ[17] = 1
    SELFREQDCO[12:10] = 4 */
    HW_WR_REG32(pll_base_addr + BOARD_PLL_CLKCTRL_OFFSET, 0x29131000);

    if(data->clkOut == BOARD_PLL_CLKDCOLDO)
    {
        /* bit 29 CLKDCOLDOEN[29] of MSS_TOPRCM_PLL_CORE_CLCKCTRL */
        CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
        BOARD_PLL_CLKCTRL_OFFSET)),
        MSS_TOPRCM_PLL_CORE_CLKCTRL_PLL_CORE_CLKCTRL_CLKDCOLDOEN, 1);
    }

    if(data->clkOut == BOARD_PLL_CLKOUT)
    {
        /* bit 20 CLKOUTEN[20] of MSS_TOPRCM_PLL_CORE_CLCKCTRL */
        CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
        BOARD_PLL_CLKCTRL_OFFSET)),
        MSS_TOPRCM_PLL_CORE_CLKCTRL_PLL_CORE_CLKCTRL_CLKOUTEN, 1);
    }

    CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_TENABLE_OFFSET)),
    MSS_TOPRCM_PLL_CORE_TENABLE_PLL_CORE_TENABLE_TENABLE, 1);

    CSL_FINSR((*(volatile uint32_t *)(pll_base_addr + BOARD_PLL_CLKCTRL_OFFSET)),
            0, 0, 1);

    /* TINTZ[0] of MSS_TOPRCM_PLL_CORE_CLCKCTRL */
    CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_CLKCTRL_OFFSET)),
    MSS_TOPRCM_PLL_CORE_CLKCTRL_PLL_CORE_CLKCTRL_TINTZ, 1);

    CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_TENABLE_OFFSET)),
    MSS_TOPRCM_PLL_CORE_TENABLE_PLL_CORE_TENABLE_TENABLE, 0);

    CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_TENABLEDIV_OFFSET)),
    MSS_TOPRCM_PLL_CORE_TENABLEDIV_PLL_CORE_TENABLEDIV_TENABLEDIV, 1);

    CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_TENABLEDIV_OFFSET)),
    MSS_TOPRCM_PLL_CORE_TENABLEDIV_PLL_CORE_TENABLEDIV_TENABLEDIV, 0);

    /*  APPLJ-1  :  loop check to PLLLOCK DONE */
    /* poll bit 10 (PHASELOCK) of PLL_STAT for 1 */
    while(!CSL_FEXT((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_STATUS_OFFSET)),
    MSS_TOPRCM_PLL_CORE_STATUS_PLL_CORE_STATUS_PHASELOCK));

    CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_HSDIVIDER_CLKOUT0_OFFSET)),
    MSS_TOPRCM_PLL_CORE_HSDIVIDER_CLKOUT0_PLL_CORE_HSDIVIDER_CLKOUT0_DIV,
    data->hsDiv0);

    CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_HSDIVIDER_CLKOUT1_OFFSET)),
    MSS_TOPRCM_PLL_CORE_HSDIVIDER_CLKOUT1_PLL_CORE_HSDIVIDER_CLKOUT1_DIV,
    data->hsDiv1);

    CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_HSDIVIDER_CLKOUT2_OFFSET)),
    MSS_TOPRCM_PLL_CORE_HSDIVIDER_CLKOUT2_PLL_CORE_HSDIVIDER_CLKOUT2_DIV,
    data->hsDiv2);

    CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_HSDIVIDER_OFFSET)),
    MSS_TOPRCM_PLL_CORE_HSDIVIDER_PLL_CORE_HSDIVIDER_TENABLEDIV, 1);

    CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_HSDIVIDER_OFFSET)),
    MSS_TOPRCM_PLL_CORE_HSDIVIDER_PLL_CORE_HSDIVIDER_TENABLEDIV, 0);

    CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_HSDIVIDER_CLKOUT0_OFFSET)),
    MSS_TOPRCM_PLL_CORE_HSDIVIDER_CLKOUT0_PLL_CORE_HSDIVIDER_CLKOUT0_GATE_CTRL,
    1);

    CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_HSDIVIDER_CLKOUT1_OFFSET)),
    MSS_TOPRCM_PLL_CORE_HSDIVIDER_CLKOUT1_PLL_CORE_HSDIVIDER_CLKOUT1_GATE_CTRL,
    1);

    CSL_FINS((*(volatile uint32_t *)(pll_base_addr +
    BOARD_PLL_HSDIVIDER_CLKOUT2_OFFSET)),
    MSS_TOPRCM_PLL_CORE_HSDIVIDER_CLKOUT2_PLL_CORE_HSDIVIDER_CLKOUT2_GATE_CTRL,
    1);
}

/**
 * \brief  PLL initialization function
 *
 * Configures different PLL controller modules.
 * After reset,PLL initialization procedures must be done properly
 * to set up the PLLs and PLL Controllers
 *
 * \param  data [IN] PLL config structure pointer
 *
 * \return None
 */
static void Board_PLLConfig(const Board_Pll_config_t *data)
{
    /* program multiplier/divider values into PLL/HSDIV */
    Board_PLLProgram(data);
}

/**
 * \brief  Function to initialize all the PLL clocks with default values
 *
 * \return Board_STATUS
 */
Board_STATUS Board_PLLInitAll(void)
{
    Board_STATUS status = BOARD_SOK;

    Board_PLLConfig(&pllConfig[BOARD_DSS_PLL]);
    Board_PLLConfig(&pllConfig[BOARD_PER_PLL]);

    return status;
}
