/*
 *  Copyright (C) 2026 Texas Instruments Incorporated
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
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file c7x_mmu_error.c
 *
 *  \brief MMU error functions should not be optimized for recovery
 *         Except for C Matrix Read/Write
 */
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "c7x_mmu_error.h"
#include "Mmu.h"

#include <stdbool.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

/*===========================================================================*/
/*                        Private Function Definitions                       */
/*===========================================================================*/

void MMU_unInitMemAccess()
{
    UART_printf("Beginning Uninitialized Memory Access test.\n");

    /* Writing to Uninitialized Memory Region to Inject Error */
    uint32_t *ptr = (uint32_t *)0x74000000U; 
    *ptr = 0x12345678;
}

void MMU_accessPermViolation()
{
    Bool            retVal;
    Mmu_MapAttrs    attrs;

    UART_printf("Beginning Access Permission Violation test.\n");

    attrs.attrIndx = Mmu_AttrIndx_MAIR0;
    attrs.ns = true;
    attrs.accessPerm = Mmu_AccessPerm_PRIV_RO_USER_RO;

    retVal = Mmu_map(0x70000000U, 0x70000000U, 0x08000000U, &attrs, !(attrs.ns));
    if(retVal==false)
    {
        UART_printf("MMU Memory Region initialization failed.\n");
    }

    /* Writing to a Read only region to inject error */
    uint32_t *ptr = (uint32_t *)0x75000000U; 
    *ptr = 0x12345678;
}
