/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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
  *  \file    safety_app.c
  *
  *  \brief   This file contains safety checkers app code containing PM, RM and TIFS checkers.
  *
  */

/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include "FreeRTOS.h"
#include "task.h"
#include <kernel/dpl/HwiP.h>
#include <kernel/dpl/TimerP.h>
#include <kernel/dpl/TaskP.h>
#include <kernel/dpl/SystemP.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/ClockP.h>
#include <sciclient.h>
#include <safety_checkers_common.h>
#include <safety_checkers_pm.h>
#include <safety_checkers_tifs.h>
#include "tifs_checkers_fwl_config.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define SAFETY_APP_RM_CHECKERS_REG_BLOB_SIZE    25000U
#define SAFETY_APP_TASK_PRI                    (configMAX_PRIORITIES-1)
#define SAFETY_APP_STACK_SIZE                  (4*1024U)
#define SAFETY_APP_LOOP_COUNT                  (10U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uintptr_t    gSafetyApp_pscRegisterData[SAFETY_CHECKERS_PM_PSC_REGDUMP_SIZE];
uintptr_t    gSafetyApp_pllRegisterData[SAFETY_CHECKERS_PM_PLL_REGDUMP_SIZE];
uintptr_t    gSafetyApp_rmRegisterData[SAFETY_APP_RM_CHECKERS_REG_BLOB_SIZE];

SafetyCheckers_TifsFwlConfig *gSafetyApp_pFwlConfig = gSafetyCheckers_TifsFwlConfig;
uint32_t gSafetyApp_tifsFwlCfgSize = TIFS_CHECKER_FWL_MAX_NUM;

uint8_t gSafetyApp_taskStack[SAFETY_APP_STACK_SIZE] __attribute__((aligned(32)));
TaskP_Object gSafetyApp_safetyLoopTask;

static SemaphoreP_Object gSafetyApp_BinarySemTaskSync;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void SafetyApp_safetyLoop(void *args);

static void SafetyApp_clockCbFxn(ClockP_Object *obj, void *args);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void SafetyApp_main(void *args)
{
    uint32_t  status = SystemP_SUCCESS;

    /* Creating a binary semaphore to sync the main task and safety loop*/
    status = SemaphoreP_constructBinary(&gSafetyApp_BinarySemTaskSync, 0);
    if(status != SystemP_SUCCESS)
    {
        DebugP_log("Binary semaphore object creation failed!!");
    }

    if(status == SystemP_SUCCESS)
    {
        /* Get PM Checkers Register Configuration */
        status = SafetyCheckers_pmGetPscRegCfg(gSafetyApp_pscRegisterData, SAFETY_CHECKERS_PM_PSC_REGDUMP_SIZE);
        if (status == SystemP_SUCCESS)
        {
            DebugP_log("Get PSC configuration successful.\r\n");
        }
        else
        {
            DebugP_log("Get PSC configuration unsuccessful.\r\n");
        }

        status = SafetyCheckers_pmGetPllRegCfg(gSafetyApp_pllRegisterData, SAFETY_CHECKERS_PM_PLL_REGDUMP_SIZE);
        if (status == SystemP_SUCCESS)
        {
            DebugP_log("Get PLL configuration successful.\r\n");
        }
        else
        {
            DebugP_log("Get PLL configuration unsuccessful.\r\n");
        }

        /* Get RM Checkers Register Configuration */
        status = SafetyCheckers_rmGetRegCfg(gSafetyApp_rmRegisterData, SAFETY_APP_RM_CHECKERS_REG_BLOB_SIZE);
        if (status == SystemP_SUCCESS)
        {
            DebugP_log("Get RM configuration successful.\r\n");
        }
        else
        {
            DebugP_log("Get RM configuration unsuccessful.\r\n");
        }

        /* Request TIFS firewall open */
        status = SafetyCheckers_tifsReqFwlOpen();
        if (status == SystemP_SUCCESS)
        {
            DebugP_log("Firewall open successful.\r\n");
        }
        else
        {
            DebugP_log("Firewall open unsuccessful.\r\n");
        }

        /* Get TIFS Checkers Register Configuration */
        status = SafetyCheckers_tifsGetFwlCfg(gSafetyApp_pFwlConfig, gSafetyApp_tifsFwlCfgSize);
        if (status == SystemP_SUCCESS)
        {
            DebugP_log("Get firewall configuration successful.\r\n");
        }
        else
        {
            DebugP_log("Get firewall configuration unsuccessful.\r\n");
        }

        /* Place to verify and save firewall configuration as golden reference */

        /* Creating safety loop thread */
        TaskP_Params SafetyLoopTaskParams;

        TaskP_Params_init(&SafetyLoopTaskParams);
        SafetyLoopTaskParams.name = "SAFETY_LOOP";
        SafetyLoopTaskParams.stackSize = SAFETY_APP_STACK_SIZE;
        SafetyLoopTaskParams.stack = gSafetyApp_taskStack;
        SafetyLoopTaskParams.priority = SAFETY_APP_TASK_PRI;

        SafetyLoopTaskParams.taskMain = SafetyApp_safetyLoop;

        status = TaskP_construct(&gSafetyApp_safetyLoopTask, &SafetyLoopTaskParams);

        if (status == SystemP_FAILURE)
        {
            DebugP_log("Safety loop task creation failed!!\r\n");
            TaskP_destruct(&gSafetyApp_safetyLoopTask);
            SemaphoreP_post(&gSafetyApp_BinarySemTaskSync);
        }

        /* Semaphore pend to see if safety loop has finished execution */
        SemaphoreP_pend(&gSafetyApp_BinarySemTaskSync, SystemP_WAIT_FOREVER);
        /* Semaphore destruct once safety loop execution has been completed */
        SemaphoreP_destruct(&gSafetyApp_BinarySemTaskSync);
    }
}

