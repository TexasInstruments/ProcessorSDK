/*
 *  Copyright (C) 2025 Texas Instruments Incorporated
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

/**
 *  \file   dm_ut_devices_list.c
 *
 *  \brief  This file contains lists of different types of devices present on J721S2.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/drv/sciclient/soc/sysfw/include/j721s2/tisci_devices.h>
#include <dm_ut_devices.h>

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
/*                           Global Variables                                */
/* ========================================================================== */

/* These devices are controlled by main_always_on LPSC and cannot be powered off by software */
const uint32_t gDMAppMainAlwaysOnDevices[] = {
	TISCI_DEV_COMPUTE_CLUSTER0_CORE_CORE,
	TISCI_DEV_CPT2_AGGR1,
	TISCI_DEV_CPT2_AGGR5,
	TISCI_DEV_CPT2_AGGR2,
	TISCI_DEV_CPT2_AGGR4,
	TISCI_DEV_CPT2_AGGR3,
	TISCI_DEV_CPT2_AGGR0,
	TISCI_DEV_DCC0,
	TISCI_DEV_DCC1,
	TISCI_DEV_DCC2,
	TISCI_DEV_DCC3,
	TISCI_DEV_DCC4,
	TISCI_DEV_DCC5,
	TISCI_DEV_DCC6,
	TISCI_DEV_DCC7,
	TISCI_DEV_DCC8,
	TISCI_DEV_DCC9,
	TISCI_DEV_TIMER4,
	TISCI_DEV_TIMER5,
	TISCI_DEV_TIMER6,
	TISCI_DEV_TIMER7,
	TISCI_DEV_ESM0,
	TISCI_DEV_GPIO0,
	TISCI_DEV_NAVSS0_BCDMA_0,
	TISCI_DEV_NAVSS0_CPTS_0,
	TISCI_DEV_NAVSS0_INTR_0,
	TISCI_DEV_NAVSS0_MAILBOX1_0,
	TISCI_DEV_NAVSS0_MAILBOX1_1,
	TISCI_DEV_NAVSS0_MAILBOX1_10,
	TISCI_DEV_NAVSS0_MAILBOX1_11,
	TISCI_DEV_NAVSS0_MAILBOX1_2,
	TISCI_DEV_NAVSS0_MAILBOX1_3,
	TISCI_DEV_NAVSS0_MAILBOX1_4,
	TISCI_DEV_NAVSS0_MAILBOX1_5,
	TISCI_DEV_NAVSS0_MAILBOX1_6,
	TISCI_DEV_NAVSS0_MAILBOX1_7,
	TISCI_DEV_NAVSS0_MAILBOX1_8,
	TISCI_DEV_NAVSS0_MAILBOX1_9,
	TISCI_DEV_NAVSS0_MAILBOX_0,
	TISCI_DEV_NAVSS0_MAILBOX_1,
	TISCI_DEV_NAVSS0_MAILBOX_10,
	TISCI_DEV_NAVSS0_MAILBOX_11,
	TISCI_DEV_NAVSS0_MAILBOX_2,
	TISCI_DEV_NAVSS0_MAILBOX_3,
	TISCI_DEV_NAVSS0_MAILBOX_4,
	TISCI_DEV_NAVSS0_MAILBOX_5,
	TISCI_DEV_NAVSS0_MAILBOX_6,
	TISCI_DEV_NAVSS0_MAILBOX_7,
	TISCI_DEV_NAVSS0_MAILBOX_8,
	TISCI_DEV_NAVSS0_MAILBOX_9,
	TISCI_DEV_NAVSS0_MCRC_0,
	TISCI_DEV_NAVSS0_MODSS,
	TISCI_DEV_NAVSS0_MODSS_INTA_0,
	TISCI_DEV_NAVSS0_MODSS_INTA_1,
	TISCI_DEV_NAVSS0_PROXY_0,
	TISCI_DEV_NAVSS0_PVU_0,
	TISCI_DEV_NAVSS0_PVU_1,
	TISCI_DEV_NAVSS0_RINGACC_0,
	TISCI_DEV_NAVSS0_SPINLOCK_0,
	TISCI_DEV_NAVSS0_TIMERMGR_0,
	TISCI_DEV_NAVSS0_TIMERMGR_1,
	TISCI_DEV_NAVSS0_UDMAP_0,
	TISCI_DEV_NAVSS0_UDMASS,
	TISCI_DEV_NAVSS0_UDMASS_INTA_0,
	TISCI_DEV_NAVSS0_VIRTSS
};

