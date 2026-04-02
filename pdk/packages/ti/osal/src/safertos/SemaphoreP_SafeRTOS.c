/*
 * Copyright ( c ) 2015-2024, Texas Instruments Incorporated
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
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT ( INCLUDING NEGLIGENCE OR
 * OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== SemaphoreP_safertos.c ========
 */
#include <ti/osal/SemaphoreP.h>

#include "SafeRTOS_priv.h"

extern uint32_t  gOsalSemAllocCnt, gOsalSemPeak;

/*!
 *  @brief    Semaphore structure
 *  The order is important as the semaphore object has to be word aligned.
 */
typedef struct SemaphoreP_safertos_s {
    uint64_t          semObj[((safertosapiQUEUE_OVERHEAD_BYTES/sizeof(uint64_t)) + (1U))];
    xSemaphoreHandle  semHndl;
    bool              used;
} SemaphoreP_safertos;

/* global pool of statically allocated semaphore pools */
static SemaphoreP_safertos gOsalSemPsafertosPool[OSAL_SAFERTOS_CONFIGNUM_SEMAPHORE] __attribute__( ( aligned( 32 ) ) );

static int32_t SemaphoreP_constructBinary( SemaphoreP_safertos *handle, uint32_t initCount );
static int32_t SemaphoreP_constructCounting( SemaphoreP_safertos *handle, uint32_t initCount, uint32_t maxCount );

/*
 *  ======== SemaphoreP_create ========
 */
SemaphoreP_Handle SemaphoreP_create( uint32_t count,
                                     const SemaphoreP_Params *params )
{
    SemaphoreP_Handle ret_handle;
    SemaphoreP_safertos *handle = ( SemaphoreP_safertos * ) NULL_PTR;
    uint32_t          i;
    uintptr_t         key;
    uintptr_t         temp;
    SemaphoreP_safertos *semPool;
    uint32_t          maxSemaphores;
    int32_t           retVal;

    /* Check if user has specified any memory block to be used, which gets
     * the precedence over the internal static memory block
     */
    if( (uintptr_t)0U != gOsal_HwAttrs.extSemaphorePBlock.base )
    {
        /* pick up the external memory block configured */
        semPool        = ( SemaphoreP_safertos * ) gOsal_HwAttrs.extSemaphorePBlock.base;
        temp           = ( uintptr_t ) gOsal_HwAttrs.extSemaphorePBlock.size;
        maxSemaphores  = ( uint32_t )( temp/( sizeof( SemaphoreP_safertos ) ) );
    }
    else
    {
        /* Pick up the internal static memory block */
        semPool        = ( SemaphoreP_safertos * ) &gOsalSemPsafertosPool[0];
        maxSemaphores  = OSAL_SAFERTOS_CONFIGNUM_SEMAPHORE;

        if( 0U == gOsalSemAllocCnt )
        {
			( void )memset(  ( void * )gOsalSemPsafertosPool,0,sizeof( gOsalSemPsafertosPool ) );
		}
    }

    key = HwiP_disable(  );

     for( i = 0U; i < maxSemaphores; i++ )
     {
         if( BFALSE == semPool[i].used )
         {
             semPool[i].used = BTRUE;
             /* Update statistics */
             gOsalSemAllocCnt++;
             if( gOsalSemAllocCnt > gOsalSemPeak )
             {
                 gOsalSemPeak = gOsalSemAllocCnt;
             }
             break;
         }
     }
     HwiP_restore( key );

    if( i < maxSemaphores )
    {
        /* Grab the memory */
        handle = ( SemaphoreP_safertos * ) &semPool[i];
    }

    if( NULL_PTR == handle ) {
        ret_handle = NULL_PTR;
    }
    else
    {
        if( NULL_PTR == params )
        {
            SemaphoreP_Params semParam;
            SemaphoreP_Params_init( &semParam );
            retVal = SemaphoreP_constructCounting( handle, count, semParam.maxCount );
        }
        else
        {
            if( SemaphoreP_Mode_BINARY == params->mode )
            {
                retVal = SemaphoreP_constructBinary( handle, count );
            }
            else
            {
                retVal = SemaphoreP_constructCounting( handle, count, params->maxCount );
            }
        }

        /* If there was an error reset the sem object and return NULL. */
        if( SemaphoreP_OK != retVal )
        {
            key = HwiP_disable(  );
            handle->used = BFALSE;
            /* Found the osal semaphore object to delete */
            if( 0U < gOsalSemAllocCnt )
            {
                gOsalSemAllocCnt--;
            }
            HwiP_restore( key );
            ret_handle = NULL_PTR;
        }
        else
        {
            ret_handle = ( ( SemaphoreP_Handle )handle );
        }
    }
    return ret_handle;
}

static int32_t SemaphoreP_constructBinary( SemaphoreP_safertos *handle, uint32_t initCount )
{
    int32_t status;

    if((NULL == handle) || (1U < initCount))
    {
        status = SemaphoreP_FAILURE;
    }
    else
    {
        portBaseType xResult = xSemaphoreCreateBinary((portInt8Type *)&(handle->semObj[0]), &handle->semHndl  );
        if( ( pdPASS != xResult ) || ( NULL == handle->semHndl ) )
        {
            status = SemaphoreP_FAILURE;
        }
        else
        {
            if( 0U == initCount )
            {
                uint32_t            isSemTaken;
                DebugP_assert(0 == Osal_isInISRContext( ));
                /* SafeRTOS on BinarySemaphore create initializes semaphore with count of 1.
                * So we need to take semaphore to make count 0, if we are creating a binary semaphore with init count of 0.
                */
                isSemTaken = (uint32_t)xSemaphoreTake( handle->semHndl, safertosapiMAX_DELAY);
                DebugP_assert(UTRUE == isSemTaken);
            }
            status = SemaphoreP_OK;
        }
    }

    return status;
}