static void SafetyApp_safetyLoop(void *args)
{
    uint32_t  status = SystemP_SUCCESS, numIter = SAFETY_APP_LOOP_COUNT;
    ClockP_Object clockObj;
    ClockP_Params clockParams;
    SemaphoreP_Object binarySemFromCbFxn;

    status = SemaphoreP_constructBinary(&binarySemFromCbFxn, 0);
    if(status != SystemP_SUCCESS)
    {
        DebugP_log("Binary semaphore object creation failed!!");
        status = SystemP_FAILURE;
    }

    /* Creating timer for safety loop thread timeout */
    if(status == SystemP_SUCCESS)
    {
        ClockP_Params_init(&clockParams);
        clockParams.timeout = 5000;
        clockParams.period = 5000;
        clockParams.start = 1;
        clockParams.callback = SafetyApp_clockCbFxn;
        clockParams.args = &binarySemFromCbFxn;

        status = ClockP_construct(&clockObj, &clockParams);
        if(status != SystemP_SUCCESS)
        {
            DebugP_log("Clock object creation failed!!");
            status = SystemP_FAILURE;
        }

        if(status == SystemP_SUCCESS)
        {
            DebugP_log("Verifying registers against golden reference...\r\n");

            while(numIter > 0)
            {
                status = SemaphoreP_pend(&binarySemFromCbFxn, SystemP_WAIT_FOREVER);

                if(status == SystemP_SUCCESS)
                {
                    /* Verify PM Checkers Register Configuration */
                    status = SafetyCheckers_pmVerifyPscRegCfg(gSafetyApp_pscRegisterData, SAFETY_CHECKERS_PM_PSC_REGDUMP_SIZE);
                    if (status == SAFETY_CHECKERS_REG_DATA_MISMATCH)
                    {
                        DebugP_log("PSC register mismatch with golden reference.\r\n");
                        break;
                    }

                    status = SafetyCheckers_pmGetPllRegCfg(gSafetyApp_pllRegisterData, SAFETY_CHECKERS_PM_PLL_REGDUMP_SIZE);
                    if (status == SAFETY_CHECKERS_REG_DATA_MISMATCH)
                    {
                        DebugP_log("PLL register mismatch with golden reference.\r\n");
                        break;
                    }

                    status = SafetyCheckers_pmRegisterLock();
                    if (status == SAFETY_CHECKERS_FAIL)
                    {
                        DebugP_log("PM register lock failed.\r\n");
                        break;
                    }

                    /* Verify RM Checkers Register Configuration */
                    status = SafetyCheckers_rmVerifyRegCfg(gSafetyApp_rmRegisterData, SAFETY_APP_RM_CHECKERS_REG_BLOB_SIZE);
                    if (status == SAFETY_CHECKERS_REG_DATA_MISMATCH)
                    {
                        DebugP_log("RM register mismatch with golden reference.\r\n");
                        break;
                    }

                    /* Verify TIFS Checkers Register Configuration */
                    status = SafetyCheckers_tifsVerifyFwlCfg(gSafetyApp_pFwlConfig, gSafetyApp_tifsFwlCfgSize);
                    if (status == SAFETY_CHECKERS_REG_DATA_MISMATCH)
                    {
                        DebugP_log("Firewall register mismatch with golden reference.\r\n");
                        break;
                    }

                    numIter--;
                }
            }

            if (status == SAFETY_CHECKERS_SOK)
            {
                DebugP_log("No register mismatch with golden reference.\r\n");
            }
            else
            {
                DebugP_log("ERROR: Safety Checker Testing Failed.\r\n");
            }
        }
    }

    /* Request TIFS firewall close */
    status = SafetyCheckers_tifsReqFwlClose();
    if (status == SystemP_SUCCESS)
    {
        DebugP_log("Firewall close successful.\r\n");
    }
    else
    {
        DebugP_log("Firewall close unsuccessful.\r\n");
    }

    ClockP_destruct(&clockObj);
    SemaphoreP_destruct(&binarySemFromCbFxn);
    SemaphoreP_post(&gSafetyApp_BinarySemTaskSync);
    TaskP_exit();
}

static void SafetyApp_clockCbFxn(ClockP_Object *obj, void *args)
{
    if (args != NULL)
    {
        SemaphoreP_Object *pSemObj = (SemaphoreP_Object *)args;
        SemaphoreP_post(pSemObj);
    }
}

