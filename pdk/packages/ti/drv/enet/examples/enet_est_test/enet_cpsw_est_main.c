/*
 *  Copyright (c) Texas Instruments Incorporated 2023
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

/*!
 * \file  main.c
 *
 * \brief Main file for EST example application.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "enet_cpsw_est_common.h"
#include "enet_cpsw_est_cfg.h"
#include "enet_cpsw_est_ts.h"
#include "enet_cpsw_est_dataflow.h"

#include <stdint.h>
#include <stdarg.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/osal/ClockP.h>

#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/udma/udma.h>
#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils_rtos.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_mcm.h>
#include <ti/drv/enet/examples/utils/include/enet_apprm.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_board.h>

#include <ti/drv/enet/include/core/enet_per.h>
#include <ti/drv/enet/include/core/enet_soc.h>
#include <ti/drv/enet/include/per/cpsw.h>
#include <ti/drv/enet/include/mod/cpsw_ale.h>
#include <ti/drv/enet/include/mod/cpsw_macport.h>
#include<ti/drv/enet/include/core/enet_mod_tas.h>

/* OSAL Header files */
#include <ti/osal/osal.h>
#include <ti/osal/LoadP.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined(SAFERTOS)
#define ENETAPP_TSK_STACK_ESTAPPINIT         (16U * 1024U)
#define ENETAPP_TSK_STACK_ESTAPPINIT_ALIGN   (ENETAPP_TSK_STACK_ESTAPPINIT)
#else
#define ENETAPP_TSK_STACK_ESTAPPINIT         (8U * 1024U)
#define ENETAPP_TSK_STACK_ESTAPPINIT_ALIGN   (32U)
#endif

/* Configuration via menu or statically */
#define APP_ENABLE_STATIC_CFG                (0U)

/* 100-ms periodic tick */
#define ENETAPP_PERIODIC_TICK_MS             (100U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
static void EnetApp_showMenu(void);

void EnetApp_getEstTestConfig(Enet_Type *enetType,
                              uint32_t *instId,
                              EnetBoard_EthPort *ethPorts,
                              uint32_t *macPortNum);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static uint8_t gEnetEstTaskStackMain[ENETAPP_TSK_STACK_ESTAPPINIT] __attribute__((aligned(ENETAPP_TSK_STACK_ESTAPPINIT_ALIGN)));

/* Use this array to select the ports that will be used in the test */
static EnetApp_TestParams testParams =
{
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
    .portTestParams =
    {
        {
            .macPort = ENET_MAC_PORT_1,
            .tasControlList =
            {
                .baseTime    = 0ULL, /* will be updated later */
                .cycleTime   = 381064ULL,
                .gateCmdList =
                {
                    { .gateStateMask = ENET_TAS_GATE_MASK(1, 1, 0, 0, 0, 0, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 1, 1, 0, 0, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 0, 0, 1, 1, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 0, 0, 0, 0, 1, 1), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 0, 0, 0, 0, 0, 0), .timeInterval = 131064U }, /* guard band */
                },
                .listLength = 5U,
            },
        },
    },
    .macPortNum = 1U,
