/*
 *   Copyright (c) Texas Instruments Incorporated 2025
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
 *  \file     soc_ecc_func.c
 *
 *  \brief    This file contains ECC SDL Function to initialize devices.
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <soc.h>
#include <sdl_types.h>
#include <sdl_ecc.h>
#include <ti/drv/uart/UART_stdio.h>
#include <include/soc/j721s2/sdlr_mcu_r5fss0_baseaddress.h>
#include <src/sdl/ecc/soc/j721s2/sdl_ecc_soc.h>
#include "ecc.h"

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void ECC_Test_lateDeviceInit(uint32_t mainMem)
{
    int32_t status = SDL_PASS;
    uint32_t i;
    uint32_t num_aux_devices = 0;
    uint32_t *aux_devices = NULL;

    switch (mainMem) {
        case SDL_ECC_MEMTYPE_A72_COREPAC:
        case SDL_ECC_MEMTYPE_A72_0:
        case SDL_ECC_MEMTYPE_A72_1:
        case SDL_ECC_MEMTYPE_MAIN_MSMC_AGGR0:
            num_aux_devices = AUX_A72_NUM_DEVICES;
            aux_devices = aux_a72_devices;
            break;
        default:
            break;
    }

    if (num_aux_devices != 0)
    {
        for (i = 0; i < num_aux_devices; i++)
        {
            status = Sciclient_pmSetModuleState(aux_devices[i],
                                                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                                TISCI_MSG_FLAG_AOP,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);

            if (status != SDL_PASS)
            {
                UART_printf("   Sciclient_pmSetModuleState 0x%x ...FAILED: retValue %d\n",
                            aux_devices[i], status);
            }
        }
    }
}