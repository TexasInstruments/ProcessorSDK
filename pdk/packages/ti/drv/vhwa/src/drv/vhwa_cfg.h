/**
 *   Copyright (c) Texas Instruments Incorporated 2018
 *   All rights reserved.
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
 *  \file   vhwa_cfg.h
 *
 *  \brief  File containning defination for common configuration
 *          used across all VHWA modules.
 */

#ifndef VHWA_CFG_H_
#define VHWA_CFG_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* This is needed for memset/memcpy */
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/**
 *  \anchor VHWA_pipeline
 *  \name   Pipeline for VHWA
 *  \brief  Pipeline used by different VHWA modules
 *
 *  @{
 */
#define VHWA_M2M_VISS_HTS_PIPELINE              (0u)
#define VHWA_M2M_LDC_HTS_PIPELINE               (1u)
#define VHWA_M2M_MSC0_HTS_PIPELINE              (2u)
#define VHWA_M2M_MSC1_HTS_PIPELINE              (3u)
#define VHWA_M2M_NF_HTS_PIPELINE                (4u)
#define VHWA_M2M_SP0_HTS_PIPELINE               (5u)
#define VHWA_M2M_SP1_HTS_PIPELINE               (6u)

#define VHWA_M2M_DOF_HTS_PIPELINE               (0u)
#define VHWA_M2M_SDE_HTS_PIPELINE               (1u)

#define VHWA_M2M_PIPELINE_INVALID               ((uint32_t) 0xFFFF0000U)
/** @} */

/**
 *  \anchor VHWA_utcId
 *  \name   UTC ID
 *  \brief  UTC ID for VHWA modules
 *
 *  @{
 */
#if defined (VHWA_VPAC_IP_REV_VPAC3L) || defined(SOC_J722S)
#define VHWA_M2M_MSC_UTC_ID                     (UDMA_UTC_ID_VPAC_TC0)
#define VHWA_M2M_NF_UTC_ID                      (UDMA_UTC_ID_VPAC_TC0)
#define VHWA_M2M_LDC_UTC_ID                     (UDMA_UTC_ID_VPAC_TC0)
#define VHWA_M2M_VISS_UTC_ID                    (UDMA_UTC_ID_VPAC_TC0)

/* Not really required for VPAC, get the compilation going */
#define VHWA_M2M_DOF_UTC_ID                     (UDMA_UTC_ID_DMPAC_TC0)
#define VHWA_M2M_SDE_UTC_ID                     (UDMA_UTC_ID_DMPAC_TC0)

#else
#if defined VHWA_M2M_VPAC_INSTANCE
#if (VHWA_M2M_VPAC_INSTANCE == 0)
    #define VHWA_M2M_MSC_UTC_ID                     (UDMA_UTC_ID_VPAC_TC1)
    #define VHWA_M2M_NF_UTC_ID                      (UDMA_UTC_ID_VPAC_TC1)
    #define VHWA_M2M_LDC_UTC_ID                     (UDMA_UTC_ID_VPAC_TC1)
    #define VHWA_M2M_VISS_UTC_ID                    (UDMA_UTC_ID_VPAC_TC1)
#endif
#endif
#if defined VHWA_M2M_VPAC_INSTANCE
#if (VHWA_M2M_VPAC_INSTANCE == 1)
    #define VHWA_M2M_MSC_UTC_ID                     (UDMA_UTC_ID_VPAC1_TC1)
    #define VHWA_M2M_NF_UTC_ID                      (UDMA_UTC_ID_VPAC1_TC1)
    #define VHWA_M2M_LDC_UTC_ID                     (UDMA_UTC_ID_VPAC1_TC1)
    #define VHWA_M2M_VISS_UTC_ID                    (UDMA_UTC_ID_VPAC1_TC1)
#endif
#endif

#define VHWA_M2M_DOF_UTC_ID                     (UDMA_UTC_ID_DMPAC_TC0)
#define VHWA_M2M_SDE_UTC_ID                     (UDMA_UTC_ID_DMPAC_TC0)
#endif
/** @} */

/**
 *  \anchor Vhwa_UtcChId
 *  \name   UTC Channels numbers
 *  \brief  UTC channel numbers used by each VHWA Modules
 *
 *  @{
 */

/* UTC channels used for the VISS */
#define VHWA_VISS_UTC_CH_START                  (0x0U)
#define VHWA_VISS_UTC_CH_NUM                    (10U)
#define VHWA_VISS_UTC_CH_END                    (VHWA_VISS_UTC_CH_START +      \
                                                    VHWA_VISS_UTC_CH_NUM)

/* UTC channels used for the LDC */
#define VHWA_LDC_UTC_CH_START                   (VHWA_VISS_UTC_CH_END)
#define VHWA_LDC_UTC_CH_NUM                     (6U)
#define VHWA_LDC_UTC_CH_END                     (VHWA_LDC_UTC_CH_START +       \
                                                    VHWA_LDC_UTC_CH_NUM)

/* UTC channels used for the MSC */
#define VHWA_MSC_UTC_CH_START                   (VHWA_LDC_UTC_CH_END)
#define VHWA_MSC_UTC_CH_NUM                     (16U)
#define VHWA_MSC_UTC_CH_END                     (VHWA_MSC_UTC_CH_START +       \
                                                    VHWA_MSC_UTC_CH_NUM)

/* UTC channels used for the NF */
#define VHWA_NF_UTC_CH_START                    (VHWA_MSC_UTC_CH_END)
#define VHWA_NF_UTC_CH_NUM                      (5U)
#define VHWA_NF_UTC_CH_END                      (VHWA_NF_UTC_CH_START +        \
                                                    VHWA_NF_UTC_CH_NUM)

/* UTC channels used for the GLBCE ctx save/restore */
#define VHWA_GLBCE_CTX_UTC_CH_START             (VHWA_NF_UTC_CH_END)
#define VHWA_GLBCE_CTX_UTC_CH_NUM               (2U)
#define VHWA_GLBCE_CTX_UTC_CH_END               (VHWA_GLBCE_CTX_UTC_CH_START +        \
                                                    VHWA_GLBCE_CTX_UTC_CH_END)

/* UTC channels used for the DOF */
#define VHWA_DOF_UTC_CH_START                   (0x0u)
#define VHWA_DOF_UTC_CH_NUM                     (15U)
#define VHWA_DOF_UTC_CH_END                     (VHWA_DOF_UTC_CH_START +       \
                                                    VHWA_DOF_UTC_CH_NUM)

#define VHWA_SDE_UTC_CH_START                   (VHWA_DOF_UTC_CH_END)
#define VHWA_SDE_UTC_CH_NUM                     (15U)
#define VHWA_SDE_UTC_CH_END                     (VHWA_SDE_UTC_CH_START +       \
                                                    VHWA_SDE_UTC_CH_END)

/** @} */


#ifdef __cplusplus
}
#endif

#endif
