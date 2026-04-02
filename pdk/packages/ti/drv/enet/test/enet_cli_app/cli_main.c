/*
 * Copyright (C) 2024 Texas Instruments Incorporated
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *
 *   Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
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
 */

/*!
 * \file  cli_main.c
 *
 * \brief This file contains the main task of the Enet CLI example.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "cli_common.h"
#include "l2_networking.h"
#include "ale_unicast.h"
#include "ale_vlan.h"
#include "cli_lwip.h"
#include <ti/drv/enet/enet_cli/enet_cli_core/include/enet_cli.h>
#include <enet_board.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Task stack size */
#define ENETCli_TASK_STACK_SZ                     (10U * 1024U)
#define ENETCli_TASK_STACK_ALIGN                  (32U)

/* 100-ms periodic tick */
#define ENETLPBK_PERIODIC_TICK_MS                  (100U)
/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void EnetApp_mainTask(void* a0, void* a1);

static void EnetApp_createClock(void);

static void EnetApp_deleteClock(void);

static void EnetApp_tickTask(void* a0, void* a1);

static void EnetApp_timerCallback(void* arg);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
static uint8_t gEnetCliTaskStackMain[ENETCli_TASK_STACK_SZ] __attribute__((aligned(ENETCli_TASK_STACK_ALIGN)));
static uint8_t gEnetCliTaskStackTick[ENETCli_TASK_STACK_SZ] __attribute__((aligned( ENETCli_TASK_STACK_ALIGN)));

