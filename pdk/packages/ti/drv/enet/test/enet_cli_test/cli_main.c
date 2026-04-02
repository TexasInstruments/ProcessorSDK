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
#include "rate_limit.h"
#include "test_phy.h"
#include "test_ale.h"
#include <ti/drv/enet/enet_cli/enet_cli_core/include/enet_cli.h>
#include <enet_board.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* This sets the default task priority */
#define DEFAULT_PRIORITY            2U

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* Holds the logic for Enet CLI */
static void EnetApp_mainTask(void *a0, void *a1);

/* Open the enet driver */
static int32_t EnetApp_openEnet(void);

/* Initializes CPSW */
static void EnetApp_initCpswCfg(Cpsw_Cfg *cpswCfg);

/* Sets up CPSW ALE */
static int32_t EnetApp_setupCpswAle(void);

/* Close the enet driver */
static void EnetApp_closeEnet(void);

/* Show which all phys are alive */
static int32_t EnetApp_showAlivePhys(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Test application stack */
static uint8_t gEnetCliTaskStackMain[ENETCli_TASK_STACK_SZ] __attribute__((aligned(ENETCli_TASK_STACK_ALIGN)));

/* A total of 8 ports may be used*/
const uint8_t allowedMacPortNums[8] = {0,1,2,3,4,5,6,7};

/* Policer table partition into chunks of different size each having different
 * priority CPSW_ALE_POLICER_PARTITION_DEFAULT having lowest and
 * CPSW_ALE_POLICER_PARTITION_LEVEL_1 having the highest priority */
static uint32_t gEthFw_policerTablePartSize[CPSW_ALE_POLICER_TABLE_PART_MAX] =
{
    /* This partition is used for private VLAN by default */
    [CPSW_ALE_POLICER_PARTITION_LEVEL_1] = 10U,
    /* This partition is used for custom policers by default.
     * These are policers which are created by ethfw application for each virtual port
     * on specific rx flows (extended).
     * Custom policer configuration can be changed from ethfw application */
    [CPSW_ALE_POLICER_PARTITION_LEVEL_2] = 10U,
    [CPSW_ALE_POLICER_PARTITION_LEVEL_3] = 10U,
    [CPSW_ALE_POLICER_PARTITION_LEVEL_4] = 10U,
    /* Give the unused/default partition size to be 0
     * Partitions with size 0 are clubbed to default partition */
    [CPSW_ALE_POLICER_PARTITION_DEFAULT] = 0U
};

/* This value should be between 0 and 7 inclusive.
 * Represents the number of macports that
 * are enabled at any moment.*/
uint8_t num_of_ports = 8;

CLI_Command_Definition_t commandList[] =
    {{.pcCommand = "enet_addtxchn",
      .pcHelpString =
          "enet_addtxchn <tx_ch_num>:\r\n Opens a Tx channel.\r\n\n",
      .pxCommandInterpreter = EnetCLI_openTxChn,
      .cExpectedNumberOfParameters = 1},
     {.pcCommand = "enet_addrxchn",
      .pcHelpString =
          "enet_addrxchn <rx_ch_num>:\r\n Opens an Rx channel.\r\n\n",
      .pxCommandInterpreter = EnetCLI_openRxChn,
      .cExpectedNumberOfParameters = 1},
     {.pcCommand = "enet_capture",
      .pcHelpString =
          "enet_capture {start | stop} <rx_ch_num>:\r\n Start/stop capturing incoming ethernet packets.\r\n\n",
      .pxCommandInterpreter = EnetCLI_capturePkt,
      .cExpectedNumberOfParameters = 2},
     {.pcCommand = "enet_printphyregs",
      .pcHelpString =
          "enet_printphyregs <port number - Only port numbers 1 and 3 are supported>\r\n\n",
      .pxCommandInterpreter = Enetcli_printPhyRegs,
      .cExpectedNumberOfParameters = 1},
     {.pcCommand = "enet_capturedump",
      .pcHelpString =
          "enet_capturedump <rx_ch_num>:\r\n Returns the last 4 packets recieved at the specified channel.\r\n\n",
      .pxCommandInterpreter = EnetCLI_dumpRxBuffer,
      .cExpectedNumberOfParameters = 1},
     {.pcCommand = "enet_phymode",
      .pcHelpString =
          "enet_phymode -p <port number> -d <duplexity> -s <speed> [-r]:\r\n Updates the phy mode of a particular "
          "port. \r\nSpeed can be one among 10m, 100m, 1g, 10g or auto\r\n"
          "Duplexity can be one among half, full or auto.\r\n"
          "Port can either be 1 or 3.\r\n"
          "Providing -r will reset the phymode of port to 1g speed and full duplexity\r\n\n",
      .pxCommandInterpreter = Enetcli_phyMode,
      .cExpectedNumberOfParameters = -1},
      {.pcCommand = "enet_hostmacaddr",
      .pcHelpString =
          "enet_hostmacaddr:\r\n Prints the host port MAC address. \r\n\n",
      .pxCommandInterpreter = EnetCLI_getHostMac,
      .cExpectedNumberOfParameters = 0},
      {.pcCommand = "enet_ratelmt",
      .pcHelpString = "enet_ratelmt -d <0 for ingress and 1 for egress port> -p <port number - "
                      "Only port numbers 1 and 3 are supported> -r <rate in terms of number of "
                      "packets per second>:\r\n Set the rate limit in terms of packets per sec "
                      "at any port for Multicast and Broadcast.\r\n\n",
      .pxCommandInterpreter = Enetcli_ratelmt,
      .cExpectedNumberOfParameters = -1},
      {.pcCommand = "enet_adducast",
      .pcHelpString = 
        "enet_adducast <mac_addr> [-d] [-b] [-a]:\r\n Adds a unicast ALE entry with the given MAC address.\r\n "
        "Use the -d tag to make the MAC address as the default source address for sending packets.\r\n "
        "Use the -b tag to make this entry a blocking entry. \r\n "
        "Use the -a tag to make this entry an ageable entry. \r\n\n",
        .pxCommandInterpreter = EnetCLI_addUcast,
        .cExpectedNumberOfParameters = -1 },
      { .pcCommand = "enet_remucast",
      .pcHelpString =
                "enet_remucast <mac_addr>:\r\n Removes unicast ALE entry with the given MAC address.\r\n\n",
        .pxCommandInterpreter = EnetCLI_removeUcast,
        .cExpectedNumberOfParameters = 1 },
      {.pcCommand = "enet_addmcast",
      .pcHelpString = 
        "enet_addmcast <mac_addr>:\r\n Adds a multicast ALE entry with the given MAC address. \r\n\n",
        .pxCommandInterpreter = EnetCLI_addMcast,
        .cExpectedNumberOfParameters = -1 },
      { .pcCommand = "enet_remmcast",
      .pcHelpString =
                "enet_remmcast <mac_addr>:\r\n Removes multicast ALE entry with the given MAC address.\r\n\n",
        .pxCommandInterpreter = EnetCLI_removeMcast,
        .cExpectedNumberOfParameters = 1 },
      {.pcCommand = "enet_addoui",
      .pcHelpString = 
        "enet_addoui <mac_addr> :\r\n Adds an OUI ALE entry corresponding to the given MAC address.\r\n\n",
        .pxCommandInterpreter = EnetCLI_addOui,
        .cExpectedNumberOfParameters = 1 },
      { .pcCommand = "enet_remoui",
      .pcHelpString =
                "enet_remoui <mac_addr>:\r\n Removes OUI entry corresponding to this MAC address.\r\n\n",
        .pxCommandInterpreter = EnetCLI_removeOui,
        .cExpectedNumberOfParameters = 1},
      {.pcCommand = "enet_addipv4",
      .pcHelpString = 
        "enet_addipv4 <IPv4_addr in the form A.B.C.D>:\r\n Adds an IPv4 ALE entry with the given IP address. \r\n\n",
        .pxCommandInterpreter = EnetCLI_addipv4,
        .cExpectedNumberOfParameters = -1 },
      { .pcCommand = "enet_remipv4",
      .pcHelpString =
                "enet_remipv4 <IPv4_addr>:\r\n Removes IPv4 ALE entry with the given IP address.\r\n\n",
        .pxCommandInterpreter = EnetCLI_removeipv4,
        .cExpectedNumberOfParameters = 1 },
      {.pcCommand = "enet_changeportstate",
      .pcHelpString = 
        "enet_changeportstate -p <mac_port> <port state - 'forward', 'block', 'disable' or 'learn'>:\r\n "
        "Changes the port state of the corresponding macport\r\n\n",
        .pxCommandInterpreter = EnetCLI_changePortState,
        .cExpectedNumberOfParameters = -1}};

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
    params.priority = DEFAULT_PRIORITY;
    params.stack = gEnetCliTaskStackMain;
    params.stacksize = sizeof(gEnetCliTaskStackMain);
    params.name = (const char *)"ENET CLI App main task";

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

static void EnetApp_mainTask(void *a0, void *a1)
{
    EnetBoard_init();

    EnetAppUtils_print("==========================\n");
    EnetAppUtils_print("    Enet CLI App     \n");
    EnetAppUtils_print("==========================\n");

    EnetApp_getTestConfig(&EnetApp_inst.enetType,
                          &EnetApp_inst.instId,
                          EnetApp_inst.macPort,
                          EnetApp_inst.macMode,
                          EnetApp_inst.boardId,
                          EnetApp_inst.expPort,
                          &EnetApp_inst.numMacPorts,
                          num_of_ports);

    EnetApp_createClock();

    EnetUtils_Cfg utilsCfg;
    Enet_IoctlPrms prms;
    SemaphoreP_Params params;
    int32_t status;

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

    /* Open Enet driver */
    status = EnetApp_openEnet();

    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to open Enet driver: %d\n", status);
    }

    if (status == ENET_SOK)
    {
        /* Attach the core with RM */
        uint32_t coreId;
        EnetPer_AttachCoreOutArgs attachCoreOutArgs;
        coreId = EnetApp_inst.coreId;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &coreId, &attachCoreOutArgs);
        status = Enet_ioctl(EnetApp_inst.hEnet, 
                            EnetApp_inst.coreId, 
                            ENET_PER_IOCTL_ATTACH_CORE, &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("EnetApp_loopbackTest failed ENET_PER_IOCTL_ATTACH_CORE: %d\n", 
                                status);
        }
        else
        {
            EnetApp_inst.coreKey = attachCoreOutArgs.coreKey;
        }
    }

    if (status == ENET_SOK)
    {
        /* memutils open should happen after Cpsw is opened as it uses CpswUtils_Q
         * functions */
        status = EnetMem_init();
        EnetAppUtils_assert(ENET_SOK == status);
    }

    /* Open DMA driver */
    if (status == ENET_SOK)
    {
        status = EnetApp_openDma();
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to open DMA: %d\n", status);
        }
    }

    /* Enable host port */
    if (status == ENET_SOK)
    {
        status = EnetApp_setupCpswAle();
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to setup CPSW ALE: %d\n", status);
        }

        if (status == ENET_SOK)
        {
            ENET_IOCTL_SET_NO_ARGS(&prms);
            status = Enet_ioctl(EnetApp_inst.hEnet, 
                                EnetApp_inst.coreId, 
                                ENET_HOSTPORT_IOCTL_ENABLE, 
                                &prms);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("Failed to enable host port: %d\n", status);
            }
        }
    }

    /* Show alive PHYs */
    if (status == ENET_SOK)
    {
        status = EnetApp_showAlivePhys();
    }

    /* Start timer */
    ClockP_start(EnetApp_inst.hTickTimer);

    if (status == ENET_SOK)
    {
        EnetApp_inst.enetType = ENET_CPSW_9G;
        EnetApp_inst.instId = 0U;

        /* Add commands provided by enet_cli library */
        EnetCli_init(EnetApp_inst.enetType, EnetApp_inst.instId);

        /* Register custom commands */
        EnetCli_registerCustomCommands(commandList, 
                                       sizeof(commandList) / sizeof(commandList[0]));

        /* Register commands that are built in in enet_cli lib */
        EnetCli_registerBuiltInCommands();

        bool xMoreDataToFollow;
        bool inTerminal = BTRUE;

        /* Create read and write buffer */
        char txBuffer[MAX_WRITE_BUFFER_LEN] = "";
        char rxBuffer[MAX_READ_BUFFER_LEN] = "";

        /* Start of CLI application */
        Enetcli_write("---------------------------------------\r\n");
        Enetcli_write("\nCLI for Jacinto Tests\r\nUse 'help' to list all available commands\r\n");

        /* Continuously take commands from the user and process the commands till application quits */
        while (inTerminal)
        {
            EnetAppUtils_print("\n> ");
            Enetcli_read(rxBuffer, MAX_READ_BUFFER_LEN);
            do
            {
                xMoreDataToFollow = EnetCli_processCommand(rxBuffer, 
                                                           txBuffer,
                                                           MAX_WRITE_BUFFER_LEN);

                Enetcli_write(txBuffer);
                memset(txBuffer, 0x00, MAX_WRITE_BUFFER_LEN);
            } while (xMoreDataToFollow);
            if (strcmp(rxBuffer, "quit") == 0)
            {
                inTerminal = BFALSE;
            }
            memset(rxBuffer, 0x00, MAX_READ_BUFFER_LEN);
        }

        /* End of CLI Application */
        Enetcli_write("\nCLI Application Closed\r\n");
    }

    /* Disable host port */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(EnetApp_inst.hEnet, 
                            EnetApp_inst.coreId, 
                            ENET_HOSTPORT_IOCTL_DISABLE, 
                            &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to disable host port: %d\n", status);
        }
    }

    /* Stop periodic tick timer */
    ClockP_stop(EnetApp_inst.hTickTimer);

    /* Close Enet DMA driver */
    EnetApp_closeDma();

    /* Close Enet driver */
    EnetApp_closeEnet();

    /* Disable peripheral clocks */
    EnetAppUtils_disableClocks(EnetApp_inst.enetType, EnetApp_inst.instId);

    /* Delete RX and TX tasks */
    EnetApp_deleteRxTxTasks();

    /* Deinit Enet driver */
    Enet_deinit();

    /* Delete all TX/RX semaphores */
    SemaphoreP_delete(EnetApp_inst.hRxDoneSem);
    EnetApp_inst.hRxDoneSem = NULL;
    SemaphoreP_delete(EnetApp_inst.hTxDoneSem);
    EnetApp_inst.hTxDoneSem = NULL;
    SemaphoreP_delete(EnetApp_inst.txSemObj[0]);
    EnetApp_inst.txSemObj[0] = NULL;
    SemaphoreP_delete(EnetApp_inst.rxSemObj[0]);
    EnetApp_inst.rxSemObj[0] = NULL;

    Enetcli_write("\nCLI Application Closed\r\n");

    EnetApp_deleteClock();

    EnetBoard_deinit();
}

