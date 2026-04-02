/******************************************************************************
 *
 * Copyright (C) 2012-2024 Cadence Design Systems, Inc.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 *
 * dp_sd0801_cfg.h
 *
 ******************************************************************************
 */

/* parasoft-begin-suppress MISRA2012-RULE-1_1_b_c90-2, "More than 31 initial characters in macro", DRV-4788*/
#ifndef DP_SD0801_CFG_H
#define DP_SD0801_CFG_H

/** Used to set-up TX_PSC registers */
#define TX_PSC_A0_VAL 0x00FBU
#define TX_PSC_A2_VAL 0x04AAU
#define TX_PSC_A3_VAL 0x04AAU

/**********************************************************
 * Values used to set-up transceiver clock dividers
 *********************************************************/
#define XCVR_DIAG_HSCLK_DIV_1p62_VAL 0x0002U
#define XCVR_DIAG_HSCLK_DIV_2p16_VAL 0x0001U
#define XCVR_DIAG_HSCLK_DIV_2p43_VAL 0x0001U
#define XCVR_DIAG_HSCLK_DIV_2p70_VAL 0x0001U
#define XCVR_DIAG_HSCLK_DIV_3p24_VAL 0x0002U
#define XCVR_DIAG_HSCLK_DIV_4p32_VAL 0x0000U
#define XCVR_DIAG_HSCLK_DIV_5p40_VAL 0x0000U
#define XCVR_DIAG_HSCLK_DIV_8p10_VAL 0x0000U

#define XCVR_DIAG_BIDI_CTRL_VAL 0x000FU
#define XCVR_DIAG_PLLDRC_CTRL_VAL 0x0001U
#define XCVR_DIAG_HSCLK_SEL_VAL 0x0000U

/**********************************************************
 * Values used to set PLL analog clock dividers
 *********************************************************/
#define CMN_PDIAG_PLL_CLK_SEL_M0_1p62_VAL 0x0F01U
#define CMN_PDIAG_PLL_CLK_SEL_M0_2p16_VAL 0x0701U
#define CMN_PDIAG_PLL_CLK_SEL_M0_2p43_VAL 0x0701U
#define CMN_PDIAG_PLL_CLK_SEL_M0_2p70_VAL 0x0701U
#define CMN_PDIAG_PLL_CLK_SEL_M0_3p24_VAL 0x0B00U
#define CMN_PDIAG_PLL_CLK_SEL_M0_4p32_VAL 0x0301U
#define CMN_PDIAG_PLL_CLK_SEL_M0_5p40_VAL 0x0301U
#define CMN_PDIAG_PLL_CLK_SEL_M0_8p10_VAL 0x0200U

/**********************************************************
 * Values used to disable SSC in common confiuration
 *********************************************************/
#define CMM_PLL_SS_CTRL1_M0_NO_SSC_VAL 0x0002U
#define CMM_PLL_SS_CTRL2_M0_NO_SSC_VAL 0x0000U
#define CMM_PLL_SS_CTRL3_M0_NO_SSC_VAL 0x0000U
#define CMM_PLL_SS_CTRL4_M0_NO_SSC_VAL 0x0000U

#ifdef REF_CLK_19_2MHz

/* Transmitter receiver detect register values */
#define TX_RCVDET_ST_TMR_VAL 0x0780U

/**********************************************************
 * Values used to programming reference clock
 *********************************************************/

#define CMN_SSM_BIAS_TMR_VAL 0x0014U
#define CMN_PLLSM0_PLLPRE_TMR_VAL 0x0027U
#define CMN_PLLSM0_PLLLOCK_TMR_VAL 0x00A1U
#define CMN_PLLSM1_PLLPRE_TMR_VAL 0x0027U
#define CMN_PLLSM1_PLLLOCK_TMR_VAL 0x00A1U
#define CMN_BGCAL_INIT_TMR_VAL 0x0060U
#define CMN_BGCAL_ITER_TMR_VAL 0x0060U
#define CMN_IBCAL_INIT_TMR_VAL 0x0014U
#define CMN_TXPUCAL_INIT_TMR_VAL 0x0018U
#define CMN_TXPUCAL_ITER_TMR_VAL 0x0005U
#define CMN_TXPDCAL_INIT_TMR_VAL 0x0018U
#define CMN_TXPDCAL_ITER_TMR_VAL 0x0005U
#define CMN_RXCAL_INIT_TMR_VAL 0x0240U
#define CMN_RXCAL_ITER_TMR_VAL 0x0005U
#define CMN_SD_CAL_INIT_TMR_VAL 0x0002U
#define CMN_SD_CAL_ITER_TMR_VAL 0x0002U
#define CMN_SD_CAL_REFTIM_START_VAL 0x000BU
#define CMN_SD_CAL_PLLCNT_START_VAL 0x0137U

