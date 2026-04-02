/*
 *   Copyright (c) Texas Instruments Incorporated 2020
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
 *  \file     pbist_test_cfg.h
 *
 *  \brief    This file contains PBIST test configuration
 *
 *  \details  PBIST Test configuration
 **/
#ifndef PBIST_TEST_CFG_H
#define PBIST_TEST_CFG_H

#ifdef __cplusplus
extern "C"
{
#endif
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "pbist_test_func.h"

/* #define DEBUG */

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

/* Note the following are array indexes into the test config array */
#define PBIST_INSTANCE_MCU                (0U)
#define PBIST_INSTANCE_MAIN_PULSAR_0      (1U)
#define PBIST_INSTANCE_MAIN_PULSAR_1      (2U)
#define PBIST_INSTANCE_C7X                (3U)
#define PBIST_INSTANCE_A72                (4U)
#define PBIST_INSTANCE_VPAC               (5U)
#define PBIST_INSTANCE_DMPAC              (6U)
#define PBIST_INSTANCE_MAIN_NAVSS         (7U)
#define PBIST_INSTANCE_HC                 (8U)
#define PBIST_INSTANCE_C66X_0             (9U)
#define PBIST_INSTANCE_C66X_1             (10U)
#define PBIST_INSTANCE_MAIN_INFRA         (11U)
#define PBIST_INSTANCE_MSMC               (12U)
#define PBIST_INSTANCE_ENCODER            (13U)
#define PBIST_INSTANCE_DECODER            (14U)
#define PBIST_INSTANCE_GPU                (15U)
#define PBIST_INSTANCE_DSS_EDP_DSI        (16U)
#define PBIST_INSTANCE_MCU_PULSAR         (17U)


#define PBIST_MAX_INSTANCE                (PBIST_INSTANCE_MCU_PULSAR+1U)

#define PBIST_NUM_INSTANCE                (PBIST_INSTANCE_ENCODER+1U)

#define PBIST_MAX_TIMEOUT_VALUE           (100000000u)

#define PBIST_REGION_LOCAL_BASE           (0x60000000u)

#define PBIST_REGION2_LOCAL_BASE          (0x68000000u)

#define PBIST_RAT_REGION_INDEX            0
#define PBIST_RAT_REGION2_INDEX           1

#define PBIST_REG_REGION_SIZE             (0x400u)
#define PBIST_REG_REGION2_SIZE            (0x10000u)

/* Firewall definitions */
#define FW_REGION_ENABLE                  (0xAU)
#define FW_MCU_R5F0_PRIVID                (96U)

#define A72_NUM_AUX_DEVICES               1

#define MAIN_INFRA_NUM_AUX_DEVICES        18

#define MSMC_NUM_AUX_DEVICES              3

#define GPU_NUM_AUX_DEVICES               2

#define DSS_NUM_AUX_DEVICES               10

#define PBIST_RAT_CFG_BASE CSL_MCU_ARMSS_RAT_CFG_BASE

#define PBIST_NEG_TEST_PBIST_CFG_BASE CSL_MCU_PBIST0_BASE

extern PBIST_TestHandle_t PBIST_TestHandleArray[PBIST_MAX_INSTANCE+1];

extern int32_t PBIST_isPostPbistTimeout(uint32_t postStatMmrRegVal,
                                        Bool *pIsTimedOut);
extern int32_t PBIST_isPostPbistDone(uint32_t postStatMmrRegVal,
                                     Bool *pIsDone);
extern int32_t PBIST_postCheckResult(uint32_t postStatMmrRegVal,
                                     Bool *pResult);

#ifdef __cplusplus
}
#endif

#endif /* PBIST_TEST_CFG_H */
/* Nothing past this point */