static int32_t EnetApp_showAlivePhys(void)
{
    Enet_IoctlPrms prms;
    bool alive = BFALSE;
    int8_t i;
    int32_t status;

    for (i = 0U; i < ENET_MDIO_PHY_CNT_MAX; i++)
    {
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &i, &alive);

        status = Enet_ioctl(EnetApp_inst.hEnet, 
                            EnetApp_inst.coreId, 
                            ENET_MDIO_IOCTL_IS_ALIVE, 
                            &prms);
        if (status == ENET_SOK)
        {
            if (alive == BTRUE)
            {
                EnetAppUtils_print("PHY %u is alive\n", i);
            }
        }
        else
        {
            EnetAppUtils_print("Failed to get PHY %u alive status: %d\n", i, 
                                status);
        }
    }

    return status;
}

static int32_t EnetApp_setupCpswAle(void)
{
    Enet_IoctlPrms prms;
    CpswAle_SetPortStateInArgs setPortStateInArgs;
    CpswAle_SetUcastEntryInArgs setUcastInArgs;
    uint32_t entryIdx;
    int32_t status;

    /* ALE entry with "secure" bit cleared is required for loopback */
    setUcastInArgs.addr.vlanId = 0U;
    setUcastInArgs.info.portNum = CPSW_ALE_HOST_PORT_NUM;
    setUcastInArgs.info.blocked = BFALSE;
    setUcastInArgs.info.secure = BFALSE;
    setUcastInArgs.info.super = BFALSE;
    setUcastInArgs.info.ageable = BFALSE;
    setUcastInArgs.info.trunk = BFALSE;
    EnetUtils_copyMacAddr(&setUcastInArgs.addr.addr[0U], EnetApp_inst.hostMacAddr);
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &entryIdx);

    status = Enet_ioctl(EnetApp_inst.hEnet, 
                        EnetApp_inst.coreId, 
                        CPSW_ALE_IOCTL_ADD_UCAST, 
                        &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to add ucast entry: %d\n", status);
    }

    /* Set host port to 'forwarding' state */
    if (status == ENET_SOK)
    {
        setPortStateInArgs.portNum = CPSW_ALE_HOST_PORT_NUM;
        setPortStateInArgs.portState = CPSW_ALE_PORTSTATE_FORWARD;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setPortStateInArgs);

        status = Enet_ioctl(EnetApp_inst.hEnet, 
                            EnetApp_inst.coreId, 
                            CPSW_ALE_IOCTL_SET_PORT_STATE, 
                            &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to set ALE port state: %d\n", status);
        }
    }

    return status;
}