#define CMN_PDIAG_PLL_CP_PADJ_M0_VAL 0x0509U
#define CMN_PDIAG_PLL_CP_IADJ_M0_VAL 0x0F00U
#define CMN_PDIAG_PLL_FILT_PADJ_M0_VAL 0x0F08U
#define CMN_PLL_DSM_DIAG_M0_VAL 0x0004U
#define CMN_PLL_VCOCAL_INIT_TMR_VAL 0x00C0U
#define CMN_PLL_VCOCAL_ITER_TMR_VAL 0x0004U
#define CMN_PLL_VCOCAL_REFTIM_START_VAL 0x0260U
#define CMN_PLL_VCOCAL_TCTRL_VAL 0x0003U

/**********************************************************
 * Values for SSC specific registers
 *********************************************************/

/* Voltage clock oscilator 10.8 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_10p8_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_10p8_VAL 0x033AU
#define CMN_PLL_SS_CTRL3_M0_VCO_10p8_VAL 0x006AU
#define CMN_PLL_SS_CTRL4_M0_VCO_10p8_VAL 0x0003U

/* Voltage clock oscilator 9.32 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_9p72_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_9p72_VAL 0x05DDU
#define CMN_PLL_SS_CTRL3_M0_VCO_9p72_VAL 0x0069U
#define CMN_PLL_SS_CTRL4_M0_VCO_9p72_VAL 0x0003U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_8p64_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_8p64_VAL 0x0536U
#define CMN_PLL_SS_CTRL3_M0_VCO_8p64_VAL 0x0069U
#define CMN_PLL_SS_CTRL4_M0_VCO_8p64_VAL 0x0003U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_8p1_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_8p1_VAL 0x04D7U
#define CMN_PLL_SS_CTRL3_M0_VCO_8p1_VAL 0x006AU
#define CMN_PLL_SS_CTRL4_M0_VCO_8p1_VAL 0x0003U

/********************************************************
 * Values for VCO specific registers
 *******************************************************/

/* Voltage clock oscilator 10.8 GHz */
#define CMN_PLL_INTDIV_M0_10p8_VAL 0x0119U
#define CMN_PLL_FRACDIVL_M0_10p8_VAL 0x4000U
#define CMN_PLL_FRACDIVH_M0_10p8_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_10p8_VAL 0x00BCU
#define CMN_PDIAG_PLL_CTRL_M0_10p8_VAL 0x0012U

/* Voltage clock oscilator 9.32 GHz */
#define CMN_PLL_INTDIV_M0_9p72_VAL 0x01FAU
#define CMN_PLL_FRACDIVL_M0_9p72_VAL 0x4000U
#define CMN_PLL_FRACDIVH_M0_9p72_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_9p72_VAL 0x0152U
#define CMN_PDIAG_PLL_CTRL_M0_9p72_VAL 0x0002U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_INTDIV_M0_8p64_VAL 0x01C2U
#define CMN_PLL_FRACDIVL_M0_8p64_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_8p64_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_8p64_VAL 0x012CU
#define CMN_PDIAG_PLL_CTRL_M0_8p64_VAL 0x0002U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_INTDIV_M0_8p1_VAL 0x01A5U
#define CMN_PLL_FRACDIVL_M0_8p1_VAL 0xE000U
#define CMN_PLL_FRACDIVH_M0_8p1_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_8p1_VAL 0x011AU
#define CMN_PDIAG_PLL_CTRL_M0_8p1_VAL 0x0002U

/***********************************************************
 * Common values for all Vref clocks
 **********************************************************/
#define CMN_PLL_VCOCAL_PLLCNT_START_SSC_VAL 0x025EU
#define CMN_PLL_VCOCAL_PLLCNT_START_NO_SSC_VAL 0x0260U

#define CMN_PLL_LOCK_PLLCNT_THR_SSC_VAL 0x0005U
#define CMN_PLL_LOCK_PLLCNT_THR_NO_SSC_VAL 0x0003U

#define CMN_PLL_LOCK_PLLCNT_START_SSC_VAL 0x0099U
#define CMN_PLL_LOCK_PLLCNT_START_NO_SSC_VAL 0x0099U

#define CMN_PLL_LOCK_REFCNT_START_VAL 0x0099U

#elif defined REF_CLK_20MHz

/* Transmitter receiver detect register values */
#define TX_RCVDET_ST_TMR_VAL 0x07D0U

/**********************************************************
 * Values used to programming reference clock
 *********************************************************/

