/********************************************************************
 * Copyright (C) 2023 Texas Instruments Incorporated.
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
 *  Name        : cslr_fss.h
 *  VPVERSION   : 3.0.362 - 2023.09.22.21.39.55
 *  VPREV       : 2.0.2.00
*/
#ifndef CSLR_FSS_H_
#define CSLR_FSS_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <ti/csl/cslr.h>
#include <stdint.h>

/**************************************************************************
* Module Base Offset Values
**************************************************************************/

#define CSL_FSS_DAT_REG0_REGS_BASE                                             (0x00000000U)
#define CSL_FSS_DAT_REG1_REGS_BASE                                             (0x00000000U)
#define CSL_FSS_DAT_REG3_REGS_BASE                                             (0x00000000U)
#define CSL_FSS_ECC_AGGR_REGS_BASE                                             (0x00000000U)
#define CSL_FSS_FSAS_GENREGS_REGS_BASE                                         (0x00000000U)
#define CSL_FSS_GENREGS_REGS_BASE                                              (0x00000000U)
#define CSL_FSS_OSPI_CFG_REGS_BASE                                             (0x00000000U)
#define CSL_FSS_OSPI_FLASH_CFG_REGS_BASE                                       (0x00000000U)


/**************************************************************************
* Hardware Region  : Global Control Registers, offset 0x00000000
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t EOI;                       /* IRQ_EOI */
    volatile uint32_t STATUS_RAW;                /* IRQ_STATUS_RAW */
    volatile uint32_t STATUS;                    /* IRQ_STATUS */
    volatile uint32_t ENABLE_SET;                /* IRQ_ENABLE_SET */
    volatile uint32_t ENABLE_CLR;                /* IRQ_ENABLE_CLR */
} CSL_fss_fsas_genregsRegs_IRQ;


typedef struct {
    volatile uint32_t ECC_RGSTRT;                /* RegionStart */
    volatile uint32_t ECC_RGSIZ;                 /* ECC RegionSize */
} CSL_fss_fsas_genregsRegs_ECC_REGCTRL;


typedef struct {
    volatile uint32_t ECC_BLOCK_ADR;             /* ERR_ECC_BLOCK_ADR */
    volatile uint32_t ECC_TYPE;                  /* ERR_ECC_TYPE */
    volatile uint32_t WRT_TYPE;                  /* ERR_WRT_TYPE */
    volatile uint8_t  Resv_16[4];
} CSL_fss_fsas_genregsRegs_ERR;


