/*
 *  Copyright ( C ) 2022 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  ( INCLUDING NEGLIGENCE OR OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/* SafeRTOS Includes */
#include "SafeRTOS_API.h"
#include "aborts.h"

#include "ti/csl/src/intc/_csl_intc.h"

/* Hook function handlers targeting the TI PDK libraries. */

/*-----------------------------------------------------------------------------
 * Function type and structure type definitions.
 *---------------------------------------------------------------------------*/

/* TI Interrupt handler function type. */
typedef void ( vInterruptDispatchFn )( void );

/* TI Interrupt handler functions. */
typedef struct InterruptDispatchTable_
{
    vInterruptDispatchFn *pxIntHandlerFn;
    vInterruptDispatchFn *pxIntHandlerCoreFn;
} InterruptDispatchTable;

/*-----------------------------------------------------------------------------
 * Private constant definitions.
 *---------------------------------------------------------------------------*/

/* Dispatch table of TI Interrupt handler functions. */
static const InterruptDispatchTable axIntDispatchTable[] =
{
    [0] =
    {
        .pxIntHandlerFn     = &CSL_intcDispatcher,
        .pxIntHandlerCoreFn = &CSL_intcDispatcherCore,
    },
    [1] =
    {
        .pxIntHandlerFn     = &CSL_intcEvent0Dispatcher,
        .pxIntHandlerCoreFn = &CSL_intcEvent0DispatcherCore,
    },
    [2] =
    {
        .pxIntHandlerFn     = &CSL_intcEvent1Dispatcher,
        .pxIntHandlerCoreFn = &CSL_intcEvent1DispatcherCore,
    },
    [3] =
    {
        .pxIntHandlerFn     = &CSL_intcEvent2Dispatcher,
        .pxIntHandlerCoreFn = &CSL_intcEvent2DispatcherCore,
    },
    [4] =
    {
        .pxIntHandlerFn     = &CSL_intcEvent3Dispatcher,
        .pxIntHandlerCoreFn = &CSL_intcEvent3DispatcherCore,
    },
};

/*-----------------------------------------------------------------------------
 * Public function definitions.
 *---------------------------------------------------------------------------*/

/* Dispatch handler for TI PDK style interrupts. */
void vApplicationInterruptHandlerHook( portUInt32Type ulInterruptVectorNum )
{
    vInterruptDispatchFn *pxIntDispatchFn;

    portUInt32Type *pxDispatchAddress;
    portUInt32Type *pxCpuInterruptTable = ( portUInt32Type * )&CSL_intcCpuIntrTable;
    portUInt32Type uxDispatchIndex;

    pxDispatchAddress = ( portUInt32Type * )( pxCpuInterruptTable + ( 1 + ulInterruptVectorNum ) );
    CSL_intcCpuIntrTable.currentVectId = ( Uint32 )pxDispatchAddress;
    pxIntDispatchFn = * ( ( vInterruptDispatchFn **  ) pxDispatchAddress );

    for ( uxDispatchIndex = 0;
          uxDispatchIndex < sizeof( axIntDispatchTable )/sizeof( axIntDispatchTable[ 0 ] );
          uxDispatchIndex++ )
    {
        if ( axIntDispatchTable[ uxDispatchIndex ].pxIntHandlerFn == pxIntDispatchFn )
        {
            break;
        }
    }

    if( uxDispatchIndex >= sizeof( axIntDispatchTable )/sizeof( axIntDispatchTable[ 0 ] ) )
    {
        vApplicationAbort();
    }
    axIntDispatchTable[ uxDispatchIndex ].pxIntHandlerCoreFn();
}
/*---------------------------------------------------------------------------*/
