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
 * \file  test_ale.c
 *
 * \brief This file contains scripts to add entries to ALE
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "cli_common.h"
#include "test_ale.h"

#include <FreeRTOS_CLI.h>


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

static int32_t EnetApp_addUcastEntry(uint8_t *macAddr,
                                     bool isAgeable,
                                     bool isBlocked);

static int32_t EnetApp_addMcastEntry(uint8_t *macAddr);

static int32_t EnetApp_addOuiEntry(uint8_t *macaddr);

static int32_t EnetApp_addIPv4Entry(uint8_t *macAddr);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

BaseType_t EnetCLI_addUcast(char *writeBuffer,
                            size_t writeBufferLen,
                            const char *commandString)
{
    int32_t status = ENET_SOK;
    uint8_t macAddr[ENET_MAC_ADDR_LEN];
    char *parameter;
    BaseType_t paramLen;
    uint32_t paramCnt = 1U;
    uint8_t makeDefault = 0U;
    bool isAgeable = BFALSE;
    bool isBlocked = BFALSE;
    char macaddrString[18]={0};

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                                                 paramCnt,
                                                 &paramLen);
    if (parameter == NULL) 
    {
        EnetAppUtils_print("MAC address must be entered\r\n");
        status = ENET_EBADARGS;
    }

    while (parameter != NULL)
    {
        if (strncmp(parameter, "-d", paramLen) == 0)
        {
            makeDefault = 1;
        }
        else if (strncmp(parameter, "-a", paramLen) == 0)
        {
            isAgeable = BTRUE;
        }
        else if (strncmp(parameter, "-b", paramLen) == 0)
        {
            isBlocked = BTRUE;
        }
        else
        {
            status = EnetAppUtils_macAddrAtoI(parameter, macAddr);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("Invalid Parameter\r\n");
                status = ENET_EBADARGS;
                break;
            }
            strncpy(macaddrString, parameter, paramLen);
        }
        paramCnt++;
        parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                                                     paramCnt,
                                                     &paramLen);
    }

    if (status == ENET_SOK)
    /* Add unicast entry to ALE */
    {
        status = EnetApp_addUcastEntry(macAddr, isAgeable, isBlocked);
    }

    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to add unicast entry to ALE: %d\r\n",
                            status);
    }
    else
    {
        EnetAppUtils_print("Added unicast entry %s to ALE\r\n",macaddrString);
        if (makeDefault)
        {
            EnetUtils_copyMacAddr(EnetApp_inst.hostMacAddr, macAddr);
            EnetAppUtils_print("[INF] %s: Default MAC address set to ",
                                __func__);
            EnetAppUtils_printMacAddr(EnetApp_inst.hostMacAddr);
        }
    }
    return pdFALSE;
}

BaseType_t EnetCLI_removeUcast(char *writeBuffer,
                               size_t writeBufferLen,
                               const char *commandString)
{
    int32_t status = ENET_SOK;
    CpswAle_MacAddrInfo inArgs;
    char *parameter;
    BaseType_t paramLen;
    Enet_IoctlPrms prms;
    char macaddrString[18]={0};

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 1, &paramLen);
    status = EnetAppUtils_macAddrAtoI(parameter, inArgs.addr);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Invalid Parameter\r\n");
    }

    if (status == ENET_SOK)
    {
        /* Remove unicast entry from ALE */
        ENET_IOCTL_SET_IN_ARGS(&prms, &inArgs);
        status = Enet_ioctl(EnetApp_inst.hEnet,
                            EnetApp_inst.coreId,
                            CPSW_ALE_IOCTL_REMOVE_ADDR,
                            &prms);
    }
    if (status != ENET_SOK)
    {
        strncpy(macaddrString, parameter, paramLen);
        EnetAppUtils_print("Failed to remove unicast entry %s from ALE\r\n", macaddrString);
    }
    else
    {
        strncpy(macaddrString, parameter, paramLen);
        EnetAppUtils_print("Removed unicast entry %s from ALE\r\n", macaddrString);
    }
    return pdFALSE;
}