/* These devices are controlled by mcu_always_on LPSC and cannot be powered off by software */
const uint32_t gDMAppMcuAlwaysOnDevices[] = {
	TISCI_DEV_MCU_CPSW0,
	TISCI_DEV_MCU_CPT2_AGGR0,
	TISCI_DEV_MCU_DCC0,
	TISCI_DEV_MCU_DCC1,
	TISCI_DEV_MCU_DCC2,
	TISCI_DEV_MCU_TIMER0,
	TISCI_DEV_MCU_TIMER1,
	TISCI_DEV_MCU_TIMER2,
	TISCI_DEV_MCU_TIMER3,
	TISCI_DEV_MCU_TIMER4,
	TISCI_DEV_MCU_TIMER5,
	TISCI_DEV_MCU_TIMER6,
	TISCI_DEV_MCU_TIMER7,
	TISCI_DEV_MCU_TIMER8,
	TISCI_DEV_MCU_TIMER9,
	TISCI_DEV_WKUP_ESM0,
	TISCI_DEV_MCU_ESM0,
	TISCI_DEV_MCU_FSS0_FSAS_0,
	TISCI_DEV_WKUP_GPIOMUX_INTRTR0,
	TISCI_DEV_WKUP_DDPA0,
	TISCI_DEV_WKUP_VTM0,
	TISCI_DEV_MCU_I2C0,
	TISCI_DEV_MCU_I2C1,
	TISCI_DEV_MCU_NAVSS0_INTR_ROUTER_0,
	TISCI_DEV_MCU_NAVSS0_MCRC_0,
	TISCI_DEV_MCU_NAVSS0_MODSS,
	TISCI_DEV_MCU_NAVSS0_PROXY0,
	TISCI_DEV_MCU_NAVSS0_RINGACC0,
	TISCI_DEV_MCU_NAVSS0_UDMAP_0,
	TISCI_DEV_MCU_NAVSS0_UDMASS,
	TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0,
	TISCI_DEV_MCU_MCSPI0,
	TISCI_DEV_MCU_MCSPI1,
	TISCI_DEV_MCU_MCSPI2,
	TISCI_DEV_MCU_UART0
	/*
	 * TODO: These are having separate LPSC other than Always On LPSC
	 * TISCI_DEV_MCU_FSS0_HYPERBUS1P0_0,
	 * TISCI_DEV_MCU_FSS0_OSPI_0,
	 * TISCI_DEV_MCU_FSS0_OSPI_1,
	 */
};

/* This list of devices can be powered ON and powered OFF by DM independently,
 * and they do not depend on current state of other devices
 */
