/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2016-2017
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
 *  \file   pm_thermal_main.c
 *
 *  \brief  pm rtos thermal management implementation example.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifndef BARE_METAL
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#if defined (__aarch64__)
#include <ti/sysbios/family/arm/v8a/Mmu.h>
#endif
#endif

#include <string.h>
#include <math.h>

#include <ti/csl/soc.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/csl_timer.h>
#include <ti/drv/pm/pmhal.h>
#include <ti/drv/pm/pmlib.h>
#include <ti/drv/pm/Power.h>
#include <ti/drv/pm/PowerExtended.h>
#include <ti/drv/pm/PowerDevice.h>
#include <ti/drv/pm/include/pmlib_boardconfig.h>
#include <ti/csl/arch/csl_arch.h>
#include "app_utils.h"

#include <ti/sysbios/knl/Clock.h>

#include <ti/csl/csl_esm.h>
#include <ti/csl/soc/am65xx/src/cslr_soc.h>
#include <ti/csl/soc/am65xx/src/cslr_intr_polarity.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define TIMER_INITIAL_COUNT             (0xFFF00000U)
#define TIMER_RLD_COUNT                 (0xFFF00000U)

#define LOW_SET_POINT_IDX (PowerDevice_SET_POINT_IDX_LOW_SENSOR0)
#define HIGH_SET_POINT_IDX (PowerDevice_SET_POINT_IDX_HIGH_SENSOR0)

/** Depth of temperature history buffer */
#define TEMP_HISTORY                         (10U)
/** Heat up loop iterations */
#define HEAT_UP_ITERATIONS                   (1000000)
/** Stable temperature standard deviation */
#define TEMP_STABLE_STDDEV                   (2U)
/** High offset from stable temperature to raise to hit hot alert */
#define TEMP_HIGH_MOD                        (8U)

#define MAIN_ESM_INSTANCE   CSL_ESM0_CFG_BASE
#define MCU_ESM_INSTANCE    CSL_MCU_ESM0_CFG_BASE
#define WKUP_ESM_INSTANCE   CSL_WKUP_ESM0_CFG_BASE

#define MAIN_ESM_INTID      CSL_MCU0_INTR_ESM2_ESM_INT_HI_LVL
#define MCU_ESM_INTID       CSL_MCU0_INTR_ESM1_ESM_INT_HI_LVL
#define WKUP_ESM_INTID      CSL_MCU0_INTR_ESM0_ESM_INT_HI_LVL

/* Using an actual number as the CSLR ESM data for WKUP ESM is not correct */
#define WKUP_ESM_ERR_SIG_OVR    (8U)
#define WKUP_ESM_ERR_SIG_UND    (9U)
/*
* To change the ESM Instance as desired, use the following definitions:
* - ESM_INSTANCE
* - ESM_INTID
* - ESM_ERR_SIG
*/
#define ESM_INSTANCE        WKUP_ESM_INSTANCE
#define ESM_INTID           WKUP_ESM_INTID

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
void mainTimerTick(UArg arg);

void CpuThermalTest(UArg arg0, UArg arg1);

void appReset(void);
#if defined (BUILD_MCU1_0)
/**
 * \brief   API to initialize ESM
 *
 * \param   none.
 *
 * \retval  none.
 */static void EsmThermal_init();
#endif
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Boolean for thermal high reached */
static bool gotThermalHighInt;
/* Boolean for thermal low reached */
static bool gotThermalLowInt;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

PowerDevice_Cfg_t PowerDevice_config;

/* On reset bios tick needs to be up
   PRCM configuration is done in the app and hence
   timer which are used by bios are to be initialized at reset only.
   PowerDevice_Cfg_Init is called to reset the global structure
   along with software init
   */
void appReset(void)
{
    PowerDevice_Cfg_Init(&PowerDevice_config);
    PowerDevice_config.swInit = TRUE;
    Power_init();
}


int main(void)
{
    Task_Handle task;
    Error_Block eb;
    Error_init(&eb);

    task = Task_create(CpuThermalTest, NULL, &eb);
    if (task == NULL)
    {
        App_printf("Task_create() CpuThermalTest failed!\n");
        BIOS_exit(0);
    }

    /* Start BIOS */
    BIOS_start();

    return 0;
}

/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */
int32_t           testResult = PM_SUCCESS;
PowerDevice_Cfg_t PowerDevice_config;
Power_NotifyObj   pmNotifyObjHot;
Power_NotifyObj   pmNotifyObjCold;
bool              unstableTemp;
int32_t           stableTemp;

