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

/** \file board_clock.h
*
*   \brief This file contains macros used by board clock module.
*/

#ifndef BOARD_CLOCK_H
#define BOARD_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "board_internal.h"

/* system clock source and clock division register offset address */
#define BOARD_MSS_CR5_DIV_VAL            (CSL_MSS_TOPRCM_MSS_CR5_DIV_VAL)
#define BOARD_MSS_CR5_CLK_SRC_SEL        (CSL_MSS_TOPRCM_MSS_CR5_CLK_SRC_SEL)
#define BOARD_SYS_CLK_DIV_VAL            (CSL_MSS_TOPRCM_SYS_CLK_DIV_VAL)
#define BOARD_DSS_DSP_CLK_SRC_SEL        (CSL_DSS_RCM_DSS_DSP_CLK_SRC_SEL)
#define BOARD_DSS_DSP_CLK_DIV_VAL        (CSL_DSS_RCM_DSS_DSP_CLK_DIV_VAL)

/* Modules clock source and clock division register offset address */
#define BOARD_MSS_MCANA_CLK_SRC_SEL_OFFSET    (CSL_MSS_RCM_MSS_MCANA_CLK_SRC_SEL)
#define BOARD_MSS_MCANB_CLK_SRC_SEL_OFFSET    (CSL_MSS_RCM_MSS_MCANB_CLK_SRC_SEL)
#define BOARD_MSS_QSPI_CLK_SRC_SEL_OFFSET     (CSL_MSS_RCM_MSS_QSPI_CLK_SRC_SEL)
#define BOARD_MSS_RTIA_CLK_SRC_SEL_OFFSET     (CSL_MSS_RCM_MSS_RTIA_CLK_SRC_SEL)
#define BOARD_MSS_RTIB_CLK_SRC_SEL_OFFSET     (CSL_MSS_RCM_MSS_RTIB_CLK_SRC_SEL)
#define BOARD_MSS_RTIC_CLK_SRC_SEL_OFFSET     (CSL_MSS_RCM_MSS_RTIC_CLK_SRC_SEL)
#define BOARD_MSS_WDT_CLK_SRC_SEL_OFFSET      (CSL_MSS_RCM_MSS_WDT_CLK_SRC_SEL)
#define BOARD_MSS_SPIA_CLK_SRC_SEL_OFFSET     (CSL_MSS_RCM_MSS_SPIA_CLK_SRC_SEL)
#define BOARD_MSS_SPIB_CLK_SRC_SEL_OFFSET     (CSL_MSS_RCM_MSS_SPIB_CLK_SRC_SEL)
#define BOARD_MSS_I2C_CLK_SRC_SEL_OFFSET      (CSL_MSS_RCM_MSS_I2C_CLK_SRC_SEL)
#define BOARD_MSS_SCIA_CLK_SRC_SEL_OFFSET     (CSL_MSS_RCM_MSS_SCIA_CLK_SRC_SEL)
#define BOARD_MSS_SCIB_CLK_SRC_SEL_OFFSET     (CSL_MSS_RCM_MSS_SCIB_CLK_SRC_SEL)
#define BOARD_MSS_CPTS_CLK_SRC_SEL_OFFSET     (CSL_MSS_RCM_MSS_CPTS_CLK_SRC_SEL)
#define BOARD_MSS_CPSW_CLK_SRC_SEL_OFFSET     (CSL_MSS_RCM_MSS_CPSW_CLK_SRC_SEL)
#define BOARD_MSS_CSIRX_CLK_SRC_SEL_OFFSET    (CSL_MSS_TOPRCM_CSIRX_CLK_SRC_SEL)

#define BOARD_DSS_RTIA_CLK_SRC_SEL_OFFSET    (CSL_DSS_RCM_DSS_RTIA_CLK_SRC_SEL)
#define BOARD_DSS_RTIB_CLK_SRC_SEL_OFFSET    (CSL_DSS_RCM_DSS_RTIB_CLK_SRC_SEL)
#define BOARD_DSS_WDT_CLK_SRC_SEL_OFFSET     (CSL_DSS_RCM_DSS_WDT_CLK_SRC_SEL)
#define BOARD_DSS_SCIA_CLK_SRC_SEL_OFFSET    (CSL_DSS_RCM_DSS_SCIA_CLK_SRC_SEL)

