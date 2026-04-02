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
 * \file  rate_limit.c
 *
 * \brief This file contains scripts to add unicast entry to ALE
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "cli_common.h"
#include "rate_limit.h"

#include "FreeRTOS_CLI.h"


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define CPSW_ALE_CFG_BCAST_MCAST_PRESCALE       (250000U)
#define DEFAULT_NUM_PACKETS_PER_SECOND          -1
#define DEFAULT_IS_EGRESS_PORT                  BFALSE
#define DEFAULT_IS_INGRESS_PORT                 BFALSE
#define DEFAULT_IS_READ_ONLY                    BTRUE
#define DEFAULT_MAC_PORT_NUM                    255

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct Enetcli_rl_config_s
{
    /* True if macport provided is supposed to be treated as an ingress port */
    bool isIngress;

    /* True if macport provided is supposed to be treated as an egress port */
    bool isEgress;
    
    /* MAC port number provided by user */
    uint8_t macPortNum;
    
    /* Rate of Number of Packets per second, provided by the user */
    uint32_t rate;

    /* True if user just wants to read stats for the existing 
    * rate limits of a particular port.
    * False if user wants to set a new rate limit for a port */
    bool isReadOnly;

    /* All packet rates must be a multiple of this prescaler pulse frequency */
    uint32_t preScalerPulseFreq;

} Enetcli_rl_config;

typedef struct EnetTestMacPortList_s
{
    uint32_t numMacPorts;

    Enet_MacPort macPortList[ENET_MAC_PORT_NUM];
} EnetTestMacPortList_t;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t EnetApp_addUcastEntry(uint8_t *macAddr);

static BaseType_t Enetcli_config(char *writeBuffer, 
                                 size_t writeBufferLen,
                                 const char *commandString);

static uint32_t Enetcli_mapNumPktsPerSec2RateLimit(uint32_t numPktsPerSec);

static void  Enetcli_getMacPortList(uint32_t macPortBitMask,
                                    EnetTestMacPortList_t *macPortList);

static int32_t Enetcli_AddAleEntry();

static int32_t Enetcli_setMcastRateLimit();

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

Enetcli_rl_config Enetcli_config_obj;

static uint8_t testMCastAddr[ENET_MAC_ADDR_LEN] =
{
    0x01, 0xFF, 0xFF, 0xFF, 0x00, 0x00
};

