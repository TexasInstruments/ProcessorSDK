/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   osal_extended_testapp_cache.c
 *
 *  \brief  OSAL Cache Sub Module testcases file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"
#if defined(BUILD_C7X)
#include "Mmu.h"
#include "Hwi.h"
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define OSAL_APP_HWI_MAX_NUM (64U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/*
 * Description: Testing MMU API for c7x
 */
static int32_t OsalApp_mmuTests(void);

/*
 * Description: Testing negative Hwi check for c7x
 */
static int32_t OsalApp_c7xHwiTests(void);

/* ========================================================================== */
/*                          Internal Function Definitions                     */
/* ========================================================================== */

static int32_t OsalApp_mmuTests(void)
{
    int32_t result = osal_OK;

#if defined (BUILD_C7X)
    /* Disable and then enable the MMU. Below APIs dont return anything. */
    Mmu_enable();
    Mmu_disable();
    Mmu_enable();

#endif
    return result;
}

static int32_t OsalApp_c7xHwiTests(void)
{
    int32_t result = osal_OK;

#if defined (BUILD_C7X)
    bool          status;
    Hwi_StackInfo stkInfo;
    /* Pass invalid interrupt number to event map. It should return abruptly.
     * Does not return any value, hence nothing to check it against.
     */
    Hwi_eventMap(OSAL_APP_HWI_MAX_NUM, 0);

    status = Hwi_getStackInfo(&stkInfo, BTRUE);
    if((0 == stkInfo.hwiStackPeak) || (BFALSE != status))
    {
        result = osal_FAILURE;
    }
#endif
    return result;
}

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_c7xArchTests(void)
{
    int32_t result = osal_OK;

    result += OsalApp_mmuTests();
    result += OsalApp_c7xHwiTests();

    return result;
}

