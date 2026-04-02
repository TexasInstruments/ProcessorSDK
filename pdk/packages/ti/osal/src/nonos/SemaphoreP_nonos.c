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
 *  ======== SemaphoreP_nonos.c ========
 */
#include <ti/osal/SemaphoreP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <ti/osal/osal.h>
#include <ti/osal/src/nonos/Nonos_config.h>

extern uint32_t  gOsalSemAllocCnt, gOsalSemPeak;

/*!
 *  @brief    Semaphore structure
 */
typedef struct Sem_Struct_s {
    bool              used;
    uint32_t          sem;
    uint32_t          count;
    SemaphoreP_Mode   mode;
} Sem_Struct;

Sem_Struct semStructs[OSAL_NONOS_CONFIGNUM_SEMAPHORE];

static void semaphoreInit( Sem_Struct *semPool, uint32_t maxSemaphores)
{
  uint32_t i;
  Sem_Struct *semPool_ptr = semPool;
  for(i = 0U; i < maxSemaphores; i++)
  {
    semPool_ptr->used = BFALSE;
    semPool_ptr->sem =  i;
    semPool_ptr->count = 0;
    semPool_ptr->mode = SemaphoreP_Mode_COUNTING;
    semPool_ptr++;
  }

}
/*
 *  ======== SemaphoreP_create ========
 */
SemaphoreP_Handle SemaphoreP_create(uint32_t count,
                                    const SemaphoreP_Params *params)
{
    uint32_t          i;
    uintptr_t         key;
    SemaphoreP_Handle ret_val;
    uintptr_t         temp;
    Sem_Struct       *semPool;
    uint32_t          maxSemaphores;

    key = HwiP_disable();

    /* Check if user has specified any memory block to be used, which gets
     * the precedence over the internal static memory block
     */
    if ((uintptr_t)0U != gOsal_HwAttrs.extSemaphorePBlock.base)
    {
        /* pick up the external memory block configured */
        semPool        = (Sem_Struct *) gOsal_HwAttrs.extSemaphorePBlock.base;
        temp           = (uintptr_t) gOsal_HwAttrs.extSemaphorePBlock.size;
        maxSemaphores  = (uint32_t)(temp/(sizeof(Sem_Struct)));
    }
    else
    {
        /* Pick up the internal static memory block */
        semPool        = (Sem_Struct *) &semStructs[0];
        maxSemaphores  = OSAL_NONOS_CONFIGNUM_SEMAPHORE;
    }
    /* First time, semaphores are not initialized */
    if(0U == gOsalSemAllocCnt)
    {
       semaphoreInit(semPool,maxSemaphores);
	}
    
    for (i = 0U; i < maxSemaphores; i++)
    {
        if (BFALSE == semPool[i].used)
        {
            semPool[i].used = BTRUE;

            /* Update statistics */
            gOsalSemAllocCnt++;
            if (gOsalSemAllocCnt > gOsalSemPeak)
            {
                gOsalSemPeak = gOsalSemAllocCnt;
            }

            semPool[i].sem = i;
            semPool[i].count = count;
            if (NULL_PTR != params)
            {
                semPool[i].mode = params->mode;
            }

            break;
        }
    }
    HwiP_restore(key);

    if (i == maxSemaphores)
    {
        ret_val = NULL_PTR;
    }
    else
    {
        ret_val = ((SemaphoreP_Handle)&semPool[i]);
    }
    return (ret_val);
}

/*
 *  ======== SemaphoreP_delete ========
 */
SemaphoreP_Status SemaphoreP_delete(SemaphoreP_Handle handle)
{
    SemaphoreP_Status ret = SemaphoreP_OK;
    uintptr_t   key;
    Sem_Struct *semS = (Sem_Struct *)handle;
    
    if((NULL_PTR != semS) && (semS->used))
    {
      key = HwiP_disable();
      semS->used = BFALSE;
      /* Found the bsp osal semaphore object to delete */
      if (0U < gOsalSemAllocCnt)
      {
        gOsalSemAllocCnt--;
      }
      HwiP_restore(key);
      ret = SemaphoreP_OK;
    }
    else
    {
      ret = SemaphoreP_FAILURE;
    }  

    return (ret);
}

/*
 *  ======== SemaphoreP_Params_init ========
 */
void SemaphoreP_Params_init(SemaphoreP_Params *params)
{
    if(NULL_PTR != params)
    {
        params->mode = SemaphoreP_Mode_COUNTING;
        params->name = (char *) NULL_PTR;
    }  
}

/*
 *  ======== SemaphoreP_pend ========
 */
SemaphoreP_Status SemaphoreP_pend(SemaphoreP_Handle handle, uint32_t timeout)
{
    uintptr_t           key;
    Sem_Struct         *semS        = (Sem_Struct *)handle;
    uint32_t            semTimeout  = timeout;
    SemaphoreP_Status   ret_val     = SemaphoreP_OK;
    bool iterate = BTRUE;
    
    if(NULL != semS) {
      while ( (SemaphoreP_OK == ret_val) && (BTRUE == iterate))
      {
        key = HwiP_disable();
        if (0U < (semS->count))
        {
            if (SemaphoreP_Mode_BINARY == semS->mode)
            {
				        semS->count = 0U;
            }
            else
            {
                semS->count--;
            }
            HwiP_restore(key);
            iterate = BFALSE;
        }
	  else
        {
            HwiP_restore(key);
            if (SemaphoreP_NO_WAIT == semTimeout)
            {
                ret_val = (SemaphoreP_TIMEOUT);
                iterate = BFALSE;
            }
            else if  (SemaphoreP_WAIT_FOREVER == semTimeout)
            {
               /* Wait forever */
            }
            else /* Timed wait */
            {
                (void)Osal_delay(1);
                semTimeout--;
            }
        }
      }
   } else
   {
	   ret_val = SemaphoreP_FAILURE;
   }   

    return (ret_val);
}

/*
 *  ======== SemaphoreP_post ========
 */
SemaphoreP_Status SemaphoreP_post(SemaphoreP_Handle handle)
{
    SemaphoreP_Status ret = SemaphoreP_OK;
    uintptr_t   key;
    Sem_Struct *semS = (Sem_Struct *)handle;
    if(NULL_PTR != semS)
    {
      
      key = HwiP_disable();
      if (SemaphoreP_Mode_BINARY == semS->mode)
      {
        semS->count = 1U;
      }
      else
      {
        semS->count++;
      }
      HwiP_restore(key);
      ret = SemaphoreP_OK;
    } 
    else
    {
      ret = SemaphoreP_FAILURE;
    }   

    return (ret);
}

/*
 *  ======== SemaphoreP_getCount ========
 */
int32_t SemaphoreP_getCount(SemaphoreP_Handle handle)

{
    int32_t ret = 0;
    Sem_Struct *semS = (Sem_Struct *)handle;
    if(NULL_PTR != semS) {
       ret = (int32_t)semS->count;
    } else {
       ret = 0;
    }
    return (ret);
}

/*
 *  ======== SemaphoreP_postFromISR ========
 */
SemaphoreP_Status SemaphoreP_postFromISR(SemaphoreP_Handle handle)
{
    return (SemaphoreP_post(handle));
}

/* Nothing past this point */

