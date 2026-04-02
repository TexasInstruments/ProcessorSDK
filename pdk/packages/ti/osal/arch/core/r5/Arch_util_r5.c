/*
 * Copyright (c) 2016-2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== Arch_util_r5.c ========
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/arch/r5/interrupt.h>
#include <ti/osal/src/nonos/Nonos_config.h>
#include <ti/osal/soc/osal_soc.h>

/* Local define */
#define HWIP_R5F_DEFAULT_PRIORITY    ((uint32_t) 0x0FU)

#define OSAL_ARCH_UTIL_ZERO          ((uint32_t) 0U)

/* Local hwi structures */

typedef struct HwiP_nonOs_s {
    bool        used;
    Hwi_Struct  hwi;
} HwiP_nonOs;

/* Local hwi structures */
static HwiP_nonOs hwiStructs[OSAL_NONOS_CONFIGNUM_HWI];

osalArch_Config_t gOsalArchConfig =
{
    .disableIrqOnInit = BFALSE,
};

void osalArch_Init (osalArch_Config_t *cfg)
{
    gOsalArchConfig = *cfg;
}

static bool gFirstTime = BTRUE;
static bool gHwiInitialized = BFALSE;
#if !defined(FREERTOS)
static bool gTimestampFirstTime = BTRUE;
static TimeStamp_Struct gTimeStamp = {(uint32_t)NULL,(uint32_t)NULL};
#endif
static CSL_vimRegs *gVimRegs;

/* This function enables the interrupt for a given interrupt number */
void OsalArch_enableInterrupt(uint32_t intNum)
{
    Intc_IntEnable((uint16_t)intNum);
    return;
}

/* This function disables the interrupt for a given interrupt number */
void OsalArch_disableInterrupt(uint32_t intNum)
{
    Intc_IntDisable((uint16_t)intNum);
    return;
}

/* Below function clears interrupt in the chip level */
void OsalArch_clearInterrupt(uint32_t intNum)
{
    Intc_IntClrPend((uint16_t)intNum);
    return;
}
/* Below function globally disable interrupt in the chip level */
uintptr_t OsalArch_globalDisableInterrupt(void)
{
    return ((uintptr_t)Intc_SystemDisable());
}

/* Below function posts the interrupt */
int32_t OsalArch_postInterrupt(uint32_t intrNum)
{
    return (CSL_vimSetIntrPending(gVimRegs, intrNum));
}


/* Below function globally restore interrupt in the chip level */
void OsalArch_globalRestoreInterrupt (uintptr_t restoreValue)
{
    Intc_SystemRestore(restoreValue);
}

