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
 *  \file     pbist_test_cfg.c
 *
 *  \brief    This file contains PBIST test configuration
 *
 *  \details  PBIST Test Configuration
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/csl/soc.h>
#include <ti/csl/csl_pbist.h>
#include <ti/drv/sciclient/sciclient.h>

#include "pbist_test_cfg.h"

/* #define DEBUG */

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
PBIST_TestHandle_t PBIST_TestHandleArray[PBIST_MAX_INSTANCE+1] =
{
    /* M4F */
    {
        .testName            = "M4F",
        .numPostPbistToCheck = 0u,
        .PBISTRegsHiAddress  = 0u,
        .pPBISTRegs          = (CSL_pbistRegs *)CSL_PBIST1_BASE, /* PBIST1: M4F */
        .pollMode            = true,
        .numPBISTRuns        = 1u,
        .PBISTConfigRun = {
            {

                .override           = 0u,  /* Memory and algorithm decided by ROM */
//                .override           = (CSL_PBIST_OVER_RINFO_MASK | CSL_PBIST_OVER_ALGO_MASK),  /* Memory and algorithm decided by ROM */
                /* NOTE: Because of override memoryGroupsBitMap & algorithmsBitMap ignored */
                .algorithmsBitMap   = 0x003fffffu,              /* Choose Algorithms 1-22 */
                .algorithmsBitMap   = 0x00000000000001Fu,       /* Memory group 1-5; Skipping 6: VIM has issue */
                .scrambleValue      = 0xFEDCBA9876543210U,      /* Scramble Value */
            }
        },
        .tisciPBISTDeviceId     = TISCI_DEV_PBIST1,         /* PBIST device id  */
        .procRstNeeded          = true,
        .secondaryCoreNeeded    = false,                /* Secondary core needed */
        .coreName               = "M4F Core",   /* Primary core   */
        .tisciProcId            = SCICLIENT_PROCID_MCU_M4FSS0_C0, /* M4F core */
        .tisciDeviceId          = TISCI_DEV_MCU_M4FSS0_CORE0,    /* M4F core */
        .coreCustPwrSeqNeeded   = false,                /* No custom power sequencing beyond Device controls */
        .numAuxDevices          = 0u,                   /* No Aux devices        */
        .doneFlag               = false,                /* Initialize done flag  */
    },
 };

int32_t PBIST_commonInit(void)
{

    return 0;
}

void PBIST_eventHandler( uint32_t instanceId)
{

    PBIST_TestHandleArray[instanceId].doneFlag = true;

    return;
}
/* Nothing past this point */
