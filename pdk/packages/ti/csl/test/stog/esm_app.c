/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2019
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
#include <stdint.h>
#include <string.h>

#include <ti/osal/osal.h>
#include "esm_app.h"

#include <ti/drv/uart/UART_stdio.h>

/* static global variables */
HwiP_Handle gEsmHiHwiPHandle;
HwiP_Handle gEsmLoHwiPHandle;
HwiP_Handle gEsmCfgHwiPHandle;

/* ----------------- local Function prototypes ------------------ */
static int32_t cslAppEnableIntr(uint32_t baseAddr, int32_t intNum);
static int32_t cslAppSetPri(uint32_t baseAddr, uint32_t pri, int32_t intNum);
static int32_t cslAppEnableEsmGlobalIntr(uint32_t baseAddr);
__attribute((section(".text:ESMApp_Handlers"))) static int32_t cslAppClearESMIntrStatus(uint32_t baseAddr, int32_t intNum);
__attribute((section(".text:ESMApp_Handlers"))) static int32_t cslAppEsmSetupHighPriHandler(uint32_t intNum);
__attribute((section(".text:ESMApp_Handlers"))) static int32_t cslAppEsmSetupLowPriHandler(uint32_t intNum);
__attribute((section(".text:ESMApp_Handlers"))) static void cslAppEsmHighInterruptHandler(uintptr_t arg);
__attribute((section(".text:ESMApp_Handlers"))) static void cslAppEsmLowInterruptHandler(uintptr_t arg);
__attribute((section(".text:ESMApp_Handlers"))) static void cslAppEsmInterruptHandler (uintptr_t baseAddr, esmIntrPriorityLvl_t esmIntrPriorityLvlType,
                                          uintptr_t arg );
__attribute((section(".text:ESMApp_Handlers"))) static void cslAppEsmProcessInterruptSource(uintptr_t arg, uint32_t intSrc);

__attribute((section(".text:ESMApp_Handlers"))) void cslAppChkIsExpectedEvent(uint32_t baseAddr,  uint32_t intSrc);


volatile uint32_t gSecTestPass;
volatile uint32_t gDedTestPass;
volatile char     gOption;

typedef struct cslAppEsmHandlerSet_s
{
    ESM_handlerPtr handle;
    uint32_t       parameter;
} cslAppEsmHandlerSet_t;

__attribute((section(".data:ESMApp_Handlers"))) static cslAppEsmHandlerSet_t cslAppMainEsmHandlerArray[ESM_MAX_EVENT_MAP_NUM_WORDS*32U]; 
__attribute((section(".data:ESMApp_Handlers"))) static cslAppEsmHandlerSet_t cslAppMcuEsmHandlerArray[ESM_MAX_EVENT_MAP_NUM_WORDS*32U];    

/* local function implementation */
static void cslAppEsmProcessInterruptSource(uintptr_t baseAddr, uint32_t intSrc)
{
    /* Check if the Interrupt caused by ESM event expected 
     */

    if (intSrc != NO_EVENT_VALUE)
    {
        cslAppChkIsExpectedEvent(baseAddr, intSrc);
        /* Clear this error */
        (void)ESMClearIntrStatus(baseAddr, intSrc);
    }
    return;
}

static void cslAppEsmInterruptHandler (uint32_t  baseAddr,
                                                                esmIntrPriorityLvl_t esmIntrPriorityLvlType,
                                                                uintptr_t arg )
{
    uint32_t             intSrc1, intSrc2;
    esmGroupIntrStatus_t localEsmGroupIntrStatus;

    /* Check on the highest priority event and handle it */
    do {
        (void)ESMGetGroupIntrStatus(baseAddr, (uint32_t)esmIntrPriorityLvlType,
                              &localEsmGroupIntrStatus);
        intSrc1 = localEsmGroupIntrStatus.highestPendPlsIntNum;
        cslAppEsmProcessInterruptSource(baseAddr, intSrc1);
#if 0
        if (intSrc1 == (uint32_t) arg)
        {
            break;
        }
#endif
        intSrc2 = localEsmGroupIntrStatus.highestPendLvlIntNum;
        cslAppEsmProcessInterruptSource(baseAddr, intSrc2);
#if 0
        if (intSrc2 == (uint32_t) arg)
        {
            break;
        }
#endif
    } while ((intSrc1 != (uint32_t)(NO_EVENT_VALUE)) || (intSrc2 != (uint32_t)(NO_EVENT_VALUE)));

    return;
}