CLI_Command_Definition_t commandList[] =
        { { .pcCommand = "enet_addtxchn",
                .pcHelpString =
                        "enet_addtxchn <tx_ch_num>:\r\n Opens a Tx channel.\r\n\n",
                .pxCommandInterpreter = EnetCLI_openTxChn,
                .cExpectedNumberOfParameters = 1 },
            { .pcCommand = "enet_addrxchn",
                .pcHelpString =
                        "enet_addrxchn <rx_ch_num>:\r\n Opens an Rx channel.\r\n\n",
                .pxCommandInterpreter = EnetCLI_openRxChn,
                .cExpectedNumberOfParameters = 1 },
            { .pcCommand = "enet_sendraw",
                .pcHelpString =
                        "enet_sendraw <tx_ch_num> [-dm <dest_mac_addr>] [-sm <src_mac_addr>] [-v <vlan_id>] [-pcp <priority>] [-m <message>]:\r\n Sends a raw ethernet packet.\r\n\n",
                .pxCommandInterpreter = EnetCLI_transmitPkt,
                .cExpectedNumberOfParameters = -1 },
            { .pcCommand = "enet_capture",
                .pcHelpString =
                        "enet_capture {start | stop} <rx_ch_num>:\r\n Start/stop capturing incoming ethernet packets.\r\n\n",
                .pxCommandInterpreter = EnetCLI_capturePkt,
                .cExpectedNumberOfParameters = 2 },
            { .pcCommand = "enet_capturedump",
                .pcHelpString =
                        "enet_capturedump <rx_ch_num>:\r\n Returns the last 4 packets recieved at the specified channel.\r\n\n",
                .pxCommandInterpreter = EnetCLI_dumpRxBuffer,
                .cExpectedNumberOfParameters = 1 }, { .pcCommand = "quit",
                .pcHelpString = "quit:\r\n Closes the CLI application.\r\n\n",
                .pxCommandInterpreter = EnetCLI_quitTerminal,
                .cExpectedNumberOfParameters = 0 },
            { .pcCommand = "enet_adducast",
                .pcHelpString =
                        "enet_adducast <mac_addr> [-d]:\r\n Adds a unicast ALE entry with the given MAC address.\r\n Use the -d tag to make the MAC address as the default source address for sending packets.\r\n\n",
                .pxCommandInterpreter = EnetCLI_addUcast,
                .cExpectedNumberOfParameters = -1 },
            { .pcCommand = "enet_remucast",
                .pcHelpString =
                        "enet_remucast <mac_addr>:\r\n Removes unicast ALE entry with the given MAC address.\r\n\n",
                .pxCommandInterpreter = EnetCLI_removeUcast,
                .cExpectedNumberOfParameters = 1 },
            { .pcCommand = "enet_addvlan",
                .pcHelpString =
                        "enet_addvlan <vlan_id> {<port1> ...}:\r\n Configures the given ports to the specified VLAN id.\r\n\n",
                .pxCommandInterpreter = EnetCLI_addVlan,
                .cExpectedNumberOfParameters = -1 },
            { .pcCommand = "enet_remvlan",
                .pcHelpString =
                        "enet_remvlan <vlan_id>:\r\n Removes VLAN config with specified ID.\r\n\n",
                .pxCommandInterpreter = EnetCLI_removeVlan,
                .cExpectedNumberOfParameters = 1 },
            { .pcCommand = "enet_hostmacaddr",
                .pcHelpString =
                        "enet_hostmacaddr:\r\n Prints the host port MAC address. \r\n\n",
                .pxCommandInterpreter = EnetCLI_getHostMac,
                .cExpectedNumberOfParameters = 0 },
            { .pcCommand = "lwip_shell", .pcHelpString =
                    "lwip_shell {start | stop}:\r\n Opens an LwIP shell.\r\n\n",
                .pxCommandInterpreter = EnetCLI_lwipShell,
                .cExpectedNumberOfParameters = 1 } 
                };

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{

    TaskP_Handle hTask;
    TaskP_Params params;
    
    OS_init();
    
    /* Initialize loopback test config */
    memset(&EnetApp_inst, 0, sizeof(EnetApp_inst));
    EnetApp_inst.exitFlag = BFALSE;

     /* Initialize the main task params */
    TaskP_Params_init(&params);
    params.priority       = 2U;
    params.stack          = gEnetCliTaskStackMain;
    params.stacksize      = sizeof(gEnetCliTaskStackMain);
    params.name           = (const char *)"ENET CLI App main task";

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

/* ========================================================================== */
/*                   Static Function Definitions                              */
/* ========================================================================== */

static void EnetApp_mainTask(void* a0, void* a1)
{
     EnetBoard_init();

    EnetAppUtils_print("==========================\n");
    EnetAppUtils_print("    Enet CLI App     \n");
    EnetAppUtils_print("==========================\n");

    EnetApp_createClock();

    EnetUtils_Cfg utilsCfg;
    SemaphoreP_Params params;

    EnetApp_inst.enetType = ENET_CPSW_9G;
    EnetApp_inst.instId = 0U;

    EnetAppUtils_enableClocks(EnetApp_inst.enetType, EnetApp_inst.instId);

    /* Create TX/RX semaphores */
    SemaphoreP_Params_init(&params);
    params.mode = SemaphoreP_Mode_BINARY;

    EnetApp_inst.rxSemObj[0] = SemaphoreP_create(0, &params);
    EnetAppUtils_assert(EnetApp_inst.rxSemObj[0] != NULL);

    EnetApp_inst.txSemObj[0] = SemaphoreP_create(0, &params);
    EnetAppUtils_assert(EnetApp_inst.txSemObj[0] != NULL);

    EnetApp_inst.hTxDoneSem = SemaphoreP_create(0, &params);
    EnetAppUtils_assert(EnetApp_inst.hTxDoneSem != NULL);

    EnetApp_inst.hRxDoneSem = SemaphoreP_create(0, &params);
    EnetAppUtils_assert(EnetApp_inst.hRxDoneSem != NULL);

    /* Local core id */
    EnetApp_inst.coreId = EnetSoc_getCoreId();

    /* Initialize Enet driver (use default utils) */
    Enet_initUtilsCfg(&utilsCfg);
    Enet_init(&utilsCfg);

    /* Add commands provided by enet_cli library */
    EnetCli_init(EnetApp_inst.enetType, EnetApp_inst.instId);

    /* Register custom commands */
    EnetCli_registerCustomCommands(commandList, sizeof(commandList)/sizeof(commandList[0]));

    /* Register commands that are built in in enet_cli lib */
    EnetCli_registerBuiltInCommands();

    EnetBoard_deinit();

    EnetApp_deleteClock();

}


static void EnetApp_createClock(void)
{
    TaskP_Params taskParams;
    SemaphoreP_Params semParams;
    ClockP_Params clkParams;

    /* Initialize timer semaphore params */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_COUNTING;

    /* Create timer semaphore */
    EnetApp_inst.hTimerSem = SemaphoreP_create(0, &semParams);

    /* Reset the exitFlag */
    EnetApp_inst.exitFlag = BFALSE;

    /* Initialize the periodic tick task params */
    TaskP_Params_init(&taskParams);
    taskParams.priority       = 7U;
    taskParams.stack          = gEnetCliTaskStackTick;
    taskParams.stacksize      = sizeof(gEnetCliTaskStackTick);
    taskParams.arg0           = (void*)EnetApp_inst.hTimerSem;
    taskParams.name           = (const char *)"Periodic tick task";

    /* Create periodic tick task */
    EnetApp_inst.hTickTask = TaskP_create(&EnetApp_tickTask, &taskParams);
    if (EnetApp_inst.hTickTask == NULL)
    {
        EnetAppUtils_print("EnetLpbk_createClock() failed to create tick task\n");
        OS_stop();
    }

    ClockP_Params_init(&clkParams);
    clkParams.startMode = ClockP_StartMode_USER;
    clkParams.period    = ENETLPBK_PERIODIC_TICK_MS;
    clkParams.runMode   = ClockP_RunMode_CONTINUOUS;
    clkParams.arg       = (void*)EnetApp_inst.hTimerSem;

    /* Creating timer and setting timer callback function*/
    EnetApp_inst.hTickTimer = ClockP_create(EnetApp_timerCallback, &clkParams);
    if (EnetApp_inst.hTickTimer == NULL)
    {
        EnetAppUtils_print("EnetLpbk_createClock() failed to create clock\n");
        OS_stop();
    }
}

static void EnetApp_deleteClock(void)
{
    EnetApp_inst.exitFlag = BTRUE;

    /* Delete periodic tick timer */
    if (EnetApp_inst.hTickTimer != NULL)
    {
        ClockP_delete(EnetApp_inst.hTickTimer);
        EnetApp_inst.hTickTimer = NULL;
    }

    /* Delete periodic tick task */
    if (EnetApp_inst.hTickTask != NULL)
    {
#if !defined (FREERTOS)
        TaskP_delete(&EnetApp_inst.hTickTask);
#endif
        EnetApp_inst.hTickTask = NULL;
    }

    /* Delete periodic tick timer */
    if (EnetApp_inst.hTimerSem != NULL)
    {
        SemaphoreP_delete(EnetApp_inst.hTimerSem);
        EnetApp_inst.hTimerSem = NULL;
    }
}

static void EnetApp_tickTask(void* a0, void* a1)
{
    SemaphoreP_Handle hSem = (SemaphoreP_Handle)a0;

    while (!EnetApp_inst.exitFlag)
    {
        SemaphoreP_pend(hSem, SemaphoreP_WAIT_FOREVER);

        /* PeriodicTick should be called from non-ISR context */
        Enet_periodicTick(EnetApp_inst.hEnet);
    }
    EnetAppUtils_print("EnetLpbk_tickTask() exiting..\n");
}

static void EnetApp_timerCallback(void* arg)
{
    SemaphoreP_Handle hSem = (SemaphoreP_Handle)arg;

    /* Tick! */
    SemaphoreP_post(hSem);
}

/* None */
