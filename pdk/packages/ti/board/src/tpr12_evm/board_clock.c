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

 /** \file board_clock.c
  * 
  *  \brief TPR12_EVM Board library clock config file.
  *
  *  PRCM manages clocks, resets, and power domain control of subsystems and
  *  modules inside the device.The Master subsystem TOPRCM module controls all
  *  the Subsystem Resets and Clocks.SubSystem RCM modules control their
  *  respective subsystem modules.
  *
  */

#include "board_clock.h"

/**
 *
 * \brief  clock control register base address
 *
 * This array gives the clock source and clock divider register base addresses
 * for different modules based on the corresponding cores.
 *
 */
uint32_t Board_Clk_base_addr[] = {
    /* MSS Core modules clock base address */
    BOARD_MSS_RCM_U_BASE,
    /* DSS Core modules clock base address */
    BOARD_DSS_RCM_U_BASE,
    /* RCSS Core modules clock base address */
    BOARD_RCSS_RCM_U_BASE
};

/**
 *
 * \brief  Modules clock configurations
 *
 * This structure defines the different module's clock configuration
 * informations like core id, module id,  clock source and divder values.
 *
 */
const Board_Clk_config_t Board_Clk_config[BOARD_TOTAL_MODULE_NUM] = {
    /* CSL_coreType core, clkSrcSelOffset, clkSrcDivOffset, clkSrc, clkDiv */

    /* MSS core modules */
    {BOARD_MSS_CORE, BOARD_MSS_MCANA_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_MCANA_CLK_DIV_VAL_OFFSET,  0x444, 0x444},

    {BOARD_MSS_CORE, BOARD_MSS_MCANB_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_MCANB_CLK_DIV_VAL_OFFSET, 0x444, 0x444},

    {BOARD_MSS_CORE, BOARD_MSS_QSPI_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_QSPI_CLK_DIV_VAL_OFFSET, 0x444, 0x444},

    {BOARD_MSS_CORE, BOARD_MSS_RTIA_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_RTIA_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_MSS_CORE, BOARD_MSS_RTIB_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_RTIB_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_MSS_CORE, BOARD_MSS_RTIC_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_RTIC_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_MSS_CORE, BOARD_MSS_WDT_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_WDT_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_MSS_CORE, BOARD_MSS_SPIA_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_SPIA_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_MSS_CORE, BOARD_MSS_SPIB_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_SPIB_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_MSS_CORE, BOARD_MSS_I2C_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_I2C_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_MSS_CORE, BOARD_MSS_SCIA_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_SCIA_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_MSS_CORE, BOARD_MSS_SCIB_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_SCIB_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_MSS_CORE, BOARD_MSS_CPTS_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_CPTS_CLK_DIV_VAL_OFFSET, 0x444, 0x111},

    {BOARD_MSS_CORE, BOARD_MSS_CPSW_CLK_SRC_SEL_OFFSET,
     BOARD_MSS_CPSW_CLK_DIV_VAL_OFFSET, 0x444, 0x111},

    /* DSP core modules */
    {BOARD_DSP_CORE,  BOARD_DSS_RTIA_CLK_SRC_SEL_OFFSET,
     BOARD_DSS_RTIA_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_DSP_CORE,  BOARD_DSS_RTIB_CLK_SRC_SEL_OFFSET,
     BOARD_DSS_RTIB_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_DSP_CORE,  BOARD_DSS_WDT_CLK_SRC_SEL_OFFSET,
    BOARD_DSS_WDT_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_DSP_CORE,  BOARD_DSS_SCIA_CLK_SRC_SEL_OFFSET,
     BOARD_DSS_SCIA_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    /*RCSS core modules */
    {BOARD_RCSS_CORE,  BOARD_RCSS_I2CA_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_I2CA_CLK_DIV_VAL_OFFSET,  0x222, 0x000},

    {BOARD_RCSS_CORE,  BOARD_RCSS_I2CB_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_I2CB_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_RCSS_CORE,  BOARD_RCSS_SCIA_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_SCIA_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_RCSS_CORE,  BOARD_RCSS_SPIA_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_SPIA_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_RCSS_CORE,  BOARD_RCSS_SPIB_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_SPIB_CLK_DIV_VAL_OFFSET, 0x222, 0x000},

    {BOARD_RCSS_CORE,  BOARD_RCSS_ATL_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_ATL_CLK_DIV_VAL_OFFSET, 0x666, 0x111},

    {BOARD_RCSS_CORE,  BOARD_RCSS_MCASPA_REF0_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_MCASPA_REF0_CLK_DIV_VAL_OFFSET, 0x111, 0x111},

    {BOARD_RCSS_CORE,  BOARD_RCSS_MCASPA_REF1_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_MCASPA_REF1_CLK_DIV_VAL_OFFSET, 0x111, 0x111},

    {BOARD_RCSS_CORE,  BOARD_RCSS_MCASPA_AUX_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_MCASPA_AUX_CLK_DIV_VAL_OFFSET, 0x222, 0x111},

    {BOARD_RCSS_CORE,  BOARD_RCSS_MCASPB_REF0_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_MCASPB_REF0_CLK_DIV_VAL_OFFSET, 0x111, 0x111},

    {BOARD_RCSS_CORE,  BOARD_RCSS_MCASPB_REF1_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_MCASPB_REF1_CLK_DIV_VAL_OFFSET, 0x111, 0x111},

    {BOARD_RCSS_CORE,  BOARD_RCSS_MCASPB_AUX_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_MCASPB_AUX_CLK_DIV_VAL_OFFSET, 0x222, 0x111},

    {BOARD_RCSS_CORE,  BOARD_RCSS_MCASPC_REF0_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_MCASPC_REF0_CLK_DIV_VAL_OFFSET, 0x111, 0x111},

    {BOARD_RCSS_CORE,  BOARD_RCSS_MCASPC_REF1_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_MCASPC_REF1_CLK_DIV_VAL_OFFSET, 0x111, 0x111},

    {BOARD_RCSS_CORE,  BOARD_RCSS_MCASPC_AUX_CLK_SRC_SEL_OFFSET,
     BOARD_RCSS_MCASPC_AUX_CLK_DIV_VAL_OFFSET, 0x222, 0x111}

};

/**
 * \brief  module clock programming function
 *
 * This function configures the clock source and divider values to module
 * clock control registers.
 *  
 * \param  Board_Clk_config_t *data [IN] module clock config structure pointer
 *
 * \return None
 */
static void Board_moduleClockProgram (const Board_Clk_config_t *data)
{
    uint32_t clkSrcBaseAddr;
    uint32_t clkDivBaseAddr;

    /* Get the clock source select register offset address */
    clkSrcBaseAddr = Board_Clk_base_addr[data->core] + data->clkSrcSelOffset;
    /* Get the clock division register offset address */
    clkDivBaseAddr = Board_Clk_base_addr[data->core] + data->clkSrcDivOffset;
    
    /* Register the clock source and divder values to the module's clock
    registers */
    HW_WR_REG32(clkSrcBaseAddr, data->clkSrcVal);
    HW_WR_REG32(clkDivBaseAddr, data->clkDivVal);

}

/**
 * \brief clock Initialization function
 *
 * Enables the clock for all the modules and subsystems by registering the
 * clock source and clokck divider values into their respective clock control
 * registers.
 *
 * \return  BOARD_SOK              - Clock initialization sucessful.
 *          BOARD_INIT_CLOCK_FAIL  - Clock initialization failed.
 *
 */
Board_STATUS Board_moduleClockInit(void)
{
    uint32_t index;

    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + BOARD_MSS_CR5_DIV_VAL, 0x000);

    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + BOARD_MSS_CR5_CLK_SRC_SEL,
    0x222);

    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + BOARD_SYS_CLK_DIV_VAL, 0x111);

    /* Generate 96 MHz CSIRX Control Clock */
    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + BOARD_MSS_CSIRX_CLK_SRC_SEL_OFFSET, 0x222);
    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + BOARD_MSS_CSIRX_CLK_DIV_VAL_OFFSET, 0x000);

    /* Add more modules, add DSP and RCSS */
    HW_WR_REG32(CSL_DSS_RCM_U_BASE + BOARD_DSS_DSP_CLK_DIV_VAL, 0x000);
    HW_WR_REG32(CSL_DSS_RCM_U_BASE + BOARD_DSS_DSP_CLK_SRC_SEL, 0x222);

    for(index = 0; index < BOARD_TOTAL_MODULE_NUM; index++)
    {
        Board_moduleClockProgram(&Board_Clk_config[index]);
    }

    return BOARD_SOK;
}