const uint32_t gDMAppDevicesWithoutDependency[] = {
	TISCI_DEV_MCU_ADC12FC_16FFC0,
	TISCI_DEV_MCU_ADC12FC_16FFC1,
	TISCI_DEV_ATL0,
	TISCI_DEV_COMPUTE_CLUSTER0_PBIST_WRAP_0,
	TISCI_DEV_CPSW1,
	TISCI_DEV_CSI_RX_IF0,
	TISCI_DEV_CSI_RX_IF1,
	TISCI_DEV_CSI_TX_IF_V2_0,
	TISCI_DEV_DMPAC0_SDE_0,
	TISCI_DEV_PBIST7,
#if !defined(BUILD_MCU2_0)
/* This devices is getting used by mcu2_0 core and powering off them will result in crash */
	TISCI_DEV_TIMER2,
#endif
#if !defined(BUILD_C7X_1)
/* These devices are getting used by c7x core and powering off them will result in crash */
	TISCI_DEV_TIMER0,
	TISCI_DEV_COMPUTE_CLUSTER0,
#endif
	TISCI_DEV_TIMER1,
	TISCI_DEV_TIMER3,
	TISCI_DEV_MMCSD0,
	TISCI_DEV_MMCSD1,
	TISCI_DEV_MCU_I3C0,
	TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_DFT_EMBED_PBIST_0,
	TISCI_DEV_DSS_DSI0,
	TISCI_DEV_DSS0,
	TISCI_DEV_PBIST5,
	TISCI_DEV_PBIST11,
	TISCI_DEV_PBIST8,
	TISCI_DEV_PBIST2,
	TISCI_DEV_PBIST10,
	TISCI_DEV_MCU_PBIST2,
	TISCI_DEV_MAIN2WKUPMCU_VD,
	TISCI_DEV_MCAN0,
	TISCI_DEV_MCAN1,
	TISCI_DEV_MCAN2,
	TISCI_DEV_MCAN3,
	TISCI_DEV_MCU_MCAN0,
	TISCI_DEV_MCU_MCAN1,
	TISCI_DEV_I2C0,
	TISCI_DEV_NAVSS0,
	TISCI_DEV_PCIE1,
	TISCI_DEV_SA2_UL0,
	TISCI_DEV_USB0,
	TISCI_DEV_VUSR_DUAL0,
	TISCI_DEV_SERDES_10G0,
	TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPUCORE_0
	/*
	 * TODO: This is related to C7X and A72 core
     * TISCI_DEV_C71X_0_PBIST_VD,
     * TISCI_DEV_C71X_1_PBIST_VD,
	 * TISCI_DEV_A72SS0_CORE0_PBIST_WRAP,
	 */

	/*
	 * TISCI_DEV_WKUP_SMS0, This cannot be tested because DM cannot control Security core.
	 * TISCI_DEV_WKUPMCU2MAIN_VD,
	 * TISCI_DEV_MCU_I3C1, This cannot be tested refer SYSFW-7767
	 */
};

/* These devices don't have any LPSCs associated to it.
 * Therefore, power state of these devices is not controlled by DM.
 */
const uint32_t gDMAppIndependentDevices[] = {
	TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_MMA_0,
	TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_PBIST_WRAP_0,
	TISCI_DEV_COMPUTE_CLUSTER0_C71SS1_PBIST_WRAP_0,
	TISCI_DEV_COMPUTE_CLUSTER0_CFG_WRAP_0,
	TISCI_DEV_COMPUTE_CLUSTER0_CLEC,
	TISCI_DEV_COMPUTE_CLUSTER0_DDR32SS_EMIF0_EW_0,
	TISCI_DEV_COMPUTE_CLUSTER0_DDR32SS_EMIF1_EW_0,
	TISCI_DEV_COMPUTE_CLUSTER0_DEBUG_WRAP_0,
	TISCI_DEV_COMPUTE_CLUSTER0_DIVH2_DIVH0_0,
	TISCI_DEV_COMPUTE_CLUSTER0_DIVH2_DIVH0_1,
	TISCI_DEV_COMPUTE_CLUSTER0_DIVP_TFT0_0,
	TISCI_DEV_COMPUTE_CLUSTER0_DIVP_TFT0_1,
	TISCI_DEV_COMPUTE_CLUSTER0_DMSC_WRAP_0,
	TISCI_DEV_COMPUTE_CLUSTER0_EN_MSMC_DOMAIN_0,
	TISCI_DEV_COMPUTE_CLUSTER0_GIC500SS,
	TISCI_DEV_J7AM_HWA_ATB_FUNNEL0,
	TISCI_DEV_MCU_FSS0,
	TISCI_DEV_LED0,
	TISCI_DEV_WKUP_PORZ_SYNC0,
	TISCI_DEV_WKUP_PSC0,
	TISCI_DEV_J7AEP_GPU_BXS464_WRAP0,
	TISCI_DEV_J7AM_32_64_ATB_FUNNEL0,
	TISCI_DEV_J7AM_32_64_ATB_FUNNEL1,
	TISCI_DEV_J7AM_32_64_ATB_FUNNEL2,
	TISCI_DEV_AGGR_ATB0,
	TISCI_DEV_J7AM_BOLT_PGD0,
	TISCI_DEV_CSI_PSILSS0,
	TISCI_DEV_DMPAC_VPAC_PSILSS0,
	TISCI_DEV_J7AM_MAIN_16FF0,
	TISCI_DEV_PSC0,
	TISCI_DEV_J7AM_PULSAR_ATB_FUNNEL0,
	TISCI_DEV_SA2_CPSW_PSILSS0,
	TISCI_DEV_WKUP_J7AM_WAKEUP_16FF0,
	TISCI_DEV_BOARD0,
	TISCI_DEV_MCU_NAVSS0,
	TISCI_DEV_R5FSS0,
	TISCI_DEV_R5FSS1,
	TISCI_DEV_MCU_R5FSS0,
	TISCI_DEV_WKUP_HSM0,
	TISCI_DEV_DMPAC0_UTC_0,
	TISCI_DEV_ACSPCIE_BUFFER0,
	TISCI_DEV_MCU_TIMER1_CLKSEL_VD,
	TISCI_DEV_MCU_TIMER3_CLKSEL_VD,
	TISCI_DEV_MCU_TIMER5_CLKSEL_VD,
	TISCI_DEV_MCU_TIMER7_CLKSEL_VD,
	TISCI_DEV_MCU_TIMER9_CLKSEL_VD,
	TISCI_DEV_TIMER1_CLKSEL_VD,
	TISCI_DEV_TIMER3_CLKSEL_VD,
	TISCI_DEV_TIMER5_CLKSEL_VD,
	TISCI_DEV_TIMER7_CLKSEL_VD,
	TISCI_DEV_TIMER9_CLKSEL_VD,
	TISCI_DEV_TIMER11_CLKSEL_VD,
	TISCI_DEV_TIMER13_CLKSEL_VD,
	TISCI_DEV_TIMER15_CLKSEL_VD,
	TISCI_DEV_TIMER17_CLKSEL_VD,
	TISCI_DEV_TIMER19_CLKSEL_VD,
	TISCI_DEV_MAIN_PLL8_SEL_EXTWAVE_VD
};

