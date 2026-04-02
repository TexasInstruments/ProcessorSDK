/********************************************************************
*
* SOC ISC PROPERTIES. header file
*
* Copyright (C) 2015-2024 Texas Instruments Incorporated.
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
#ifndef CSLR_SOC_ISC_H_
#define CSLR_SOC_ISC_H_

#include <ti/csl/cslr.h>
#include <ti/csl/tistdtypes.h>
#ifdef __cplusplus
extern "C"
{
#endif

/*
* Auto-generated CSL definitions for SoC ISC Instances:
*/

#define CSL_ISC_DEFAULT                                                                            (0U)
#define CSL_ISC_CC                                                                                 (1U)

/* Properties of firewall at master: WKUP_SMS_LITE0_TIFS_CBASS_0 */
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_I_MST_TYPE                                     (CSL_ISC_DEFAULT)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_I_MST_ID                                       (32U)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_I_MST_DEFAULT_PRIV_ID                          (202U)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_I_MST_MMR_BASE                                 (0x45808000ul)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_I_MST_REGION_COUNT                             (1U)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_D_MST_TYPE                                     (CSL_ISC_DEFAULT)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_D_MST_ID                                       (33U)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_D_MST_DEFAULT_PRIV_ID                          (202U)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_D_MST_MMR_BASE                                 (0x45808400ul)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_D_MST_REGION_COUNT                             (1U)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_S_MST_TYPE                                     (CSL_ISC_DEFAULT)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_S_MST_ID                                       (34U)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_S_MST_DEFAULT_PRIV_ID                          (202U)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_S_MST_MMR_BASE                                 (0x45808800ul)
#define CSL_ISC_WKUP_SMS_LITE0_TIFS_CBASS_0_IQUASAR_S_MST_REGION_COUNT                             (1U)

/* Properties of firewall at master: COMPUTE_CLUSTER0 */
#define CSL_ISC_COMPUTE_CLUSTER0_A53_DUAL_WRAP_CBA_AXI_R_TYPE                                      (CSL_ISC_DEFAULT)
#define CSL_ISC_COMPUTE_CLUSTER0_A53_DUAL_WRAP_CBA_AXI_R_ID                                        (128U)
#define CSL_ISC_COMPUTE_CLUSTER0_A53_DUAL_WRAP_CBA_AXI_R_DEFAULT_PRIV_ID                           (0U)
#define CSL_ISC_COMPUTE_CLUSTER0_A53_DUAL_WRAP_CBA_AXI_R_MMR_BASE                                  (0x45820000ul)
#define CSL_ISC_COMPUTE_CLUSTER0_A53_DUAL_WRAP_CBA_AXI_R_REGION_COUNT                              (0U)
#define CSL_ISC_COMPUTE_CLUSTER0_A53_DUAL_WRAP_CBA_AXI_W_TYPE                                      (CSL_ISC_DEFAULT)
#define CSL_ISC_COMPUTE_CLUSTER0_A53_DUAL_WRAP_CBA_AXI_W_ID                                        (130U)
#define CSL_ISC_COMPUTE_CLUSTER0_A53_DUAL_WRAP_CBA_AXI_W_DEFAULT_PRIV_ID                           (4U)
#define CSL_ISC_COMPUTE_CLUSTER0_A53_DUAL_WRAP_CBA_AXI_W_MMR_BASE                                  (0x45820800ul)
#define CSL_ISC_COMPUTE_CLUSTER0_A53_DUAL_WRAP_CBA_AXI_W_REGION_COUNT                              (1U)

/* Properties of firewall at master: USB0 */
#define CSL_ISC_USB0_MSTR0_TYPE                                                                    (CSL_ISC_DEFAULT)
#define CSL_ISC_USB0_MSTR0_ID                                                                      (128U)
#define CSL_ISC_USB0_MSTR0_DEFAULT_PRIV_ID                                                         (0U)
#define CSL_ISC_USB0_MSTR0_MMR_BASE                                                                (0x45820000ul)
#define CSL_ISC_USB0_MSTR0_REGION_COUNT                                                            (0U)
#define CSL_ISC_USB0_MSTW0_TYPE                                                                    (CSL_ISC_DEFAULT)
#define CSL_ISC_USB0_MSTW0_ID                                                                      (144U)
#define CSL_ISC_USB0_MSTW0_DEFAULT_PRIV_ID                                                         (155U)
#define CSL_ISC_USB0_MSTW0_MMR_BASE                                                                (0x45824000ul)
#define CSL_ISC_USB0_MSTW0_REGION_COUNT                                                            (1U)

