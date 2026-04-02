/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
 *  ======== MutexP_freertos.c ========
 */

#include <ti/osal/MutexP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ti/csl/csl_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/soc/osal_soc.h>


MutexP_Handle MutexP_create(MutexP_Object *mutexObj)
{
    MutexP_Handle ret_handle = NULL;
    SemaphoreP_Params prms;
    SemaphoreP_Handle handle = NULL;

    if (NULL == mutexObj)
    {
        ret_handle =  NULL;
    }
    else
    {
      SemaphoreP_Params_init(&prms);
      prms.mode = SemaphoreP_Mode_BINARY;
      handle = (void *)SemaphoreP_create(1U, &prms);

      mutexObj->object = (void *)handle;
      mutexObj->key = 0;
      ret_handle = (MutexP_Handle)mutexObj;
    }

    return ret_handle;
}

MutexP_Status MutexP_delete(MutexP_Handle handle)
{
    SemaphoreP_Handle semHandle = NULL;
    MutexP_Status     status = MutexP_OK;
    MutexP_Object     *mutexObj = (MutexP_Object *)handle;
    
    if (NULL_PTR != mutexObj)
    {
        semHandle = (void *)mutexObj->object;
        if (SemaphoreP_OK == SemaphoreP_delete(semHandle))
        {
            status = MutexP_OK;
        }
        else
        {
            status = MutexP_FAILURE;
        }
    }
    else
    {
        status = MutexP_FAILURE;
    }
    return status;
}

MutexP_Status MutexP_lock(MutexP_Handle handle, uint32_t timeout)
{
    SemaphoreP_Handle semHandle = NULL;
    MutexP_Status     status = MutexP_OK;
    MutexP_Object     *mutexObj = (MutexP_Object *)handle;

    if(NULL_PTR != mutexObj)
    {
        semHandle = (void *)mutexObj->object;
        if(SemaphoreP_OK == SemaphoreP_pend(semHandle, timeout))
        {
            status = MutexP_OK;
        }
        else
        {
            status = MutexP_FAILURE;
        }
    }
    else
    {
        status = MutexP_FAILURE;
    }
    
    return status;
}

MutexP_Status MutexP_unlock(MutexP_Handle handle)
{
    SemaphoreP_Handle semHandle = NULL;
    MutexP_Status     status = MutexP_OK;
    MutexP_Object     *mutexObj = (MutexP_Object *)handle;

    if(NULL_PTR != mutexObj)
    {
        semHandle = (void *)mutexObj->object;
        if(SemaphoreP_OK == SemaphoreP_post(semHandle))
        {
            status = MutexP_OK;
        }
        else
        {
            status = MutexP_FAILURE;
        }
    }
    else
    {
        status = MutexP_FAILURE;
    }

    return status;
}


/* Nothing past this point */
