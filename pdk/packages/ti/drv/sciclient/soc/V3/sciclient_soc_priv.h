/*
 *  Copyright (C) 2019-2020 Texas Instruments Incorporated
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
 *  \file V3/sciclient_soc_priv.h
 *
 *  \brief Private AM64x specific SOC file
 */

#ifndef SCICLIENT_SOC_PRIV_H_
#define SCICLIENT_SOC_PRIV_H_

#include <ti/drv/sciclient/soc/V3/sciclient_irq_rm.h>

#include <ti/drv/sciclient/soc/V3/sciclient_defaultBoardcfg_hex.h>
#include <ti/drv/sciclient/soc/V3/sciclient_defaultBoardcfg_pm_hex.h>
#include <ti/drv/sciclient/soc/V3/sciclient_defaultBoardcfg_rm_hex.h>
#include <ti/drv/sciclient/soc/V3/sciclient_defaultBoardcfg_security_hex.h>

#if defined (BUILD_MCU1_0)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_R5_0_NONSEC_0)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_R5_0_SEC_0)
#endif
#if defined (BUILD_MCU1_1)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_R5_0_NONSEC_1)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_R5_0_SEC_1)
#endif
#if defined (BUILD_MCU2_0)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_R5_1_NONSEC_0)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_R5_1_SEC_0)
#endif
#if defined (BUILD_MCU2_1)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_R5_1_NONSEC_1)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_R5_1_SEC_1)
#endif
#if defined (BUILD_MPU1_0)
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_A53_NONSEC_1)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_A53_SEC_0)
#endif
#if defined (BUILD_M4F_0)
/* TBD */
#define SCICLIENT_CONTEXT_NONSEC    (SCICLIENT_CONTEXT_M4_NONSEC_0)
#define SCICLIENT_CONTEXT_SEC       (SCICLIENT_CONTEXT_M4_SEC_0)
#endif

#endif /* SCICLIENT_SOC_PRIV_H_ */