void cslAppEsmHighInterruptHandler (uintptr_t arg)
{
    uint32_t intNumber = (uint32_t) arg;
    uintptr_t baseAddr;
    
    switch(intNumber)
    {
        case ESM_HI_INT:
            baseAddr = ESM_CFG_BASE;
            break;

        case MCU_ESM_HI_INT:
            baseAddr = MCU_ESM_CFG_BASE;
            break;
    }

    /* Call common Interrupt handler */
    cslAppEsmInterruptHandler(baseAddr, ESM_INTR_PRIORITY_LEVEL_HIGH, intNumber);

    /* Write end of interrupt */
//    (void)ESMClearIntrStatus(((uint32_t)ESM_CFG_BASE), esm_err_evt);
    return ;
}

void cslAppEsmLowInterruptHandler (uintptr_t arg)
{
    uint32_t intNumber = (uint32_t) arg;
    uint32_t baseAddr;

    switch(intNumber)
    {
        case ESM_LO_INT:
            baseAddr = ESM_CFG_BASE;
            break;

        case MCU_ESM_LO_INT:
            baseAddr = MCU_ESM_CFG_BASE;
            break;
    }

    /* Call common Interrupt handler */
    cslAppEsmInterruptHandler(baseAddr, ESM_INTR_PRIORITY_LEVEL_LOW, intNumber);

    /* Write end of interrupt */
//    (void)ESMClearIntrStatus(((uint32_t)ESM_CFG_BASE), esm_err_evt);

    return ;
}

static int32_t cslAppEsmSetupHighPriHandler(uint32_t intNum)
{
    HwiP_Params       hwiParams;

    HwiP_disableInterrupt(intNum);
    HwiP_Params_init(&hwiParams);
    hwiParams.arg = intNum;
    hwiParams.enableIntr = FALSE;
    /* register the call back function for ESM Hi interrupt */
    gEsmHiHwiPHandle = HwiP_create(intNum,
                                   (HwiP_Fxn) cslAppEsmHighInterruptHandler,
                                   (void *)&hwiParams);
    HwiP_enableInterrupt(intNum);

    return (CSL_PASS);
}

static int32_t cslAppEsmSetupLowPriHandler(uint32_t intNum)
{
    HwiP_Params       hwiParams;

    HwiP_disableInterrupt(intNum);
    HwiP_Params_init(&hwiParams);
    hwiParams.arg = intNum;
    hwiParams.enableIntr = FALSE;
    /* register the call back function for ESM Lo interrupt */
    gEsmHiHwiPHandle = HwiP_create(intNum,
                                   (HwiP_Fxn) cslAppEsmLowInterruptHandler,
                                   (void *)&hwiParams);
    HwiP_enableInterrupt(intNum);
    return (CSL_PASS);
}

/* This function clears the ESM interrrupt status */
static int32_t cslAppClearESMIntrStatus(uint32_t baseAddr, int32_t intNum)
{
    int32_t   cslRet;
    uint32_t  intStatus;

    /* Clear interrupt status, so that we start with clean state */
    cslRet = ESMClearIntrStatus(baseAddr, intNum);

    if (cslRet == CSL_PASS)
    {
        cslRet = ESMGetIntrStatus(baseAddr, intNum, &intStatus);
    }
    if (cslRet == CSL_PASS)
    {
        if (intStatus != ((uint32_t)0U))
        {
            cslRet = CSL_EFAIL;
        }
    }
    return (cslRet);
}

/* Enable the ESM event */
static int32_t cslAppEnableIntr(uint32_t baseAddr, int32_t intNum)
{
    int32_t   cslRet;
    uint32_t  intStatus;

    /* Enable interrupt and verify if interrupt status is enabled */
    cslRet = ESMEnableIntr(baseAddr, intNum);

    if (cslRet == CSL_PASS)
    {
        cslRet = ESMIsEnableIntr(baseAddr, intNum, &intStatus);
    }
    if (cslRet == CSL_PASS)
    {
        if (intStatus != ((uint32_t)1U))
        {
            cslRet = CSL_EFAIL;
        }
    }
    return (cslRet);
}

