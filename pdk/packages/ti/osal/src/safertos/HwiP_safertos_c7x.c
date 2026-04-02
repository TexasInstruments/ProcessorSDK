/*
 * Copyright (c) 2022, Texas Instruments Incorporated
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
 *  ======== HwiP_safertos_c7x.c ========
 */

#include <ti/osal/HwiP.h>

#include "SafeRTOS_priv.h"

#define OSAL_SAFERTOS_C7X_CONFIGNUM_HWI                 (64U)

#ifndef OSAL_TARGET_PROC_MASK_DEFAULT
#define OSAL_TARGET_PROC_MASK_DEFAULT (0xFFFFU)
#endif

#ifndef NULL_PTR
#define NULL_PTR ((void *)0x0)
#endif

extern Osal_HwAttrs  gOsal_HwAttrs;

extern void Osal_DebugP_assert(int32_t expression, const char *file, int32_t line);
extern uint32_t  gOsalHwiAllocCnt, gOsalHwiPeak;

/*!
 *  @brief    Hwi structure
 */
typedef struct HwiP_safeRtos_s {
    bool              used;
    HwiC7x_Struct     hwi;
} HwiP_safeRtos;

/* global pool of statically allocated semaphore pools */
static HwiP_safeRtos gOsalHwiPSafeRtosPool[OSAL_SAFERTOS_C7X_CONFIGNUM_HWI];

/*
 *  ======== HwiP_clearInterrupt ========
 */
void HwiP_clearInterrupt(uint32_t interruptNum)
{
    Hwi_clearInterrupt(interruptNum);
}

/*
 *  ======== HwiP_create ========
 */
HwiP_Handle HwiP_create(uint32_t interruptNum, HwiP_Fxn hwiFxn,
                        const HwiP_Params *params)
{
    HwiP_safeRtos *handle = (HwiP_safeRtos *)NULL_PTR;
    Hwi_Params  hwiParams;
    uint32_t          i;
    uintptr_t         key;
    uintptr_t         temp;
    HwiP_safeRtos     *hwiPool;
    uint32_t          maxHwi;
    int32_t           iStat;

    /* Check if user has specified any memory block to be used, which gets
     * the precedence over the internal static memory block
     */
    if((uintptr_t)(0U) != gOsal_HwAttrs.extHwiPBlock.base)
    {
        /* pick up the external memory block configured */
        hwiPool        = (HwiP_safeRtos *) gOsal_HwAttrs.extHwiPBlock.base;
        temp           = (uintptr_t)  gOsal_HwAttrs.extHwiPBlock.size;
        maxHwi         = (uint32_t)(temp/(sizeof(HwiP_safeRtos)));
    }
    else
    {
        /* Pick up the internal static memory block */
        hwiPool        = (HwiP_safeRtos *) &gOsalHwiPSafeRtosPool[0];
        maxHwi         = OSAL_SAFERTOS_C7X_CONFIGNUM_HWI;
        
        if(0U == gOsalHwiAllocCnt) 
        {
            (void)memset((void *)gOsalHwiPSafeRtosPool,0,sizeof(gOsalHwiPSafeRtosPool));
		}
    }

    /* Grab the memory */
    key = HwiP_disable();

    for(i = 0U; i < maxHwi; i++)
    {
        if(BFALSE == hwiPool[i].used)
        {
            hwiPool[i].used = BTRUE;
            /* Update statistics */
            gOsalHwiAllocCnt++;
            if(gOsalHwiAllocCnt > gOsalHwiPeak)
            {
                gOsalHwiPeak = gOsalHwiAllocCnt;
            }
            break;
        }
    }
    HwiP_restore(key);

    if(i < maxHwi)
    {
        /* Grab the memory */
        handle = (HwiP_safeRtos *) &hwiPool[i];
    }

    if(NULL_PTR != handle)
    {
        if(NULL_PTR == params)
        {
            Hwi_Params_init(&hwiParams);
            iStat = Hwi_construct(&handle->hwi, interruptNum, (Hwi_FuncPtr)hwiFxn, &hwiParams);
        }
        else
        {
            Hwi_Params_init(&hwiParams);
            hwiParams.arg            = params->arg;

            if(0U == params->priority) 
            {
               /* A priority of 0 is invalid for many targets. -1 forces 
                  sysbios to assign a default priority */
               hwiParams.priority       = -1;
            }
            else
            {
               hwiParams.priority       = (int32_t)params->priority;
            }

            hwiParams.eventId        = params->evtId;
            if(UTRUE == params->enableIntr)
            {
                hwiParams.enableInt      = BTRUE;
            }
            else
            {
                hwiParams.enableInt      = BFALSE;
            }
            hwiParams.maskSetting    = Hwi_MaskingOption_SELF;
            iStat = Hwi_construct(&handle->hwi, interruptNum, (Hwi_FuncPtr)hwiFxn,
                          &hwiParams);

            if(0 != iStat)
            {
                /* Free the allocated memory and return null */
                handle->used = BFALSE;
                gOsalHwiAllocCnt--;
                handle = (HwiP_safeRtos *) NULL_PTR;
            }
        }
    }

    return ((HwiP_Handle)handle);
}

