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
 *  \file   pm_main_dmsc.c
 *
 *  \brief  PM rtos implementation example for DMSC based devices.
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
#if defined (__aarch64__)
#include <ti/sysbios/family/arm/v8a/Mmu.h>
#endif
#endif

#include <ti/csl/csl_types.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/pm/pmlib.h>
#include <ti/drv/pm/Power.h>
#include <ti/drv/pm/PowerExtended.h>
#include <ti/drv/pm/PowerDevice.h>
#include "app_utils.h"

#include <ti/sysbios/knl/Clock.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#define APP_TSK_STACK_MAIN              (10U * 1024U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

void CpuIdleTest(UArg arg0, UArg arg1);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */


/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */
int32_t           testResult;
PowerDevice_Cfg_t PowerDevice_config __attribute__ ((section (".noinit")));
Power_NotifyObj   pmNotiObj;

void appReset(void);
static int NotifyCallbk(unsigned int eventType, uintptr_t eventArg,
                        uintptr_t clientArg);
/* Test application stack */
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(32)));;
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

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
    uint32_t    retVal = CSL_PASS;
    Task_Params taskParams;
    
    Error_init(&eb);
    
    /* Initialize the task params */
    Task_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority     = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stackSize    = sizeof (gAppTskStackMain);
        
    task = Task_create(CpuIdleTest, NULL, &eb);
    if (task == NULL)
    {
        App_printf("Task_create() CpuIdleTest failed!\n");
        BIOS_exit(0);
    }

    /* Start BIOS */
    BIOS_start();

    return retVal;
}


static int NotifyCallbk(unsigned int eventType, uintptr_t eventArg,
                        uintptr_t clientArg)
{
    App_printf(
               " \n NotifyCallbk registered for %d and called for event %d ",
               pmNotiObj.eventTypes, eventType);
    return Power_NOTIFYDONE;
}

void CpuIdleTest(UArg arg0, UArg arg1)
{
    uint32_t perfLevel;
    int32_t retVal = PM_SUCCESS;
    int32_t dependencyCount;
    pmlibSysConfigPowerState_t    modState;
    volatile uint32_t loopForever = 1U;

    AppUtils_defaultInit();
    App_printf("PM RTOS Test App\n");
    
    PowerDevice_Cfg_Init(&PowerDevice_config);

    PowerDevice_config.swInit = TRUE;
    PowerDevice_config.hwInit = TRUE;
    PowerDevice_config.enablePerf       = TRUE;
    PowerDevice_config.initialPerfLevel = Power_PERFLEVEL_NOM;
    PowerDevice_config.thermalCfg.cfgThermalAlert = FALSE;

    /* Initialize the system here */
    /* assign policy function for CPU idle */
    PowerDevice_config.policyInitFxn   = &PowerDevice_policyInitFxn;
    PowerDevice_config.policyDeinitFxn = &PowerDevice_policyDeinitFxn;
    PowerDevice_config.policyFxn       = &PowerDevice_policyFxn;

    /* enable policy function for CPU idle */
    Power_enablePolicy();
    /* Power Init is done in task context as the semaphores can then be
     * created.
     */
    Power_init();
    App_printf("PM Initialization complete\n");

    /* register a notify callback */
    Power_registerNotify(&pmNotiObj, Power_EVENT_ALL,
                         (Power_NotifyFxn) NotifyCallbk,
                         (uintptr_t)NULL);
    /* set the constraint such that usecase doesn't shutdown */
    Power_setConstraint(Power_DISALLOW_SHUTDOWN);

    /* try to setperf for cores other than a15 it should fail */
    /* Test OPP_NOM */
    retVal = Power_setPerformanceLevel(Power_PERFLEVEL_NOM);
    if (PM_SUCCESS != retVal)
    {
        App_printf("\n Power_setPerformanceLevel returned %d \n", retVal);
        App_printf(" Enable Perf was not set TEST PASS \n");
        testResult += PM_SUCCESS;
    }

    perfLevel = Power_getPerformanceLevel();
    App_printf("\n Power_getPerformanceLevel returned %d \n", perfLevel);

    /* Test OPP_OD */
    retVal = Power_setPerformanceLevel(Power_PERFLEVEL_OD);
    if (PM_SUCCESS != retVal)
    {
        App_printf("\n Power_setPerformanceLevel returned %d \n", retVal);
        App_printf(" Enable Perf was not set TEST PASS \n");
        testResult += PM_SUCCESS;
    }

    perfLevel = Power_getPerformanceLevel();
    App_printf("\n Power_getPerformanceLevel returned %d \n", perfLevel);

    /* Test OPP_TURBO */
    retVal = Power_setPerformanceLevel(Power_PERFLEVEL_HIGH);
    if (PM_SUCCESS != retVal)
    {
        App_printf("\n Power_setPerformanceLevel returned %d \n", retVal);
        App_printf(" Enable Perf was not set TEST PASS \n");
        testResult += PM_SUCCESS;
    }

    perfLevel = Power_getPerformanceLevel();
    App_printf("\n Power_getPerformanceLevel returned %d \n", perfLevel);

    /* TEST set release and dependency count */
    testResult += Power_setDependency(TISCI_DEV_DCC0);
    testResult += Power_setDependency(TISCI_DEV_CPT2_AGGR0);

    dependencyCount =  Power_getDependencyCount(TISCI_DEV_DCC0);
    if(dependencyCount != 0)
    {
        App_printf(" Power_releaseDependency for DCC0 dependencyCount %d \n", dependencyCount);
        testResult +=  Power_setDependency(TISCI_DEV_DCC0);
        PMLIBSysConfigGetPowerState(TISCI_DEV_DCC0,&modState,NULL);
        if(modState == PMLIB_SYS_CONFIG_ALWAYS_ENABLED)
        {
            App_printf(" Module is enabled \n");
        }
        else
        {
            testResult += PM_FAIL;
            App_printf(" Module is disabled \n");

        }
        dependencyCount =  Power_getDependencyCount(TISCI_DEV_DCC0);

        if(dependencyCount != 2)
        {
            App_printf(" FAIL Power_releaseDependency for DCC0 dependencyCount %d \n", dependencyCount);
            testResult += PM_FAIL;
        }
    }
    else
    {
        App_printf(" FAIL Power_releaseDependency for DCC0 dependencyCount %d \n", dependencyCount);
        testResult += PM_FAIL;
    }

    testResult += Power_releaseDependency(TISCI_DEV_CPT2_AGGR0);
    testResult += Power_releaseDependency(TISCI_DEV_DCC0);

    dependencyCount =  Power_getDependencyCount(TISCI_DEV_DCC0);
    if(dependencyCount == 1)
    {
        App_printf(" Power_releaseDependency for DCC0 dependencyCount %d \n", dependencyCount);
        testResult +=  Power_releaseDependency(TISCI_DEV_DCC0);
        dependencyCount =  Power_getDependencyCount(TISCI_DEV_DCC0);
        if(dependencyCount != 0)
        {
            App_printf(" FAIL Power_releaseDependency for DCC0 dependencyCount %d \n", dependencyCount);
            testResult += PM_FAIL;
        }
    }
    else
    {
        App_printf(" FAIL Power_releaseDependency for DCC0 dependencyCount %d \n", dependencyCount);
        testResult += PM_FAIL;
    }

    Power_deinit();
    if (testResult == PM_SUCCESS)
    {
        App_printf("All tests have passed.\n");
    }
    else
    {
        App_printf("Some tests have failed.\n");
    }
    while (loopForever) {;}
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif
