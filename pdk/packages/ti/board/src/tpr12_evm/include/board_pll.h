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
 * \file   board_pll.h
 *
 * \brief  TPR12 EVM PLL configurations header file
 *
 * This file includes the structures, enums and register offsets
 * for PLL configurations
 *
 */

#ifndef BOARD_PLL_H
#define BOARD_PLL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/csl/soc.h>
#include "csl_types.h"
#include "board.h"

/*PLL register offsets */
#define BOARD_MSS_TOPRCM_U_BASE             (CSL_MSS_TOPRCM_U_BASE)

#define BOARD_PLL_CORE_PWRCTRL             (CSL_MSS_TOPRCM_PLL_CORE_PWRCTRL)
#define BOARD_PLL_CORE_CLKCTRL             (CSL_MSS_TOPRCM_PLL_CORE_CLKCTRL)
#define BOARD_PLL_CORE_TENABLE             (CSL_MSS_TOPRCM_PLL_CORE_TENABLE)
#define BOARD_PLL_CORE_TENABLEDIV          (CSL_MSS_TOPRCM_PLL_CORE_TENABLEDIV)
#define BOARD_PLL_CORE_M2NDIV              (CSL_MSS_TOPRCM_PLL_CORE_M2NDIV)
#define BOARD_PLL_CORE_MN2DIV              (CSL_MSS_TOPRCM_PLL_CORE_MN2DIV)
#define BOARD_PLL_CORE_FRACDIV             (CSL_MSS_TOPRCM_PLL_CORE_FRACDIV)
#define BOARD_PLL_CORE_BWCTRL              (CSL_MSS_TOPRCM_PLL_CORE_BWCTRL)
#define BOARD_PLL_CORE_FRACCTRL            (CSL_MSS_TOPRCM_PLL_CORE_FRACCTRL)
#define BOARD_PLL_CORE_STATUS              (CSL_MSS_TOPRCM_PLL_CORE_STATUS)
#define BOARD_PLL_CORE_HSDIVIDER           (CSL_MSS_TOPRCM_PLL_CORE_HSDIVIDER)
#define BOARD_PLL_DSP_PWRCTRL              (CSL_MSS_TOPRCM_PLL_DSP_PWRCTRL)
#define BOARD_PLL_DSP_CLKCTRL              (CSL_MSS_TOPRCM_PLL_DSP_CLKCTRL)
#define BOARD_PLL_DSP_TENABLE              (CSL_MSS_TOPRCM_PLL_DSP_TENABLE)
#define BOARD_PLL_DSP_TENABLEDIV           (CSL_MSS_TOPRCM_PLL_DSP_TENABLEDIV)
#define BOARD_PLL_DSP_M2NDIV               (CSL_MSS_TOPRCM_PLL_DSP_M2NDIV)
#define BOARD_PLL_DSP_MN2DIV               (CSL_MSS_TOPRCM_PLL_DSP_MN2DIV)
#define BOARD_PLL_DSP_FRACDIV              (CSL_MSS_TOPRCM_PLL_DSP_FRACDIV)
#define BOARD_PLL_DSP_BWCTRL               (CSL_MSS_TOPRCM_PLL_DSP_BWCTRL)
#define BOARD_PLL_DSP_FRACCTRL             (CSL_MSS_TOPRCM_PLL_DSP_FRACCTRL)
#define BOARD_PLL_DSP_STATUS               (CSL_MSS_TOPRCM_PLL_DSP_STATUS)
#define BOARD_PLL_DSP_HSDIVIDER            (CSL_MSS_TOPRCM_PLL_DSP_HSDIVIDER)
#define BOARD_PLL_PER_PWRCTRL              (CSL_MSS_TOPRCM_PLL_PER_PWRCTRL)
#define BOARD_PLL_PER_CLKCTRL              (CSL_MSS_TOPRCM_PLL_PER_CLKCTRL)
#define BOARD_PLL_PER_TENABLE              (CSL_MSS_TOPRCM_PLL_PER_TENABLE)
#define BOARD_PLL_PER_TENABLEDIV           (CSL_MSS_TOPRCM_PLL_PER_TENABLEDIV)
#define BOARD_PLL_PER_M2NDIV               (CSL_MSS_TOPRCM_PLL_PER_M2NDIV)
#define BOARD_PLL_PER_MN2DIV               (CSL_MSS_TOPRCM_PLL_PER_MN2DIV)
#define BOARD_PLL_PER_FRACDIV              (CSL_MSS_TOPRCM_PLL_PER_FRACDIV)
#define BOARD_PLL_PER_BWCTRL               (CSL_MSS_TOPRCM_PLL_PER_BWCTRL)
#define BOARD_PLL_PER_FRACCTRL             (CSL_MSS_TOPRCM_PLL_PER_FRACCTRL)
#define BOARD_PLL_PER_STATUS               (CSL_MSS_TOPRCM_PLL_PER_STATUS)
#define BOARD_PLL_PER_HSDIVIDER            (CSL_MSS_TOPRCM_PLL_PER_HSDIVIDER)

