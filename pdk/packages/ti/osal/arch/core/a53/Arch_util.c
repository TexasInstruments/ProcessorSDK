/*
 * Copyright (c) 2017-2018, Texas Instruments Incorporated
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
 *  ======== Arch_util.c ========
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ti/osal/src/nonos/Nonos_config.h>
#include <ti/csl/src/ip/arm_gic/V2/csl_gic.h>
#include <ti/csl/soc.h>
#include <ti/csl/tistdtypes.h>

/* Local define */
#define HWIP_A53_DEFAULT_PRIORITY    ((uint32_t) 0xE0U)

/* Local hwi structures */

typedef struct HwiP_nonOs_s {
    bool        used;
    Hwi_Struct  hwi;
} HwiP_nonOs;

/* Local hwi structures */
static HwiP_nonOs hwiStructs[OSAL_NONOS_CONFIGNUM_HWI];
static bool gTimestampFirstTime = BTRUE;
static bool gHwiInitialized = BFALSE;

#if defined (SOC_AM65XX)
#define GIC_BASE_ADDR               (CSL_GIC0_DISTRIBUTOR_BASE)
#elif defined (SOC_AM64X)
#define GIC_BASE_ADDR               (CSL_GICSS0_GIC_BASE)
#else
#define GIC_BASE_ADDR               (CSL_COMPUTE_CLUSTER0_GIC_DISTRIBUTOR_BASE)
#endif

void OsalArch_gicInit(void)
{
    uint64_t cpuId;
    uint8_t  coreId;
    Bool     initStatus;

    initStatus = Intc_isInitialized();
    if (UFALSE == initStatus)
    {
        cpuId = CSL_a53v8GetCpuId();
        coreId = (uint8_t) cpuId & (uint8_t)0xFFU;
        /* If not initialized via startup, initialze now
         * initStatus FALSE, indicates that the CSL startup
         * is not followed and possibly, some other schemes
         * such as SBL/diag has started up and has done other
         * required initializations, only pending initialization
         * is Arm GIC */
        /* Initialize the GIC V3 */
        Intc_Init(coreId);
    }

    return;
}

/* This function enables the interrupt for a given interrupt number */
void OsalArch_enableInterrupt(uint32_t intNum)
{
    (void)Intc_SystemEnable((uint16_t)intNum);
    return;
}

/* This function disables the interrupt for a given interrupt number */
void OsalArch_disableInterrupt(uint32_t intNum)
{
    (void)Intc_SystemDisable((uint16_t)intNum);
    return;
}

/* Below function clears interrupt in the chip level */
void OsalArch_clearInterrupt(uint32_t intNum)
{
    return;
}

/* Below function posts the interrupt */
int32_t OsalArch_postInterrupt(uint32_t intrNum)
{
    CSL_gic500_gicdRegs *gicdRegs = (CSL_gic500_gicdRegs *)(GIC_BASE_ADDR);
    CSL_gic500_gicrRegs *gicrRegs = (CSL_gic500_gicrRegs *) ( GIC_BASE_ADDR + CSL_GIC500_GICR_CORE_CONTROL_CTLR(0U));
    uint64_t cpuId = CSL_a53GetCpuId();
    uint8_t  coreId;
    cpuId &= 0xFFU;
    coreId = (uint8_t)cpuId;

    if (32U > intrNum)
    {
      (void)CSL_gicSetPendingSgiPpiIntr(gicrRegs, coreId, intrNum);
    }
    else if (960U > intrNum)
    {
      (void)CSL_gicSetPendingSpiIntr(gicdRegs, intrNum);
    }
    else
    {
        /* No action */
    }

    CSL_a53v8DsbSy();

    return (osal_OK);

}

/* Below function globally disable interrupt in the chip level */
uintptr_t OsalArch_globalDisableInterrupt(void)
{
    return ((uintptr_t)CSL_a53v8ArmGicv3HwiDisable());
}

/* Below function globally restore interrupt in the chip level */
void OsalArch_globalRestoreInterrupt (uintptr_t restoreValue)
{
   CSL_a53v8ArmGicv3HwiRestore((uint64_t)restoreValue);
   return;
}

