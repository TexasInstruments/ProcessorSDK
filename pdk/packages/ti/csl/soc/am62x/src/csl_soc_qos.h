/********************************************************************
*
* SOC QOS PROPERTIES. header file
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
#ifndef CSLR_SOC_QOS_H_
#define CSLR_SOC_QOS_H_

#include <ti/csl/cslr.h>
#include <ti/csl/tistdtypes.h>
#ifdef __cplusplus
extern "C"
{
#endif

/*
* Auto-generated CSL definitions for SoC QOS Instances:
*/


/* Properties of QOS in: ICSSM0 */
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP0_MMR_BASE                                                     (0x45d04000ul)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP0_CHANNEL_COUNT                                                (1U)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP0_QOS_CAPABLE                                                  (0U)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP0_VIRTID_CAPABLE                                               (0U)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP0_ORDERID_CAPABLE                                              (1U)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP0_EPRIORITY_CAPABLE                                            (1U)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP0_ATYPE_CAPABLE                                                (0U)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP0_ASEL_CAPABLE                                                 (1U)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP1_MMR_BASE                                                     (0x45d04400ul)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP1_CHANNEL_COUNT                                                (1U)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP1_QOS_CAPABLE                                                  (0U)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP1_VIRTID_CAPABLE                                               (0U)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP1_ORDERID_CAPABLE                                              (1U)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP1_EPRIORITY_CAPABLE                                            (1U)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP1_ATYPE_CAPABLE                                                (0U)
#define CSL_QOS_ICSSM0_PR1_MST_VBUSP1_ASEL_CAPABLE                                                 (1U)

/* Properties of QOS in: WKUP_R5FSS0 */
#define CSL_QOS_WKUP_R5FSS0_CPU0_RMST_MMR_BASE                                                     (0x45d14000ul)
#define CSL_QOS_WKUP_R5FSS0_CPU0_RMST_CHANNEL_COUNT                                                (1U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_RMST_QOS_CAPABLE                                                  (0U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_RMST_VIRTID_CAPABLE                                               (0U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_RMST_ORDERID_CAPABLE                                              (1U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_RMST_EPRIORITY_CAPABLE                                            (1U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_RMST_ATYPE_CAPABLE                                                (0U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_RMST_ASEL_CAPABLE                                                 (1U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_WMST_MMR_BASE                                                     (0x45d14400ul)
#define CSL_QOS_WKUP_R5FSS0_CPU0_WMST_CHANNEL_COUNT                                                (1U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_WMST_QOS_CAPABLE                                                  (0U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_WMST_VIRTID_CAPABLE                                               (0U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_WMST_ORDERID_CAPABLE                                              (1U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_WMST_EPRIORITY_CAPABLE                                            (1U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_WMST_ATYPE_CAPABLE                                                (0U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_WMST_ASEL_CAPABLE                                                 (1U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_PMST_MMR_BASE                                                     (0x45d14800ul)
#define CSL_QOS_WKUP_R5FSS0_CPU0_PMST_CHANNEL_COUNT                                                (1U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_PMST_QOS_CAPABLE                                                  (0U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_PMST_VIRTID_CAPABLE                                               (0U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_PMST_ORDERID_CAPABLE                                              (1U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_PMST_EPRIORITY_CAPABLE                                            (1U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_PMST_ATYPE_CAPABLE                                                (0U)
#define CSL_QOS_WKUP_R5FSS0_CPU0_PMST_ASEL_CAPABLE                                                 (1U)

/* Properties of QOS in: MCU_M4FSS0 */
#define CSL_QOS_MCU_M4FSS0_VBUSP_M_MMR_BASE                                                        (0x45d18000ul)
#define CSL_QOS_MCU_M4FSS0_VBUSP_M_CHANNEL_COUNT                                                   (1U)
#define CSL_QOS_MCU_M4FSS0_VBUSP_M_QOS_CAPABLE                                                     (0U)
#define CSL_QOS_MCU_M4FSS0_VBUSP_M_VIRTID_CAPABLE                                                  (0U)
#define CSL_QOS_MCU_M4FSS0_VBUSP_M_ORDERID_CAPABLE                                                 (0U)
#define CSL_QOS_MCU_M4FSS0_VBUSP_M_EPRIORITY_CAPABLE                                               (1U)
#define CSL_QOS_MCU_M4FSS0_VBUSP_M_ATYPE_CAPABLE                                                   (0U)
#define CSL_QOS_MCU_M4FSS0_VBUSP_M_ASEL_CAPABLE                                                    (1U)

