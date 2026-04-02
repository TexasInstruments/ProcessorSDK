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
 *  ======== Arch_util_c6x.c ========
 */

#include <c6x.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/csl/csl_tsc.h>
#include <ti/osal/src/nonos/Nonos_config.h>
#include <ti/csl/tistdtypes.h>

/* Local structure definition */
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

/* Below function posts the interrupt delay */
static void OsalArch_delay( void );

void osalArch_Init (osalArch_Config_t *cfg)
{
    gOsalArchConfig = *cfg;
}

static CSL_IntcContext               gContext;
static CSL_IntcEventHandlerRecord    gEventRecord[OSAL_NONOS_CONFIGNUM_HWI];
static bool gFirstTime = BFALSE;
static bool gHwiInitialized = BFALSE;
#if !defined(FREERTOS)
static bool gTimestampFirstTime = BTRUE;
#endif
/* This function enables the interrupt for a given interrupt number */
void OsalArch_enableInterrupt(uint32_t intNum)
{
    (void)CSL_intcInterruptEnable((CSL_IntcVectId)intNum);
    return;
}

/* This function disables the interrupt for a given interrupt number */
void OsalArch_disableInterrupt(uint32_t intNum)
{
    (void)CSL_intcInterruptDisable((CSL_IntcVectId)intNum);
    return;
}

/* Below function clears interrupt in the chip level */
void OsalArch_clearInterrupt(uint32_t intNum)
{
    CSL_intcInterruptClear((CSL_IntcVectId)intNum);
    return;
}
extern cregister volatile uint32_t IRP;
extern cregister volatile uint32_t IER;
extern cregister volatile uint32_t ISR;
extern cregister volatile uint32_t ICR;
extern cregister volatile uint32_t ISTP;


static void OsalArch_delay( void )
{
   asm ("  NOP 4  "); /* Compliant */
}

/* Below function posts the interrupt */
int32_t OsalArch_postInterrupt(uint32_t intrNum)
{
    ISR = ((uint32_t)1U << intrNum);
    OsalArch_delay();

    return (osal_UNSUPPORTED);
}

/* Below function globally disable interrupt in the chip level */
uintptr_t OsalArch_globalDisableInterrupt(void)
{
  return (uintptr_t)(_disable_interrupts());
}

/* Below function globally restore interrupt in the chip level */
void OsalArch_globalRestoreInterrupt (uintptr_t restoreValue)
{
    (void)_restore_interrupts(restoreValue);
}

/* Below function initializes the intc module,
   this is needed only once per start */
void OsalArch_oneTimeInit(void)
{
     if (BFALSE == gFirstTime) {
         /* record the index in the handle */
         gContext.numEvtEntries      = (Uint16)OSAL_NONOS_CONFIGNUM_HWI;
         gContext.eventhandlerRecord = gEventRecord;
         (void)CSL_intcInit(&gContext);
         gFirstTime = BTRUE;
     }
}