#define BOARD_RCSS_I2CA_CLK_SRC_SEL_OFFSET   (CSL_RCSS_RCM_RCSS_I2CA_CLK_SRC_SEL)
#define BOARD_RCSS_I2CB_CLK_SRC_SEL_OFFSET   (CSL_RCSS_RCM_RCSS_I2CB_CLK_SRC_SEL)
#define BOARD_RCSS_SCIA_CLK_SRC_SEL_OFFSET   (CSL_RCSS_RCM_RCSS_SCIA_CLK_SRC_SEL)
#define BOARD_RCSS_SPIA_CLK_SRC_SEL_OFFSET   (CSL_RCSS_RCM_RCSS_SPIA_CLK_SRC_SEL)
#define BOARD_RCSS_SPIB_CLK_SRC_SEL_OFFSET   (CSL_RCSS_RCM_RCSS_SPIB_CLK_SRC_SEL)
#define BOARD_RCSS_ATL_CLK_SRC_SEL_OFFSET    (CSL_RCSS_RCM_RCSS_ATL_CLK_SRC_SEL)
#define BOARD_RCSS_MCASPA_REF0_CLK_SRC_SEL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPA_REF0_CLK_SRC_SEL)

#define BOARD_RCSS_MCASPA_REF1_CLK_SRC_SEL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPA_REF1_CLK_SRC_SEL)
#define BOARD_RCSS_MCASPA_AUX_CLK_SRC_SEL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPA_AUX_CLK_SRC_SEL)
#define BOARD_RCSS_MCASPB_REF0_CLK_SRC_SEL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPB_REF0_CLK_SRC_SEL)
#define BOARD_RCSS_MCASPB_REF1_CLK_SRC_SEL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPB_REF1_CLK_SRC_SEL)
#define BOARD_RCSS_MCASPB_AUX_CLK_SRC_SEL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPB_AUX_CLK_SRC_SEL)
#define BOARD_RCSS_MCASPC_REF0_CLK_SRC_SEL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPC_REF0_CLK_SRC_SEL)
#define BOARD_RCSS_MCASPC_REF1_CLK_SRC_SEL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPC_REF1_CLK_SRC_SEL)
#define BOARD_RCSS_MCASPC_AUX_CLK_SRC_SEL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPC_AUX_CLK_SRC_SEL)

#define BOARD_MSS_MCANA_CLK_DIV_VAL_OFFSET    (CSL_MSS_RCM_MSS_MCANA_CLK_DIV_VAL)
#define BOARD_MSS_MCANB_CLK_DIV_VAL_OFFSET    (CSL_MSS_RCM_MSS_MCANB_CLK_DIV_VAL)
#define BOARD_MSS_QSPI_CLK_DIV_VAL_OFFSET     (CSL_MSS_RCM_MSS_QSPI_CLK_DIV_VAL)
#define BOARD_MSS_RTIA_CLK_DIV_VAL_OFFSET     (CSL_MSS_RCM_MSS_RTIA_CLK_DIV_VAL)
#define BOARD_MSS_RTIB_CLK_DIV_VAL_OFFSET     (CSL_MSS_RCM_MSS_RTIB_CLK_DIV_VAL)
#define BOARD_MSS_RTIC_CLK_DIV_VAL_OFFSET     (CSL_MSS_RCM_MSS_RTIC_CLK_DIV_VAL)
#define BOARD_MSS_WDT_CLK_DIV_VAL_OFFSET      (CSL_MSS_RCM_MSS_WDT_CLK_DIV_VAL)
#define BOARD_MSS_SPIA_CLK_DIV_VAL_OFFSET     (CSL_MSS_RCM_MSS_SPIA_CLK_DIV_VAL)
#define BOARD_MSS_SPIB_CLK_DIV_VAL_OFFSET     (CSL_MSS_RCM_MSS_SPIB_CLK_DIV_VAL)
#define BOARD_MSS_I2C_CLK_DIV_VAL_OFFSET      (CSL_MSS_RCM_MSS_I2C_CLK_DIV_VAL)
#define BOARD_MSS_SCIA_CLK_DIV_VAL_OFFSET     (CSL_MSS_RCM_MSS_SCIA_CLK_DIV_VAL)
#define BOARD_MSS_SCIB_CLK_DIV_VAL_OFFSET     (CSL_MSS_RCM_MSS_SCIB_CLK_DIV_VAL)
#define BOARD_MSS_CPTS_CLK_DIV_VAL_OFFSET     (CSL_MSS_RCM_MSS_CPTS_CLK_DIV_VAL)
#define BOARD_MSS_CPSW_CLK_DIV_VAL_OFFSET     (CSL_MSS_RCM_MSS_CPSW_CLK_DIV_VAL)
#define BOARD_MSS_CSIRX_CLK_DIV_VAL_OFFSET    (CSL_MSS_TOPRCM_CSIRX_DIV_VAL)