BaseType_t EnetCLI_addMcast(char *writeBuffer,
                            size_t writeBufferLen,
                            const char *commandString)
{
    int32_t status = ENET_SOK;
    uint8_t macAddr[ENET_MAC_ADDR_LEN];
    char *parameter;
    BaseType_t paramLen;
    uint32_t paramCnt = 1U;
    char macaddrString[18]={0};

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                                                 paramCnt,
                                                 &paramLen);

    if (parameter == NULL)
    {
        status = ENET_EBADARGS;
    }

    if (status == ENET_SOK)
    {
        status = EnetAppUtils_macAddrAtoI(parameter, macAddr);
    }
    
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Invalid Parameter\r\n");
        status = ENET_EBADARGS;
    }

    if (status == ENET_SOK)
    {
        /* Add multicast entry to ALE */
        status = EnetApp_addMcastEntry(macAddr);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to add multicast entry to ALE\r\n");
        }
        else
        {
            strncpy(macaddrString, parameter, paramLen);
            EnetAppUtils_print("Added multicast entry to ALE\r\n", macaddrString);
        }
    }
    return pdFALSE;
}

BaseType_t EnetCLI_removeMcast(char *writeBuffer,
                               size_t writeBufferLen,
                               const char *commandString)
{
    int32_t status = ENET_SOK;
    CpswAle_MacAddrInfo inArgs;
    char *parameter;
    BaseType_t paramLen;
    Enet_IoctlPrms prms;
    char macaddrString[18]={0};

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 1, &paramLen);
    status = EnetAppUtils_macAddrAtoI(parameter, inArgs.addr);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Invalid Parameter\r\n");
        status = ENET_EBADARGS;
    }

    if (status == ENET_SOK)
    {
        /* Remove multicast entry from ALE */
        ENET_IOCTL_SET_IN_ARGS(&prms, &inArgs);
        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId,
                            CPSW_ALE_IOCTL_REMOVE_ADDR, &prms);
        if (status != ENET_SOK)
        {
            strncpy(macaddrString, parameter, paramLen);
            EnetAppUtils_print("Failed to remove multicast entry %s from ALE\r\n", macaddrString);
        }
        else
        {
            strncpy(macaddrString, parameter, paramLen);
            EnetAppUtils_print("Removed multicast entry %s from ALE\r\n", macaddrString);
        }
    }
    return pdFALSE;
}

BaseType_t EnetCLI_addOui(char *writeBuffer,
                          size_t writeBufferLen,
                          const char *commandString)
{
    int32_t status = ENET_SOK;
    char *parameter;
    BaseType_t paramLen;
    uint32_t paramCnt = 1U;
    uint8_t macAddr[ENET_MAC_ADDR_LEN];
    char macaddrString[18]={0};

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                                                 paramCnt,
                                                 &paramLen);

    if (parameter != NULL)
    {
        status = EnetAppUtils_macAddrAtoI(parameter, macAddr);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Invalid Parameter\r\n");
            status = ENET_EBADARGS;
        }
    }
    else
    {
        EnetAppUtils_print("No Parameter Specified\r\n");
        status = ENET_EBADARGS;
    }

    if (status == ENET_SOK)
    {
        status = EnetApp_addOuiEntry(macAddr);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to add OUI entry for the MAC address \r\n");
        }
        else
        {
            strncpy(macaddrString, parameter, paramLen);
            EnetAppUtils_print("OUI entry added for MAC address %s \r\n", macaddrString);
        }
    }
    return pdFALSE;
}

BaseType_t EnetCLI_removeOui(char *writeBuffer,
                             size_t writeBufferLen,
                             const char *commandString)
{
    int32_t status = ENET_SOK;
    char *parameter;
    BaseType_t paramLen;
    uint8_t macAddr[ENET_MAC_ADDR_LEN];
    Enet_IoctlPrms prms;
    char macaddrString[18]={0};

    CpswAle_OuiEntryInfo setOuiInArgs;

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 1, &paramLen);
    if (parameter != NULL)
    {
        status = EnetAppUtils_macAddrAtoI(parameter, macAddr);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Invalid Parameter\r\n");
            status = ENET_EBADARGS;
        }
    }
    else
    {
        EnetAppUtils_print("No Parameter Specified\r\n");
        status = ENET_EBADARGS;
    }

    if (status == ENET_SOK)
    {
        /* Remove OUI config from ALE */
        memcpy(&setOuiInArgs.ouiAddr[0], macAddr, sizeof(setOuiInArgs.ouiAddr));

        ENET_IOCTL_SET_IN_ARGS(&prms, &setOuiInArgs);
        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId,
                            CPSW_ALE_IOCTL_REMOVE_OUI, &prms);
        if (status != ENET_SOK)
        {
            strncpy(macaddrString, parameter, paramLen);
            EnetAppUtils_print("Failed to remove OUI for MAC address %s \r\n", macaddrString);
        }
        else
        {
            strncpy(macaddrString, parameter, paramLen);
            EnetAppUtils_print("OUI configuration removed for MAC address %s\r\n", macaddrString);
        }
    }
    return pdFALSE;
}