/* Below function registers the interrupt for a given ISR */
HwiP_Handle OsalArch_HwiPCreate(uint32_t interruptNum, HwiP_Fxn hwiFxn,
                         const HwiP_Params *params)
{
    Hwi_Struct                   *hwi_handle = NULL;

    uint32_t i;
    uintptr_t key;
    uint16_t priority;
    uintptr_t         temp;
    HwiP_nonOs       *hwiPool;
    uint32_t          maxHwi;
    HwiP_Handle       retHandle = NULL_PTR;
    CSL_ArmR5CPUInfo  info = {0U, 0U, 0U};

    /* Check if user has specified any memory block to be used, which gets
     * the precedence over the internal static memory block
     */
    if ((uintptr_t)(0U) != gOsal_HwAttrs.extHwiPBlock.base)
    {
        /* pick up the external memory block configured */
        hwiPool        = (HwiP_nonOs *) gOsal_HwAttrs.extHwiPBlock.base;
        temp           = ((uintptr_t) hwiPool) + gOsal_HwAttrs.extHwiPBlock.size;
        maxHwi         = (uint32_t)(temp/(sizeof(Hwi_Struct)));
    }
    else
    {
        /* Pick up the internal static memory block */
        hwiPool        = (HwiP_nonOs *) &hwiStructs[0];
        maxHwi         = OSAL_NONOS_CONFIGNUM_HWI;

        if(BFALSE == gHwiInitialized)
        {
          /* Initializing the first time */
          (void)memset((void *)hwiStructs,0,sizeof(hwiStructs));
          gHwiInitialized = BTRUE;
        }
    }

    if (NULL_PTR != params)
    {

    key = OsalArch_globalDisableInterrupt();
    for (i = 0U; i < maxHwi; i++) {
        if (BFALSE == hwiPool[i].used) {
            hwiPool[i].used = BTRUE;
            break;
        }
    }
    OsalArch_globalRestoreInterrupt(key);

    if (i != maxHwi)
    {
      hwi_handle = &(hwiPool[i].hwi);
      retHandle  = (HwiP_Handle)&hwiPool[i];
    }
    else
    {
      retHandle  = (HwiP_Handle)(NULL);
    }

    if (NULL_PTR != hwi_handle)
    {
        if (BTRUE == gFirstTime)
        {
            Intc_Init();
            CSL_armR5GetCpuID(&info);
#if defined(CSL_MAIN_DOMAIN_VIM_BASE_ADDR0) && defined(CSL_MAIN_DOMAIN_VIM_BASE_ADDR1)
            if (CSL_ARM_R5_CLUSTER_GROUP_ID_0 == info.grpId)
            {
                /* MCU SS Pulsar R5 SS */
                if (CSL_ARM_R5_CPU_ID_0 == info.cpuID)
                {
                    gVimRegs = (CSL_vimRegs *)(uintptr_t)CSL_MCU_DOMAIN_VIM_BASE_ADDR0;
                }
                else
                {
                    gVimRegs = (CSL_vimRegs *)(uintptr_t)CSL_MCU_DOMAIN_VIM_BASE_ADDR1;
                }
            }
            else
            {
                /* MAIN SS Pulsar R5 SS */
                if (CSL_ARM_R5_CPU_ID_0 == info.cpuID)
                {
                    gVimRegs = (CSL_vimRegs *)(uintptr_t)CSL_MAIN_DOMAIN_VIM_BASE_ADDR0;
                }
                else
                {
                    gVimRegs = (CSL_vimRegs *)(uintptr_t)CSL_MAIN_DOMAIN_VIM_BASE_ADDR1;
                }
            }
#else
            /* MCU SS Pulsar R5 SS */
            if (CSL_ARM_R5_CPU_ID_0 == info.cpuID)
            {
                gVimRegs = (CSL_vimRegs *)(uintptr_t)CSL_MCU_DOMAIN_VIM_BASE_ADDR0;
            }
            else
            {
                gVimRegs = (CSL_vimRegs *)(uintptr_t)CSL_MCU_DOMAIN_VIM_BASE_ADDR1;
            }
#endif
            gFirstTime = BFALSE;
            if (BFALSE == gOsalArchConfig.disableIrqOnInit)
            {
                Intc_SystemEnable();
            }
        }

        /* Disable the interrupt first */
        Intc_IntDisable((uint16_t)interruptNum);

        /* Set the trigger type */
        if ((OSAL_ARM_GIC_TRIG_TYPE_HIGH_LEVEL == params->triggerSensitivity) ||
            (OSAL_ARM_GIC_TRIG_TYPE_LEVEL      == params->triggerSensitivity) ||
            (OSAL_ARM_GIC_TRIG_TYPE_LOW_LEVEL  == params->triggerSensitivity))
        {
            Intc_IntSetSrcType((uint16_t)interruptNum, CSL_VIM_INTR_TYPE_LEVEL);
        }
        else
        {
            Intc_IntSetSrcType((uint16_t)interruptNum, CSL_VIM_INTR_TYPE_PULSE);
        }

        hwi_handle->intNum = interruptNum;

        /* Set the priority to default priority if priority is set un-initialized */
        if ((params->priority > CSL_VIM_PRI_INT_VAL_MAX) ||
            (HWIP_USE_DEFAULT_PRIORITY == params->priority))
        {
            priority = (uint16_t)HWIP_R5F_DEFAULT_PRIORITY;
        }
        else
        {
            priority = (uint16_t)params->priority;
        }

        /* Setting the priority for the UART interrupt in INTC. */
        Intc_IntPrioritySet((uint16_t)interruptNum, priority, 0);

        /* Registering the Interrupt Service Routine(ISR). */
        Intc_IntRegister((uint16_t)interruptNum, (IntrFuncPtr) hwiFxn, (void *)params->arg);

        /* Enabling the interrupt if configured */
        if (UTRUE == params->enableIntr)
        {
            /* Enabling the interrupt in INTC. */
            Intc_IntEnable((uint16_t)interruptNum);
        }
        else
        {
            /* Disabling the interrupt in INTC. */
            Intc_IntDisable((uint16_t)interruptNum);
        }
    }
  }
    return ((HwiP_Handle) (retHandle) );

}

