/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   osal_extended_testapp_hwi.c
 *
 *  \brief  OSAL Hwi Sub Module testcases file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"
#if defined (BUILD_MCU) && defined (FREERTOS)
#include <ti/csl/arch/r5/csl_vim.h>
#include <ti/kernel/freertos/portable/TI_CGT/r5f/port_Hwi_priv.h>
#endif
#if defined (BUILD_C7X)
#include "Hwi.h"
#include "Mmu.h"
#include "Cache.h"
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define OSAL_APP_IRQ_SECONDARY_INT_NUM (29U)
#if defined(SOC_J721E)
#define OSAL_INUSE_INTR                (16U)
#else
#define OSAL_INUSE_INTR                (14U)
#endif
/* C7x cores can serve only 64 interrupts, numbered 0 to 63. */
#if defined (BUILD_C7X)
#define INVALID_INT_NUM_C7X            (64U)
#define HWI_OFFSET                     (8U)     /* Padded sctructure offset */
#endif
#if defined (BUILD_MCU)
/* Maximum interrupts supported by R5F is 512. */
#define INVALID_R5F_INT_NUM            (513U)
#endif
#define OSAL_APP_IRQ_INT_NUM           (28U)
#define OSAL_APP_FIQ_INT_NUM           (29U)
#define OSAL_APP_HWI_MAX_NUM           (64U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

volatile uint32_t gOsalAppFlagHwiTest = UFALSE, gOsalAllFIQFlag = UFALSE;
volatile uintptr_t gOsalAppHwiKey;

volatile uint32_t gOsalPreISRHookCount = 0U;
volatile uint32_t gOsalPostISRHookCount = 0U;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

#if defined (BUILD_MCU) && defined (FREERTOS)
extern void HwiP_fiq_handler(void);

/*
 * Description : Testing FIQ interrupts
 */
static int32_t OsalApp_hwiCreateFIQTest(CSL_VimIntrType leveltype);
#endif

/*
 * Description : Testing Null check for Hwi API
 */
static int32_t OsalApp_hwiNullTest(void);

/*
 * Description : Test posting of interrupts. 
 */
static int32_t OsalApp_hwiCreateAllocOvrflwTest(void);

/*
 * Description : Testing Negative test for API Hwi_delete
 */
static int32_t OsalApp_hwiDeleteNegativeTest(void);

#if defined (BUILD_MCU)
/*
 * Description : Create a Pulse interrupt and test the VIM APIs.
 */
static int32_t OsalApp_hwiCreatePulseIntrTest(void);
#endif

/*
 * Description : Testing Negative test for API HwiP_create
 */
#if defined(BUILD_C7X) 
static int32_t OsalApp_hwiCreateNegativeTest(void);

/*
 * Description : Testing Maximum Hwi creation
 */
static int32_t OsalApp_hwiCreateMaxTest(void);
#endif

#if defined (SAFERTOS) && (defined (BUILD_MCU) || defined (BUILD_C7X))

/*
 * Description : Testing Pre ISR hook
 */
static void OsalApp_preISRHook(void);

/*
 * Description : Testing Post ISR hook
 */
static void OsalApp_postISRHook(void);

#endif

/* ========================================================================== */
/*                       Internal Function Definitions                        */
/* ========================================================================== */

#if defined (BUILD_MCU) && defined (FREERTOS)
static void OsalApp_hwiFIQ(uintptr_t arg)
{
    gOsalAllFIQFlag = UTRUE;
}
#endif

