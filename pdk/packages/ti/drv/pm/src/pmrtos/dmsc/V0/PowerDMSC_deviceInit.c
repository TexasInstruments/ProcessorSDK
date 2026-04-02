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

/**
 * \file    PowerDMSC_deviceInit.c
 *
 * \brief   This file contains the DMSC based device initialization.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <Power.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/pm/pmlib.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

pmlibSysConfigPowerStateParams_t gInputTable[] =
{
    {TISCI_DEV_MCU_ADC0,                                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_ADC1,                                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CAL0,                                    PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_CMPEVENT_INTRTR0,                        PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_CPSW0,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CPT2_AGGR0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_CPT2_AGGR0,                          PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_STM0,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DCC0,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DCC1,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DCC2,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DCC3,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DCC4,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DCC5,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DCC6,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DCC7,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_DCC0,                                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_DCC1,                                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_DCC2,                                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DDRSS0,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DEBUGSS_WRAP0,                           PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_DMSC0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_TIMER0,                                  PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_TIMER1,                                  PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_TIMER10,                                 PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_TIMER11,                                 PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_TIMER2,                                  PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_TIMER3,                                  PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_TIMER4,                                  PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_TIMER5,                                  PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_TIMER6,                                  PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_TIMER7,                                  PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_TIMER8,                                  PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_TIMER9,                                  PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_MCU_TIMER0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_TIMER1,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_TIMER2,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_TIMER3,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_ECAP0,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_EHRPWM0,                                 PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_EHRPWM1,                                 PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_EHRPWM2,                                 PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_EHRPWM3,                                 PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_EHRPWM4,                                 PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_EHRPWM5,                                 PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_ELM0,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MMCSD0,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MMCSD1,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_EQEP0,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_EQEP1,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_EQEP2,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_ESM0,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_ESM0,                                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_ESM0,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_FSS0_FSAS_0,                         PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_FSS0_HYPERBUS0,                      PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_FSS0_OSPI_0,                         PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_FSS0_OSPI_1,                         PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_GIC0,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_GPIO0,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_GPIO1,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_GPIO0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_GPMC0,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_GTC0,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PRU_ICSSG0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PRU_ICSSG1,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PRU_ICSSG2,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_GPU0,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CCDEBUGSS0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DSS0,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DEBUGSS0,                                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_EFUSE0,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PSC0,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_DEBUGSS0,                            PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_EFUSE0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PBIST0,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PBIST1,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_PBIST0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PLLCTRL0,                                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_PLLCTRL0,                           PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_ROM0,                                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_PSC0,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_VTM0,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DEBUGSUSPENDRTR0,                        PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CBASS0,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CBASS_DEBUG0,                            PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CBASS_FW0,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CBASS_INFRA0,                            PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_ECC_AGGR0,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_ECC_AGGR1,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_ECC_AGGR2,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_CBASS0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_CBASS_DEBUG0,                        PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_CBASS_FW0,                           PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_ECC_AGGR0,                           PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_ECC_AGGR1,                           PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_CBASS0,                             PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_ECC_AGGR0,                          PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_CBASS_FW0,                          PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MAIN2MCU_LVL_INTRTR0,                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MAIN2MCU_PLS_INTRTR0,                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CTRL_MMR0,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_GPIOMUX_INTRTR0,                         PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PLL_MMR0,                                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_MCAN0,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_MCAN1,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCASP0,                                  PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_MCASP1,                                  PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_MCASP2,                                  PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_MCU_CTRL_MMR0,                           PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_PLL_MMR0,                            PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_SEC_MMR0,                            PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_I2C0,                                    PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_I2C1,                                    PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_I2C2,                                    PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_I2C3,                                    PMLIB_SYS_CONFIG_DISABLED},
    {TISCI_DEV_MCU_I2C0,                                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_I2C0,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_MSRAM0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_DFTSS0,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_NAVSS0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PCIE0,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PCIE1,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PDMA_DEBUG0,                             PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PDMA0,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PDMA1,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_PDMA0,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_PDMA1,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_PSRAM0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_PSRAMECC0,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_ARMSS0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_RTI0,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_RTI1,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_RTI2,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_RTI3,                                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_RTI0,                                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_RTI1,                                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_SA2_UL0,                                 PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCSPI0,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCSPI1,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCSPI2,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCSPI3,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCSPI4,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_MCSPI0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_MCSPI1,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_MCSPI2,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_TIMESYNC_INTRTR0,                        PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_UART0,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_UART1,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_UART2,                                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_UART0,                               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_UART0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_USB3SS0,                                 PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_USB3SS1,                                 PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_SERDES0,                                 PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_SERDES1,                                 PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_CTRL_MMR0,                          PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_GPIOMUX_INTRTR0,                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_BOARD0,                                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_ARMSS0_CPU0,                         PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_ARMSS0,                              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_WKUP_DMSC0_CORTEX_M3_0,                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_CPTS0,                            PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MAILBOX0_CLUSTER0,                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MAILBOX0_CLUSTER1,                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MAILBOX0_CLUSTER2,                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MAILBOX0_CLUSTER3,                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MAILBOX0_CLUSTER4,                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MAILBOX0_CLUSTER5,                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MAILBOX0_CLUSTER6,                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MAILBOX0_CLUSTER7,                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MAILBOX0_CLUSTER8,                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MAILBOX0_CLUSTER9,                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MAILBOX0_CLUSTER10,               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MAILBOX0_CLUSTER11,               PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MCRC0,                            PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_PVU0,                             PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_PVU1,                             PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_UDMASS_INTA0,                     PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MODSS_INTA0,                      PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_MODSS_INTA1,                      PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_INTR_ROUTER_0,                    PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_TIMER_MGR0,                       PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_TIMER_MGR1,                       PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_PROXY0,                           PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_RINGACC0,                         PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_NAVSS0_UDMAP0,                           PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_NAVSS0_INTR_AGGR_0,                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_NAVSS0_INTR_ROUTER_0,                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_NAVSS0_PROXY0,                       PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_NAVSS0_MCRC0,                        PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_NAVSS0_UDMAP0,                       PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_NAVSS0_RINGACC0,                     PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CPT2_PROBE_VBUSM_MAIN_NAVSRAMLO_4,       PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CPT2_PROBE_VBUSM_MCU_FSS_S1_3,           PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CPT2_PROBE_VBUSM_MCU_EXPORT_SLV_0,       PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CPT2_PROBE_VBUSM_MAIN_NAVSRAMHI_3,       PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CPT2_PROBE_VBUSM_MCU_SRAM_SLV_1,         PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CPT2_PROBE_VBUSM_MAIN_NAVDDRHI_5,        PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CPT2_PROBE_VBUSM_MAIN_NAVDDRLO_6,        PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CPT2_PROBE_VBUSM_MAIN_CAL0_0,            PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CPT2_PROBE_VBUSM_MAIN_DSS_2,             PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_CPT2_PROBE_VBUSM_MCU_FSS_S0_2,           PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_OLDI_TX_CORE_MAIN_0,                     PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_COMPUTE_CLUSTER_MSMC0,                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_COMPUTE_CLUSTER_PBIST0,                  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_COMPUTE_CLUSTER_CPAC0,                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_COMPUTE_CLUSTER_CPAC_PBIST0,             PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_COMPUTE_CLUSTER_CPAC1,                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_COMPUTE_CLUSTER_CPAC_PBIST1,             PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_COMPUTE_CLUSTER_A53_0,                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_COMPUTE_CLUSTER_A53_1,                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_COMPUTE_CLUSTER_A53_2,                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_COMPUTE_CLUSTER_A53_3,                   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_K3_ARM_ATB_FUNNEL_3_32_MCU_0,            PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_ICEMELTER_WKUP_0,                        PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_K3_LED_MAIN_0,                           PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_VDC_DATA_VBUSM_32B_REF_WKUP2MCU,         PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_VDC_DATA_VBUSM_32B_REF_MCU2WKUP,         PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_VDC_DATA_VBUSM_64B_REF_MAIN2MCU,         PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_VDC_DATA_VBUSM_64B_REF_MCU2MAIN,         PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_VDC_DMSC_DBG_VBUSP_32B_REF_DBG2DMSC,     PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_VDC_INFRA_VBUSP_32B_REF_WKUP2MAIN_INFRA, PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_VDC_INFRA_VBUSP_32B_REF_MCU2MAIN_INFRA,  PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_VDC_SOC_FW_VBUSP_32B_REF_FWWKUP2MCU,     PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_VDC_SOC_FW_VBUSP_32B_REF_FWMCU2MAIN,     PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_VDC_MCU_DBG_VBUSP_32B_REF_DBGMAIN2MCU,   PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_VDC_NAV_PSIL_128B_REF_MAIN2MCU,          PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_GS80PRG_SOC_WRAP_WKUP_0,                 PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_GS80PRG_MCU_WRAP_WKUP_0,                 PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MX_WAKEUP_RESET_SYNC_WKUP_0,             PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MX_EFUSE_MAIN_CHAIN_MAIN_0,              PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MX_EFUSE_MCU_CHAIN_MCU_0,                PMLIB_SYS_CONFIG_ALWAYS_ENABLED},
    {TISCI_DEV_MCU_ARMSS0_CPU1,                         PMLIB_SYS_CONFIG_ALWAYS_ENABLED}
};

const uint32_t numTableEntries = sizeof (gInputTable) /
                                 sizeof (pmlibSysConfigPowerStateParams_t);

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t DMSC_InitModulePowerState()
{
    int32_t retVal = PM_SUCCESS;
    uint32_t i = 0U;
    pmlibSysConfigPowerState_t    modState;

    for (i = 0U; i < numTableEntries; i++)
    {
        PMLIBSysConfigGetPowerState(gInputTable[i].modId,&modState,NULL);
        if(modState != gInputTable[i].pwrState)
        {
            if (gInputTable[i].pwrState == PMLIB_SYS_CONFIG_ALWAYS_ENABLED)
            {
                retVal = Power_setDependency(gInputTable[i].modId);
            }
            if (gInputTable[i].pwrState == PMLIB_SYS_CONFIG_DISABLED)
            {
                retVal = PMLIBSysConfigSetPowerState(&gInputTable[i], 1U,
                                                PM_TIMEOUT_INFINITE, NULL);
            }
            if (retVal != PM_SUCCESS)
            {
                break;
            }
        }
    }
    return retVal;
}

/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */

/* None */