#define CMN_SSM_BIAS_TMR_VAL 0x0014U
#define CMN_PLLSM0_PLLPRE_TMR_VAL 0x0028U
#define CMN_PLLSM0_PLLLOCK_TMR_VAL 0x00A8U
#define CMN_PLLSM1_PLLPRE_TMR_VAL 0x0028U
#define CMN_PLLSM1_PLLLOCK_TMR_VAL 0x00A8U
#define CMN_BGCAL_INIT_TMR_VAL 0x0064U
#define CMN_BGCAL_ITER_TMR_VAL 0x0064U
#define CMN_IBCAL_INIT_TMR_VAL 0x0014U
#define CMN_TXPUCAL_INIT_TMR_VAL 0x0018U
#define CMN_TXPUCAL_ITER_TMR_VAL 0x0005U
#define CMN_TXPDCAL_INIT_TMR_VAL 0x0018U
#define CMN_TXPDCAL_ITER_TMR_VAL 0x0005U
#define CMN_RXCAL_INIT_TMR_VAL 0x0258U
#define CMN_RXCAL_ITER_TMR_VAL 0x0005U
#define CMN_SD_CAL_INIT_TMR_VAL 0x0002U
#define CMN_SD_CAL_ITER_TMR_VAL 0x0002U
#define CMN_SD_CAL_REFTIM_START_VAL 0x000BU
#define CMN_SD_CAL_PLLCNT_START_VAL 0x012BU

#define CMN_PDIAG_PLL_CP_PADJ_M0_VAL 0x0509U
#define CMN_PDIAG_PLL_CP_IADJ_M0_VAL 0x0F00U
#define CMN_PDIAG_PLL_FILT_PADJ_M0_VAL 0x0F08U
#define CMN_PLL_DSM_DIAG_M0_VAL 0x0004U
#define CMN_PLL_VCOCAL_INIT_TMR_VAL 0x00C8U
#define CMN_PLL_VCOCAL_ITER_TMR_VAL 0x0004U
#define CMN_PLL_VCOCAL_REFTIM_START_VAL 0x0279U
#define CMN_PLL_VCOCAL_TCTRL_VAL 0x0003U

/**********************************************************
 * Values for SSC specific registers
 *********************************************************/

/* Voltage clock oscilator 10.8 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_10p8_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_10p8_VAL 0x05F8U
#define CMN_PLL_SS_CTRL3_M0_VCO_10p8_VAL 0x006EU
#define CMN_PLL_SS_CTRL4_M0_VCO_10p8_VAL 0x0003U

/* Voltage clock oscilator 9.32 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_9p72_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_9p72_VAL 0x0553U
#define CMN_PLL_SS_CTRL3_M0_VCO_9p72_VAL 0x006FU
#define CMN_PLL_SS_CTRL4_M0_VCO_9p72_VAL 0x0003U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_8p64_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_8p64_VAL 0x04DDU
#define CMN_PLL_SS_CTRL3_M0_VCO_8p64_VAL 0x006CU
#define CMN_PLL_SS_CTRL4_M0_VCO_8p64_VAL 0x0003U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_8p1_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_8p1_VAL 0x047AU
#define CMN_PLL_SS_CTRL3_M0_VCO_8p1_VAL 0x006EU
#define CMN_PLL_SS_CTRL4_M0_VCO_8p1_VAL 0x0003U

/********************************************************
 * Values for VCO specific registers
 *******************************************************/

/* Voltage clock oscilator 10.8 GHz */
#define CMN_PLL_INTDIV_M0_10p8_VAL 0x010EU
#define CMN_PLL_FRACDIVL_M0_10p8_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_10p8_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_10p8_VAL 0x00B4U
#define CMN_PDIAG_PLL_CTRL_M0_10p8_VAL 0x0012U

/* Voltage clock oscilator 9.32 GHz */
#define CMN_PLL_INTDIV_M0_9p72_VAL 0x01E6U
#define CMN_PLL_FRACDIVL_M0_9p72_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_9p72_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_9p72_VAL 0x0144U
#define CMN_PDIAG_PLL_CTRL_M0_9p72_VAL 0x0002U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_INTDIV_M0_8p64_VAL 0x01B0U
#define CMN_PLL_FRACDIVL_M0_8p64_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_8p64_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_8p64_VAL 0x0120U
#define CMN_PDIAG_PLL_CTRL_M0_8p64_VAL 0x0002U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_INTDIV_M0_8p1_VAL 0x0195U
#define CMN_PLL_FRACDIVL_M0_8p1_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_8p1_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_8p1_VAL 0x010EU
#define CMN_PDIAG_PLL_CTRL_M0_8p1_VAL 0x0002U

/***********************************************************
 * Common values for all Vref clocks
 **********************************************************/
#define CMN_PLL_VCOCAL_PLLCNT_START_SSC_VAL 0x0277U
#define CMN_PLL_VCOCAL_PLLCNT_START_NO_SSC_VAL 0x0279U

#define CMN_PLL_LOCK_PLLCNT_THR_SSC_VAL 0x0005U
#define CMN_PLL_LOCK_PLLCNT_THR_NO_SSC_VAL 0x0003U

#define CMN_PLL_LOCK_PLLCNT_START_SSC_VAL 0x009FU
#define CMN_PLL_LOCK_PLLCNT_START_NO_SSC_VAL 0x009FU