/* These devices rely on power state of dependent devices
 * and cannot be powered on and off directly by DM.
 * They must follow a specific sequence for power off (right to left in list provided).
 */
DMApp_DependentDevInfo gDMAppDevicesWithDependency[] = {
#if !defined(BUILD_C7X_1) && !defined(BUILD_MCU2_0)
/* These devices are getting used by mcu2_0 and c7x core and powering off them will result in crash */
	{0,
		{
			TISCI_DEV_FFI_MAIN_IP_CBASS_VD,
			TISCI_DEV_ATL0,
			TISCI_DEV_MCASP0,
			TISCI_DEV_MCASP1,
			TISCI_DEV_MCASP2,
			TISCI_DEV_MCASP3,
			TISCI_DEV_MCASP4,
			TISCI_DEV_TIMER10,
			TISCI_DEV_TIMER11,
			TISCI_DEV_TIMER12,
			TISCI_DEV_TIMER13,
			TISCI_DEV_TIMER14,
			TISCI_DEV_TIMER15,
			TISCI_DEV_TIMER16,
			TISCI_DEV_TIMER17,
			TISCI_DEV_TIMER18,
			TISCI_DEV_TIMER19,
			TISCI_DEV_TIMER8,
			TISCI_DEV_TIMER9,
			TISCI_DEV_MCSPI4,
			TISCI_DEV_MCSPI5,
			TISCI_DEV_MCSPI6,
			TISCI_DEV_MCSPI7,
			TISCI_DEV_UART2,
			TISCI_DEV_UART3,
			TISCI_DEV_I2C4,
			TISCI_DEV_I2C5,
			TISCI_DEV_I2C6,
			TISCI_DEV_TIMER0,
			TISCI_DEV_TIMER1,
			TISCI_DEV_TIMER2,
			TISCI_DEV_TIMER3,
			0
		}
	},

	{0,
		{
			TISCI_DEV_UART4,
			TISCI_DEV_UART5,
			TISCI_DEV_UART6,
			TISCI_DEV_UART7,
			TISCI_DEV_UART8,
			TISCI_DEV_UART9,
		}
	},
#endif

	{0,
		{
			TISCI_DEV_MCASP0,
			TISCI_DEV_MCASP1,
			TISCI_DEV_MCASP2,
			TISCI_DEV_MCASP3,
			TISCI_DEV_MCASP4,
			0
		}
	},

	{0,
		{
			TISCI_DEV_TIMER10,
			TISCI_DEV_TIMER11,
			TISCI_DEV_TIMER12,
			TISCI_DEV_TIMER13,
			TISCI_DEV_TIMER14,
			TISCI_DEV_TIMER15,
			TISCI_DEV_TIMER16,
			TISCI_DEV_TIMER17,
			TISCI_DEV_TIMER18,
			TISCI_DEV_TIMER19,
			TISCI_DEV_TIMER8,
			TISCI_DEV_TIMER9,
			0
		}
	},

	{0,
		{
			TISCI_DEV_MCSPI4,
			TISCI_DEV_MCSPI5,
			TISCI_DEV_MCSPI6,
			TISCI_DEV_MCSPI7,
			0
		}
	},

	{0,
		{
			TISCI_DEV_UART2,
			TISCI_DEV_UART3,
			0
		}
	},

	{0,
		{
			TISCI_DEV_I2C4,
			TISCI_DEV_I2C5,
			TISCI_DEV_I2C6,
			0
		}
	},

	{0,
		{
			TISCI_DEV_FFI_MAIN_AC_CBASS_VD,
			TISCI_DEV_DSS0,
			TISCI_DEV_DPHY_TX0,
			TISCI_DEV_DSS_DSI0,
			TISCI_DEV_CSI_TX_IF_V2_0,
			TISCI_DEV_DPHY_RX1,
			TISCI_DEV_CSI_RX_IF1,
			TISCI_DEV_DMPAC0,
			TISCI_DEV_DMPAC0_CTSET_0,
			TISCI_DEV_DMPAC0_INTD_0,
			TISCI_DEV_DMPAC0_SDE_0,
			TISCI_DEV_PBIST7,
			TISCI_DEV_VPAC0,
			TISCI_DEV_PBIST8,
			TISCI_DEV_DPHY_TX1,
			TISCI_DEV_CSI_TX_IF_V2_1,
			TISCI_DEV_DSS_DSI1,
			TISCI_DEV_DPHY_RX0,
			TISCI_DEV_CSI_RX_IF0,
			0
		}
	},

	{0,
		{
			TISCI_DEV_DPHY_TX0,
			TISCI_DEV_DSS_DSI0,
			TISCI_DEV_CSI_TX_IF_V2_0,
			0
		}
	},

	{0,
		{
			TISCI_DEV_DPHY_RX1,
			TISCI_DEV_CSI_RX_IF1,
			0
		}
	},

	{0,
		{
			TISCI_DEV_DMPAC0,
			TISCI_DEV_DMPAC0_CTSET_0,
			TISCI_DEV_DMPAC0_INTD_0,
			TISCI_DEV_DMPAC0_SDE_0,
			TISCI_DEV_PBIST7,
			0
		}
	},

	{0,
		{
			TISCI_DEV_VPAC0,
			TISCI_DEV_PBIST8,
			0
		}
	},

	{0,
		{
			TISCI_DEV_DPHY_TX1,
			TISCI_DEV_CSI_TX_IF_V2_1,
			TISCI_DEV_DSS_DSI1,
			0
		}
	},

	{0,
		{
			TISCI_DEV_CSI_TX_IF_V2_1,
			TISCI_DEV_DSS_DSI1,
			0
		}
	},

	{0,
		{
			TISCI_DEV_DPHY_RX0,
			TISCI_DEV_CSI_RX_IF0,
			0
		}
	},

	{0,
		{
			TISCI_DEV_STM0,
			TISCI_DEV_DEBUGSS_WRAP0,
			TISCI_DEV_DEBUGSUSPENDRTR0,
			0
		}
	},

	{0,
		{
			TISCI_DEV_FFI_MAIN_INFRA_CBASS_VD,
			TISCI_DEV_GPIO2,
			TISCI_DEV_GPIO4,
			TISCI_DEV_GPIO6,
			TISCI_DEV_GTC0,
			TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
			TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
			TISCI_DEV_TIMESYNC_INTRTR0,
			TISCI_DEV_GPIOMUX_INTRTR0,
			TISCI_DEV_CMPEVENT_INTRTR0,
			0
		}
	},

	{0,
		{
			TISCI_DEV_GPIO2,
			TISCI_DEV_GPIO4,
			TISCI_DEV_GPIO6,
			TISCI_DEV_GTC0,
			TISCI_DEV_MAIN2MCU_LVL_INTRTR0,
			TISCI_DEV_MAIN2MCU_PLS_INTRTR0,
			TISCI_DEV_TIMESYNC_INTRTR0,
			TISCI_DEV_GPIOMUX_INTRTR0,
			TISCI_DEV_CMPEVENT_INTRTR0,
			0
		}
	},

	{0,
		{
			TISCI_DEV_ECAP0,
			TISCI_DEV_ECAP1,
			TISCI_DEV_ECAP2,
			TISCI_DEV_EQEP0,
			TISCI_DEV_EQEP1,
			TISCI_DEV_EQEP2,
			TISCI_DEV_EPWM0,
			TISCI_DEV_EPWM1,
			TISCI_DEV_EPWM2,
			TISCI_DEV_EPWM3,
			TISCI_DEV_EPWM4,
			TISCI_DEV_EPWM5,
			0
		}
	},

	{0,
		{
			TISCI_DEV_ELM0,
			TISCI_DEV_GPMC0,
			0
		}
	},

	{0,
		{
			TISCI_DEV_FFI_MAIN_HC_CBASS_VD,
			TISCI_DEV_USB0,
			TISCI_DEV_VUSR_DUAL0,
			TISCI_DEV_MMCSD0,
			TISCI_DEV_PCIE1,
			TISCI_DEV_SA2_UL0,
			0
		}
	},

	{0,
		{
			TISCI_DEV_FFI_MAIN_RC_CBASS_VD,
			TISCI_DEV_MMCSD1,
			0
		}
	},

	{0,
		{
			TISCI_DEV_FFI_MAIN_AC_QM_CBASS_VD,
			TISCI_DEV_CODEC0,
			TISCI_DEV_PBIST11,
			TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPU_SS_0,TISCI_DEV_RTI15,
			TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_DFT_EMBED_PBIST_0,
			TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPUCORE_0,
			0
		}
	},

	{0,
		{
			TISCI_DEV_CODEC0,
			TISCI_DEV_PBIST11,
			0
		}
	},

	{0,
		{
			TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPU_SS_0,
			TISCI_DEV_RTI15,
			TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_DFT_EMBED_PBIST_0,
			TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPUCORE_0,
			0
		}
	},

	{0,
		{
			TISCI_DEV_UART0,
			TISCI_DEV_UART1,
			0
		}
	},

	{0,
		{
			TISCI_DEV_DSS_EDP0,
			TISCI_DEV_PBIST5,
			0
		}
	},

	{0,
		{
			TISCI_DEV_PBIST3,
			TISCI_DEV_PBIST0,
			TISCI_DEV_PBIST1,
			TISCI_DEV_PBIST4,
			0
		}
	},

#if !defined(BUILD_MCU2_0)
	{0,
		{
			TISCI_DEV_R5FSS0_CORE1,
			TISCI_DEV_RTI29,
			TISCI_DEV_PBIST2,
			0
		}
	},
#endif

	{0,
		{
			TISCI_DEV_R5FSS1_CORE1,
			TISCI_DEV_RTI31,
			TISCI_DEV_PBIST10,
			0
		}
	},

	{0,
		{
			TISCI_DEV_MCU_PBIST0,
			TISCI_DEV_MCU_PBIST1,
			0
		}
	},

	{0,
		{
			TISCI_DEV_MCAN4,
			TISCI_DEV_MCAN5,
			TISCI_DEV_MCAN6,
			TISCI_DEV_MCAN7,
			TISCI_DEV_MCAN8,
			TISCI_DEV_MCAN9,
			0
		}
	},

	{0,
		{
			TISCI_DEV_MCAN10,
			TISCI_DEV_MCAN11,
			TISCI_DEV_MCAN12,
			TISCI_DEV_MCAN13,
			TISCI_DEV_MCAN14,
			TISCI_DEV_MCAN15,
			TISCI_DEV_MCAN16,
			TISCI_DEV_MCAN17,
			0
		}
	},

	{0,
		{
			TISCI_DEV_I2C1,
			TISCI_DEV_I2C2,
			TISCI_DEV_I2C3,
			0
		}
	},

	{0,
		{
			TISCI_DEV_MCSPI0,
			TISCI_DEV_MCSPI1,
			TISCI_DEV_MCSPI2,
			TISCI_DEV_MCSPI3,
			0
		}
	},

	{0,
		{
			TISCI_DEV_WKUP_GPIO0,
			TISCI_DEV_WKUP_GPIO1,
			TISCI_DEV_WKUP_I2C0,
			TISCI_DEV_WKUP_UART0,
			0
		}
	}

	/*
	 * TODO: Need to get sequence to test these devices(R5F, C7X AND A72 core devices)

	 {0,
		{
			TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_0,
			TISCI_DEV_RTI16,
			TISCI_DEV_C71X_0_PBIST_VD,
			0
		}
	},

	{0,
		{
			TISCI_DEV_A72SS0,
			TISCI_DEV_A72SS0_CORE0_PBIST_WRAP,
			TISCI_DEV_A72SS0_CORE0,
			TISCI_DEV_RTI0,
			TISCI_DEV_A72SS0_CORE1,
			TISCI_DEV_RTI1,
			0
		}
	},

	{0,
		{
			TISCI_DEV_A72SS0_CORE0,
			TISCI_DEV_RTI0,
			0
		}
	},

	{0,
		{
			TISCI_DEV_A72SS0_CORE1,
			TISCI_DEV_RTI1,
			0
		}
	},

	{0,
		{
			TISCI_DEV_COMPUTE_CLUSTER0_C71SS1_0,
			TISCI_DEV_RTI17,
			TISCI_DEV_C71X_1_PBIST_VD,
			0
		}
	},

	{0,
		{
			TISCI_DEV_DDR0,
			TISCI_DEV_EMIF_DATA_0_VD,
			0
		}
	},

	{0,
		{
			TISCI_DEV_DDR1,
			TISCI_DEV_EMIF_DATA_1_VD,
			0
		}
	},

	{0,
		{
			TISCI_DEV_MCU_R5FSS0_CORE1,
			TISCI_DEV_MCU_RTI1,
			TISCI_DEV_MCU_PBIST2,
			0
		}
	},

	{0,
		{
			TISCI_DEV_R5FSS0_CORE0,
			TISCI_DEV_RTI28,
			0
		}
	},

	{0,
		{
			TISCI_DEV_R5FSS1_CORE0,
			TISCI_DEV_RTI30,
			0
		}
	},

	{0,
		{
			TISCI_DEV_MCU_R5FSS0_CORE0,
			TISCI_DEV_MCU_RTI0,
			0
		}
	},
	*/
};

uint32_t gDMAppDevicesWithDependencySize = sizeof(gDMAppDevicesWithDependency)/sizeof(gDMAppDevicesWithDependency[0]);
uint32_t  gDMAppMainAlwaysOnDevicesSize =  sizeof(gDMAppMainAlwaysOnDevices)/sizeof(gDMAppMainAlwaysOnDevices[0]);
uint32_t gDMAppMcuAlwaysOnDevicesSize = sizeof(gDMAppMcuAlwaysOnDevices)/sizeof(gDMAppMcuAlwaysOnDevices[0]);
uint32_t gDMAppDevicesWithoutDependencySize = sizeof(gDMAppDevicesWithoutDependency)/sizeof((gDMAppDevicesWithoutDependency)[0]);

/* These are DDR devices and we are not testing these devices in DM
 * because DDR is a critical device used for program/data memory.
 * Disabling DDR will lead to system crash.
{
	TISCI_DEV_EMIF_DATA_0_VD,
	TISCI_DEV_EMIF_DATA_1_VD,
}
 */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* None */
