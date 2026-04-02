/*
 * ESM Example
 *
 * Error Signaling Module (ESM) Example Application
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
 *  \brief ESM utility functions and handler ISR's
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
/* For Timer functions */
#include <ti/osal/osal.h>

/* For interrupt ID numbers */
#include <ti/csl/soc/cslr_soc_intr.h>

/* Sciclient */
#include <ti/csl/csl_types.h>
#include <ti/drv/sciclient/sciclient.h>

/* Diagnostic example utility functions, e.g. print outputs */
#include <diag_utils.h>
#include <sdr_esm.h>

#include "esm.h"
#include "event_trig.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define MAX_ESM_EVENTS_LOGGED (20)
#define MAX_ESM_INSTANCE      (SDR_ESM_INSTANCE_MAIN)
#define MAX_ESM_INT_TYPE      (SDR_ESM_INT_TYPE_CFG)

/* ESM event log entry */
typedef struct
{
    SDR_ESM_InstanceType esmInstance;
    SDR_ESM_IntType      intType;
    uint32_t             grpChannel;
    uint32_t             index;
    uint32_t             intSrc;
    uint8_t              useCaseNum;
} ESM_Example_log_entry_t;

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t ESM_Example_EsmInitHandlerInit(SDR_ESM_InstanceType esmInstType);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static ESM_Example_log_entry_t esmEventLog[MAX_ESM_EVENTS_LOGGED];
static uint32_t totalEventsLogged   = 0;
static uint32_t totalHiEventsLogged = 0;
static uint32_t totalLoEventsLogged = 0;
static int32_t  thresholdsReset     = 0;

/* Initialization structure for MCU ESM instance */
static SDR_ESM_InitConfig_t ESM_Example_esmInitConfig_MCU =
{
    .esmErrorConfig = {0u, 3u}, /* Self test error config */
    .eventMap = {0xffffffffu, 0x033fffffu, 0x7fffffffu, 0xffffffe7u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                },
     /**< All events enable: except timer and self test  events, */
    /*    and Main ESM output.  Disabling vim compare error as well.
     *    Exclude MCU Timer 0, which is left on by SBL in continuous mode,
     *    and MCU Timer 1, which is used for controlling the Output pin clearing.
     *    MCU Timer 2 set to High Priority with output pin routing.
     *    MCU Timer 3 set to Low Priorirty with no Output pin routing. */
    .eventPriorityMap = {0xffffffffu, 0x013fffffu, 0x7fffffffu, 0xffffffe6u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                        },
    /**< All events high priority: except timer, selftest error events,
     *   and Main ESM output. Exclude MCU Timer 0, which is left on by
     *   SBL in continuous mode, and MCU Timer 1, which is used for controlling
     *   the Output pin clearing.
     *   MCU Timer 2 set to High Priority with output pin routing.
     *   MCU Timer 3 set to Low Priorirty with no Output pin routing. */
    .errorOutputMap = {0xffffffffu, 0x013fffffu, 0x7fffffffu, 0xffffffe6u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                      },
    /**< All events high priority: except timer, selftest error events,
     *   and Main ESM output. Exclude MCU Timer 0, which is left on by
     *   SBL in continuous mode, and MCU Timer 1, which is used for controlling
     *   the Output pin clearing.
     *   MCU Timer 2 set to High Priority with output pin routing.
     *   MCU Timer 3 set to Low Priorirty with no Output pin routing. */
};

/* Initialization structure for WKUP ESM instance */
static SDR_ESM_InitConfig_t ESM_Example_esmInitConfig_WKUP =
{
    .esmErrorConfig = {0u, 0u}, /* Self test error config */
    .eventMap = {0xffffffffu, 0x00180003u, 0xffffffffu, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                },
     /**< All events enable: except clkstop events for unused clocks */
    .eventPriorityMap = {0xfffffcffu, 0x00180003u, 0xffffffffu, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                         0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                        },
    /**< All events high priority: except clkstop events for unused clocks,
     *   and make VTM WKUP_VTM0_THERM_LVL_GT_TH1_INTR_0 and
     *   WKUP_VTM0_THERM_LVL_LT_TH0_INTR_0 events low priority */
    .errorOutputMap = {0xfffffcffu, 0x00180003u, 0xffffffffu, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                       0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                      },
    /**< All events high priority: except clkstop for unused clocks
     *   and make VTM WKUP_VTM0_THERM_LVL_GT_TH1_INTR_0 and
     *   WKUP_VTM0_THERM_LVL_LT_TH0_INTR_0 events not output to pin */
};