#define CMN_PLL_LOCK_REFCNT_START_VAL 0x009FU

#elif defined REF_CLK_24MHz

/* Transmitter receiver detect register values */
#define TX_RCVDET_ST_TMR_VAL 0x0960U

/**********************************************************
 * Values used to programming reference clock
 *********************************************************/

#define CMN_SSM_BIAS_TMR_VAL 0x0018U
#define CMN_PLLSM0_PLLPRE_TMR_VAL 0x0030U
#define CMN_PLLSM0_PLLLOCK_TMR_VAL 0x00C9U
#define CMN_PLLSM1_PLLPRE_TMR_VAL 0x0030U
#define CMN_PLLSM1_PLLLOCK_TMR_VAL 0x00C9U
#define CMN_BGCAL_INIT_TMR_VAL 0x0078U
#define CMN_BGCAL_ITER_TMR_VAL 0x0078U
#define CMN_IBCAL_INIT_TMR_VAL 0x0018U
#define CMN_TXPUCAL_INIT_TMR_VAL 0x001DU
#define CMN_TXPUCAL_ITER_TMR_VAL 0x0006U
#define CMN_TXPDCAL_INIT_TMR_VAL 0x001DU
#define CMN_TXPDCAL_ITER_TMR_VAL 0x0006U
#define CMN_RXCAL_INIT_TMR_VAL 0x02D0U
#define CMN_RXCAL_ITER_TMR_VAL 0x0006U
#define CMN_SD_CAL_INIT_TMR_VAL 0x0002U
#define CMN_SD_CAL_ITER_TMR_VAL 0x0002U
#define CMN_SD_CAL_REFTIM_START_VAL 0x000EU
#define CMN_SD_CAL_PLLCNT_START_VAL 0x0137U

#define CMN_PDIAG_PLL_CP_PADJ_M0_VAL 0x0509U
#define CMN_PDIAG_PLL_CP_IADJ_M0_VAL 0x0F00U
#define CMN_PDIAG_PLL_FILT_PADJ_M0_VAL 0x0F08U
#define CMN_PLL_DSM_DIAG_M0_VAL 0x0004U
#define CMN_PLL_VCOCAL_INIT_TMR_VAL 0x00F0U
#define CMN_PLL_VCOCAL_ITER_TMR_VAL 0x0004U
#define CMN_PLL_VCOCAL_REFTIM_START_VAL 0x02F8U
#define CMN_PLL_VCOCAL_TCTRL_VAL 0x0003U

/**********************************************************
 * Values for SSC specific registers
 *********************************************************/

/* Voltage clock oscilator 10.8 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_10p8_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_10p8_VAL 0x044FU
#define CMN_PLL_SS_CTRL3_M0_VCO_10p8_VAL 0x007FU
#define CMN_PLL_SS_CTRL4_M0_VCO_10p8_VAL 0x0003U

/* Voltage clock oscilator 9.32 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_9p72_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_9p72_VAL 0x0401U
#define CMN_PLL_SS_CTRL3_M0_VCO_9p72_VAL 0x007BU
#define CMN_PLL_SS_CTRL4_M0_VCO_9p72_VAL 0x0003U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_8p64_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_8p64_VAL 0x038FU
#define CMN_PLL_SS_CTRL3_M0_VCO_8p64_VAL 0x007BU
#define CMN_PLL_SS_CTRL4_M0_VCO_8p64_VAL 0x0003U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_8p1_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_8p1_VAL 0x0342U
#define CMN_PLL_SS_CTRL3_M0_VCO_8p1_VAL 0x007EU
#define CMN_PLL_SS_CTRL4_M0_VCO_8p1_VAL 0x006CU

/********************************************************
 * Values for VCO specific registers
 *******************************************************/

/* Voltage clock oscilator 10.8 GHz */
#define CMN_PLL_INTDIV_M0_10p8_VAL 0x01C2U
#define CMN_PLL_FRACDIVL_M0_10p8_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_10p8_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_10p8_VAL 0x012CU
#define CMN_PDIAG_PLL_CTRL_M0_10p8_VAL 0x0002U

/* Voltage clock oscilator 9.32 GHz */
#define CMN_PLL_INTDIV_M0_9p72_VAL 0x0195U
#define CMN_PLL_FRACDIVL_M0_9p72_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_9p72_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_9p72_VAL 0x010EU
#define CMN_PDIAG_PLL_CTRL_M0_9p72_VAL 0x0002U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_INTDIV_M0_8p64_VAL 0x0168U
#define CMN_PLL_FRACDIVL_M0_8p64_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_8p64_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_8p64_VAL 0x00F0U
#define CMN_PDIAG_PLL_CTRL_M0_8p64_VAL 0x0002U