static void OsalApp_hwiIRQ(uintptr_t arg)
{
#if defined (BUILD_MCU)    
    CSL_ArmR5CPUInfo  info;
    CSL_vimRegs       *pVimRegs;
    uint32_t          intNum, intPri;

    /* Call into CSL Arch VIM APIs to test them */
    CSL_armR5GetCpuID(&info);
    if (CSL_ARM_R5_CLUSTER_GROUP_ID_0 == info.grpId)
    {
        /* MCU SS Pulsar R5 SS */
        pVimRegs = (CSL_vimRegs *)CSL_MCU_DOMAIN_VIM_BASE_ADDR;
    }
    else
    {
        /* MAIN SS Pulsar R5 SS */
        pVimRegs = (CSL_vimRegs *)CSL_MAIN_DOMAIN_VIM_BASE_ADDR;
    }

    CSL_vimGetGroupsIntrPending(pVimRegs, OSAL_APP_IRQ_INT_NUM);
    CSL_vimGetGroupsIntrPending(NULL, OSAL_APP_IRQ_INT_NUM);

    if (CSL_VIM_INTR_TYPE_LEVEL == CSL_vimGetIntrType( pVimRegs, OSAL_APP_IRQ_INT_NUM ))
    {
        if((1U << OSAL_APP_IRQ_INT_NUM) == CSL_vimGetGroupIntrPending(pVimRegs, CSL_VIM_INTR_MAP_IRQ, OSAL_APP_IRQ_INT_NUM/32U, BTRUE) &&
            0U == CSL_vimGetGroupIntrPending(pVimRegs, CSL_VIM_INTR_MAP_FIQ, 0, BTRUE) &&
            0U == CSL_vimGetGroupIntrPending(pVimRegs, CSL_VIM_INTR_MAP_ALL, 2, BTRUE) &&
            (CSL_PASS) == CSL_vimGetActivePendingIntr(pVimRegs, CSL_VIM_INTR_MAP_IRQ, &intNum, &intPri) &&
            (CSL_PASS) == CSL_vimGetActivePendingIntr( pVimRegs, CSL_VIM_INTR_MAP_IRQ, NULL, NULL ) &&
            (CSL_EFAIL) == CSL_vimGetActivePendingIntr( pVimRegs, CSL_VIM_INTR_MAP_FIQ, NULL, NULL ))
        {
            Intc_IntClrPend(OSAL_APP_IRQ_INT_NUM);
            gOsalAppFlagHwiTest = UTRUE;
        }
    }
    else
#endif
    {
        gOsalAppFlagHwiTest = UTRUE;
    }
#if defined (BUILD_MCU)
    CSL_vimAckIntr(pVimRegs, CSL_VIM_INTR_MAP_IRQ);
    CSL_vimAckIntr(pVimRegs, CSL_VIM_INTR_MAP_FIQ);
    CSL_vimAckIntr(NULL, CSL_VIM_INTR_MAP_FIQ);
#endif
    /* Testing the APIs called from interrupt context */
    Osal_delay(0);
    gOsalAppHwiKey = HwiP_disable();
    HwiP_restore(gOsalAppHwiKey);
}

static int32_t OsalApp_hwiNullTest(void)
{
    HwiP_Params      hwiParams;
    HwiP_Handle      hwiHandle = NULL_PTR;
    int32_t          result = osal_OK;

    HwiP_Params_init(&hwiParams);

    hwiHandle = HwiP_create(CSL_INVALID_VEC_ID, NULL_PTR, NULL_PTR);
#if defined(BUILD_C7X)
    if(NULL_PTR == hwiHandle)
    {
        result = osal_FAILURE;
    }
#else
    if(NULL_PTR != hwiHandle)
    {
        result = osal_FAILURE;
    }
#endif
    if(NULL_PTR != hwiHandle)
    {
        if(HwiP_OK != HwiP_delete(hwiHandle))
        {
            result = osal_FAILURE;
        }
    }
    if(osal_OK == result)
    {
        if(NULL_PTR != HwiP_createDirect(CSL_INVALID_VEC_ID, NULL_PTR, NULL_PTR))
        {
            result = osal_FAILURE;
        }
    }

    if(osal_OK == result)
    {
        if(HwiP_OK == HwiP_delete(NULL_PTR))
        {
            result = osal_FAILURE;
        }
    }
    
    if(osal_OK != result)
    {
        OSAL_log("\n HwiP Null test failed!\n");
    }

    return result;
}

