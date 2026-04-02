/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 */

/*
 *   @file  app_sync.c
 *
 *   @brief
 *      sync code used for synchronization the execution between cores.
 *      This is implemented using the spare registers.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/csl/soc.h>
#include <ti/csl/hw_types.h>
#include "app_sync.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static uint32_t dssSyncRegAddr = CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_HW_SPARE_RW0;
static uint32_t mssSyncRegAddr = CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_HW_SPARE_RW1;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void App_setDssState(uint32_t state)
{
    HW_WR_REG32(dssSyncRegAddr, state);
}

uint32_t App_getDssState(void)
{
    return (HW_RD_REG32(dssSyncRegAddr));
}

void App_setMssState(uint32_t state)
{
    HW_WR_REG32(mssSyncRegAddr, state);
}

uint32_t App_getMssState(void)
{
    return (HW_RD_REG32(mssSyncRegAddr));
}