/* Voltage clock oscilator 8.1 GHz */
#define CMN_PLL_INTDIV_M0_8p1_VAL 0x0151U
#define CMN_PLL_FRACDIVL_M0_8p1_VAL 0x8000U
#define CMN_PLL_FRACDIVH_M0_8p1_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_8p1_VAL 0x00E2U
#define CMN_PDIAG_PLL_CTRL_M0_8p1_VAL 0x0002U

/***********************************************************
 * Common values for all Vref clocks
 **********************************************************/
#define CMN_PLL_VCOCAL_PLLCNT_START_SSC_VAL 0x02F6U
#define CMN_PLL_VCOCAL_PLLCNT_START_NO_SSC_VAL 0x02F8U

#define CMN_PLL_LOCK_PLLCNT_THR_SSC_VAL 0x0005U
#define CMN_PLL_LOCK_PLLCNT_THR_NO_SSC_VAL 0x0003U

#define CMN_PLL_LOCK_PLLCNT_SSC_START_VAL 0x00BFU
#define CMN_PLL_LOCK_PLLCNT_NO_SSC_START_VAL 0x00BFU

#define CMN_PLL_LOCK_REFCNT_START_VAL 0x00BFU

#elif defined REF_CLK_26MHz

/* Transmitter receiver detect register values */
#define TX_RCVDET_ST_TMR_VAL 0x0A28U

/**********************************************************
 * Values used to programming reference clock
 *********************************************************/

#define CMN_SSM_BIAS_TMR_VAL 0x001AU
#define CMN_PLLSM0_PLLPRE_TMR_VAL 0x0034U
#define CMN_PLLSM0_PLLLOCK_TMR_VAL 0x00DAU
#define CMN_PLLSM1_PLLPRE_TMR_VAL 0x0034U
#define CMN_PLLSM1_PLLLOCK_TMR_VAL 0x00DAU
#define CMN_BGCAL_INIT_TMR_VAL 0x0082U
#define CMN_BGCAL_ITER_TMR_VAL 0x0082U
#define CMN_IBCAL_INIT_TMR_VAL 0x001AU
#define CMN_TXPUCAL_INIT_TMR_VAL 0x0020U
#define CMN_TXPUCAL_ITER_TMR_VAL 0x0007U
#define CMN_TXPDCAL_INIT_TMR_VAL 0x0020U
#define CMN_TXPDCAL_ITER_TMR_VAL 0x0007U
#define CMN_RXCAL_INIT_TMR_VAL 0x030CU
#define CMN_RXCAL_ITER_TMR_VAL 0x0007U
#define CMN_SD_CAL_INIT_TMR_VAL 0x0003U
#define CMN_SD_CAL_ITER_TMR_VAL 0x0003U
#define CMN_SD_CAL_REFTIM_START_VAL 0x000FU
#define CMN_SD_CAL_PLLCNT_START_VAL 0x0132U

#define CMN_PDIAG_PLL_CP_PADJ_M0_VAL 0x0509U
#define CMN_PDIAG_PLL_CP_IADJ_M0_VAL 0x0F00U
#define CMN_PDIAG_PLL_FILT_PADJ_M0_VAL 0x0F08U
#define CMN_PLL_DSM_DIAG_M0_VAL 0x0004U
#define CMN_PLL_VCOCAL_INIT_TMR_VAL 0x0104U
#define CMN_PLL_VCOCAL_ITER_TMR_VAL 0x0005U
#define CMN_PLL_VCOCAL_REFTIM_START_VAL 0x0337U
#define CMN_PLL_VCOCAL_TCTRL_VAL 0x0003U

/**********************************************************
 * Values for SSC specific registers
 *********************************************************/

/* Voltage clock oscilator 10.8 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_10p8_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_10p8_VAL 0x04C4U
#define CMN_PLL_SS_CTRL3_M0_VCO_10p8_VAL 0x006AU
#define CMN_PLL_SS_CTRL4_M0_VCO_10p8_VAL 0x0004U

/* Voltage clock oscilator 9.32 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_9p72_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_9p72_VAL 0x044AU
#define CMN_PLL_SS_CTRL3_M0_VCO_9p72_VAL 0x006AU
#define CMN_PLL_SS_CTRL4_M0_VCO_9p72_VAL 0x0004U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_8p64_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_8p64_VAL 0x03D0U
#define CMN_PLL_SS_CTRL3_M0_VCO_8p64_VAL 0x006AU
#define CMN_PLL_SS_CTRL4_M0_VCO_8p64_VAL 0x0004U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_8p1_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_8p1_VAL 0x0382U
#define CMN_PLL_SS_CTRL3_M0_VCO_8p1_VAL 0x006CU
#define CMN_PLL_SS_CTRL4_M0_VCO_8p1_VAL 0x0004U

/********************************************************
 * Values for VCO specific registers
 *******************************************************/