BaseType_t EnetCLI_addipv4(char *writeBuffer,
                           size_t writeBufferLen,
                           const char *commandString)
{
    int32_t status = ENET_SOK;
    uint8_t ipAddr[ENET_IPv4_ADDR_LEN];
    char *parameter;
    BaseType_t paramLen;
    uint32_t paramCnt = 1U;

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                                                 paramCnt,
                                                 &paramLen);

    if (parameter == NULL)
    {
        status = ENET_EBADARGS;
    }

    if (status == ENET_SOK)
    {
        status = EnetAppUtils_ipAddrAtoI(parameter, ipAddr);
    }
    
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Invalid Parameter\r\n");
        status = ENET_EBADARGS;
    }

    if (status == ENET_SOK)
    {
        /* Add ipv4 entry to ALE */
        status = EnetApp_addIPv4Entry(ipAddr);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to add IPv4 entry to ALE for IP address\r\n");
        }
        else
        {
            EnetAppUtils_print("Added IPv4 entry %s to ALE\r\n", parameter);
        }
    }
    return pdFALSE;
}

BaseType_t EnetCLI_removeipv4(char *writeBuffer,
                              size_t writeBufferLen,
                              const char *commandString)
{
    int32_t status = ENET_SOK;
    CpswAle_IPv4EntryInfo setIpv4InArgs;
    char *parameter;
    BaseType_t paramLen;
    setIpv4InArgs.numLSBIgnoreBits = 0;
    Enet_IoctlPrms prms;
    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, 1, &paramLen);
    
    if (parameter == NULL)
    {
        status = ENET_EBADARGS;
    }

    if (status == ENET_SOK)
    {
        status = EnetAppUtils_ipAddrAtoI(parameter, setIpv4InArgs.ipv4Addr);
    }
    
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Invalid Parameter\r\n");
        status = ENET_EBADARGS;
    }

    if (status == ENET_SOK)
    {
        /* Remove ipv4 entry from ALE */
        ENET_IOCTL_SET_IN_ARGS(&prms, &setIpv4InArgs);
        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId,
                            CPSW_ALE_IOCTL_REMOVE_IPV4ADDR, &prms);

        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to remove IPv4 %s entry from ALE\r\n", parameter);
        }
        else
        {
            EnetAppUtils_print("Removed IPv4 entry %s from ALE\r\n", parameter);
        }
    }
    return pdFALSE;
}

