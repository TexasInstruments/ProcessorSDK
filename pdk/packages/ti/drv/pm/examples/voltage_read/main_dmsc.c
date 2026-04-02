/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2018
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
 *  \file   main_dmsc.c
 *
 *  \brief  System Config implementation example.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ti/drv/pm/pmlib.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/pm/include/dmsc/pmlib_vtm_vd.h>
#include "app_utils.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t main(void)
{
    int32_t status = PM_FAIL;

    uint32_t vDomain, opp;
    int32_t voltVal = 0;

    AppUtils_defaultInit();
    /*AVS is only available for VD 2*/
    vDomain = 2U;

    App_printf("PM Voltage Read Test App\n");

    uint32_t count = 80;
    while(count--)App_printf("-");
    App_printf("\n");

    /*AVS only applies to VDD_MPU OPP*/
    opp = 1U;

    status = PMLIBReadVoltageVD(vDomain, opp, &voltVal);

    if(PM_SUCCESS==status)
    {
        App_printf("Voltage value for voltage domain %d and OPP %d : %d mV\n", vDomain, opp, voltVal);
        App_printf("All tests have passed.\n");
    }
    else if(PM_VOLTAGE_INVALID == status)
    {
        App_printf("Efuse not programmed for the device\nVoltage is nominal for voltage domain %d and OPP %d\n", vDomain, opp);
    }
    else
    {
        App_printf("Invalid voltage read for voltage domain %d and OPP %d\n", vDomain, opp);
    }
    return 0;
}