#elif defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
    .portTestParams =
    {
        {
            .macPort = ENET_MAC_PORT_1,
            .tasControlList =
            {
                .baseTime    = 0ULL, /* will be updated later */
                .cycleTime   = 381064ULL, //381064ULL,
                .gateCmdList =
                {
                    { .gateStateMask = ENET_TAS_GATE_MASK(1, 1, 0, 0, 0, 0, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 1, 1, 0, 0, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 0, 0, 1, 1, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 0, 0, 0, 0, 1, 1), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 0, 0, 0, 0, 0, 0), .timeInterval = 131064U }, /* guard band */
                },
                .listLength = 5U,
            },
        },
        {
            .macPort     = ENET_MAC_PORT_2,
            .tasControlList =
            {
                .baseTime    = 0ULL, /* will be updated later */
                .cycleTime   = 250000ULL,
                .gateCmdList =
                {
                    { .gateStateMask = ENET_TAS_GATE_MASK(1, 1, 0, 0, 0, 0, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 1, 1, 0, 0, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 0, 0, 1, 1, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 0, 0, 0, 0, 1, 1), .timeInterval = 62500U },
                },
                .listLength = 4U,
            },
        },
        {
            .macPort = ENET_MAC_PORT_3,
            .tasControlList =
            {
                .baseTime    = 0ULL, /* will be updated later */
                .cycleTime   = 250000ULL,
                .gateCmdList =
                {
                    { .gateStateMask = ENET_TAS_GATE_MASK(1, 1, 0, 0, 0, 0, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 1, 1, 0, 0, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 0, 0, 1, 1, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 0, 0, 0, 0, 1, 1), .timeInterval = 62500U },
                },
                .listLength = 4U,
            },
        },
        {
            .macPort = ENET_MAC_PORT_4,
            .tasControlList =
            {
                .baseTime    = 0ULL, /* will be updated later */
                .cycleTime   = 250000ULL,
                .gateCmdList =
                {
                    { .gateStateMask = ENET_TAS_GATE_MASK(1, 1, 0, 0, 0, 0, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 1, 1, 0, 0, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 0, 0, 1, 1, 0, 0), .timeInterval = 62500U },
                    { .gateStateMask = ENET_TAS_GATE_MASK(0, 0, 0, 0, 0, 0, 1, 1), .timeInterval = 62500U },
                },
                .listLength = 4U,
            },
        },
    },
    .macPortNum = 4U,
#endif
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    TaskP_Handle hTask;
    TaskP_Params params;

    OS_init();

    /* Initialize the main task params */
    TaskP_Params_init(&params);
    params.priority       = 1U;
    params.stack          = gEnetEstTaskStackMain;
    params.stacksize      = sizeof(gEnetEstTaskStackMain);
    params.name           = (const char *)"EST app main task";

    hTask = TaskP_create(&EnetApp_mainTask, &params);
    if (hTask == NULL)
    {
        EnetAppUtils_print("main() failed to create main task\n");
        OS_stop();
    }

    /* Does not return */
    OS_start();

    return 0;
}