static void EnetApp_closeEnet(void)
{
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;
    uint8_t portNum;

    for ( portNum = 0; portNum < num_of_ports; portNum++)
    {
        /* Close port link */
        if (EnetApp_inst.macPort[allowedMacPortNums[portNum]] != ENET_MAC_PORT_INV)
        {
            ENET_IOCTL_SET_IN_ARGS(&prms, &EnetApp_inst.macPort[allowedMacPortNums[portNum]]);

            status = Enet_ioctl(EnetApp_inst.hEnet, 
                                EnetApp_inst.coreId, 
                                ENET_PER_IOCTL_CLOSE_PORT_LINK, 
                                &prms);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("Failed to close port link: %d\n", status);
            }
        }
    }

    /* Detach core */
    if (status == ENET_SOK)
    {
        ENET_IOCTL_SET_IN_ARGS(&prms, &EnetApp_inst.coreKey);

        status = Enet_ioctl(EnetApp_inst.hEnet, 
                            EnetApp_inst.coreId, 
                            ENET_PER_IOCTL_DETACH_CORE, 
                            &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to detach core key %u: %d\n", 
                                EnetApp_inst.coreKey, 
                                status);
        }
    }

    /* Close Enet driver */
    Enet_close(EnetApp_inst.hEnet);

    /* Close UDMA */
    EnetAppUtils_udmaclose(EnetApp_inst.hUdmaDrv);

    EnetApp_inst.hEnet = NULL;
}