/* Properties of firewall at master: GICSS0 */
#define CSL_ISC_GICSS0_MEM_RD_VBUSM_TYPE                                                           (CSL_ISC_DEFAULT)
#define CSL_ISC_GICSS0_MEM_RD_VBUSM_ID                                                             (128U)
#define CSL_ISC_GICSS0_MEM_RD_VBUSM_DEFAULT_PRIV_ID                                                (0U)
#define CSL_ISC_GICSS0_MEM_RD_VBUSM_MMR_BASE                                                       (0x45820000ul)
#define CSL_ISC_GICSS0_MEM_RD_VBUSM_REGION_COUNT                                                   (0U)
#define CSL_ISC_GICSS0_MEM_WR_VBUSM_TYPE                                                           (CSL_ISC_DEFAULT)
#define CSL_ISC_GICSS0_MEM_WR_VBUSM_ID                                                             (136U)
#define CSL_ISC_GICSS0_MEM_WR_VBUSM_DEFAULT_PRIV_ID                                                (154U)
#define CSL_ISC_GICSS0_MEM_WR_VBUSM_MMR_BASE                                                       (0x45822000ul)
#define CSL_ISC_GICSS0_MEM_WR_VBUSM_REGION_COUNT                                                   (1U)

/* Properties of firewall at master: MMCSD1 */
#define CSL_ISC_MMCSD1_EMMCSDSS_RD_TYPE                                                            (CSL_ISC_DEFAULT)
#define CSL_ISC_MMCSD1_EMMCSDSS_RD_ID                                                              (128U)
#define CSL_ISC_MMCSD1_EMMCSDSS_RD_DEFAULT_PRIV_ID                                                 (0U)
#define CSL_ISC_MMCSD1_EMMCSDSS_RD_MMR_BASE                                                        (0x45820000ul)
#define CSL_ISC_MMCSD1_EMMCSDSS_RD_REGION_COUNT                                                    (0U)
#define CSL_ISC_MMCSD1_EMMCSDSS_WR_TYPE                                                            (CSL_ISC_DEFAULT)
#define CSL_ISC_MMCSD1_EMMCSDSS_WR_ID                                                              (141U)
#define CSL_ISC_MMCSD1_EMMCSDSS_WR_DEFAULT_PRIV_ID                                                 (129U)
#define CSL_ISC_MMCSD1_EMMCSDSS_WR_MMR_BASE                                                        (0x45823400ul)
#define CSL_ISC_MMCSD1_EMMCSDSS_WR_REGION_COUNT                                                    (1U)

/* Properties of firewall at master: MMCSD0 */
#define CSL_ISC_MMCSD0_EMMCSDSS_WR_TYPE                                                            (CSL_ISC_DEFAULT)
#define CSL_ISC_MMCSD0_EMMCSDSS_WR_ID                                                              (128U)
#define CSL_ISC_MMCSD0_EMMCSDSS_WR_DEFAULT_PRIV_ID                                                 (0U)
#define CSL_ISC_MMCSD0_EMMCSDSS_WR_MMR_BASE                                                        (0x45820000ul)
#define CSL_ISC_MMCSD0_EMMCSDSS_WR_REGION_COUNT                                                    (0U)
#define CSL_ISC_MMCSD0_EMMCSDSS_RD_TYPE                                                            (CSL_ISC_DEFAULT)
#define CSL_ISC_MMCSD0_EMMCSDSS_RD_ID                                                              (138U)
#define CSL_ISC_MMCSD0_EMMCSDSS_RD_DEFAULT_PRIV_ID                                                 (128U)
#define CSL_ISC_MMCSD0_EMMCSDSS_RD_MMR_BASE                                                        (0x45822800ul)
#define CSL_ISC_MMCSD0_EMMCSDSS_RD_REGION_COUNT                                                    (1U)

