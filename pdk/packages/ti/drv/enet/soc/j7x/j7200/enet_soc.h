/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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

/*!
 * \file  enet_soc.h
 *
 * \brief This file contains J7200 SoC specific definition.
 */

#ifndef ENET_SOC_J7200_H_
#define ENET_SOC_J7200_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/drv/enet/include/core/enet_dma.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/*! \brief Max number of supported channels */
#define ENET_UDMA_NUM_RXCHAN_MAX      (1U)

/*! \brief Main PLL3 HSDIV1 Clockout for CPTS */
#define ENET_MCU_CPSW0_CPTS_CLKSEL_MAIN_PLL3_HSDIV1_CLKOUT      (0x0U)
/*! \brief Main PLL0 HSDIV6 Clockout for CPTS */
#define ENET_MCU_CPSW0_CPTS_CLKSEL_MAIN_PLL0_HSDIV6_CLKOUT      (0x1U)
/*! \brief MCU CPTS Reference Clockout */
#define ENET_MCU_CPSW0_CPTS_CLKSEL_MCU_CPTS_REF_CLK             (0x2U)
/*! \brief CPTS RFT Clock */
#define ENET_MCU_CPSW0_CPTS_CLKSEL_CPTS_RFT_CLK                 (0x3U)
/*! \brief MCU External Reference Clock0 */
#define ENET_MCU_CPSW0_CPTS_CLKSEL_MCU_EXT_REFCLK0              (0x4U)
/*! \brief External Reference Clock 1 for CPTS */
#define ENET_MCU_CPSW0_CPTS_CLKSEL_EXT_REFCLK1                  (0x5U)
/*! \brief SERDES0 IP2 Lane0 Tx MCLK */
#define ENET_MCU_CPSW0_CPTS_CLKSEL_SERDES0_IP2_LN0_TXMCLK       (0x6U)
/*! \brief SERDES0 IP2 Lane1 Tx MCLK */
#define ENET_MCU_CPSW0_CPTS_CLKSEL_SERDES0_IP2_LN1_TXMCLK       (0x7U)
/*! \brief SERDES0 IP2 Lane2 Tx MCLK */
#define ENET_MCU_CPSW0_CPTS_CLKSEL_SERDES0_IP2_LN2_TXMCLK       (0x8U)
/*! \brief SERDES0 IP2 Lane3 Tx MCLK */
#define ENET_MCU_CPSW0_CPTS_CLKSEL_SERDES0_IP2_LN3_TXMCLK       (0x9U)
/*! \brief MCU PLL2 HSDIV1 Clockout */
#define ENET_MCU_CPSW0_CPTS_CLKSEL_MCU_PLL2_HSDIV1_CLKOUT       (0xEU)
/*! \brief MCU System Clock (divided by 2) */
#define ENET_MCU_CPSW0_CPTS_CLKSEL_MCU_SYSCLK0                  (0xFU)

/*! \brief Main PLL3 HSDIV1 Clockout for CPTS */
#define ENET_CPSW0_CPTS_CLKSEL_MAIN_PLL3_HSDIV1_CLKOUT          (0x0U)
/*! \brief Main PLL0 HSDIV6 Clockout for CPTS */
#define ENET_CPSW0_CPTS_CLKSEL_MAIN_PLL0_HSDIV6_CLKOUT          (0x1U)
/*! \brief MCU CPTS Reference Clockout */
#define ENET_CPSW0_CPTS_CLKSEL_MCU_CPTS_REF_CLK                 (0x2U)
/*! \brief CPTS RFT Clock */
#define ENET_CPSW0_CPTS_CLKSEL_CPTS_RFT_CLK                     (0x3U)
/*! \brief MCU External Reference Clock0 */
#define ENET_CPSW0_CPTS_CLKSEL_MCU_EXT_REFCLK0                  (0x4U)
/*! \brief External Reference Clock 1 for CPTS */
#define ENET_CPSW0_CPTS_CLKSEL_EXT_REFCLK1                      (0x5U)
/*! \brief SERDES0 IP2 Lane0 Tx MCLK */
#define ENET_CPSW0_CPTS_CLKSEL_SERDES0_IP2_LN0_TXMCLK           (0x6U)
/*! \brief SERDES0 IP2 Lane1 Tx MCLK */
#define ENET_CPSW0_CPTS_CLKSEL_SERDES0_IP2_LN1_TXMCLK           (0x7U)
/*! \brief SERDES0 IP2 Lane2 Tx MCLK */
#define ENET_CPSW0_CPTS_CLKSEL_SERDES0_IP2_LN2_TXMCLK           (0x8U)
/*! \brief SERDES0 IP2 Lane3 Tx MCLK */
#define ENET_CPSW0_CPTS_CLKSEL_SERDES0_IP2_LN3_TXMCLK           (0x9U)
/*! \brief MCU PLL2 HSDIV1 Clockout */
#define ENET_CPSW0_CPTS_CLKSEL_MCU_PLL2_HSDIV1_CLKOUT           (0xEU)
/*! \brief Main System Clock */
#define ENET_CPSW0_CPTS_CLKSEL_MAIN_SYSCLK0                     (0xFU)

/*! \brief TimeSyncRouter MCU CPSW_2G HW3_PUSH output interrupt number */
#define ENET_TIMESYNCRTR_OUT_MCU_CPSW0_CPTS_HW3_PUSH            (24U)
/*! \brief TimeSyncRouter MCU CPSW_2G HW4_PUSH output interrupt number */
#define ENET_TIMESYNCRTR_OUT_MCU_CPSW0_CPTS_HW4_PUSH            (25U)

/*! \brief TimeSyncRouter CPSW_5G HW1_PUSH output interrupt number */
#define ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW1_PUSH                (26U)
/*! \brief TimeSyncRouter CPSW_5G HW2_PUSH output interrupt number */
#define ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW2_PUSH                (27U)
/*! \brief TimeSyncRouter CPSW_5G HW3_PUSH output interrupt number */
#define ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW3_PUSH                (28U)
/*! \brief TimeSyncRouter CPSW_5G HW4_PUSH output interrupt number */
#define ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW4_PUSH                (29U)
/*! \brief TimeSyncRouter CPSW_5G HW5_PUSH output interrupt number */
#define ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW5_PUSH                (30U)
/*! \brief TimeSyncRouter CPSW_5G HW6_PUSH output interrupt number */
#define ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW6_PUSH                (31U)
/*! \brief TimeSyncRouter CPSW_5G HW7_PUSH output interrupt number */
#define ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW7_PUSH                (32U)
/*! \brief TimeSyncRouter CPSW_5G HW8_PUSH output interrupt number */
#define ENET_TIMESYNCRTR_OUT_CPSW0_CPTS_HW8_PUSH                (33U)

/*! \brief Number of TimeSyncRouter inputs */
#define ENET_TIMESYNCRTR_NUM_INPUT                              (56U)
/*! \brief Number of TimeSyncRouter outputs */
#define ENET_TIMESYNCRTR_NUM_OUTPUT                             (48U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Global Variables Declarations                      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Deprecated Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* ENET_SOC_J7200_H_ */