/* Properties of QOS in: COMPUTE_CLUSTER0 */
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_R_MMR_BASE                                  (0x45d20400ul)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_R_CHANNEL_COUNT                             (1U)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_R_QOS_CAPABLE                               (0U)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_R_VIRTID_CAPABLE                            (0U)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_R_ORDERID_CAPABLE                           (1U)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_R_EPRIORITY_CAPABLE                         (1U)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_R_ATYPE_CAPABLE                             (0U)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_R_ASEL_CAPABLE                              (1U)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_W_MMR_BASE                                  (0x45d20800ul)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_W_CHANNEL_COUNT                             (1U)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_W_QOS_CAPABLE                               (0U)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_W_VIRTID_CAPABLE                            (0U)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_W_ORDERID_CAPABLE                           (1U)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_W_EPRIORITY_CAPABLE                         (1U)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_W_ATYPE_CAPABLE                             (0U)
#define CSL_QOS_COMPUTE_CLUSTER0_A53_QUAD_WRAP_CBA_AXI_W_ASEL_CAPABLE                              (1U)

/* Properties of QOS in: GPU0 */
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_W_MMR_BASE                                                     (0x45d20c00ul)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_W_CHANNEL_COUNT                                                (6U)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_W_QOS_CAPABLE                                                  (0U)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_W_VIRTID_CAPABLE                                               (0U)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_W_ORDERID_CAPABLE                                              (1U)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_W_EPRIORITY_CAPABLE                                            (1U)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_W_ATYPE_CAPABLE                                                (0U)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_W_ASEL_CAPABLE                                                 (1U)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_R_MMR_BASE                                                     (0x45d21000ul)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_R_CHANNEL_COUNT                                                (6U)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_R_QOS_CAPABLE                                                  (0U)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_R_VIRTID_CAPABLE                                               (0U)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_R_ORDERID_CAPABLE                                              (1U)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_R_EPRIORITY_CAPABLE                                            (1U)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_R_ATYPE_CAPABLE                                                (0U)
#define CSL_QOS_GPU0_K3_GPU_M_VBUSM_R_ASEL_CAPABLE                                                 (1U)

/* Properties of QOS in: DEBUGSS_WRAP0 */
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMW_MMR_BASE                                                      (0x45d21800ul)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMW_CHANNEL_COUNT                                                 (1U)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMW_QOS_CAPABLE                                                   (0U)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMW_VIRTID_CAPABLE                                                (0U)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMW_ORDERID_CAPABLE                                               (1U)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMW_EPRIORITY_CAPABLE                                             (1U)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMW_ATYPE_CAPABLE                                                 (0U)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMW_ASEL_CAPABLE                                                  (1U)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMR_MMR_BASE                                                      (0x45d21c00ul)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMR_CHANNEL_COUNT                                                 (1U)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMR_QOS_CAPABLE                                                   (0U)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMR_VIRTID_CAPABLE                                                (0U)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMR_ORDERID_CAPABLE                                               (1U)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMR_EPRIORITY_CAPABLE                                             (1U)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMR_ATYPE_CAPABLE                                                 (0U)
#define CSL_QOS_DEBUGSS_WRAP0_VBUSMR_ASEL_CAPABLE                                                  (1U)