static int NotifyHotCallbk(unsigned int eventType, uintptr_t eventArg,
                           uintptr_t clientArg)
{
    uint32_t setPointIdx = eventArg;
    int      minTemp, maxTemp;

    Power_getCurrentTemp(setPointIdx, &minTemp, &maxTemp);

    gotThermalHighInt = TRUE;
    ESMClearIntrStatus(ESM_INSTANCE, WKUP_ESM_ERR_SIG_OVR);
    ESMResetErrPin(ESM_INSTANCE);
    /* enable policy function for CPU idle */
    Power_enablePolicy();

    return Power_NOTIFYDONE;
}

static int NotifyColdCallbk(unsigned int eventType, uintptr_t eventArg,
                            uintptr_t clientArg)
{
    uint32_t setPointIdx = eventArg;
    int      minTemp, maxTemp;

    Power_getCurrentTemp(setPointIdx, &minTemp, &maxTemp);

    gotThermalLowInt = TRUE;
    
    ESMClearIntrStatus(ESM_INSTANCE, WKUP_ESM_ERR_SIG_UND);
    ESMResetErrPin(ESM_INSTANCE);
    /* disable policy function for CPU idle */
    Power_disablePolicy();

    return Power_NOTIFYDONE;
}

void CpuThermalTest(UArg arg0, UArg arg1)
{
    int32_t retVal;
    int32_t avgTempHist[TEMP_HISTORY];
    int     minTemp, maxTemp;

    AppUtils_defaultInit();
    App_printf("\n PM RTOS Thermal Test App");

    PowerDevice_Cfg_Init(&PowerDevice_config);

    PowerDevice_config.swInit           = TRUE;
    PowerDevice_config.hwInit           = TRUE;

    /* Assign policy function for CPU idle */
    PowerDevice_config.policyInitFxn   = &PowerDevice_policyInitFxn;
    PowerDevice_config.policyDeinitFxn = &PowerDevice_policyDeinitFxn;
    PowerDevice_config.policyFxn       = &PowerDevice_policyFxn;

    /* Thermal interrupt configuration */
    PowerDevice_config.thermalCfg.cfgThermalAlert = TRUE;
    PowerDevice_config.thermalCfg.alertIntPriority = 0x0U;

    PowerDevice_config.thermalCfg.eventId = ESM_INTID;
    PowerDevice_config.thermalCfg.intNum = ESM_INTID;

    PowerDevice_config.thermalCfg.xbarCpuIrq = 0;

    gotThermalHighInt = FALSE;
    gotThermalLowInt = FALSE;

    /* Initialize the system here */
    retVal = Power_init();
    if (retVal != Power_SOK)
    {
        App_printf(" \n Power_init returned %d ", retVal);
        App_printf(" \n TEST FAILED ");
        testResult += retVal;
        return;
    }

    /* register notify callbacks for hot and cold events */
    Power_registerNotify(&pmNotifyObjHot, PowerDevice_THERMAL_HIGH,
                         (Power_NotifyFxn) NotifyHotCallbk, 0);
    Power_registerNotify(&pmNotifyObjCold, PowerDevice_THERMAL_LOW,
                         (Power_NotifyFxn) NotifyColdCallbk, 0);

    /* get temperature until stable */
    App_printf(" \n Calculating stable temperature.");
    unstableTemp = TRUE;
    while (unstableTemp)
    {
        uint32_t i;
        double   sum;
        double   mean;
        double   stddev;

        memset(&avgTempHist[0], 0, sizeof(avgTempHist));

        for (i = 0; i < TEMP_HISTORY; i++)
        {
            retVal = Power_getCurrentTemp(HIGH_SET_POINT_IDX, &minTemp, &maxTemp);
            if (retVal != Power_SOK)
            {
                App_printf(" \n Power_getCurrTemp returned %d ", retVal);
                App_printf(" \n TEST FAILED ");
                testResult += retVal;
                return;
            }

            /* take average of returned temp range then convert to C from
             * milli-degree C*/
            avgTempHist[i] = (((minTemp + maxTemp) / 2U) / 1000U);
            /* wait until next measurement */
            App_printf(".");
            Task_sleep(100000/Clock_tickPeriod);
        }

        /* find standard deviation */
        for (i = 0, sum = 0; i < TEMP_HISTORY; i++)
        {
            sum += (double) avgTempHist[i];
        }
        mean = sum / (double)TEMP_HISTORY;

        for (i = 0, sum = 0; i < TEMP_HISTORY; i++)
        {
            sum += pow((double)avgTempHist[i] - mean, 2);
        }
        stddev = sqrt(sum / (double)TEMP_HISTORY);

        if (stddev < TEMP_STABLE_STDDEV)
        {
            unstableTemp = FALSE;
            stableTemp = (int32_t) mean;
        }
    }

    App_printf(" \n Stable temperature reached : %d C ", stableTemp);

    /* configure high setpoint to be slightly above average temp */
#if defined (BUILD_MCU1_0)
    EsmThermal_init();
    retVal = Power_enableThermalSetPoint(HIGH_SET_POINT_IDX,
                                        (stableTemp - TEMP_HIGH_MOD) * 1000);
    if (retVal != Power_SOK)
    {
        App_printf(" \n Power_enableThermalSetPoint returned %d ", retVal);
        App_printf(" \n TEST FAILED ");
        testResult += retVal;
        return;
    }
    else
    {
        Power_getThermalSetPoint(HIGH_SET_POINT_IDX, &minTemp, &maxTemp);
        App_printf(" \n Configured high set point to %d milli deg C ", minTemp);
    }

    while (!gotThermalHighInt)
    {
        /* wait until next measurement */
        Task_sleep(10000/Clock_tickPeriod);
        if (!gotThermalHighInt)
        {
            /* Print last temp range to relay progress */
            App_printf(
                " \n Raising temp - current range : %d milli deg C ", minTemp);
        }
    }

    Power_getCurrentTemp(HIGH_SET_POINT_IDX, &minTemp, &maxTemp);
    App_printf(" \n Temperature range at moment of high alert: %d milli deg C", minTemp);

    /* configure low setpoint to be stable temp */
    retVal = Power_enableThermalSetPoint(LOW_SET_POINT_IDX,
                                        (stableTemp + TEMP_HIGH_MOD) * 1000);

    if (retVal != Power_SOK)
    {
        App_printf(" \n Power_enableThermalSetPoint returned %d ", retVal);
        App_printf(" \n TEST FAILED ");
        testResult += retVal;
        return;
    }
    else
    {
        Power_getThermalSetPoint(LOW_SET_POINT_IDX, &minTemp, &maxTemp);
        App_printf(" \n Configured low set point to %d milli deg C", minTemp);
    }

    /* Wait for temperature to come down to stable temp */
    while (!gotThermalLowInt)
    {
        /* wait until next measurement */
        Task_sleep(10000/Clock_tickPeriod);

        if (!gotThermalLowInt)
        {
            Power_getCurrentTemp(LOW_SET_POINT_IDX, &minTemp, &maxTemp);
            App_printf(
                " \n Lowering temp - current range : %d milli deg C", minTemp);
        }
    }

    Power_getCurrentTemp(LOW_SET_POINT_IDX, &minTemp, &maxTemp);
    App_printf(" \n Temperature range at moment of low alert: %d milli deg C", minTemp);
#endif
    App_printf("\n Test Completed!!");

    retVal = Power_deinit();
    if (retVal != Power_SOK)
    {
        App_printf(" \n Power_deinit returned %d ", retVal);
        App_printf(" \n Some tests have failed.");
    }
    else
    {
        App_printf("\n All tests have passed.");
    }
}