static int32_t EnetApp_openEnet(void)
{
    Cpsw_Cfg cpswCfg;
    EnetDma_Cfg dmaCfg;
    Enet_IoctlPrms prms;
    EnetPer_PortLinkCfg portLinkCfg;
    CpswMacPort_Cfg macCfg;
    int32_t status = ENET_SOK;

    cpswCfg.dmaCfg = &dmaCfg;

    /* Initialize peripheral config */
    EnetApp_initCpswCfg(&cpswCfg);

    if (EnetApp_inst.enetType == ENET_CPSW_9G)
    {
        EnetAppUtils_print("CPSW_9G Test on MAIN NAVSS\n");
    }
    else if (EnetApp_inst.enetType == ENET_CPSW_5G)
    {
        EnetAppUtils_print("CPSW_5G Test on MAIN NAVSS\n");
    }
    else if (EnetApp_inst.enetType == ENET_CPSW_2G)
    {
        if (EnetApp_inst.instId == 0)
        {
            EnetAppUtils_print("CPSW_2G Test on MCU NAVSS\n");
        }
        else if (EnetApp_inst.instId == 1)
        {
            EnetAppUtils_print("CPSW_2G Test on MAIN NAVSS\n");
        }
    }

    dmaCfg.rxChInitPrms.dmaPriority = UDMA_DEFAULT_RX_CH_DMA_PRIORITY;

    /* App should open UDMA first as UDMA handle is needed to initialize
     * CPSW RX channel */
    EnetApp_inst.hUdmaDrv = EnetAppUtils_udmaOpen(EnetApp_inst.enetType, NULL);
    EnetAppUtils_assert(NULL != EnetApp_inst.hUdmaDrv);

    dmaCfg.hUdmaDrv = EnetApp_inst.hUdmaDrv;

    /* Open the Enet driver */
    EnetApp_inst.hEnet = Enet_open(EnetApp_inst.enetType, 
                                   EnetApp_inst.instId, &cpswCfg, 
                                   sizeof(cpswCfg));
    if (EnetApp_inst.hEnet == NULL)
    {
        EnetAppUtils_print("Failed to open Enet driver\n");
        status = ENET_EFAIL;
    }

    uint8_t portNum;
    for (portNum = 0; portNum < num_of_ports && status == ENET_SOK; portNum++)
    {
        if (EnetApp_inst.macPort[allowedMacPortNums[portNum]] != ENET_MAC_PORT_INV)
        {
            /* Setup port link open parameters */
            if (status == ENET_SOK)
            {
                EnetBoard_EthPort ethPort;
                EnetMacPort_LinkCfg *linkCfg = &portLinkCfg.linkCfg;
                EnetMacPort_Interface *mii = &portLinkCfg.mii;
                EnetPhy_Cfg *phyCfg = &portLinkCfg.phyCfg;
                EnetPhy_Mii phyMii;

                /* Setup board for requested Ethernet port */
                ethPort.macPort = EnetApp_inst.macPort[allowedMacPortNums[portNum]];
                ethPort.boardId = EnetApp_inst.boardId[allowedMacPortNums[portNum]];
                ethPort.expPort = EnetApp_inst.expPort[allowedMacPortNums[portNum]];
                EnetApp_macMode2MacMii(EnetApp_inst.macMode[allowedMacPortNums[portNum]], 
                                       &ethPort.mii);

                status = EnetBoard_setupPorts(EnetApp_inst.enetType, 
                                              EnetApp_inst.instId, 
                                              &ethPort, 
                                              8U);
                EnetAppUtils_assert(status == ENET_SOK);

                /* Set port link params */
                portLinkCfg.macPort = EnetApp_inst.macPort[allowedMacPortNums[portNum]];
                portLinkCfg.macCfg = &macCfg;

                CpswMacPort_initCfg(&macCfg);
                EnetApp_macMode2MacMii(EnetApp_inst.macMode[allowedMacPortNums[portNum]], mii);

                /* Following is the phy configuration parameters */
                const EnetBoard_PortCfg *portCfg = NULL;

                /* Set PHY configuration params */
                EnetPhy_initCfg(phyCfg);
                status = EnetApp_macMode2PhyMii(EnetApp_inst.macMode[allowedMacPortNums[portNum]], 
                                                &phyMii);

                if (status == ENET_SOK)
                {
                    portCfg = EnetBoard_getPortCfg(EnetApp_inst.enetType, 
                                                   EnetApp_inst.instId, 
                                                   &ethPort);
                    if (portCfg != NULL)
                    {
                        phyCfg->phyAddr = portCfg->phyCfg.phyAddr;
                        phyCfg->loopbackEn = BFALSE;
                        phyCfg->isStrapped = portCfg->phyCfg.isStrapped;
                        phyCfg->skipExtendedCfg = portCfg->phyCfg.skipExtendedCfg;
                        phyCfg->extendedCfgSize = portCfg->phyCfg.extendedCfgSize;
                        memcpy(phyCfg->extendedCfg, 
                               portCfg->phyCfg.extendedCfg, 
                               phyCfg->extendedCfgSize);

                        macCfg.sgmiiMode = portCfg->sgmiiMode;
                        *linkCfg = portCfg->linkCfg;
                    }
                    else
                    {
                        EnetAppUtils_print("Port info not found\n");
                        EnetAppUtils_assert(BFALSE);
                    }
                }

                mii->layerType = ethPort.mii.layerType;
                mii->sublayerType = ethPort.mii.sublayerType;
                mii->variantType = ENET_MAC_VARIANT_FORCED;

                /* Open port link */
                if (status == ENET_SOK)
                {
                    ENET_IOCTL_SET_IN_ARGS(&prms, &portLinkCfg);

                    status = Enet_ioctl(EnetApp_inst.hEnet, 
                                        EnetApp_inst.coreId, 
                                        ENET_PER_IOCTL_OPEN_PORT_LINK, 
                                        &prms);
                    if (status != ENET_SOK)
                    {
                        EnetAppUtils_print("Failed to open port link: %d\n", status);
                    }
                }
            }
        }
    }
    
    return status;
}