#if defined (BUILD_MCU)
static int32_t OsalApp_hwiCreatePulseIntrTest(void)
{
    HwiP_Params hwiParams;
    HwiP_Handle handle;
    CSL_vimRegs reg;
    int32_t result = osal_OK;

    HwiP_Params_init(&hwiParams);
    hwiParams.triggerSensitivity = OSAL_ARM_GIC_TRIG_TYPE_EDGE;

    if (CSL_EFAIL != CSL_vimCfgIntr( &reg, INVALID_R5F_INT_NUM, 0, 0, 0, 0 ) || 
        CSL_EFAIL != CSL_vimCfgIntr( NULL, INVALID_R5F_INT_NUM, 0, 0, 0, 0 ) ||
        CSL_EFAIL != CSL_vimVerifyCfgIntr( &reg, INVALID_R5F_INT_NUM, 0, 0, 0, 0 ) || 
        CSL_EFAIL != CSL_vimVerifyCfgIntr( NULL, INVALID_R5F_INT_NUM, 0, 0, 0, 0 ))
    {
        result = osal_FAILURE;
    }

    /* Test Register and deregister CSL_VIM_INTR_MAP_FIQ */
    Intc_IntPrioritySet(OSAL_APP_FIQ_INT_NUM, 0U, 1U); 
    Intc_IntRegister(OSAL_APP_FIQ_INT_NUM, NULL, NULL);
    Intc_IntUnregister( OSAL_APP_FIQ_INT_NUM );

    /* Test IntC APIs with Negative conditions. */
    Intc_IntSetSrcType(INVALID_R5F_INT_NUM, 0);
    Intc_IntPrioritySet(INVALID_R5F_INT_NUM, 0, 0);
    Intc_IntEnable(INVALID_R5F_INT_NUM);
    Intc_IntDisable(INVALID_R5F_INT_NUM);
    Intc_IntRegister(INVALID_R5F_INT_NUM, NULL, NULL);
    Intc_IntUnregister( INVALID_R5F_INT_NUM );

    handle = HwiP_create(OSAL_APP_IRQ_INT_NUM, (HwiP_Fxn)OsalApp_hwiIRQ, &hwiParams);

    if(NULL_PTR == handle)
    {
        result = osal_FAILURE;
    }

    if(osal_OK == result)
    {
        gOsalAppFlagHwiTest = UFALSE;
        HwiP_post(OSAL_APP_IRQ_INT_NUM);
        /* Wait till the interupt is hit */
        while(UFALSE == gOsalAppFlagHwiTest)
        {
            /* Do nothing */
        }
        gOsalAppFlagHwiTest = UFALSE;
        if(HwiP_OK != HwiP_delete(handle))
        {
            result = osal_FAILURE;
        }
    }
    if(osal_OK != result)
    {
        OSAL_log("\n Pulse Interrupt test has failed!\n");
    }

    return result;
}
#endif

#if defined (BUILD_MCU) && defined (FREERTOS)
static int32_t OsalApp_hwiCreateFIQTest(CSL_VimIntrType leveltype)
{
    HwiP_Params      hwiParams;
    HwiP_Handle      handle;
    int32_t          result = osal_OK;
    CSL_ArmR5CPUInfo info;
    CSL_vimRegs      *pVimRegs;

    /* Call into CSL Arch VIM APIs to test them */
    CSL_armR5GetCpuID(&info);
    if (CSL_ARM_R5_CLUSTER_GROUP_ID_0 == info.grpId)
    {
        /* MCU SS Pulsar R5 SS */
        pVimRegs = (CSL_vimRegs *)CSL_MCU_DOMAIN_VIM_BASE_ADDR;
    }
    else
    {
        /* MAIN SS Pulsar R5 SS */
        pVimRegs = (CSL_vimRegs *)CSL_MAIN_DOMAIN_VIM_BASE_ADDR;
    }

    HwiP_Params_init(&hwiParams);
    handle = HwiP_create(OSAL_APP_FIQ_INT_NUM, (HwiP_Fxn)OsalApp_hwiFIQ, &hwiParams);

    if(0 != CSL_vimCfgIntr(pVimRegs,
                    OSAL_APP_FIQ_INT_NUM,
                    0U,
                    CSL_VIM_INTR_MAP_FIQ,
                    leveltype,
                    ((uint32_t)HwiP_fiq_handler) & (~0x3U)))
    {
        result = osal_FAILURE;
    }
    if(osal_OK == result)
    {
        if( 0 != CSL_vimVerifyCfgIntr(pVimRegs,
                    OSAL_APP_FIQ_INT_NUM,
                    0U,
                    CSL_VIM_INTR_MAP_FIQ,
                    leveltype,
                    ((uint32_t)HwiP_fiq_handler) & (~0x3U)))
        {
            result = osal_FAILURE;
        }
    }

    HwiP_post(OSAL_APP_FIQ_INT_NUM);

    if(osal_OK == result)
    {
        /* Wait till FIQ is hit */
        while(UFALSE == gOsalAllFIQFlag)
        {
            /* Do nothing */
        }
    }

    HwiP_delete(handle);
    if(osal_OK != result)
    {
        OSAL_log("\n FIQ test has failed!\n");
    }

    return result;
}
#endif /* #if defined (FREERTOS) */
void reconfigTestFunc(uint32_t arg1)
{

}