/* Voltage clock oscilator 10.8 GHz */
#define CMN_PLL_INTDIV_M0_10p8_VAL 0x019FU
#define CMN_PLL_FRACDIVL_M0_10p8_VAL 0x6276U
#define CMN_PLL_FRACDIVH_M0_10p8_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_10p8_VAL 0x0115U
#define CMN_PDIAG_PLL_CTRL_M0_10p8_VAL 0x0002U

/* Voltage clock oscilator 9.32 GHz */
#define CMN_PLL_INTDIV_M0_9p72_VAL 0x0175U
#define CMN_PLL_FRACDIVL_M0_9p72_VAL 0xD89EU
#define CMN_PLL_FRACDIVH_M0_9p72_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_9p72_VAL 0x00FAU
#define CMN_PDIAG_PLL_CTRL_M0_9p72_VAL 0x0002U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_INTDIV_M0_8p64_VAL 0x014CU
#define CMN_PLL_FRACDIVL_M0_8p64_VAL 0x4EC5U
#define CMN_PLL_FRACDIVH_M0_8p64_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_8p64_VAL 0x00DEU
#define CMN_PDIAG_PLL_CTRL_M0_8p64_VAL 0x0002U

/* Voltage clock oscilator 8.1 GHz */
#define CMN_PLL_INTDIV_M0_8p1_VAL 0x0137U
#define CMN_PLL_FRACDIVL_M0_8p1_VAL 0x89D9U
#define CMN_PLL_FRACDIVH_M0_8p1_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_8p1_VAL 0x00D0U
#define CMN_PDIAG_PLL_CTRL_M0_8p1_VAL 0x0002U

/***********************************************************
 * Common values for all Vref clocks
 **********************************************************/
#define CMN_PLL_VCOCAL_PLLCNT_START_SSC_VAL 0x0335U
#define CMN_PLL_VCOCAL_PLLCNT_START_NO_SSC_VAL 0x0337U

#define CMN_PLL_LOCK_PLLCNT_THR_SSC_VAL 0x0005U
#define CMN_PLL_LOCK_PLLCNT_THR_NO_SSC_VAL 0x0003U

#define CMN_PLL_LOCK_PLLCNT_START_SSC_VAL 0x00CEU
#define CMN_PLL_LOCK_PLLCNT_START_NO_SSC_VAL 0x00CFU

#define CMN_PLL_LOCK_REFCNT_START_VAL 0x00CFU

#elif defined REF_CLK_27MHz

/* Transmitter receiver detect register values */
#define TX_RCVDET_ST_TMR_VAL 0x0A8CU

/**********************************************************
 * Values used to programming reference clock
 *********************************************************/

#define CMN_SSM_BIAS_TMR_VAL 0x001BU
#define CMN_PLLSM0_PLLPRE_TMR_VAL 0x0036U
#define CMN_PLLSM0_PLLLOCK_TMR_VAL 0x00E2U
#define CMN_PLLSM1_PLLPRE_TMR_VAL 0x0036U
#define CMN_PLLSM1_PLLLOCK_TMR_VAL 0xE2U
#define CMN_BGCAL_INIT_TMR_VAL 0x0087U
#define CMN_BGCAL_ITER_TMR_VAL 0x0087U
#define CMN_IBCAL_INIT_TMR_VAL 0x001BU
#define CMN_TXPUCAL_INIT_TMR_VAL 0x0021U
#define CMN_TXPUCAL_ITER_TMR_VAL 0x0007U
#define CMN_TXPDCAL_INIT_TMR_VAL 0x0021U
#define CMN_TXPDCAL_ITER_TMR_VAL 0x0007U
#define CMN_RXCAL_INIT_TMR_VAL 0x032AU
#define CMN_RXCAL_ITER_TMR_VAL 0x0007U
#define CMN_SD_CAL_INIT_TMR_VAL 0x0003U
#define CMN_SD_CAL_ITER_TMR_VAL 0x0003U
#define CMN_SD_CAL_REFTIM_START_VAL 0x0010U
#define CMN_SD_CAL_PLLCNT_START_VAL 0x0139U

#define CMN_PDIAG_PLL_CP_PADJ_M0_VAL 0x0509U
#define CMN_PDIAG_PLL_CP_IADJ_M0_VAL 0x0F00U
#define CMN_PDIAG_PLL_FILT_PADJ_M0_VAL 0x0F08U
#define CMN_PLL_DSM_DIAG_M0_VAL 0x0004U
#define CMN_PLL_VCOCAL_INIT_TMR_VAL 0x010EU
#define CMN_PLL_VCOCAL_ITER_TMR_VAL 0x0005U
#define CMN_PLL_VCOCAL_REFTIM_START_VAL 0x0357U
#define CMN_PLL_VCOCAL_TCTRL_VAL 0x0003U

/**********************************************************
 * Values for SSC specific registers
 *********************************************************/