void mainTimerTick(UArg arg)
{
    Clock_tick();
}
#if defined (BUILD_MCU1_0)
static void EsmThermal_init()
{
    /* ESM Variables */
    esmInfo_t appEsmInfo;

    /* Check INFO register for ESM last reset cause */
    ESMGetInfo(ESM_INSTANCE, &appEsmInfo);
    ESMReset(ESM_INSTANCE);

    /* The below function can be changed to force an error for diagnostic
     * reasons. */
    /* make sure we're not in force error mode */
    ESMSetMode(ESM_INSTANCE, ESM_OPERATION_MODE_NORMAL);

    /* Enable this ESM Error Signal */
    ESMEnableIntr(ESM_INSTANCE, WKUP_ESM_ERR_SIG_OVR);

    /* Set the output interrupt priority level */
    ESMSetIntrPriorityLvl(ESM_INSTANCE, WKUP_ESM_ERR_SIG_OVR, ESM_INTR_PRIORITY_LEVEL_HIGH);

    /* Enable Error Pin on this ESM Error Signal */
    ESMSetInfluenceOnErrPin(ESM_INSTANCE, WKUP_ESM_ERR_SIG_OVR, TRUE);
    
    /* Enable this ESM Error Signal */
    ESMEnableIntr(ESM_INSTANCE, WKUP_ESM_ERR_SIG_UND);

    /* Set the output interrupt priority level */
    ESMSetIntrPriorityLvl(ESM_INSTANCE, WKUP_ESM_ERR_SIG_UND, ESM_INTR_PRIORITY_LEVEL_HIGH);

    /* Enable Error Pin on this ESM Error Signal */
    ESMSetInfluenceOnErrPin(ESM_INSTANCE, WKUP_ESM_ERR_SIG_UND, TRUE);

    /* Enable for all ESM Error Signals */
    ESMEnableGlobalIntr(ESM_INSTANCE);
}
#endif

#ifndef BARE_METAL
#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif
#endif
