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

/* Generic headers */
#include <stdlib.h>
#include <string.h>

/* SafeRTOS includes */
#include "SafeRTOS_API.h"
#include "SafeRTOSConfig.h"
#include "SafeRTOS_priv.h"

#include <ti/osal/CacheP.h>
#include "ti/csl/csl_tsc.h"
#include <ti/osal/src/nonos/Nonos_config.h>
#include <ti/csl/csl_chipAux.h>
#include <ti/csl/src/intc/_csl_intc.h>
#include <ti/osal/DebugP.h>
#include <ti/drv/sciclient/sciclient.h>

/* Function declaration */
extern void vPortSetInterruptVectors( void );
static portBaseType prvC66xTickInterruptConfig( void );
static void vPortCacheConfig( void );

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
static const InterruptDispatchTable axIntDispatchTable[5] =
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
/* Hardware setup using the TI PDK libraries. */
portBaseType prvSetupHardware( void )
{
    portBaseType xStatus = pdPASS;
    int32_t ret;
    Sciclient_ConfigPrms_t config;

    Sciclient_configPrmsInit(&config);

    ret = Sciclient_init(&config);
    if(  CSL_PASS != ret )
    {
        xStatus = pdFAIL;
    }
    else
    {
        xStatus = prvC66xTickInterruptConfig();
    }
    DebugP_assert(pdPASS == xStatus);    
    vPortSetInterruptVectors();

    return xStatus;
}
/*-------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Override functions for TI PDK library startup code.
 *---------------------------------------------------------------------------*/


/* Override the startup code mpu initialisation - not used in this demo. */
/*
void __mpu_init( void )
{
}
*/
/*-------------------------------------------------------------------------*/

/*****************************************************************************/
/* _SYSTEM_PRE_INIT() - _system_pre_init() is called in the C/C++ startup    */
/* routine (_c_int00()) and provides a mechanism for the user to             */
/* insert application specific low level initialization instructions prior   */
/* to calling main().  The return value of _system_pre_init() is used to     */
/* determine whether or not C/C++ global data initialization will be         */
/* performed (return value of 0 to bypass C/C++ auto-initialization).        */
/*                                                                           */
/* PLEASE NOTE THAT BYPASSING THE C/C++ AUTO-INITIALIZATION ROUTINE MAY      */
/* RESULT IN PROGRAM FAILURE.                                                */
/*                                                                           */
/* The version of _system_pre_init() below is skeletal and is provided to    */
/* illustrate the interface and provide default behavior.  To replace this   */
/* version rewrite the routine and include it as part of the current project.*/
/* The linker will include the updated version if it is linked in prior to   */
/* linking with the C/C++ runtime library.                                   */
/*****************************************************************************/
portBaseType _system_pre_init( void )
{
    vPortCacheConfig();
    extended_system_pre_init();
    return 1;
}
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
/* _SYSTEM_POST_CINIT() - _system_post_cinit() is a hook function called in  */
/* the C/C++ auto-initialization function after cinit() and before pinit().  */
/*                                                                           */
/* The version of _system_post_cinit() below is skeletal and is provided to  */
/* illustrate the interface and provide default behavior.  To replace this   */
/* version rewrite the routine and include it as part of the current project.*/
/* The linker will include the updated version if it is linked in prior to   */
/* linking with the C/C++ runtime library.                                   */
/*****************************************************************************/
void _system_post_cinit( void )
{
    osalArch_Config_t cfg;

    cfg.disableIrqOnInit = BTRUE;
    osalArch_Init( &cfg );
    extended_system_post_cinit();
}
/*---------------------------------------------------------------------------*/

/* Dispatch handler for TI PDK style interrupts. */
void vApplicationInterruptHandlerHook( portUInt32Type ulInterruptVectorNum )
{
    vInterruptDispatchFn *pxIntDispatchFn;

    portUInt32Type *pxDispatchAddress;
    portUInt32Type *pxCpuInterruptTable = ( portUInt32Type * )&CSL_intcCpuIntrTable;
    portUInt32Type uxDispatchIndex;
    portUInt32Type uxIntDispatchTableSize = sizeof( axIntDispatchTable )/sizeof(axIntDispatchTable[0]);

    pxDispatchAddress = ( portUInt32Type * )( pxCpuInterruptTable + ( 1U + ulInterruptVectorNum ) );
    CSL_intcCpuIntrTable.currentVectId = ( Uint32 )pxDispatchAddress;
    pxIntDispatchFn = * ( ( vInterruptDispatchFn **  ) pxDispatchAddress );

    for( uxDispatchIndex = 0;uxDispatchIndex < uxIntDispatchTableSize; uxDispatchIndex++ )
    {
        if( axIntDispatchTable[ uxDispatchIndex ].pxIntHandlerFn == pxIntDispatchFn )
        {
            break;
        }
    }

    if( uxDispatchIndex >= uxIntDispatchTableSize )
    {
       DebugP_assert(BFALSE);
    }
    else
    {
        axIntDispatchTable[ uxDispatchIndex ].pxIntHandlerCoreFn();
    }
}
/*-----------------------------------------------------------------------------
 * End of Override functions for TI PDK library startup code.
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
static portBaseType prvC66xTickInterruptConfig( void )
{
    struct tisci_msg_rm_irq_set_req     rmIrqReq;
    struct tisci_msg_rm_irq_set_resp    rmIrqResp;

    rmIrqReq.valid_params           = TISCI_MSG_VALUE_RM_DST_ID_VALID |
                                      TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
    rmIrqReq.src_index              = 0U;
    if(0U == CSL_chipReadDNUM())
    {
        rmIrqReq.src_id                 = TISCI_DEV_TIMER0;
        rmIrqReq.dst_id                 = TISCI_DEV_C66SS0_CORE0;
        rmIrqReq.dst_host_irq           = 21U;
    }
    else
    {
        rmIrqReq.src_id                 = TISCI_DEV_TIMER1;
        rmIrqReq.dst_id                 = TISCI_DEV_C66SS1_CORE0;
        rmIrqReq.dst_host_irq           = 20U;
    }
    /* Unused params */
    rmIrqReq.global_event           = 0U;
    rmIrqReq.ia_id                  = 0U;
    rmIrqReq.vint                   = 0U;
    rmIrqReq.vint_status_bit_index  = 0U;
    rmIrqReq.secondary_host         = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

    return ( ( CSL_PASS == Sciclient_rmIrqSet(&rmIrqReq, &rmIrqResp, SCICLIENT_SERVICE_WAIT_FOREVER) )? pdPASS : pdFAIL );
}
/*-------------------------------------------------------------------------*/

static void vPortCacheConfig( void )
{
    DSPICFGCacheEnable( SOC_DSP_ICFG_BASE,
                        DSPICFG_MEM_L1P,
                        configCACHE_LIP_SIZE );
    DSPICFGCacheEnable( SOC_DSP_ICFG_BASE,
                        DSPICFG_MEM_L1D,
                        configCACHE_LID_SIZE );
    DSPICFGCacheEnable( SOC_DSP_ICFG_BASE,
                        DSPICFG_MEM_L2,
                        configCACHE_L2_SIZE );

    /* Enable cache for all DDR space */
    CacheP_setMar((void *)configDDR_START, 
                  (uint32_t)configDDR_SIZE, 
                  CacheP_Mar_ENABLE);
}
/*---------------------------------------------------------------------------*/