static uint8_t testBCastAddr[ENET_MAC_ADDR_LEN] =
{
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

BaseType_t Enetcli_ratelmt(char *writeBuffer,
                           size_t writeBufferLen,
                           const char *commandString)
{
    int32_t status = ENET_SOK;
    BaseType_t config_status;

    /* Pre-Initialization of config params*/
    Enetcli_config_obj.rate = DEFAULT_NUM_PACKETS_PER_SECOND;
    Enetcli_config_obj.isEgress = DEFAULT_IS_EGRESS_PORT;
    Enetcli_config_obj.isIngress = DEFAULT_IS_INGRESS_PORT;
    Enetcli_config_obj.isReadOnly = DEFAULT_IS_READ_ONLY;
    Enetcli_config_obj.macPortNum = DEFAULT_MAC_PORT_NUM;

    /* This will set the configuration */
    config_status = Enetcli_config(writeBuffer,
                                   writeBufferLen,
                                   commandString);

    if (config_status == pdFALSE)
    {
        return config_status;
    }

    if (Enetcli_config_obj.isReadOnly == BFALSE)
    {
        status = Enetcli_setMcastRateLimit();
        if (ENET_SOK == status)
        {
            status = Enetcli_AddAleEntry();
            if (ENET_SOK == status)
            {
                Enet_IoctlPrms prms;

                ENET_IOCTL_SET_NO_ARGS(&prms);
                status = Enet_ioctl(EnetApp_inst.hEnet, 
                                    EnetApp_inst.coreId, 
                                    CPSW_ALE_IOCTL_DUMP_TABLE,
                                    &prms);
                EnetAppUtils_assert(status == ENET_SOK);
            }
            else
            {
                EnetAppUtils_print("Not able to add entry to ALE table\n");
            }
        }
    }

    else
    {
        Enet_IoctlPrms prms;
        ENET_IOCTL_SET_NO_ARGS(&prms);
        CpswAle_GetBcastMcastRateLimitOutArgs getOutArgs;
        memset(&getOutArgs, 0, sizeof(getOutArgs));
        ENET_IOCTL_SET_OUT_ARGS(&prms, &getOutArgs);

        status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId,
                            CPSW_ALE_IOCTL_GET_BCAST_MCAST_LIMIT,
                            &prms);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("EnetTestBcastMcastLimit_Config() failed "
                               "ENET_MACPORT_IOCTL_GET_BCASTMCAST_LIMIT: %d\n",
                               status);
        }
        if(status == ENET_SOK)
        {
            uint32_t i = 0;
            if (getOutArgs.numPorts > 0)
            {
                EnetAppUtils_print("\nPrinting statistics for a total of "
                                   "%d ports with rate limiting enabled:\n",
                                   getOutArgs.numPorts);
                for (i=0; i<getOutArgs.numPorts; i++)
                {
                    EnetAppUtils_print("\nPort number: %d\n", 
                                        getOutArgs.portPrms[i].portNum);
                    EnetAppUtils_print("Existing rate limit value: %d\n", 
                                        getOutArgs.portPrms[i].bcastLimitNumPktsPerSec);
                }
            }
            else
            {
                EnetAppUtils_print("No ports have rate limiting enabled.\n");
            }
        }
    }

    return status;
}

/* ========================================================================== */
/*                   Static Function Definitions                              */
/* ========================================================================== */

/* Packets will be sent once every prescale pulse. This prescale pulse frequency
*  is much less than the CPU clock frequency and the prescale register holds this
*  value by which we must divide the clock frequency to get the frequency of 
*  prescale pulse. */
static uint32_t Enetcli_mapNumPktsPerSec2RateLimit(uint32_t numPktsPerSec)
{
    int32_t status = ENET_EFAIL;
    uint32_t alePrescale;
    uint32_t preScalerPulseFreq;
    uint32_t cppiClkFreqHz;

    /* If the prescale register is not set, we assume that the prescale pulse
    *  frequency will be equal to the CPU clock frequency. */
    alePrescale = CPSW_ALE_CFG_BCAST_MCAST_PRESCALE;
    if (alePrescale == 0)
    {
        alePrescale = 1;
    }
    
    /* Get the clock frequency. */
    cppiClkFreqHz = EnetSoc_getClkFreq(EnetApp_inst.enetType,
                                       EnetApp_inst.instId,
                                       CPSW_CPPI_CLK);

    /* Get the prescale pulse frequency. */
    preScalerPulseFreq = cppiClkFreqHz / alePrescale;

    /* To confirm whether the number of packets is  multiple of the
    *  prescale pulse frequency or not, we return the status here
    *  and check whether the value provided by the user as
    *  "numPktsPerSec" is valid or not. We check whether that value
    *  is a multiple of the prescaler pulse frequency or not */
    status = ((numPktsPerSec / preScalerPulseFreq) * preScalerPulseFreq);
    Enetcli_config_obj.preScalerPulseFreq = preScalerPulseFreq;

    return status;
}

static void  Enetcli_getMacPortList(uint32_t macPortBitMask,
                                    EnetTestMacPortList_t *macPortList)
{
    Enet_MacPort portIndex;
    uint32_t numMacPorts = 0U;

    for (portIndex = ENET_MAC_PORT_FIRST; portIndex <= ENET_MAC_PORT_NUM; portIndex++)
    {
        if (ENET_BIT(ENET_MACPORT_NORM(portIndex)) & macPortBitMask)
        {
           EnetAppUtils_assert(numMacPorts < ENET_ARRAYSIZE(macPortList->macPortList));
            macPortList->macPortList[numMacPorts] = portIndex;
            numMacPorts++;
        }
    }

    macPortList->numMacPorts = numMacPorts;
}