void EnetApp_mainTask(void *args1, void *args2)
{
    char option;
    uint64_t tsVal;
    uint32_t i;
    int32_t status = ENET_SOK;

    EnetBoard_init();

    EnetAppUtils_print("==========================\n");
    EnetAppUtils_print("      Enet EST App       \n");
    EnetAppUtils_print("==========================\n");

    memset(&gEnetApp, 0, sizeof(gEnetApp));

    EnetApp_getEstTestConfig(&gEnetApp.enetType,
                             &gEnetApp.instId,
                             &gEnetApp.ethPorts[0U],
                             &gEnetApp.macPortNum);

    /* Initialize test config params */
    gEnetApp.exitFlag = BFALSE;
    gEnetApp.run = BTRUE;
    gEnetApp.enableTs = BFALSE;
    gEnetApp.macPort = gEnetApp.ethPorts[0].macPort;

    EnetApp_createClock();

    for (i = 0U; i < gEnetApp.macPortNum; i++)
    {
        memcpy(&gEnetApp.tasControlList[i],
               &testParams.portTestParams[i].tasControlList,
               sizeof(EnetTas_ControlList));
    }

    gEnetApp.coreId = EnetSoc_getCoreId();;

    /* Open CPSW peripheral */
    status = EnetApp_open();
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to open peripherals: %d\r\n", status);
    }

    if (status == ENET_SOK)
    {
        /* Wait for user input to exit the test */
        EnetApp_showMenu();
        while (BTRUE)
        {
            option = EnetAppUtils_getChar();
            if (option == 'x')
            {
                EnetAppUtils_print("Stopping...\r\n");
                gEnetApp.run = BFALSE;
                break;
            }
            else if (option == 'c')
            {
                tsVal = EnetApp_getCurrentTime();
                EnetAppUtils_print("Current time is: %llu\r\n", tsVal);
            }
            else if (option == 't')
            {
                gEnetApp.enableTs = !gEnetApp.enableTs;
                EnetAppUtils_print("\n%s timestamp printing\r\n", (gEnetApp.enableTs ? "Enable" : "Disable"));
                for (i = 0U; i < gEnetApp.macPortNum; i++)
                {
                    if (gEnetApp.macPort == gEnetApp.ethPorts[i].macPort)
                    {
                        if (gEnetApp.enableTs)
                        {
                            EnetApp_enableTimestamp(gEnetApp.ethPorts[i].macPort);
                        }
                        else
                        {
                            EnetApp_disableTimestamp(gEnetApp.ethPorts[i].macPort);
                        }
                    }
                }
            }
            else if (option == 'S')
            {
                for (i = 0U; i < gEnetApp.macPortNum; i++)
                {
                    if (gEnetApp.macPort == gEnetApp.ethPorts[i].macPort)
                    {
                        EnetApp_printEstStatus(gEnetApp.ethPorts[i].macPort);
                    }
                }
            }
            else if (option == 's')
            {
                EnetApp_printStats();
            }
            else if (option == 'r')
            {
                EnetApp_resetStats();
            }
            else if (option == 'E')
            {
                for (i = 0U; i < gEnetApp.macPortNum; i++)
                {
                    EnetApp_setEstState(gEnetApp.ethPorts[i].macPort, ENET_TAS_ENABLE);
                }
            }
            else if (option == 'D')
            {
                gEnetApp.usingDfltSched = BFALSE;

                for (i = 0U; i < gEnetApp.macPortNum; i++)
                {
                    EnetApp_setEstState(gEnetApp.ethPorts[i].macPort, ENET_TAS_DISABLE);
                }
            }
            else if (option == 'R')
            {
                gEnetApp.usingDfltSched = BFALSE;

                for (i = 0U; i < gEnetApp.macPortNum; i++)
                {
                    EnetApp_setEstState(gEnetApp.ethPorts[i].macPort, ENET_TAS_RESET);
                }
            }
            else if (option == 'T')
            {
                EnetApp_txTest();
                if (gEnetApp.enableTs)
                {
                    //EnetAppUtils_delayInUsec(1000000);
                    EnetApp_checkEstTimestamps();
                }
            }
            else if ((option == 'h') || (option == 'H'))
            {
                EnetApp_showMenu();
            }
            else
            {
                EnetAppUtils_print("Invalid option, try again...\r\n");
                EnetApp_showMenu();
            }

            TaskP_yield();
        }

        /* Print statistics */
        EnetApp_printStats();

        /* Wait until RX task has exited */
        EnetAppUtils_print("Waiting for tasks to exit\r\n");
        SemaphoreP_post(&gEnetApp.hRxSem);
        SemaphoreP_pend(&gEnetApp.hRxDoneSem, SemaphoreP_WAIT_FOREVER);

        EnetAppUtils_print("All tasks have exited\r\n");

    }

    /* Close peripheral */
    EnetApp_close();

    EnetApp_deleteClock();

    EnetBoard_deinit();
}

static void EnetApp_showMenu(void)
{
    EnetAppUtils_print("\r\nCPSW EST Test Menu:\r\n");
    EnetAppUtils_print(" EST control list tests:\r\n");
    EnetAppUtils_print(" 'T'  -  Send test packets from host port\r\n");

    EnetAppUtils_print(" EST state:\r\n");
    EnetAppUtils_print(" 'E'  -  Set EST state to 'ENABLE'\r\n");
    EnetAppUtils_print(" 'D'  -  Set EST state to 'DISABLE'\r\n");
    EnetAppUtils_print(" 'R'  -  Set EST state to 'RESET'\r\n");

    EnetAppUtils_print(" Others:\r\n");
    EnetAppUtils_print(" 'c'  -  Get current time\r\n");
    EnetAppUtils_print(" 't'  -  Toggle printing timestamps\r\n");
    EnetAppUtils_print(" 'S'  -  Print EST status\r\n");
    EnetAppUtils_print(" 's'  -  Print statistics\r\n");
    EnetAppUtils_print(" 'r'  -  Reset statistics\r\n");
    EnetAppUtils_print(" 'x'  -  Stop the test\r\n");
    EnetAppUtils_print(" 'h'  -  Show this menu\r\n");

    EnetAppUtils_print("\r\n");
}