/*
 *  ======== HwiP_createDirect ========
 */
HwiP_Handle HwiP_createDirect(uint32_t interruptNum, HwiP_DirectFxn hwiFxn,
                              const HwiP_Params *params)
{
    HwiP_Handle handle;
    handle = NULL_PTR;

    /* Not supported */
    return (handle);
}

/*
 *  ======== HwiP_delete ========
 */
HwiP_Status HwiP_delete(HwiP_Handle handle)
{
    uintptr_t   key;
    HwiP_Status ret;
    
    HwiP_safeRtos *hwi = (HwiP_safeRtos *)handle;
    
    if((NULL_PTR != hwi) && (BTRUE == hwi->used)) {
      Hwi_destruct(&hwi->hwi);
      key = HwiP_disable();
      hwi->used = BFALSE;
      /* Found the osal hwi object to delete */
      if(0U < gOsalHwiAllocCnt)
      {
        gOsalHwiAllocCnt--;
      }

      HwiP_restore(key);
      ret = HwiP_OK;
    }
    else
    {
       ret = HwiP_FAILURE;
    } 
    return (ret);
}

/*
 *  ======== HwiP_disable ========
 */
uintptr_t HwiP_disable(void)
{
    uintptr_t key = (uintptr_t)NULL_PTR;

    if(( 1 == Osal_isInISRContext() ) ||
       ( pdFALSE    == xTaskIsSchedulerStarted() ))
    {
        key = Hwi_disable();
    }
    else
    {
        portENTER_CRITICAL_WITHIN_API();
    }

    return (key);
}
/*
 *  ======== HwiP_disableInterrupt ========
 */
void HwiP_disableInterrupt(uint32_t interruptNum)
{
    (void)Hwi_disableInterrupt(interruptNum);
}

/*
 *  ======== HwiP_enableInterrupt ========
 */
void HwiP_enableInterrupt(uint32_t interruptNum)
{
    (void)Hwi_enableInterrupt(interruptNum);
}

/*
 *  ======== HwiP_Params_init ========
 */
void HwiP_Params_init(HwiP_Params *params)
{
    params->name = (char *) NULL_PTR;
    params->arg = 0U;
    params->priority = HWIP_USE_DEFAULT_PRIORITY;
    params->evtId    = EventP_ID_NONE;
    params->enableIntr = UTRUE;
}

/*
 *  ======== HwiP_postInterrupt ========
 */

int32_t HwiP_post(uint32_t interruptNum)
{
    Hwi_post(interruptNum);
    
    return osal_OK; 
    /* Please note that in future,for targets which do not support Hwi_Post,
       add #ifdefs appropriately to return osal_UNSUPPORTED */
}

/*
 *  ======== HwiP_restore ========
 */
void HwiP_restore(uintptr_t key)
{
    if(( 1 == Osal_isInISRContext() ) ||
       ( pdFALSE    == xTaskIsSchedulerStarted() ))
    {
        (void)Hwi_restore((uint32_t)key);
    }
    else
    {
        portEXIT_CRITICAL_WITHIN_API();
    }

    return;
}

