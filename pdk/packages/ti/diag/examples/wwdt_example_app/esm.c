/*
 * ESM functions
 *
 * ESM functionality for the Windowed Watchdog Timer (WWDT) Example
 * Application
 *
 *  Copyright (c) 2020 Texas Instruments Incorporated
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
/**
 *  \file esm.c
 *
 *  \brief This file contains functions that provide ESM setup.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* For interrupt configuration */

#include "esm.h"

#include <ti/osal/osal.h>

#include <ti/csl/hw_types.h>
#include <ti/csl/csl_rti.h>
#include <ti/csl/csl_esm.h>

#include <sdr_common.h>
#include <sdr_esm.h>

/* Diagnostic example utility functions, e.g. print outputs */
#include <diag_utils.h>

#include <ti/csl/soc.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */


/* Initialization structure for MCU ESM instance */
static SDR_ESM_InitConfig_t WWDT_Example_esmInitConfig_Main =
{
    .esmErrorConfig = {0u, 0u}, /* Self test error config */
    .eventMap = {0x00000000u, 0xfffffffbu, 0x7fffffffu, 0xffffffffu,
                 0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                 0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
               /* RTI28_INTR_WWD_0 event enabled in this byte,   ^
                  i.e. offset 359 (change byte to 7f to disable) */

                 0xffffffffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0xffffffffu, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                },
     /**< All events enable: except clkstop events for unused clocks
      *   and PCIE events */
    .eventPriorityMap = {0x00000000u, 0xfffffffbu, 0x7fffffffu, 0xffffffffu,
                         0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                         0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                       /*                                               ^
                           RTI28_INTR_WWD_0 priority set high in this bit,
                          i.e. offset 359 (change byte to 7f to disable) */
                         0xffffffffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0xffffffffu, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                        },
    /**< All events high priority: except clkstop events for unused clocks
     *   and PCIE events */
    .errorOutputMap = {0x00000000u, 0xfffffffbu, 0x7fffffffu, 0xffffffffu,
                       0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu,
                       0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffff7fu,
                       /*                                             ^
                           RTI28_INTR_WWD_0 pin output enabled in this bit,
                           i.e. offset 359 (change byte to 7f to disable) 
                           Not needed for this example */
                       0xffffffffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0xffffffffu, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                      },
    /**< All events high priority: except clkstop for unused clocks
     *   and PCIE events */
};

static HwiP_Handle WWDT_Example_EsmCfgHwiPHandle;
static HwiP_Handle WWDT_Example_EsmHiHwiPHandle;
static HwiP_Handle WWDT_Example_EsmLoHwiPHandle;



/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


/*********************************************************************
* @fn      ESM_init
*
* @brief   Initializes Board, ESM module and handlers
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t ESM_init (void)
{
    int32_t retval = 0;
    uint32_t intNumHi, intNumLo, intNumCfg;
    HwiP_Params       hwiParams;
    void (*pHiInterruptHandler)(uintptr_t);
    void (*pLoInterruptHandler)(uintptr_t);
    void (*pConfigInterruptHandler)(uintptr_t);
    SDR_Result result;

    pHiInterruptHandler = &SDR_ESM_hiInterruptHandler_MAIN;
    pLoInterruptHandler = &SDR_ESM_loInterruptHandler_MAIN;
    pConfigInterruptHandler = &SDR_ESM_configInterruptHandler_MAIN;

    /* Initialize MCU ESM module */
    result = SDR_ESM_init(SDR_ESM_INSTANCE_MAIN, &WWDT_Example_esmInitConfig_Main);
    if (result != SDR_PASS)
    {
        /* print error and quit */
        DIAG_printf("WWDT Example ESM_init: Error initializing Main ESM \n");

        retval = -1;
    }

    /* Get the interrupt number corresponding to ESM HI interrupt */
    intNumHi = SDR_ESM_getIntNumber(SDR_ESM_INSTANCE_MAIN,
                                    SDR_ESM_INT_TYPE_HI);
    if (intNumHi == SDR_ESM_INTNUMBER_INVALID)
    {
        retval = -1;
    }
    else
    {
        /* Get the interrupt number corresponding to ESM LO interrupt */
        intNumLo = SDR_ESM_getIntNumber(SDR_ESM_INSTANCE_MAIN,
                                        SDR_ESM_INT_TYPE_LO);
        if (intNumLo == SDR_ESM_INTNUMBER_INVALID)
        {
            retval = -1;
        }
        else
        {
            /* Get the interrupt number corresponding to ESM Config interrupt */
            intNumCfg = SDR_ESM_getIntNumber(SDR_ESM_INSTANCE_MAIN,
                                             SDR_ESM_INT_TYPE_CFG);
            if (intNumCfg == SDR_ESM_INTNUMBER_INVALID)
            {
                retval = -1;
            }
            else
            {
                /* Clear all interrupts first */
                HwiP_disableInterrupt(intNumHi);
                HwiP_disableInterrupt(intNumLo);
                HwiP_disableInterrupt(intNumCfg);

                /* Default parameter initialization */
                HwiP_Params_init(&hwiParams);

                hwiParams.arg = intNumHi;

                /* Register call back function for ESM Hi Interrupt */
                WWDT_Example_EsmHiHwiPHandle = HwiP_create(intNumHi,
                                                          (HwiP_Fxn)(*pHiInterruptHandler),
                                                          (void *)&hwiParams);
                if (WWDT_Example_EsmHiHwiPHandle == (HwiP_Handle) NULL)
                {
                    retval = -1;
                }
                else
                {
                    hwiParams.arg = intNumLo;
                    /* Register call back function for ESM Lo Interrupt */
                    WWDT_Example_EsmLoHwiPHandle = HwiP_create(intNumLo,
                                                              (HwiP_Fxn)(*pLoInterruptHandler),
                                                              (void *)&hwiParams);
                    if (WWDT_Example_EsmLoHwiPHandle == (HwiP_Handle) NULL)
                    {
                        retval = -1;
                    }
                    else
                    {
                        hwiParams.arg = intNumCfg;
                        /* Register call back function for ESM Config Interrupt */
                        WWDT_Example_EsmCfgHwiPHandle = HwiP_create(intNumCfg,
                                                   (HwiP_Fxn)(*pConfigInterruptHandler),
                                                   (void *)&hwiParams);
                        if (WWDT_Example_EsmCfgHwiPHandle == (HwiP_Handle) NULL)
                        {
                            retval = -1;
                        }
                        else
                        {
                            /* Enable all ESM Interrupts */
                            HwiP_enableInterrupt(intNumHi);
                            HwiP_enableInterrupt(intNumLo);
                            HwiP_enableInterrupt(intNumCfg);
                       }
                    }
                }
            }
        }
    }

    return retval;
}