typedef struct {
    volatile uint32_t REVISION;                  /* REVISION */
    volatile uint32_t SYSCONFIG;                 /* SYSCONFIG */
    volatile uint8_t  Resv_16[8];
    CSL_fss_fsas_genregsRegs_IRQ IRQ;
    volatile uint8_t  Resv_48[12];
    CSL_fss_fsas_genregsRegs_ECC_REGCTRL ECC_REGCTRL[4];
    volatile uint8_t  Resv_112[32];
    CSL_fss_fsas_genregsRegs_ERR ERR;
} CSL_fss_fsas_genregsRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_FSS_FSAS_GENREGS_REVISION                                          (0x00000000U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG                                         (0x00000004U)
#define CSL_FSS_FSAS_GENREGS_IRQ_EOI                                           (0x00000010U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_RAW                                    (0x00000014U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS                                        (0x00000018U)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_SET                                    (0x0000001CU)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_CLR                                    (0x00000020U)
#define CSL_FSS_FSAS_GENREGS_ECC_REGCTRL_ECC_RGSTRT(ECC_REGCTRL)               (0x00000030U+((ECC_REGCTRL)*0x8U))
#define CSL_FSS_FSAS_GENREGS_ECC_REGCTRL_ECC_RGSIZ(ECC_REGCTRL)                (0x00000034U+((ECC_REGCTRL)*0x8U))
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_BLOCK_ADR                                 (0x00000070U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE                                      (0x00000074U)
#define CSL_FSS_FSAS_GENREGS_ERR_WRT_TYPE                                      (0x00000078U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* EOI */

#define CSL_FSS_FSAS_GENREGS_IRQ_EOI_EOI_VECTOR_MASK                           (0x00000001U)
#define CSL_FSS_FSAS_GENREGS_IRQ_EOI_EOI_VECTOR_SHIFT                          (0x00000000U)
#define CSL_FSS_FSAS_GENREGS_IRQ_EOI_EOI_VECTOR_MAX                            (0x00000001U)

/* STATUS_RAW */

#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_RAW_ECC_WRITE_NONALIGN_MASK            (0x00000004U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_RAW_ECC_WRITE_NONALIGN_SHIFT           (0x00000002U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_RAW_ECC_WRITE_NONALIGN_MAX             (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_RAW_ECC_ERROR_2BIT_MASK                (0x00000002U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_RAW_ECC_ERROR_2BIT_SHIFT               (0x00000001U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_RAW_ECC_ERROR_2BIT_MAX                 (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_RAW_ECC_ERROR_1BIT_MASK                (0x00000001U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_RAW_ECC_ERROR_1BIT_SHIFT               (0x00000000U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_RAW_ECC_ERROR_1BIT_MAX                 (0x00000001U)

/* STATUS */

#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_ECC_WRITE_NONALIGN_MASK                (0x00000004U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_ECC_WRITE_NONALIGN_SHIFT               (0x00000002U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_ECC_WRITE_NONALIGN_MAX                 (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_ECC_ERROR_2BIT_MASK                    (0x00000002U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_ECC_ERROR_2BIT_SHIFT                   (0x00000001U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_ECC_ERROR_2BIT_MAX                     (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_ECC_ERROR_1BIT_MASK                    (0x00000001U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_ECC_ERROR_1BIT_SHIFT                   (0x00000000U)
#define CSL_FSS_FSAS_GENREGS_IRQ_STATUS_ECC_ERROR_1BIT_MAX                     (0x00000001U)

/* ENABLE_SET */

#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_SET_ECC_WRITE_NONALIGN_MASK            (0x00000004U)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_SET_ECC_WRITE_NONALIGN_SHIFT           (0x00000002U)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_SET_ECC_WRITE_NONALIGN_MAX             (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_SET_ECC_ERROR_2BIT_MASK                (0x00000002U)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_SET_ECC_ERROR_2BIT_SHIFT               (0x00000001U)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_SET_ECC_ERROR_2BIT_MAX                 (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_SET_ECC_ERROR_1BIT_MASK                (0x00000001U)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_SET_ECC_ERROR_1BIT_SHIFT               (0x00000000U)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_SET_ECC_ERROR_1BIT_MAX                 (0x00000001U)

/* ENABLE_CLR */

#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_CLR_ECC_WRITE_NONALIGN_MASK            (0x00000004U)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_CLR_ECC_WRITE_NONALIGN_SHIFT           (0x00000002U)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_CLR_ECC_WRITE_NONALIGN_MAX             (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_CLR_ECC_ERROR_2BIT_MASK                (0x00000002U)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_CLR_ECC_ERROR_2BIT_SHIFT               (0x00000001U)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_CLR_ECC_ERROR_2BIT_MAX                 (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_CLR_ECC_ERROR_1BIT_MASK                (0x00000001U)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_CLR_ECC_ERROR_1BIT_SHIFT               (0x00000000U)
#define CSL_FSS_FSAS_GENREGS_IRQ_ENABLE_CLR_ECC_ERROR_1BIT_MAX                 (0x00000001U)

/* ECC_RGSTRT */

#define CSL_FSS_FSAS_GENREGS_ECC_REGCTRL_ECC_RGSTRT_R_START_MASK               (0x000FFFFFU)
#define CSL_FSS_FSAS_GENREGS_ECC_REGCTRL_ECC_RGSTRT_R_START_SHIFT              (0x00000000U)
#define CSL_FSS_FSAS_GENREGS_ECC_REGCTRL_ECC_RGSTRT_R_START_MAX                (0x000FFFFFU)

/* ECC_RGSIZ */

#define CSL_FSS_FSAS_GENREGS_ECC_REGCTRL_ECC_RGSIZ_R_SIZE_MASK                 (0x000FFFFFU)
#define CSL_FSS_FSAS_GENREGS_ECC_REGCTRL_ECC_RGSIZ_R_SIZE_SHIFT                (0x00000000U)
#define CSL_FSS_FSAS_GENREGS_ECC_REGCTRL_ECC_RGSIZ_R_SIZE_MAX                  (0x000FFFFFU)

/* ECC_BLOCK_ADR */

#define CSL_FSS_FSAS_GENREGS_ERR_ECC_BLOCK_ADR_ECC_ERROR_BLOCK_ADDR_MASK       (0xFFFFFFE0U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_BLOCK_ADR_ECC_ERROR_BLOCK_ADDR_SHIFT      (0x00000005U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_BLOCK_ADR_ECC_ERROR_BLOCK_ADDR_MAX        (0x07FFFFFFU)

/* ECC_TYPE */

#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_VALID_MASK                   (0x80000000U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_VALID_SHIFT                  (0x0000001FU)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_VALID_MAX                    (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_ADR_MASK                     (0x00000020U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_ADR_SHIFT                    (0x00000005U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_ADR_MAX                      (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_MAC_MASK                     (0x00000010U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_MAC_SHIFT                    (0x00000004U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_MAC_MAX                      (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_DA1_MASK                     (0x00000008U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_DA1_SHIFT                    (0x00000003U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_DA1_MAX                      (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_DA0_MASK                     (0x00000004U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_DA0_SHIFT                    (0x00000002U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_DA0_MAX                      (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_DED_MASK                     (0x00000002U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_DED_SHIFT                    (0x00000001U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_DED_MAX                      (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_SEC_MASK                     (0x00000001U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_SEC_SHIFT                    (0x00000000U)
#define CSL_FSS_FSAS_GENREGS_ERR_ECC_TYPE_ECC_ERR_SEC_MAX                      (0x00000001U)

/* WRT_TYPE */

#define CSL_FSS_FSAS_GENREGS_ERR_WRT_TYPE_WRT_ERR_VALID_MASK                   (0x80000000U)
#define CSL_FSS_FSAS_GENREGS_ERR_WRT_TYPE_WRT_ERR_VALID_SHIFT                  (0x0000001FU)
#define CSL_FSS_FSAS_GENREGS_ERR_WRT_TYPE_WRT_ERR_VALID_MAX                    (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_ERR_WRT_TYPE_WRT_ERR_ADR_MASK                     (0x00001000U)
#define CSL_FSS_FSAS_GENREGS_ERR_WRT_TYPE_WRT_ERR_ADR_SHIFT                    (0x0000000CU)
#define CSL_FSS_FSAS_GENREGS_ERR_WRT_TYPE_WRT_ERR_ADR_MAX                      (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_ERR_WRT_TYPE_WRT_ERR_BEN_MASK                     (0x00002000U)
#define CSL_FSS_FSAS_GENREGS_ERR_WRT_TYPE_WRT_ERR_BEN_SHIFT                    (0x0000000DU)
#define CSL_FSS_FSAS_GENREGS_ERR_WRT_TYPE_WRT_ERR_BEN_MAX                      (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_ERR_WRT_TYPE_WRT_ERR_ROUTEID_MASK                 (0x00000FFFU)
#define CSL_FSS_FSAS_GENREGS_ERR_WRT_TYPE_WRT_ERR_ROUTEID_SHIFT                (0x00000000U)
#define CSL_FSS_FSAS_GENREGS_ERR_WRT_TYPE_WRT_ERR_ROUTEID_MAX                  (0x00000FFFU)

/* REVISION */

#define CSL_FSS_FSAS_GENREGS_REVISION_MODID_MASK                               (0xFFFF0000U)
#define CSL_FSS_FSAS_GENREGS_REVISION_MODID_SHIFT                              (0x00000010U)
#define CSL_FSS_FSAS_GENREGS_REVISION_MODID_MAX                                (0x0000FFFFU)

#define CSL_FSS_FSAS_GENREGS_REVISION_REVRTL_MASK                              (0x0000F800U)
#define CSL_FSS_FSAS_GENREGS_REVISION_REVRTL_SHIFT                             (0x0000000BU)
#define CSL_FSS_FSAS_GENREGS_REVISION_REVRTL_MAX                               (0x0000001FU)

#define CSL_FSS_FSAS_GENREGS_REVISION_REVMAJ_MASK                              (0x00000700U)
#define CSL_FSS_FSAS_GENREGS_REVISION_REVMAJ_SHIFT                             (0x00000008U)
#define CSL_FSS_FSAS_GENREGS_REVISION_REVMAJ_MAX                               (0x00000007U)

#define CSL_FSS_FSAS_GENREGS_REVISION_CUSTOM_MASK                              (0x000000C0U)
#define CSL_FSS_FSAS_GENREGS_REVISION_CUSTOM_SHIFT                             (0x00000006U)
#define CSL_FSS_FSAS_GENREGS_REVISION_CUSTOM_MAX                               (0x00000003U)

#define CSL_FSS_FSAS_GENREGS_REVISION_REVMIN_MASK                              (0x0000003FU)
#define CSL_FSS_FSAS_GENREGS_REVISION_REVMIN_SHIFT                             (0x00000000U)
#define CSL_FSS_FSAS_GENREGS_REVISION_REVMIN_MAX                               (0x0000003FU)

/* SYSCONFIG */

#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_DISXIP_MASK                             (0x00000080U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_DISXIP_SHIFT                            (0x00000007U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_DISXIP_MAX                              (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_DISXIP_VAL_FALSE                        (0x0U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_DISXIP_VAL_TRUE                         (0x1U)

#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_OSPI_32B_DISABLE_MODE_MASK              (0x00000100U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_OSPI_32B_DISABLE_MODE_SHIFT             (0x00000008U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_OSPI_32B_DISABLE_MODE_MAX               (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_OSPI_32B_DISABLE_MODE_VAL_FALSE         (0x0U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_OSPI_32B_DISABLE_MODE_VAL_TRUE          (0x1U)

#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_OSPI_DDR_DISABLE_MODE_MASK              (0x00000040U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_OSPI_DDR_DISABLE_MODE_SHIFT             (0x00000006U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_OSPI_DDR_DISABLE_MODE_MAX               (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_OSPI_DDR_DISABLE_MODE_VAL_FALSE         (0x0U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_OSPI_DDR_DISABLE_MODE_VAL_TRUE          (0x1U)

#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_ECC_DISABLE_ADR_MASK                    (0x00000008U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_ECC_DISABLE_ADR_SHIFT                   (0x00000003U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_ECC_DISABLE_ADR_MAX                     (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_ECC_DISABLE_ADR_VAL_FALSE               (0x0U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_ECC_DISABLE_ADR_VAL_TRUE                (0x1U)

#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_ECC_EN_MASK                             (0x00000001U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_ECC_EN_SHIFT                            (0x00000000U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_ECC_EN_MAX                              (0x00000001U)

#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_ECC_EN_VAL_FALSE                        (0x0U)
#define CSL_FSS_FSAS_GENREGS_SYSCONFIG_ECC_EN_VAL_TRUE                         (0x1U)

/**************************************************************************
* Hardware Region  : Global Control Registers, offset 0x00000000
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t REVISION;                  /* REVISION */
} CSL_fss_genregsRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_FSS_GENREGS_REVISION                                               (0x00000000U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* REVISION */

#define CSL_FSS_GENREGS_REVISION_MODID_MASK                                    (0xFFFF0000U)
#define CSL_FSS_GENREGS_REVISION_MODID_SHIFT                                   (0x00000010U)
#define CSL_FSS_GENREGS_REVISION_MODID_MAX                                     (0x0000FFFFU)

#define CSL_FSS_GENREGS_REVISION_REVRTL_MASK                                   (0x0000F800U)
#define CSL_FSS_GENREGS_REVISION_REVRTL_SHIFT                                  (0x0000000BU)
#define CSL_FSS_GENREGS_REVISION_REVRTL_MAX                                    (0x0000001FU)

#define CSL_FSS_GENREGS_REVISION_REVMAJ_MASK                                   (0x00000700U)
#define CSL_FSS_GENREGS_REVISION_REVMAJ_SHIFT                                  (0x00000008U)
#define CSL_FSS_GENREGS_REVISION_REVMAJ_MAX                                    (0x00000007U)

#define CSL_FSS_GENREGS_REVISION_CUSTOM_MASK                                   (0x000000C0U)
#define CSL_FSS_GENREGS_REVISION_CUSTOM_SHIFT                                  (0x00000006U)
#define CSL_FSS_GENREGS_REVISION_CUSTOM_MAX                                    (0x00000003U)

#define CSL_FSS_GENREGS_REVISION_REVMIN_MASK                                   (0x0000003FU)
#define CSL_FSS_GENREGS_REVISION_REVMIN_SHIFT                                  (0x00000000U)
#define CSL_FSS_GENREGS_REVISION_REVMIN_MAX                                    (0x0000003FU)

/**************************************************************************
* Hardware Region  : FSAS data region0
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t HPB_DATA_MEM[1];   /* FSAS data region0 */
} CSL_fss_dat_reg0Regs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_FSS_DAT_REG0_HPB_DATA_MEM(HPB_DATA_MEM)                                (0x00000000U+((HPB_DATA_MEM)*0x4U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* HPB_DATA_MEM */

#define CSL_FSS_DAT_REG0_HPB_DATA_MEM_HPB_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_FSS_DAT_REG0_HPB_DATA_MEM_HPB_DATA_SHIFT                               (0x00000000U)
#define CSL_FSS_DAT_REG0_HPB_DATA_MEM_HPB_DATA_MAX                                 (0xFFFFFFFFU)

/**************************************************************************
* Hardware Region  : FSAS data region1
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t HPB_DATA_MEM[1];   /* FSAS data region1 */
} CSL_fss_dat_reg1Regs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_FSS_DAT_REG1_HPB_DATA_MEM(HPB_DATA_MEM)                                (0x00000000U+((HPB_DATA_MEM)*0x4U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* HPB_DATA_MEM */

#define CSL_FSS_DAT_REG1_HPB_DATA_MEM_HPB_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_FSS_DAT_REG1_HPB_DATA_MEM_HPB_DATA_SHIFT                               (0x00000000U)
#define CSL_FSS_DAT_REG1_HPB_DATA_MEM_HPB_DATA_MAX                                 (0xFFFFFFFFU)

/**************************************************************************
* Hardware Region  : FSAS data region3
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t HPB_DATA_MEM[1];   /* FSAS data region3 */
} CSL_fss_dat_reg3Regs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_FSS_DAT_REG3_HPB_DATA_MEM(HPB_DATA_MEM)                                (0x00000000U+((HPB_DATA_MEM)*0x4U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* HPB_DATA_MEM */

#define CSL_FSS_DAT_REG3_HPB_DATA_MEM_HPB_DATA_MASK                                (0xFFFFFFFFU)
#define CSL_FSS_DAT_REG3_HPB_DATA_MEM_HPB_DATA_SHIFT                               (0x00000000U)
#define CSL_FSS_DAT_REG3_HPB_DATA_MEM_HPB_DATA_MAX                                 (0xFFFFFFFFU)

#ifdef __cplusplus
}
#endif
#endif