static HwiP_Handle ESM_Example_EsmHiHwiPHandle;
static HwiP_Handle ESM_Example_EsmLoHwiPHandle;
static HwiP_Handle ESM_Example_EsmCfgHwiPHandle;

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* Completion of Use Case from ESM Handler perspective updates these flags */
extern volatile uint32_t esmOutputResult[5];
/* Current use case being run */
extern volatile uint8_t  currUseCase;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*********************************************************************
* @fn      ESM_Example_init
*
* @brief   Initializes Board, Timers, and ESM module
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t ESM_Example_init (void)
{
    int32_t retValue=0;
    Sciclient_ConfigPrms_t config;
    SDR_Result result;

#ifdef UART_PRINTF
    /* Initialize UART instance */
    DIAG_uartStdioInit();
#endif

    DIAG_printf("\nESM_Example_init: UART ready to print, proceeding with ESM Example init \n");

    if (retValue == 0) {
        /* Initialize Sciclient */
        retValue = Sciclient_configPrmsInit(&config);
        if (retValue == CSL_PASS) {
            retValue = Sciclient_init(&config);
        }
        if (retValue != CSL_PASS) {
            DIAG_printf("ESM_Example_init: Error initializing Sciclient: result = %d\n", retValue);
            retValue = -1;
        }
    }

    if (retValue == 0) {
        /* Initialize MCU ESM module */
        result = SDR_ESM_init(SDR_ESM_INSTANCE_MCU, &ESM_Example_esmInitConfig_MCU);
        if (result != SDR_PASS) {
            /* print error and quit */
             DIAG_printf("ESM_Example_init: Error initializing MCU ESM: result = %d\n", result);

            retValue = -1;
        } else {
            DIAG_printf("\nESM_Example_init: Init MCU ESM complete \n");
        }
    }

    if (retValue == 0) {
        /* Initialize WKUP ESM module */
        result = SDR_ESM_init(SDR_ESM_INSTANCE_WKUP, &ESM_Example_esmInitConfig_WKUP);
        if (result != SDR_PASS) {
            /* print error and quit */
            DIAG_printf("ESM_Example_init: Error initializing WKUP ESM: result = %d\n", result);

            retValue = -1;
        } else {
            DIAG_printf("\nESM_Example_init: Init WKUP ESM complete \n");
        }
    }

    if (retValue == 0) {
        /* Register MCU Esm handlers */
        retValue = ESM_Example_EsmInitHandlerInit(SDR_ESM_INSTANCE_MCU);
        if (retValue != 0) {
            DIAG_printf("\nESM_Example_init: Init MCU ESM handlers failed \n");
        } else {
            DIAG_printf("\nESM_Example_init: Init MCU ESM handlers complete \n");
        }

        /* Register WKUP Esm handlers */
        retValue = ESM_Example_EsmInitHandlerInit(SDR_ESM_INSTANCE_WKUP);
        if (retValue != 0) {
            DIAG_printf("\nESM_Example_init: Init WKUP ESM handlers failed \n");
        } else {
            DIAG_printf("\nESM_Example_init: Init WKUP ESM handlers complete \n");
        }
    }

    return retValue;
}