#define BOARD_DSS_RTIA_CLK_DIV_VAL_OFFSET     (CSL_DSS_RCM_DSS_RTIA_CLK_DIV_VAL)
#define BOARD_DSS_RTIB_CLK_DIV_VAL_OFFSET     (CSL_DSS_RCM_DSS_RTIB_CLK_DIV_VAL)
#define BOARD_DSS_WDT_CLK_DIV_VAL_OFFSET      (CSL_DSS_RCM_DSS_WDT_CLK_DIV_VAL)
#define BOARD_DSS_SCIA_CLK_DIV_VAL_OFFSET     (CSL_DSS_RCM_DSS_SCIA_CLK_DIV_VAL)

#define BOARD_RCSS_I2CA_CLK_DIV_VAL_OFFSET    (CSL_RCSS_RCM_RCSS_I2CA_CLK_DIV_VAL)
#define BOARD_RCSS_I2CB_CLK_DIV_VAL_OFFSET    (CSL_RCSS_RCM_RCSS_I2CB_CLK_DIV_VAL)
#define BOARD_RCSS_SCIA_CLK_DIV_VAL_OFFSET    (CSL_RCSS_RCM_RCSS_SCIA_CLK_DIV_VAL)
#define BOARD_RCSS_SPIA_CLK_DIV_VAL_OFFSET    (CSL_RCSS_RCM_RCSS_SPIA_CLK_DIV_VAL)
#define BOARD_RCSS_SPIB_CLK_DIV_VAL_OFFSET    (CSL_RCSS_RCM_RCSS_SPIB_CLK_DIV_VAL)
#define BOARD_RCSS_ATL_CLK_DIV_VAL_OFFSET     (CSL_RCSS_RCM_RCSS_ATL_CLK_DIV_VAL)
#define BOARD_RCSS_MCASPA_REF0_CLK_DIV_VAL_OFFSET  \
                                    (CSL_RCSS_RCM_RCSS_MCASPA_REF0_CLK_DIV_VAL)

#define BOARD_RCSS_MCASPA_REF1_CLK_DIV_VAL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPA_REF1_CLK_DIV_VAL)
#define BOARD_RCSS_MCASPA_AUX_CLK_DIV_VAL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPA_AUX_CLK_DIV_VAL)
#define BOARD_RCSS_MCASPB_REF0_CLK_DIV_VAL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPB_REF0_CLK_DIV_VAL)
#define BOARD_RCSS_MCASPB_REF1_CLK_DIV_VAL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPB_REF1_CLK_DIV_VAL)
#define BOARD_RCSS_MCASPB_AUX_CLK_DIV_VAL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPB_AUX_CLK_DIV_VAL)
#define BOARD_RCSS_MCASPC_REF0_CLK_DIV_VAL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPC_REF0_CLK_DIV_VAL)
#define BOARD_RCSS_MCASPC_REF1_CLK_DIV_VAL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPC_REF1_CLK_DIV_VAL)
#define BOARD_RCSS_MCASPC_AUX_CLK_DIV_VAL_OFFSET \
                                    (CSL_RCSS_RCM_RCSS_MCASPC_AUX_CLK_DIV_VAL)

#define BOARD_TOTAL_MODULE_NUM    (34U)

/**
 *
 * \brief Module core type
 *
 * This enumeration defines cores corresponding to different modules.
 *
 */
typedef enum Board_Core_s
{
    BOARD_MSS_CORE,    /*  MSS core  */
    BOARD_DSP_CORE,    /*  DSP core  */
    BOARD_RCSS_CORE,   /*  RCSS core  */
} Board_Core_t;

/**
 *  \brief This structure defines the specific cores, module IDs, clock source
 *  and clock divider values for different modules.
 *
 */
typedef struct Board_Clk_config_s
{
    Board_Core_t core;
    /**< core specific to module */
    uint32_t clkSrcSelOffset;
    /**< clock source select register offset */
    uint32_t clkSrcDivOffset;
    /**< clock division register offset */
    uint32_t clkSrcVal;
    /**< clock source select value */
    uint32_t clkDivVal;
    /**< clock source division value*/
} Board_Clk_config_t;

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
Board_STATUS Board_moduleClockInit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* BOARD_CLOCK_H */
