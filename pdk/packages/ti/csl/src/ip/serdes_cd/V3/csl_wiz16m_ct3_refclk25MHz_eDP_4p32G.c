/**
* File: csl_wiz16m_ct3_refclk25MHz_eDP_4p32G.c
*
*  using_plllc
*  no multilink
*
*  ============================================================================
*  (C) Copyright 2019, Texas Instruments, Inc.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
#ifndef CSL_WIZ16M_ct3_REFCLK25MHz_eDP_4p32G_C
#define CSL_WIZ16M_ct3_REFCLK25MHz_eDP_4p32G_C

#include <stdint.h>
#include <ti/csl/soc.h>
#include <ti/csl/csl.h>
#include <ti/csl/src/ip/serdes_cd/V3/cslr_wiz16b8m4ct3_nda.h>

void csl_wiz16m_ct3_refclk25MHz_eDP_4p32G(uint32_t baseAddr, uint32_t laneNum)
{
    CSL_wiz16b8m4ct3Regs *torrent_sds_reg = (CSL_wiz16b8m4ct3Regs *)(uintptr_t)(baseAddr);

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PHY_PCS_CMN_REGISTERS.PHY_AUTO_CFG_SPDUP__PHY_PLL_CFG),15,0,(uint32_t)0x0000);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PDIAG_PLL0_CLK_SEL_M0__CMN_PDIAG_PLL0_CTRL_M0),31,16,(uint32_t)0x0301);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[laneNum].XCVR_DIAG_HSCLK_DIV__XCVR_DIAG_HSCLK_SEL),15,0,(uint32_t)0x0000);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[laneNum].XCVR_DIAG_HSCLK_DIV__XCVR_DIAG_HSCLK_SEL),31,16,(uint32_t)0x0000);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[laneNum].XCVR_DIAG_PLLDRC_CTRL__XCVR_DIAG_XDP_PWRI_STAT),31,16,(uint32_t)0x0001);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_DSM_FBH_OVRD_M0__CMN_PLL0_DSM_DIAG_M0),15,0,(uint32_t)0x0004);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_DSM_FBH_OVRD_M0__CMN_PLL1_DSM_DIAG_M0),15,0,(uint32_t)0x0004);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PDIAG_PLL0_CP_IADJ_M0__CMN_PDIAG_PLL0_CP_PADJ_M0),15,0,(uint32_t)0x0509);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PDIAG_PLL1_CP_IADJ_M0__CMN_PDIAG_PLL1_CP_PADJ_M0),15,0,(uint32_t)0x0509);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PDIAG_PLL0_CP_IADJ_M0__CMN_PDIAG_PLL0_CP_PADJ_M0),31,16,(uint32_t)0x0F00);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PDIAG_PLL1_CP_IADJ_M0__CMN_PDIAG_PLL1_CP_PADJ_M0),31,16,(uint32_t)0x0F00);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PDIAG_PLL0_CP_TUNE_M0__CMN_PDIAG_PLL0_FILT_PADJ_M0),15,0,(uint32_t)0x0F08);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PDIAG_PLL1_CP_TUNE_M0__CMN_PDIAG_PLL1_FILT_PADJ_M0),15,0,(uint32_t)0x0F08);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_FRACDIVL_M0__CMN_PLL0_INTDIV_M0),15,0,(uint32_t)0x0159);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_FRACDIVL_M0__CMN_PLL1_INTDIV_M0),15,0,(uint32_t)0x0159);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_FRACDIVL_M0__CMN_PLL0_INTDIV_M0),31,16,(uint32_t)0x999A);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_FRACDIVL_M0__CMN_PLL1_INTDIV_M0),31,16,(uint32_t)0x999A);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_HIGH_THR_M0__CMN_PLL0_FRACDIVH_M0),15,0,(uint32_t)0x0002);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_HIGH_THR_M0__CMN_PLL1_FRACDIVH_M0),15,0,(uint32_t)0x0002);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_HIGH_THR_M0__CMN_PLL0_FRACDIVH_M0),31,16,(uint32_t)0x00E8);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_HIGH_THR_M0__CMN_PLL1_FRACDIVH_M0),31,16,(uint32_t)0x00E8);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PDIAG_PLL0_CLK_SEL_M0__CMN_PDIAG_PLL0_CTRL_M0),15,0,(uint32_t)0x0002);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PDIAG_PLL1_CLK_SEL_M0__CMN_PDIAG_PLL1_CTRL_M0),15,0,(uint32_t)0x0002);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_VCOCAL_ITER_TMR__CMN_PLL0_VCOCAL_INIT_TMR),15,0,(uint32_t)0x00FA);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_VCOCAL_ITER_TMR__CMN_PLL1_VCOCAL_INIT_TMR),15,0,(uint32_t)0x00FA);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_VCOCAL_ITER_TMR__CMN_PLL0_VCOCAL_INIT_TMR),31,16,(uint32_t)0x0004);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_VCOCAL_ITER_TMR__CMN_PLL1_VCOCAL_INIT_TMR),31,16,(uint32_t)0x0004);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_VCOCAL_REFTIM_START),15,0,(uint32_t)0x0317);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_VCOCAL_REFTIM_START),15,0,(uint32_t)0x0317);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_VCOCAL_PLLCNT_START),15,0,(uint32_t)0x0317);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_VCOCAL_PLLCNT_START),15,0,(uint32_t)0x0317);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_VCOCAL_OVRD__CMN_PLL0_VCOCAL_TCTRL),15,0,(uint32_t)0x0003);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_VCOCAL_OVRD__CMN_PLL1_VCOCAL_TCTRL),15,0,(uint32_t)0x0003);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_LOCK_REFCNT_IDLE__CMN_PLL0_LOCK_REFCNT_START),15,0,(uint32_t)0x00C7);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_LOCK_REFCNT_IDLE__CMN_PLL1_LOCK_REFCNT_START),15,0,(uint32_t)0x00C7);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_LOCK_PLLCNT_THR__CMN_PLL0_LOCK_PLLCNT_START),15,0,(uint32_t)0x00C7);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_LOCK_PLLCNT_THR__CMN_PLL1_LOCK_PLLCNT_START),15,0,(uint32_t)0x00C7);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[laneNum].TX_PSC_A1__TX_PSC_A0),15,0,(uint32_t)0x00FB);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[laneNum].TX_PSC_A3__TX_PSC_A2),15,0,(uint32_t)0x04AA);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[laneNum].TX_PSC_A3__TX_PSC_A2),31,16,(uint32_t)0x04AA);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[laneNum].RX_PSC_A1__RX_PSC_A0),15,0,(uint32_t)0x0000);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[laneNum].RX_PSC_A3__RX_PSC_A2),15,0,(uint32_t)0x0000);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[laneNum].RX_PSC_A3__RX_PSC_A2),31,16,(uint32_t)0x0000);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[laneNum].RX_PSC_RDY__RX_PSC_CAL),15,0,(uint32_t)0x0000);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[laneNum].XCVR_DIAG_PSC_OVRD__XCVR_DIAG_BIDI_CTRL),15,0,(uint32_t)0x000F);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[laneNum].RX_REE_GCSM1_EQENM_PH1__RX_REE_GCSM1_CTRL),15,0,(uint32_t)0x0000);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[laneNum].RX_REE_GCSM2_EQENM_PH1__RX_REE_GCSM2_CTRL),15,0,(uint32_t)0x0000);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[laneNum].RX_REE_PERGCSM_EQENM_PH1__RX_REE_PERGCSM_CTRL),15,0,(uint32_t)0x0000);
}

#endif

