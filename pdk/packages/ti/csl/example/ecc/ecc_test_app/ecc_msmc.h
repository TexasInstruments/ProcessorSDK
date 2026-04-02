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

#ifndef APP_ECC_MSMC_H
#define APP_ECC_MSMC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/csl/soc.h>
#if defined (BUILD_MPU)
#include "esm_app.h"
#endif

#include <ti/csl/csl_ecc_aggr.h>
#if defined (SOC_J721E) || defined (SOC_J7200)
#include <ti/csl/soc/cslr_soc_ecc_aggr.h>
#endif

#define MSMC_ECC_SEC_ERR_TEST                    ('1')
#define MSMC_ECC_DED_ERR_TEST                    ('2')
#define MSMC_ECC_EXIT                            ('x')

/* Group pattern to use */
#define MSMC_ECC_TEST_GROUP_PATTERN              (CSL_ECC_AGGR_INJECT_PATTERN_A)

/* Test bit locations for introducing single and double bit errors */
#define MSMC_ECC_ERR_BIT_1                       (2u)
#define MSMC_ECC_ERR_BIT_2                       (8u)
#define MSMC_ECC_PARITY_ECC_GROUP_ACCESS_SEL     (3u)

/* Below Memory types for ECC are supported in the test
 * Please update the MAX Entries accordingly */
#define APP_ECC_MEMTYPE_MSMC                     (0u)

#define APP_ECC_MEMTYPE_MAX                      (APP_ECC_MEMTYPE_MSMC)

/* Max entries based on max mem type */
#if !defined(APP_ECC_AGGREGATOR_MAX_ENTRIES)
#define APP_ECC_AGGREGATOR_MAX_ENTRIES (APP_ECC_MEMTYPE_MAX + 1u)
#endif

extern CSL_ecc_aggrRegs * const cslAppEccAggrAddrTable[APP_ECC_AGGREGATOR_MAX_ENTRIES];

#if defined(SOC_AM65XX)
/* MSMC RAM ID for Bank 1 is 56 : MSMC - rmw1_queue_busecc_0
   MSMC RAM ID for Bank 0 is 49 : MSMC - rmw0_queue_busecc_1
   This test demonstrates the MSMC ECC using Bank 1
*/
#define CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID      (56U)
#endif
#if defined(SOC_J721E) 
/* MSMC RAM ID for Bank 1 is 78 : MSMC - rmw1_queue_busecc_0
   This test demonstrates the MSMC ECC using Bank 1
*/
/* TODO: Update macro with CSLR define after the CSLR bugfix for ECC aggregators on J721S2.*/
#define CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID      (CSL_COMPUTE_CLUSTER0_RMW0_QUEUE_BUSECC_0_RAM_ID)
#elif defined(SOC_J7200)
#define CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID      (CSL_COMPUTE_CLUSTER0_MSMC_J7VCL_CFG_WRAP_CBASS_VBUSP_MSMC_ECC_AGGR2_P2P_BRIDGE_EDC_CTRL_0_RAM_ID)
#elif defined(SOC_J721S2)
#define CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID      (36U)
#endif

#define ECC_AGGR_UNKNOWN_RAM_ID                      (0xFFFFU)

int32_t msmcEccTest(void);
void cslAppChkIsExpectedRamId(uint32_t testId);

/* exteran variable references */
extern uint32_t gUartBaseAddr;

#ifdef __cplusplus
}
#endif

#endif