BaseType_t EnetCLI_changePortState(char *writeBuffer,
                                   size_t writeBufferLen,
                                   const char *commandString)
{
    Enet_IoctlPrms prms;
    CpswAle_SetPortStateInArgs setPortStateInArgs;
    int32_t status = ENET_SOK;
    uint8_t macPortNum;
    CpswAle_PortState portState;
    bool isMacPortSet = BFALSE;
    bool isPortStateSet = BFALSE;
    char *parameter;
    BaseType_t paramLen;
    uint32_t paramCnt = 1;

    parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                                                 paramCnt,
                                                 &paramLen);

    while (parameter != NULL)
    {
        /* Checking for the -p parameter which is the macport number.*/
        if (strncmp(parameter, "-p", paramLen) == 0)
        {
            parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                                                         paramCnt+1,
                                                         &paramLen);
            if (parameter != NULL)
            {
                uint8_t macport = atoi(parameter);
                macPortNum = CPSW_ALE_ALEPORT_TO_MACPORT(macport);
                if ((macPortNum != ENET_MAC_PORT_1) && (macPortNum != ENET_MAC_PORT_3))
                {
                    EnetAppUtils_print("Invalid Parameter for macport number "
                                       "(mac port number should be either 1 or 3 only)\r\n");
                    status = ENET_EBADARGS;
                    break;
                }
                isMacPortSet = BTRUE;
                paramCnt += 1;
            }
            else
            {
                EnetAppUtils_print("No Parameter provided for macport number\r\n");
                status = ENET_EBADARGS;
                break;
            }
        }
        else if (strncmp(parameter, "forward", paramLen) == 0)
        {
            isPortStateSet = BTRUE;
            portState = CPSW_ALE_PORTSTATE_FORWARD;
        }

        else if (strncmp(parameter, "block", paramLen) == 0)
        {
            isPortStateSet = BTRUE;
            portState = CPSW_ALE_PORTSTATE_BLOCKED;
        }

        else if (strncmp(parameter, "disable", paramLen) == 0)
        {
            isPortStateSet = BTRUE;
            portState = CPSW_ALE_PORTSTATE_DISABLED;
        }

        else if (strncmp(parameter, "learn", paramLen) == 0)
        {
            isPortStateSet = BTRUE;
            portState = CPSW_ALE_PORTSTATE_LEARN;
        }
        else
        {
            EnetAppUtils_print("Invalid Parameter\r\n");
            status = ENET_EBADARGS;
            break;
        }
        paramCnt++;
        parameter = (char*) FreeRTOS_CLIGetParameter(commandString,
                                                     paramCnt,
                                                     &paramLen);
    }

    if ((isMacPortSet == BFALSE) && (status == ENET_SOK))
    {
        EnetAppUtils_print("Macport must be passed\r\n");
        status = ENET_EBADARGS;
    }
    if ((isPortStateSet == BFALSE) && (status == ENET_SOK))
    {
        EnetAppUtils_print("Port state must be passed\r\n");
        status = ENET_EBADARGS;
    }
    if (status == ENET_SOK)
    {
        /* Set port state for macport */
        setPortStateInArgs.portNum   = macPortNum;
        setPortStateInArgs.portState = portState;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setPortStateInArgs);

        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, CPSW_ALE_IOCTL_SET_PORT_STATE, &prms);
        if (ENET_SOK != status)
        {
            EnetAppUtils_print("Failed to set ALE port state: %d\n", status);
        }
        else
        {
            switch(portState) {

                case CPSW_ALE_PORTSTATE_FORWARD: EnetAppUtils_print("Successfully set Macport %d to Forwarding state\r\n", 
                                                                     CPSW_ALE_MACPORT_TO_ALEPORT(macPortNum));
                                                 break;

                case CPSW_ALE_PORTSTATE_BLOCKED: EnetAppUtils_print("Successfully set Macport %d to Blocked state\r\n", 
                                                                     CPSW_ALE_MACPORT_TO_ALEPORT(macPortNum));
                                                 break;

                case CPSW_ALE_PORTSTATE_LEARN: EnetAppUtils_print("Successfully set Macport %d to Learning state\r\n", 
                                                                   CPSW_ALE_MACPORT_TO_ALEPORT(macPortNum));
                                                 break;

                case CPSW_ALE_PORTSTATE_DISABLED: EnetAppUtils_print("Successfully set Macport %d to Disabled state\r\n", 
                                                                      CPSW_ALE_MACPORT_TO_ALEPORT(macPortNum));
                                                 break;
            }
        }
    }
    return pdFALSE;
}

/* ========================================================================== */
/*                   Static Function Definitions                              */
/* ========================================================================== */