/* Below function registers the direct interrupt for a given ISR */
HwiP_Handle OsalArch_HwiPCreateDirect(uint32_t interruptNum, HwiP_DirectFxn hwiFxn,
                                      const HwiP_Params *params)
{
    Hwi_Struct                   *hwi_handle = NULL;

    uint32_t i;
    uintptr_t key;
    uintptr_t         temp;
    HwiP_nonOs       *hwiPool;
    uint32_t          maxHwi;
    HwiP_Handle       retHandle = NULL_PTR;
    CSL_ArmR5CPUInfo  info = {0U, 0U, 0U};
    uint32_t          intrSrcType;

    /* Check if user has specified any memory block to be used, which gets
     * the precedence over the internal static memory block
     */
    if ((uintptr_t)(0U) != gOsal_HwAttrs.extHwiPBlock.base)
    {
        /* pick up the external memory block configured */
        hwiPool        = (HwiP_nonOs *) gOsal_HwAttrs.extHwiPBlock.base;
        temp           = ((uintptr_t) hwiPool) + gOsal_HwAttrs.extHwiPBlock.size;
        maxHwi         = (uint32_t)(temp/(sizeof(Hwi_Struct)));
    }
    else
    {
        /* Pick up the internal static memory block */
        hwiPool        = (HwiP_nonOs *) &hwiStructs[0];
        maxHwi         = OSAL_NONOS_CONFIGNUM_HWI;

        if(BFALSE == gHwiInitialized)
        {
          /* Initializing the first time */
          (void)memset((void *)hwiStructs,0,sizeof(hwiStructs));
          gHwiInitialized = BTRUE;
        }
    }

    if (NULL_PTR != params)
    {

        key = OsalArch_globalDisableInterrupt();
        for (i = 0U; i < maxHwi; i++) {
            if (BFALSE == hwiPool[i].used) {
                hwiPool[i].used = BTRUE;
                break;
            }
        }
        OsalArch_globalRestoreInterrupt(key);

        if (i != maxHwi)
        {
            hwi_handle = &(hwiPool[i].hwi);
            retHandle  = (HwiP_Handle)&hwiPool[i];
        }
        else
        {
            retHandle  = (HwiP_Handle)(NULL);
        }

        if (NULL_PTR != hwi_handle)
        {
            if (BTRUE == gFirstTime)
            {
                Intc_Init();
                CSL_armR5GetCpuID(&info);
#if defined(CSL_MAIN_DOMAIN_VIM_BASE_ADDR0) && defined(CSL_MAIN_DOMAIN_VIM_BASE_ADDR1)
                if (CSL_ARM_R5_CLUSTER_GROUP_ID_0 == info.grpId)
                {
                    /* MCU SS Pulsar R5 SS */
                    if (CSL_ARM_R5_CPU_ID_0 == info.cpuID)
                    {
                        gVimRegs = (CSL_vimRegs *)(uintptr_t)CSL_MCU_DOMAIN_VIM_BASE_ADDR0;
                    }
                    else
                    {
                        gVimRegs = (CSL_vimRegs *)(uintptr_t)CSL_MCU_DOMAIN_VIM_BASE_ADDR1;
                    }
                }
                else
                {
                    /* MAIN SS Pulsar R5 SS */
                    if (CSL_ARM_R5_CPU_ID_0 == info.cpuID)
                    {
                        gVimRegs = (CSL_vimRegs *)(uintptr_t)CSL_MAIN_DOMAIN_VIM_BASE_ADDR0;
                    }
                    else
                    {
                        gVimRegs = (CSL_vimRegs *)(uintptr_t)CSL_MAIN_DOMAIN_VIM_BASE_ADDR1;
                    }
                }
#else
                /* MCU SS Pulsar R5 SS */
                if (CSL_ARM_R5_CPU_ID_0 == info.cpuID)
                {
                    gVimRegs = (CSL_vimRegs *)(uintptr_t)CSL_MCU_DOMAIN_VIM_BASE_ADDR0;
                }
                else
                {
                    gVimRegs = (CSL_vimRegs *)(uintptr_t)CSL_MCU_DOMAIN_VIM_BASE_ADDR1;
                }
#endif
                gFirstTime = BFALSE;
                if (BFALSE == gOsalArchConfig.disableIrqOnInit)
                {
                    Intc_SystemEnable();
                }
            }

            /* Disable the interrupt first */
            Intc_IntDisable((uint16_t)interruptNum);

            /* Set the trigger type */
            if ((OSAL_ARM_GIC_TRIG_TYPE_HIGH_LEVEL == params->triggerSensitivity) ||
                (OSAL_ARM_GIC_TRIG_TYPE_LEVEL      == params->triggerSensitivity) ||
                (OSAL_ARM_GIC_TRIG_TYPE_LOW_LEVEL  == params->triggerSensitivity))
            {
                intrSrcType = CSL_VIM_INTR_TYPE_LEVEL;
                Intc_IntSetSrcType((uint16_t)interruptNum, CSL_VIM_INTR_TYPE_LEVEL);
            }
            else
            {
                intrSrcType = CSL_VIM_INTR_TYPE_PULSE;
                Intc_IntSetSrcType((uint16_t)interruptNum, CSL_VIM_INTR_TYPE_PULSE);
            }

            hwi_handle->intNum = interruptNum;

            /* Registering the Interrupt Service Routine(ISR). */
            (void)CSL_vimCfgIntr( (CSL_vimRegs *)gVimRegs, interruptNum,
                           params->priority, (CSL_VimIntrMap)0x0U,
                           (CSL_VimIntrType)intrSrcType, (uint32_t)hwiFxn );

            /* Enabling the interrupt if configured */
            if (UTRUE == params->enableIntr)
            {
                /* Enabling the interrupt in INTC. */
                Intc_IntEnable((uint16_t)interruptNum);
            }
            else
            {
                /* Disabling the interrupt in INTC. */
                Intc_IntDisable((uint16_t)interruptNum);
            }
        }
    }
    return ((HwiP_Handle) (retHandle) );

}

