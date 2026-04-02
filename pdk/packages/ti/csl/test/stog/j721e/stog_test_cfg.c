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
 *  \file     stog_test_cfg.c
 *
 *  \brief    This file contains Slave TOG test configuration
 *
 *  \details  Slave TOG Test Configuration
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/csl_tog.h>
#include <ti/csl/soc.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/sciclient/sciclient.h>

/* Osal API header files */
#include <ti/osal/HwiP.h>
#include <ti/osal/TimerP.h>
#include "esm_app.h"
#include "stog_test_cfg.h"
#include "stog_test_utils.h"

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */


/* ========================================================================== */
/*                                Function prototypes                         */
/* ========================================================================== */
__attribute((section(".text:STOG_test"))) void STOG_injectESMError(uint32_t instanceIndex);
__attribute((section(".text:STOG_test"))) void STOG_injectMCU64B2TimeoutError(uint32_t instanceIndex);
__attribute((section(".text:STOG_test"))) void STOG_injectMCUINFRATimeoutError(uint32_t instanceIndex);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static volatile uint32_t gTestMSMCLocation[256] __attribute((section(".data:STOG_MSMC_location")));

STOG_TestHandle_t STOG_TestHandleArray[STOG_MAX_INSTANCE+1] __attribute((section(".data:STOG_test")))=
{

    /* MCU_INFRA0 */
    {
        .instanceName           = "MCU INFRA0",
        .pRegMap                = (CSL_ksbus_vbusm_to_gasketRegs *)CSL_MCU_TIMEOUT_INFRA0_CFG_BASE,

        .handler                = STOG_eventHandler,                   /* STOG event handler */
        .injectFunction         = STOG_injectMCUINFRATimeoutError,     /* STOG inject error */
        .ESMEventNumber         = CSLR_MCU_ESM0_ESM_LVL_EVENT_MCU_TIMEOUT_INFRA0_SAFEG_TRANS_ERR_LVL_0, /* STOG ESM event number */

        .doneFlag               = false,                 /* Initialize done flag */
        .timeoutValue           = 0x10000U,              /* timeout value */
        .intSrcBitmap           = (CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT |
                                   CSL_SLV_TOG_INTRSRC_UNEXPECTED_RESPONSE),

    },

    /* MCU_FW1 */
    {
        .instanceName           = "MCU FW1",
        .pRegMap                = (CSL_ksbus_vbusm_to_gasketRegs *)CSL_MCU_TIMEOUT_FW1_CFG_BASE,

        .handler                = STOG_eventHandler,       /* STOG event handler */
        .injectFunction         = STOG_injectESMError,     /* STOG inject ESM error */
        .ESMEventNumber         = CSLR_MCU_ESM0_ESM_LVL_EVENT_MCU_TIMEOUT_FW1_SAFEG_TRANS_ERR_LVL_0, /* STOG ESM event number */

        .doneFlag               = false,                  /* Initialize done flag */
        .timeoutValue           = 0x10000U,               /* timeout value */
        .intSrcBitmap           = 0x0U,

    },

    /* MCU_64B2 */
    {
        .instanceName           = "MCU 64B2",
        .pRegMap                = (CSL_ksbus_vbusm_to_gasketRegs *)CSL_MCU_TIMEOUT_64B2_CFG_BASE,

        .handler                = STOG_eventHandler,                   /* STOG event handler */
        .injectFunction         = STOG_injectMCU64B2TimeoutError,     /* STOG inject error */
        .ESMEventNumber         = CSLR_MCU_ESM0_ESM_LVL_EVENT_MCU_TIMEOUT_64B2_TRANS_ERR_LVL_0, /* STOG ESM event number */

        .doneFlag               = false,                 /* Initialize done flag */
        .timeoutValue           = 0x10000U,              /* timeout value */
        .intSrcBitmap           = (CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT |
                                   CSL_SLV_TOG_INTRSRC_UNEXPECTED_RESPONSE),

    },

    /* WKUP INFRA0 */
    {
        .instanceName           = "WKUP INFRA0",
        .pRegMap                = (CSL_ksbus_vbusm_to_gasketRegs *)CSL_WKUP_TIMEOUT_INFRA0_CFG_BASE,

        .handler                = STOG_eventHandler,       /* STOG event handler */
        .injectFunction         = STOG_injectESMError,     /* STOG inject ESM error */
        .ESMEventNumber         = CSLR_WKUP_ESM0_ESM_LVL_EVENT_WKUP_TIMEOUT_INFRA0_SAFEG_TRANS_ERR_LVL_0, /* STOG ESM event number */

        .doneFlag               = false,                  /* Initialize done flag */
        .timeoutValue           = 0x10000U,               /* timeout value */
        .intSrcBitmap           = 0x0U,

    },

};

void STOG_injectESMError(uint32_t instanceIndex)
{
    /* Do transaction that will exercise TOG */
    ESMSetIntrStatusRAW(MCU_ESM_BASE, STOG_TestHandleArray[instanceIndex].ESMEventNumber);
}

void STOG_injectMCU64B2TimeoutError(uint32_t instanceIndex)
{
    int32_t status;
    CSL_ksbus_vbusm_to_gasketRegs *pRegMap = STOG_TestHandleArray[instanceIndex].pRegMap;
    int i;

    /* Injecting error can result in a Data abort, so disable temporarily */
    disableABORT();

    /* Call CSL API to set smaller timeout to trigger error */
    status = CSL_slvTogSetTimeoutVal(pRegMap, 1u);
    if (status != CSL_PASS)
    {
        UART_printf("   Inject CSL_slvTogSetTimeoutVal Failed \n");
        /* Assert */
    }

    /* Do something to trigger transaction through the Gasket */
    for (i=0; i< 256; i++)
        gTestMSMCLocation[i] = 1U;

    /* Call CSL API to set configure back to original timeout value */
    status = CSL_slvTogSetTimeoutVal(pRegMap, STOG_TestHandleArray[instanceIndex].timeoutValue);
    if (status != CSL_PASS)
    {
        UART_printf("   Configure back CSL_slvTogSetTimeoutVal Failed \n");
        /* Assert */
    }

    /* Enable back Abort */
    enableABORT();
}

void STOG_injectMCUINFRATimeoutError(uint32_t instanceIndex)
{
    int32_t status;
    CSL_ksbus_vbusm_to_gasketRegs *pRegMap = STOG_TestHandleArray[instanceIndex].pRegMap;
    volatile esmRevisionId_t esmRevisionId;

    /* Injecting error can result in a Data abort, so disable temporarily */
    disableABORT();

    /* Call CSL API to set smaller timeout to trigger error */
    status = CSL_slvTogSetTimeoutVal(pRegMap, 1u);
    if (status != CSL_PASS)
    {
        UART_printf("   Inject CSL_slvTogSetTimeoutVal Failed \n");
        /* Assert */
    }

    /* Access Main ESM to trigger transaction through the Gasket */
    (void)ESMGetRevisionId(ESM_CFG_BASE, (esmRevisionId_t *)&esmRevisionId);

    /* Call CSL API to set configure back to original timeout value */
    status = CSL_slvTogSetTimeoutVal(pRegMap, STOG_TestHandleArray[instanceIndex].timeoutValue);
    if (status != CSL_PASS)
    {
        UART_printf("   Configure back CSL_slvTogSetTimeoutVal Failed \n");
        /* Assert */
    }

    /* Enable back Abort */
    enableABORT();
}
/* Nothing past this point */