/* Voltage clock oscilator 10.8 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_10p8_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_10p8_VAL 0x046CU
#define CMN_PLL_SS_CTRL3_M0_VCO_10p8_VAL 0x006EU
#define CMN_PLL_SS_CTRL4_M0_VCO_10p8_VAL 0x0004U

/* Voltage clock oscilator 9.32 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_9p72_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_9p72_VAL 0x0404U
#define CMN_PLL_SS_CTRL3_M0_VCO_9p72_VAL 0x006DU
#define CMN_PLL_SS_CTRL4_M0_VCO_9p72_VAL 0x0004U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_8p64_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_8p64_VAL 0x03A3U
#define CMN_PLL_SS_CTRL3_M0_VCO_8p64_VAL 0x006BU
#define CMN_PLL_SS_CTRL4_M0_VCO_8p64_VAL 0x0004U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_8p1_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_8p1_VAL 0x0351U
#define CMN_PLL_SS_CTRL3_M0_VCO_8p1_VAL 0x006EU
#define CMN_PLL_SS_CTRL4_M0_VCO_8p1_VAL 0x0004U

/********************************************************
 * Values for VCO specific registers
 *******************************************************/

/* Voltage clock oscilator 10.8 GHz */
#define CMN_PLL_INTDIV_M0_10p8_VAL 0x0190U
#define CMN_PLL_FRACDIVL_M0_10p8_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_10p8_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_10p8_VAL 0x010CU
#define CMN_PDIAG_PLL_CTRL_M0_10p8_VAL 0x0002U

/* Voltage clock oscilator 9.32 GHz */
#define CMN_PLL_INTDIV_M0_9p72_VAL 0x0168U
#define CMN_PLL_FRACDIVL_M0_9p72_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_9p72_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_9p72_VAL 0x00F0U
#define CMN_PDIAG_PLL_CTRL_M0_9p72_VAL 0x0002U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_INTDIV_M0_8p64_VAL 0x0140U
#define CMN_PLL_FRACDIVL_M0_8p64_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_8p64_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_8p64_VAL 0x00D6U
#define CMN_PDIAG_PLL_CTRL_M0_8p64_VAL 0x0002U

/* Voltage clock oscilator 8.1 GHz */
#define CMN_PLL_INTDIV_M0_8p1_VAL 0x012CU
#define CMN_PLL_FRACDIVL_M0_8p1_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_8p1_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_8p1_VAL 0x00C8U
#define CMN_PDIAG_PLL_CTRL_M0_8p1_VAL 0x0002U

/***********************************************************
 * Common values for all Vref clocks
 **********************************************************/
#define CMN_PLL_VCOCAL_PLLCNT_START_SSC_VAL 0x0355U
#define CMN_PLL_VCOCAL_PLLCNT_START_NO_SSC_VAL 0x0357U

#define CMN_PLL_LOCK_PLLCNT_THR_SSC_VAL 0x0005U
#define CMN_PLL_LOCK_PLLCNT_THR_NO_SSC_VAL 0x0003U

#define CMN_PLL_LOCK_PLLCNT_START_SSC_VAL 0x00D6U
#define CMN_PLL_LOCK_PLLCNT_START_NO_SSC_VAL 0x00D7U

#define CMN_PLL_LOCK_REFCNT_START_VAL 0x00D7U

#else /* default - 25MHz */

/* Transmitter receiver detect register values */
#define TX_RCVDET_ST_TMR_VAL 0x09C4U

/**********************************************************
 * Values used to programming reference clock
 *********************************************************/

#define CMN_SSM_BIAS_TMR_VAL 0x0019U
#define CMN_PLLSM0_PLLPRE_TMR_VAL 0x0032U
#define CMN_PLLSM0_PLLLOCK_TMR_VAL 0x00D1U
#define CMN_PLLSM1_PLLPRE_TMR_VAL 0x0032U
#define CMN_PLLSM1_PLLLOCK_TMR_VAL 0x00D1U
#define CMN_BGCAL_INIT_TMR_VAL 0x007DU
#define CMN_BGCAL_ITER_TMR_VAL 0x007DU
#define CMN_IBCAL_INIT_TMR_VAL 0x0019U
#define CMN_TXPUCAL_INIT_TMR_VAL 0x001EU
#define CMN_TXPUCAL_ITER_TMR_VAL 0x0006U
#define CMN_TXPDCAL_INIT_TMR_VAL 0x001EU
#define CMN_TXPDCAL_ITER_TMR_VAL 0x0006U
#define CMN_RXCAL_INIT_TMR_VAL 0x02EEU
#define CMN_RXCAL_ITER_TMR_VAL 0x0006U
#define CMN_SD_CAL_INIT_TMR_VAL 0x0002U
#define CMN_SD_CAL_ITER_TMR_VAL 0x0002U
#define CMN_SD_CAL_REFTIM_START_VAL 0x000EU
#define CMN_SD_CAL_PLLCNT_START_VAL 0x012BU