static void EnetApp_initCpswCfg(Cpsw_Cfg *cpswCfg)
{
    CpswHostPort_Cfg *hostPortCfg = &cpswCfg->hostPortCfg;
    CpswAle_Cfg *aleCfg = &cpswCfg->aleCfg;
    CpswCpts_Cfg *cptsCfg = &cpswCfg->cptsCfg;

    /* Set initial config */
    Enet_initCfg(EnetApp_inst.enetType, EnetApp_inst.instId, cpswCfg, sizeof(*cpswCfg));

    /* Peripheral config */
    cpswCfg->vlanCfg.vlanAware = BTRUE;

    /* Host port config */
    hostPortCfg->removeCrc = BTRUE;
    hostPortCfg->padShortPacket = BTRUE;
    hostPortCfg->passCrcErrors = BTRUE;

    /* ALE config */
    aleCfg->modeFlags = CPSW_ALE_CFG_MODULE_EN;
    aleCfg->agingCfg.autoAgingEn = BTRUE;
    aleCfg->agingCfg.agingPeriodInMs = 1000;
    aleCfg->nwSecCfg.vid0ModeEn = BTRUE;
    aleCfg->vlanCfg.aleVlanAwareMode = BTRUE;
    aleCfg->vlanCfg.cpswVlanAwareMode = BTRUE;
    aleCfg->vlanCfg.unknownUnregMcastFloodMask = CPSW_ALE_ALL_PORTS_MASK;
    aleCfg->vlanCfg.unknownRegMcastFloodMask = CPSW_ALE_ALL_PORTS_MASK;
    aleCfg->vlanCfg.unknownVlanMemberListMask = CPSW_ALE_ALL_PORTS_MASK;

    /* ALE policer partition configuration */
    memcpy(aleCfg->policerTablePartSize, gEthFw_policerTablePartSize, sizeof(gEthFw_policerTablePartSize));

    /* CPTS config */
    /* Note: Timestamping and MAC loopback are not supported together because of
     * IP limitation, so disabling timestamping for this application */
    cptsCfg->hostRxTsEn = BFALSE;

    EnetAppUtils_initResourceConfig(EnetApp_inst.enetType, 
                                    EnetApp_inst.instId, 
                                    EnetApp_inst.coreId, 
                                    &cpswCfg->resCfg);
}
/* None */