/* Enable the ESM event */
static int32_t cslAppDisableIntr(uint32_t baseAddr, int32_t intNum)
{
    int32_t   cslRet;
    uint32_t  intStatus;

    /* Enable interrupt and verify if interrupt status is enabled */
    cslRet = ESMDisableIntr(baseAddr, intNum);

    if (cslRet == CSL_PASS)
    {
        cslRet = ESMIsEnableIntr(baseAddr, intNum, &intStatus);
    }
    if (cslRet == CSL_PASS)
    {
        if (intStatus != ((uint32_t)0U))
        {
            cslRet = CSL_EFAIL;
        }
    }
    return (cslRet);
}
/* Set the ESM Pri for that event */
static int32_t cslAppSetPri(uint32_t baseAddr, uint32_t pri, int32_t intNum)
{
    int32_t    cslRet;
    esmIntrPriorityLvl_t intrPriorityLvlWr, intrPriorityLvlRd;

    intrPriorityLvlWr = pri;

    cslRet = ESMSetIntrPriorityLvl(baseAddr, intNum, intrPriorityLvlWr);

    if (cslRet == CSL_PASS)
    {
        cslRet = ESMGetIntrPriorityLvl(baseAddr,
                                       intNum,
                                       &intrPriorityLvlRd);
    }
    if (cslRet == CSL_PASS)
    {
        if (intrPriorityLvlWr != intrPriorityLvlRd)
        {
            cslRet = CSL_EFAIL;
        }
    }
    return (cslRet);
}

/* Enable the global interrupt */
static int32_t cslAppEnableEsmGlobalIntr(uint32_t baseAddr)
{
    int32_t     cslRet;
    uint32_t    intStatus;

    /* Enable Global interrupt and verify if global interrupt is enabled for ESM */
    cslRet = ESMEnableGlobalIntr(baseAddr);

    if (cslRet == CSL_PASS)
    {
        cslRet = ESMGetGlobalIntrEnabledStatus(baseAddr, &intStatus);
    }
    if (cslRet == CSL_PASS)
    {
        if (intStatus != CSL_TEST_ESM_EN_KEY_ENBALE_VAL)
        {
            cslRet = CSL_EFAIL;
        }
    }
    return (cslRet);
}

/* ---------------------------------------------------- */
void cslAppChkIsExpectedEvent(uint32_t baseAddr,  uint32_t intSrc)
{
    switch(baseAddr)
    {
        case ESM_CFG_BASE:
            if (cslAppMainEsmHandlerArray[intSrc].handle != NULL_PTR)
                cslAppMainEsmHandlerArray[intSrc].handle(cslAppMainEsmHandlerArray[intSrc].parameter);
            break;

        case MCU_ESM_CFG_BASE:
            if (cslAppMcuEsmHandlerArray[intSrc].handle != NULL_PTR)
                cslAppMcuEsmHandlerArray[intSrc].handle(cslAppMcuEsmHandlerArray[intSrc].parameter);
            break;
    }

    return;
}

/* function initializes ESM */
int32_t cslAppEsmSetupCommon(uint32_t  baseAddr, uint32_t hiIntNum, uint32_t loIntNum)
{
    int32_t     cslRet;

    /* ESM reset and configure */
    cslRet = ESMReset(baseAddr);
    if ( cslRet != CSL_PASS)
    {
        UART_printf( "\r\nESM reset failed...");
    }
     
    if (cslRet == CSL_PASS)
    {
        cslRet = cslAppEnableEsmGlobalIntr(baseAddr);
        if ( cslRet != CSL_PASS)
        {
            UART_printf("\r\nESM Enable Global Interrupt Failed...");
        }
    }

    if (cslRet == CSL_PASS)
    {
        if ( hiIntNum != CSL_APP_INT_NUM_INVALID)
        {
            cslRet = cslAppEsmSetupHighPriHandler(hiIntNum);
            if ( cslRet != CSL_PASS)
            {
                UART_printf("\r\nESM High Priority handler setup Failed...");
            }
        }
    }

    if (cslRet == CSL_PASS)
    {
        if ( loIntNum != CSL_APP_INT_NUM_INVALID)
        {
            cslRet = cslAppEsmSetupLowPriHandler(loIntNum);
            if ( cslRet != CSL_PASS)
            {
                UART_printf("\r\nESM Lo Priority handler setup Failed...");
            }
        }
    }

    if (cslRet == CSL_PASS)
    {
        UART_printf("\r\n cslAppEsmSetupCommon...Done");
    }
    return (cslRet);
}