static int32_t OsalApp_hwiCreateAllocOvrflwTest(void)
{
    HwiP_Params hwiParams;
    HwiP_Handle handle1, handle2;
    int32_t result = osal_OK;

#if defined(BUILD_C7X)
    Hwi_Params  hwiParamsCsl;
    Hwi_Handle  hwiHandle;

    Hwi_Params_init(NULL);
#endif
    HwiP_Params_init(&hwiParams);

    /* Create 2 HwiP to register a change in peakHandles */
    handle1 = HwiP_create(OSAL_APP_IRQ_INT_NUM, (HwiP_Fxn)OsalApp_hwiIRQ, &hwiParams);
#if defined(BUILD_C7X)
    if (NULL_PTR != HwiP_create(OSAL_APP_IRQ_INT_NUM, (HwiP_Fxn)OsalApp_hwiIRQ, &hwiParams))
    {
        result = osal_FAILURE;
    }
#endif
    handle2 = HwiP_createDirect(OSAL_APP_IRQ_SECONDARY_INT_NUM, (HwiP_DirectFxn)OsalApp_hwiIRQ, &hwiParams);

    /* handle2 is not deleted for C7x anf c66x as HwiP_createDirect returns NULL */
#if defined(BUILD_C7X) || defined(BUILD_C66X)
    if((NULL_PTR == handle1) || (NULL_PTR != handle2))
#else
    if((NULL_PTR == handle1) || (NULL_PTR == handle2) || (HwiP_OK != HwiP_delete(handle2)))
#endif
    {
        result = osal_FAILURE;
    }

    if(osal_OK == result)
    {
        HwiP_post(OSAL_APP_IRQ_INT_NUM);
#if defined(BUILD_C66X)
        /* Posting interrupt is not supported fro c66x cores */
        if(osal_UNSUPPORTED != HwiP_post(OSAL_APP_IRQ_INT_NUM))
#else
        if(HwiP_OK == HwiP_post(OSAL_APP_IRQ_INT_NUM))
#endif
        {
            /* Wait till the interupt is hit */
            while(UFALSE == gOsalAppFlagHwiTest)
            {
                /* Do nothing */
            }
            gOsalAppFlagHwiTest = UFALSE;
        }

#if defined(BUILD_C7X)
        hwiParamsCsl.maskSetting = Hwi_MaskingOption_NONE;
        hwiHandle = Hwi_getHandle(OSAL_APP_IRQ_INT_NUM);
        Hwi_reconfig(hwiHandle, reconfigTestFunc, &hwiParamsCsl);
        if ((0 != hwiHandle->disableMask) || (0 != hwiHandle->restoreMask))
        {
            result = osal_FAILURE;
        }

        hwiParamsCsl.maskSetting = Hwi_MaskingOption_ALL;
        Hwi_reconfig(hwiHandle, reconfigTestFunc, &hwiParamsCsl);
        if ((0xFFFFFFFFFFFFFFFFUL != hwiHandle->disableMask) || (0xFFFFFFFFFFFFFFFFUL != hwiHandle->restoreMask))
        {
            result = osal_FAILURE;
        }

        hwiParamsCsl.maskSetting = Hwi_MaskingOption_BITMASK;
        hwiParamsCsl.disableMask = 0xABCDABCD;
        hwiParamsCsl.restoreMask = 0xABCDABCD;
        Hwi_reconfig(hwiHandle, reconfigTestFunc, &hwiParamsCsl);
        if ((0xABCDABCD != hwiHandle->disableMask) || (0xABCDABCD != hwiHandle->restoreMask))
        {
            result = osal_FAILURE;
        }
        
        /* Pass a garbage parameters, so that reconfig will erminate abruptly. */
        Hwi_reconfig((Hwi_Handle)0xABCDABCD, reconfigTestFunc, &hwiParamsCsl);
        hwiParamsCsl.maskSetting = Hwi_MaskingOption_LOWER;
        hwiParamsCsl.disableMask = 0xABCDABCD;
        hwiParamsCsl.restoreMask = 0xABCDABCD;
        Hwi_reconfig(hwiHandle, reconfigTestFunc, &hwiParamsCsl);
        hwiParamsCsl.eventId = CSL_INVALID_EVENT_ID;
        hwiParamsCsl.enableInt = BFALSE;
        Hwi_reconfig(hwiHandle, reconfigTestFunc, &hwiParamsCsl);

#endif

        if(HwiP_OK != HwiP_delete(handle1))
        {
            result = osal_FAILURE;
        }
    }
    if(osal_OK != result)
    {
        OSAL_log("\n HwiP create Alloc overflow test failed!\n");
    }

    return result;
}