/*********************************************************************
* @fn      ESM_Example_EsmInitHandlerInit
*
* @brief   Register all callbacks for an ESM instance
*
* @param   esmInstType: Instance type of ESM 
*
* @return    0 : Success; < 0 for failures
*/
static int32_t ESM_Example_EsmInitHandlerInit(SDR_ESM_InstanceType esmInstType)
{
    int32_t result = 0;
    uint32_t intNumHi, intNumLo, intNumCfg;
    HwiP_Params       hwiParams;
    void (*pHiInterruptHandler)(uintptr_t);
    void (*pLoInterruptHandler)(uintptr_t);
    void (*pConfigInterruptHandler)(uintptr_t);

    switch (esmInstType) {
        case (SDR_ESM_INSTANCE_WKUP):
            pHiInterruptHandler = &SDR_ESM_hiInterruptHandler_WKUP;
            pLoInterruptHandler = &SDR_ESM_loInterruptHandler_WKUP;
            pConfigInterruptHandler = &SDR_ESM_configInterruptHandler_WKUP;
            break;

        case (SDR_ESM_INSTANCE_MAIN):
            pHiInterruptHandler = &SDR_ESM_hiInterruptHandler_MAIN;
            pLoInterruptHandler = &SDR_ESM_loInterruptHandler_MAIN;
            pConfigInterruptHandler = &SDR_ESM_configInterruptHandler_MAIN;
            break;

        case (SDR_ESM_INSTANCE_MCU):
        default:
            pHiInterruptHandler = &SDR_ESM_hiInterruptHandler_MCU;
            pLoInterruptHandler = &SDR_ESM_loInterruptHandler_MCU;
            pConfigInterruptHandler = &SDR_ESM_configInterruptHandler_MCU;
            break;
    }

    /* Get the interrupt number corresponding to ESM HI interrupt */
    intNumHi = SDR_ESM_getIntNumber(esmInstType,
                                    SDR_ESM_INT_TYPE_HI);
    if ( intNumHi == SDR_ESM_INTNUMBER_INVALID ) {
        result = -1;
    } else {
        /* Get the interrupt number corresponding to ESM LO interrupt */
        intNumLo = SDR_ESM_getIntNumber(esmInstType,
                                        SDR_ESM_INT_TYPE_LO);
        if ( intNumLo == SDR_ESM_INTNUMBER_INVALID ) {
            result = -1;
        } else {
            /* Get the interrupt number corresponding to ESM Config interrupt */
            intNumCfg = SDR_ESM_getIntNumber(esmInstType,
                                             SDR_ESM_INT_TYPE_CFG);
            if ( intNumCfg == SDR_ESM_INTNUMBER_INVALID ) {
                result = -1;
            } else {
                DIAG_printf("\n ESM_Example_EsmInitHandlerInit: " \
                            "Obtain interrupt number complete \n");
                /* Clear all interrupts first */
                HwiP_disableInterrupt(intNumHi);
                HwiP_disableInterrupt(intNumLo);
                HwiP_disableInterrupt(intNumCfg);

                DIAG_printf("\n ESM_Example_EsmInitHandlerInit: " \
                            "Disable interrupts complete \n");

                /* Default parameter initialization */
                HwiP_Params_init(&hwiParams);

                hwiParams.arg = intNumHi;
                DIAG_printf("\n ESM_Example_EsmInitHandlerInit: " \
                            "HwiP_Params_init complete \n");

                /* Register call back function for ESM Hi Interrupt */
                ESM_Example_EsmHiHwiPHandle = HwiP_create(intNumHi,
                                                          (HwiP_Fxn)(*pHiInterruptHandler),
                                                          (void *)&hwiParams);
                DIAG_printf("\n ESM_Example_EsmInitHandlerInit: " \
                            "intNumHi registration complete \n");
                if (ESM_Example_EsmHiHwiPHandle == (HwiP_Handle) NULL) {
                    result = -1;
                } else {
                    hwiParams.arg = intNumLo;
                    /* Register call back function for ESM Lo Interrupt */
                    ESM_Example_EsmLoHwiPHandle = HwiP_create(intNumLo,
                                                              (HwiP_Fxn)(*pLoInterruptHandler),
                                                              (void *)&hwiParams);
                    DIAG_printf("\n ESM_Example_EsmInitHandlerInit: " \
                                "intNumLo registration complete \n");
                    if (ESM_Example_EsmLoHwiPHandle == (HwiP_Handle) NULL) {
                        result = -1;
                    } else {
                        hwiParams.arg = intNumCfg;
                        /* Register call back function for ESM Config Interrupt */
                        ESM_Example_EsmCfgHwiPHandle = HwiP_create(intNumCfg,
                                                   (HwiP_Fxn)(*pConfigInterruptHandler),
                                                   (void *)&hwiParams);
                        DIAG_printf("\n ESM_Example_EsmInitHandlerInit: " \
                                    "intNumCfg registration complete \n");
                        if (ESM_Example_EsmCfgHwiPHandle == (HwiP_Handle) NULL) {
                            result = -1;
                        } else {
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

    return result;
}

static int32_t deactivateTrigger(SDR_ESM_InstanceType esmInstType,
                                 SDR_ESM_IntType esmIntType,
                                 uint32_t intEsmSrc)
{
    int32_t retVal = 0;

    if ((esmInstType == SDR_ESM_INSTANCE_WKUP) && (esmIntType == SDR_ESM_INT_TYPE_LO)) {
        /* UC-1: Low Priority interrupt on WKUP ESM -
         * VTM greater than THR1 */
        if (intEsmSrc ==
            CSLR_WKUP_ESM0_ESM_LVL_EVENT_WKUP_VTM0_THERM_LVL_GT_TH1_INTR_0)
        {
            if (currUseCase == 0)
            {
                if (thresholdsReset == 0)
                {
                    /* Simulate thresholds as if temperature is going to be reduced
                     * below lt_Thr0 */
                    diag_esm_setNormalThresholds();
#ifdef DEBUG_PRINT
                    diag_esm_printCurrentThresholds();
#endif
                    thresholdsReset = 1;
                }
                diag_esm_vtmGtThr1CrossedUpdateInt();
            } else if ((currUseCase == 1) || (currUseCase == 2))
            {
                if (thresholdsReset == 0)
                {
                    /* Simulate thresholds as if temperature continues to increase
                     * toward gt_Thr2 */
                    diag_esm_setThresholdsForCriticalTrigger();
#ifdef DEBUG_PRINT
                    diag_esm_printCurrentThresholds();
#endif
                    thresholdsReset = 1;
                }
                diag_esm_vtmGtThr1CrossedUpdateInt();
            }
        } else if (intEsmSrc ==
                   CSLR_WKUP_ESM0_ESM_LVL_EVENT_WKUP_VTM0_THERM_LVL_LT_TH0_INTR_0)
        {
            diag_esm_vtmLtThr0CrossedUpdateInt();
#ifdef DEBUG_PRINT
            diag_esm_printCurrentThresholds();
#endif
            thresholdsReset = 0;
            esmOutputResult[currUseCase] = USE_CASE_STATUS_COMPLETED_SUCCESS;
            if (currUseCase == 1) {
                /* At end of this use case, clear the Pin that was left on
                 * throughout the use case*/
                Osal_delay(4);
                diag_esm_startPinClearTimer(true);
            }
        }
    } else if ((esmInstType == SDR_ESM_INSTANCE_WKUP) &&
               (esmIntType == SDR_ESM_INT_TYPE_HI)) {
        if ((currUseCase == 1) || (currUseCase == 2)) {
            /* UC-2: High Priority interrupt on WKUP ESM -
             * VTM greater than THR2, no clearing of
             * MCU_SAFETY_ERRORn pin */
            /* UC-3: High Priority interrupt on WKUP ESM -
             * VTM greater than THR2 with clearing
             * of MCU_SAFETY_ERRORn pin */
            if (thresholdsReset == 1)
            {
                /* Simulate thresholds as if temperature is going to be reduced
                 * below lt_Thr0 */
                diag_esm_setNormalThresholds();
#ifdef DEBUG_PRINT
                diag_esm_printCurrentThresholds();
#endif
                thresholdsReset = 2;
            }
            if (currUseCase == 2) {
                diag_esm_startPinClearTimer(false);
            }
            diag_esm_vtmGtThr2CrossedUpdateInt();
        } else {
            retVal = -1;
        }
    } else if (((esmInstType == SDR_ESM_INSTANCE_MCU) &&
                (esmIntType == SDR_ESM_INT_TYPE_LO)) ||
               ((esmInstType == SDR_ESM_INSTANCE_MCU) &&
                (esmIntType == SDR_ESM_INT_TYPE_HI))) {
        /* UC-4: Low Priority interrupt on MCU ESM -
         * MCU TIMER 2 expiration */
        /* UC-5: High Priority interrupt on MCU ESM -
         * MCU TIMER 1 expiration */
        /* Clear the timer interrupt */
        retVal = diag_esm_timerUpdateInt(intEsmSrc);
        if (retVal == 0) {
            esmOutputResult[currUseCase] = USE_CASE_STATUS_COMPLETED_SUCCESS;
        }
    } else {
        DIAG_printf("ERR: Unexpected ESM Instance %d and ESM Interrupt Type %d \n",
                    esmInstType, esmIntType);
        retVal = -1;
    }

   return (retVal); 
}

static const char *printEsmIntType(SDR_ESM_IntType esmIntType)
{
    char *pStr;

    switch(esmIntType)
    {
        case SDR_ESM_INT_TYPE_HI:
            pStr = "High Priority ESM event";
            break;
        case SDR_ESM_INT_TYPE_LO:
            pStr = "Low Priority ESM event";
            break;
        case SDR_ESM_INT_TYPE_CFG:
            pStr = "Config ESM event";
            break;
        default:
            pStr = NULL;
            break;
    }

    return pStr;
}

/*********************************************************************
* @fn      ESM_Example_printSummary
*
* @brief   Print summary of all the example use cases run
*
* @param   None
*
* @return  None
*/
void ESM_Example_printSummary(void)
{
    int32_t i;

    DIAG_printf("\n\n");
    DIAG_printf("ESM Example Application summary\n");
    DIAG_printf("-------------------------------\n");
    DIAG_printf("Completed %d Use Cases\n", currUseCase);
    DIAG_printf("Received %d High Priority Interrupts\n", totalHiEventsLogged);
    DIAG_printf("Received %d Low Priority Interrupts\n", totalLoEventsLogged);

    DIAG_printf("\nUse Case Event Log\n");
    DIAG_printf("------------------\n");
    for (i = 0; i < totalEventsLogged; i++) {
        DIAG_printf("\nUse Case %d: ESM Call back function called : grpChannel 0x%x, " \
                    "index 0x%x, intSrc 0x%x \n",
                    esmEventLog[i].useCaseNum,
                    esmEventLog[i].grpChannel,
                    esmEventLog[i].index,
                    esmEventLog[i].intSrc);
        DIAG_printf("  ESM instance #%d, ESM interrupt type = %s\n",
                    esmEventLog[i].esmInstance,
                    printEsmIntType(esmEventLog[i].intType));

    }
}

void SDR_ESM_applicationCallbackFunction(SDR_ESM_InstanceType esmInstType,
                                         SDR_ESM_IntType esmIntType,
                                         uint32_t grpChannel,
                                         uint32_t index,
                                         uint32_t intSrc)
{
    /* Log the event */
    esmEventLog[totalEventsLogged].useCaseNum  = currUseCase;
    esmEventLog[totalEventsLogged].esmInstance = esmInstType;
    esmEventLog[totalEventsLogged].intType     = esmIntType;
    esmEventLog[totalEventsLogged].grpChannel  = grpChannel;
    esmEventLog[totalEventsLogged].index       = index;
    esmEventLog[totalEventsLogged].intSrc      = intSrc;

    totalEventsLogged++;
    if (esmIntType == SDR_ESM_INT_TYPE_HI) {
        totalHiEventsLogged++;
    } else if (esmIntType == SDR_ESM_INT_TYPE_LO) {
        totalLoEventsLogged++;
    }

    /* Any additional customer-specific actions to address ESM event
     * can be added here */

    deactivateTrigger(esmInstType, esmIntType, intSrc);

#ifdef PRINT_EVENTS
    /* Print information to screen */
    DIAG_printf("\n  ESM Call back function called : instType 0x%x, intType 0x%x, " \
                "grpChannel 0x%x, index 0x%x, intSrc 0x%x \n",
                esmInstType, esmIntType, grpChannel, index, intSrc);
    DIAG_printf("  Take action \n");

    DIAG_printf("  ESM instance #%d, ESM interrupt type = %s\n",
                esmInstType, printEsmIntType(esmIntType));
#endif
    esmOutputResult[currUseCase]= USE_CASE_STATUS_COMPLETED_SUCCESS;

    return;
}

void SDR_assertExternalFunction(SDR_assertErrorNumber errorNumber)
{

    DIAG_printf("\n  Assert called with error Number %d \n",
                errorNumber);
    /* Asserts are fatal: Currently just wait for ever */
    while(1);
}
