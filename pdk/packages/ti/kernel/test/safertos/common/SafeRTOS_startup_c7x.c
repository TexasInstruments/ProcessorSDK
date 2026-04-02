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

/* SafeRTOS includes */
#include "SafeRTOS_API.h"
#include "SafeRTOSConfig.h"

#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include "Mmu.h"


/*-----------------------------------------------------------------------------
 * Private function declarations.
 *---------------------------------------------------------------------------*/

static void prvMmuInit( Bool isSecure );
static void prvCfgClecAccessCtrl ( Bool onlyInSecure );


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
    extern void c7x_startup_init(void);
    c7x_startup_init();
}
/*---------------------------------------------------------------------------*/

void vInitMmu( void )
{
    prvMmuInit( BFALSE );
    prvMmuInit( BTRUE );

    /* Setup CLEC access/configure in non-secure mode */
    prvCfgClecAccessCtrl( BFALSE );
}
/*-------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Private function definitions.
 *---------------------------------------------------------------------------*/


static void prvMmuInit( Bool isSecure )
{
    Mmu_MapAttrs    attrs;

    Mmu_initMapAttrs( &attrs );
    attrs.attrIndx = Mmu_AttrIndx_MAIR0;

    if( UTRUE == isSecure )
    {
        attrs.ns = 0;
    }
    else
    {
        attrs.ns = 1;
    }

    /* Register region */
    ( void )Mmu_map( 0x00000000U, 0x00000000U, 0x20000000U, &attrs, isSecure );
    ( void )Mmu_map( 0x20000000U, 0x20000000U, 0x20000000U, &attrs, isSecure );
    ( void )Mmu_map( 0x40000000U, 0x40000000U, 0x20000000U, &attrs, isSecure );
    ( void )Mmu_map( 0x60000000U, 0x60000000U, 0x10000000U, &attrs, isSecure );
    ( void )Mmu_map( 0x78000000U, 0x78000000U, 0x08000000U, &attrs, isSecure ); /* CLEC */

    attrs.attrIndx = Mmu_AttrIndx_MAIR7;
    ( void )Mmu_map( 0x80000000U, 0x80000000U, 0x20000000U, &attrs, isSecure ); /* DDR */
    ( void )Mmu_map( 0xA0000000U, 0xA0000000U, 0x20000000U, &attrs, isSecure ); /* DDR */
    ( void )Mmu_map( 0x70000000U, 0x70000000U, 0x00800000U, &attrs, isSecure ); /* MSMC - 8MB */
    ( void )Mmu_map( 0x41C00000U, 0x41C00000U, 0x00080000U, &attrs, isSecure ); /* OCMC - 512KB */

    /*
     * DDR range 0xA0000000 - 0xAA000000 : Used as RAM by multiple
     * remote cores, no need to mmp_map this range.
     * IPC VRing Buffer - uncached
     */
    attrs.attrIndx =  Mmu_AttrIndx_MAIR4;
    ( void )Mmu_map( 0xAA000000U, 0xAA000000U, 0x02000000U, &attrs, isSecure );

    return;
}
/*-------------------------------------------------------------------------*/

static void prvCfgClecAccessCtrl ( Bool onlyInSecure )
{
    CSL_ClecEventConfig cfgClec;
    CSL_CLEC_EVTRegs   *clecBaseAddr = ( CSL_CLEC_EVTRegs* ) CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE;
    uint32_t            i, maxInputs = 2048U;

    cfgClec.secureClaimEnable = onlyInSecure;
    cfgClec.evtSendEnable     = UFALSE;
    cfgClec.rtMap             = CSL_CLEC_RTMAP_DISABLE;
    cfgClec.extEvtNum         = 0U;
    cfgClec.c7xEvtNum         = 0U;
    for(i = 0U; i < maxInputs; i++)
    {
        CSL_clecConfigEvent( clecBaseAddr, i, &cfgClec );
    }
}
/*-------------------------------------------------------------------------*/