/* function initializes ESM */
int32_t cslAppEsmConfig(uint32_t  baseAddr, uint32_t esmEventNumber,
                        uint32_t priority, ESM_handlerPtr pESMHandler,
                        uint32_t parameter)
{
    int32_t     cslRet;

    cslRet = cslAppClearESMIntrStatus(baseAddr, esmEventNumber);
    if ( cslRet != CSL_PASS)
    {
        UART_printf("\r\n cslAppClearESMIntrStatus hi pri event failed...");
    }

     /* Enable interrupt and verify if interrupt status is enabled */
     if (cslRet == CSL_PASS)
    {
        cslRet = cslAppEnableIntr(baseAddr, esmEventNumber);
        if ( cslRet != CSL_PASS)
        {
            UART_printf("\r\nError in ESM Intr Enable for hi pri Event...");
        }  
    }

    /* Assign the priority for the events
     * as single bit errors are corrected, can be assigned to lo pri
     * and double bit errors are not corrected, can be assigned to hi pri
     */

    if (cslRet == CSL_PASS)
    {
        cslRet = cslAppSetPri(baseAddr, priority, esmEventNumber);
        if ( cslRet != CSL_PASS)
        {
            UART_printf("\r\nError in setting Pri for hi pri Event...");
        }
    }

    if (cslRet == CSL_PASS)
    {
        if (baseAddr == MCU_ESM_CFG_BASE)
        {
            cslAppMcuEsmHandlerArray[esmEventNumber].handle = pESMHandler;
            cslAppMcuEsmHandlerArray[esmEventNumber].parameter = parameter;
        } else if (baseAddr == ESM_CFG_BASE)
        {
            cslAppMainEsmHandlerArray[esmEventNumber].handle = pESMHandler;
            cslAppMainEsmHandlerArray[esmEventNumber].parameter = parameter;
        }
    }

    if (cslRet == CSL_PASS)
    {
        UART_printf("\r\n cslAppEsmConfig...Done");
    }
    return (cslRet);
}

int32_t cslAppEsmDisable(uint32_t  baseAddr, uint32_t esmEventNumber)
{
    int32_t     cslRet;

    cslRet = cslAppClearESMIntrStatus(baseAddr, esmEventNumber);
    if ( cslRet != CSL_PASS)
    {
        UART_printf("\r\n cslAppClearESMIntrStatus hi pri event failed...");
    }

     /* Enable interrupt and verify if interrupt status is enabled */
     if (cslRet == CSL_PASS)
    {
        cslRet = cslAppDisableIntr(baseAddr, esmEventNumber);
        if ( cslRet != CSL_PASS)
        {
            UART_printf("\r\nError in ESM Intr Enable for hi pri Event...");
        }  
    }

    /* Disable priority
     */

    if (cslRet == CSL_PASS)
    {
        cslRet = cslAppSetPri(baseAddr, 0U, esmEventNumber);
        if ( cslRet != CSL_PASS)
        {
            UART_printf("\r\nError in setting Pri for hi pri Event...");
        }
    }

    if (cslRet == CSL_PASS)
    {
        if (baseAddr == MCU_ESM_CFG_BASE)
        {
            cslAppMcuEsmHandlerArray[esmEventNumber].handle = NULL;
            cslAppMcuEsmHandlerArray[esmEventNumber].parameter = 0U;
        } else if (baseAddr == ESM_CFG_BASE)
        {
            cslAppMainEsmHandlerArray[esmEventNumber].handle = NULL;
            cslAppMainEsmHandlerArray[esmEventNumber].parameter = 0U;
        }
    }

    if (cslRet == CSL_PASS)
    {
        UART_printf("\r\n cslAppEsmDisable...Done");
    }
    return (cslRet);
}


int32_t cslAppEsmSetup(void)
{
    int32_t     cslRet;

    memset(cslAppMcuEsmHandlerArray, 0, sizeof(cslAppMcuEsmHandlerArray));
    memset(cslAppMainEsmHandlerArray, 0, sizeof(cslAppMainEsmHandlerArray));

    cslRet = cslAppEsmSetupCommon(MCU_ESM_CFG_BASE,
                                  ESM_HI_INT,
                                  ESM_LO_INT);

    if (cslRet == CSL_PASS)
    {
         cslRet = cslAppEsmSetupCommon(ESM_CFG_BASE,
                                       MCU_ESM_HI_INT,
                                       MCU_ESM_LO_INT);
    }

    if (cslRet == CSL_PASS)
    {
        UART_printf("\r\n cslAppEsmSetup...Done");
    }
    return (cslRet);

}