/* Below function registers the interrupt for a given ISR */
HwiP_Handle OsalArch_HwiPCreate(uint32_t interruptNum, HwiP_Fxn hwiFxn,
                          const HwiP_Params *params)
{
    Hwi_Struct        *hwi_handle = (Hwi_Struct *) NULL;
    CSL_IntcParam     vectId;
    uint32_t          i;
    uintptr_t         key;
    uintptr_t         temp;
    HwiP_nonOs       *hwiPool;
    uint32_t          maxHwi;
    HwiP_Handle       retHandle;

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

    if (NULL_PTR == params)
    {
        retHandle = NULL_PTR;
    }
    else
    {
    
      key = OsalArch_globalDisableInterrupt();
      for (i = 0U; i < maxHwi; i++) 
      {
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
         OsalArch_oneTimeInit();
         (void)CSL_intcGlobalNmiEnable();
         if (BFALSE == gOsalArchConfig.disableIrqOnInit)
         {
            (void)CSL_intcGlobalEnable((CSL_IntcGlobalEnableState *)NULL);
         }

         vectId = (CSL_IntcParam)interruptNum;
         hwi_handle->handle = CSL_intcOpen (&hwi_handle->intcObj, (CSL_IntcEventId)params->evtId, &vectId, (CSL_Status *) NULL);
     
         if(NULL_PTR != hwi_handle->handle)
         {
             CSL_IntcEventHandlerRecord  evtHandler;
             evtHandler.handler = (CSL_IntcEventHandler)hwiFxn;
             evtHandler.arg     = (void *) params->arg;
     
             (void)CSL_intcPlugEventHandler(hwi_handle->handle, &evtHandler);
             (void)CSL_intcHwControl(hwi_handle->handle,CSL_INTC_CMD_EVTCLEAR,NULL_PTR);
             (void)CSL_intcHwControl(hwi_handle->handle,CSL_INTC_CMD_EVTENABLE,NULL_PTR);

             /* Enabling the interrupt if configured */
             if (UTRUE == params->enableIntr)
             {
                 /* Enabling the interrupt in INTC. */
                 OsalArch_enableInterrupt(interruptNum);
             }
             else
             {
                 /* Disabling the interrupt in INTC. */
                 OsalArch_disableInterrupt(interruptNum);
             }
         }
         else
         {
             /* Free the pool */
             hwiPool[i].used = BFALSE;
             retHandle = NULL_PTR;
         }
    }

    return ( (HwiP_Handle) (retHandle) );  /* _TMS320C6X */
}

HwiP_Status OsalArch_HwiPDelete(HwiP_Handle handle)
{
    CSL_Status  status = CSL_SOK;
    HwiP_nonOs *hwi_hnd = (HwiP_nonOs*) handle;
    uintptr_t   key;
    HwiP_Status ret_val = HwiP_FAILURE;

    /* mark that handle as free */
    key = OsalArch_globalDisableInterrupt();
    if (BTRUE == hwi_hnd->used)
    {
        hwi_hnd->used = BFALSE;
        if(CSL_SOK != CSL_intcUnplugEventHandler(hwi_hnd->hwi.handle))
        {
            ret_val = (HwiP_FAILURE);
        }
        status = CSL_intcClose(hwi_hnd->hwi.handle);

        if (CSL_SOK == status)
        {
            ret_val = (HwiP_OK);
        }
        else
        {
            ret_val = (HwiP_FAILURE);
        }
    }
    OsalArch_globalRestoreInterrupt(key);
    return (ret_val);
}

/*
 *  ======== HwiP_getHandle ========
 *  Returns the HwiP handle associated with an interrupt number 
  */
HwiP_Handle OsalArch_getHandle(uint32_t interruptNum)
{
   uint32_t i;
   Hwi_Struct *handle= (Hwi_Struct *) NULL, *handle_temp;
   uintptr_t         temp;
   HwiP_nonOs       *hwiPool;
   uint32_t          maxHwi = 0U;

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
   }

   /* Fetch it if it is already allocated, else return NULL_PTR */
     /* Go through the list and find out if there is an HWI whose interrupt number matches */

     /* First go through the external memory pool */
     for (i = 0U; i < maxHwi; i++)
     {
         if(BTRUE == hwiPool[i].used) {
             handle_temp=&hwiPool[i].hwi;
             if(handle_temp->intcObj.vectId == (CSL_IntcVectId)interruptNum) {
                handle=handle_temp;
                break;
             }
         }
     }

     /* Now check the internal static pool, if not found already */
     if (NULL_PTR != handle)
     {
         for(i = 0U; i < OSAL_NONOS_CONFIGNUM_HWI; i++) {
              if(BTRUE == hwiStructs[i].used) {
                  handle_temp = &hwiStructs[i].hwi;
                  if(handle_temp->intcObj.vectId == (CSL_IntcVectId)interruptNum) {
                     handle = handle_temp;
                     break;
                  }
              }
         }
     }
    return((HwiP_Handle)handle);
}
/*
 *  ======== HwiP_getEventId ========
 *  Returns the Event ID associated with an interrupt
  */
uint32_t OsalArch_getEventId(uint32_t interruptNum)
{
   Hwi_Struct *handle;
   uint32_t retVal = CSL_INVALID_EVENT_ID;
   /* Find the handle registered to this interrupt number */
   handle = (Hwi_Struct *)OsalArch_getHandle(interruptNum);
   if(NULL_PTR != handle) {
      retVal = handle->intcObj.eventId;
   } 
   
   return(retVal);
}

/* Return the cycle frequency used for timeStamp */
uint32_t  osalArch_TimeStampGetFreqKHz(void)
{
    /* TSCH/TSCL runs at CPU speed*/
    return (gOsal_HwAttrs.cpuFreqKHz);
}

/* Initialize the time stamp module */
void    osalArch_TimestampInit(void)
{
    /* FreeRTOS C66x port already initialized TSC counter as part of 
     * schedular start for runtime measurement */
#if !defined(FREERTOS)
    if (BTRUE == gTimestampFirstTime)
    {
        /* Initialize TSCL to 0, for count */
        CSL_tscEnable();
        gTimestampFirstTime = BFALSE;
    }
#endif

    return;
}

/* Osal time stamp provider implementations */
void osalArch_TimestampGet64(TimeStamp_Struct *tStamp)
{
    uintptr_t  key;
    uint64_t   cycle, cycleHi;
    uint32_t   lo, hi;

    osalArch_TimestampInit();

    key     = HwiP_disable();
    cycle   = CSL_tscRead();
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
