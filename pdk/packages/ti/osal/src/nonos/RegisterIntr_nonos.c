/*
 * Copyright (c) 2017-present, Texas Instruments Incorporated
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
 *  ======== RegisterIntr_nonos.c ========
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ti/osal/osal.h>
#include <ti/osal/src/nonos/Nonos_config.h>
#include <ti/osal/RegisterIntr.h>
#include <ti/csl/csl.h>
#ifdef _TMS320C6X
#include <ti/csl/src/intc/csl_intcAux.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined (_TMS320C6X)
#define CSL_COREPAC_MAX_EVENT_ID (128U)
#endif

/*
 * Purpose:     Initializes the interrupt registration config structure
 * Description: Initializes the interrupt registration config structure prior to
 *              invoking the Osal_registerIntr API
 */
void Osal_RegisterInterrupt_initParams(OsalRegisterIntrParams_t *interruptRegParams)
{
  if(NULL_PTR != interruptRegParams) {
      /* Corepac config */
      interruptRegParams->corepacConfig.name = (char *) NULL_PTR;
      interruptRegParams->corepacConfig.corepacEventNum = CSL_INVALID_EVENT_ID;
      interruptRegParams->corepacConfig.intVecNum = CSL_INVALID_VEC_ID;
      interruptRegParams->corepacConfig.isrRoutine = (void (*)(uintptr_t arg)) NULL_PTR;
      interruptRegParams->corepacConfig.arg = (uintptr_t)0U;
      interruptRegParams->corepacConfig.priority = 0x20U; /* Default */
      interruptRegParams->corepacConfig.enableIntr = UTRUE;
#if defined (__ARM_ARCH_7A__)
      interruptRegParams->corepacConfig.triggerSensitivity = 0x3; /* interrupt edge triggered */
#endif

#if defined(__aarch64__) || ((__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R') )
      interruptRegParams->corepacConfig.triggerSensitivity = OSAL_ARM_GIC_TRIG_TYPE_LEVEL; /* interrupt level triggered */
#endif
      /* SOC Mux Config */
      interruptRegParams->socMuxConfig.muxInParams = (MuxIntcP_inParams *) NULL_PTR;
      interruptRegParams->socMuxConfig.muxOutParams = (MuxIntcP_outParams *) NULL_PTR;
  }
  return;
}
/*
 * Purpose:     Registers the interrupt for the event with the params provided
 * Description: Registers the event with the event combiner/Interrupt Vector.
 *
 * Returns:     The Hwi Handle and the return codes defined in OsalInterruptRetCode_e
 */