static int32_t Enetcli_setMcastRateLimit()
{
    uint32_t macPortNum = Enetcli_config_obj.macPortNum;
    uint32_t rate = Enetcli_config_obj.rate;
    Enet_IoctlPrms prms;
    CpswAle_SetBcastMcastRateLimitInArgs setInArgs;
    int32_t status;

    /* CPSW_ALE_IOCTL_DISABLE_BCAST_MCAST_LIMIT
     */
    memset(&setInArgs, 0, sizeof(setInArgs));
    setInArgs.numPorts = 1U;

    if (Enetcli_config_obj.isEgress)
    {
        setInArgs.rateLimitAtTxPort = 1U;
        
    }
    /* BCAST/MCAST Rate Limit for Ingress Port */
    setInArgs.portPrms[0].portNum = macPortNum;
    setInArgs.portPrms[0].bcastRateLimitForPortEn = BTRUE;
    setInArgs.portPrms[0].mcastRateLimitForPortEn = BTRUE;
    setInArgs.portPrms[0].mcastLimitNumPktsPerSec = rate;
    setInArgs.portPrms[0].bcastLimitNumPktsPerSec = rate;


    ENET_IOCTL_SET_IN_ARGS(&prms, &setInArgs);

    status = Enet_ioctl(EnetApp_inst.hEnet, EnetApp_inst.coreId,
                        CPSW_ALE_IOCTL_SET_BCAST_MCAST_LIMIT,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTestBcastMcastLimit_Config() failed CPSW_ALE_IOCTL_SET_BCAST_MCAST_LIMIT: %d\n",
                           status);
    }

    return status;
}

static BaseType_t Enetcli_config(char *writeBuffer, size_t writeBufferLen,
        const char *commandString)
{
    char *parameter;
    BaseType_t paramLen;
    uint32_t paramCnt = 1;
    
    parameter = (char*) FreeRTOS_CLIGetParameter(commandString, paramCnt,
            &paramLen);

    while (parameter != NULL)
    {
        /* Checking for the -r parameter which is the rate limit in number of packets per second.
        * If rate limit is passed by user, then the macport will be set to that rate.
        * If rate is not passed, just print the existing value of rate at that macport number */

        if (strncmp("-r", parameter, paramLen) == 0)
        {
            parameter = (char*) FreeRTOS_CLIGetParameter(commandString, paramCnt+1,
                &paramLen);
            uint32_t numPktsPerSecond = atoi(parameter);
            
            uint32_t correctedRate = Enetcli_mapNumPktsPerSec2RateLimit(numPktsPerSecond);
            if (correctedRate == numPktsPerSecond)
            {
                Enetcli_config_obj.rate = numPktsPerSecond;
                Enetcli_config_obj.isReadOnly = BFALSE;
                paramCnt += 2;
            }
            else
            {
                snprintf(writeBuffer, 
                         writeBufferLen, 
                         "Invalid value of -r. Provided value %d is not a multiple of %d\r\n", 
                         numPktsPerSecond, 
                         Enetcli_config_obj.preScalerPulseFreq);
                return pdFALSE;
            }
        }

        /* Checking for the -p parameter which is the macport number. Mandatory to be passed by user */
        else if (strncmp("-p", parameter, paramLen) == 0)
        {
            parameter = (char*) FreeRTOS_CLIGetParameter(commandString, paramCnt+1,
                &paramLen);

            uint8_t macPortNum = atoi(parameter);
            if ((macPortNum == 1) || (macPortNum == 3))
            {
                Enetcli_config_obj.macPortNum = macPortNum;
                paramCnt += 2;
            }
            else
            {
                snprintf(writeBuffer, writeBufferLen, "Invalid Parameter for -p (mac port number should be either 1 or 3 only)\r\n");
                return pdFALSE;
            }
        }

        /* Checking for the -d parameter which is 0 for ingress and 1 for egress. Mandatory to be passed by user */
        else if (strncmp("-d", parameter, paramLen) == 0)
        {
            parameter = (char*) FreeRTOS_CLIGetParameter(commandString, paramCnt+1,
            &paramLen);
            uint32_t paramValue = atoi(parameter);

            if (paramValue == 0)
            {
                Enetcli_config_obj.isIngress = BTRUE;
            }
            else if (paramValue == 1)
            {
                Enetcli_config_obj.isEgress = BTRUE;
            }
            else
            {
                snprintf(writeBuffer, 
                         writeBufferLen, 
                         "Invalid Parameter for -d (0 for ingress and 1 for egress)\r\n");
                return pdFALSE;
            }
            paramCnt += 2;
        }

        parameter = (char*) FreeRTOS_CLIGetParameter(commandString, paramCnt,
        &paramLen);
    }

    if( (Enetcli_config_obj.isEgress == BFALSE) && (Enetcli_config_obj.isIngress == BFALSE) )
    {
        snprintf(writeBuffer, 
                 writeBufferLen, 
                 "Invalid Parameter for -d (0 for ingress and 1 for egress)\r\n");
        return pdFALSE;
    }

    if(Enetcli_config_obj.macPortNum == 255)
    {
        snprintf(writeBuffer, 
                 writeBufferLen, 
                 "Invalid Parameter for -p (mac port number should be provided)\r\n");
        return pdFALSE;
    }
    return pdTRUE;
}