static int32_t OsalApp_hwiDeleteNegativeTest(void)
{
    HwiP_Params    hwiParams;
    HwiP_Handle    handle;
    HwiP_Handle    junkHandle;
    uint8_t        tempStackVar[8];
    int32_t        result = osal_OK;
    uint32_t       count = 0U;
    
    HwiP_Params_init(&hwiParams);

    handle = HwiP_create(OSAL_APP_IRQ_INT_NUM, (HwiP_Fxn)OsalApp_hwiIRQ, &hwiParams);

    /* Here handleAddr is used to get the memory location of the handle
     * we are corrupting the content of the handle and passing in a corrupt handle to the driver
     * to test negative condition for HwiP_delete API
     */
    uint32_t *handleAddr = (uint32_t *)handle, temp;
    temp = (*handleAddr);
    (*handleAddr) = 0U;

    if((NULL_PTR == handle) || (HwiP_OK == HwiP_delete(handle)))
    {
        result = osal_FAILURE;
    }

    /* Restore the value in the handle and delete the created Hwi. */
    *handleAddr = temp;
    if((osal_OK != result) || (HwiP_OK != HwiP_delete(handle)))
    {
        result = osal_FAILURE;
    }

    /* This handle is already deleted, but we are setting the 
     * isUsed parameter to 1(forced corruption), to see how the driver reacts. */
    if(osal_OK == result)
    {
        for(count = 0U; count < 2U; count++)
        {
            *handleAddr = 1U;
            if(HwiP_OK != HwiP_delete(handle))
            {
                break;
            }
        }
    }

    memset(tempStackVar, 0xFF, 8U);
    junkHandle = (HwiP_Handle)tempStackVar;
    /* Hwi_finalize will return abruptly as passed handle is a junk. Nothing to test here. */
    HwiP_delete(junkHandle);

    if(osal_OK != result)
    {
        OSAL_log("\n Hwi Delete negative test failed!\n");
    }
    
    return result;
}

