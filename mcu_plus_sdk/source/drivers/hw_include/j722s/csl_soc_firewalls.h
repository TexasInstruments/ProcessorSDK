/********************************************************************
*
* SOC FIREWALL PROPERTIES. header file
*
* Copyright (C) 2015-2023 Texas Instruments Incorporated.
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
#ifndef CSLR_SOC_FW_H_
#define CSLR_SOC_FW_H_

#include <drivers/hw_include/cslr.h>
#include <drivers/hw_include/tistdtypes.h>
#ifdef __cplusplus
extern "C"
{
#endif

/*
* Auto-generated CSL definitions for SoC Firewalls:
*/

#define CSL_FW_SECURITY                                                                            (0U)
#define CSL_FW_CHANNEL                                                                             (1U)
#define CSL_FW_MST                                                                                 (2U)

/* Standard Security Master Firewall Definitions */

/* Standard Security Slave Firewall Definitions */

/* Properties of firewall at slave: DDR32SS0_SDRAM_FW0 */
#define CSL_STD_FW_DDR32SS0_SDRAM_FW0_ID                                                           (0U)
#define CSL_STD_FW_DDR32SS0_SDRAM_FW0_TYPE                                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_DDR32SS0_SDRAM_FW0_MMR_BASE                                                     (0x00000045000000U)
#define CSL_STD_FW_DDR32SS0_SDRAM_FW0_NUM_REGIONS                                                  (16U)
#define CSL_STD_FW_DDR32SS0_SDRAM_FW0_NUM_PRIV_IDS_PER_REGION                                      (3U)
#define CSL_STD_FW_DDR32SS0_SDRAM_FW0_SDRAM_START                                                  (0x00000080000000U)
#define CSL_STD_FW_DDR32SS0_SDRAM_FW0_SDRAM_END                                                    (0x000000ffffffffU)

