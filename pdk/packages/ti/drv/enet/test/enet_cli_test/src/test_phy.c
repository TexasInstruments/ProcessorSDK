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
 * \file  test_phy.c
 *
 * \brief This file contains scripts to print the MDIO phy registers for 
 * clause-22
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "cli_common.h"
#include "test_phy.h"
#include <enet_board.h>

#include "FreeRTOS_CLI.h"


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct Enetcli_Cfg_s
{
    /* Duplexity of the phy mode. Can be Full duplex, Half duplex or Auto. */
    Enet_Duplexity duplexity;

    /* Speed of the phy mode. Can be 10mbit, 100mbit, 1gbit, 10gbit or Auto. */
    Enet_Speed speed;

    /* Macport where the phy mode needs to be set. */
    uint32_t macport;

    /* If this set to true, we reset the phymode to 1gbit and Full Duplex */
    bool isReset;

} Enetcli_Cfg;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* Reads the parameters and sets configuration. */
static BaseType_t Enetcli_config(char *writeBuffer,
                                 size_t writeBufferLen,
                                 const char *commandString);

/* Reads the parameters and sets configuration to set the phy
*  mode for a particular macport. */
static BaseType_t Enetcli_phyModeCfg(char *writeBuffer,
                                         size_t writeBufferLen,
                                         const char *commandString);

/* Function which closes a port which is open. */
static int32_t Enetcli_closePortLink();

/* Function which opens the port link after setting the phy
*  mode configuration according to what the user set. */
static int32_t Enetcli_openPortLink();

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint8_t gMacPortNum;
Enetcli_Cfg Enetcli_configObj;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* Prints the Clause-22 MDIO Phy registers */
BaseType_t Enetcli_printPhyRegs(char *writeBuffer,
                                size_t writeBufferLen,
                                const char *commandString)
{
    int32_t status = ENET_SOK;
    BaseType_t config_status = BTRUE;
    Enet_IoctlPrms prms;
    EnetPhy_GenericInArgs inArgs;

    /* This will set the configuration */
    config_status = Enetcli_config(writeBuffer,
                                   writeBufferLen,
                                   commandString);

    if (config_status == pdFALSE)
    {
        status = ENET_EBADARGS;
    }

    if (status == ENET_SOK)
    {
        /* Take the port that user provides */
        inArgs.macPort = gMacPortNum;

        ENET_IOCTL_SET_IN_ARGS(&prms, &inArgs);
        status = Enet_ioctl(EnetApp_inst.hEnet,
                            EnetApp_inst.coreId,
                            ENET_PHY_IOCTL_PRINT_REGS,
                            &prms);

        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to print the registers: %d\n", status);
        }
    }
    return pdFALSE;
}

BaseType_t Enetcli_phyMode(char *writeBuffer,
                           size_t writeBufferLen,
                           const char *commandString)
{
    int32_t status = ENET_SOK;
    BaseType_t config_status = BTRUE;

    Enetcli_configObj.duplexity = ENET_DUPLEX_FULL;
    Enetcli_configObj.speed = ENET_SPEED_1GBIT;
    Enetcli_configObj.macport = 2;
    Enetcli_configObj.isReset = BFALSE;

    /* This will set the configuration */
    config_status = Enetcli_phyModeCfg(writeBuffer,
                                       writeBufferLen,
                                       commandString);

    if (config_status == pdFALSE)
    {
        return config_status;
    }

    if (Enetcli_configObj.isReset == BFALSE)
    {
        if (EnetApp_inst.macPort[Enetcli_configObj.macport] != ENET_MAC_PORT_INV)
        {
            status = Enetcli_closePortLink();
            if (status == ENET_SOK)
            {
                status = Enetcli_openPortLink();
            }
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("Was not able to set the phy mode.\r\n");
            }
        }
        else
        {
            status = ENET_EFAIL;
            EnetAppUtils_print("Macport configuration is not correct. Please try again.\r\n");
        }

        if (status == ENET_SOK)
        {
            EnetAppUtils_print("Reset phy mode successfully.\r\n");
        }
    }

    else
    {
        if (EnetApp_inst.macPort[Enetcli_configObj.macport] != ENET_MAC_PORT_INV)
        {
            /* This configuration resets the corresponding port.*/
            Enetcli_configObj.duplexity = ENET_DUPLEX_FULL;
            Enetcli_configObj.speed = ENET_SPEED_1GBIT;

            status = Enetcli_closePortLink();
            if (status == ENET_SOK)
            {
                status = Enetcli_openPortLink();
            }
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("Was not able to reset the phy mode.\r\n");
            }
        }
        else
        {
            status = ENET_EFAIL;
            EnetAppUtils_print("Macport configuration is not correct. Please try again.\r\n");
        }

        if (status == ENET_SOK)
        {
            EnetAppUtils_print("Reset phy mode successfully.\r\n");
        }
    }