OsalInterruptRetCode_e Osal_RegisterInterrupt(OsalRegisterIntrParams_t *interruptRegParams,HwiP_Handle *hwiPHandlePtr)
{

    OsalInterruptRetCode_e     ret=OSAL_INT_SUCCESS;
    HwiP_Handle                hwiPHandle=NULL_PTR;
    HwiP_Params                 hwiInputParams;

     /* Program the corepac interrupt */
      if ((NULL==interruptRegParams) ||
          ((void (*)(uintptr_t arg)) NULL_PTR == interruptRegParams->corepacConfig.isrRoutine) ||
          (NULL == hwiPHandlePtr) ||
#if defined (_TMS320C6X)
          (CSL_COREPAC_MAX_EVENT_ID <= interruptRegParams->corepacConfig.corepacEventNum) ||
#endif
          (CSL_INVALID_EVENT_ID == interruptRegParams->corepacConfig.intVecNum)
          )
      {
          ret = OSAL_INT_ERR_INVALID_PARAMS;
      }
      
      else
      {
            HwiP_Params_init(&hwiInputParams);

            hwiInputParams.name = interruptRegParams->corepacConfig.name;
            hwiInputParams.arg  = interruptRegParams->corepacConfig.arg;
            hwiInputParams.priority = interruptRegParams->corepacConfig.priority;
            hwiInputParams.evtId = interruptRegParams->corepacConfig.corepacEventNum;
            hwiInputParams.enableIntr = interruptRegParams->corepacConfig.enableIntr;
#if defined (__aarch64__) || ((__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R') )
            hwiInputParams.triggerSensitivity = interruptRegParams->corepacConfig.triggerSensitivity;
#endif

#ifdef _TMS320C6X
            /* Maps the core_event_in to the hwi Vector core_intVecNum (4-15) for C6x and core_intNum for ARM */
            /*      For C66x
                    Use Event Combiner ALWAYS for c6x. Event_combiner_event (0-3) =  core_event_in/32.
                    Hook the this Event_combiner_event(0-3) to the core_intVecNum.
                    if core_intVecNum is set to -1 as input, it means Let RM find a free unused vector number;
                    HwiP_Create(Event_combiner_event,core_intVecNum,Event_dispatcher_plug);
            */
            if(OSAL_REGINT_INTVEC_EVENT_COMBINER == interruptRegParams->corepacConfig.intVecNum) {
                OsalArch_oneTimeInit();
                /* Map to a particular group */
                if(3U < hwiInputParams.evtId) {
                    /* For C66X the interrupt needs to be grouped to {0,1,2,3} to either of the four 32-bit event registers  */
                    hwiInputParams.evtId = (interruptRegParams->corepacConfig.corepacEventNum)/32U;
                }
                /* The dispatch function in the event combiner case is EventCombiner_dispatch */

                /* Find out if the event combiner is already registered, if so, dont re-register */
                hwiPHandle = EventCombinerP_getHwi(hwiInputParams.evtId);
                if(NULL_PTR == hwiPHandle) {
                    /* The event hasn't been registered yet. Register it as per the defaults provided by OSAL */
                    Osal_HwAttrs hwAttrs;

                        /* Get the default OSAL mapped ones */
                    (void)Osal_getHwAttrs(&hwAttrs);

                    /* Unconditionally enable interrupt if creating interrupt for event combiner */
                    hwiInputParams.enableIntr = UTRUE;

                    /* No need to register seperately in case of baremetal , the HwiP_Create() takes care of it */
                    HwiP_create(hwAttrs.ECM_intNum[hwiInputParams.evtId],interruptRegParams->corepacConfig.isrRoutine, &hwiInputParams);
                    /* Get the interrupt handler corresponding to the event id. */
                    hwiPHandle = EventCombinerP_getHwi(hwiInputParams.evtId);
                
                    if(NULL_PTR == hwiPHandle) {
                        ret = OSAL_INT_ERR_EVENTCOMBINER_REG;
                    }
                }
                if(OSAL_INT_SUCCESS == ret) {
                    /* The Event combiner handle already exists or created. Now plug the ISR routine in to
                        the CSL_intcEventHandlerRecord_p */
                        (void)EventCombinerP_dispatchPlug((uint32_t)interruptRegParams->corepacConfig.corepacEventNum,
                                                    interruptRegParams->corepacConfig.isrRoutine,
                                                    interruptRegParams->corepacConfig.arg,
                                                    (UFALSE != interruptRegParams->corepacConfig.enableIntr));
                }
            } else {
                /* Do not use the event combiner. Use the supplied ISR routine */
                hwiPHandle =  HwiP_create(interruptRegParams->corepacConfig.intVecNum,interruptRegParams->corepacConfig.isrRoutine, &hwiInputParams);
                if(NULL_PTR == hwiPHandle) {
                    ret = OSAL_INT_ERR_HWICREATE;
                }
            }
#else
#if defined (__aarch64__)
        /* Initialize GIC if not done already */
        Osal_HwAttrs hwAttrs;
        (void)Osal_getHwAttrs(&hwAttrs);
        if(OSAL_HWACCESS_UNRESTRICTED == hwAttrs.hwAccessType)
        {
        /* Do GIC init only in the case of unrestricted hw access */
        OsalArch_gicInit();
        }
#endif
        hwiPHandle =  HwiP_create(interruptRegParams->corepacConfig.intVecNum,interruptRegParams->corepacConfig.isrRoutine, &hwiInputParams);
        if(NULL_PTR == hwiPHandle) {
            ret = OSAL_INT_ERR_HWICREATE;
        }
#endif

        *hwiPHandlePtr=hwiPHandle;
      }
      
  return ret ;
}