/* Offset addresses from the PLL clock cotrol register */
#define BOARD_PLL_CLKCTRL_OFFSET                (0x00U)
#define BOARD_PLL_TENABLE_OFFSET                (0x04U)
#define BOARD_PLL_TENABLEDIV_OFFSET             (0x08U)
#define BOARD_PLL_M2NDIV_OFFSET                 (0x0CU)
#define BOARD_PLL_MN2DIV_OFFSET                 (0x10U)
#define BOARD_PLL_FRACDIV_OFFSET                (0x14U)
#define BOARD_PLL_STATUS_OFFSET                 (0x20U)
#define BOARD_PLL_HSDIVIDER_OFFSET              (0x24U)
#define BOARD_PLL_HSDIVIDER_CLKOUT0_OFFSET      (0x28U)
#define BOARD_PLL_HSDIVIDER_CLKOUT1_OFFSET      (0x2CU)
#define BOARD_PLL_HSDIVIDER_CLKOUT2_OFFSET      (0x30U)
#define BOARD_PLL_HSDIVIDER_CLKOUT3_OFFSET      (0x34U)

/* Total number of PLLs */
#define BOARD_PLL_COUNT                            (3U)

/**
 *
 * \brief PLL controller type
 *
 * This enumeration defines PLL controller types
 *
 */
typedef enum Board_Pll_type_e
{
    BOARD_CORE_PLL,    /*  MCU PLL  */
    BOARD_DSS_PLL,     /*  DSP PLL  */
    BOARD_PER_PLL,     /*  PER PLL  */
} Board_Pll_type;
 
/**
 *
 * \brief PLL clock output type
 *
 * This enumeration defines PLL clock output types
 *
 */
typedef enum Board_Pll_clkout_type_e
{
    BOARD_PLL_CLKOUTLDO,      /*  CLKOUTLDO O/P  */
    BOARD_PLL_CLKOUT,         /*  CLKOUT O/P  */
    BOARD_PLL_CLKDCOLDO,    /*  CLKDCOLDO O/P  */
} Board_Pll_clkout_type;

/**
 *  \brief This structure defines the various Configuration Parameters for
 *         PLL controller.
 *
 */
typedef struct  Board_Pll_config_s
{
    Board_Pll_type pll;
    /**< PLL controller type */
    Board_Pll_clkout_type clkOut;
    /**< PLL clock output type */
    uint32_t mIntMult;
    /**< PLL  feedback multiplier */
    uint32_t nDiv;
    /**< PLL n divider */
    uint32_t m2Div;
    /**< DCO output divider */
    uint32_t n2Div;
    /**< Fractional multiplier */
    uint32_t fracM;
    /**< Bypass divider */
    uint32_t hsDiv3;
    /**< High speed divider 3 */
    uint32_t hsDiv2;
    /**< High speed divider 2 */
    uint32_t hsDiv1;
    /**< High speed divider 1 */
    uint32_t hsDiv0;
    /**< High speed divider 0 */
} Board_Pll_config_t;

/**
 * \brief  Function to initialize all the PLL clocks with default values
 *
 * \return Board_STATUS
 */
Board_STATUS Board_PLLInitAll(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* BOARD_PLL_H */

