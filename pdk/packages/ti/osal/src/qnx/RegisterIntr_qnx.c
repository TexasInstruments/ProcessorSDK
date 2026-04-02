/*
 * Copyright (c) 2017-2021, Texas Instruments Incorporated
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
 *  ======== RegisterIntr_qnx.c ========
 */


#include <ti/osal/osal.h>
#include <ti/osal/RegisterIntr.h>
#include <ti/csl/csl.h>
#include <ti/csl/tistdtypes.h>
#include <ti/osal/src/nonos/Nonos_config.h>

/*
 *  ======== Osal_RegisterInterrupt_initParams ========
 *
 * Purpose:     Initializes the interrupt registration config structure
 * Description: Initializes the interrupt registration config structure prior to
 *              invoking the Osal_registerIntr API
 */
void Osal_RegisterInterrupt_initParams(OsalRegisterIntrParams_t *interruptRegParams)
{
    if (NULL != interruptRegParams) {
        /* Corepac config */
        interruptRegParams->corepacConfig.name = NULL;
        interruptRegParams->corepacConfig.corepacEventNum = CSL_INVALID_EVENT_ID;
        interruptRegParams->corepacConfig.intVecNum = CSL_INVALID_VEC_ID;
        interruptRegParams->corepacConfig.isrRoutine = NULL;
        interruptRegParams->corepacConfig.arg = (uintptr_t)NULL;
        interruptRegParams->corepacConfig.priority = 0x20U; /* Default */
        interruptRegParams->corepacConfig.intAutoEnable = 0; /* Don't automatically re-enable interrupt */
#if defined (__ARM_ARCH_7A__)
        interruptRegParams->corepacConfig.triggerSensitivity = 0x3U; /* interrupt edge triggered */
#endif

#if defined(__aarch64__) || defined (__TI_ARM_V7R4__)
        interruptRegParams->corepacConfig.triggerSensitivity = OSAL_ARM_GIC_TRIG_TYPE_LEVEL; /* interrupt level triggered */
#endif
        /* SOC Mux Config */
        interruptRegParams->socMuxConfig.muxInParams = NULL;
        interruptRegParams->socMuxConfig.muxOutParams = NULL;
    }

    HwiP_Params_init(NULL);
    return;
}

/*
 *  ======== Osal_RegisterInterrupt ========
 *
 * Purpose:     Registers the interrupt for the event with the params provided
 * Description: Registers the event with the event combiner/Interrupt Vector.
 *
 * Returns:     The Hwi Handle and the return codes defined in OsalInterruptRetCode_e
 */
OsalInterruptRetCode_e Osal_RegisterInterrupt(OsalRegisterIntrParams_t *interruptRegParams,HwiP_Handle *hwiPHandlePtr)
{

    OsalInterruptRetCode_e     ret = OSAL_INT_SUCCESS;
    HwiP_Handle                hwiPHandle = NULL;
    HwiP_Params                hwiInputParams;

    /* Program the corepac interrupt */
    if( (NULL == interruptRegParams->corepacConfig.isrRoutine) ||
        (0 > interruptRegParams->corepacConfig.corepacEventNum) ) {
        ret = OSAL_INT_ERR_INVALID_PARAMS;
    }

    hwiInputParams.name = interruptRegParams->corepacConfig.name;
    hwiInputParams.arg  = (uintptr_t)interruptRegParams->corepacConfig.arg;
    hwiInputParams.priority = interruptRegParams->corepacConfig.priority;
    hwiInputParams.evtId = interruptRegParams->corepacConfig.corepacEventNum;
#if defined (__ARM_ARCH_7A__) || defined (__aarch64__) || defined (__TI_ARM_V7R4__)
    hwiInputParams.triggerSensitivity = interruptRegParams->corepacConfig.triggerSensitivity;
#endif
    hwiInputParams.autoEnable = interruptRegParams->corepacConfig.intAutoEnable;

    hwiPHandle =  HwiP_create(interruptRegParams->corepacConfig.intVecNum,interruptRegParams->corepacConfig.isrRoutine, &hwiInputParams);
    if(NULL == hwiPHandle) {
        ret = OSAL_INT_ERR_HWICREATE;
        goto finish_processing;
    }

finish_processing:
  *hwiPHandlePtr = hwiPHandle;
  return ret ;
}

/*
 *  ======== Osal_DeleteInterrupt ========
 *
 * Purpose:     Deletes/Unregisters an interrupt
 * Description: Deletes/Unregisters an interrupt associated with an event number(0-127).
 *
 * Returns:     OSAL_INT_SUCCESS if successful,OSAL_INT_ERR_DELETE if failed
 */
OsalInterruptRetCode_e Osal_DeleteInterrupt(HwiP_Handle handle,uint32_t corepacEventNum)
{
    HwiP_Status hwiP_ret = HwiP_OK;
    OsalInterruptRetCode_e ret = OSAL_INT_SUCCESS;

    /* For non-c66x cases, just delete the handle */
    hwiP_ret = HwiP_delete(handle);
    if(HwiP_OK != hwiP_ret) {
      ret = OSAL_INT_ERR_DELETE;
    }

    return(ret);
}



/*
 *  ======== Osal_EnableInterrupt ========
 *
 * Purpose:     Enables an event/interrupt
 * Description: Enables the interrupt associated with an event number(0-127).
 *              In case of the event combiner, the corresponding event is
 *              enabled within the Event Combiner.
 *
 * Returns:     None
 */
void Osal_EnableInterrupt(uint32_t corepacEvent,uint32_t interruptNum)
{
    /* For non c66x cases, there is no event combiner , just invoke the HwiP */
    HwiP_enableInterrupt(interruptNum);
}

/*
 *  ======== Osal_DisableInterrupt ========
 *
 * Purpose:     Disables an event/interrupt
 * Description: Disables the interrupt associated with an event number(0-127).
 *              In case of the event combiner, the corresponding event is
 *              disabled within the Event Combiner.
 *
 * Returns:     None
 */
void Osal_DisableInterrupt(uint32_t corepacEvent,uint32_t interruptNum)
{
    /* For non c66x cases, there is no event combiner , just invoke the HwiP */
    HwiP_disableInterrupt(interruptNum);
}

/*
 *  ======== Osal_ClearInterrupt ========
 *
 * Purpose:     Clears a pending interrupt
 * Description: Clears a pending interrupt associated with an event number(0-127).
 *
 * Returns:     None
 */
void Osal_ClearInterrupt(uint32_t corepacEvent,uint32_t interruptNum)
{
    uint32_t intNum = interruptNum;

    /* For non c66x cases, there is no event combiner , just clear the interruptNum's vector */
    HwiP_clearInterrupt(intNum);
}

/* Nothing past this point */