static int32_t EnetApp_addUcastEntry(uint8_t *macAddr, bool isAgeable, bool isBlocked)
{
    CpswAle_SetUcastEntryInArgs setUcastInArgs;
    uint32_t entryIdx;
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;

    setUcastInArgs.addr.vlanId = 0U;
    setUcastInArgs.info.portNum = CPSW_ALE_HOST_PORT_NUM;
    setUcastInArgs.info.blocked = isBlocked;
    setUcastInArgs.info.secure = BFALSE;
    setUcastInArgs.info.super = BFALSE;
    setUcastInArgs.info.ageable = isAgeable;
    setUcastInArgs.info.trunk = BFALSE;
    EnetUtils_copyMacAddr(&setUcastInArgs.addr.addr[0U], macAddr);
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &entryIdx);
    status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId,
                        CPSW_ALE_IOCTL_ADD_UCAST, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("[ERR] %s: Failed to add unicast entry: %d\r\n",
                            __func__, status);
        status = ENET_EFAIL;
    }
    else
    {
        EnetAppUtils_print("[INF] %s: Added Unicast entry with MAC address: ",
                            __func__);
        EnetAppUtils_printMacAddr(macAddr);
        
        ENET_IOCTL_SET_NO_ARGS(&prms);

        if (isAgeable == true)
        {
            status = Enet_ioctl(EnetApp_inst.hEnet,
                                EnetApp_inst.coreId,
                                CPSW_ALE_IOCTL_DUMP_TABLE,
                                &prms);
        }

        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to print ALE Table \r\n");
        }
    }
    return status;
}

static int32_t EnetApp_addMcastEntry(uint8_t *macAddr)
{
    uint32_t setMcastOutArgs;
    CpswAle_SetMcastEntryInArgs setMcastInArgs;
    Enet_IoctlPrms prms;
    int32_t status = ENET_SOK;

    setMcastInArgs.addr.vlanId = 0U;
    setMcastInArgs.info.super = BFALSE;
    setMcastInArgs.info.fwdState   = CPSW_ALE_FWDSTLVL_FWD_LRN;
    setMcastInArgs.info.portMask   = CPSW_ALE_ALL_PORTS_MASK;
    setMcastInArgs.info.numIgnBits = 0U;

    EnetUtils_copyMacAddr(&setMcastInArgs.addr.addr[0U], macAddr);
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setMcastInArgs, &setMcastOutArgs);

    status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId,
                        CPSW_ALE_IOCTL_ADD_MCAST, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("[ERR] %s: Failed to add multicast entry: %d\r\n",
                           __func__, status);
        status = ENET_EFAIL;
    }
    if (status == ENET_SOK)
    {
        EnetAppUtils_print("[INF] %s: Added Multicast entry with MAC address: ",
                           __func__);
        EnetAppUtils_printMacAddr(macAddr);
        
        ENET_IOCTL_SET_NO_ARGS(&prms);

        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId,
                            CPSW_ALE_IOCTL_DUMP_TABLE, &prms);
    }

    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to print ALE Table \r\n");
    }
    return status;
}

static int32_t EnetApp_addOuiEntry(uint8_t *macaddr)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    uint32_t setOuiOutArgs;
    CpswAle_OuiEntryInfo setOuiInArgs;

    memcpy(&setOuiInArgs.ouiAddr[0], macaddr, sizeof(setOuiInArgs.ouiAddr));

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setOuiInArgs, &setOuiOutArgs);

    status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId, CPSW_ALE_IOCTL_ADD_OUI,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTest_setOUIEntry() failed : %d\n", status);
    }
    return status;
}

static int32_t EnetApp_addIPv4Entry(uint8_t *ipv4Addr)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    uint32_t setIpv4OutArgs;
    CpswAle_IPv4EntryInfo setIpv4InArgs;

    memcpy(&setIpv4InArgs.ipv4Addr[0], ipv4Addr, sizeof(setIpv4InArgs.ipv4Addr));
    setIpv4InArgs.numLSBIgnoreBits = 0U;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setIpv4InArgs, &setIpv4OutArgs);

    status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId,
                        CPSW_ALE_IOCTL_ADD_IPV4ADDR, &prms);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("[ERR] %s: Failed to add IPV4 entry: %d\r\n",
                           __func__, status);
        status = ENET_EFAIL;
    }
    else
    {
        EnetAppUtils_print("[INF] %s: Added IPV4 entry\r\n",
                           __func__);

        ENET_IOCTL_SET_NO_ARGS(&prms);

        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId,
                            CPSW_ALE_IOCTL_DUMP_TABLE, &prms);

        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to print ALE Table \r\n");
        }
    }
    return status;
}