static int32_t Enetcli_AddAleEntry()
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setMcastOutArgs;
    CpswAle_SetMcastEntryInArgs setMcastInArgs;

    memset(&setMcastInArgs, 0, sizeof(setMcastInArgs));
    memcpy(&setMcastInArgs.addr.addr[0U], &testMCastAddr, sizeof(setMcastInArgs.addr.addr));
    setMcastInArgs.addr.vlanId  = 0U;
    setMcastInArgs.info.super      = BFALSE;
    setMcastInArgs.info.numIgnBits = 0U;
    setMcastInArgs.info.fwdState = CPSW_ALE_FWDSTLVL_FWD;
    setMcastInArgs.info.portMask = CPSW_ALE_ALL_PORTS_MASK;
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setMcastInArgs, &setMcastOutArgs);
    status = Enet_ioctl(EnetApp_inst.hEnet, 
                        EnetApp_inst.coreId, 
                        CPSW_ALE_IOCTL_ADD_MCAST,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("Enetcli_AddAleEntry() failed CPSW_ALE_IOCTL_ADD_MCAST: %d\n",
                           status);
    }
    if (status == ENET_SOK)
    {
        memset(&setMcastInArgs, 0, sizeof(setMcastInArgs));
        memcpy(&setMcastInArgs.addr.addr[0U], testBCastAddr,
               sizeof(setMcastInArgs.addr.addr));
        setMcastInArgs.addr.vlanId  = 0U;
        setMcastInArgs.info.super      = BFALSE;
        setMcastInArgs.info.numIgnBits = 0U;
        setMcastInArgs.info.fwdState = CPSW_ALE_FWDSTLVL_FWD;
        setMcastInArgs.info.portMask = CPSW_ALE_ALL_PORTS_MASK;
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &setMcastInArgs, &setMcastOutArgs);
        status = Enet_ioctl(EnetApp_inst.hEnet, 
                            EnetApp_inst.coreId, 
                            CPSW_ALE_IOCTL_ADD_MCAST, 
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("Enetcli_AddAleEntry() failed CPSW_ALE_IOCTL_ADD_MCAST: %d\n",
                               status);
        }
    }
    return status;
}