return pdFALSE;
}

/* ========================================================================== */
/*                   Static Function Definitions                              */
/* ========================================================================== */

static BaseType_t Enetcli_config(char *writeBuffer,
                                 size_t writeBufferLen,
                                 const char *commandString)
{
    char *parameter;
    BaseType_t paramLen;
    uint32_t status = ENET_SOK;
    BaseType_t retVal = pdFALSE;

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                                                 1,
                                                 &paramLen);

    if (parameter != NULL)
    {
        /* Checking for the -p parameter which is the macport number.*/
        uint8_t macport = atoi(parameter);
        gMacPortNum = CPSW_ALE_ALEPORT_TO_MACPORT(macport);
        if ((gMacPortNum != ENET_MAC_PORT_1) && (gMacPortNum != ENET_MAC_PORT_3))
        {
            EnetAppUtils_print("Invalid Parameter for macport number "
                               "(mac port number should be either 1 or 3 only)\r\n");
            status = ENET_EBADARGS;
        }
    }
    else
    {
        EnetAppUtils_print("No Parameter provided for macport number\r\n");
        status = ENET_EBADARGS;
    }

    if (status == ENET_SOK)
    {
        retVal = pdTRUE;
    }
    return retVal;
}

static BaseType_t Enetcli_phyModeCfg(char *writeBuffer,
                                         size_t writeBufferLen,
                                         const char *commandString)
{
    char *parameter;
    BaseType_t paramLen;
    uint32_t paramCnt = 1;
    bool isPortSet = BFALSE;
    bool isDuplexitySet = BFALSE;
    bool isSpeedSet = BFALSE;

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                                                 paramCnt,
                                                 &paramLen);

    while (parameter != NULL)
    {
        /* Checking for the -s parameter which is the speed of the phymode */
        if (strncmp("-s", parameter, paramLen) == 0)
        {
            parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                                                         paramCnt+1,
                                                         &paramLen);
            if(parameter != NULL)
            {
                if(strncmp("10m", parameter, paramLen) == 0)
                {
                    Enetcli_configObj.speed = ENET_SPEED_10MBIT;
                }
                else if(strncmp("100m", parameter, paramLen) == 0)
                {
                    Enetcli_configObj.speed = ENET_SPEED_100MBIT;
                }
                else if(strncmp("1g", parameter, paramLen) == 0)
                {
                    Enetcli_configObj.speed = ENET_SPEED_1GBIT;
                }
                else if(strncmp("10g", parameter, paramLen) == 0)
                {
                    Enetcli_configObj.speed = ENET_SPEED_10GBIT;
                }
                else if(strncmp("auto", parameter, paramLen) == 0)
                {
                    Enetcli_configObj.speed = ENET_SPEED_AUTO;
                }
                else
                {
                    snprintf(writeBuffer, writeBufferLen, "Invalid Parameter for -s\r\n");
                    return pdFALSE;
                }
                isSpeedSet = BTRUE;
                paramCnt += 2;
            }
            else
            {
                snprintf(writeBuffer, writeBufferLen, "No Parameter provided for -s\r\n");
                return pdFALSE;
            }
        }

        /* Checking for the -p parameter which is the macport number.*/
        else if (strncmp("-p", parameter, paramLen) == 0)
        {
            parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                                                         paramCnt+1,
                                                         &paramLen);
            if(parameter != NULL)
            {
                uint8_t macPortNum = atoi(parameter);
                macPortNum = CPSW_ALE_ALEPORT_TO_MACPORT(macPortNum);
                if ((macPortNum >= ENET_MAC_PORT_1) && (macPortNum <= ENET_MAC_PORT_8))
                {
                    Enetcli_configObj.macport = macPortNum;
                    paramCnt += 2;
                }
                else
                {
                    snprintf(writeBuffer, writeBufferLen, "Invalid Parameter for -p (mac port number should be between 1 or 8 only)\r\n");
                    return pdFALSE;
                }
                isPortSet = BTRUE;
            }
            else
            {
                snprintf(writeBuffer, writeBufferLen, "No Parameter provided for -p\r\n");
                return pdFALSE;
            }
        }

        /* Checking for the -d parameter which is the duplexity. */
        else if (strncmp("-d", parameter, paramLen) == 0)
        {
            parameter = (char*) FreeRTOS_CLIGetParameter(commandString, paramCnt+1,
            &paramLen);
            if(parameter != NULL)
            {
                if (strncmp("half", parameter, paramLen) == 0)
                {
                    Enetcli_configObj.duplexity = ENET_DUPLEX_HALF;
                }
                else if (strncmp("full", parameter, paramLen) == 0)
                {
                    Enetcli_configObj.duplexity = ENET_DUPLEX_FULL;
                }

                else if (strncmp("auto", parameter, paramLen) == 0)
                {
                    Enetcli_configObj.duplexity = ENET_DUPLEX_AUTO;
                }
                else
                {
                    snprintf(writeBuffer, 
                             writeBufferLen, 
                             "Invalid Parameter for -d\r\n");
                    return pdFALSE;
                }
                isDuplexitySet = BTRUE;
                paramCnt += 2;
            }
            else
            {
                snprintf(writeBuffer, writeBufferLen, "No Parameter provided for -d\r\n");
                return pdFALSE;
            }
        }
        /* -r flag means that the phymode for the macport will be reset.*/
        else if (strncmp("-r", parameter, paramLen) == 0)
        {
            EnetAppUtils_print("Reset triggered\n");
            Enetcli_configObj.isReset = BTRUE;
            paramCnt += 1;
        }
        parameter = (char*) FreeRTOS_CLIGetParameter(commandString, paramCnt,
        &paramLen);
    }

    if (isPortSet == BFALSE )
    {
        snprintf(writeBuffer, 
                 writeBufferLen, 
                 "Provide port number\r\n");
        return pdFALSE;
    }

    if ((isDuplexitySet == BFALSE) && (Enetcli_configObj.isReset == BFALSE))
    {
        snprintf(writeBuffer, 
                 writeBufferLen, 
                 "Duplexity value not provided\r\n");
        return pdFALSE;
    }

    if ((isSpeedSet == BFALSE) && (Enetcli_configObj.isReset == BFALSE))
    {
        snprintf(writeBuffer, 
                 writeBufferLen, 
                 "Speed value not provided\r\n");
        return pdFALSE;
    }
    return pdTRUE;
}
static int32_t Enetcli_openPortLink()
{
    /* Setup port link open parameters */
    EnetPer_PortLinkCfg portLinkCfg;
    EnetBoard_EthPort ethPort;
    EnetMacPort_LinkCfg *linkCfg = &portLinkCfg.linkCfg;
    EnetMacPort_Interface *mii = &portLinkCfg.mii;
    EnetPhy_Cfg *phyCfg = &portLinkCfg.phyCfg;
    CpswMacPort_Cfg macCfg;
    EnetPhy_Mii phyMii;
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;

    /* Following is the phy configuration parameters */
    const EnetBoard_PortCfg *portCfg = NULL;

    /* Setup board for requested Ethernet port */
    ethPort.macPort = EnetApp_inst.macPort[Enetcli_configObj.macport];
    ethPort.boardId = EnetApp_inst.boardId[Enetcli_configObj.macport];
    ethPort.expPort = EnetApp_inst.expPort[Enetcli_configObj.macport];
    EnetApp_macMode2MacMii(EnetApp_inst.macMode[Enetcli_configObj.macport], 
                            &ethPort.mii);

    status = EnetBoard_setupPorts(EnetApp_inst.enetType, 
                                    EnetApp_inst.instId, 
                                    &ethPort, 
                                    8U);
    EnetAppUtils_assert(status == ENET_SOK);

    /* Set port link params */
    portLinkCfg.macPort = EnetApp_inst.macPort[Enetcli_configObj.macport];
    portLinkCfg.macCfg = &macCfg;

    CpswMacPort_initCfg(&macCfg);
    EnetApp_macMode2MacMii(EnetApp_inst.macMode[Enetcli_configObj.macport], mii);

    /* Set PHY configuration params */
    EnetPhy_initCfg(phyCfg);
    status = EnetApp_macMode2PhyMii(EnetApp_inst.macMode[Enetcli_configObj.macport], 
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

        }
        else
        {
            EnetAppUtils_print("Port info not found\n");
            status = ENET_EFAIL;
        }
    }

    if(status == ENET_SOK)
    {
        linkCfg->speed = Enetcli_configObj.speed;
        linkCfg->duplexity = Enetcli_configObj.duplexity;
    }

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

    return status;
}

static int32_t Enetcli_closePortLink()
{
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;

    /* Close port link */
    ENET_IOCTL_SET_IN_ARGS(&prms, &EnetApp_inst.macPort[Enetcli_configObj.macport]);

    status = Enet_ioctl(EnetApp_inst.hEnet,
                        EnetApp_inst.coreId,
                        ENET_PER_IOCTL_CLOSE_PORT_LINK,
                        &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to close port link: %d\n", status);
    }
    return status;
}