#if defined(BUILD_C7X) 
static int32_t  OsalApp_hwiCreateNegativeTest(void)
{
    HwiP_Params hwiParams;
    HwiP_Handle handle;
    int32_t result = osal_OK;

    HwiP_Params_init(&hwiParams); 
    
    hwiParams.priority  = 0U;
    hwiParams.enableIntr  = 0U;
    
    /*when OSAL_APP_IRQ_INT_NUM 64 for c7x,Hwi_construct API will fails
    and HwiP_create API will return NULL_PTR */
    handle = HwiP_create(INVALID_INT_NUM_C7X, (HwiP_Fxn)OsalApp_hwiIRQ, &hwiParams);
    
    /* Negative test for getEventId in CSL Arch */
    if( (-1) != Hwi_getEventId(INVALID_INT_NUM_C7X))
    {
       result = osal_FAILURE;
    }

    if (NULL_PTR != handle)
    {
       result = osal_FAILURE;
    }
    
    if(osal_OK == result)
    {
        OSAL_log("\n Hwi Create negative test passed!\n");
    }
    else
    {
        OSAL_log("\n Hwi Create negative test failed!\n");
    }
    return result;
  
}

static int32_t OsalApp_hwiCreateMaxTest(void)
{
    HwiP_Params     hwiParams;
    HwiP_Handle     handle[OSAL_APP_HWI_MAX_NUM];
    int32_t         result = osal_OK;
    uint32_t        hwiIndex = 0U, maxHwi;

    HwiP_Params_init(&hwiParams);

    /* Testing Maximum Hwi creation */
    for(hwiIndex = 0U; hwiIndex <= OSAL_APP_HWI_MAX_NUM; hwiIndex++)
    {
        handle[hwiIndex] = HwiP_create(hwiIndex, (HwiP_Fxn)OsalApp_hwiIRQ, &hwiParams);
        if(NULL_PTR == handle[hwiIndex])
        {
            if(OSAL_INUSE_INTR != hwiIndex)
            {
                break;
            }
        }
    }
    maxHwi = hwiIndex;
    for(hwiIndex = 0U; hwiIndex < maxHwi; hwiIndex++)
    {
        if(HwiP_OK != HwiP_delete(handle[hwiIndex]))
        {
            if(OSAL_INUSE_INTR != hwiIndex)
            {
                result = osal_FAILURE;
            }
        }
    }

    if(osal_OK != result)
    {
        OSAL_log("\n Multiple Hwi create test failed! \n");
    }
    
    return result;
}
#endif

#if defined (SAFERTOS) && (defined (BUILD_MCU) || defined (BUILD_C7X))
static void OsalApp_preISRHook(void)
{
    gOsalPreISRHookCount++;
}

static void OsalApp_postISRHook(void)
{

    gOsalPostISRHookCount++;
}
#endif


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_hwiTests(void)
{
    int32_t result = osal_OK;
#if defined(BUILD_MCU)
    CSL_R5ExptnHandlers handlers;

    Intc_InitExptnHandlers(&handlers);
    Intc_RegisterExptnHandlers(&handlers);
#endif

#if defined (SAFERTOS) && (defined (BUILD_MCU) || defined (BUILD_C7X))
    Osal_ISRHooks isrHooks;
    Osal_initISRHooks(&isrHooks);
    isrHooks.preISRHook=(IsrHookPtr)OsalApp_preISRHook;
    isrHooks.postISRHook=(IsrHookPtr)OsalApp_postISRHook;
    Osal_registerISRHooks(&isrHooks);
#endif

#if defined(BUILD_C7X) 
    result += OsalApp_hwiCreateNegativeTest();
    result += OsalApp_hwiCreateMaxTest();
#endif
    result += OsalApp_hwiCreateAllocOvrflwTest();
    result += OsalApp_hwiNullTest();
    result += OsalApp_hwiDeleteNegativeTest();
#if defined (BUILD_MCU)
    result += OsalApp_hwiCreatePulseIntrTest();
#endif
#if defined (BUILD_MCU) && defined (FREERTOS)
    result += OsalApp_hwiCreateFIQTest(CSL_VIM_INTR_TYPE_LEVEL);
    result += OsalApp_hwiCreateFIQTest(CSL_VIM_INTR_TYPE_PULSE);
#endif

    if(osal_OK == result)
    {
        OSAL_log("\n All HwiP tests have Passed!!\n");
    }
    else
    {
        OSAL_log("\n Some or all HwiP tests have Failed!!\n");
    }

    return result;
}

