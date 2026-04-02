/**
* File: csl_wiz16m_ct3_refclk156p25MHz_USXGMII_100MHz_SGMII_multilink.c
*
*  using_plllc
*  multilink
*
* ============================================================================
* (C) Copyright 2022, Texas Instruments, Inc.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the
* distribution.
*
* Neither the name of Texas Instruments Incorporated nor the names of
* its contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
#ifndef CSL_WIZ16M_CT3_REFCLK156p25MHz_USXGMII_100MHz_SGMII_C
#define CSL_WIZ16M_CT3_REFCLK156p25MHz_USXGMII_100MHz_SGMII_C

#include <stdint.h>
#include <ti/csl/soc.h>
#include <ti/csl/csl.h>
#include <ti/csl/src/ip/serdes_cd/V3/cslr_wiz16b8m4ct3_nda.h>

void csl_wiz16m_ct3_refclk156p25MHz_USXGMII_100MHz_SGMII_multilink(uint32_t baseAddr, uint32_t laneNum)
{
    CSL_wiz16b8m4ct3Regs *torrent_sds_reg = (CSL_wiz16b8m4ct3Regs *)(uintptr_t)(baseAddr);

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_SSM_BIAS_TMR),15,0,(uint32_t)0x0014);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLLSM0_PLLVREF_TMR__CMN_PLLSM0_PLLPRE_TMR),15,0,(uint32_t)0x0028);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLLSM0_PLLCLKDIS_TMR__CMN_PLLSM0_PLLLOCK_TMR),15,0,(uint32_t)0x00A4);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLLSM1_PLLVREF_TMR__CMN_PLLSM1_PLLPRE_TMR),15,0,(uint32_t)0x0028);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLLSM1_PLLCLKDIS_TMR__CMN_PLLSM1_PLLLOCK_TMR),15,0,(uint32_t)0x00A4);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_BGCAL_ITER_TMR__CMN_BGCAL_INIT_TMR),15,0,(uint32_t)0x0062);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_BGCAL_ITER_TMR__CMN_BGCAL_INIT_TMR),31,16,(uint32_t)0x0062);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_IBCAL_ITER_TMR__CMN_IBCAL_INIT_TMR),15,0,(uint32_t)0x0014);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_VCOCAL_OVRD__CMN_PLL0_VCOCAL_TCTRL),15,0,(uint32_t)0x0003);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_VCOCAL_ITER_TMR__CMN_PLL0_VCOCAL_INIT_TMR),15,0,(uint32_t)0x061B);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_VCOCAL_ITER_TMR__CMN_PLL0_VCOCAL_INIT_TMR),31,16,(uint32_t)0x0019);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_VCOCAL_REFTIM_START),15,0,(uint32_t)0x1354);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_VCOCAL_PLLCNT_START),15,0,(uint32_t)0x1354);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_DSM_FBH_OVRD_M0__CMN_PLL0_DSM_DIAG_M0),31,16,(uint32_t)0x0012);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_DSM_FBL_OVRD_M0),15,0,(uint32_t)0x0006);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_LOCK_REFCNT_IDLE__CMN_PLL0_LOCK_REFCNT_START),15,0,(uint32_t)0x0138);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL0_LOCK_PLLCNT_THR__CMN_PLL0_LOCK_PLLCNT_START),15,0,(uint32_t)0x0138);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_VCOCAL_OVRD__CMN_PLL1_VCOCAL_TCTRL),15,0,(uint32_t)0x0003);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_FRACDIVL_M0__CMN_PLL1_INTDIV_M0),15,0,(uint32_t)0x0067);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_FRACDIVL_M0__CMN_PLL1_INTDIV_M0),31,16,(uint32_t)0x2000);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_HIGH_THR_M0__CMN_PLL1_FRACDIVH_M0),15,0,(uint32_t)0x0002);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_HIGH_THR_M0__CMN_PLL1_FRACDIVH_M0),31,16,(uint32_t)0x0046);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PLL1_DSM_FBH_OVRD_M0__CMN_PLL1_DSM_DIAG_M0),15,0,(uint32_t)0x0004);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_TXPUCAL_TUNE__CMN_TXPUCAL_START),31,16,(uint32_t)0x007F);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_TXPUCAL_ITER_TMR__CMN_TXPUCAL_INIT_TMR),15,0,(uint32_t)0x0018);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_TXPUCAL_ITER_TMR__CMN_TXPUCAL_INIT_TMR),31,16,(uint32_t)0x0005);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_TXPDCAL_TUNE__CMN_TXPDCAL_START),31,16,(uint32_t)0x007F);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_TXPDCAL_ITER_TMR__CMN_TXPDCAL_INIT_TMR),15,0,(uint32_t)0x0018);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_TXPDCAL_ITER_TMR__CMN_TXPDCAL_INIT_TMR),31,16,(uint32_t)0x0005);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_RXCAL_ITER_TMR__CMN_RXCAL_INIT_TMR),15,0,(uint32_t)0x024A);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_RXCAL_ITER_TMR__CMN_RXCAL_INIT_TMR),31,16,(uint32_t)0x0005);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_SD_CAL_REFTIM_START),15,0,(uint32_t)0x000B);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_SD_CAL_PLLCNT_START),15,0,(uint32_t)0x0132);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PDIAG_PLL1_CLK_SEL_M0__CMN_PDIAG_PLL1_CTRL_M0),15,0,(uint32_t)0x0002);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PDIAG_PLL1_CP_IADJ_M0__CMN_PDIAG_PLL1_CP_PADJ_M0),15,0,(uint32_t)0x0509);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PDIAG_PLL1_CP_IADJ_M0__CMN_PDIAG_PLL1_CP_PADJ_M0),31,16,(uint32_t)0x0F00);
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_CMN_REGISTERS.CMN_PDIAG_PLL1_CP_TUNE_M0__CMN_PDIAG_PLL1_FILT_PADJ_M0),15,0,(uint32_t)0x0F08);

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[0].TX_TXCC_CPOST_MULT_01__TX_TXCC_CPOST_MULT_00),15,0,(uint32_t)0x0000); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[1].TX_TXCC_CPOST_MULT_01__TX_TXCC_CPOST_MULT_00),15,0,(uint32_t)0x0000); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[2].TX_TXCC_CPOST_MULT_01__TX_TXCC_CPOST_MULT_00),15,0,(uint32_t)0x0000); //USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[3].TX_TXCC_CPOST_MULT_01__TX_TXCC_CPOST_MULT_00),15,0,(uint32_t)0x0000); //SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[0].DRV_DIAG_TX_DRV),15,0,(uint32_t)0x00B3); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[1].DRV_DIAG_TX_DRV),15,0,(uint32_t)0x00B3); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[3].DRV_DIAG_TX_DRV),15,0,(uint32_t)0x00B3); //SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[0].XCVR_DIAG_PLLDRC_CTRL__XCVR_DIAG_XDP_PWRI_STAT),31,16,(uint32_t)0x0113); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[1].XCVR_DIAG_PLLDRC_CTRL__XCVR_DIAG_XDP_PWRI_STAT),31,16,(uint32_t)0x0113); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[2].XCVR_DIAG_PLLDRC_CTRL__XCVR_DIAG_XDP_PWRI_STAT),31,16,(uint32_t)0x0089); //USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[3].XCVR_DIAG_PLLDRC_CTRL__XCVR_DIAG_XDP_PWRI_STAT),31,16,(uint32_t)0x0113); //SGMII

    //CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[0].XCVR_DIAG_HSCLK_DIV__XCVR_DIAG_HSCLK_SEL),15,0,(uint32_t)0x0111); //SGMII
    //CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[1].XCVR_DIAG_HSCLK_DIV__XCVR_DIAG_HSCLK_SEL),15,0,(uint32_t)0x0111); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[2].XCVR_DIAG_HSCLK_DIV__XCVR_DIAG_HSCLK_SEL),15,0,(uint32_t)0x0011); //USXGMII
    //CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[3].XCVR_DIAG_HSCLK_DIV__XCVR_DIAG_HSCLK_SEL),15,0,(uint32_t)0x0111); //SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[0].XCVR_DIAG_HSCLK_DIV__XCVR_DIAG_HSCLK_SEL),31,16,(uint32_t)0x0003); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[1].XCVR_DIAG_HSCLK_DIV__XCVR_DIAG_HSCLK_SEL),31,16,(uint32_t)0x0003); //SGMII
    //CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[2].XCVR_DIAG_HSCLK_DIV__XCVR_DIAG_HSCLK_SEL),31,16,(uint32_t)0x0001); //USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[3].XCVR_DIAG_HSCLK_DIV__XCVR_DIAG_HSCLK_SEL),31,16,(uint32_t)0x0003); //SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[0].XCVR_DIAG_PSC_OVRD__XCVR_DIAG_BIDI_CTRL),31,16,(uint32_t)0x0002); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[1].XCVR_DIAG_PSC_OVRD__XCVR_DIAG_BIDI_CTRL),31,16,(uint32_t)0x0002); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[2].XCVR_DIAG_PSC_OVRD__XCVR_DIAG_BIDI_CTRL),31,16,(uint32_t)0x0000); //USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[3].XCVR_DIAG_PSC_OVRD__XCVR_DIAG_BIDI_CTRL),31,16,(uint32_t)0x0002); //SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[0].TX_PSC_A1__TX_PSC_A0),15,0,(uint32_t)0x00F3); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[1].TX_PSC_A1__TX_PSC_A0),15,0,(uint32_t)0x00F3); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[2].TX_PSC_A1__TX_PSC_A0),15,0,(uint32_t)0x00F3); //USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[3].TX_PSC_A1__TX_PSC_A0),15,0,(uint32_t)0x00F3); //SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[0].TX_PSC_A3__TX_PSC_A2),15,0,(uint32_t)0x04A2); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[1].TX_PSC_A3__TX_PSC_A2),15,0,(uint32_t)0x04A2); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[2].TX_PSC_A3__TX_PSC_A2),15,0,(uint32_t)0x04A2); //USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[3].TX_PSC_A3__TX_PSC_A2),15,0,(uint32_t)0x04A2); //SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[0].TX_PSC_A3__TX_PSC_A2),31,16,(uint32_t)0x04A2); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[1].TX_PSC_A3__TX_PSC_A2),31,16,(uint32_t)0x04A2); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[2].TX_PSC_A3__TX_PSC_A2),31,16,(uint32_t)0x04A2); //USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[3].TX_PSC_A3__TX_PSC_A2),31,16,(uint32_t)0x04A2); //SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[0].TX_RCVDET_ST_TMR__TX_RCVDET_EN_TMR),31,16,(uint32_t)0x07A2);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[1].TX_RCVDET_ST_TMR__TX_RCVDET_EN_TMR),31,16,(uint32_t)0x07A2);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[2].TX_RCVDET_ST_TMR__TX_RCVDET_EN_TMR),31,16,(uint32_t)0x07A2);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_TX_LANE_REGISTERS[3].TX_RCVDET_ST_TMR__TX_RCVDET_EN_TMR),31,16,(uint32_t)0x07A2);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_PSC_A1__RX_PSC_A0),15,0,(uint32_t)0x091D); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_PSC_A1__RX_PSC_A0),15,0,(uint32_t)0x091D); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_PSC_A1__RX_PSC_A0),15,0,(uint32_t)0x091D); //USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_PSC_A1__RX_PSC_A0),15,0,(uint32_t)0x091D); //SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_PSC_A3__RX_PSC_A2),15,0,(uint32_t)0x0900); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_PSC_A3__RX_PSC_A2),15,0,(uint32_t)0x0900); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_PSC_A3__RX_PSC_A2),15,0,(uint32_t)0x0900); //USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_PSC_A3__RX_PSC_A2),15,0,(uint32_t)0x0900); //SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_PSC_A3__RX_PSC_A2),31,16,(uint32_t)0x0100); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_PSC_A3__RX_PSC_A2),31,16,(uint32_t)0x0100); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_PSC_A3__RX_PSC_A2),31,16,(uint32_t)0x0100); //USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_PSC_A3__RX_PSC_A2),31,16,(uint32_t)0x0100); //SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_SDCAL0_ITER_TMR__RX_SDCAL0_INIT_TMR),15,0,(uint32_t)0x0014); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_SDCAL0_ITER_TMR__RX_SDCAL0_INIT_TMR),31,16,(uint32_t)0x0062); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_SDCAL1_ITER_TMR__RX_SDCAL1_INIT_TMR),15,0,(uint32_t)0x0014);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_SDCAL1_ITER_TMR__RX_SDCAL1_INIT_TMR),31,16,(uint32_t)0x0062);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_SDCAL0_ITER_TMR__RX_SDCAL0_INIT_TMR),15,0,(uint32_t)0x0014);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_SDCAL0_ITER_TMR__RX_SDCAL0_INIT_TMR),31,16,(uint32_t)0x0062);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_SDCAL1_ITER_TMR__RX_SDCAL1_INIT_TMR),15,0,(uint32_t)0x0014);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_SDCAL1_ITER_TMR__RX_SDCAL1_INIT_TMR),31,16,(uint32_t)0x0062);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_SDCAL0_ITER_TMR__RX_SDCAL0_INIT_TMR),15,0,(uint32_t)0x0014);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_SDCAL0_ITER_TMR__RX_SDCAL0_INIT_TMR),31,16,(uint32_t)0x0062);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_SDCAL1_ITER_TMR__RX_SDCAL1_INIT_TMR),15,0,(uint32_t)0x0014);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_SDCAL1_ITER_TMR__RX_SDCAL1_INIT_TMR),31,16,(uint32_t)0x0062);//USXGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_SDCAL0_ITER_TMR__RX_SDCAL0_INIT_TMR),15,0,(uint32_t)0x0014);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_SDCAL0_ITER_TMR__RX_SDCAL0_INIT_TMR),31,16,(uint32_t)0x0062);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_SDCAL1_ITER_TMR__RX_SDCAL1_INIT_TMR),15,0,(uint32_t)0x0014);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_SDCAL1_ITER_TMR__RX_SDCAL1_INIT_TMR),31,16,(uint32_t)0x0062);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_CDRLF_CNFG2__RX_CDRLF_CNFG),31,16,(uint32_t)0x018C); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_CDRLF_CNFG2__RX_CDRLF_CNFG),31,16,(uint32_t)0x018C); //SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_CDRLF_CNFG2__RX_CDRLF_CNFG),31,16,(uint32_t)0x018C); //USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_CDRLF_CNFG2__RX_CDRLF_CNFG),31,16,(uint32_t)0x018C); //SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_REE_GCSM1_EQENM_PH1__RX_REE_GCSM1_CTRL),31,16,(uint32_t)0x03C7);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_REE_GCSM1_EQENM_PH1__RX_REE_GCSM1_CTRL),31,16,(uint32_t)0x03C7);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_REE_GCSM1_EQENM_PH1__RX_REE_GCSM1_CTRL),31,16,(uint32_t)0x03C7);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_REE_GCSM1_EQENM_PH1__RX_REE_GCSM1_CTRL),31,16,(uint32_t)0x03C7);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_REE_GCSM1_START_TMR__RX_REE_GCSM1_EQENM_PH2),15,0,(uint32_t)0x01C7);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_REE_GCSM1_START_TMR__RX_REE_GCSM1_EQENM_PH2),15,0,(uint32_t)0x01C7);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_REE_GCSM1_START_TMR__RX_REE_GCSM1_EQENM_PH2),15,0,(uint32_t)0x01C7);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_REE_GCSM1_START_TMR__RX_REE_GCSM1_EQENM_PH2),15,0,(uint32_t)0x01C7);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_REE_TAP1_CLIP__RX_REE_ADDR_CFG),31,16,(uint32_t)0x0019);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_REE_TAP1_CLIP__RX_REE_ADDR_CFG),31,16,(uint32_t)0x0019);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_REE_TAP1_CLIP__RX_REE_ADDR_CFG),31,16,(uint32_t)0x0019);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_REE_TAP1_CLIP__RX_REE_ADDR_CFG),31,16,(uint32_t)0x0019);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_REE_CTRL_DATA_MASK__RX_REE_TAP2TON_CLIP),15,0,(uint32_t)0x0019);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_REE_CTRL_DATA_MASK__RX_REE_TAP2TON_CLIP),15,0,(uint32_t)0x0019);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_REE_CTRL_DATA_MASK__RX_REE_TAP2TON_CLIP),15,0,(uint32_t)0x0019);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_REE_CTRL_DATA_MASK__RX_REE_TAP2TON_CLIP),15,0,(uint32_t)0x0019);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_REE_SMGM_CTRL1__RX_REE_TXEQEVAL_CTRL),31,16,(uint32_t)0x0030);//USXGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_DIAG_DFE_AMP_TUNE__RX_DIAG_DFE_CTRL),15,0,(uint32_t)0x0000);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_DIAG_DFE_AMP_TUNE__RX_DIAG_DFE_CTRL),15,0,(uint32_t)0x0000);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_DIAG_DFE_AMP_TUNE__RX_DIAG_DFE_CTRL),15,0,(uint32_t)0x0000);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_DIAG_DFE_AMP_TUNE__RX_DIAG_DFE_CTRL),15,0,(uint32_t)0x0000);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_DIAG_DFE_AMP_TUNE_3__RX_DIAG_DFE_AMP_TUNE_2),15,0,(uint32_t)0x0C01);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_DIAG_DFE_AMP_TUNE_3__RX_DIAG_DFE_AMP_TUNE_2),15,0,(uint32_t)0x0C01);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_DIAG_DFE_AMP_TUNE_3__RX_DIAG_DFE_AMP_TUNE_2),15,0,(uint32_t)0x0C21);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_DIAG_DFE_AMP_TUNE_3__RX_DIAG_DFE_AMP_TUNE_2),15,0,(uint32_t)0x0C01);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_DIAG_DFE_AMP_TUNE_3__RX_DIAG_DFE_AMP_TUNE_2),31,16,(uint32_t)0x0000);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_DIAG_DFE_AMP_TUNE_3__RX_DIAG_DFE_AMP_TUNE_2),31,16,(uint32_t)0x0000);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_DIAG_DFE_AMP_TUNE_3__RX_DIAG_DFE_AMP_TUNE_2),31,16,(uint32_t)0x0002);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_DIAG_DFE_AMP_TUNE_3__RX_DIAG_DFE_AMP_TUNE_2),31,16,(uint32_t)0x0000);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_DIAG_NQST_CTRL__RX_DIAG_REE_DAC_CTRL),31,16,(uint32_t)0x0098);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_DIAG_NQST_CTRL__RX_DIAG_REE_DAC_CTRL),31,16,(uint32_t)0x0098);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_DIAG_NQST_CTRL__RX_DIAG_REE_DAC_CTRL),31,16,(uint32_t)0x00B9);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_DIAG_NQST_CTRL__RX_DIAG_REE_DAC_CTRL),31,16,(uint32_t)0x0098);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_DIAG_PI_CAP__RX_DIAG_PI_RATE),15,0,(uint32_t)0x0010);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_DIAG_PI_CAP__RX_DIAG_PI_RATE),15,0,(uint32_t)0x0010);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_DIAG_PI_CAP__RX_DIAG_PI_RATE),15,0,(uint32_t)0x0033);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_DIAG_PI_CAP__RX_DIAG_PI_RATE),15,0,(uint32_t)0x0010);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_DIAG_PI_CAP__RX_DIAG_PI_RATE),31,16,(uint32_t)0x0000);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_DIAG_PI_CAP__RX_DIAG_PI_RATE),31,16,(uint32_t)0x0000);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_DIAG_PI_CAP__RX_DIAG_PI_RATE),31,16,(uint32_t)0x0000);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[0].RX_DIAG_ACYA__RX_DIAG_DCYA),31,16,(uint32_t)0x0001);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[1].RX_DIAG_ACYA__RX_DIAG_DCYA),31,16,(uint32_t)0x0001);//SGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[2].RX_DIAG_ACYA__RX_DIAG_DCYA),31,16,(uint32_t)0x0001);//USXGMII
    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PMA_RX_LANE_REGISTERS[3].RX_DIAG_ACYA__RX_DIAG_DCYA),31,16,(uint32_t)0x0001);//SGMII

    CSL_FINSR(*(volatile uint32_t *)(&torrent_sds_reg->PHY_PCS_CMN_REGISTERS.PHY_AUTO_CFG_SPDUP__PHY_PLL_CFG),15,0,(uint32_t)0x0002);
}

#endif

