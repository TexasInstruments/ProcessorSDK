/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2019-2022
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


#ifndef ESM_APP_R5_H
#define ESM_APP_R5_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/csl/soc.h>
#include <ti/csl/csl_esm.h>
#include "ecc_ddr.h"
#include "ecc_msmc.h"
#include "ecc_msmc_mem_parity.h"

#define BITS_PER_WORD                       (32u)

typedef struct CSL_esm_app_R5_cfg_s {
    uint32_t    hi_pri_evt;
    uint32_t    lo_pri_evt;
}CSL_esm_app_R5_cfg;


/* ESM Base Addresses */

#define CSL_TEST_ESM_EN_KEY_ENBALE_VAL             (0xFU)
#define BITS_PERWORD                               (32u)
#define GROUP_NUMBER_BIT_SHIFT                     (5u)
#define NO_EVENT_VALUE                             (0xffffu)

#if defined(SOC_AM65XX)
    #define DDRSS_CFG_BASE                  (CSL_COMPUTE_CLUSTER0_SS_CFG_BASE)
    #if defined(BUILD_MPU)
        #define ESM_CFG_BASE                (CSL_ESM0_CFG_BASE)
        #define ESM_LO_INT                  (CSL_GIC0_INTR_ESM0_BUS_ESM_INT_LOW_LVL)
        #define ESM_HI_INT                  (CSL_GIC0_INTR_ESM0_BUS_ESM_INT_HI_LVL)
        #define ESM_CFG_ERR_INT             (CSL_GIC0_INTR_ESM0_BUS_ESM_INT_CFG_LVL)
        #define ESM_MAX_EVENT_MAP_NUM_WORDS (8u)
        /* esm_lvl_event_48 */
        #define MSMC_ECC_AGGR0_DED_ERR_EVENT    (CSL_ESM0_INTR_COMPUTE_CLUSTER0_BUS_ESM_EVENTS_OUT_LEVEL_0)
        /* esm_lvl_event_49 */
        #define MSMC_ECC_AGGR0_SEC_ERR_EVENT    (CSL_ESM0_INTR_COMPUTE_CLUSTER0_BUS_ESM_EVENTS_OUT_LEVEL_1)
    #else
        #define ESM_CFG_BASE                (CSL_MCU_ESM0_CFG_BASE)
        #define ESM_LO_INT                  (CSL_MCU0_INTR_ESM1_ESM_INT_LOW_LVL)
        #define ESM_HI_INT                  (CSL_MCU0_INTR_ESM1_ESM_INT_HI_LVL)
        #define ESM_CFG_ERR_INT             (CSL_MCU0_INTR_ESM1_ESM_INT_CFG_LVL)
        #define ESM_MAX_EVENT_MAP_NUM_WORDS (4u)
        /* esm_lvl_event_48 */
        #define MSMC_ECC_AGGR0_DED_ERR_EVENT    (CSL_ESM0_INTR_COMPUTE_CLUSTER0_BUS_ESM_EVENTS_OUT_LEVEL_0)
        /* esm_lvl_event_49 */
        #define MSMC_ECC_AGGR0_SEC_ERR_EVENT    (CSL_ESM0_INTR_COMPUTE_CLUSTER0_BUS_ESM_EVENTS_OUT_LEVEL_1)
    #endif
#endif

#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
    #if defined(SOC_J721E) || defined(SOC_J7200)
        #define DDRSS_CFG_BASE                  (CSL_COMPUTE_CLUSTER0_SS_CFG_BASE)
    #endif
    #if defined(SOC_J721S2)
        #define DDRSS_CFG_BASE                  (CSL_COMPUTE_CLUSTER0_DDR0_0_SS_CFG_BASE)
    #endif
    #if defined(SOC_J784S4) || defined(SOC_J742S2)
        #define DDRSS_CFG_BASE                  (CSL_COMPUTE_CLUSTER0_VBUSP_DDRSS0_SSCFG_BASE)
    #endif
    #if defined(BUILD_MPU)
        #define ESM_CFG_BASE                (CSL_ESM0_CFG_BASE)
        #define ESM_LO_INT                  (CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_ESM0_ESM_INT_LOW_LVL_0)
        #define ESM_HI_INT                  (CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_ESM0_ESM_INT_HI_LVL_0)
        #define ESM_CFG_ERR_INT             (CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_ESM0_ESM_INT_CFG_LVL_0)
        // #define ESM_MAX_EVENT_MAP_NUM_WORDS (8u)
        /* esm_lvl_event_48 */
        #define MSMC_ECC_AGGR0_DED_ERR_EVENT    (CSLR_ESM0_ESM_LVL_EVENT_COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_0)
        /* esm_lvl_event_49 */
        #define MSMC_ECC_AGGR0_SEC_ERR_EVENT    (CSLR_ESM0_ESM_LVL_EVENT_COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_1)
    #else
        #define ESM_CFG_BASE                (CSL_ESM0_CFG_BASE)
        #define ESM_LO_INT                  (CSLR_MCU_R5FSS0_CORE0_INTR_ESM0_ESM_INT_LOW_LVL_0)
        #define ESM_HI_INT                  (CSLR_MCU_R5FSS0_CORE0_INTR_ESM0_ESM_INT_HI_LVL_0)
        #define ESM_CFG_ERR_INT             (CSLR_MCU_R5FSS0_CORE0_INTR_ESM0_ESM_INT_CFG_LVL_0)
        #define ESM_MAX_EVENT_MAP_NUM_WORDS (4u)
        /* esm_lvl_event for DDR single error */
        #define DDR_ECC_AGGR0_SEC_ERR_EVENT    (CSLR_ESM0_ESM_LVL_EVENT_DDR0_DDRSS_DRAM_ECC_CORR_ERR_LVL_0)
        /* esm_lvl_event for DDR double error */
        #define DDR_ECC_AGGR0_DED_ERR_EVENT    (CSLR_ESM0_ESM_LVL_EVENT_DDR0_DDRSS_DRAM_ECC_UNCORR_ERR_LVL_0)
        /* esm_lvl_event_48 */
        #define MSMC_ECC_AGGR0_DED_ERR_EVENT    (CSLR_ESM0_ESM_LVL_EVENT_COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_0)
        /* esm_lvl_event_49 */
        #define MSMC_ECC_AGGR0_SEC_ERR_EVENT    (CSLR_ESM0_ESM_LVL_EVENT_COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_1)
    #endif
