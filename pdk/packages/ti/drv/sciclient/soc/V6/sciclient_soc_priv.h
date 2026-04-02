/*
 *  Copyright (C) 2025 Texas Instruments Incorporated
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
 *  \file V6/sciclient_soc_priv.h
 *
 *  \brief Private J784S4 specific SOC file
 */

#ifndef SCICLIENT_SOC_PRIV_H_
#define SCICLIENT_SOC_PRIV_H_

#include <ti/drv/sciclient/soc/V6/sciclient_irq_rm.h>

#if defined (BUILD_HS)
#include <ti/drv/sciclient/soc/V6/sciclient_defaultBoardcfg_hexhs.h>
#include <ti/drv/sciclient/soc/V6/sciclient_defaultBoardcfg_rm_hexhs.h>
#include <ti/drv/sciclient/soc/V6/sciclient_defaultBoardcfg_pm_hexhs.h>
#include <ti/drv/sciclient/soc/V6/sciclient_defaultBoardcfg_security_hexhs.h>
#else
#include <ti/drv/sciclient/soc/V6/sciclient_defaultBoardcfg_hex.h>
#include <ti/drv/sciclient/soc/V6/sciclient_defaultBoardcfg_rm_hex.h>
#include <ti/drv/sciclient/soc/V6/sciclient_defaultBoardcfg_pm_hex.h>
#include <ti/drv/sciclient/soc/V6/sciclient_defaultBoardcfg_security_hex.h>
#endif

#if defined (BUILD_MCU1_0) || defined (SCICLIENT_MERGED)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_R5_NONSEC_0)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_R5_SEC_0)
#else
#if defined (BUILD_MCU1_1)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_R5_NONSEC_1)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_R5_SEC_1)
#endif
#if defined (BUILD_MPU1_0)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_A72_NONSEC_0)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_A72_SEC_0)
#endif
#if defined (BUILD_MPU1_1)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_A72_NONSEC_1)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_A72_SEC_1)
#endif
#if defined (BUILD_MPU2_0)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_A72_NONSEC_2)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_A72_SEC_2)
#endif
#if defined (BUILD_MPU2_1)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_A72_NONSEC_3)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_A72_SEC_3)
#endif
#if defined (BUILD_MCU2_0)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_MAIN_0_R5_NONSEC_0)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_MAIN_0_R5_SEC_0)
#endif
#if defined (BUILD_MCU2_1)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_MAIN_0_R5_NONSEC_1)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_MAIN_0_R5_SEC_1)
#endif
#if defined (BUILD_MCU3_0)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_MAIN_1_R5_NONSEC_0)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_MAIN_1_R5_SEC_0)
#endif
#if defined (BUILD_MCU3_1)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_MAIN_1_R5_NONSEC_1)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_MAIN_1_R5_SEC_1)
#endif
#if defined (BUILD_MCU4_0)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_MAIN_2_R5_NONSEC_0)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_MAIN_2_R5_SEC_0)
#endif
#if defined (BUILD_MCU4_1)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_MAIN_2_R5_NONSEC_1)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_MAIN_2_R5_SEC_1)
#endif
#if defined (BUILD_C7X_1)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_C7X_NONSEC_0)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_C7X_SEC_0)
#endif
#if defined (BUILD_C7X_2)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_C7X_NONSEC_1)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_C7X_SEC_1)
#endif
#if defined (BUILD_C7X_3)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_C7X_NONSEC_2)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_C7X_SEC_2)
#endif
#if defined (BUILD_C7X_4)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_C7X_NONSEC_3)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_C7X_SEC_3)
#endif
#endif /* #if defined (BUILD_MCU1_0) || defined (SCICLIENT_MERGED) */

#endif /* SCICLIENT_SOC_PRIV_H_ */