/* Properties of QOS in: GICSS0 */
#define CSL_QOS_GICSS0_MEM_WR_VBUSM_MMR_BASE                                                       (0x45d22000ul)
#define CSL_QOS_GICSS0_MEM_WR_VBUSM_CHANNEL_COUNT                                                  (1U)
#define CSL_QOS_GICSS0_MEM_WR_VBUSM_QOS_CAPABLE                                                    (0U)
#define CSL_QOS_GICSS0_MEM_WR_VBUSM_VIRTID_CAPABLE                                                 (0U)
#define CSL_QOS_GICSS0_MEM_WR_VBUSM_ORDERID_CAPABLE                                                (1U)
#define CSL_QOS_GICSS0_MEM_WR_VBUSM_EPRIORITY_CAPABLE                                              (1U)
#define CSL_QOS_GICSS0_MEM_WR_VBUSM_ATYPE_CAPABLE                                                  (0U)
#define CSL_QOS_GICSS0_MEM_WR_VBUSM_ASEL_CAPABLE                                                   (1U)
#define CSL_QOS_GICSS0_MEM_RD_VBUSM_MMR_BASE                                                       (0x45d22400ul)
#define CSL_QOS_GICSS0_MEM_RD_VBUSM_CHANNEL_COUNT                                                  (1U)
#define CSL_QOS_GICSS0_MEM_RD_VBUSM_QOS_CAPABLE                                                    (0U)
#define CSL_QOS_GICSS0_MEM_RD_VBUSM_VIRTID_CAPABLE                                                 (0U)
#define CSL_QOS_GICSS0_MEM_RD_VBUSM_ORDERID_CAPABLE                                                (1U)
#define CSL_QOS_GICSS0_MEM_RD_VBUSM_EPRIORITY_CAPABLE                                              (1U)
#define CSL_QOS_GICSS0_MEM_RD_VBUSM_ATYPE_CAPABLE                                                  (0U)
#define CSL_QOS_GICSS0_MEM_RD_VBUSM_ASEL_CAPABLE                                                   (1U)

/* Properties of QOS in: MMCSD0 */
#define CSL_QOS_MMCSD0_EMMCSDSS_RD_MMR_BASE                                                        (0x45d22800ul)
#define CSL_QOS_MMCSD0_EMMCSDSS_RD_CHANNEL_COUNT                                                   (1U)
#define CSL_QOS_MMCSD0_EMMCSDSS_RD_QOS_CAPABLE                                                     (0U)
#define CSL_QOS_MMCSD0_EMMCSDSS_RD_VIRTID_CAPABLE                                                  (0U)
#define CSL_QOS_MMCSD0_EMMCSDSS_RD_ORDERID_CAPABLE                                                 (1U)
#define CSL_QOS_MMCSD0_EMMCSDSS_RD_EPRIORITY_CAPABLE                                               (1U)
#define CSL_QOS_MMCSD0_EMMCSDSS_RD_ATYPE_CAPABLE                                                   (0U)
#define CSL_QOS_MMCSD0_EMMCSDSS_RD_ASEL_CAPABLE                                                    (1U)
#define CSL_QOS_MMCSD0_EMMCSDSS_WR_MMR_BASE                                                        (0x45d22c00ul)
#define CSL_QOS_MMCSD0_EMMCSDSS_WR_CHANNEL_COUNT                                                   (1U)
#define CSL_QOS_MMCSD0_EMMCSDSS_WR_QOS_CAPABLE                                                     (0U)
#define CSL_QOS_MMCSD0_EMMCSDSS_WR_VIRTID_CAPABLE                                                  (0U)
#define CSL_QOS_MMCSD0_EMMCSDSS_WR_ORDERID_CAPABLE                                                 (1U)
#define CSL_QOS_MMCSD0_EMMCSDSS_WR_EPRIORITY_CAPABLE                                               (1U)
#define CSL_QOS_MMCSD0_EMMCSDSS_WR_ATYPE_CAPABLE                                                   (0U)
#define CSL_QOS_MMCSD0_EMMCSDSS_WR_ASEL_CAPABLE                                                    (1U)

/* Properties of QOS in: MMCSD1 */
#define CSL_QOS_MMCSD1_EMMCSDSS_RD_MMR_BASE                                                        (0x45d23000ul)
#define CSL_QOS_MMCSD1_EMMCSDSS_RD_CHANNEL_COUNT                                                   (1U)
#define CSL_QOS_MMCSD1_EMMCSDSS_RD_QOS_CAPABLE                                                     (0U)
#define CSL_QOS_MMCSD1_EMMCSDSS_RD_VIRTID_CAPABLE                                                  (0U)
#define CSL_QOS_MMCSD1_EMMCSDSS_RD_ORDERID_CAPABLE                                                 (1U)
#define CSL_QOS_MMCSD1_EMMCSDSS_RD_EPRIORITY_CAPABLE                                               (1U)
#define CSL_QOS_MMCSD1_EMMCSDSS_RD_ATYPE_CAPABLE                                                   (0U)
#define CSL_QOS_MMCSD1_EMMCSDSS_RD_ASEL_CAPABLE                                                    (1U)
#define CSL_QOS_MMCSD1_EMMCSDSS_WR_MMR_BASE                                                        (0x45d23400ul)
#define CSL_QOS_MMCSD1_EMMCSDSS_WR_CHANNEL_COUNT                                                   (1U)
#define CSL_QOS_MMCSD1_EMMCSDSS_WR_QOS_CAPABLE                                                     (0U)
#define CSL_QOS_MMCSD1_EMMCSDSS_WR_VIRTID_CAPABLE                                                  (0U)
#define CSL_QOS_MMCSD1_EMMCSDSS_WR_ORDERID_CAPABLE                                                 (1U)
#define CSL_QOS_MMCSD1_EMMCSDSS_WR_EPRIORITY_CAPABLE                                               (1U)
#define CSL_QOS_MMCSD1_EMMCSDSS_WR_ATYPE_CAPABLE                                                   (0U)
#define CSL_QOS_MMCSD1_EMMCSDSS_WR_ASEL_CAPABLE                                                    (1U)