/*
 * Purpose:     Registers the interrupt for the event with the params provided
 * Description: Registers the event with the event combiner/Interrupt Vector.
 *
 * Returns:     The Hwi Handle and the return codes defined in OsalInterruptRetCode_e
 */
OsalInterruptRetCode_e Osal_RegisterInterruptDirect(OsalRegisterIntrParams_t *interruptRegParams,
                                                    HwiP_DirectFxn isrFxn, HwiP_Handle *hwiPHandlePtr)
{

    OsalInterruptRetCode_e     ret=OSAL_INT_SUCCESS;
#ifndef _TMS320C6X
    HwiP_Handle                hwiPHandle=NULL_PTR;
#endif /* ifndef  */
    HwiP_Params                 hwiInputParams;

    /* Program the corepac interrupt */
    if ((NULL==interruptRegParams) ||
        ( (void (*)(uintptr_t arg)) NULL_PTR == interruptRegParams->corepacConfig.isrRoutine) ||
        (NULL == hwiPHandlePtr) ||
#if defined (_TMS320C6X)
        (CSL_INVALID_EVENT_ID == interruptRegParams->corepacConfig.corepacEventNum) ||
#endif
        (CSL_INVALID_EVENT_ID == interruptRegParams->corepacConfig.intVecNum)
        )
    {
        ret = OSAL_INT_ERR_INVALID_PARAMS;
    }
    else
    {
        HwiP_Params_init(&hwiInputParams);

        hwiInputParams.name = interruptRegParams->corepacConfig.name;
        hwiInputParams.arg  = interruptRegParams->corepacConfig.arg;
        hwiInputParams.priority = interruptRegParams->corepacConfig.priority;
        hwiInputParams.evtId = interruptRegParams->corepacConfig.corepacEventNum;
        hwiInputParams.enableIntr = interruptRegParams->corepacConfig.enableIntr;
#if defined (__ARM_ARCH_7A__) || defined (__aarch64__) || ((__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R') )
        hwiInputParams.triggerSensitivity = interruptRegParams->corepacConfig.triggerSensitivity;
#endif

#ifdef _TMS320C6X
        ret = OSAL_INT_UNSUPPORTED;
#else

#if defined (__aarch64__)
        /* Initialize GIC if not done already */
        Osal_HwAttrs hwAttrs;
        (void)Osal_getHwAttrs(&hwAttrs);
        if(OSAL_HWACCESS_UNRESTRICTED == hwAttrs.hwAccessType)
        {
            /* Do GIC init only in the case of unrestricted hw access */
            OsalArch_gicInit();
        }
#endif

        hwiPHandle =  HwiP_createDirect(interruptRegParams->corepacConfig.intVecNum,isrFxn, &hwiInputParams);
        if(NULL_PTR == hwiPHandle) 
        {
            ret = OSAL_INT_ERR_HWICREATE;
        }
        else
        {
            *hwiPHandlePtr=hwiPHandle;
        }
#endif
    }

    return ret ;
}

/*
 * Purpose:     Deletes/Unregisters an interrupt
 * Description: Deletes/Unregisters an interrupt associated with an event number(0-127).
 *
 * Returns:     OSAL_INT_SUCCESS if successful,OSAL_INT_ERR_DELETE if failed
 */