/* Properties of firewall at slave: DDR32SS0_SDRAM_FW1 */
#define CSL_STD_FW_DDR32SS0_SDRAM_FW1_ID                                                           (1U)
#define CSL_STD_FW_DDR32SS0_SDRAM_FW1_TYPE                                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_DDR32SS0_SDRAM_FW1_MMR_BASE                                                     (0x00000045000400U)
#define CSL_STD_FW_DDR32SS0_SDRAM_FW1_NUM_REGIONS                                                  (16U)
#define CSL_STD_FW_DDR32SS0_SDRAM_FW1_NUM_PRIV_IDS_PER_REGION                                      (3U)
#define CSL_STD_FW_DDR32SS0_SDRAM_FW1_SDRAM_START                                                  (0x00000080000000U)
#define CSL_STD_FW_DDR32SS0_SDRAM_FW1_SDRAM_END                                                    (0x000000ffffffffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_ROM_TABLE_0_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_0_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_0_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_0_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_0_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_0_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_0_ROM_TABLE_0_0_START                                 (0x00000700000000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_0_ROM_TABLE_0_0_END                                   (0x00000700000fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_RESV0_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_0_ID                                                        (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_0_TYPE                                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_0_MMR_BASE                                                  (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_0_NUM_REGIONS                                               (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_0_NUM_PRIV_IDS_PER_REGION                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_0_RESV0_0_START                                             (0x00000700001000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_0_RESV0_0_END                                               (0x00000700001fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CFGAP_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_0_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_0_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_0_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_0_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_0_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_0_CFGAP0_START                                          (0x00000700002000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_0_CFGAP0_END                                            (0x000007000020ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_APBAP_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_0_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_0_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_0_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_0_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_0_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_0_APBAP0_START                                          (0x00000700002100U)
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_0_APBAP0_END                                            (0x000007000021ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_AXIAP_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_0_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_0_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_0_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_0_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_0_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_0_AXIAP0_START                                          (0x00000700002200U)
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_0_AXIAP0_END                                            (0x000007000022ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_PWRAP_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_0_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_0_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_0_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_0_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_0_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_0_PWRAP0_START                                          (0x00000700002300U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_0_PWRAP0_END                                            (0x000007000023ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_PVIEW_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_0_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_0_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_0_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_0_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_0_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_0_PVIEW0_START                                          (0x00000700002400U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_0_PVIEW0_END                                            (0x000007000024ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_JTAGAP_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_0_ID                                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_0_TYPE                                                 (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_0_MMR_BASE                                             (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_0_NUM_REGIONS                                          (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_0_NUM_PRIV_IDS_PER_REGION                              (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_0_JTAGAP0_START                                        (0x00000700002500U)
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_0_JTAGAP0_END                                          (0x000007000025ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_SECAP_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_0_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_0_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_0_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_0_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_0_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_0_SECAP0_START                                          (0x00000700002600U)
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_0_SECAP0_END                                            (0x000007000026ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX0_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_0_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_0_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_0_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_0_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_0_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_0_CORTEX0_CFG0_START                                  (0x00000700002700U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_0_CORTEX0_CFG0_END                                    (0x000007000027ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX1_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_0_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_0_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_0_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_0_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_0_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_0_CORTEX1_CFG0_START                                  (0x00000700002800U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_0_CORTEX1_CFG0_END                                    (0x000007000028ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX2_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_0_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_0_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_0_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_0_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_0_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_0_CORTEX2_CFG0_START                                  (0x00000700002900U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_0_CORTEX2_CFG0_END                                    (0x000007000029ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX3_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_0_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_0_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_0_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_0_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_0_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_0_CORTEX3_CFG0_START                                  (0x00000700002a00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_0_CORTEX3_CFG0_END                                    (0x00000700002affU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX4_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_0_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_0_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_0_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_0_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_0_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_0_CORTEX4_CFG0_START                                  (0x00000700002b00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_0_CORTEX4_CFG0_END                                    (0x00000700002bffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX5_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_0_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_0_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_0_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_0_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_0_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_0_CORTEX5_CFG0_START                                  (0x00000700002c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_0_CORTEX5_CFG0_END                                    (0x00000700002cffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX6_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_0_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_0_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_0_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_0_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_0_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_0_CORTEX6_CFG0_START                                  (0x00000700002d00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_0_CORTEX6_CFG0_END                                    (0x00000700002dffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX7_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_0_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_0_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_0_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_0_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_0_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_0_CORTEX7_CFG0_START                                  (0x00000700002e00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_0_CORTEX7_CFG0_END                                    (0x00000700002effU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX8_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_0_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_0_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_0_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_0_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_0_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_0_CORTEX8_CFG0_START                                  (0x00000700002f00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_0_CORTEX8_CFG0_END                                    (0x00000700002fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_RESV1_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_0_ID                                                        (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_0_TYPE                                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_0_MMR_BASE                                                  (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_0_NUM_REGIONS                                               (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_0_NUM_PRIV_IDS_PER_REGION                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_0_RESV1_0_START                                             (0x00000700003000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_0_RESV1_0_END                                               (0x00000700003fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_RESV2_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_0_ID                                                        (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_0_TYPE                                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_0_MMR_BASE                                                  (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_0_NUM_REGIONS                                               (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_0_NUM_PRIV_IDS_PER_REGION                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_0_RESV2_0_START                                             (0x00000700004000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_0_RESV2_0_END                                               (0x00000702003fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_ROM_TABLE_1_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_0_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_0_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_0_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_0_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_0_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_0_ROM_TABLE_1_0_START                                 (0x00000720000000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_0_ROM_TABLE_1_0_END                                   (0x00000720000fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CSCTI_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_0_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_0_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_0_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_0_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_0_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_0_CSCTI0_START                                          (0x00000720001000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_0_CSCTI0_END                                            (0x00000720001fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_DRM_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_0_ID                                                      (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_0_TYPE                                                    (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_0_MMR_BASE                                                (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_0_NUM_REGIONS                                             (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_0_NUM_PRIV_IDS_PER_REGION                                 (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_0_DRM0_START                                              (0x00000720002000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_0_DRM0_END                                                (0x00000720002fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_RESV3_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_0_ID                                                        (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_0_TYPE                                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_0_MMR_BASE                                                  (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_0_NUM_REGIONS                                               (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_0_NUM_PRIV_IDS_PER_REGION                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_0_RESV3_0_START                                             (0x00000720003000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_0_RESV3_0_END                                               (0x00000720003fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CSTPIU_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_0_ID                                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_0_TYPE                                                 (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_0_MMR_BASE                                             (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_0_NUM_REGIONS                                          (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_0_NUM_PRIV_IDS_PER_REGION                              (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_0_CSTPIU0_START                                        (0x00000720004000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_0_CSTPIU0_END                                          (0x00000720004fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CTF_CFG_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_0_ID                                                      (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_0_TYPE                                                    (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_0_MMR_BASE                                                (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_0_NUM_REGIONS                                             (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_0_NUM_PRIV_IDS_PER_REGION                                 (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_0_CTF0_START                                              (0x00000720005000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_0_CTF0_END                                                (0x00000720005fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_RESV4_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_0_ID                                                        (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_0_TYPE                                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_0_MMR_BASE                                                  (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_0_NUM_REGIONS                                               (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_0_NUM_PRIV_IDS_PER_REGION                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_0_RESV4_0_START                                             (0x00000720006000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_0_RESV4_0_END                                               (0x00000721005fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_EXT_APB_0 */
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_0_ID                                                      (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_0_TYPE                                                    (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_0_MMR_BASE                                                (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_0_NUM_REGIONS                                             (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_0_NUM_PRIV_IDS_PER_REGION                                 (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_0_EXT_APB0_START                                          (0x00000730000000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_0_EXT_APB0_END                                            (0x0000073fffffffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_ROM_TABLE_0_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_1_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_1_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_1_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_1_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_1_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_1_ROM_TABLE_0_1_START                                 (0x00000740000000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_0_1_ROM_TABLE_0_1_END                                   (0x00000740000fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_RESV0_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_1_ID                                                        (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_1_TYPE                                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_1_MMR_BASE                                                  (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_1_NUM_REGIONS                                               (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_1_NUM_PRIV_IDS_PER_REGION                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_1_RESV0_1_START                                             (0x00000740001000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV0_1_RESV0_1_END                                               (0x00000740001fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CFGAP_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_1_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_1_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_1_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_1_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_1_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_1_CFGAP1_START                                          (0x00000740002000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CFGAP_CFG_1_CFGAP1_END                                            (0x000007400020ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_APBAP_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_1_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_1_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_1_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_1_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_1_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_1_APBAP1_START                                          (0x00000740002100U)
#define CSL_STD_FW_DEBUGSS_WRAP0_APBAP_CFG_1_APBAP1_END                                            (0x000007400021ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_AXIAP_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_1_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_1_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_1_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_1_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_1_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_1_AXIAP1_START                                          (0x00000740002200U)
#define CSL_STD_FW_DEBUGSS_WRAP0_AXIAP_CFG_1_AXIAP1_END                                            (0x000007400022ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_PWRAP_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_1_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_1_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_1_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_1_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_1_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_1_PWRAP1_START                                          (0x00000740002300U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PWRAP_CFG_1_PWRAP1_END                                            (0x000007400023ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_PVIEW_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_1_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_1_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_1_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_1_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_1_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_1_PVIEW1_START                                          (0x00000740002400U)
#define CSL_STD_FW_DEBUGSS_WRAP0_PVIEW_CFG_1_PVIEW1_END                                            (0x000007400024ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_JTAGAP_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_1_ID                                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_1_TYPE                                                 (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_1_MMR_BASE                                             (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_1_NUM_REGIONS                                          (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_1_NUM_PRIV_IDS_PER_REGION                              (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_1_JTAGAP1_START                                        (0x00000740002500U)
#define CSL_STD_FW_DEBUGSS_WRAP0_JTAGAP_CFG_1_JTAGAP1_END                                          (0x000007400025ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_SECAP_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_1_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_1_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_1_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_1_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_1_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_1_SECAP1_START                                          (0x00000740002600U)
#define CSL_STD_FW_DEBUGSS_WRAP0_SECAP_CFG_1_SECAP1_END                                            (0x000007400026ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX0_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_1_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_1_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_1_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_1_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_1_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_1_CORTEX0_CFG1_START                                  (0x00000740002700U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX0_CFG_1_CORTEX0_CFG1_END                                    (0x000007400027ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX1_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_1_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_1_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_1_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_1_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_1_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_1_CORTEX1_CFG1_START                                  (0x00000740002800U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX1_CFG_1_CORTEX1_CFG1_END                                    (0x000007400028ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX2_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_1_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_1_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_1_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_1_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_1_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_1_CORTEX2_CFG1_START                                  (0x00000740002900U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX2_CFG_1_CORTEX2_CFG1_END                                    (0x000007400029ffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX3_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_1_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_1_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_1_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_1_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_1_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_1_CORTEX3_CFG1_START                                  (0x00000740002a00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX3_CFG_1_CORTEX3_CFG1_END                                    (0x00000740002affU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX4_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_1_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_1_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_1_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_1_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_1_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_1_CORTEX4_CFG1_START                                  (0x00000740002b00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX4_CFG_1_CORTEX4_CFG1_END                                    (0x00000740002bffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX5_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_1_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_1_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_1_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_1_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_1_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_1_CORTEX5_CFG1_START                                  (0x00000740002c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX5_CFG_1_CORTEX5_CFG1_END                                    (0x00000740002cffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX6_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_1_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_1_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_1_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_1_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_1_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_1_CORTEX6_CFG1_START                                  (0x00000740002d00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX6_CFG_1_CORTEX6_CFG1_END                                    (0x00000740002dffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX7_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_1_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_1_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_1_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_1_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_1_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_1_CORTEX7_CFG1_START                                  (0x00000740002e00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX7_CFG_1_CORTEX7_CFG1_END                                    (0x00000740002effU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CORTEX8_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_1_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_1_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_1_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_1_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_1_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_1_CORTEX8_CFG1_START                                  (0x00000740002f00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CORTEX8_CFG_1_CORTEX8_CFG1_END                                    (0x00000740002fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_RESV1_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_1_ID                                                        (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_1_TYPE                                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_1_MMR_BASE                                                  (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_1_NUM_REGIONS                                               (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_1_NUM_PRIV_IDS_PER_REGION                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_1_RESV1_1_START                                             (0x00000740003000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV1_1_RESV1_1_END                                               (0x00000740003fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_RESV2_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_1_ID                                                        (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_1_TYPE                                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_1_MMR_BASE                                                  (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_1_NUM_REGIONS                                               (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_1_NUM_PRIV_IDS_PER_REGION                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_1_RESV2_1_START                                             (0x00000740004000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV2_1_RESV2_1_END                                               (0x00000742003fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_ROM_TABLE_1_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_1_ID                                                  (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_1_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_1_MMR_BASE                                            (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_1_NUM_REGIONS                                         (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_1_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_1_ROM_TABLE_1_1_START                                 (0x00000760000000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_ROM_TABLE_1_1_ROM_TABLE_1_1_END                                   (0x00000760000fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CSCTI_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_1_ID                                                    (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_1_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_1_MMR_BASE                                              (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_1_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_1_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_1_CSCTI1_START                                          (0x00000760001000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSCTI_CFG_1_CSCTI1_END                                            (0x00000760001fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_DRM_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_1_ID                                                      (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_1_TYPE                                                    (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_1_MMR_BASE                                                (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_1_NUM_REGIONS                                             (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_1_NUM_PRIV_IDS_PER_REGION                                 (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_1_DRM1_START                                              (0x00000760002000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_DRM_CFG_1_DRM1_END                                                (0x00000760002fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_RESV3_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_1_ID                                                        (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_1_TYPE                                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_1_MMR_BASE                                                  (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_1_NUM_REGIONS                                               (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_1_NUM_PRIV_IDS_PER_REGION                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_1_RESV3_1_START                                             (0x00000760003000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV3_1_RESV3_1_END                                               (0x00000760003fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CSTPIU_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_1_ID                                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_1_TYPE                                                 (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_1_MMR_BASE                                             (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_1_NUM_REGIONS                                          (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_1_NUM_PRIV_IDS_PER_REGION                              (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_1_CSTPIU1_START                                        (0x00000760004000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CSTPIU_CFG_1_CSTPIU1_END                                          (0x00000760004fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_CTF_CFG_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_1_ID                                                      (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_1_TYPE                                                    (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_1_MMR_BASE                                                (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_1_NUM_REGIONS                                             (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_1_NUM_PRIV_IDS_PER_REGION                                 (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_1_CTF1_START                                              (0x00000760005000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_CTF_CFG_1_CTF1_END                                                (0x00000760005fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_RESV4_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_1_ID                                                        (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_1_TYPE                                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_1_MMR_BASE                                                  (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_1_NUM_REGIONS                                               (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_1_NUM_PRIV_IDS_PER_REGION                                   (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_1_RESV4_1_START                                             (0x00000760006000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_RESV4_1_RESV4_1_END                                               (0x00000761005fffU)

/* Properties of firewall at slave: DEBUGSS_WRAP0_EXT_APB_1 */
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_1_ID                                                      (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_1_TYPE                                                    (CSL_FW_SECURITY)
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_1_MMR_BASE                                                (0x00000045000c00U)
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_1_NUM_REGIONS                                             (4U)
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_1_NUM_PRIV_IDS_PER_REGION                                 (3U)
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_1_EXT_APB1_START                                          (0x00000770000000U)
#define CSL_STD_FW_DEBUGSS_WRAP0_EXT_APB_1_EXT_APB1_END                                            (0x0000077fffffffU)

/* Properties of firewall at slave: C7X256V0_CORE0_C7XV_UMC_CFG */
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_CFG_ID                                                  (4U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_CFG_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_CFG_MMR_BASE                                            (0x00000045001000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_CFG_NUM_REGIONS                                         (8U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_CFG_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_CFG_UMC_START                                           (0x0000007c000000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_CFG_UMC_END                                             (0x0000007c01ffffU)

/* Properties of firewall at slave: C7X256V0_CLEC_C7XV_CLEC_CFG */
#define CSL_STD_FW_C7X256V0_CLEC_C7XV_CLEC_CFG_ID                                                  (4U)
#define CSL_STD_FW_C7X256V0_CLEC_C7XV_CLEC_CFG_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V0_CLEC_C7XV_CLEC_CFG_MMR_BASE                                            (0x00000045001000U)
#define CSL_STD_FW_C7X256V0_CLEC_C7XV_CLEC_CFG_NUM_REGIONS                                         (8U)
#define CSL_STD_FW_C7X256V0_CLEC_C7XV_CLEC_CFG_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_C7X256V0_CLEC_C7XV_CLEC_CFG_CLEC_START                                          (0x0000007c200000U)
#define CSL_STD_FW_C7X256V0_CLEC_C7XV_CLEC_CFG_CLEC_END                                            (0x0000007c2fffffU)

/* Properties of firewall at slave: C7X256V0_CORE0_C7XV_DRU_CFG_DRU */
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_ID                                              (4U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_TYPE                                            (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_MMR_BASE                                        (0x00000045001000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_NUM_REGIONS                                     (8U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_NUM_PRIV_IDS_PER_REGION                         (3U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_DRU_START                                       (0x0000007c400000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_DRU_END                                         (0x0000007c403fffU)

/* Properties of firewall at slave: C7X256V0_CORE0_C7XV_DRU_CFG_DRU_SET */
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_SET_ID                                          (4U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_SET_TYPE                                        (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_SET_MMR_BASE                                    (0x00000045001000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_SET_NUM_REGIONS                                 (8U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_SET_NUM_PRIV_IDS_PER_REGION                     (3U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_SET_DRU_SET_START                               (0x0000007c404000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_SET_DRU_SET_END                                 (0x0000007c407fffU)

/* Properties of firewall at slave: C7X256V0_CORE0_C7XV_DRU_CFG_DRU_QUEUE */
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_QUEUE_ID                                        (4U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_QUEUE_TYPE                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_QUEUE_MMR_BASE                                  (0x00000045001000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_QUEUE_NUM_REGIONS                               (8U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_QUEUE_NUM_PRIV_IDS_PER_REGION                   (3U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_QUEUE_DRU_QUEUE_START                           (0x0000007c408000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_QUEUE_DRU_QUEUE_END                             (0x0000007c40ffffU)

/* Properties of firewall at slave: C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHNRT */
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHNRT_ID                                        (4U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHNRT_TYPE                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHNRT_MMR_BASE                                  (0x00000045001000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHNRT_NUM_REGIONS                               (8U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHNRT_NUM_PRIV_IDS_PER_REGION                   (3U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHNRT_DRU_CHNRT_START                           (0x0000007c440000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHNRT_DRU_CHNRT_END                             (0x0000007c45ffffU)

/* Properties of firewall at slave: C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHRT */
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHRT_ID                                         (4U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHRT_TYPE                                       (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHRT_MMR_BASE                                   (0x00000045001000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHRT_NUM_REGIONS                                (8U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHRT_NUM_PRIV_IDS_PER_REGION                    (3U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHRT_DRU_CHRT_START                             (0x0000007c460000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHRT_DRU_CHRT_END                               (0x0000007c47ffffU)

/* Properties of firewall at slave: C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG */
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_ID                               (4U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_TYPE                             (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_MMR_BASE                         (0x00000045001000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_NUM_REGIONS                      (8U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_NUM_PRIV_IDS_PER_REGION          (3U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_DRU_CHATOMIC_DEBUG_START         (0x0000007c480000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_DRU_CHATOMIC_DEBUG_END           (0x0000007c49ffffU)

/* Properties of firewall at slave: C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHCORE */
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHCORE_ID                                       (4U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHCORE_TYPE                                     (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHCORE_MMR_BASE                                 (0x00000045001000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHCORE_NUM_REGIONS                              (8U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHCORE_NUM_PRIV_IDS_PER_REGION                  (3U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHCORE_DRU_CHCORE_START                         (0x0000007c4a0000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CHCORE_DRU_CHCORE_END                           (0x0000007c4bffffU)

/* Properties of firewall at slave: C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CAUSE */
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CAUSE_ID                                        (4U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CAUSE_TYPE                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CAUSE_MMR_BASE                                  (0x00000045001000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CAUSE_NUM_REGIONS                               (8U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CAUSE_NUM_PRIV_IDS_PER_REGION                   (3U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CAUSE_DRU_CAUSE_START                           (0x0000007c4e0000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_DRU_CFG_DRU_CAUSE_DRU_CAUSE_END                             (0x0000007c4fffffU)

/* Properties of firewall at slave: C7X256V0_CORE0_C7XV_UMC_MEM_MAIN */
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_MAIN_ID                                             (4U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_MAIN_TYPE                                           (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_MAIN_MMR_BASE                                       (0x00000045001000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_MAIN_NUM_REGIONS                                    (8U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_MAIN_NUM_PRIV_IDS_PER_REGION                        (3U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_MAIN_UMC_MEM_MAIN_START                             (0x0000007e000000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_MAIN_UMC_MEM_MAIN_END                               (0x0000007e1fffffU)

/* Properties of firewall at slave: C7X256V0_CORE0_C7XV_UMC_MEM_AUX */
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_AUX_ID                                              (4U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_AUX_TYPE                                            (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_AUX_MMR_BASE                                        (0x00000045001000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_AUX_NUM_REGIONS                                     (8U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_AUX_NUM_PRIV_IDS_PER_REGION                         (3U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_AUX_UMC_MEM_AUX_START                               (0x0000007f000000U)
#define CSL_STD_FW_C7X256V0_CORE0_C7XV_UMC_MEM_AUX_UMC_MEM_AUX_END                                 (0x0000007f07ffffU)

/* Properties of firewall at slave: VPAC0_MEM_SLV_DATA */
#define CSL_STD_FW_VPAC0_MEM_SLV_DATA_ID                                                           (5U)
#define CSL_STD_FW_VPAC0_MEM_SLV_DATA_TYPE                                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_MEM_SLV_DATA_MMR_BASE                                                     (0x00000045001400U)
#define CSL_STD_FW_VPAC0_MEM_SLV_DATA_NUM_REGIONS                                                  (8U)
#define CSL_STD_FW_VPAC0_MEM_SLV_DATA_NUM_PRIV_IDS_PER_REGION                                      (3U)
#define CSL_STD_FW_VPAC0_MEM_SLV_DATA_MEM_SLV_DATA_START                                           (0x00000071000000U)
#define CSL_STD_FW_VPAC0_MEM_SLV_DATA_MEM_SLV_DATA_END                                             (0x0000007107ffffU)

/* Properties of firewall at slave: GPMC0_CFG */
#define CSL_STD_FW_GPMC0_CFG_ID                                                                    (6U)
#define CSL_STD_FW_GPMC0_CFG_TYPE                                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_GPMC0_CFG_MMR_BASE                                                              (0x00000045001800U)
#define CSL_STD_FW_GPMC0_CFG_NUM_REGIONS                                                           (8U)
#define CSL_STD_FW_GPMC0_CFG_NUM_PRIV_IDS_PER_REGION                                               (3U)
#define CSL_STD_FW_GPMC0_CFG_CFG_START                                                             (0x0000003b000000U)
#define CSL_STD_FW_GPMC0_CFG_CFG_END                                                               (0x0000003b0003ffU)

/* Properties of firewall at slave: GPMC0_DATA */
#define CSL_STD_FW_GPMC0_DATA_ID                                                                   (6U)
#define CSL_STD_FW_GPMC0_DATA_TYPE                                                                 (CSL_FW_SECURITY)
#define CSL_STD_FW_GPMC0_DATA_MMR_BASE                                                             (0x00000045001800U)
#define CSL_STD_FW_GPMC0_DATA_NUM_REGIONS                                                          (8U)
#define CSL_STD_FW_GPMC0_DATA_NUM_PRIV_IDS_PER_REGION                                              (3U)
#define CSL_STD_FW_GPMC0_DATA_DATA_START                                                           (0x00000050000000U)
#define CSL_STD_FW_GPMC0_DATA_DATA_END                                                             (0x00000057ffffffU)

/* Properties of firewall at slave: FSS0_FSAS_0_DAT_REG1 */
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG1_ID                                                         (7U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG1_TYPE                                                       (CSL_FW_SECURITY)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG1_MMR_BASE                                                   (0x00000045001c00U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG1_NUM_REGIONS                                                (8U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG1_NUM_PRIV_IDS_PER_REGION                                    (3U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG1_DAT_REG1_START                                             (0x00000060000000U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG1_DAT_REG1_END                                               (0x00000067ffffffU)

/* Properties of firewall at slave: FSS0_FSAS_0_DAT_REG0 */
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG0_ID                                                         (7U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG0_TYPE                                                       (CSL_FW_SECURITY)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG0_MMR_BASE                                                   (0x00000045001c00U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG0_NUM_REGIONS                                                (8U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG0_NUM_PRIV_IDS_PER_REGION                                    (3U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG0_DAT_REG0_START                                             (0x00000400000000U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG0_DAT_REG0_END                                               (0x000004ffffffffU)

/* Properties of firewall at slave: FSS0_FSAS_0_DAT_REG3 */
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG3_ID                                                         (7U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG3_TYPE                                                       (CSL_FW_SECURITY)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG3_MMR_BASE                                                   (0x00000045001c00U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG3_NUM_REGIONS                                                (8U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG3_NUM_PRIV_IDS_PER_REGION                                    (3U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG3_DAT_REG3_START                                             (0x00000500000000U)
#define CSL_STD_FW_FSS0_FSAS_0_DAT_REG3_DAT_REG3_END                                               (0x000005ffffffffU)

/* Properties of firewall at slave: DMASS0_ECC_AGGR_0_ECCAGGR_REGS */
#define CSL_STD_FW_DMASS0_ECC_AGGR_0_ECCAGGR_REGS_ID                                               (9U)
#define CSL_STD_FW_DMASS0_ECC_AGGR_0_ECCAGGR_REGS_TYPE                                             (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_ECC_AGGR_0_ECCAGGR_REGS_MMR_BASE                                         (0x00000045002400U)
#define CSL_STD_FW_DMASS0_ECC_AGGR_0_ECCAGGR_REGS_NUM_REGIONS                                      (32U)
#define CSL_STD_FW_DMASS0_ECC_AGGR_0_ECCAGGR_REGS_NUM_PRIV_IDS_PER_REGION                          (3U)
#define CSL_STD_FW_DMASS0_ECC_AGGR_0_ECCAGGR_REGS_ECCAGGR_START                                    (0x0000003f005000U)
#define CSL_STD_FW_DMASS0_ECC_AGGR_0_ECCAGGR_REGS_ECCAGGR_END                                      (0x0000003f0053ffU)

/* Properties of firewall at slave: DMASS0_INTAGGR_0_INTAGGR_INTR */
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_INTR_ID                                                (9U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_INTR_TYPE                                              (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_INTR_MMR_BASE                                          (0x00000045002400U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_INTR_NUM_REGIONS                                       (32U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_INTR_NUM_PRIV_IDS_PER_REGION                           (3U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_INTR_INTAGGR_INTR_START                                (0x00000048000000U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_INTR_INTAGGR_INTR_END                                  (0x000000480fffffU)

/* Properties of firewall at slave: DMASS0_INTAGGR_0_INTAGGR_IMAP */
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_IMAP_ID                                                (9U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_IMAP_TYPE                                              (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_IMAP_MMR_BASE                                          (0x00000045002400U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_IMAP_NUM_REGIONS                                       (32U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_IMAP_NUM_PRIV_IDS_PER_REGION                           (3U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_IMAP_INTAGGR_IMAP_START                                (0x00000048100000U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_IMAP_INTAGGR_IMAP_END                                  (0x00000048103fffU)

/* Properties of firewall at slave: DMASS0_INTAGGR_0_INTAGGR_CFG */
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_CFG_ID                                                 (9U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_CFG_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_CFG_MMR_BASE                                           (0x00000045002400U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_CFG_NUM_REGIONS                                        (32U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_CFG_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_CFG_INTAGGR_CFG_START                                  (0x00000048110000U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_CFG_INTAGGR_CFG_END                                    (0x0000004811001fU)

/* Properties of firewall at slave: DMASS0_INTAGGR_0_INTAGGR_L2G */
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_L2G_ID                                                 (9U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_L2G_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_L2G_MMR_BASE                                           (0x00000045002400U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_L2G_NUM_REGIONS                                        (32U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_L2G_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_L2G_INTAGGR_L2G_START                                  (0x00000048120000U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_L2G_INTAGGR_L2G_END                                    (0x000000481203ffU)

/* Properties of firewall at slave: DMASS0_PSILCFG_0_PSILCFG_PROXY */
#define CSL_STD_FW_DMASS0_PSILCFG_0_PSILCFG_PROXY_ID                                               (9U)
#define CSL_STD_FW_DMASS0_PSILCFG_0_PSILCFG_PROXY_TYPE                                             (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_PSILCFG_0_PSILCFG_PROXY_MMR_BASE                                         (0x00000045002400U)
#define CSL_STD_FW_DMASS0_PSILCFG_0_PSILCFG_PROXY_NUM_REGIONS                                      (32U)
#define CSL_STD_FW_DMASS0_PSILCFG_0_PSILCFG_PROXY_NUM_PRIV_IDS_PER_REGION                          (3U)
#define CSL_STD_FW_DMASS0_PSILCFG_0_PSILCFG_PROXY_PSILCFG_PROXY_START                              (0x00000048130000U)
#define CSL_STD_FW_DMASS0_PSILCFG_0_PSILCFG_PROXY_PSILCFG_PROXY_END                                (0x000000481301ffU)

/* Properties of firewall at slave: DMASS0_PSILSS_0_PSILSS_MMRS */
#define CSL_STD_FW_DMASS0_PSILSS_0_PSILSS_MMRS_ID                                                  (9U)
#define CSL_STD_FW_DMASS0_PSILSS_0_PSILSS_MMRS_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_PSILSS_0_PSILSS_MMRS_MMR_BASE                                            (0x00000045002400U)
#define CSL_STD_FW_DMASS0_PSILSS_0_PSILSS_MMRS_NUM_REGIONS                                         (32U)
#define CSL_STD_FW_DMASS0_PSILSS_0_PSILSS_MMRS_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DMASS0_PSILSS_0_PSILSS_MMRS_PSILSS_MMRS_START                                   (0x00000048140000U)
#define CSL_STD_FW_DMASS0_PSILSS_0_PSILSS_MMRS_PSILSS_MMRS_END                                     (0x00000048140fffU)

/* Properties of firewall at slave: DMASS0_INTAGGR_0_INTAGGR_UNMAP */
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_UNMAP_ID                                               (9U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_UNMAP_TYPE                                             (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_UNMAP_MMR_BASE                                         (0x00000045002400U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_UNMAP_NUM_REGIONS                                      (32U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_UNMAP_NUM_PRIV_IDS_PER_REGION                          (3U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_UNMAP_INTAGGR_UNMAP_START                              (0x00000048180000U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_UNMAP_INTAGGR_UNMAP_END                                (0x0000004819ffffU)

/* Properties of firewall at slave: DMASS0_INTAGGR_0_INTAGGR_MCAST */
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_MCAST_ID                                               (9U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_MCAST_TYPE                                             (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_MCAST_MMR_BASE                                         (0x00000045002400U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_MCAST_NUM_REGIONS                                      (32U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_MCAST_NUM_PRIV_IDS_PER_REGION                          (3U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_MCAST_INTAGGR_MCAST_START                              (0x00000048210000U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_MCAST_INTAGGR_MCAST_END                                (0x00000048210fffU)

/* Properties of firewall at slave: DMASS0_INTAGGR_0_INTAGGR_GCNTCFG */
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTCFG_ID                                             (9U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTCFG_TYPE                                           (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTCFG_MMR_BASE                                       (0x00000045002400U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTCFG_NUM_REGIONS                                    (32U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTCFG_NUM_PRIV_IDS_PER_REGION                        (3U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTCFG_INTAGGR_GCNTCFG_START                          (0x00000048220000U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTCFG_INTAGGR_GCNTCFG_END                            (0x00000048221fffU)

/* Properties of firewall at slave: DMASS0_ETLSW_MMRS */
#define CSL_STD_FW_DMASS0_ETLSW_MMRS_ID                                                            (9U)
#define CSL_STD_FW_DMASS0_ETLSW_MMRS_TYPE                                                          (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_ETLSW_MMRS_MMR_BASE                                                      (0x00000045002400U)
#define CSL_STD_FW_DMASS0_ETLSW_MMRS_NUM_REGIONS                                                   (32U)
#define CSL_STD_FW_DMASS0_ETLSW_MMRS_NUM_PRIV_IDS_PER_REGION                                       (3U)
#define CSL_STD_FW_DMASS0_ETLSW_MMRS_ETLSW_MMRS_START                                              (0x00000048230000U)
#define CSL_STD_FW_DMASS0_ETLSW_MMRS_ETLSW_MMRS_END                                                (0x00000048230fffU)

/* Properties of firewall at slave: DMASS0_RINGACC_0_RINGACC_GCFG */
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_GCFG_ID                                                (9U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_GCFG_TYPE                                              (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_GCFG_MMR_BASE                                          (0x00000045002400U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_GCFG_NUM_REGIONS                                       (32U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_GCFG_NUM_PRIV_IDS_PER_REGION                           (3U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_GCFG_RINGACC_GCFG_START                                (0x00000048240000U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_GCFG_RINGACC_GCFG_END                                  (0x000000482403ffU)

/* Properties of firewall at slave: DMASS0_SEC_PROXY_0_SEC_PROXY_MMRS */
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_MMRS_ID                                            (9U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_MMRS_TYPE                                          (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_MMRS_MMR_BASE                                      (0x00000045002400U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_MMRS_NUM_REGIONS                                   (32U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_MMRS_NUM_PRIV_IDS_PER_REGION                       (3U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_MMRS_SEC_PROXY_MMRS_START                          (0x00000048250000U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_MMRS_SEC_PROXY_MMRS_END                            (0x000000482500ffU)

/* Properties of firewall at slave: DMASS0_BCDMA_0_BCDMA_BCHAN */
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHAN_ID                                                   (9U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHAN_TYPE                                                 (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHAN_MMR_BASE                                             (0x00000045002400U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHAN_NUM_REGIONS                                          (32U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHAN_NUM_PRIV_IDS_PER_REGION                              (3U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHAN_BCDMA_BCHAN_START                                    (0x00000048420000U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHAN_BCDMA_BCHAN_END                                      (0x00000048421fffU)

/* Properties of firewall at slave: DMASS0_PKTDMA_0_PKTDMA_RFLOW */
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RFLOW_ID                                                 (9U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RFLOW_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RFLOW_MMR_BASE                                           (0x00000045002400U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RFLOW_NUM_REGIONS                                        (32U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RFLOW_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RFLOW_PKTDMA_RFLOW_START                                 (0x00000048430000U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RFLOW_PKTDMA_RFLOW_END                                   (0x00000048430fffU)

/* Properties of firewall at slave: DMASS0_PKTDMA_0_PKTDMA_TCHAN */
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHAN_ID                                                 (9U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHAN_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHAN_MMR_BASE                                           (0x00000045002400U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHAN_NUM_REGIONS                                        (32U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHAN_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHAN_PKTDMA_TCHAN_START                                 (0x000000484a0000U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHAN_PKTDMA_TCHAN_END                                   (0x000000484a1fffU)

/* Properties of firewall at slave: DMASS0_BCDMA_0_BCDMA_TCHAN */
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHAN_ID                                                   (9U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHAN_TYPE                                                 (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHAN_MMR_BASE                                             (0x00000045002400U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHAN_NUM_REGIONS                                          (32U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHAN_NUM_PRIV_IDS_PER_REGION                              (3U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHAN_BCDMA_TCHAN_START                                    (0x000000484a4000U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHAN_BCDMA_TCHAN_END                                      (0x000000484a5fffU)

/* Properties of firewall at slave: DMASS0_PKTDMA_0_PKTDMA_RCHAN */
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHAN_ID                                                 (9U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHAN_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHAN_MMR_BASE                                           (0x00000045002400U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHAN_NUM_REGIONS                                        (32U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHAN_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHAN_PKTDMA_RCHAN_START                                 (0x000000484c0000U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHAN_PKTDMA_RCHAN_END                                   (0x000000484c1fffU)

/* Properties of firewall at slave: DMASS0_BCDMA_0_BCDMA_RCHAN */
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHAN_ID                                                   (9U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHAN_TYPE                                                 (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHAN_MMR_BASE                                             (0x00000045002400U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHAN_NUM_REGIONS                                          (32U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHAN_NUM_PRIV_IDS_PER_REGION                              (3U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHAN_BCDMA_RCHAN_START                                    (0x000000484c2000U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHAN_BCDMA_RCHAN_END                                      (0x000000484c3fffU)

/* Properties of firewall at slave: DMASS0_PKTDMA_0_PKTDMA_GCFG */
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_GCFG_ID                                                  (9U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_GCFG_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_GCFG_MMR_BASE                                            (0x00000045002400U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_GCFG_NUM_REGIONS                                         (32U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_GCFG_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_GCFG_PKTDMA_GCFG_START                                   (0x000000485c0000U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_GCFG_PKTDMA_GCFG_END                                     (0x000000485c00ffU)

/* Properties of firewall at slave: DMASS0_BCDMA_0_BCDMA_GCFG */
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_GCFG_ID                                                    (9U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_GCFG_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_GCFG_MMR_BASE                                              (0x00000045002400U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_GCFG_NUM_REGIONS                                           (32U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_GCFG_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_GCFG_BCDMA_GCFG_START                                      (0x000000485c0100U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_GCFG_BCDMA_GCFG_END                                        (0x000000485c01ffU)

/* Properties of firewall at slave: DMASS0_PKTDMA_0_PKTDMA_RING */
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RING_ID                                                  (9U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RING_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RING_MMR_BASE                                            (0x00000045002400U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RING_NUM_REGIONS                                         (32U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RING_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RING_PKTDMA_RING_START                                   (0x000000485e0000U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RING_PKTDMA_RING_END                                     (0x000000485effffU)

/* Properties of firewall at slave: DMASS0_BCDMA_0_BCDMA_RING */
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RING_ID                                                    (9U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RING_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RING_MMR_BASE                                              (0x00000045002400U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RING_NUM_REGIONS                                           (32U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RING_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RING_BCDMA_RING_START                                      (0x00000048600000U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RING_BCDMA_RING_END                                        (0x00000048607fffU)

/* Properties of firewall at slave: DMASS0_RINGACC_0_RINGACC_RT */
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_RT_ID                                                  (9U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_RT_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_RT_MMR_BASE                                            (0x00000045002400U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_RT_NUM_REGIONS                                         (32U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_RT_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_RT_RINGACC_RT_START                                    (0x00000049000000U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_RT_RINGACC_RT_END                                      (0x000000493fffffU)

/* Properties of firewall at slave: DMASS0_RINGACC_0_RINGACC_CFG */
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_CFG_ID                                                 (9U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_CFG_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_CFG_MMR_BASE                                           (0x00000045002400U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_CFG_NUM_REGIONS                                        (32U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_CFG_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_CFG_RINGACC_CFG_START                                  (0x00000049800000U)
#define CSL_STD_FW_DMASS0_RINGACC_0_RINGACC_CFG_RINGACC_CFG_END                                    (0x0000004983ffffU)

/* Properties of firewall at slave: DMASS0_INTAGGR_0_INTAGGR_GCNTRTI */
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTRTI_ID                                             (9U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTRTI_TYPE                                           (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTRTI_MMR_BASE                                       (0x00000045002400U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTRTI_NUM_REGIONS                                    (32U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTRTI_NUM_PRIV_IDS_PER_REGION                        (3U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTRTI_INTAGGR_GCNTRTI_START                          (0x0000004a000000U)
#define CSL_STD_FW_DMASS0_INTAGGR_0_INTAGGR_GCNTRTI_INTAGGR_GCNTRTI_END                            (0x0000004a0fffffU)

/* Properties of firewall at slave: DMASS0_SEC_PROXY_0_SEC_PROXY_SCFG */
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SCFG_ID                                            (9U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SCFG_TYPE                                          (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SCFG_MMR_BASE                                      (0x00000045002400U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SCFG_NUM_REGIONS                                   (32U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SCFG_NUM_PRIV_IDS_PER_REGION                       (3U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SCFG_SEC_PROXY_SCFG_START                          (0x0000004a400000U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SCFG_SEC_PROXY_SCFG_END                            (0x0000004a47ffffU)

/* Properties of firewall at slave: DMASS0_SEC_PROXY_0_SEC_PROXY_RT */
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_RT_ID                                              (9U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_RT_TYPE                                            (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_RT_MMR_BASE                                        (0x00000045002400U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_RT_NUM_REGIONS                                     (32U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_RT_NUM_PRIV_IDS_PER_REGION                         (3U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_RT_SEC_PROXY_RT_START                              (0x0000004a600000U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_RT_SEC_PROXY_RT_END                                (0x0000004a67ffffU)

/* Properties of firewall at slave: DMASS0_PKTDMA_0_PKTDMA_RCHANRT */
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHANRT_ID                                               (9U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHANRT_TYPE                                             (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHANRT_MMR_BASE                                         (0x00000045002400U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHANRT_NUM_REGIONS                                      (32U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHANRT_NUM_PRIV_IDS_PER_REGION                          (3U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHANRT_PKTDMA_RCHANRT_START                             (0x0000004a800000U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RCHANRT_PKTDMA_RCHANRT_END                               (0x0000004a81ffffU)

/* Properties of firewall at slave: DMASS0_BCDMA_0_BCDMA_RCHANRT */
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHANRT_ID                                                 (9U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHANRT_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHANRT_MMR_BASE                                           (0x00000045002400U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHANRT_NUM_REGIONS                                        (32U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHANRT_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHANRT_BCDMA_RCHANRT_START                                (0x0000004a820000U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RCHANRT_BCDMA_RCHANRT_END                                  (0x0000004a83ffffU)

/* Properties of firewall at slave: DMASS0_PKTDMA_0_PKTDMA_TCHANRT */
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHANRT_ID                                               (9U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHANRT_TYPE                                             (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHANRT_MMR_BASE                                         (0x00000045002400U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHANRT_NUM_REGIONS                                      (32U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHANRT_NUM_PRIV_IDS_PER_REGION                          (3U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHANRT_PKTDMA_TCHANRT_START                             (0x0000004aa00000U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_TCHANRT_PKTDMA_TCHANRT_END                               (0x0000004aa1ffffU)

/* Properties of firewall at slave: DMASS0_BCDMA_0_BCDMA_TCHANRT */
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHANRT_ID                                                 (9U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHANRT_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHANRT_MMR_BASE                                           (0x00000045002400U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHANRT_NUM_REGIONS                                        (32U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHANRT_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHANRT_BCDMA_TCHANRT_START                                (0x0000004aa40000U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_TCHANRT_BCDMA_TCHANRT_END                                  (0x0000004aa5ffffU)

/* Properties of firewall at slave: DMASS0_PKTDMA_0_PKTDMA_RINGRT */
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RINGRT_ID                                                (9U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RINGRT_TYPE                                              (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RINGRT_MMR_BASE                                          (0x00000045002400U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RINGRT_NUM_REGIONS                                       (32U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RINGRT_NUM_PRIV_IDS_PER_REGION                           (3U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RINGRT_PKTDMA_RINGRT_START                               (0x0000004b800000U)
#define CSL_STD_FW_DMASS0_PKTDMA_0_PKTDMA_RINGRT_PKTDMA_RINGRT_END                                 (0x0000004b9fffffU)

/* Properties of firewall at slave: DMASS0_BCDMA_0_BCDMA_RINGRT */
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RINGRT_ID                                                  (9U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RINGRT_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RINGRT_MMR_BASE                                            (0x00000045002400U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RINGRT_NUM_REGIONS                                         (32U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RINGRT_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RINGRT_BCDMA_RINGRT_START                                  (0x0000004bc00000U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_RINGRT_BCDMA_RINGRT_END                                    (0x0000004bcfffffU)

/* Properties of firewall at slave: DMASS0_BCDMA_0_BCDMA_BCHANRT */
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHANRT_ID                                                 (9U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHANRT_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHANRT_MMR_BASE                                           (0x00000045002400U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHANRT_NUM_REGIONS                                        (32U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHANRT_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHANRT_BCDMA_BCHANRT_START                                (0x0000004c000000U)
#define CSL_STD_FW_DMASS0_BCDMA_0_BCDMA_BCHANRT_BCDMA_BCHANRT_END                                  (0x0000004c01ffffU)

/* Properties of firewall at slave: MCRC64_0_MCRC64_REGS */
#define CSL_STD_FW_MCRC64_0_MCRC64_REGS_ID                                                         (13U)
#define CSL_STD_FW_MCRC64_0_MCRC64_REGS_TYPE                                                       (CSL_FW_SECURITY)
#define CSL_STD_FW_MCRC64_0_MCRC64_REGS_MMR_BASE                                                   (0x00000045003400U)
#define CSL_STD_FW_MCRC64_0_MCRC64_REGS_NUM_REGIONS                                                (1U)
#define CSL_STD_FW_MCRC64_0_MCRC64_REGS_NUM_PRIV_IDS_PER_REGION                                    (3U)
#define CSL_STD_FW_MCRC64_0_MCRC64_REGS_REGS_START                                                 (0x00000030300000U)
#define CSL_STD_FW_MCRC64_0_MCRC64_REGS_REGS_END                                                   (0x00000030300fffU)

/* Properties of firewall at slave: MSRAM8KX256E0_RAM */
#define CSL_STD_FW_MSRAM8KX256E0_RAM_ID                                                            (15U)
#define CSL_STD_FW_MSRAM8KX256E0_RAM_TYPE                                                          (CSL_FW_SECURITY)
#define CSL_STD_FW_MSRAM8KX256E0_RAM_MMR_BASE                                                      (0x00000045003c00U)
#define CSL_STD_FW_MSRAM8KX256E0_RAM_NUM_REGIONS                                                   (16U)
#define CSL_STD_FW_MSRAM8KX256E0_RAM_NUM_PRIV_IDS_PER_REGION                                       (3U)
#define CSL_STD_FW_MSRAM8KX256E0_RAM_RAM_START                                                     (0x00000043c40000U)
#define CSL_STD_FW_MSRAM8KX256E0_RAM_RAM_END                                                       (0x00000043c7ffffU)

/* Properties of firewall at slave: C7X256V1_CORE0_C7XV_UMC_CFG */
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_CFG_ID                                                  (16U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_CFG_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_CFG_MMR_BASE                                            (0x00000045004000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_CFG_NUM_REGIONS                                         (8U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_CFG_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_CFG_UMC_START                                           (0x0000007d000000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_CFG_UMC_END                                             (0x0000007d01ffffU)

/* Properties of firewall at slave: C7X256V1_CLEC_C7XV_CLEC_CFG */
#define CSL_STD_FW_C7X256V1_CLEC_C7XV_CLEC_CFG_ID                                                  (16U)
#define CSL_STD_FW_C7X256V1_CLEC_C7XV_CLEC_CFG_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V1_CLEC_C7XV_CLEC_CFG_MMR_BASE                                            (0x00000045004000U)
#define CSL_STD_FW_C7X256V1_CLEC_C7XV_CLEC_CFG_NUM_REGIONS                                         (8U)
#define CSL_STD_FW_C7X256V1_CLEC_C7XV_CLEC_CFG_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_C7X256V1_CLEC_C7XV_CLEC_CFG_CLEC_START                                          (0x0000007d200000U)
#define CSL_STD_FW_C7X256V1_CLEC_C7XV_CLEC_CFG_CLEC_END                                            (0x0000007d2fffffU)

/* Properties of firewall at slave: C7X256V1_CORE0_C7XV_DRU_CFG_DRU */
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_ID                                              (16U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_TYPE                                            (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_MMR_BASE                                        (0x00000045004000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_NUM_REGIONS                                     (8U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_NUM_PRIV_IDS_PER_REGION                         (3U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_DRU_START                                       (0x0000007d400000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_DRU_END                                         (0x0000007d403fffU)

/* Properties of firewall at slave: C7X256V1_CORE0_C7XV_DRU_CFG_DRU_SET */
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_SET_ID                                          (16U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_SET_TYPE                                        (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_SET_MMR_BASE                                    (0x00000045004000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_SET_NUM_REGIONS                                 (8U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_SET_NUM_PRIV_IDS_PER_REGION                     (3U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_SET_DRU_SET_START                               (0x0000007d404000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_SET_DRU_SET_END                                 (0x0000007d407fffU)

/* Properties of firewall at slave: C7X256V1_CORE0_C7XV_DRU_CFG_DRU_QUEUE */
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_QUEUE_ID                                        (16U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_QUEUE_TYPE                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_QUEUE_MMR_BASE                                  (0x00000045004000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_QUEUE_NUM_REGIONS                               (8U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_QUEUE_NUM_PRIV_IDS_PER_REGION                   (3U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_QUEUE_DRU_QUEUE_START                           (0x0000007d408000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_QUEUE_DRU_QUEUE_END                             (0x0000007d40ffffU)

/* Properties of firewall at slave: C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHNRT */
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHNRT_ID                                        (16U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHNRT_TYPE                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHNRT_MMR_BASE                                  (0x00000045004000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHNRT_NUM_REGIONS                               (8U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHNRT_NUM_PRIV_IDS_PER_REGION                   (3U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHNRT_DRU_CHNRT_START                           (0x0000007d440000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHNRT_DRU_CHNRT_END                             (0x0000007d45ffffU)

/* Properties of firewall at slave: C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHRT */
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHRT_ID                                         (16U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHRT_TYPE                                       (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHRT_MMR_BASE                                   (0x00000045004000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHRT_NUM_REGIONS                                (8U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHRT_NUM_PRIV_IDS_PER_REGION                    (3U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHRT_DRU_CHRT_START                             (0x0000007d460000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHRT_DRU_CHRT_END                               (0x0000007d47ffffU)

/* Properties of firewall at slave: C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG */
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_ID                               (16U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_TYPE                             (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_MMR_BASE                         (0x00000045004000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_NUM_REGIONS                      (8U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_NUM_PRIV_IDS_PER_REGION          (3U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_DRU_CHATOMIC_DEBUG_START         (0x0000007d480000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHATOMIC_DEBUG_DRU_CHATOMIC_DEBUG_END           (0x0000007d49ffffU)

/* Properties of firewall at slave: C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHCORE */
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHCORE_ID                                       (16U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHCORE_TYPE                                     (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHCORE_MMR_BASE                                 (0x00000045004000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHCORE_NUM_REGIONS                              (8U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHCORE_NUM_PRIV_IDS_PER_REGION                  (3U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHCORE_DRU_CHCORE_START                         (0x0000007d4a0000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CHCORE_DRU_CHCORE_END                           (0x0000007d4bffffU)

/* Properties of firewall at slave: C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CAUSE */
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CAUSE_ID                                        (16U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CAUSE_TYPE                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CAUSE_MMR_BASE                                  (0x00000045004000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CAUSE_NUM_REGIONS                               (8U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CAUSE_NUM_PRIV_IDS_PER_REGION                   (3U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CAUSE_DRU_CAUSE_START                           (0x0000007d4e0000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_DRU_CFG_DRU_CAUSE_DRU_CAUSE_END                             (0x0000007d4fffffU)

/* Properties of firewall at slave: C7X256V1_CORE0_C7XV_UMC_MEM_MAIN */
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_MAIN_ID                                             (16U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_MAIN_TYPE                                           (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_MAIN_MMR_BASE                                       (0x00000045004000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_MAIN_NUM_REGIONS                                    (8U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_MAIN_NUM_PRIV_IDS_PER_REGION                        (3U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_MAIN_UMC_MEM_MAIN_START                             (0x0000007e200000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_MAIN_UMC_MEM_MAIN_END                               (0x0000007e3fffffU)

/* Properties of firewall at slave: C7X256V1_CORE0_C7XV_UMC_MEM_AUX */
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_AUX_ID                                              (16U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_AUX_TYPE                                            (CSL_FW_SECURITY)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_AUX_MMR_BASE                                        (0x00000045004000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_AUX_NUM_REGIONS                                     (8U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_AUX_NUM_PRIV_IDS_PER_REGION                         (3U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_AUX_UMC_MEM_AUX_START                               (0x0000007f800000U)
#define CSL_STD_FW_C7X256V1_CORE0_C7XV_UMC_MEM_AUX_UMC_MEM_AUX_END                                 (0x0000007f87ffffU)

/* Properties of firewall at slave: PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT0 */
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT0_ID                                            (18U)
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT0_TYPE                                          (CSL_FW_SECURITY)
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT0_MMR_BASE                                      (0x00000045004800U)
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT0_NUM_REGIONS                                   (8U)
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT0_NUM_PRIV_IDS_PER_REGION                       (3U)
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT0_DAT0_START                                    (0x00000068000000U)
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT0_DAT0_END                                      (0x0000006fffffffU)

/* Properties of firewall at slave: PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT1 */
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT1_ID                                            (18U)
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT1_TYPE                                          (CSL_FW_SECURITY)
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT1_MMR_BASE                                      (0x00000045004800U)
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT1_NUM_REGIONS                                   (8U)
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT1_NUM_PRIV_IDS_PER_REGION                       (3U)
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT1_DAT1_START                                    (0x00000600000000U)
#define CSL_STD_FW_PCIE0_CORE_CORE_DAT_SLV_PCIE_DAT1_DAT1_END                                      (0x000006ffffffffU)

/* Properties of firewall at slave: DMPAC0_DMPAC_TOP_DOF_INFRA_DMPAC_BASE_MEM_SLV_CBASS_STRIPE_MSRAM_SLV */
#define CSL_STD_FW_DMPAC0_DMPAC_TOP_DOF_INFRA_DMPAC_BASE_MEM_SLV_CBASS_STRIPE_MSRAM_SLV_ID         (22U)
#define CSL_STD_FW_DMPAC0_DMPAC_TOP_DOF_INFRA_DMPAC_BASE_MEM_SLV_CBASS_STRIPE_MSRAM_SLV_TYPE       (CSL_FW_SECURITY)
#define CSL_STD_FW_DMPAC0_DMPAC_TOP_DOF_INFRA_DMPAC_BASE_MEM_SLV_CBASS_STRIPE_MSRAM_SLV_MMR_BASE   (0x00000045005800U)
#define CSL_STD_FW_DMPAC0_DMPAC_TOP_DOF_INFRA_DMPAC_BASE_MEM_SLV_CBASS_STRIPE_MSRAM_SLV_NUM_REGIONS (8U)
#define CSL_STD_FW_DMPAC0_DMPAC_TOP_DOF_INFRA_DMPAC_BASE_MEM_SLV_CBASS_STRIPE_MSRAM_SLV_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_DMPAC0_DMPAC_TOP_DOF_INFRA_DMPAC_BASE_MEM_SLV_CBASS_STRIPE_MSRAM_SLV_DMPAC_TOP_DOF_INFRA_DMPAC_BASE_MEM_SLV_CBASS_STRIPE_MSRAM_SLV_START (0x00000010400000U)
#define CSL_STD_FW_DMPAC0_DMPAC_TOP_DOF_INFRA_DMPAC_BASE_MEM_SLV_CBASS_STRIPE_MSRAM_SLV_DMPAC_TOP_DOF_INFRA_DMPAC_BASE_MEM_SLV_CBASS_STRIPE_MSRAM_SLV_END (0x0000001047ffffU)

/* Properties of firewall at slave: WKUP_R5FSS0_CORE0_ICACHE0 */
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ICACHE0_ID                                                    (32U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ICACHE0_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ICACHE0_MMR_BASE                                              (0x00000045008000U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ICACHE0_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ICACHE0_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ICACHE0_CORE0_ICACHE_START                                    (0x00000074000000U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ICACHE0_CORE0_ICACHE_END                                      (0x000000747fffffU)

/* Properties of firewall at slave: WKUP_R5FSS0_CORE0_DCACHE0 */
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_DCACHE0_ID                                                    (32U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_DCACHE0_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_DCACHE0_MMR_BASE                                              (0x00000045008000U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_DCACHE0_NUM_REGIONS                                           (4U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_DCACHE0_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_DCACHE0_CORE0_DCACHE_START                                    (0x00000074800000U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_DCACHE0_CORE0_DCACHE_END                                      (0x00000074ffffffU)

/* Properties of firewall at slave: WKUP_R5FSS0_CORE0_ATCM0 */
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ATCM0_ID                                                      (32U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ATCM0_TYPE                                                    (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ATCM0_MMR_BASE                                                (0x00000045008000U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ATCM0_NUM_REGIONS                                             (4U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ATCM0_NUM_PRIV_IDS_PER_REGION                                 (3U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ATCM0_CORE0_ATCM_START                                        (0x00000078000000U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_ATCM0_CORE0_ATCM_END                                          (0x00000078007fffU)

/* Properties of firewall at slave: WKUP_R5FSS0_CORE0_BTCM0 */
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_BTCM0_ID                                                      (32U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_BTCM0_TYPE                                                    (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_BTCM0_MMR_BASE                                                (0x00000045008000U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_BTCM0_NUM_REGIONS                                             (4U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_BTCM0_NUM_PRIV_IDS_PER_REGION                                 (3U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_BTCM0_CORE0_BTCM_START                                        (0x00000078100000U)
#define CSL_STD_FW_WKUP_R5FSS0_CORE0_BTCM0_CORE0_BTCM_END                                          (0x00000078107fffU)

/* Properties of firewall at slave: WKUP_GTC0_GTC_CFG0 */
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG0_ID                                                           (34U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG0_TYPE                                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG0_MMR_BASE                                                     (0x00000045008800U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG0_NUM_REGIONS                                                  (16U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG0_NUM_PRIV_IDS_PER_REGION                                      (3U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG0_GTC_CFG0_START                                               (0x00000000a80000U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG0_GTC_CFG0_END                                                 (0x00000000a803ffU)

/* Properties of firewall at slave: WKUP_GTC0_GTC_CFG1 */
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG1_ID                                                           (34U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG1_TYPE                                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG1_MMR_BASE                                                     (0x00000045008800U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG1_NUM_REGIONS                                                  (16U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG1_NUM_PRIV_IDS_PER_REGION                                      (3U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG1_GTC_CFG1_START                                               (0x00000000a90000U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG1_GTC_CFG1_END                                                 (0x00000000a93fffU)

/* Properties of firewall at slave: WKUP_GTC0_GTC_CFG2 */
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG2_ID                                                           (34U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG2_TYPE                                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG2_MMR_BASE                                                     (0x00000045008800U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG2_NUM_REGIONS                                                  (16U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG2_NUM_PRIV_IDS_PER_REGION                                      (3U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG2_GTC_CFG2_START                                               (0x00000000aa0000U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG2_GTC_CFG2_END                                                 (0x00000000aa3fffU)

/* Properties of firewall at slave: WKUP_GTC0_GTC_CFG3 */
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG3_ID                                                           (34U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG3_TYPE                                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG3_MMR_BASE                                                     (0x00000045008800U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG3_NUM_REGIONS                                                  (16U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG3_NUM_PRIV_IDS_PER_REGION                                      (3U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG3_GTC_CFG3_START                                               (0x00000000ab0000U)
#define CSL_STD_FW_WKUP_GTC0_GTC_CFG3_GTC_CFG3_END                                                 (0x00000000ab3fffU)

/* Properties of firewall at slave: WKUP_VTM0_MMR_VBUSP_CFG1 */
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG1_ID                                                     (34U)
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG1_TYPE                                                   (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG1_MMR_BASE                                               (0x00000045008800U)
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG1_NUM_REGIONS                                            (16U)
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG1_NUM_PRIV_IDS_PER_REGION                                (3U)
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG1_MMR_VBUSP_CFG1_START                                   (0x00000000b00000U)
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG1_MMR_VBUSP_CFG1_END                                     (0x00000000b003ffU)

/* Properties of firewall at slave: WKUP_VTM0_MMR_VBUSP_CFG2 */
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG2_ID                                                     (34U)
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG2_TYPE                                                   (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG2_MMR_BASE                                               (0x00000045008800U)
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG2_NUM_REGIONS                                            (16U)
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG2_NUM_PRIV_IDS_PER_REGION                                (3U)
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG2_MMR_VBUSP_CFG2_START                                   (0x00000000b01000U)
#define CSL_STD_FW_WKUP_VTM0_MMR_VBUSP_CFG2_MMR_VBUSP_CFG2_END                                     (0x00000000b013ffU)

/* Properties of firewall at slave: WKUP_VTM0_ECCAGGR_CFG_REGS */
#define CSL_STD_FW_WKUP_VTM0__ECCAGGR_CFG_REGS_ID                                                  (34U)
#define CSL_STD_FW_WKUP_VTM0__ECCAGGR_CFG_REGS_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_VTM0__ECCAGGR_CFG_REGS_MMR_BASE                                            (0x00000045008800U)
#define CSL_STD_FW_WKUP_VTM0__ECCAGGR_CFG_REGS_NUM_REGIONS                                         (16U)
#define CSL_STD_FW_WKUP_VTM0__ECCAGGR_CFG_REGS_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_WKUP_VTM0__ECCAGGR_CFG_REGS_ECCAGGR_CFG_START                                   (0x00000000b02000U)
#define CSL_STD_FW_WKUP_VTM0__ECCAGGR_CFG_REGS_ECCAGGR_CFG_END                                     (0x00000000b023ffU)

/* Properties of firewall at slave: WKUP_RTI0_CFG */
#define CSL_STD_FW_WKUP_RTI0_CFG_ID                                                                (34U)
#define CSL_STD_FW_WKUP_RTI0_CFG_TYPE                                                              (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_RTI0_CFG_MMR_BASE                                                          (0x00000045008800U)
#define CSL_STD_FW_WKUP_RTI0_CFG_NUM_REGIONS                                                       (16U)
#define CSL_STD_FW_WKUP_RTI0_CFG_NUM_PRIV_IDS_PER_REGION                                           (3U)
#define CSL_STD_FW_WKUP_RTI0_CFG_CFG_START                                                         (0x0000002b000000U)
#define CSL_STD_FW_WKUP_RTI0_CFG_CFG_END                                                           (0x0000002b0000ffU)

/* Properties of firewall at slave: WKUP_RTCSS0_RTC_REGS */
#define CSL_STD_FW_WKUP_RTCSS0_RTC_REGS_ID                                                         (34U)
#define CSL_STD_FW_WKUP_RTCSS0_RTC_REGS_TYPE                                                       (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_RTCSS0_RTC_REGS_MMR_BASE                                                   (0x00000045008800U)
#define CSL_STD_FW_WKUP_RTCSS0_RTC_REGS_NUM_REGIONS                                                (16U)
#define CSL_STD_FW_WKUP_RTCSS0_RTC_REGS_NUM_PRIV_IDS_PER_REGION                                    (3U)
#define CSL_STD_FW_WKUP_RTCSS0_RTC_REGS_RTC_START                                                  (0x0000002b1f0000U)
#define CSL_STD_FW_WKUP_RTCSS0_RTC_REGS_RTC_END                                                    (0x0000002b1f007fU)

/* Properties of firewall at slave: WKUP_I2C0_CFG */
#define CSL_STD_FW_WKUP_I2C0_CFG_ID                                                                (34U)
#define CSL_STD_FW_WKUP_I2C0_CFG_TYPE                                                              (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_I2C0_CFG_MMR_BASE                                                          (0x00000045008800U)
#define CSL_STD_FW_WKUP_I2C0_CFG_NUM_REGIONS                                                       (16U)
#define CSL_STD_FW_WKUP_I2C0_CFG_NUM_PRIV_IDS_PER_REGION                                           (3U)
#define CSL_STD_FW_WKUP_I2C0_CFG_CFG_START                                                         (0x0000002b200000U)
#define CSL_STD_FW_WKUP_I2C0_CFG_CFG_END                                                           (0x0000002b2000ffU)

/* Properties of firewall at slave: WKUP_UART0_MEM */
#define CSL_STD_FW_WKUP_UART0_MEM_ID                                                               (34U)
#define CSL_STD_FW_WKUP_UART0_MEM_TYPE                                                             (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_UART0_MEM_MMR_BASE                                                         (0x00000045008800U)
#define CSL_STD_FW_WKUP_UART0_MEM_NUM_REGIONS                                                      (16U)
#define CSL_STD_FW_WKUP_UART0_MEM_NUM_PRIV_IDS_PER_REGION                                          (3U)
#define CSL_STD_FW_WKUP_UART0_MEM_START                                                            (0x0000002b300000U)
#define CSL_STD_FW_WKUP_UART0_MEM_END                                                              (0x0000002b3001ffU)

/* Properties of firewall at slave: WKUP_CBASS0_ERR_REGS */
#define CSL_STD_FW_WKUP_CBASS0_ERR_REGS_ID                                                         (34U)
#define CSL_STD_FW_WKUP_CBASS0_ERR_REGS_TYPE                                                       (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_CBASS0_ERR_REGS_MMR_BASE                                                   (0x00000045008800U)
#define CSL_STD_FW_WKUP_CBASS0_ERR_REGS_NUM_REGIONS                                                (16U)
#define CSL_STD_FW_WKUP_CBASS0_ERR_REGS_NUM_PRIV_IDS_PER_REGION                                    (3U)
#define CSL_STD_FW_WKUP_CBASS0_ERR_REGS_ERR_START                                                  (0x0000002b400000U)
#define CSL_STD_FW_WKUP_CBASS0_ERR_REGS_ERR_END                                                    (0x0000002b4003ffU)

/* Properties of firewall at slave: WKUP_PBIST0_MEM */
#define CSL_STD_FW_WKUP_PBIST0_MEM_ID                                                              (34U)
#define CSL_STD_FW_WKUP_PBIST0_MEM_TYPE                                                            (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_PBIST0_MEM_MMR_BASE                                                        (0x00000045008800U)
#define CSL_STD_FW_WKUP_PBIST0_MEM_NUM_REGIONS                                                     (16U)
#define CSL_STD_FW_WKUP_PBIST0_MEM_NUM_PRIV_IDS_PER_REGION                                         (3U)
#define CSL_STD_FW_WKUP_PBIST0_MEM_START                                                           (0x0000002b500000U)
#define CSL_STD_FW_WKUP_PBIST0_MEM_END                                                             (0x0000002b5003ffU)

/* Properties of firewall at slave: WKUP_PBIST1_MEM */
#define CSL_STD_FW_WKUP_PBIST1_MEM_ID                                                              (34U)
#define CSL_STD_FW_WKUP_PBIST1_MEM_TYPE                                                            (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_PBIST1_MEM_MMR_BASE                                                        (0x00000045008800U)
#define CSL_STD_FW_WKUP_PBIST1_MEM_NUM_REGIONS                                                     (16U)
#define CSL_STD_FW_WKUP_PBIST1_MEM_NUM_PRIV_IDS_PER_REGION                                         (3U)
#define CSL_STD_FW_WKUP_PBIST1_MEM_START                                                           (0x0000002b510000U)
#define CSL_STD_FW_WKUP_PBIST1_MEM_END                                                             (0x0000002b5103ffU)

/* Properties of firewall at slave: WKUP_ECC_AGGR0_REGS */
#define CSL_STD_FW_WKUP_ECC_AGGR0_REGS_ID                                                          (34U)
#define CSL_STD_FW_WKUP_ECC_AGGR0_REGS_TYPE                                                        (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_ECC_AGGR0_REGS_MMR_BASE                                                    (0x00000045008800U)
#define CSL_STD_FW_WKUP_ECC_AGGR0_REGS_NUM_REGIONS                                                 (16U)
#define CSL_STD_FW_WKUP_ECC_AGGR0_REGS_NUM_PRIV_IDS_PER_REGION                                     (3U)
#define CSL_STD_FW_WKUP_ECC_AGGR0_REGS_ECC_AGGR_START                                              (0x0000002b600000U)
#define CSL_STD_FW_WKUP_ECC_AGGR0_REGS_ECC_AGGR_END                                                (0x0000002b6003ffU)

/* Properties of firewall at slave: WKUP_ECC_AGGR1_REGS */
#define CSL_STD_FW_WKUP_ECC_AGGR1_REGS_ID                                                          (34U)
#define CSL_STD_FW_WKUP_ECC_AGGR1_REGS_TYPE                                                        (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_ECC_AGGR1_REGS_MMR_BASE                                                    (0x00000045008800U)
#define CSL_STD_FW_WKUP_ECC_AGGR1_REGS_NUM_REGIONS                                                 (16U)
#define CSL_STD_FW_WKUP_ECC_AGGR1_REGS_NUM_PRIV_IDS_PER_REGION                                     (3U)
#define CSL_STD_FW_WKUP_ECC_AGGR1_REGS_ECC_AGGR_START                                              (0x0000002b601000U)
#define CSL_STD_FW_WKUP_ECC_AGGR1_REGS_ECC_AGGR_END                                                (0x0000002b6013ffU)

/* Properties of firewall at slave: WKUP_PSRAMECC_8K0_REGS */
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_REGS_ID                                                       (34U)
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_REGS_TYPE                                                     (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_REGS_MMR_BASE                                                 (0x00000045008800U)
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_REGS_NUM_REGIONS                                              (16U)
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_REGS_NUM_PRIV_IDS_PER_REGION                                  (3U)
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_REGS_REGS_START                                               (0x0000002b608000U)
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_REGS_REGS_END                                                 (0x0000002b6083ffU)

/* Properties of firewall at slave: WKUP_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS */
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_ID                                      (34U)
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_TYPE                                    (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_MMR_BASE                                (0x00000045008800U)
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_NUM_REGIONS                             (16U)
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_NUM_PRIV_IDS_PER_REGION                 (3U)
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_EVNT_BUS_VBUSP_MMRS_START               (0x0000003c018000U)
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_EVNT_BUS_VBUSP_MMRS_END                 (0x0000003c0180ffU)

/* Properties of firewall at slave: R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS */
#define CSL_STD_FW_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_ID                                           (34U)
#define CSL_STD_FW_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_TYPE                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_MMR_BASE                                     (0x00000045008800U)
#define CSL_STD_FW_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_NUM_REGIONS                                  (16U)
#define CSL_STD_FW_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_NUM_PRIV_IDS_PER_REGION                      (3U)
#define CSL_STD_FW_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_EVNT_BUS_VBUSP_MMRS_START                    (0x0000003c038000U)
#define CSL_STD_FW_R5FSS0_COMMON0_EVNT_BUS_VBUSP_MMRS_EVNT_BUS_VBUSP_MMRS_END                      (0x0000003c0380ffU)

/* Properties of firewall at slave: R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS */
#define CSL_STD_FW_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_ID                                        (34U)
#define CSL_STD_FW_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_TYPE                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_MMR_BASE                                  (0x00000045008800U)
#define CSL_STD_FW_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_NUM_REGIONS                               (16U)
#define CSL_STD_FW_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_NUM_PRIV_IDS_PER_REGION                   (3U)
#define CSL_STD_FW_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_CORE0_ECC_AGGR_START                      (0x0000003f000000U)
#define CSL_STD_FW_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_CORE0_ECC_AGGR_END                        (0x0000003f0003ffU)

/* Properties of firewall at slave: WKUP_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS */
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_ID                                   (34U)
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_TYPE                                 (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_MMR_BASE                             (0x00000045008800U)
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_NUM_REGIONS                          (16U)
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_NUM_PRIV_IDS_PER_REGION              (3U)
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_CORE0_ECC_AGGR_START                 (0x0000003f00d000U)
#define CSL_STD_FW_WKUP_R5FSS0_COMMON0_CPU0_ECC_AGGR_CFG_REGS_CORE0_ECC_AGGR_END                   (0x0000003f00d3ffU)

/* Properties of firewall at slave: WKUP_PSRAMECC_8K0_RAM */
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_RAM_ID                                                        (34U)
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_RAM_TYPE                                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_RAM_MMR_BASE                                                  (0x00000045008800U)
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_RAM_NUM_REGIONS                                               (16U)
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_RAM_NUM_PRIV_IDS_PER_REGION                                   (3U)
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_RAM_RAM_START                                                 (0x00000041880000U)
#define CSL_STD_FW_WKUP_PSRAMECC_8K0_RAM_RAM_END                                                   (0x00000041887fffU)

/* Properties of firewall at slave: WKUP_CTRL_MMR0_CFG0 */
#define CSL_STD_FW_WKUP_CTRL_MMR0_CFG0_ID                                                          (34U)
#define CSL_STD_FW_WKUP_CTRL_MMR0_CFG0_TYPE                                                        (CSL_FW_SECURITY)
#define CSL_STD_FW_WKUP_CTRL_MMR0_CFG0_MMR_BASE                                                    (0x00000045008800U)
#define CSL_STD_FW_WKUP_CTRL_MMR0_CFG0_NUM_REGIONS                                                 (16U)
#define CSL_STD_FW_WKUP_CTRL_MMR0_CFG0_NUM_PRIV_IDS_PER_REGION                                     (3U)
#define CSL_STD_FW_WKUP_CTRL_MMR0_CFG0_CFG0_START                                                  (0x00000043000000U)
#define CSL_STD_FW_WKUP_CTRL_MMR0_CFG0_CFG0_END                                                    (0x0000004301ffffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_KSDW_ECC_AGGR_CFG_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_KSDW_ECC_AGGR_CFG_REGS_ID                             (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_KSDW_ECC_AGGR_CFG_REGS_TYPE                           (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_KSDW_ECC_AGGR_CFG_REGS_MMR_BASE                       (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_KSDW_ECC_AGGR_CFG_REGS_NUM_REGIONS                    (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_KSDW_ECC_AGGR_CFG_REGS_NUM_PRIV_IDS_PER_REGION        (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_KSDW_ECC_AGGR_CFG_REGS_IVPAC_TOP_0_CFG_SLV_KSDW_ECC_AGGR_CFG_START (0x0000002b604000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_KSDW_ECC_AGGR_CFG_REGS_IVPAC_TOP_0_CFG_SLV_KSDW_ECC_AGGR_CFG_END (0x0000002b6043ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_ID      (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_TYPE    (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_KSDW_ECC_AGGR_CFG_START (0x0000002b605000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_KSDW_ECC_AGGR_CFG_END (0x0000002b6053ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_ID       (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_TYPE     (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_KSDW_ECC_AGGR_CFG_START (0x0000002b607000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_KSDW_ECC_AGGR_CFG_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_KSDW_ECC_AGGR_CFG_END (0x0000002b6073ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_VPAC_REGS_VPAC_REGS_CFG_IP_MMRS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_VPAC_REGS_VPAC_REGS_CFG_IP_MMRS_ID                    (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_VPAC_REGS_VPAC_REGS_CFG_IP_MMRS_TYPE                  (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_VPAC_REGS_VPAC_REGS_CFG_IP_MMRS_MMR_BASE              (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_VPAC_REGS_VPAC_REGS_CFG_IP_MMRS_NUM_REGIONS           (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_VPAC_REGS_VPAC_REGS_CFG_IP_MMRS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_VPAC_REGS_VPAC_REGS_CFG_IP_MMRS_IVPAC_TOP_0_CFG_SLV_VPAC_REGS_VPAC_REGS_CFG_IP_MMRS_START (0x0000002c000000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_VPAC_REGS_VPAC_REGS_CFG_IP_MMRS_IVPAC_TOP_0_CFG_SLV_VPAC_REGS_VPAC_REGS_CFG_IP_MMRS_END (0x0000002c0003ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_CTSET2_WRAP_CFG_CTSET2_CFG */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CTSET2_WRAP_CFG_CTSET2_CFG_ID                         (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CTSET2_WRAP_CFG_CTSET2_CFG_TYPE                       (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CTSET2_WRAP_CFG_CTSET2_CFG_MMR_BASE                   (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CTSET2_WRAP_CFG_CTSET2_CFG_NUM_REGIONS                (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CTSET2_WRAP_CFG_CTSET2_CFG_NUM_PRIV_IDS_PER_REGION    (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CTSET2_WRAP_CFG_CTSET2_CFG_IVPAC_TOP_0_CFG_SLV_CTSET2_WRAP_CFG_CTSET2_CFG_START (0x0000002c002000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CTSET2_WRAP_CFG_CTSET2_CFG_IVPAC_TOP_0_CFG_SLV_CTSET2_WRAP_CFG_CTSET2_CFG_END (0x0000002c003fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_CP_INTD_INTD_CFG_INTD_CFG */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CP_INTD_INTD_CFG_INTD_CFG_ID                          (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CP_INTD_INTD_CFG_INTD_CFG_TYPE                        (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CP_INTD_INTD_CFG_INTD_CFG_MMR_BASE                    (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CP_INTD_INTD_CFG_INTD_CFG_NUM_REGIONS                 (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CP_INTD_INTD_CFG_INTD_CFG_NUM_PRIV_IDS_PER_REGION     (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CP_INTD_INTD_CFG_INTD_CFG_IVPAC_TOP_0_CFG_SLV_CP_INTD_CFG_INTD_CFG_START (0x0000002c004000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_CP_INTD_INTD_CFG_INTD_CFG_IVPAC_TOP_0_CFG_SLV_CP_INTD_CFG_INTD_CFG_END (0x0000002c004fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_HTS_S_VBUSP_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_HTS_S_VBUSP_REGS_ID                                   (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_HTS_S_VBUSP_REGS_TYPE                                 (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_HTS_S_VBUSP_REGS_MMR_BASE                             (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_HTS_S_VBUSP_REGS_NUM_REGIONS                          (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_HTS_S_VBUSP_REGS_NUM_PRIV_IDS_PER_REGION              (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_HTS_S_VBUSP_REGS_IVPAC_TOP_0_CFG_SLV_HTS_S_VBUSP_START (0x0000002c010000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_HTS_S_VBUSP_REGS_IVPAC_TOP_0_CFG_SLV_HTS_S_VBUSP_END  (0x0000002c01ffffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MMR_VBUSP_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MMR_VBUSP_REGS_ID               (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MMR_VBUSP_REGS_TYPE             (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MMR_VBUSP_REGS_MMR_BASE         (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MMR_VBUSP_REGS_NUM_REGIONS      (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MMR_VBUSP_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MMR_VBUSP_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MMR_VBUSP_START (0x0000002c020000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MMR_VBUSP_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MMR_VBUSP_END (0x0000002c0203ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_VPAC_LDC_LSE_CFG_VP_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_VPAC_LDC_LSE_CFG_VP_REGS_ID     (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_VPAC_LDC_LSE_CFG_VP_REGS_TYPE   (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_VPAC_LDC_LSE_CFG_VP_REGS_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_VPAC_LDC_LSE_CFG_VP_REGS_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_VPAC_LDC_LSE_CFG_VP_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_VPAC_LDC_LSE_CFG_VP_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_VPAC_LDC_LSE_CFG_VP_START (0x0000002c020400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_VPAC_LDC_LSE_CFG_VP_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_VPAC_LDC_LSE_CFG_VP_END (0x0000002c0205ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALY_LUTCFG_DUALY_LUT */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALY_LUTCFG_DUALY_LUT_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALY_LUTCFG_DUALY_LUT_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALY_LUTCFG_DUALY_LUT_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALY_LUTCFG_DUALY_LUT_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALY_LUTCFG_DUALY_LUT_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALY_LUTCFG_DUALY_LUT_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALY_LUTCFG_DUALY_LUT_START (0x0000002c020800U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALY_LUTCFG_DUALY_LUT_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALY_LUTCFG_DUALY_LUT_END (0x0000002c020fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALC_LUTCFG_DUALC_LUT */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALC_LUTCFG_DUALC_LUT_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALC_LUTCFG_DUALC_LUT_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALC_LUTCFG_DUALC_LUT_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALC_LUTCFG_DUALC_LUT_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALC_LUTCFG_DUALC_LUT_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALC_LUTCFG_DUALC_LUT_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALC_LUTCFG_DUALC_LUT_START (0x0000002c021000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALC_LUTCFG_DUALC_LUT_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_PIXWRINTF_DUALC_LUTCFG_DUALC_LUT_END (0x0000002c0217ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_MESH_VBUSPI_MESH_MEM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_MESH_VBUSPI_MESH_MEM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_MESH_VBUSPI_MESH_MEM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_MESH_VBUSPI_MESH_MEM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_MESH_VBUSPI_MESH_MEM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_MESH_VBUSPI_MESH_MEM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_MESH_VBUSPI_MESH_MEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_MESH_VBUSPI_MESH_MEM_START (0x0000002c022000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_MESH_VBUSPI_MESH_MEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_MESH_VBUSPI_MESH_MEM_END (0x0000002c023fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_Y_VBUSPI_Y_MEM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_Y_VBUSPI_Y_MEM_ID   (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_Y_VBUSPI_Y_MEM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_Y_VBUSPI_Y_MEM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_Y_VBUSPI_Y_MEM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_Y_VBUSPI_Y_MEM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_Y_VBUSPI_Y_MEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_Y_VBUSPI_Y_MEM_START (0x0000002c028000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_Y_VBUSPI_Y_MEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_Y_VBUSPI_Y_MEM_END (0x0000002c02ffffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_CBCR_VBUSPI_CBCR_MEM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_CBCR_VBUSPI_CBCR_MEM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_CBCR_VBUSPI_CBCR_MEM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_CBCR_VBUSPI_CBCR_MEM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_CBCR_VBUSPI_CBCR_MEM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_CBCR_VBUSPI_CBCR_MEM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_CBCR_VBUSPI_CBCR_MEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_CBCR_VBUSPI_CBCR_MEM_START (0x0000002c030000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_CBCR_VBUSPI_CBCR_MEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_LDC0_S_VBUSP_MEMCFG_LOOP_CBCR_VBUSPI_CBCR_MEM_END (0x0000002c037fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_CFG_VP_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_CFG_VP_REGS_ID                    (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_CFG_VP_REGS_TYPE                  (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_CFG_VP_REGS_MMR_BASE              (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_CFG_VP_REGS_NUM_REGIONS           (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_CFG_VP_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_CFG_VP_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_CFG_VP_START (0x0000002c0c0000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_CFG_VP_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_CFG_VP_END (0x0000002c0c07ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_LSE_CFG_VP_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_LSE_CFG_VP_REGS_ID                (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_LSE_CFG_VP_REGS_TYPE              (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_LSE_CFG_VP_REGS_MMR_BASE          (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_LSE_CFG_VP_REGS_NUM_REGIONS       (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_LSE_CFG_VP_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_LSE_CFG_VP_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_LSE_CFG_VP_START (0x0000002c0c0800U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_LSE_CFG_VP_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_MSC_CFG_VP_LSE_CFG_VP_END (0x0000002c0c09ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_MMR_CFG_VISS_TOP_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_MMR_CFG_VISS_TOP_REGS_ID       (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_MMR_CFG_VISS_TOP_REGS_TYPE     (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_MMR_CFG_VISS_TOP_REGS_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_MMR_CFG_VISS_TOP_REGS_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_MMR_CFG_VISS_TOP_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_MMR_CFG_VISS_TOP_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_MMR_CFG_VISS_TOP_START (0x0000002c100000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_MMR_CFG_VISS_TOP_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_MMR_CFG_VISS_TOP_END (0x0000002c1001ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VPAC_VISS_LSE_CFG_VP_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VPAC_VISS_LSE_CFG_VP_REGS_ID   (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VPAC_VISS_LSE_CFG_VP_REGS_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VPAC_VISS_LSE_CFG_VP_REGS_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VPAC_VISS_LSE_CFG_VP_REGS_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VPAC_VISS_LSE_CFG_VP_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VPAC_VISS_LSE_CFG_VP_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VPAC_VISS_LSE_CFG_VP_START (0x0000002c100400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VPAC_VISS_LSE_CFG_VP_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VPAC_VISS_LSE_CFG_VP_END (0x0000002c1005ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_CFG_GLBCE_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_CFG_GLBCE_REGS_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_CFG_GLBCE_REGS_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_CFG_GLBCE_REGS_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_CFG_GLBCE_REGS_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_CFG_GLBCE_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_CFG_GLBCE_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_CFG_GLBCE_START (0x0000002c103800U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_CFG_GLBCE_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_CFG_GLBCE_END (0x0000002c103fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_STATMEM_CFG_GLBCE_STATMEM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_STATMEM_CFG_GLBCE_STATMEM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_STATMEM_CFG_GLBCE_STATMEM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_STATMEM_CFG_GLBCE_STATMEM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_STATMEM_CFG_GLBCE_STATMEM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_STATMEM_CFG_GLBCE_STATMEM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_STATMEM_CFG_GLBCE_STATMEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_STATMEM_CFG_GLBCE_STATMEM_START (0x0000002c104000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_STATMEM_CFG_GLBCE_STATMEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_K3_GLBCE_TOP_STATMEM_CFG_GLBCE_STATMEM_END (0x0000002c107fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_REGS_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_REGS_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_REGS_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_REGS_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_START (0x0000002c108000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_END (0x0000002c10ffffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_REGS_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_REGS_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_REGS_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_REGS_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_START (0x0000002c110000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_END (0x0000002c1107ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC1 */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC1_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC1_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC1_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC1_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC1_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC1_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC1_START (0x0000002c110800U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC1_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC1_END (0x0000002c110fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC2 */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC2_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC2_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC2_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC2_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC2_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC2_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC2_START (0x0000002c111000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC2_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC2_END (0x0000002c1117ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC3 */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC3_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC3_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC3_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC3_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC3_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC3_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC3_START (0x0000002c111800U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC3_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_CONTRASTC3_END (0x0000002c111fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_Y8R8 */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_Y8R8_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_Y8R8_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_Y8R8_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_Y8R8_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_Y8R8_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_Y8R8_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_Y8R8_START (0x0000002c112000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_Y8R8_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_Y8R8_END (0x0000002c1127ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_C8G8 */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_C8G8_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_C8G8_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_C8G8_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_C8G8_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_C8G8_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_C8G8_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_C8G8_START (0x0000002c112800U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_C8G8_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_C8G8_END (0x0000002c112fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_S8B8 */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_S8B8_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_S8B8_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_S8B8_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_S8B8_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_S8B8_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_S8B8_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_S8B8_START (0x0000002c113000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_S8B8_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_S8B8_END (0x0000002c1137ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_HIST */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_HIST_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_HIST_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_HIST_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_HIST_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_HIST_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_HIST_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_HIST_START (0x0000002c113800U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_HIST_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_HIST_END (0x0000002c113fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_LINE */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_LINE_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_LINE_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_LINE_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_LINE_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_LINE_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_LINE_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_LINE_START (0x0000002c118000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_LINE_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_FCC_VBUSP_FLEXCC_LINE_END (0x0000002c1187ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_MMR_S_VBUSP_RAWFE_CFG */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_MMR_S_VBUSP_RAWFE_CFG_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_MMR_S_VBUSP_RAWFE_CFG_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_MMR_S_VBUSP_RAWFE_CFG_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_MMR_S_VBUSP_RAWFE_CFG_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_MMR_S_VBUSP_RAWFE_CFG_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_MMR_S_VBUSP_RAWFE_CFG_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_MMR_S_VBUSP_RAWFE_CFG_START (0x0000002c120000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_MMR_S_VBUSP_RAWFE_CFG_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_MMR_S_VBUSP_RAWFE_CFG_END (0x0000002c1203ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_CFG_RAWFE_H3A_CFG */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_CFG_RAWFE_H3A_CFG_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_CFG_RAWFE_H3A_CFG_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_CFG_RAWFE_H3A_CFG_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_CFG_RAWFE_H3A_CFG_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_CFG_RAWFE_H3A_CFG_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_CFG_RAWFE_H3A_CFG_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_CFG_RAWFE_H3A_CFG_START (0x0000002c120400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_CFG_RAWFE_H3A_CFG_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_CFG_RAWFE_H3A_CFG_END (0x0000002c1204ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT3_RAM_RAWFE_PWL_LUT3_RAM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT3_RAM_RAWFE_PWL_LUT3_RAM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT3_RAM_RAWFE_PWL_LUT3_RAM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT3_RAM_RAWFE_PWL_LUT3_RAM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT3_RAM_RAWFE_PWL_LUT3_RAM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT3_RAM_RAWFE_PWL_LUT3_RAM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT3_RAM_RAWFE_PWL_LUT3_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT3_RAM_RAWFE_PWL_LUT3_RAM_START (0x0000002c120800U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT3_RAM_RAWFE_PWL_LUT3_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT3_RAM_RAWFE_PWL_LUT3_RAM_END (0x0000002c120fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT2_RAM_RAWFE_PWL_LUT2_RAM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT2_RAM_RAWFE_PWL_LUT2_RAM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT2_RAM_RAWFE_PWL_LUT2_RAM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT2_RAM_RAWFE_PWL_LUT2_RAM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT2_RAM_RAWFE_PWL_LUT2_RAM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT2_RAM_RAWFE_PWL_LUT2_RAM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT2_RAM_RAWFE_PWL_LUT2_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT2_RAM_RAWFE_PWL_LUT2_RAM_START (0x0000002c121000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT2_RAM_RAWFE_PWL_LUT2_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT2_RAM_RAWFE_PWL_LUT2_RAM_END (0x0000002c1217ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT1_RAM_RAWFE_PWL_LUT1_RAM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT1_RAM_RAWFE_PWL_LUT1_RAM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT1_RAM_RAWFE_PWL_LUT1_RAM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT1_RAM_RAWFE_PWL_LUT1_RAM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT1_RAM_RAWFE_PWL_LUT1_RAM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT1_RAM_RAWFE_PWL_LUT1_RAM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT1_RAM_RAWFE_PWL_LUT1_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT1_RAM_RAWFE_PWL_LUT1_RAM_START (0x0000002c121800U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT1_RAM_RAWFE_PWL_LUT1_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LUT1_RAM_RAWFE_PWL_LUT1_RAM_END (0x0000002c121fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_WDR_LUT_RAM_RAWFE_WDR_LUT_RAM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_WDR_LUT_RAM_RAWFE_WDR_LUT_RAM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_WDR_LUT_RAM_RAWFE_WDR_LUT_RAM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_WDR_LUT_RAM_RAWFE_WDR_LUT_RAM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_WDR_LUT_RAM_RAWFE_WDR_LUT_RAM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_WDR_LUT_RAM_RAWFE_WDR_LUT_RAM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_WDR_LUT_RAM_RAWFE_WDR_LUT_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_WDR_LUT_RAM_RAWFE_WDR_LUT_RAM_START (0x0000002c122000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_WDR_LUT_RAM_RAWFE_WDR_LUT_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_WDR_LUT_RAM_RAWFE_WDR_LUT_RAM_END (0x0000002c1227ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_LUT_RAM_RAWFE_H3A_LUT_RAM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_LUT_RAM_RAWFE_H3A_LUT_RAM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_LUT_RAM_RAWFE_H3A_LUT_RAM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_LUT_RAM_RAWFE_H3A_LUT_RAM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_LUT_RAM_RAWFE_H3A_LUT_RAM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_LUT_RAM_RAWFE_H3A_LUT_RAM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_LUT_RAM_RAWFE_H3A_LUT_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_LUT_RAM_RAWFE_H3A_LUT_RAM_START (0x0000002c122800U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_LUT_RAM_RAWFE_H3A_LUT_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_LUT_RAM_RAWFE_H3A_LUT_RAM_END (0x0000002c122fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_RAM_RAWFE_DPC_LUT_RAM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_RAM_RAWFE_DPC_LUT_RAM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_RAM_RAWFE_DPC_LUT_RAM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_RAM_RAWFE_DPC_LUT_RAM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_RAM_RAWFE_DPC_LUT_RAM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_RAM_RAWFE_DPC_LUT_RAM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_RAM_RAWFE_DPC_LUT_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_RAM_RAWFE_DPC_LUT_RAM_START (0x0000002c123000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_RAM_RAWFE_DPC_LUT_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_RAM_RAWFE_DPC_LUT_RAM_END (0x0000002c1233ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_LRAM_RAWFE_DPC_LRAM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_LRAM_RAWFE_DPC_LRAM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_LRAM_RAWFE_DPC_LRAM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_LRAM_RAWFE_DPC_LRAM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_LRAM_RAWFE_DPC_LRAM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_LRAM_RAWFE_DPC_LRAM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_LRAM_RAWFE_DPC_LRAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_LRAM_RAWFE_DPC_LRAM_START (0x0000002c124000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_LRAM_RAWFE_DPC_LRAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_LRAM_RAWFE_DPC_LRAM_END (0x0000002c125fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LSC_RAM_RAWFE_LSC_LUT_RAM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LSC_RAM_RAWFE_LSC_LUT_RAM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LSC_RAM_RAWFE_LSC_LUT_RAM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LSC_RAM_RAWFE_LSC_LUT_RAM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LSC_RAM_RAWFE_LSC_LUT_RAM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LSC_RAM_RAWFE_LSC_LUT_RAM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LSC_RAM_RAWFE_LSC_LUT_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LSC_RAM_RAWFE_LSC_LUT_RAM_START (0x0000002c128000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LSC_RAM_RAWFE_LSC_LUT_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_LSC_RAM_RAWFE_LSC_LUT_RAM_END (0x0000002c12ffffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_ARAM_RAWFE_H3A_ARAM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_ARAM_RAWFE_H3A_ARAM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_ARAM_RAWFE_H3A_ARAM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_ARAM_RAWFE_H3A_ARAM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_ARAM_RAWFE_H3A_ARAM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_ARAM_RAWFE_H3A_ARAM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_ARAM_RAWFE_H3A_ARAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_ARAM_RAWFE_H3A_ARAM_START (0x0000002c130000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_ARAM_RAWFE_H3A_ARAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_ARAM_RAWFE_H3A_ARAM_END (0x0000002c131fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_LRAM_RAWFE_H3A_LRAM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_LRAM_RAWFE_H3A_LRAM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_LRAM_RAWFE_H3A_LRAM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_LRAM_RAWFE_H3A_LRAM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_LRAM_RAWFE_H3A_LRAM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_LRAM_RAWFE_H3A_LRAM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_LRAM_RAWFE_H3A_LRAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_LRAM_RAWFE_H3A_LRAM_START (0x0000002c132000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_LRAM_RAWFE_H3A_LRAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_H3A_WRAP_LRAM_RAWFE_H3A_LRAM_END (0x0000002c133fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_STATRAM_RAWFE_DPC_STATRAM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_STATRAM_RAWFE_DPC_STATRAM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_STATRAM_RAWFE_DPC_STATRAM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_STATRAM_RAWFE_DPC_STATRAM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_STATRAM_RAWFE_DPC_STATRAM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_STATRAM_RAWFE_DPC_STATRAM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_STATRAM_RAWFE_DPC_STATRAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_STATRAM_RAWFE_DPC_STATRAM_START (0x0000002c136000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_STATRAM_RAWFE_DPC_STATRAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_RAWFE_CFG_DPC_STATRAM_RAWFE_DPC_STATRAM_END (0x0000002c137fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MMR_MMR_VBUSP_NSF4VCORE_REG */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MMR_MMR_VBUSP_NSF4VCORE_REG_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MMR_MMR_VBUSP_NSF4VCORE_REG_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MMR_MMR_VBUSP_NSF4VCORE_REG_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MMR_MMR_VBUSP_NSF4VCORE_REG_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MMR_MMR_VBUSP_NSF4VCORE_REG_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MMR_MMR_VBUSP_NSF4VCORE_REG_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MMR_VBUSP_NSF4VCORE_START (0x0000002c140000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MMR_MMR_VBUSP_NSF4VCORE_REG_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MMR_VBUSP_NSF4VCORE_END (0x0000002c1407ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTDATA_VBUSP_RAWHIST */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTDATA_VBUSP_RAWHIST_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTDATA_VBUSP_RAWHIST_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTDATA_VBUSP_RAWHIST_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTDATA_VBUSP_RAWHIST_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTDATA_VBUSP_RAWHIST_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTDATA_VBUSP_RAWHIST_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTDATA_VBUSP_RAWHIST_START (0x0000002c140800U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTDATA_VBUSP_RAWHIST_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTDATA_VBUSP_RAWHIST_END (0x0000002c1409ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTLUT_VBUSP_RAWHIST_LUT */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTLUT_VBUSP_RAWHIST_LUT_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTLUT_VBUSP_RAWHIST_LUT_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTLUT_VBUSP_RAWHIST_LUT_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTLUT_VBUSP_RAWHIST_LUT_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTLUT_VBUSP_RAWHIST_LUT_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTLUT_VBUSP_RAWHIST_LUT_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTLUT_VBUSP_RAWHIST_LUT_START (0x0000002c141000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTLUT_VBUSP_RAWHIST_LUT_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_RAWHIST_HISTLUT_VBUSP_RAWHIST_LUT_END (0x0000002c1417ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MEM_MMR_MMRRAM_VBUSP_MMR_RAM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MEM_MMR_MMRRAM_VBUSP_MMR_RAM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MEM_MMR_MMRRAM_VBUSP_MMR_RAM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MEM_MMR_MMRRAM_VBUSP_MMR_RAM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MEM_MMR_MMRRAM_VBUSP_MMR_RAM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MEM_MMR_MMRRAM_VBUSP_MMR_RAM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MEM_MMR_MMRRAM_VBUSP_MMR_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MEM_MMRRAM_VBUSP_MMR_RAM_START (0x0000002c144000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MEM_MMR_MMRRAM_VBUSP_MMR_RAM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_NSF4V_CFG_MEM_MMRRAM_VBUSP_MMR_RAM_END (0x0000002c147fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_EE_VBUSP_FLEXEE_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_EE_VBUSP_FLEXEE_REGS_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_EE_VBUSP_FLEXEE_REGS_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_EE_VBUSP_FLEXEE_REGS_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_EE_VBUSP_FLEXEE_REGS_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_EE_VBUSP_FLEXEE_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_EE_VBUSP_FLEXEE_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_EE_VBUSP_FLEXEE_START (0x0000002c150000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_EE_VBUSP_FLEXEE_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_EE_VBUSP_FLEXEE_END (0x0000002c157fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_DLUTS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_DLUTS_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_DLUTS_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_DLUTS_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_DLUTS_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_DLUTS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_DLUTS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_DLUTS_START (0x0000002c158000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_DLUTS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_FCP_CFA_VBUSP_FLEXCFA_DLUTS_END (0x0000002c15bfffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_MMR_MMRCFG_CAC_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_MMR_MMRCFG_CAC_REGS_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_MMR_MMRCFG_CAC_REGS_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_MMR_MMRCFG_CAC_REGS_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_MMR_MMRCFG_CAC_REGS_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_MMR_MMRCFG_CAC_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_MMR_MMRCFG_CAC_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_MMRCFG_CAC_START (0x0000002c180000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_MMR_MMRCFG_CAC_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_MMRCFG_CAC_END (0x0000002c1803ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_CORE_LUT_CFG_LUT_MEM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_CORE_LUT_CFG_LUT_MEM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_CORE_LUT_CFG_LUT_MEM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_CORE_LUT_CFG_LUT_MEM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_CORE_LUT_CFG_LUT_MEM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_CORE_LUT_CFG_LUT_MEM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_CORE_LUT_CFG_LUT_MEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_CORE_LUT_CFG_LUT_MEM_START (0x0000002c182000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_CORE_LUT_CFG_LUT_MEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_CORE_LUT_CFG_LUT_MEM_END (0x0000002c183fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_LINEMEM_CFG_LINE_MEM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_LINEMEM_CFG_LINE_MEM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_LINEMEM_CFG_LINE_MEM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_LINEMEM_CFG_LINE_MEM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_LINEMEM_CFG_LINE_MEM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_LINEMEM_CFG_LINE_MEM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_LINEMEM_CFG_LINE_MEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_LINEMEM_CFG_LINE_MEM_START (0x0000002c184000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_LINEMEM_CFG_LINE_MEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_CAC_S_VBUSP_LINEMEM_CFG_LINE_MEM_END (0x0000002c187fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_MMR_MMRCFG_PCID_REGS */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_MMR_MMRCFG_PCID_REGS_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_MMR_MMRCFG_PCID_REGS_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_MMR_MMRCFG_PCID_REGS_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_MMR_MMRCFG_PCID_REGS_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_MMR_MMRCFG_PCID_REGS_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_MMR_MMRCFG_PCID_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_MMRCFG_PCID_START (0x0000002c188000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_MMR_MMRCFG_PCID_REGS_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_MMRCFG_PCID_END (0x0000002c1883ffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_IR_REMAPLUT_LUT_CFG_IRREMAP_LUT */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_IR_REMAPLUT_LUT_CFG_IRREMAP_LUT_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_IR_REMAPLUT_LUT_CFG_IRREMAP_LUT_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_IR_REMAPLUT_LUT_CFG_IRREMAP_LUT_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_IR_REMAPLUT_LUT_CFG_IRREMAP_LUT_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_IR_REMAPLUT_LUT_CFG_IRREMAP_LUT_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_IR_REMAPLUT_LUT_CFG_IRREMAP_LUT_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_IR_REMAPLUT_LUT_CFG_IRREMAP_LUT_START (0x0000002c188800U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_IR_REMAPLUT_LUT_CFG_IRREMAP_LUT_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_IR_REMAPLUT_LUT_CFG_IRREMAP_LUT_END (0x0000002c188fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_CFG_LINEMEM_LINEMEM_CFG_LINE_MEM */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_CFG_LINEMEM_LINEMEM_CFG_LINE_MEM_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_CFG_LINEMEM_LINEMEM_CFG_LINE_MEM_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_CFG_LINEMEM_LINEMEM_CFG_LINE_MEM_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_CFG_LINEMEM_LINEMEM_CFG_LINE_MEM_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_CFG_LINEMEM_LINEMEM_CFG_LINE_MEM_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_CFG_LINEMEM_LINEMEM_CFG_LINE_MEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_CFG_LINEMEM_CFG_LINE_MEM_START (0x0000002c18c000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_CFG_LINEMEM_LINEMEM_CFG_LINE_MEM_IVPAC_TOP_0_CFG_SLV_PAR_VPAC_VISS0_S_VBUSP_VISS_PCID_S_VBUSP_CFG_LINEMEM_CFG_LINE_MEM_END (0x0000002c18ffffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_ID              (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_TYPE            (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_MMR_BASE        (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_NUM_REGIONS     (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_START (0x0000002c200000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_END (0x0000002c203fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_SET */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_SET_ID          (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_SET_TYPE        (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_SET_MMR_BASE    (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_SET_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_SET_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_SET_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_SET_START (0x0000002c204000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_SET_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_SET_END (0x0000002c207fffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_QUEUE */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_QUEUE_ID        (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_QUEUE_TYPE      (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_QUEUE_MMR_BASE  (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_QUEUE_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_QUEUE_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_QUEUE_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_QUEUE_START (0x0000002c208000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_QUEUE_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_QUEUE_END (0x0000002c20ffffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHNRT */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHNRT_ID        (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHNRT_TYPE      (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHNRT_MMR_BASE  (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHNRT_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHNRT_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHNRT_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_CHNRT_START (0x0000002c240000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHNRT_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_CHNRT_END (0x0000002c25ffffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHRT */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHRT_ID         (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHRT_TYPE       (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHRT_MMR_BASE   (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHRT_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHRT_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHRT_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_CHRT_START (0x0000002c260000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHRT_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_CHRT_END (0x0000002c27ffffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHATOMIC_DEBUG */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHATOMIC_DEBUG_ID (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHATOMIC_DEBUG_TYPE (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHATOMIC_DEBUG_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHATOMIC_DEBUG_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHATOMIC_DEBUG_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHATOMIC_DEBUG_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_CHATOMIC_DEBUG_START (0x0000002c280000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHATOMIC_DEBUG_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_CHATOMIC_DEBUG_END (0x0000002c29ffffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHCORE */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHCORE_ID       (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHCORE_TYPE     (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHCORE_MMR_BASE (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHCORE_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHCORE_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHCORE_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_CHCORE_START (0x0000002c2a0000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CHCORE_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_CHCORE_END (0x0000002c2bffffU)

/* Properties of firewall at slave: VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CAUSE */
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CAUSE_ID        (37U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CAUSE_TYPE      (CSL_FW_SECURITY)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CAUSE_MMR_BASE  (0x00000045009400U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CAUSE_NUM_REGIONS (4U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CAUSE_NUM_PRIV_IDS_PER_REGION (3U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CAUSE_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_CAUSE_START (0x0000002c2e0000U)
#define CSL_STD_FW_VPAC0_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_DRU_CAUSE_IVPAC_TOP_0_CFG_SLV_DRU_UTC_VPAC0_DRU_MMR_CFG_DRU_DRU_CAUSE_END (0x0000002c2fffffU)

/* Properties of firewall at slave: R5FSS0_CORE0_ICACHE0 */
#define CSL_STD_FW_R5FSS0_CORE0_ICACHE0_ID                                                         (42U)
#define CSL_STD_FW_R5FSS0_CORE0_ICACHE0_TYPE                                                       (CSL_FW_SECURITY)
#define CSL_STD_FW_R5FSS0_CORE0_ICACHE0_MMR_BASE                                                   (0x0000004500a800U)
#define CSL_STD_FW_R5FSS0_CORE0_ICACHE0_NUM_REGIONS                                                (1U)
#define CSL_STD_FW_R5FSS0_CORE0_ICACHE0_NUM_PRIV_IDS_PER_REGION                                    (3U)
#define CSL_STD_FW_R5FSS0_CORE0_ICACHE0_CORE0_ICACHE_START                                         (0x00000076000000U)
#define CSL_STD_FW_R5FSS0_CORE0_ICACHE0_CORE0_ICACHE_END                                           (0x000000767fffffU)

/* Properties of firewall at slave: R5FSS0_CORE0_DCACHE0 */
#define CSL_STD_FW_R5FSS0_CORE0_DCACHE0_ID                                                         (42U)
#define CSL_STD_FW_R5FSS0_CORE0_DCACHE0_TYPE                                                       (CSL_FW_SECURITY)
#define CSL_STD_FW_R5FSS0_CORE0_DCACHE0_MMR_BASE                                                   (0x0000004500a800U)
#define CSL_STD_FW_R5FSS0_CORE0_DCACHE0_NUM_REGIONS                                                (1U)
#define CSL_STD_FW_R5FSS0_CORE0_DCACHE0_NUM_PRIV_IDS_PER_REGION                                    (3U)
#define CSL_STD_FW_R5FSS0_CORE0_DCACHE0_CORE0_DCACHE_START                                         (0x00000076800000U)
#define CSL_STD_FW_R5FSS0_CORE0_DCACHE0_CORE0_DCACHE_END                                           (0x00000076ffffffU)

/* Properties of firewall at slave: R5FSS0_CORE0_ATCM0 */
#define CSL_STD_FW_R5FSS0_CORE0_ATCM0_ID                                                           (42U)
#define CSL_STD_FW_R5FSS0_CORE0_ATCM0_TYPE                                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_R5FSS0_CORE0_ATCM0_MMR_BASE                                                     (0x0000004500a800U)
#define CSL_STD_FW_R5FSS0_CORE0_ATCM0_NUM_REGIONS                                                  (1U)
#define CSL_STD_FW_R5FSS0_CORE0_ATCM0_NUM_PRIV_IDS_PER_REGION                                      (3U)
#define CSL_STD_FW_R5FSS0_CORE0_ATCM0_CORE0_ATCM_START                                             (0x00000078400000U)
#define CSL_STD_FW_R5FSS0_CORE0_ATCM0_CORE0_ATCM_END                                               (0x00000078407fffU)

/* Properties of firewall at slave: R5FSS0_CORE0_BTCM0 */
#define CSL_STD_FW_R5FSS0_CORE0_BTCM0_ID                                                           (42U)
#define CSL_STD_FW_R5FSS0_CORE0_BTCM0_TYPE                                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_R5FSS0_CORE0_BTCM0_MMR_BASE                                                     (0x0000004500a800U)
#define CSL_STD_FW_R5FSS0_CORE0_BTCM0_NUM_REGIONS                                                  (1U)
#define CSL_STD_FW_R5FSS0_CORE0_BTCM0_NUM_PRIV_IDS_PER_REGION                                      (3U)
#define CSL_STD_FW_R5FSS0_CORE0_BTCM0_CORE0_BTCM_START                                             (0x00000078500000U)
#define CSL_STD_FW_R5FSS0_CORE0_BTCM0_CORE0_BTCM_END                                               (0x00000078507fffU)

/* Properties of firewall at slave: DMASS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA */
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_ID                                 (160U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_TYPE                               (CSL_FW_SECURITY)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_MMR_BASE                           (0x00000045028000U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_NUM_REGIONS                        (8U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_NUM_PRIV_IDS_PER_REGION            (3U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_SEC_PROXY_SRC_TARGET_DATA_START    (0x0000004d000000U)
#define CSL_STD_FW_DMASS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_SEC_PROXY_SRC_TARGET_DATA_END      (0x0000004d07ffffU)

/* Properties of firewall at slave: SA3_SS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA */
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_ID                                (161U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_TYPE                              (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_MMR_BASE                          (0x00000045028400U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_NUM_REGIONS                       (8U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_NUM_PRIV_IDS_PER_REGION           (3U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_SEC_PROXY_SRC_TARGET_DATA_START   (0x00000043600000U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_SEC_PROXY_SRC_TARGET_DATA_SEC_PROXY_SRC_TARGET_DATA_END     (0x0000004360ffffU)

/* Properties of firewall at slave: SMS0_ROM_0_IROM_SLV_ROM */
#define CSL_STD_FW_SMS0_ROM_0_IROM_SLV_ROM_ID                                                      (512U)
#define CSL_STD_FW_SMS0_ROM_0_IROM_SLV_ROM_TYPE                                                    (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_ROM_0_IROM_SLV_ROM_MMR_BASE                                                (0x00000045080000U)
#define CSL_STD_FW_SMS0_ROM_0_IROM_SLV_ROM_NUM_REGIONS                                             (2U)
#define CSL_STD_FW_SMS0_ROM_0_IROM_SLV_ROM_NUM_PRIV_IDS_PER_REGION                                 (1U)
#define CSL_STD_FW_SMS0_ROM_0_IROM_SLV_ROM_IROM_SLV_ROM_START                                      (0x00000000000000U)
#define CSL_STD_FW_SMS0_ROM_0_IROM_SLV_ROM_IROM_SLV_ROM_END                                        (0x0000000003ffffU)

/* Properties of firewall at slave: SMS0_TIFS_SRAM_0_TIFS_SRAM0 */
#define CSL_STD_FW_SMS0_TIFS_SRAM_0_TIFS_SRAM0_ID                                                  (513U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_0_TIFS_SRAM0_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_TIFS_SRAM_0_TIFS_SRAM0_MMR_BASE                                            (0x00000045080400U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_0_TIFS_SRAM0_NUM_REGIONS                                         (8U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_0_TIFS_SRAM0_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_0_TIFS_SRAM0_TIFS_SRAM0_START                                    (0x00000044040000U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_0_TIFS_SRAM0_TIFS_SRAM0_END                                      (0x0000004405ffffU)

/* Properties of firewall at slave: SMS0_TIFS_SRAM_1_TIFS_SRAM1_0 */
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_0_ID                                                (514U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_0_TYPE                                              (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_0_MMR_BASE                                          (0x00000045080800U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_0_NUM_REGIONS                                       (8U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_0_NUM_PRIV_IDS_PER_REGION                           (3U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_0_TIFS_SRAM1_0_START                                (0x00000044060000U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_0_TIFS_SRAM1_0_END                                  (0x00000044067fffU)

/* Properties of firewall at slave: SMS0_TIFS_SRAM_1_TIFS_SRAM1_1 */
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_1_ID                                                (514U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_1_TYPE                                              (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_1_MMR_BASE                                          (0x00000045080800U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_1_NUM_REGIONS                                       (8U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_1_NUM_PRIV_IDS_PER_REGION                           (3U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_1_TIFS_SRAM1_1_START                                (0x00000044068000U)
#define CSL_STD_FW_SMS0_TIFS_SRAM_1_TIFS_SRAM1_1_TIFS_SRAM1_1_END                                  (0x0000004406bfffU)

/* Properties of firewall at slave: SMS0_PWRCTRL_0_TIFS_PWR_MMR */
#define CSL_STD_FW_SMS0_PWRCTRL_0_TIFS_PWR_MMR_ID                                                  (528U)
#define CSL_STD_FW_SMS0_PWRCTRL_0_TIFS_PWR_MMR_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_PWRCTRL_0_TIFS_PWR_MMR_MMR_BASE                                            (0x00000045084000U)
#define CSL_STD_FW_SMS0_PWRCTRL_0_TIFS_PWR_MMR_NUM_REGIONS                                         (1U)
#define CSL_STD_FW_SMS0_PWRCTRL_0_TIFS_PWR_MMR_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_SMS0_PWRCTRL_0_TIFS_PWR_MMR_PWR_START                                           (0x00000044130000U)
#define CSL_STD_FW_SMS0_PWRCTRL_0_TIFS_PWR_MMR_PWR_END                                             (0x000000441307ffU)

/* Properties of firewall at slave: SMS0_DMTIMER_0_TIFS_DMTIMER0 */
#define CSL_STD_FW_SMS0_DMTIMER_0_TIFS_DMTIMER0_ID                                                 (536U)
#define CSL_STD_FW_SMS0_DMTIMER_0_TIFS_DMTIMER0_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_DMTIMER_0_TIFS_DMTIMER0_MMR_BASE                                           (0x00000045086000U)
#define CSL_STD_FW_SMS0_DMTIMER_0_TIFS_DMTIMER0_NUM_REGIONS                                        (1U)
#define CSL_STD_FW_SMS0_DMTIMER_0_TIFS_DMTIMER0_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_SMS0_DMTIMER_0_TIFS_DMTIMER0_DMTIMER0_START                                     (0x00000044133000U)
#define CSL_STD_FW_SMS0_DMTIMER_0_TIFS_DMTIMER0_DMTIMER0_END                                       (0x000000441333ffU)

/* Properties of firewall at slave: SMS0_DMTIMER_1_TIFS_DMTIMER1 */
#define CSL_STD_FW_SMS0_DMTIMER_1_TIFS_DMTIMER1_ID                                                 (537U)
#define CSL_STD_FW_SMS0_DMTIMER_1_TIFS_DMTIMER1_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_DMTIMER_1_TIFS_DMTIMER1_MMR_BASE                                           (0x00000045086400U)
#define CSL_STD_FW_SMS0_DMTIMER_1_TIFS_DMTIMER1_NUM_REGIONS                                        (1U)
#define CSL_STD_FW_SMS0_DMTIMER_1_TIFS_DMTIMER1_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_SMS0_DMTIMER_1_TIFS_DMTIMER1_DMTIMER1_START                                     (0x00000044134000U)
#define CSL_STD_FW_SMS0_DMTIMER_1_TIFS_DMTIMER1_DMTIMER1_END                                       (0x000000441343ffU)

/* Properties of firewall at slave: SMS0_RTI_0_TIFS_WDT_RTI */
#define CSL_STD_FW_SMS0_RTI_0_TIFS_WDT_RTI_ID                                                      (544U)
#define CSL_STD_FW_SMS0_RTI_0_TIFS_WDT_RTI_TYPE                                                    (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_RTI_0_TIFS_WDT_RTI_MMR_BASE                                                (0x00000045088000U)
#define CSL_STD_FW_SMS0_RTI_0_TIFS_WDT_RTI_NUM_REGIONS                                             (1U)
#define CSL_STD_FW_SMS0_RTI_0_TIFS_WDT_RTI_NUM_PRIV_IDS_PER_REGION                                 (3U)
#define CSL_STD_FW_SMS0_RTI_0_TIFS_WDT_RTI_WDT_RTI_START                                           (0x00000044135000U)
#define CSL_STD_FW_SMS0_RTI_0_TIFS_WDT_RTI_WDT_RTI_END                                             (0x000000441350ffU)

/* Properties of firewall at slave: SMS0_WDTCTRL_0_TIFS_RTI_MMR */
#define CSL_STD_FW_SMS0_WDTCTRL_0_TIFS_RTI_MMR_ID                                                  (545U)
#define CSL_STD_FW_SMS0_WDTCTRL_0_TIFS_RTI_MMR_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_WDTCTRL_0_TIFS_RTI_MMR_MMR_BASE                                            (0x00000045088400U)
#define CSL_STD_FW_SMS0_WDTCTRL_0_TIFS_RTI_MMR_NUM_REGIONS                                         (1U)
#define CSL_STD_FW_SMS0_WDTCTRL_0_TIFS_RTI_MMR_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_SMS0_WDTCTRL_0_TIFS_RTI_MMR_RTI_START                                           (0x00000044135100U)
#define CSL_STD_FW_SMS0_WDTCTRL_0_TIFS_RTI_MMR_RTI_END                                             (0x000000441351ffU)

/* Properties of firewall at slave: SMS0_RAT_0_TIFS_RAT_MMRS */
#define CSL_STD_FW_SMS0_RAT_0_TIFS_RAT_MMRS_ID                                                     (552U)
#define CSL_STD_FW_SMS0_RAT_0_TIFS_RAT_MMRS_TYPE                                                   (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_RAT_0_TIFS_RAT_MMRS_MMR_BASE                                               (0x0000004508a000U)
#define CSL_STD_FW_SMS0_RAT_0_TIFS_RAT_MMRS_NUM_REGIONS                                            (1U)
#define CSL_STD_FW_SMS0_RAT_0_TIFS_RAT_MMRS_NUM_PRIV_IDS_PER_REGION                                (3U)
#define CSL_STD_FW_SMS0_RAT_0_TIFS_RAT_MMRS_RAT_START                                              (0x00000044200000U)
#define CSL_STD_FW_SMS0_RAT_0_TIFS_RAT_MMRS_RAT_END                                                (0x00000044200fffU)

/* Properties of firewall at slave: SMS0_TIFS_ECC_AGGR_0_TIFS_ECC_REGS */
#define CSL_STD_FW_SMS0_TIFS_ECC_AGGR_0_TIFS_ECC_REGS_ID                                           (562U)
#define CSL_STD_FW_SMS0_TIFS_ECC_AGGR_0_TIFS_ECC_REGS_TYPE                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_TIFS_ECC_AGGR_0_TIFS_ECC_REGS_MMR_BASE                                     (0x0000004508c800U)
#define CSL_STD_FW_SMS0_TIFS_ECC_AGGR_0_TIFS_ECC_REGS_NUM_REGIONS                                  (2U)
#define CSL_STD_FW_SMS0_TIFS_ECC_AGGR_0_TIFS_ECC_REGS_NUM_PRIV_IDS_PER_REGION                      (3U)
#define CSL_STD_FW_SMS0_TIFS_ECC_AGGR_0_TIFS_ECC_REGS_ECC_AGGR_START                               (0x00000043700000U)
#define CSL_STD_FW_SMS0_TIFS_ECC_AGGR_0_TIFS_ECC_REGS_ECC_AGGR_END                                 (0x000000437003ffU)

/* Properties of firewall at slave: SMS0_SECCTRL_0_TIFS_SEC_MMR */
#define CSL_STD_FW_SMS0_SECCTRL_0_TIFS_SEC_MMR_ID                                                  (576U)
#define CSL_STD_FW_SMS0_SECCTRL_0_TIFS_SEC_MMR_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_SECCTRL_0_TIFS_SEC_MMR_MMR_BASE                                            (0x00000045090000U)
#define CSL_STD_FW_SMS0_SECCTRL_0_TIFS_SEC_MMR_NUM_REGIONS                                         (1U)
#define CSL_STD_FW_SMS0_SECCTRL_0_TIFS_SEC_MMR_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_SMS0_SECCTRL_0_TIFS_SEC_MMR_SEC_START                                           (0x00000044230000U)
#define CSL_STD_FW_SMS0_SECCTRL_0_TIFS_SEC_MMR_SEC_END                                             (0x00000044230fffU)

/* Properties of firewall at slave: SMS0_DBG_AUTH_0_TIFS_DBGAUTH */
#define CSL_STD_FW_SMS0_DBG_AUTH_0_TIFS_DBGAUTH_ID                                                 (578U)
#define CSL_STD_FW_SMS0_DBG_AUTH_0_TIFS_DBGAUTH_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_DBG_AUTH_0_TIFS_DBGAUTH_MMR_BASE                                           (0x00000045090800U)
#define CSL_STD_FW_SMS0_DBG_AUTH_0_TIFS_DBGAUTH_NUM_REGIONS                                        (1U)
#define CSL_STD_FW_SMS0_DBG_AUTH_0_TIFS_DBGAUTH_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_SMS0_DBG_AUTH_0_TIFS_DBGAUTH_TIFS_DBGAUTH_START                                 (0x00000044232000U)
#define CSL_STD_FW_SMS0_DBG_AUTH_0_TIFS_DBGAUTH_TIFS_DBGAUTH_END                                   (0x000000442320ffU)

/* Properties of firewall at slave: SMS0_SEC_MGR_0_TIFS_SECMGR */
#define CSL_STD_FW_SMS0_SEC_MGR_0_TIFS_SECMGR_ID                                                   (582U)
#define CSL_STD_FW_SMS0_SEC_MGR_0_TIFS_SECMGR_TYPE                                                 (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_SEC_MGR_0_TIFS_SECMGR_MMR_BASE                                             (0x00000045091800U)
#define CSL_STD_FW_SMS0_SEC_MGR_0_TIFS_SECMGR_NUM_REGIONS                                          (2U)
#define CSL_STD_FW_SMS0_SEC_MGR_0_TIFS_SECMGR_NUM_PRIV_IDS_PER_REGION                              (3U)
#define CSL_STD_FW_SMS0_SEC_MGR_0_TIFS_SECMGR_SECMGR_START                                         (0x00000044234000U)
#define CSL_STD_FW_SMS0_SEC_MGR_0_TIFS_SECMGR_SECMGR_END                                           (0x00000044237fffU)

/* Properties of firewall at slave: SMS0_DMTIMER_2_TIFS_DMTIMER2 */
#define CSL_STD_FW_SMS0_DMTIMER_2_TIFS_DMTIMER2_ID                                                 (592U)
#define CSL_STD_FW_SMS0_DMTIMER_2_TIFS_DMTIMER2_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_DMTIMER_2_TIFS_DMTIMER2_MMR_BASE                                           (0x00000045094000U)
#define CSL_STD_FW_SMS0_DMTIMER_2_TIFS_DMTIMER2_NUM_REGIONS                                        (1U)
#define CSL_STD_FW_SMS0_DMTIMER_2_TIFS_DMTIMER2_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_SMS0_DMTIMER_2_TIFS_DMTIMER2_DMTIMER2_START                                     (0x00000044238000U)
#define CSL_STD_FW_SMS0_DMTIMER_2_TIFS_DMTIMER2_DMTIMER2_END                                       (0x000000442383ffU)

/* Properties of firewall at slave: SMS0_DMTIMER_3_TIFS_DMTIMER3 */
#define CSL_STD_FW_SMS0_DMTIMER_3_TIFS_DMTIMER3_ID                                                 (593U)
#define CSL_STD_FW_SMS0_DMTIMER_3_TIFS_DMTIMER3_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_DMTIMER_3_TIFS_DMTIMER3_MMR_BASE                                           (0x00000045094400U)
#define CSL_STD_FW_SMS0_DMTIMER_3_TIFS_DMTIMER3_NUM_REGIONS                                        (1U)
#define CSL_STD_FW_SMS0_DMTIMER_3_TIFS_DMTIMER3_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_SMS0_DMTIMER_3_TIFS_DMTIMER3_DMTIMER3_START                                     (0x00000044239000U)
#define CSL_STD_FW_SMS0_DMTIMER_3_TIFS_DMTIMER3_DMTIMER3_END                                       (0x000000442393ffU)

/* Properties of firewall at slave: SMS0_AESEIP38T_0_TIFS_AES38T */
#define CSL_STD_FW_SMS0_AESEIP38T_0_TIFS_AES38T_ID                                                 (602U)
#define CSL_STD_FW_SMS0_AESEIP38T_0_TIFS_AES38T_TYPE                                               (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_AESEIP38T_0_TIFS_AES38T_MMR_BASE                                           (0x00000045096800U)
#define CSL_STD_FW_SMS0_AESEIP38T_0_TIFS_AES38T_NUM_REGIONS                                        (2U)
#define CSL_STD_FW_SMS0_AESEIP38T_0_TIFS_AES38T_NUM_PRIV_IDS_PER_REGION                            (3U)
#define CSL_STD_FW_SMS0_AESEIP38T_0_TIFS_AES38T_AES_START                                          (0x0000004423c000U)
#define CSL_STD_FW_SMS0_AESEIP38T_0_TIFS_AES38T_AES_END                                            (0x0000004423dfffU)

/* Properties of firewall at slave: SA3_SS0_DMSS_ECCAGGR_0_ECCAGGR_CFG_REGS */
#define CSL_STD_FW_SA3_SS0_DMSS_ECCAGGR_0__ECCAGGR_CFG_REGS_ID                                     (638U)
#define CSL_STD_FW_SA3_SS0_DMSS_ECCAGGR_0__ECCAGGR_CFG_REGS_TYPE                                   (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_DMSS_ECCAGGR_0__ECCAGGR_CFG_REGS_MMR_BASE                               (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_DMSS_ECCAGGR_0__ECCAGGR_CFG_REGS_NUM_REGIONS                            (24U)
#define CSL_STD_FW_SA3_SS0_DMSS_ECCAGGR_0__ECCAGGR_CFG_REGS_NUM_PRIV_IDS_PER_REGION                (3U)
#define CSL_STD_FW_SA3_SS0_DMSS_ECCAGGR_0__ECCAGGR_CFG_REGS_ECCAGGR_CFG_START                      (0x00000043702000U)
#define CSL_STD_FW_SA3_SS0_DMSS_ECCAGGR_0__ECCAGGR_CFG_REGS_ECCAGGR_CFG_END                        (0x000000437023ffU)

/* Properties of firewall at slave: SA3_SS0_PSILCFG_0_PSILCFG_CFG_PROXY */
#define CSL_STD_FW_SA3_SS0_PSILCFG_0_PSILCFG_CFG_PROXY_ID                                          (638U)
#define CSL_STD_FW_SA3_SS0_PSILCFG_0_PSILCFG_CFG_PROXY_TYPE                                        (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_PSILCFG_0_PSILCFG_CFG_PROXY_MMR_BASE                                    (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_PSILCFG_0_PSILCFG_CFG_PROXY_NUM_REGIONS                                 (24U)
#define CSL_STD_FW_SA3_SS0_PSILCFG_0_PSILCFG_CFG_PROXY_NUM_PRIV_IDS_PER_REGION                     (3U)
#define CSL_STD_FW_SA3_SS0_PSILCFG_0_PSILCFG_CFG_PROXY_PSILCFG_CFG_PROXY_START                     (0x00000044801000U)
#define CSL_STD_FW_SA3_SS0_PSILCFG_0_PSILCFG_CFG_PROXY_PSILCFG_CFG_PROXY_END                       (0x000000448011ffU)

/* Properties of firewall at slave: SA3_SS0_PSILSS_0_PSILSS_CFG_MMRS */
#define CSL_STD_FW_SA3_SS0_PSILSS_0_PSILSS_CFG_MMRS_ID                                             (638U)
#define CSL_STD_FW_SA3_SS0_PSILSS_0_PSILSS_CFG_MMRS_TYPE                                           (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_PSILSS_0_PSILSS_CFG_MMRS_MMR_BASE                                       (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_PSILSS_0_PSILSS_CFG_MMRS_NUM_REGIONS                                    (24U)
#define CSL_STD_FW_SA3_SS0_PSILSS_0_PSILSS_CFG_MMRS_NUM_PRIV_IDS_PER_REGION                        (3U)
#define CSL_STD_FW_SA3_SS0_PSILSS_0_PSILSS_CFG_MMRS_PSILSS_CFG_MMRS_START                          (0x00000044802000U)
#define CSL_STD_FW_SA3_SS0_PSILSS_0_PSILSS_CFG_MMRS_PSILSS_CFG_MMRS_END                            (0x00000044802fffU)

/* Properties of firewall at slave: SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_MMRS */
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_MMRS_ID                                 (638U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_MMRS_TYPE                               (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_MMRS_MMR_BASE                           (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_MMRS_NUM_REGIONS                        (24U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_MMRS_NUM_PRIV_IDS_PER_REGION            (3U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_MMRS_IPCSS_SEC_PROXY_CFG_MMRS_START     (0x00000044804000U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_MMRS_IPCSS_SEC_PROXY_CFG_MMRS_END       (0x000000448040ffU)

/* Properties of firewall at slave: SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_GCFG */
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_GCFG_ID                                     (638U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_GCFG_TYPE                                   (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_GCFG_MMR_BASE                               (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_GCFG_NUM_REGIONS                            (24U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_GCFG_NUM_PRIV_IDS_PER_REGION                (3U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_GCFG_IPCSS_RINGACC_CFG_GCFG_START           (0x00000044805000U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_GCFG_IPCSS_RINGACC_CFG_GCFG_END             (0x000000448053ffU)

/* Properties of firewall at slave: SA3_SS0_INTAGGR_0_INTAGGR_CFG_CFG */
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_CFG_ID                                            (638U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_CFG_TYPE                                          (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_CFG_MMR_BASE                                      (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_CFG_NUM_REGIONS                                   (24U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_CFG_NUM_PRIV_IDS_PER_REGION                       (3U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_CFG_INTAGGR_CFG_START                             (0x00000044808000U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_CFG_INTAGGR_CFG_END                               (0x0000004480801fU)

/* Properties of firewall at slave: SA3_SS0_INTAGGR_0_INTAGGR_CFG_IMAP */
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_IMAP_ID                                           (638U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_IMAP_TYPE                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_IMAP_MMR_BASE                                     (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_IMAP_NUM_REGIONS                                  (24U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_IMAP_NUM_PRIV_IDS_PER_REGION                      (3U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_IMAP_INTAGGR_CFG_IMAP_START                       (0x00000044809000U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_IMAP_INTAGGR_CFG_IMAP_END                         (0x000000448093ffU)

/* Properties of firewall at slave: SA3_SS0_INTAGGR_0_INTAGGR_CFG_MCAST */
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_MCAST_ID                                          (638U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_MCAST_TYPE                                        (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_MCAST_MMR_BASE                                    (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_MCAST_NUM_REGIONS                                 (24U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_MCAST_NUM_PRIV_IDS_PER_REGION                     (3U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_MCAST_INTAGGR_CFG_MCAST_START                     (0x0000004480a000U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_MCAST_INTAGGR_CFG_MCAST_END                       (0x0000004480a3ffU)

/* Properties of firewall at slave: SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTCFG */
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTCFG_ID                                        (638U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTCFG_TYPE                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTCFG_MMR_BASE                                  (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTCFG_NUM_REGIONS                               (24U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTCFG_NUM_PRIV_IDS_PER_REGION                   (3U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTCFG_INTAGGR_CFG_GCNTCFG_START                 (0x0000004480b000U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTCFG_INTAGGR_CFG_GCNTCFG_END                   (0x0000004480b3ffU)

/* Properties of firewall at slave: SA3_SS0_INTAGGR_0_INTAGGR_CFG_INTR */
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_INTR_ID                                           (638U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_INTR_TYPE                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_INTR_MMR_BASE                                     (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_INTR_NUM_REGIONS                                  (24U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_INTR_NUM_PRIV_IDS_PER_REGION                      (3U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_INTR_INTAGGR_CFG_INTR_START                       (0x00000044810000U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_INTR_INTAGGR_CFG_INTR_END                         (0x00000044817fffU)

/* Properties of firewall at slave: SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTRTI */
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTRTI_ID                                        (638U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTRTI_TYPE                                      (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTRTI_MMR_BASE                                  (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTRTI_NUM_REGIONS                               (24U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTRTI_NUM_PRIV_IDS_PER_REGION                   (3U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTRTI_INTAGGR_CFG_GCNTRTI_START                 (0x00000044820000U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_GCNTRTI_INTAGGR_CFG_GCNTRTI_END                   (0x0000004483ffffU)

/* Properties of firewall at slave: SA3_SS0_INTAGGR_0_INTAGGR_CFG_UNMAP */
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_UNMAP_ID                                          (638U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_UNMAP_TYPE                                        (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_UNMAP_MMR_BASE                                    (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_UNMAP_NUM_REGIONS                                 (24U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_UNMAP_NUM_PRIV_IDS_PER_REGION                     (3U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_UNMAP_INTAGGR_CFG_UNMAP_START                     (0x00000044840000U)
#define CSL_STD_FW_SA3_SS0_INTAGGR_0_INTAGGR_CFG_UNMAP_INTAGGR_CFG_UNMAP_END                       (0x0000004484ffffU)

/* Properties of firewall at slave: SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_SCFG */
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_SCFG_ID                                 (638U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_SCFG_TYPE                               (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_SCFG_MMR_BASE                           (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_SCFG_NUM_REGIONS                        (24U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_SCFG_NUM_PRIV_IDS_PER_REGION            (3U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_SCFG_IPCSS_SEC_PROXY_CFG_SCFG_START     (0x00000044860000U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_SCFG_IPCSS_SEC_PROXY_CFG_SCFG_END       (0x0000004487ffffU)

/* Properties of firewall at slave: SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_RT */
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_RT_ID                                   (638U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_RT_TYPE                                 (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_RT_MMR_BASE                             (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_RT_NUM_REGIONS                          (24U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_RT_NUM_PRIV_IDS_PER_REGION              (3U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_RT_IPCSS_SEC_PROXY_CFG_RT_START         (0x00000044880000U)
#define CSL_STD_FW_SA3_SS0_SEC_PROXY_0_IPCSS_SEC_PROXY_CFG_RT_IPCSS_SEC_PROXY_CFG_RT_END           (0x0000004489ffffU)

/* Properties of firewall at slave: SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_CFG */
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_CFG_ID                                      (638U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_CFG_TYPE                                    (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_CFG_MMR_BASE                                (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_CFG_NUM_REGIONS                             (24U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_CFG_NUM_PRIV_IDS_PER_REGION                 (3U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_CFG_IPCSS_RINGACC_CFG_START                 (0x000000448c0000U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_CFG_IPCSS_RINGACC_CFG_END                   (0x000000448fffffU)

/* Properties of firewall at slave: SA3_SS0_PKTDMA_0_PKTDMA_CFG_GCFG */
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_GCFG_ID                                             (638U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_GCFG_TYPE                                           (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_GCFG_MMR_BASE                                       (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_GCFG_NUM_REGIONS                                    (24U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_GCFG_NUM_PRIV_IDS_PER_REGION                        (3U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_GCFG_PKTDMA_CFG_GCFG_START                          (0x00000044910000U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_GCFG_PKTDMA_CFG_GCFG_END                            (0x000000449100ffU)

/* Properties of firewall at slave: SA3_SS0_PKTDMA_0_PKTDMA_CFG_RFLOW */
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RFLOW_ID                                            (638U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RFLOW_TYPE                                          (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RFLOW_MMR_BASE                                      (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RFLOW_NUM_REGIONS                                   (24U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RFLOW_NUM_PRIV_IDS_PER_REGION                       (3U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RFLOW_PKTDMA_CFG_RFLOW_START                        (0x00000044911000U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RFLOW_PKTDMA_CFG_RFLOW_END                          (0x000000449113ffU)

/* Properties of firewall at slave: SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHAN */
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHAN_ID                                            (638U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHAN_TYPE                                          (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHAN_MMR_BASE                                      (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHAN_NUM_REGIONS                                   (24U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHAN_NUM_PRIV_IDS_PER_REGION                       (3U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHAN_PKTDMA_CFG_RCHAN_START                        (0x00000044912000U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHAN_PKTDMA_CFG_RCHAN_END                          (0x000000449123ffU)

/* Properties of firewall at slave: SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHAN */
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHAN_ID                                            (638U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHAN_TYPE                                          (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHAN_MMR_BASE                                      (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHAN_NUM_REGIONS                                   (24U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHAN_NUM_PRIV_IDS_PER_REGION                       (3U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHAN_PKTDMA_CFG_TCHAN_START                        (0x00000044913000U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHAN_PKTDMA_CFG_TCHAN_END                          (0x000000449131ffU)

/* Properties of firewall at slave: SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHANRT */
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHANRT_ID                                          (638U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHANRT_TYPE                                        (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHANRT_MMR_BASE                                    (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHANRT_NUM_REGIONS                                 (24U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHANRT_NUM_PRIV_IDS_PER_REGION                     (3U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHANRT_PKTDMA_CFG_RCHANRT_START                    (0x00000044914000U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RCHANRT_PKTDMA_CFG_RCHANRT_END                      (0x00000044917fffU)

/* Properties of firewall at slave: SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHANRT */
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHANRT_ID                                          (638U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHANRT_TYPE                                        (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHANRT_MMR_BASE                                    (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHANRT_NUM_REGIONS                                 (24U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHANRT_NUM_PRIV_IDS_PER_REGION                     (3U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHANRT_PKTDMA_CFG_TCHANRT_START                    (0x00000044918000U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_TCHANRT_PKTDMA_CFG_TCHANRT_END                      (0x00000044919fffU)

/* Properties of firewall at slave: SA3_SS0_PKTDMA_0_PKTDMA_CFG_RING */
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RING_ID                                             (638U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RING_TYPE                                           (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RING_MMR_BASE                                       (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RING_NUM_REGIONS                                    (24U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RING_NUM_PRIV_IDS_PER_REGION                        (3U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RING_PKTDMA_CFG_RING_START                          (0x0000004491a000U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RING_PKTDMA_CFG_RING_END                            (0x0000004491bfffU)

/* Properties of firewall at slave: SA3_SS0_PKTDMA_0_PKTDMA_CFG_RINGRT */
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RINGRT_ID                                           (638U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RINGRT_TYPE                                         (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RINGRT_MMR_BASE                                     (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RINGRT_NUM_REGIONS                                  (24U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RINGRT_NUM_PRIV_IDS_PER_REGION                      (3U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RINGRT_PKTDMA_CFG_RINGRT_START                      (0x00000044940000U)
#define CSL_STD_FW_SA3_SS0_PKTDMA_0_PKTDMA_CFG_RINGRT_PKTDMA_CFG_RINGRT_END                        (0x0000004497ffffU)

/* Properties of firewall at slave: SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_RT */
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_RT_ID                                       (638U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_RT_TYPE                                     (CSL_FW_SECURITY)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_RT_MMR_BASE                                 (0x0000004509f800U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_RT_NUM_REGIONS                              (24U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_RT_NUM_PRIV_IDS_PER_REGION                  (3U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_RT_IPCSS_RINGACC_CFG_RT_START               (0x00000044c00000U)
#define CSL_STD_FW_SA3_SS0_RINGACC_0_IPCSS_RINGACC_CFG_RT_IPCSS_RINGACC_CFG_RT_END                 (0x00000044ffffffU)

/* Properties of firewall at slave: SMS0_FWMGR_0_TIFS_FW_VBUS_CFG */
#define CSL_STD_FW_SMS0_FWMGR_0_TIFS_FW_VBUS_CFG_ID                                                (639U)
#define CSL_STD_FW_SMS0_FWMGR_0_TIFS_FW_VBUS_CFG_TYPE                                              (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_FWMGR_0_TIFS_FW_VBUS_CFG_MMR_BASE                                          (0x0000004509fc00U)
#define CSL_STD_FW_SMS0_FWMGR_0_TIFS_FW_VBUS_CFG_NUM_REGIONS                                       (8U)
#define CSL_STD_FW_SMS0_FWMGR_0_TIFS_FW_VBUS_CFG_NUM_PRIV_IDS_PER_REGION                           (3U)
#define CSL_STD_FW_SMS0_FWMGR_0_TIFS_FW_VBUS_CFG_FW_START                                          (0x00000045000000U)
#define CSL_STD_FW_SMS0_FWMGR_0_TIFS_FW_VBUS_CFG_FW_END                                            (0x00000045ffffffU)

/* Properties of firewall at slave: SMS0_HSM_SRAM_0_HSM_SRAM0_0 */
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_0_ID                                                  (641U)
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_0_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_0_MMR_BASE                                            (0x000000450a0400U)
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_0_NUM_REGIONS                                         (8U)
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_0_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_0_HSM_SRAM0_0_START                                   (0x00000043c00000U)
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_0_HSM_SRAM0_0_END                                     (0x00000043c1ffffU)

/* Properties of firewall at slave: SMS0_HSM_SRAM_0_HSM_SRAM0_1 */
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_1_ID                                                  (641U)
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_1_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_1_MMR_BASE                                            (0x000000450a0400U)
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_1_NUM_REGIONS                                         (8U)
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_1_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_1_HSM_SRAM0_1_START                                   (0x00000043c20000U)
#define CSL_STD_FW_SMS0_HSM_SRAM_0_HSM_SRAM0_1_HSM_SRAM0_1_END                                     (0x00000043c2ffffU)

/* Properties of firewall at slave: SMS0_HSM_SRAM_1_HSM_SRAM1 */
#define CSL_STD_FW_SMS0_HSM_SRAM_1_HSM_SRAM1_ID                                                    (642U)
#define CSL_STD_FW_SMS0_HSM_SRAM_1_HSM_SRAM1_TYPE                                                  (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_HSM_SRAM_1_HSM_SRAM1_MMR_BASE                                              (0x000000450a0800U)
#define CSL_STD_FW_SMS0_HSM_SRAM_1_HSM_SRAM1_NUM_REGIONS                                           (8U)
#define CSL_STD_FW_SMS0_HSM_SRAM_1_HSM_SRAM1_NUM_PRIV_IDS_PER_REGION                               (3U)
#define CSL_STD_FW_SMS0_HSM_SRAM_1_HSM_SRAM1_HSM_SRAM1_START                                       (0x00000043c30000U)
#define CSL_STD_FW_SMS0_HSM_SRAM_1_HSM_SRAM1_HSM_SRAM1_END                                         (0x00000043c3ffffU)

/* Properties of firewall at slave: SMS0_RTI_1_HSM_WDT_RTI */
#define CSL_STD_FW_SMS0_RTI_1_HSM_WDT_RTI_ID                                                       (672U)
#define CSL_STD_FW_SMS0_RTI_1_HSM_WDT_RTI_TYPE                                                     (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_RTI_1_HSM_WDT_RTI_MMR_BASE                                                 (0x000000450a8000U)
#define CSL_STD_FW_SMS0_RTI_1_HSM_WDT_RTI_NUM_REGIONS                                              (1U)
#define CSL_STD_FW_SMS0_RTI_1_HSM_WDT_RTI_NUM_PRIV_IDS_PER_REGION                                  (3U)
#define CSL_STD_FW_SMS0_RTI_1_HSM_WDT_RTI_HSM_WDT_RTI_START                                        (0x00000043935000U)
#define CSL_STD_FW_SMS0_RTI_1_HSM_WDT_RTI_HSM_WDT_RTI_END                                          (0x000000439350ffU)

/* Properties of firewall at slave: SMS0_WDTCTRL_1_HSM_CTRL_MMR */
#define CSL_STD_FW_SMS0_WDTCTRL_1_HSM_CTRL_MMR_ID                                                  (673U)
#define CSL_STD_FW_SMS0_WDTCTRL_1_HSM_CTRL_MMR_TYPE                                                (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_WDTCTRL_1_HSM_CTRL_MMR_MMR_BASE                                            (0x000000450a8400U)
#define CSL_STD_FW_SMS0_WDTCTRL_1_HSM_CTRL_MMR_NUM_REGIONS                                         (1U)
#define CSL_STD_FW_SMS0_WDTCTRL_1_HSM_CTRL_MMR_NUM_PRIV_IDS_PER_REGION                             (3U)
#define CSL_STD_FW_SMS0_WDTCTRL_1_HSM_CTRL_MMR_HSM_CTRL_MMR_START                                  (0x00000043936000U)
#define CSL_STD_FW_SMS0_WDTCTRL_1_HSM_CTRL_MMR_HSM_CTRL_MMR_END                                    (0x00000043936fffU)

/* Properties of firewall at slave: SMS0_RAT_1_HSM_RAT_MMRS */
#define CSL_STD_FW_SMS0_RAT_1_HSM_RAT_MMRS_ID                                                      (680U)
#define CSL_STD_FW_SMS0_RAT_1_HSM_RAT_MMRS_TYPE                                                    (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_RAT_1_HSM_RAT_MMRS_MMR_BASE                                                (0x000000450aa000U)
#define CSL_STD_FW_SMS0_RAT_1_HSM_RAT_MMRS_NUM_REGIONS                                             (1U)
#define CSL_STD_FW_SMS0_RAT_1_HSM_RAT_MMRS_NUM_PRIV_IDS_PER_REGION                                 (3U)
#define CSL_STD_FW_SMS0_RAT_1_HSM_RAT_MMRS_HSM_RAT_MMRS_START                                      (0x00000043a00000U)
#define CSL_STD_FW_SMS0_RAT_1_HSM_RAT_MMRS_HSM_RAT_MMRS_END                                        (0x00000043a00fffU)

/* Properties of firewall at slave: SMS0_HSM_ECC_AGGR_0_HSM_ECC_REGS */
#define CSL_STD_FW_SMS0_HSM_ECC_AGGR_0_HSM_ECC_REGS_ID                                             (690U)
#define CSL_STD_FW_SMS0_HSM_ECC_AGGR_0_HSM_ECC_REGS_TYPE                                           (CSL_FW_SECURITY)
#define CSL_STD_FW_SMS0_HSM_ECC_AGGR_0_HSM_ECC_REGS_MMR_BASE                                       (0x000000450ac800U)
#define CSL_STD_FW_SMS0_HSM_ECC_AGGR_0_HSM_ECC_REGS_NUM_REGIONS                                    (2U)
#define CSL_STD_FW_SMS0_HSM_ECC_AGGR_0_HSM_ECC_REGS_NUM_PRIV_IDS_PER_REGION                        (3U)
#define CSL_STD_FW_SMS0_HSM_ECC_AGGR_0_HSM_ECC_REGS_HSM_ECC_START                                  (0x00000043701000U)
#define CSL_STD_FW_SMS0_HSM_ECC_AGGR_0_HSM_ECC_REGS_HSM_ECC_END                                    (0x000000437013ffU)

/* Channelized Firewall Definitions */

/* Properties of channelized firewall at: PSC0_VBUS */
#define CSL_CH_FW_PSC0_VBUS_ID                                                                     (128U)
#define CSL_CH_FW_PSC0_VBUS_TYPE                                                                   (CSL_FW_CHANNEL)
#define CSL_CH_FW_PSC0_VBUS_MMR_BASE                                                               (0x00000045020000U)
#define CSL_CH_FW_PSC0_VBUS_NUM_REGIONS                                                            (5U)
#define CSL_CH_FW_PSC0_VBUS_NUM_PRIV_IDS_PER_REGION                                                (3U)
#define CSL_CH_FW_PSC0_VBUS_SMS_COMMON_START                                                       (0x0000000000400000U)
#define CSL_CH_FW_PSC0_VBUS_SMS_COMMON_END                                                         (0x0000000000400004U)
#define CSL_CH_FW_PSC0_VBUS_SMS_COMMON_OFFSET_FIRST_RES                                            (0x0000000000000000U)
#define CSL_CH_FW_PSC0_VBUS_SMS_COMMON_NUM_CHANNELS                                                (1U)
#define CSL_CH_FW_PSC0_VBUS_SMS_COMMON_CHANNEL_SIZE                                                (4U)
#define CSL_CH_FW_PSC0_VBUS_TIFS_START                                                             (0x0000000000400000U)
#define CSL_CH_FW_PSC0_VBUS_TIFS_END                                                               (0x0000000000400004U)
#define CSL_CH_FW_PSC0_VBUS_TIFS_OFFSET_FIRST_RES                                                  (0x0000000000000020U)
#define CSL_CH_FW_PSC0_VBUS_TIFS_NUM_CHANNELS                                                      (1U)
#define CSL_CH_FW_PSC0_VBUS_TIFS_CHANNEL_SIZE                                                      (4U)
#define CSL_CH_FW_PSC0_VBUS_HSM_START                                                              (0x0000000000400000U)
#define CSL_CH_FW_PSC0_VBUS_HSM_END                                                                (0x0000000000400004U)
#define CSL_CH_FW_PSC0_VBUS_HSM_OFFSET_FIRST_RES                                                   (0x0000000000000040U)
#define CSL_CH_FW_PSC0_VBUS_HSM_NUM_CHANNELS                                                       (1U)
#define CSL_CH_FW_PSC0_VBUS_HSM_CHANNEL_SIZE                                                       (4U)
#define CSL_CH_FW_PSC0_VBUS_SA3UL_START                                                            (0x0000000000400000U)
#define CSL_CH_FW_PSC0_VBUS_SA3UL_END                                                              (0x0000000000400004U)
#define CSL_CH_FW_PSC0_VBUS_SA3UL_OFFSET_FIRST_RES                                                 (0x0000000000000060U)
#define CSL_CH_FW_PSC0_VBUS_SA3UL_NUM_CHANNELS                                                     (1U)
#define CSL_CH_FW_PSC0_VBUS_SA3UL_CHANNEL_SIZE                                                     (4U)
#define CSL_CH_FW_PSC0_VBUS_HSM_ISO_START                                                          (0x0000000000400000U)
#define CSL_CH_FW_PSC0_VBUS_HSM_ISO_END                                                            (0x0000000000400004U)
#define CSL_CH_FW_PSC0_VBUS_HSM_ISO_OFFSET_FIRST_RES                                               (0x0000000000000080U)
#define CSL_CH_FW_PSC0_VBUS_HSM_ISO_NUM_CHANNELS                                                   (1U)
#define CSL_CH_FW_PSC0_VBUS_HSM_ISO_CHANNEL_SIZE                                                   (4U)

#ifdef __cplusplus
}
#endif
#endif /* CSLR_SOC_FW_H_ */