static int32_t SemaphoreP_constructCounting( SemaphoreP_safertos *handle, uint32_t initCount, uint32_t maxCount )
{
    int32_t status;

    portBaseType xResult = xSemaphoreCreateCounting(
                                maxCount,
                                initCount,
                                (portInt8Type *)&(handle->semObj[0]),
								&handle->semHndl  );
    if( ( pdPASS != xResult ) || ( NULL == handle->semHndl ) )
    {
        status = SemaphoreP_FAILURE;
    }
    else
    {
        status = SemaphoreP_OK;
    }

    return status;
}

/*
 *  ======== SemaphoreP_delete ========
 */
SemaphoreP_Status SemaphoreP_delete( SemaphoreP_Handle handle )
{
    uintptr_t   key;
    SemaphoreP_Status ret = SemaphoreP_OK;
    SemaphoreP_safertos *semaphore = ( SemaphoreP_safertos * )handle;

    /*
     * NOTE : there is no delete Semaphore API in safertos.
     * We just memset the memory to zero.
     */
    if( ( NULL_PTR != semaphore ) && ( BTRUE == semaphore->used ) )
    {
        memset(&semaphore->semObj, 0, sizeof(semaphore->semObj));
        semaphore->semHndl = NULL;
        key = HwiP_disable(  );
        semaphore->used = BFALSE;
        /* Found the osal semaphore object to delete */
        if( 0U < gOsalSemAllocCnt )
        {
            gOsalSemAllocCnt--;
        }
        HwiP_restore( key );
        ret = SemaphoreP_OK;
    }
    else
    {
       ret = SemaphoreP_FAILURE;
    }

    return ( ret );
}

/*
 *  ======== SemaphoreP_Params_init ========
 */
void SemaphoreP_Params_init( SemaphoreP_Params *params )
{
    if(NULL_PTR != params)
    {
        params->mode = SemaphoreP_Mode_COUNTING;
        params->name = ( char * ) NULL_PTR;
        params->maxCount = 0xFFU;
    }
}

/*
 *  ======== SemaphoreP_pend ========
 */
SemaphoreP_Status SemaphoreP_pend( SemaphoreP_Handle handle, uint32_t timeout )
{
    portBaseType        isSemTaken;
    SemaphoreP_Status   ret_val = SemaphoreP_OK;
    SemaphoreP_safertos *pSemaphore = ( SemaphoreP_safertos * )handle;

    if ( NULL_PTR == handle )
    {
        ret_val = SemaphoreP_FAILURE;
    }
    else
    {
        if( 1 == Osal_isInISRContext() )
        {
            /* timeout is ignored when in ISR mode */
            isSemTaken = (portBaseType)xSemaphoreTakeFromISR( pSemaphore->semHndl);
            safertosapiYIELD_FROM_ISR();
        }
        else
        {
            if ( SemaphoreP_WAIT_FOREVER == timeout )
            {
                isSemTaken = (portBaseType)xSemaphoreTake( pSemaphore->semHndl, safertosapiMAX_DELAY );
                if (pdPASS != isSemTaken)
                {
                    ret_val = SemaphoreP_TIMEOUT;
                }
            }
            else
            {
                isSemTaken = (portBaseType)xSemaphoreTake( pSemaphore->semHndl, timeout );
                if (pdPASS != isSemTaken)
                {
                    ret_val = SemaphoreP_TIMEOUT;
                }
            }
        }
    }

    return ret_val;
}

/*
 *  ======== SemaphoreP_post ========
 */
SemaphoreP_Status SemaphoreP_post( SemaphoreP_Handle handle )
{
    SemaphoreP_Status ret = SemaphoreP_OK;
    SemaphoreP_safertos *pSemaphore = ( SemaphoreP_safertos * )handle;

    if(NULL_PTR != pSemaphore)
    {
        if( 1 == Osal_isInISRContext() )
        {
            xSemaphoreGiveFromISR( pSemaphore->semHndl);
            safertosapiYIELD_FROM_ISR();
        }
        else
        {
            xSemaphoreGive( pSemaphore->semHndl );
        }
        ret = SemaphoreP_OK;
    }
    else
    {
        ret = SemaphoreP_FAILURE;
    }

    return ret;
}

/*
 *  ======== SemaphoreP_postFromClock ========
 */
SemaphoreP_Status SemaphoreP_postFromClock( SemaphoreP_Handle handle )
{
    return ( SemaphoreP_post( handle ) );
}

/*
 *  ======== SemaphoreP_postFromISR ========
 */
SemaphoreP_Status SemaphoreP_postFromISR( SemaphoreP_Handle handle )
{
    return ( SemaphoreP_post( handle ) );
}
/*
 *  ======== SemaphoreP_getCount ========
 */
int32_t SemaphoreP_getCount( SemaphoreP_Handle handle )
{
    portBaseType xResult;
    portUnsignedBaseType uxCount = 0U;

    if(NULL_PTR != handle)
    {
        SemaphoreP_safertos *pSemaphore = ( SemaphoreP_safertos * )handle;
        xResult = xSemaphoreGetCountDepth(  pSemaphore->semHndl, &uxCount  );
        if(pdPASS != xResult)
        {
            uxCount = 0U;
        }
    }
    
    return (int32_t)uxCount;
}
/* Nothing past this point */