HwiP_Status OsalArch_HwiPDelete(HwiP_Handle handle)
{
    HwiP_nonOs *hwi_hnd = (HwiP_nonOs*) handle;
    uintptr_t   key;
    HwiP_Status ret_val = HwiP_OK;

    /* mark that handle as free */
    key = OsalArch_globalDisableInterrupt();
    if (BTRUE == hwi_hnd->used)
    {
        hwi_hnd->used = BFALSE;
        Intc_IntUnregister((uint16_t)(hwi_hnd->hwi.intNum));
        /* Disabling the interrupt in INTC. */
        Intc_IntDisable((uint16_t)(hwi_hnd->hwi.intNum));
    }
    else
    {
        ret_val = HwiP_FAILURE;
    }
    OsalArch_globalRestoreInterrupt(key);
    return (ret_val);
}

/* Return the cycle frequency used for timeStamp */
uint32_t  osalArch_TimeStampGetFreqKHz(void)
{
    /* R5F time Stamp is via PMU, which runs at CPU speed */
    return (gOsal_HwAttrs.cpuFreqKHz);
}

/* Initialize the time stamp module */
void    osalArch_TimestampInit(void)
{
    /* FreeRTOS R5F port already initialized PMU counter as part of 
     * schedular start for runtime measurement */
#if !defined(FREERTOS)
    if (BTRUE == gTimestampFirstTime)
    {
        osal_TimestampProvider_initCCNT();
        /* One time initialization is done */
        gTimestampFirstTime = BFALSE;
    }
#endif

    return;
}

/* Osal time stamp provider implementations */
void osalArch_TimestampGet64(TimeStamp_Struct *tStamp)
{
    if (NULL_PTR != tStamp)
    {
    #if defined(FREERTOS)
        /* FreeRTOS R5F port handles PMU counter init and overflow.
         * Hence use the same and avoid duplicate overflow handle */
        uint64_t   cycle, cycleHi;
        uint32_t   lo, hi;
        
        extern uint64_t uxPortReadPmuCounter(void);
        
        cycle = uxPortReadPmuCounter();
        cycleHi = ((uint64_t)(cycle >> 32U));

        /* get the lo and hi parts */
        lo    = ((uint32_t)(cycle   & ((uint32_t)(0xFFFFFFFFU))));
        hi    = ((uint32_t)(cycleHi & ((uint32_t)(0xFFFFFFFFU))));

        tStamp->lo         = lo;
        tStamp->hi         = hi;
    #else
        uint32_t    lo, ovsrStatus;

        /* Make sure init is done, if not done already */
        osalArch_TimestampInit();
        lo = CSL_armR5PmuReadCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM);

        ovsrStatus = osal_TimestampProvider_getOverflowCCNT();

        if (OSAL_ARCH_UTIL_ZERO != ovsrStatus)
        {
            lo = CSL_armR5PmuReadCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM);
            gTimeStamp.hi++;
        }

        gTimeStamp.lo      = lo;
        tStamp->lo         = lo;
        tStamp->hi         = gTimeStamp.hi;
    #endif
    }
    /* return time in micro seconds */
    return;
}

/* Needs to be run at least once after a over flow happens and before next overflow */
void osalArch_TimestampCcntAutoRefresh(uintptr_t arg)
{
    /* FreeRTOS R5F port handles PMU counter overflow and checks periodically 
     * in every OS tick.
     * Hence avoid duplicate overflow handle */
#if !defined(FREERTOS)
    uint32_t    ovsrStatus;
    ovsrStatus = osal_TimestampProvider_getOverflowCCNT();

    if (OSAL_ARCH_UTIL_ZERO != ovsrStatus)
    {
        gTimeStamp.lo = CSL_armR5PmuReadCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM);
        gTimeStamp.hi++;
    }
#endif

    return;
}

bool Osal_isInAbortContext( void )
{
    bool retVal = BFALSE;
    extern volatile uint32_t gCurrentProcessorState;
    if (CSL_ARM_R5_ABORT_MODE == gCurrentProcessorState)
    {
        retVal = BTRUE;
    }

    return retVal;
}

/* Nothing past this point */