#define CMN_PDIAG_PLL_CP_PADJ_M0_VAL 0x0509U
#define CMN_PDIAG_PLL_CP_IADJ_M0_VAL 0x0F00U
#define CMN_PDIAG_PLL_FILT_PADJ_M0_VAL 0x0F08U
#define CMN_PLL_DSM_DIAG_M0_VAL 0x0004U
#define CMN_PLL_VCOCAL_INIT_TMR_VAL 0x00FAU
#define CMN_PLL_VCOCAL_ITER_TMR_VAL 0x0004U
#define CMN_PLL_VCOCAL_REFTIM_START_VAL 0x0317U
#define CMN_PLL_VCOCAL_TCTRL_VAL 0x0003U

/**********************************************************
 * Values for SSC specific registers
 *********************************************************/

/* Voltage clock oscilator 10.8 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_10p8_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_10p8_VAL 0x0423U
#define CMN_PLL_SS_CTRL3_M0_VCO_10p8_VAL 0x007FU
#define CMN_PLL_SS_CTRL4_M0_VCO_10p8_VAL 0x0003U

/* Voltage clock oscilator 9.72 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_9p72_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_9p72_VAL 0x03B9U
#define CMN_PLL_SS_CTRL3_M0_VCO_9p72_VAL 0x007FU
#define CMN_PLL_SS_CTRL4_M0_VCO_9p72_VAL 0x0003U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_8p64_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_8p64_VAL 0x034FU
#define CMN_PLL_SS_CTRL3_M0_VCO_8p64_VAL 0x007FU
#define CMN_PLL_SS_CTRL4_M0_VCO_8p64_VAL 0x0003U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_SS_CTRL1_M0_VCO_8p1_VAL 0x0001U
#define CMN_PLL_SS_CTRL2_M0_VCO_8p1_VAL 0x031AU
#define CMN_PLL_SS_CTRL3_M0_VCO_8p1_VAL 0x007FU
#define CMN_PLL_SS_CTRL4_M0_VCO_8p1_VAL 0x0003U

/********************************************************
 * Values for VCO specific registers
 *******************************************************/

/* Voltage clock oscilator 10.8 GHz */
#define CMN_PLL_INTDIV_M0_10p8_VAL 0x01B0U
#define CMN_PLL_FRACDIVL_M0_10p8_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_10p8_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_10p8_VAL 0x0120U
#define CMN_PDIAG_PLL_CTRL_M0_10p8_VAL 0x0002U

/* Voltage clock oscilator 9.32 GHz */
#define CMN_PLL_INTDIV_M0_9p72_VAL 0x0184U
#define CMN_PLL_FRACDIVL_M0_9p72_VAL 0xCCCDU
#define CMN_PLL_FRACDIVH_M0_9p72_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_9p72_VAL 0x0104U
#define CMN_PDIAG_PLL_CTRL_M0_9p72_VAL 0x0002U

/* Voltage clock oscilator 8.64 GHz */
#define CMN_PLL_INTDIV_M0_8p64_VAL 0x0159U
#define CMN_PLL_FRACDIVL_M0_8p64_VAL 0x999AU
#define CMN_PLL_FRACDIVH_M0_8p64_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_8p64_VAL 0x00E8U
#define CMN_PDIAG_PLL_CTRL_M0_8p64_VAL 0x0002U

/* Voltage clock oscilator 8.1 GHz */
#define CMN_PLL_INTDIV_M0_8p1_VAL 0x0144U
#define CMN_PLL_FRACDIVL_M0_8p1_VAL 0x0000U
#define CMN_PLL_FRACDIVH_M0_8p1_VAL 0x0002U
#define CMN_PLL_HIGH_THR_M0_8p1_VAL 0x00D8U
#define CMN_PDIAG_PLL_CTRL_M0_8p1_VAL 0x0002U

/***********************************************************
 * Common values for all Vref clocks
 **********************************************************/
#define CMN_PLL_VCOCAL_PLLCNT_START_SSC_VAL 0x0315U
#define CMN_PLL_VCOCAL_PLLCNT_START_NO_SSC_VAL 0x0317U

#define CMN_PLL_LOCK_PLLCNT_THR_SSC_VAL 0x0005U
#define CMN_PLL_LOCK_PLLCNT_THR_NO_SSC_VAL 0x0003U

#define CMN_PLL_LOCK_PLLCNT_START_SSC_VAL 0x00C7U
#define CMN_PLL_LOCK_PLLCNT_START_NO_SSC_VAL 0x00C7U

#define CMN_PLL_LOCK_REFCNT_START_VAL 0x00C7U

#endif

#endif /* DP_SD0801_CFG_H */
/* parasoft-end-suppress MISRA2012-RULE-1_1_b_c90-2 */
