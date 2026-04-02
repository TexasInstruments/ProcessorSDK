/********************************************************************
*
* PRU_ICSSG0 BASEADDRESS. header file
*
* Copyright (C) 2015-2019 Texas Instruments Incorporated.
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
#ifndef CSLR_PRU_ICSSG0_BASEADDRESS_H_
#define CSLR_PRU_ICSSG0_BASEADDRESS_H_

#include <ti/csl/cslr.h>
#include <ti/csl/tistdtypes.h>
#ifdef __cplusplus
extern "C"
{
#endif

/*
* Auto-generated PRU_ICSSG0 Base Address File:
*/

#define CSL_PRU_ICSSG0_DRAM0_SLV_RAM_BASE                                                          (0x0UL)
#define CSL_PRU_ICSSG0_DRAM0_SLV_RAM_SIZE                                                          (0x2000UL)
#define CSL_PRU_ICSSG0_DRAM1_SLV_RAM_BASE                                                          (0x2000UL)
#define CSL_PRU_ICSSG0_DRAM1_SLV_RAM_SIZE                                                          (0x2000UL)
#define CSL_PRU_ICSSG0_PR1_RTU0_PR1_RTU0_IRAM_RAM_BASE                                             (0x4000UL)
#define CSL_PRU_ICSSG0_PR1_RTU0_PR1_RTU0_IRAM_RAM_SIZE                                             (0x2000UL)
#define CSL_PRU_ICSSG0_PR1_RTU1_PR1_RTU1_IRAM_RAM_BASE                                             (0x6000UL)
#define CSL_PRU_ICSSG0_PR1_RTU1_PR1_RTU1_IRAM_RAM_SIZE                                             (0x2000UL)
#define CSL_PRU_ICSSG0_RAT_SLICE0_CFG_BASE                                                         (0x8000UL)
#define CSL_PRU_ICSSG0_RAT_SLICE0_CFG_SIZE                                                         (0x1000UL)
#define CSL_PRU_ICSSG0_RAT_SLICE1_CFG_BASE                                                         (0x9000UL)
#define CSL_PRU_ICSSG0_RAT_SLICE1_CFG_SIZE                                                         (0x1000UL)
#define CSL_PRU_ICSSG0_PR1_PDSP_TX0_IRAM_RAM_BASE                                                  (0xa000UL)
#define CSL_PRU_ICSSG0_PR1_PDSP_TX0_IRAM_RAM_SIZE                                                  (0x2000UL)
#define CSL_PRU_ICSSG0_PR1_PDSP_TX1_IRAM_RAM_BASE                                                  (0xc000UL)
#define CSL_PRU_ICSSG0_PR1_PDSP_TX1_IRAM_RAM_SIZE                                                  (0x2000UL)
#define CSL_PRU_ICSSG0_RAM_SLV_RAM_BASE                                                            (0x10000UL)
#define CSL_PRU_ICSSG0_RAM_SLV_RAM_SIZE                                                            (0x10000UL)
#define CSL_PRU_ICSSG0_PR1_ICSS_INTC_INTC_SLV_BASE                                                 (0x20000UL)
#define CSL_PRU_ICSSG0_PR1_ICSS_INTC_INTC_SLV_SIZE                                                 (0x2000UL)
#define CSL_PRU_ICSSG0_PR1_PDSP0_IRAM_BASE                                                         (0x22000UL)
#define CSL_PRU_ICSSG0_PR1_PDSP0_IRAM_SIZE                                                         (0x100UL)
#define CSL_PRU_ICSSG0_PR1_PDSP0_IRAM_DEBUG_BASE                                                   (0x22400UL)
#define CSL_PRU_ICSSG0_PR1_PDSP0_IRAM_DEBUG_SIZE                                                   (0x100UL)
#define CSL_PRU_ICSSG0_PR1_RTU0_PR1_RTU0_IRAM_BASE                                                 (0x23000UL)
#define CSL_PRU_ICSSG0_PR1_RTU0_PR1_RTU0_IRAM_SIZE                                                 (0x100UL)
#define CSL_PRU_ICSSG0_PR1_RTU0_PR1_RTU0_IRAM_DEBUG_BASE                                           (0x23400UL)
#define CSL_PRU_ICSSG0_PR1_RTU0_PR1_RTU0_IRAM_DEBUG_SIZE                                           (0x100UL)
#define CSL_PRU_ICSSG0_PR1_RTU1_PR1_RTU1_IRAM_BASE                                                 (0x23800UL)
#define CSL_PRU_ICSSG0_PR1_RTU1_PR1_RTU1_IRAM_SIZE                                                 (0x100UL)
#define CSL_PRU_ICSSG0_PR1_RTU1_PR1_RTU1_IRAM_DEBUG_BASE                                           (0x23c00UL)
#define CSL_PRU_ICSSG0_PR1_RTU1_PR1_RTU1_IRAM_DEBUG_SIZE                                           (0x100UL)
#define CSL_PRU_ICSSG0_PR1_PDSP1_IRAM_BASE                                                         (0x24000UL)
#define CSL_PRU_ICSSG0_PR1_PDSP1_IRAM_SIZE                                                         (0x100UL)
#define CSL_PRU_ICSSG0_PR1_PDSP1_IRAM_DEBUG_BASE                                                   (0x24400UL)
#define CSL_PRU_ICSSG0_PR1_PDSP1_IRAM_DEBUG_SIZE                                                   (0x100UL)
#define CSL_PRU_ICSSG0_PR1_PROT_SLV_BASE                                                           (0x24c00UL)
#define CSL_PRU_ICSSG0_PR1_PROT_SLV_SIZE                                                           (0x100UL)
#define CSL_PRU_ICSSG0_PR1_PDSP_TX0_IRAM_BASE                                                      (0x25000UL)
#define CSL_PRU_ICSSG0_PR1_PDSP_TX0_IRAM_SIZE                                                      (0x100UL)
#define CSL_PRU_ICSSG0_PR1_PDSP_TX0_IRAM_DEBUG_BASE                                                (0x25400UL)
#define CSL_PRU_ICSSG0_PR1_PDSP_TX0_IRAM_DEBUG_SIZE                                                (0x100UL)
#define CSL_PRU_ICSSG0_PR1_PDSP_TX1_IRAM_BASE                                                      (0x25800UL)
#define CSL_PRU_ICSSG0_PR1_PDSP_TX1_IRAM_SIZE                                                      (0x100UL)
#define CSL_PRU_ICSSG0_PR1_PDSP_TX1_IRAM_DEBUG_BASE                                                (0x25c00UL)
#define CSL_PRU_ICSSG0_PR1_PDSP_TX1_IRAM_DEBUG_SIZE                                                (0x100UL)
#define CSL_PRU_ICSSG0_PR1_CFG_SLV_BASE                                                            (0x26000UL)
#define CSL_PRU_ICSSG0_PR1_CFG_SLV_SIZE                                                            (0x200UL)
#define CSL_PRU_ICSSG0_PA_STAT_WRAP_PA_SLV_QSTAT_BASE                                              (0x27000UL)
#define CSL_PRU_ICSSG0_PA_STAT_WRAP_PA_SLV_QSTAT_SIZE                                              (0x800UL)
#define CSL_PRU_ICSSG0_PR1_ICSS_UART_UART_SLV_BASE                                                 (0x28000UL)
#define CSL_PRU_ICSSG0_PR1_ICSS_UART_UART_SLV_SIZE                                                 (0x40UL)
#define CSL_PRU_ICSSG0_PR1_TASKS_MGR_PRU0_PR1_TASKS_MGR_PRU0_MMR_BASE                              (0x2a000UL)
#define CSL_PRU_ICSSG0_PR1_TASKS_MGR_PRU0_PR1_TASKS_MGR_PRU0_MMR_SIZE                              (0x100UL)
#define CSL_PRU_ICSSG0_PR1_TASKS_MGR_RTU0_PR1_TASKS_MGR_RTU0_MMR_BASE                              (0x2a100UL)
#define CSL_PRU_ICSSG0_PR1_TASKS_MGR_RTU0_PR1_TASKS_MGR_RTU0_MMR_SIZE                              (0x100UL)
#define CSL_PRU_ICSSG0_PR1_TASKS_MGR_PRU1_PR1_TASKS_MGR_PRU1_MMR_BASE                              (0x2a200UL)
#define CSL_PRU_ICSSG0_PR1_TASKS_MGR_PRU1_PR1_TASKS_MGR_PRU1_MMR_SIZE                              (0x100UL)
#define CSL_PRU_ICSSG0_PR1_TASKS_MGR_RTU1_PR1_TASKS_MGR_RTU1_MMR_BASE                              (0x2a300UL)
#define CSL_PRU_ICSSG0_PR1_TASKS_MGR_RTU1_PR1_TASKS_MGR_RTU1_MMR_SIZE                              (0x100UL)
#define CSL_PRU_ICSSG0_PR1_TASKS_MGR_PRU_TX0_PR1_TASKS_MGR_PRU_TX0_MMR_BASE                        (0x2a400UL)
#define CSL_PRU_ICSSG0_PR1_TASKS_MGR_PRU_TX0_PR1_TASKS_MGR_PRU_TX0_MMR_SIZE                        (0x100UL)
#define CSL_PRU_ICSSG0_PR1_TASKS_MGR_PRU_TX1_PR1_TASKS_MGR_PRU_TX1_MMR_BASE                        (0x2a500UL)
#define CSL_PRU_ICSSG0_PR1_TASKS_MGR_PRU_TX1_PR1_TASKS_MGR_PRU_TX1_MMR_SIZE                        (0x100UL)
#define CSL_PRU_ICSSG0_PA_STAT_WRAP_PA_SLV_CSTAT_BASE                                              (0x2c000UL)
#define CSL_PRU_ICSSG0_PA_STAT_WRAP_PA_SLV_CSTAT_SIZE                                              (0x800UL)
#define CSL_PRU_ICSSG0_IEP0_BASE                                                                   (0x2e000UL)
#define CSL_PRU_ICSSG0_IEP0_SIZE                                                                   (0x1000UL)
#define CSL_PRU_ICSSG0_IEP1_BASE                                                                   (0x2f000UL)
#define CSL_PRU_ICSSG0_IEP1_SIZE                                                                   (0x1000UL)
#define CSL_PRU_ICSSG0_PR1_ICSS_ECAP0_ECAP_SLV_BASE                                                (0x30000UL)
#define CSL_PRU_ICSSG0_PR1_ICSS_ECAP0_ECAP_SLV_SIZE                                                (0x100UL)
#define CSL_PRU_ICSSG0_PR1_MII_RT_PR1_MII_RT_CFG_BASE                                              (0x32000UL)
#define CSL_PRU_ICSSG0_PR1_MII_RT_PR1_MII_RT_CFG_SIZE                                              (0x100UL)
#define CSL_PRU_ICSSG0_PR1_MII_RT_PR1_SGMII0_CFG_SGMII0_BASE                                       (0x32100UL)
#define CSL_PRU_ICSSG0_PR1_MII_RT_PR1_SGMII0_CFG_SGMII0_SIZE                                       (0x100UL)
#define CSL_PRU_ICSSG0_PR1_MII_RT_PR1_SGMII1_CFG_SGMII1_BASE                                       (0x32200UL)
#define CSL_PRU_ICSSG0_PR1_MII_RT_PR1_SGMII1_CFG_SGMII1_SIZE                                       (0x100UL)
#define CSL_PRU_ICSSG0_PR1_MDIO_V1P7_MDIO_BASE                                                     (0x32400UL)
#define CSL_PRU_ICSSG0_PR1_MDIO_V1P7_MDIO_SIZE                                                     (0x100UL)
#define CSL_PRU_ICSSG0_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_BASE                                     (0x33000UL)
#define CSL_PRU_ICSSG0_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_SIZE                                     (0x1000UL)
#define CSL_PRU_ICSSG0_PR1_PDSP0_IRAM_RAM_BASE                                                     (0x34000UL)
#define CSL_PRU_ICSSG0_PR1_PDSP0_IRAM_RAM_SIZE                                                     (0x4000UL)
#define CSL_PRU_ICSSG0_PR1_PDSP1_IRAM_RAM_BASE                                                     (0x38000UL)
#define CSL_PRU_ICSSG0_PR1_PDSP1_IRAM_RAM_SIZE                                                     (0x4000UL)
#define CSL_PRU_ICSSG0_PA_STAT_WRAP_PA_SLV_BASE                                                    (0x3c000UL)
#define CSL_PRU_ICSSG0_PA_STAT_WRAP_PA_SLV_SIZE                                                    (0x100UL)
#define CSL_PRU_ICSSG0_RAT_REGION0_BASE                                                            (0x40000UL)
#define CSL_PRU_ICSSG0_RAT_REGION0_SIZE                                                            (0xfffc0000UL)

#ifdef __cplusplus
}
#endif
#endif /* CSLR_PRU_ICSSG0_BASEADDRESS_H_ */