#endif
#if defined(SOC_AM64X)
    #define DDRSS_CFG_BASE                  (CSL_DDR16SS0_SS_CFG_BASE)
    #if defined(BUILD_MPU)
        #define ESM_CFG_BASE                (CSL_ESM0_CFG_BASE)
        #define ESM_LO_INT                  (CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_ESM0_ESM_INT_LOW_LVL_0)
        #define ESM_HI_INT                  (CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_ESM0_ESM_INT_HI_LVL_0)
        #define ESM_CFG_ERR_INT             (CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_ESM0_ESM_INT_CFG_LVL_0)
        /* #define ESM_MAX_EVENT_MAP_NUM_WORDS (8u) */
        /* esm_lvl_event_48 */
        #define MSMC_ECC_AGGR0_DED_ERR_EVENT    (CSLR_ESM0_ESM_LVL_EVENT_COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_0)
        /* esm_lvl_event_49 */
        #define MSMC_ECC_AGGR0_SEC_ERR_EVENT    (CSLR_ESM0_ESM_LVL_EVENT_COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_1)
    #else
        #define ESM_CFG_BASE                (CSL_ESM0_CFG_BASE)
        #define ESM_LO_INT                  (CSLR_R5FSS0_CORE0_INTR_ESM0_ESM_INT_LOW_LVL_0)
        #define ESM_HI_INT                  (CSLR_R5FSS0_CORE0_INTR_ESM0_ESM_INT_HI_LVL_0)
        #define ESM_CFG_ERR_INT             (CSLR_R5FSS0_CORE0_INTR_ESM0_ESM_INT_CFG_LVL_0)
        #define ESM_MAX_EVENT_MAP_NUM_WORDS (4u)
        /* esm_lvl_event for DDR single error */
        #define DDR_ECC_AGGR0_SEC_ERR_EVENT    (CSLR_ESM0_ESM_LVL_EVENT_DDR16SS0_DDRSS_DRAM_ECC_CORR_ERR_LVL_0)
        /* esm_lvl_event for DDR double error */
        #define DDR_ECC_AGGR0_DED_ERR_EVENT    (CSLR_ESM0_ESM_LVL_EVENT_DDR16SS0_DDRSS_DRAM_ECC_UNCORR_ERR_LVL_0)
        /* esm_lvl_event_48 */
        #define MSMC_ECC_AGGR0_DED_ERR_EVENT    (CSLR_ESM0_ESM_LVL_EVENT_COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_0)
        /* esm_lvl_event_49 */
        #define MSMC_ECC_AGGR0_SEC_ERR_EVENT    (CSLR_ESM0_ESM_LVL_EVENT_COMPUTE_CLUSTER0_CLEC_ESM_EVENTS_OUT_LEVEL_1)
    #endif
#endif
int32_t cslAppEsmSetup(uint32_t baseAddr, CSL_esm_app_R5_cfg* cfg);
int32_t cslAppEsmRegisterIntr(void);

/* extern functions */
void cslAppChkIsExpectedEvent(uint32_t eventId);
extern uintptr_t DDRGetTranslatedAddress(uintptr_t nonECCAddress);

/* exteran variable references */
extern uint32_t gUartBaseAddr;
extern uint32_t gRamIdExpected;
extern volatile Bool   gMsmcMemParityInterrupt;

#ifdef __cplusplus
}
#endif

#endif