/* Properties of QOS in: MMCSD2 */
#define CSL_QOS_MMCSD2_EMMCSDSS_WR_MMR_BASE                                                        (0x45d23800ul)
#define CSL_QOS_MMCSD2_EMMCSDSS_WR_CHANNEL_COUNT                                                   (1U)
#define CSL_QOS_MMCSD2_EMMCSDSS_WR_QOS_CAPABLE                                                     (0U)
#define CSL_QOS_MMCSD2_EMMCSDSS_WR_VIRTID_CAPABLE                                                  (0U)
#define CSL_QOS_MMCSD2_EMMCSDSS_WR_ORDERID_CAPABLE                                                 (1U)
#define CSL_QOS_MMCSD2_EMMCSDSS_WR_EPRIORITY_CAPABLE                                               (1U)
#define CSL_QOS_MMCSD2_EMMCSDSS_WR_ATYPE_CAPABLE                                                   (0U)
#define CSL_QOS_MMCSD2_EMMCSDSS_WR_ASEL_CAPABLE                                                    (1U)
#define CSL_QOS_MMCSD2_EMMCSDSS_RD_MMR_BASE                                                        (0x45d23c00ul)
#define CSL_QOS_MMCSD2_EMMCSDSS_RD_CHANNEL_COUNT                                                   (1U)
#define CSL_QOS_MMCSD2_EMMCSDSS_RD_QOS_CAPABLE                                                     (0U)
#define CSL_QOS_MMCSD2_EMMCSDSS_RD_VIRTID_CAPABLE                                                  (0U)
#define CSL_QOS_MMCSD2_EMMCSDSS_RD_ORDERID_CAPABLE                                                 (1U)
#define CSL_QOS_MMCSD2_EMMCSDSS_RD_EPRIORITY_CAPABLE                                               (1U)
#define CSL_QOS_MMCSD2_EMMCSDSS_RD_ATYPE_CAPABLE                                                   (0U)
#define CSL_QOS_MMCSD2_EMMCSDSS_RD_ASEL_CAPABLE                                                    (1U)

/* Properties of QOS in: USB0 */
#define CSL_QOS_USB0_MSTW0_MMR_BASE                                                                (0x45d24000ul)
#define CSL_QOS_USB0_MSTW0_CHANNEL_COUNT                                                           (1U)
#define CSL_QOS_USB0_MSTW0_QOS_CAPABLE                                                             (0U)
#define CSL_QOS_USB0_MSTW0_VIRTID_CAPABLE                                                          (0U)
#define CSL_QOS_USB0_MSTW0_ORDERID_CAPABLE                                                         (1U)
#define CSL_QOS_USB0_MSTW0_EPRIORITY_CAPABLE                                                       (1U)
#define CSL_QOS_USB0_MSTW0_ATYPE_CAPABLE                                                           (0U)
#define CSL_QOS_USB0_MSTW0_ASEL_CAPABLE                                                            (1U)
#define CSL_QOS_USB0_MSTR0_MMR_BASE                                                                (0x45d24400ul)
#define CSL_QOS_USB0_MSTR0_CHANNEL_COUNT                                                           (1U)
#define CSL_QOS_USB0_MSTR0_QOS_CAPABLE                                                             (0U)
#define CSL_QOS_USB0_MSTR0_VIRTID_CAPABLE                                                          (0U)
#define CSL_QOS_USB0_MSTR0_ORDERID_CAPABLE                                                         (1U)
#define CSL_QOS_USB0_MSTR0_EPRIORITY_CAPABLE                                                       (1U)
#define CSL_QOS_USB0_MSTR0_ATYPE_CAPABLE                                                           (0U)
#define CSL_QOS_USB0_MSTR0_ASEL_CAPABLE                                                            (1U)