/* Properties of firewall at master: MMCSD2 */
#define CSL_ISC_MMCSD2_EMMCSDSS_RD_TYPE                                                            (CSL_ISC_DEFAULT)
#define CSL_ISC_MMCSD2_EMMCSDSS_RD_ID                                                              (128U)
#define CSL_ISC_MMCSD2_EMMCSDSS_RD_DEFAULT_PRIV_ID                                                 (0U)
#define CSL_ISC_MMCSD2_EMMCSDSS_RD_MMR_BASE                                                        (0x45820000ul)
#define CSL_ISC_MMCSD2_EMMCSDSS_RD_REGION_COUNT                                                    (0U)
#define CSL_ISC_MMCSD2_EMMCSDSS_WR_TYPE                                                            (CSL_ISC_DEFAULT)
#define CSL_ISC_MMCSD2_EMMCSDSS_WR_ID                                                              (142U)
#define CSL_ISC_MMCSD2_EMMCSDSS_WR_DEFAULT_PRIV_ID                                                 (130U)
#define CSL_ISC_MMCSD2_EMMCSDSS_WR_MMR_BASE                                                        (0x45823800ul)
#define CSL_ISC_MMCSD2_EMMCSDSS_WR_REGION_COUNT                                                    (1U)

/* Properties of firewall at master: USB1 */
#define CSL_ISC_USB1_MSTW0_TYPE                                                                    (CSL_ISC_DEFAULT)
#define CSL_ISC_USB1_MSTW0_ID                                                                      (128U)
#define CSL_ISC_USB1_MSTW0_DEFAULT_PRIV_ID                                                         (0U)
#define CSL_ISC_USB1_MSTW0_MMR_BASE                                                                (0x45820000ul)
#define CSL_ISC_USB1_MSTW0_REGION_COUNT                                                            (0U)
#define CSL_ISC_USB1_MSTR0_TYPE                                                                    (CSL_ISC_DEFAULT)
#define CSL_ISC_USB1_MSTR0_ID                                                                      (146U)
#define CSL_ISC_USB1_MSTR0_DEFAULT_PRIV_ID                                                         (156U)
#define CSL_ISC_USB1_MSTR0_MMR_BASE                                                                (0x45824800ul)
#define CSL_ISC_USB1_MSTR0_REGION_COUNT                                                            (1U)

/* Properties of firewall at master: LED0 */
#define CSL_ISC_LED0_VBUSP_TYPE                                                                    (CSL_ISC_DEFAULT)
#define CSL_ISC_LED0_VBUSP_ID                                                                      (133U)
#define CSL_ISC_LED0_VBUSP_DEFAULT_PRIV_ID                                                         (176U)
#define CSL_ISC_LED0_VBUSP_MMR_BASE                                                                (0x45821400ul)
#define CSL_ISC_LED0_VBUSP_REGION_COUNT                                                            (1U)

/* Properties of firewall at master: DSS0 */
#define CSL_ISC_DSS0_VBUSM_DMA_TYPE                                                                (CSL_ISC_DEFAULT)
#define CSL_ISC_DSS0_VBUSM_DMA_ID                                                                  (148U)
#define CSL_ISC_DSS0_VBUSM_DMA_DEFAULT_PRIV_ID                                                     (173U)
#define CSL_ISC_DSS0_VBUSM_DMA_MMR_BASE                                                            (0x45825000ul)
#define CSL_ISC_DSS0_VBUSM_DMA_REGION_COUNT                                                        (2U)

/* Properties of firewall at master: DEBUGSS_WRAP0 */
#define CSL_ISC_DEBUGSS_WRAP0_VBUSMR_TYPE                                                          (CSL_ISC_DEFAULT)
#define CSL_ISC_DEBUGSS_WRAP0_VBUSMR_ID                                                            (224U)
#define CSL_ISC_DEBUGSS_WRAP0_VBUSMR_DEFAULT_PRIV_ID                                               (0U)
#define CSL_ISC_DEBUGSS_WRAP0_VBUSMR_MMR_BASE                                                      (0x45838000ul)
#define CSL_ISC_DEBUGSS_WRAP0_VBUSMR_REGION_COUNT                                                  (0U)
#define CSL_ISC_DEBUGSS_WRAP0_VBUSMW_TYPE                                                          (CSL_ISC_DEFAULT)
#define CSL_ISC_DEBUGSS_WRAP0_VBUSMW_ID                                                            (230U)
#define CSL_ISC_DEBUGSS_WRAP0_VBUSMW_DEFAULT_PRIV_ID                                               (177U)
#define CSL_ISC_DEBUGSS_WRAP0_VBUSMW_MMR_BASE                                                      (0x45839800ul)
#define CSL_ISC_DEBUGSS_WRAP0_VBUSMW_REGION_COUNT                                                  (1U)

#ifdef __cplusplus
}
#endif
#endif /* CSLR_SOC_ISC_H_ */