/* Below function registers the interrupt for a given ISR */
HwiP_Handle OsalArch_HwiPCreate(uint32_t interruptNum, HwiP_Fxn hwiFxn,
                         const HwiP_Params *params)
{
    Hwi_Struct                   *hwi_handle = NULL_PTR;

    uint32_t          i;
    uintptr_t         key;
    uint16_t          priority;
    uintptr_t         temp;
    HwiP_nonOs       *hwiPool;
    uint32_t          maxHwi;
    HwiP_Handle       retHandle = NULL_PTR;
    uint64_t          cpuId;
    uint8_t           coreId;

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

    cpuId = CSL_a53GetCpuId();
    coreId = (uint8_t)(cpuId) & (uint8_t)0xFF;
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
      retHandle  = NULL_PTR;
     }
   }

    if (NULL_PTR != hwi_handle)
    {
        /* Registering the Interrupt Service Routine(ISR). */
        Intc_IntRegister((uint16_t)interruptNum, (IntrFuncPtr) hwiFxn, (void *)params->arg);

        /* Set the priority to default priority if priority is set un-initialized */

        if (HWIP_USE_DEFAULT_PRIORITY == params->priority)
        {
            priority = (uint16_t)HWIP_A53_DEFAULT_PRIORITY;
        } else
        {
			priority = (uint16_t)params->priority;
		}

        /* Setting the priority for the interrupts in INTC. */
        Intc_IntPrioritySet((uint16_t)interruptNum, priority,coreId);

        /* Set the trigger type */
        if ((OSAL_ARM_GIC_TRIG_TYPE_HIGH_LEVEL == params->triggerSensitivity) ||
            (OSAL_ARM_GIC_TRIG_TYPE_LEVEL      == params->triggerSensitivity) ||
            (OSAL_ARM_GIC_TRIG_TYPE_LOW_LEVEL  == params->triggerSensitivity))
        {
            (void)Intc_IntAssignLevelIntType((uint16_t)interruptNum);
        }

        if ((OSAL_ARM_GIC_TRIG_TYPE_RISING_EDGE  == params->triggerSensitivity) ||
            (OSAL_ARM_GIC_TRIG_TYPE_EDGE         == params->triggerSensitivity) ||
            (OSAL_ARM_GIC_TRIG_TYPE_FALLING_EDGE == params->triggerSensitivity))
        {
            (void)Intc_IntAssignEdgeIntType((uint16_t)interruptNum);
        }

        /* Return Non Zero value for the handle for A15 target */
        hwi_handle->intNum = interruptNum;

        /* Enabling the interrupt if configured */
        if (UTRUE == params->enableIntr)
        {
            OsalArch_enableInterrupt(interruptNum);
        }
        else
        {
            OsalArch_disableInterrupt(interruptNum);
        }
    }
    return ((HwiP_Handle) (retHandle) );

}

/* Below function deletes the hwi created */
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
    }
    else
    {
        ret_val = HwiP_FAILURE;
    }
    OsalArch_globalRestoreInterrupt(key);
    return (ret_val);
}

/* Initialize the time stamp module */
void    osalArch_TimestampInit(void)
{
    if (BTRUE == gTimestampFirstTime)
    {
        CSL_initGTC();
        gTimestampFirstTime = BFALSE;
    }
    return;
}

/* Return the cycle frequency used for timeStamp */
uint32_t  osalArch_TimeStampGetFreqKHz(void)
{
   /* A53 timeStamp provider is GTC, which is at 200MHz */
   return OSAL_SOC_MPU_GTC_CLK;
}

/* Osal time stamp provider implementations */
void osalArch_TimestampGet64(TimeStamp_Struct *tStamp)
{
    uintptr_t  key;
    uint64_t   cycle, cycleHi;
    uint32_t   lo, hi;

    key        = HwiP_disable();
    /* GTC clock runs at 200 Mhz
       hence a millisec is 200000 cycle counts
     */
     cycle   = CSL_getGTCcount();
     cycleHi = ((uint64_t)(cycle >> 32U));

    /* get the lo and hi parts */
     lo    = ((uint32_t)(cycle   & ((uint32_t)(0xFFFFFFFFU))));
     hi    = ((uint32_t)(cycleHi & ((uint32_t)(0xFFFFFFFFU))));

     tStamp->lo         = lo;
     tStamp->hi         = hi;
     /* restore */
     HwiP_restore(key);
}

void osalArch_TimestampCcntAutoRefresh(uintptr_t arg)
{
    return;
}

/* Nothing past this point */