OsalInterruptRetCode_e Osal_DeleteInterrupt(HwiP_Handle handle,uint32_t corepacEventNum)
{

    HwiP_Status hwiP_ret=HwiP_OK;
    OsalInterruptRetCode_e ret=OSAL_INT_SUCCESS;
#ifdef _TMS320C6X
   /* For C66x, it needs to be found out if this is derived out of
    * an event combiner. If so, do not delete the HWI. Just unregister the function from from the event combiner & disable the event.
    */
    {
       CSL_IntcEventId hwi_eventId;
       
       /* In the baremetal case the handle holds the information (handle->eventId) 
          if this is a event combiner handle, in which case the handle->eventId = [0-3]
        */  
       hwi_eventId = (((Hwi_Struct *)handle)->handle)->eventId;
       if( (0U == hwi_eventId) && (4U > hwi_eventId)) {
           /* This is event combiner, so do not destroy the hwi, but remove the dispatch table entry */
           EventCombinerP_dispatchUnplug(corepacEventNum);
          /* Return Success */
       } else {
          /* This is not an event dispatcher function, so destruct the HwiP as usual */
           hwiP_ret = HwiP_delete(handle);
           if(HwiP_OK != hwiP_ret) {
             ret = OSAL_INT_ERR_DELETE;
           }
       }

    }
#else
    /* For non-c66x cases, just delete the handle */
    hwiP_ret = HwiP_delete(handle);
    if(HwiP_OK != hwiP_ret) {
      ret = OSAL_INT_ERR_DELETE;
    }
#endif
    return(ret);
}



/*
 * Purpose:     Enables an event/interrupt
 * Description: Enables the interrupt associated with an event number(0-127).
 *              In case of the event combiner, the corresponding event is
 *              enabled within the Event Combiner.
 *
 * Returns:     None
 */
void Osal_EnableInterrupt(uint32_t corepacEvent,uint32_t interruptNum)
{
#ifdef _TMS320C6X
    CSL_Reg32    csrRegVal;
    if(OSAL_REGINT_INTVEC_EVENT_COMBINER == interruptNum) {
     /* If this is called from a module which uses event combiner */
     /* Just enable the event inside the event combiner and
      * not the whole interrupt as it will be used by the event dispatcher */
        (void)EventCombinerP_enableEvent(corepacEvent);
        /* Make sure GIE bit in CSR is set for interrupts */
        csrRegVal = CSL_chipReadCSR();
        if (0U == (csrRegVal & CSL_CHIP_CSR_GIE_ENABLE))  {
           csrRegVal |= CSL_CHIP_CSR_GIE_ENABLE;
           (void)CSL_chipWriteCSR(csrRegVal);
        }
    } else {
      /* This is not an event combiner based interrupt */
        HwiP_enableInterrupt(interruptNum);
    }
#else
    /* For non c66x cases, there is no event combiner , just invoke the HwiP */
    HwiP_enableInterrupt(interruptNum);
#endif
}

/*
 * Purpose:     Disables an event/interrupt
 * Description: Disables the interrupt associated with an event number(0-127).
 *              In case of the event combiner, the corresponding event is
 *              disabled within the Event Combiner.
 *
 * Returns:     None
 */
void Osal_DisableInterrupt(uint32_t corepacEvent,uint32_t interruptNum)
{
#ifdef _TMS320C6X
    if(OSAL_REGINT_INTVEC_EVENT_COMBINER == interruptNum) {
     /* If this is called from a module which uses event combiner */
     /* Just enable the event inside the event combiner and
      * not the whole interrupt as it will be used by the event dispatcher */
        (void)EventCombinerP_disableEvent(corepacEvent);
    } else {
      /* This is not an event combiner based interrupt */
        HwiP_disableInterrupt(interruptNum);
    }
#else
    /* For non c66x cases, there is no event combiner , just invoke the HwiP */
    HwiP_disableInterrupt(interruptNum);
#endif
}

/*
 * Purpose:     Clears a pending interrupt
 * Description: Clears a pending interrupt associated with an event number(0-127).
 *
 * Returns:     None
 */
void Osal_ClearInterrupt(uint32_t corepacEvent,uint32_t interruptNum)
{
#ifdef _TMS320C6X
    if(OSAL_REGINT_INTVEC_EVENT_COMBINER == interruptNum) {
     /* If this is called from a module which uses event combiner,
      * Just clear the event inside the event combiner and
      * not the whole interrupt as it will be used by the event dispatcher */
        (void)EventCombinerP_clearEvent(corepacEvent);
    } else {
      /* This is not an event combiner based interrupt */
        HwiP_clearInterrupt(interruptNum);
    }
#else
    /* For non c66x cases, there is no event combiner , just clear the interruptNum's vector */
    HwiP_clearInterrupt(interruptNum);
#endif
}

#ifdef __cplusplus
}
#endif

