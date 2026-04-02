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

#include <ti/osal/CacheP.h>
#include "ti/csl/csl_tsc.h"
#include <ti/osal/src/nonos/Nonos_config.h>

/* ------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Override functions for TI PDK library startup code.
 *---------------------------------------------------------------------------*/

void vPortCacheConfig( void )
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
}
/*---------------------------------------------------------------------------*/