/* Properties of QOS in: USB1 */
#define CSL_QOS_USB1_MSTR0_MMR_BASE                                                                (0x45d24800ul)
#define CSL_QOS_USB1_MSTR0_CHANNEL_COUNT                                                           (1U)
#define CSL_QOS_USB1_MSTR0_QOS_CAPABLE                                                             (0U)
#define CSL_QOS_USB1_MSTR0_VIRTID_CAPABLE                                                          (0U)
#define CSL_QOS_USB1_MSTR0_ORDERID_CAPABLE                                                         (1U)
#define CSL_QOS_USB1_MSTR0_EPRIORITY_CAPABLE                                                       (1U)
#define CSL_QOS_USB1_MSTR0_ATYPE_CAPABLE                                                           (0U)
#define CSL_QOS_USB1_MSTR0_ASEL_CAPABLE                                                            (1U)
#define CSL_QOS_USB1_MSTW0_MMR_BASE                                                                (0x45d24c00ul)
#define CSL_QOS_USB1_MSTW0_CHANNEL_COUNT                                                           (1U)
#define CSL_QOS_USB1_MSTW0_QOS_CAPABLE                                                             (0U)
#define CSL_QOS_USB1_MSTW0_VIRTID_CAPABLE                                                          (0U)
#define CSL_QOS_USB1_MSTW0_ORDERID_CAPABLE                                                         (1U)
#define CSL_QOS_USB1_MSTW0_EPRIORITY_CAPABLE                                                       (1U)
#define CSL_QOS_USB1_MSTW0_ATYPE_CAPABLE                                                           (0U)
#define CSL_QOS_USB1_MSTW0_ASEL_CAPABLE                                                            (1U)

/* Properties of QOS in: DSS0 */
#define CSL_QOS_DSS0_VBUSM_DMA_MMR_BASE                                                            (0x45d25000ul)
#define CSL_QOS_DSS0_VBUSM_DMA_CHANNEL_COUNT                                                       (4U)
#define CSL_QOS_DSS0_VBUSM_DMA_QOS_CAPABLE                                                         (0U)
#define CSL_QOS_DSS0_VBUSM_DMA_VIRTID_CAPABLE                                                      (0U)
#define CSL_QOS_DSS0_VBUSM_DMA_ORDERID_CAPABLE                                                     (1U)
#define CSL_QOS_DSS0_VBUSM_DMA_EPRIORITY_CAPABLE                                                   (0U)
#define CSL_QOS_DSS0_VBUSM_DMA_ATYPE_CAPABLE                                                       (0U)
#define CSL_QOS_DSS0_VBUSM_DMA_ASEL_CAPABLE                                                        (1U)

/* Properties of QOS in: SA3_SS0 */
#define CSL_QOS_SA3_SS0_CTXCACH_EXT_DMA_MMR_BASE                                                   (0x45d25400ul)
#define CSL_QOS_SA3_SS0_CTXCACH_EXT_DMA_CHANNEL_COUNT                                              (1U)
#define CSL_QOS_SA3_SS0_CTXCACH_EXT_DMA_QOS_CAPABLE                                                (0U)
#define CSL_QOS_SA3_SS0_CTXCACH_EXT_DMA_VIRTID_CAPABLE                                             (0U)
#define CSL_QOS_SA3_SS0_CTXCACH_EXT_DMA_ORDERID_CAPABLE                                            (1U)
#define CSL_QOS_SA3_SS0_CTXCACH_EXT_DMA_EPRIORITY_CAPABLE                                          (1U)
#define CSL_QOS_SA3_SS0_CTXCACH_EXT_DMA_ATYPE_CAPABLE                                              (0U)
#define CSL_QOS_SA3_SS0_CTXCACH_EXT_DMA_ASEL_CAPABLE                                               (1U)

#ifdef __cplusplus
}
#endif
#endif /* CSLR_SOC_QOS_H_ */

