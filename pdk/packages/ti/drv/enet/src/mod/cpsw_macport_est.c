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
 * \file  cpsw_macport_est.c
 *
 * \brief This file contains the implementation of the CPSW EST functionality.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x208

#include <stdint.h>
#include <stdarg.h>
#include <csl_cpswitch.h>
#include <enet_cfg.h>
#include <include/core/enet_utils.h>
#include <include/core/enet_soc.h>
#include <include/mod/cpsw_macport.h>
#include <priv/core/enet_trace_priv.h>
#include <priv/mod/cpsw_macport_priv.h>
#include <include/per/cpsw_clks.h>

#if ENET_CFG_IS_ON(CPSW_MACPORT_EST)

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*! EST upper buffer location offset */
#define CPSW_MACPORT_EST_BUF_UPPER_LOC        (64U)

/*! EST fetch buffer size (per bank in two-buffer mode) */
#define CPSW_MACPORT_EST_BUF_SIZE             (64U)

/*! EST non-zero minimum time interval. Needed to guarantee that next fetch
 *  value has time to be fetched before current fetch count is over */
#define CPSW_MACPORT_EST_TIME_INTERVAL_MIN    (0x010U)

/*! EST maximum time interval (14-bit) */
#define CPSW_MACPORT_EST_TIME_INTERVAL_MAX    (0x3FFFU)

/*! Time interval step in nsecs for 1 Gbps link */
#define CPSW_MACPORT_EST_TIME_STEP_1G         (8U)

/*! Time interval step in nsecs for 100 Mbps link */
#define CPSW_MACPORT_EST_TIME_STEP_100M       (40U)

/*! Time interval step in nsecs for a 10 Mbps link */
#define CPSW_MACPORT_EST_TIME_STEP_10M        (400U)

/*! Min time interval in nsecs for given link speed */
#define CPSW_MACPORT_EST_TIME_MIN(speed)      (CPSW_MACPORT_EST_TIME_STEP_##speed * CPSW_MACPORT_EST_TIME_INTERVAL_MIN)

/*! Max time interval in nsecs for given link speed */
#define CPSW_MACPORT_EST_TIME_MAX(speed)      (CPSW_MACPORT_EST_TIME_STEP_##speed * CPSW_MACPORT_EST_TIME_INTERVAL_MAX)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t CpswEst_checkGateCmdList(CpswMacPort_Handle hPort,
                                        const EnetTas_ControlList *controlList);

static int32_t CpswEst_writeGateControlList(CpswMacPort_Handle hPort,
                                            const EnetTas_ControlList *controlList);

static int32_t CpswEst_readGateControlList(CpswMacPort_Handle hPort,
                                           bool estBufUpper,
                                           EnetTas_ControlList *controlList);

static void CpswEst_clearEstBuf(CpswMacPort_Handle hPort);

static EnetTas_TasState CpswEst_getState(CpswMacPort_Handle hPort);

static int32_t CpswEst_setState(CpswMacPort_Handle hPort,
                                EnetTas_TasState state);

static EnetTas_OperStatus CpswEst_getOperListStatus(CpswMacPort_Handle hPort);

static int32_t CpswEst_enableTimestamp(CpswMacPort_Handle hPort,
                                       const CpswMacPort_EstTimestampCfg *cfg);

static void CpswEst_disableTimestamp(CpswMacPort_Handle hPort);

static void CpswEst_getStatus(CpswMacPort_Handle hPort,
                              CpswMacPort_EstStatus *estStatus);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t CpswMacPort_openEst(EnetMod_Handle hMod)
{
    CpswMacPort_Handle hPort = (CpswMacPort_Handle)hMod;
    Enet_MacPort macPort = hPort->macPort;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    CSL_CPSW_EST_CONFIG estCfg;
    uint32_t portNum = ENET_MACPORT_NORM(macPort) + 1U;
    int32_t status;

    /* Initialize state and control lists. Clears both EST buffer banks
     * and set lower bank for first admin list */
    status = CpswEst_setState(hPort, ENET_TAS_RESET);
    ENETTRACE_ERR_IF((status != ENET_SOK), status,
                     "MAC %u: failed to reset EST", ENET_MACPORT_ID(macPort));

    /* Configure EST in two-buffer mode */
    memset(&estCfg, 0, sizeof(estCfg));
    estCfg.estBufSel = 0U;
    estCfg.estOneBuf = 0U;
    CSL_CPSW_setPortEstConfig(regs, portNum, &estCfg);

    return status;
}

int32_t CpswMacPort_ioctlEst(EnetMod_Handle hMod,
                             uint32_t cmd,
                             Enet_IoctlPrms *prms)
{
    CpswMacPort_Handle hPort = (CpswMacPort_Handle)hMod;
    Enet_MacPort macPort = hPort->macPort;
    uint32_t portId = ENET_MACPORT_ID(macPort);
    int32_t status = ENET_SOK;

    ENETTRACE_VAR(portId);

    switch (cmd)
    {
        case ENET_TAS_IOCTL_SET_ADMIN_LIST:
        {
            EnetTas_SetAdminListInArgs *inArgs = (EnetTas_SetAdminListInArgs *)prms->inArgs;
            EnetTas_ControlList *adminList = &inArgs->adminList;

            Enet_devAssert((macPort == inArgs->macPort),
                           "MAC %u: Port mismatch %u", portId, inArgs->macPort);

            status = CpswEst_writeGateControlList(hPort, adminList);
        }
        break;

        case ENET_TAS_IOCTL_GET_OPER_LIST_STATUS:
        {
            EnetTas_GenericInArgs *inArgs = (EnetTas_GenericInArgs *)prms->inArgs;
            EnetTas_OperStatus *operStatus = (EnetTas_OperStatus *)prms->outArgs;

            Enet_devAssert((macPort == inArgs->macPort),
                           "MAC %u: Port mismatch %u", portId, inArgs->macPort);

            *operStatus = CpswEst_getOperListStatus(hPort);
        }
        break;

        case ENET_TAS_IOCTL_SET_STATE:
        {
            EnetTas_SetStateInArgs *inArgs = (EnetTas_SetStateInArgs *)prms->inArgs;

            Enet_devAssert((macPort == inArgs->macPort),
                           "MAC %u: Port mismatch %u", portId, inArgs->macPort);

            status = CpswEst_setState(hPort, inArgs->state);
        }
        break;

        case ENET_TAS_IOCTL_GET_STATE:
        {
            EnetTas_GenericInArgs *inArgs = (EnetTas_GenericInArgs *)prms->inArgs;
            EnetTas_TasState *state = (EnetTas_TasState *)prms->outArgs;

            Enet_devAssert((macPort == inArgs->macPort),
                           "MAC %u: Port mismatch %u", portId, inArgs->macPort);

            *state = CpswEst_getState(hPort);
        }
        break;

        case ENET_TAS_IOCTL_GET_ADMIN_LIST:
        {
            EnetTas_GenericInArgs *inArgs = (EnetTas_GenericInArgs *)prms->inArgs;
            EnetTas_ControlList *adminList = (EnetTas_ControlList *)prms->outArgs;

            Enet_devAssert((macPort == inArgs->macPort),
                           "MAC %u: Port mismatch %u", portId, inArgs->macPort);

            memcpy(adminList, &hPort->adminList, sizeof(*adminList));
        }
        break;

        case ENET_TAS_IOCTL_GET_OPER_LIST:
        {
            EnetTas_GenericInArgs *inArgs = (EnetTas_GenericInArgs *)prms->inArgs;
            EnetTas_ControlList *operList = (EnetTas_ControlList *)prms->outArgs;

            Enet_devAssert((macPort == inArgs->macPort),
                           "MAC %u: Port mismatch %u", portId, inArgs->macPort);

            memcpy(operList, &hPort->operList, sizeof(*operList));
        }
        break;

        case ENET_TAS_IOCTL_CONFIG_CHANGE_STATUS_PARAMS:
        {
            EnetTas_GenericInArgs *inArgs = (EnetTas_GenericInArgs *)prms->inArgs;
            EnetTas_ConfigStatus *configChangeStatus = (EnetTas_ConfigStatus *)prms->outArgs;

            Enet_devAssert((macPort == inArgs->macPort),
                           "MAC %u: Port mismatch %u", portId, inArgs->macPort);

            memcpy(configChangeStatus, &hPort->configStatus, sizeof(*configChangeStatus));
        }
        break;

        case CPSW_MACPORT_IOCTL_EST_ENABLE_TIMESTAMP:
        {
            CpswMacPort_EstTimestampCfg *inArgs = (CpswMacPort_EstTimestampCfg *)prms->inArgs;

            Enet_devAssert((macPort == inArgs->macPort),
                           "MAC %u: Port mismatch %u", portId, inArgs->macPort);

            status = CpswEst_enableTimestamp(hPort, inArgs);
        }
        break;

        case CPSW_MACPORT_IOCTL_EST_DISABLE_TIMESTAMP:
        {
            EnetMacPort_GenericInArgs *inArgs = (EnetMacPort_GenericInArgs *)prms->inArgs;

            Enet_devAssert((macPort == inArgs->macPort),
                           "MAC %u: Port mismatch %u", portId, inArgs->macPort);

            CpswEst_disableTimestamp(hPort);
        }
        break;

        case CPSW_MACPORT_IOCTL_EST_GET_STATUS:
        {
            EnetMacPort_GenericInArgs *inArgs = (EnetMacPort_GenericInArgs *)prms->inArgs;
            CpswMacPort_EstStatus *outArgs = (CpswMacPort_EstStatus *)prms->outArgs;

            Enet_devAssert((macPort == inArgs->macPort),
                           "MAC %u: Port mismatch %u", portId, inArgs->macPort);

            CpswEst_getStatus(hPort, outArgs);
        }
        break;

        default:
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(status, "Invalid IOCTL cmd 0x%08x", cmd);
        }
        break;
    }

    return status;
}

static int32_t CpswEst_checkGateCmdList(CpswMacPort_Handle hPort,
                                        const EnetTas_ControlList *controlList)
{
    Enet_MacPort macPort = hPort->macPort;
    const EnetTas_GateCmdEntry *cmd;
    uint32_t listLength = controlList->listLength;
    uint32_t maxListLength;
    uint32_t timeIntvlMin;
    uint32_t timeIntvlMax;
    bool zeroIntvl = BFALSE;
    uint32_t i;
    int32_t status = ENET_SOK;

    ENETTRACE_VAR(macPort);

    switch (hPort->linkCfg.speed)
    {
        case ENET_SPEED_10MBIT:
            timeIntvlMin = CPSW_MACPORT_EST_TIME_MIN(10M);
            timeIntvlMax = CPSW_MACPORT_EST_TIME_MAX(10M);
            break;
        case ENET_SPEED_100MBIT:
            timeIntvlMin = CPSW_MACPORT_EST_TIME_MIN(100M);
            timeIntvlMax = CPSW_MACPORT_EST_TIME_MAX(100M);
            break;
        case ENET_SPEED_1GBIT:
        default:
            timeIntvlMin = CPSW_MACPORT_EST_TIME_MIN(1G);
            timeIntvlMax = CPSW_MACPORT_EST_TIME_MAX(1G);
            break;
    }

    /* Check that EST bank size is non-zero and is sufficient for admin list length */
    maxListLength = EnetUtils_min(ENET_TAS_MAX_CMD_LISTS, CPSW_MACPORT_EST_BUF_SIZE);
    if (listLength == 0U)
    {
        status = ENET_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "MAC %u: Invalid gate control list length (%u), can't be 0",
                      ENET_MACPORT_ID(macPort), listLength);
    }
    else if (listLength > maxListLength)
    {
        status = ENET_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "MAC %u: Invalid gate control list length (%u), CPSW max %u, LLD max %u",
                      ENET_MACPORT_ID(macPort), listLength,
                      CPSW_MACPORT_EST_BUF_SIZE, ENET_TAS_MAX_CMD_LISTS);
    }

    /* Check that all time intervals in admin list are valid */
    if (status == ENET_SOK)
    {
        for (i = 0U; i < listLength; i++)
        {
            cmd = &controlList->gateCmdList[i];

            if (cmd->timeInterval == 0U)
            {
                zeroIntvl = BTRUE;
            }
            else if (cmd->timeInterval < timeIntvlMin)
            {
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_ERR(status, "MAC %u: idx %u: Invalid time interval (%u ns), min is %u ns",
                              ENET_MACPORT_ID(macPort), i,
                              cmd->timeInterval,
                              timeIntvlMin);
            }
            else if (cmd->timeInterval > timeIntvlMax)
            {
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_ERR(status, "MAC %u: idx %u: Invalid time interval (%u ns), max is %u ns",
                              ENET_MACPORT_ID(macPort), i,
                              cmd->timeInterval,
                              timeIntvlMax);
            }
            else
            {
                if (zeroIntvl)
                {
                    status = ENET_EINVALIDPARAMS;
                    ENETTRACE_ERR(status, "MAC %u: idx %u: Non-zero interval can't follow a zero interval",
                                  ENET_MACPORT_ID(macPort), i);
                }
            }
        }
    }

    return status;
}

static int32_t CpswEst_writeGateControlList(CpswMacPort_Handle hPort,
                                            const EnetTas_ControlList *controlList)
{
    EnetMod_Handle hMod = ENET_MOD(hPort);
    Enet_MacPort macPort = hPort->macPort;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    CSL_CPSW_EST_CONFIG estCfg;
    const EnetTas_GateCmdEntry *cmd;
    uint32_t portNum = ENET_MACPORT_NORM(macPort) + 1U;
    uint32_t estBufLoc;
    uint32_t fetchCnt;
    uint32_t timeIntvlStep;
    uint64_t timeIntvlAcc = 0LU;
    uint8_t lastGateStateMask = 0U;
    uint32_t i;
    int32_t status = ENET_SOK;

    /* Link speed is needed to compute fetch count as it is in Ethernet wireside clocks
     * (bytes in 1000, nibbles in 10/100) */
    if (hPort->enabled)
    {
        switch (hPort->linkCfg.speed)
        {
            case ENET_SPEED_10MBIT:
                timeIntvlStep = CPSW_MACPORT_EST_TIME_STEP_10M;
                break;
            case ENET_SPEED_100MBIT:
                timeIntvlStep = CPSW_MACPORT_EST_TIME_STEP_100M;
                break;
            case ENET_SPEED_1GBIT:
            default:
                timeIntvlStep = CPSW_MACPORT_EST_TIME_STEP_1G;
                break;
        }
    }
    else
    {
        status = ENET_EPERM;
        ENETTRACE_ERR(status, "MAC %u: Admin list can be set only when link is up",
                      ENET_MACPORT_ID(macPort));
    }

    /* Check gate command list parameters passed from application */
    if (status == ENET_SOK)
    {
        status = CpswEst_checkGateCmdList(hPort, controlList);
        ENETTRACE_ERR_IF((status != ENET_SOK), status,
                         "MAC %u: Invalid gate control list params",
                         ENET_MACPORT_ID(macPort));
    }

    if (status == ENET_SOK)
    {
        /* Start offset depends on the admin EST bank */
        estBufLoc = hPort->estBufUpper ? CPSW_MACPORT_EST_BUF_UPPER_LOC : 0U;

        /* Write control list to EST fetch buffer */
        for (i = 0U; i < controlList->listLength; i++)
        {
            cmd = &controlList->gateCmdList[i];

            /* Stop processing the control list when a zero time interval is found */
            if (cmd->timeInterval == 0U)
            {
                break;
            }

            fetchCnt = ENET_DIV_ROUNDUP(cmd->timeInterval, timeIntvlStep);
            lastGateStateMask = cmd->gateStateMask;

            ENETTRACE_DBG("MAC %u: idx %u: loc %u: timeInterval=%u ns (%u) gateStateMask=0x%02x",
                          ENET_MACPORT_ID(macPort),
                          i, estBufLoc,
                          cmd->timeInterval,
                          fetchCnt,
                          cmd->gateStateMask);

            CSL_CPSW_writeEstFetchCmd(regs, portNum, estBufLoc,
                                      fetchCnt,
                                      cmd->gateStateMask);

            timeIntvlAcc += cmd->timeInterval;
            estBufLoc++;
        }

        /* Stretch last entry if cycle time is longer than total time intervals */
        if (timeIntvlAcc < controlList->cycleTime)
        {
            if (i < CPSW_MACPORT_EST_BUF_SIZE)
            {
                ENETTRACE_DBG("MAC %u: idx %u: loc %u: timeInterval=stretch gateStateMask=0x%02x",
                              ENET_MACPORT_ID(macPort),
                              i, estBufLoc,
                              lastGateStateMask);
                CSL_CPSW_writeEstFetchCmd(regs, portNum, estBufLoc, 0, lastGateStateMask);
                estBufLoc++;
                i++;
            }
        }

        /* Zero-out the remaining EST fetch buffer entries */
        for (; i < CPSW_MACPORT_EST_BUF_SIZE; i++)
        {
            CSL_CPSW_writeEstFetchCmd(regs, portNum, estBufLoc, 0U, 0U);
            estBufLoc++;
        }

        /* Select EST bank just populated as the active one */
        CSL_CPSW_getPortEstConfig(regs, portNum, &estCfg);
        estCfg.estBufSel = hPort->estBufUpper ? 1U : 0U;
        CSL_CPSW_setPortEstConfig(regs, portNum, &estCfg);

        /* Update local config status */
        hPort->configStatus.configChangeTime = controlList->baseTime;
        hPort->configStatus.configPending = 1U;
        hPort->configStatus.configChange  = 1U;
        if (hPort->state == ENET_TAS_ENABLE)
        {
            hPort->configStatus.configChangeErrorCounter++;
        }
        else if (hPort->state == ENET_TAS_RESET)
        {
            hPort->state = ENET_TAS_DISABLE;
        }

        /* Use other bank for next admin list */
        hPort->estBufUpper = !hPort->estBufUpper;
    }

    /* Save last successful admin list */
    if (status == ENET_SOK)
    {
        memcpy(&hPort->adminList, controlList, sizeof(hPort->adminList));
    }

    return status;
}

static int32_t CpswEst_readGateControlList(CpswMacPort_Handle hPort,
                                           bool estBufUpper,
                                           EnetTas_ControlList *controlList)
{
    EnetMod_Handle hMod = ENET_MOD(hPort);
    Enet_MacPort macPort = hPort->macPort;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    EnetTas_GateCmdEntry *cmd;
    uint32_t portNum = ENET_MACPORT_NORM(macPort) + 1U;
    uint32_t estBufLoc;
    uint32_t timeIntvlStep;
    uint32_t fetchCnt;
    uint8_t fetchAllow;
    uint32_t i;
    int32_t status = ENET_SOK;

    /* Link speed is needed to compute fetch count as it is in Ethernet wireside clocks
     * (bytes in 1000, nibbles in 10/100) */
    if (hPort->enabled)
    {
        switch (hPort->linkCfg.speed)
        {
            case ENET_SPEED_10MBIT:
                timeIntvlStep = CPSW_MACPORT_EST_TIME_STEP_10M;
                break;
            case ENET_SPEED_100MBIT:
                timeIntvlStep = CPSW_MACPORT_EST_TIME_STEP_100M;
                break;
            case ENET_SPEED_1GBIT:
            default:
                timeIntvlStep = CPSW_MACPORT_EST_TIME_STEP_1G;
                break;
        }
    }
    else
    {
        status = ENET_EPERM;
        ENETTRACE_ERR(status, "MAC %u: Oper list can be read only when link is up",
                      ENET_MACPORT_ID(macPort));
    }

    if (status == ENET_SOK)
    {
        if (ENET_TAS_MAX_CMD_LISTS > CPSW_MACPORT_EST_BUF_SIZE)
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(status, "MAC %u: Invalid gate control list length (%u), max is %u",
                          ENET_MACPORT_ID(macPort), ENET_TAS_MAX_CMD_LISTS, CPSW_MACPORT_EST_BUF_SIZE);
        }
    }

    /* Read control list from EST fetch buffer */
    if (status == ENET_SOK)
    {
        estBufLoc = estBufUpper ? CPSW_MACPORT_EST_BUF_UPPER_LOC : 0U;

        for (i = 0U; i < CPSW_MACPORT_EST_BUF_SIZE; i++)
        {
            CSL_CPSW_readEstFetchCmd(regs, portNum, estBufLoc,
                                     &fetchCnt,
                                     &fetchAllow);

            if ((fetchCnt == 0U) &&
                (fetchAllow == 0U))
            {
                controlList->listLength = i;
                break;
            }

            if (i == ENET_TAS_MAX_CMD_LISTS)
            {
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_WARN("MAC %u: Partial list, length %u not enough",
                               ENET_MACPORT_ID(macPort), ENET_TAS_MAX_CMD_LISTS);
                break;
            }

            cmd = &controlList->gateCmdList[i];
            cmd->timeInterval  = fetchCnt * timeIntvlStep;
            cmd->gateStateMask = fetchAllow;

            ENETTRACE_DBG("MAC %u: idx %u: loc %u: timeInterval=%u ns (%u) gateStateMask=0x%02x",
                          ENET_MACPORT_ID(macPort),
                          i, estBufLoc,
                          cmd->timeInterval,
                          fetchCnt,
                          cmd->gateStateMask);

            estBufLoc++;
        }
    }

    return status;
}

static void CpswEst_clearEstBuf(CpswMacPort_Handle hPort)
{
    EnetMod_Handle hMod = ENET_MOD(hPort);
    Enet_MacPort macPort = hPort->macPort;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    uint32_t portNum = ENET_MACPORT_NORM(macPort) + 1U;
    uint32_t i;

    ENETTRACE_DBG("MAC %u: Clearing both EST buffers", ENET_MACPORT_ID(macPort));

    for (i = 0U; i < (2U * CPSW_MACPORT_EST_BUF_SIZE); i++)
    {
        CSL_CPSW_writeEstFetchCmd(regs, portNum, i, 0U, 0U);
    }

    memset(&hPort->adminList, 0, sizeof(hPort->adminList));
    memset(&hPort->operList, 0, sizeof(hPort->operList));
}

static EnetTas_TasState CpswEst_getState(CpswMacPort_Handle hPort)
{
    EnetMod_Handle hMod = ENET_MOD(hPort);
    Enet_MacPort macPort = hPort->macPort;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    CSL_CPSW_PORT_CONTROL portControl;
    uint32_t portNum = ENET_MACPORT_NORM(macPort) + 1U;
    EnetTas_TasState state;

    CSL_CPSW_getPortControlReg(regs, portNum, &portControl);
    state = (portControl.estPortEnable != 0U) ? ENET_TAS_ENABLE : ENET_TAS_DISABLE;

    return state;
}

static int32_t CpswEst_setState(CpswMacPort_Handle hPort,
                                EnetTas_TasState state)
{
    EnetMod_Handle hMod = ENET_MOD(hPort);
    Enet_MacPort macPort = hPort->macPort;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    CSL_CPSW_PORT_CONTROL portControl;
    CSL_CPSW_EST_CONFIG estCfg;
    uint32_t portNum = ENET_MACPORT_NORM(macPort) + 1U;
    int32_t status = ENET_SOK;

    switch (state)
    {
        case ENET_TAS_ENABLE:
        {
            /* Allow EST enable only if a valid admin list has been set before */
            if ((hPort->state == ENET_TAS_RESET) ||
                (hPort->operList.listLength == 0U))
            {
                status = ENET_EPERM;
                ENETTRACE_ERR(status, "MAC %u: EST can be enabled only after a valid admin list is set",
                              ENET_MACPORT_ID(macPort));
            }

            if (status == ENET_SOK)
            {
                ENETTRACE_DBG("MAC %u: Set EST state: ENABLE", ENET_MACPORT_ID(macPort));

                /* Enable EST on the port */
                CSL_CPSW_getPortControlReg(regs, portNum, &portControl);
                portControl.estPortEnable = 1U;
                CSL_CPSW_setPortControlReg(regs, portNum, &portControl);
            }
            break;
        }

        case ENET_TAS_DISABLE:
        {
            ENETTRACE_DBG("MAC %u: Set EST state: DISABLE", ENET_MACPORT_ID(macPort));

            /* Disable EST on the port */
            CSL_CPSW_getPortControlReg(regs, portNum, &portControl);
            portControl.estPortEnable = 0U;
            CSL_CPSW_setPortControlReg(regs, portNum, &portControl);
            break;
        }

        case ENET_TAS_RESET:
        {
            ENETTRACE_DBG("MAC %u: Set EST state: RESET", ENET_MACPORT_ID(macPort));

            memset(&hPort->configStatus, 0, sizeof(hPort->configStatus));

            /* Disable EST on the port */
            CSL_CPSW_getPortControlReg(regs, portNum, &portControl);
            portControl.estPortEnable = 0U;
            CSL_CPSW_setPortControlReg(regs, portNum, &portControl);

            /* Clear both EST buffer banks */
            CpswEst_clearEstBuf(hPort);

            /* Next admin bank is EST lower buffer */
            hPort->estBufUpper = BFALSE;
            CSL_CPSW_getPortEstConfig(regs, portNum, &estCfg);
            estCfg.estBufSel = hPort->estBufUpper ? 1U : 0U;
            CSL_CPSW_setPortEstConfig(regs, portNum, &estCfg);
            break;
        }

        default:
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_WARN("MAC %u: Invalid state %u", ENET_MACPORT_ID(macPort), state);
            break;
        }
    }

    if (status == ENET_SOK)
    {
        hPort->state = state;
    }

    return status;
}

static EnetTas_OperStatus CpswEst_getOperListStatus(CpswMacPort_Handle hPort)
{
    EnetMod_Handle hMod = ENET_MOD(hPort);
    Enet_MacPort macPort = hPort->macPort;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    EnetTas_OperStatus operStatus = ENET_TAS_OPER_LIST_NOT_YET_UPDATED;
    CSL_CPSW_PORT_CONTROL portControl;
    CSL_CPGMAC_SL_FIFOSTATUS fifoStatus;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    bool operEstBufUpper;
    bool curEstBufUpper;
    int32_t status = ENET_SOK;

    ENETTRACE_VAR(status);

    if (hPort->configStatus.configChange == 1U)
    {
        /* Oper EST bank is the opposite bank of next admin bank (estBufUpper) */
        operEstBufUpper = !hPort->estBufUpper;

        CSL_CPSW_getPortControlReg(regs, portNum + 1U, &portControl);
        if (portControl.estPortEnable == 0U)
        {
            /* Active EST buffer can't be checked when EST is disabled */
            curEstBufUpper = operEstBufUpper;
            ENETTRACE_DBG("MAC %u: Ignoring active buffer bank check as EST is disabled",
                          ENET_MACPORT_ID(macPort));
        }
        else
        {
            /* Get active EST buffer for the MAC port */
            CSL_CPGMAC_SL_getFifoStatus(regs, portNum, &fifoStatus);
            curEstBufUpper = (fifoStatus.estBufAct == 1U);

            ENETTRACE_DBG("MAC %u: Active buffer bank: %s, target oper bank: %s",
                          ENET_MACPORT_ID(macPort),
                          curEstBufUpper ? "LOWER" : "UPPER",
                          operEstBufUpper ? "LOWER" : "UPPER");
        }

        /* Check if active EST buffer is the 'oper' EST bank */
        if (curEstBufUpper == operEstBufUpper)
        {
            operStatus = ENET_TAS_OPER_LIST_UPDATED;

            hPort->operList.cycleTime = hPort->adminList.cycleTime;
            hPort->configStatus.configPending = 0U;
            hPort->configStatus.configChange  = 0U;

            status = CpswEst_readGateControlList(hPort, operEstBufUpper, &hPort->operList);
            ENETTRACE_WARN_IF((status != ENET_SOK),
                              "MAC %u: Failed to save oper list",
                              ENET_MACPORT_ID(macPort));
        }
        else
        {
            operStatus = ENET_TAS_OPER_LIST_NOT_YET_UPDATED;
        }

        /* TODO - Check error status in FIFO_STATUS_REG */
    }

    return operStatus;
}

static int32_t CpswEst_enableTimestamp(CpswMacPort_Handle hPort,
                                       const CpswMacPort_EstTimestampCfg *cfg)
{
    EnetMod_Handle hMod = ENET_MOD(hPort);
    Enet_MacPort macPort = hPort->macPort;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    CSL_CPSW_EST_CONFIG estCfg;
    uint32_t portNum = ENET_MACPORT_NORM(macPort) + 1U;
    int32_t status = ENET_SOK;

    CSL_CPSW_getPortEstConfig(regs, portNum, &estCfg);

    switch (cfg->mode)
    {
        case CPSW_MACPORT_EST_TIMESTAMP_ALL:
        {
            ENETTRACE_DBG("MAC %u: Enable timestamping on all packets",
                          ENET_MACPORT_ID(macPort));

            /* All express packets on any priority */
            estCfg.estTsFirst  = 0U;
            estCfg.estTsOnePri = 0U;
            estCfg.estTsPri    = 0U;
            break;
        }

        case CPSW_MACPORT_EST_TIMESTAMP_ONEPRI:
        {
            ENETTRACE_DBG("MAC %u: Enable timestamping on all packets of priority %u",
                          ENET_MACPORT_ID(macPort), cfg->priority);

            /* All packet of the given priority */
            if (cfg->priority <= ENET_PRI_MAX)
            {
                estCfg.estTsFirst  = 0U;
                estCfg.estTsOnePri = 1U;
                estCfg.estTsPri    = cfg->priority;
            }
            else
            {
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_ERR(status, "MAC %u: Invalid timestamping priority %u",
                              ENET_MACPORT_ID(macPort), cfg->priority);
            }
            break;
        }

        case CPSW_MACPORT_EST_TIMESTAMP_FIRST:
        {
            ENETTRACE_DBG("MAC %u: Enable timestamping on first packet of each time interval",
                          ENET_MACPORT_ID(macPort));

            /* First packet of each time interval */
            estCfg.estTsFirst  = 1U;
            estCfg.estTsOnePri = 0U;
            estCfg.estTsPri    = 0U;
            break;
        }

        case CPSW_MACPORT_EST_TIMESTAMP_FIRST_ONEPRI:
        {
            ENETTRACE_DBG("MAC %u: Enable timestamping on first packet of priority %u",
                          ENET_MACPORT_ID(macPort), cfg->priority);

            /* First packet of the given priority */
            if (cfg->priority <= ENET_PRI_MAX)
            {
                estCfg.estTsFirst  = 1U;
                estCfg.estTsOnePri = 1U;
                estCfg.estTsPri    = cfg->priority;
            }
            else
            {
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_ERR(status, "MAC %u: Invalid timestamping priority %u",
                              ENET_MACPORT_ID(macPort), cfg->priority);
            }
            break;
        }

        default:
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(status, "MAC %u: Invalid EST timestamping mode %u",
                          ENET_MACPORT_ID(macPort), cfg->mode);
            break;
        }
    }

    if (status == ENET_SOK)
    {
        CSL_CPSW_setEstTsDomain(regs, cfg->domain);

        estCfg.estTsEnable = 1U;
        CSL_CPSW_setPortEstConfig(regs, portNum, &estCfg);
    }

    return status;
}

static void CpswEst_disableTimestamp(CpswMacPort_Handle hPort)
{
    EnetMod_Handle hMod = ENET_MOD(hPort);
    Enet_MacPort macPort = hPort->macPort;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    CSL_CPSW_EST_CONFIG estCfg;
    uint32_t portNum = ENET_MACPORT_NORM(macPort) + 1U;

    ENETTRACE_DBG("MAC %u: Enable timestamping on first packet of each time interval",
                  ENET_MACPORT_ID(macPort));

    CSL_CPSW_getPortEstConfig(regs, portNum, &estCfg);

    estCfg.estTsFirst  = 0U;
    estCfg.estTsOnePri = 0U;
    estCfg.estTsPri    = 0U;
    estCfg.estTsEnable = 1U;

    CSL_CPSW_setPortEstConfig(regs, portNum, &estCfg);
}

static void CpswEst_getStatus(CpswMacPort_Handle hPort,
                              CpswMacPort_EstStatus *estStatus)
{
    EnetMod_Handle hMod = ENET_MOD(hPort);
    Enet_MacPort macPort = hPort->macPort;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t fifoStatus;

    /* TODO - Replace with CSL-FR function */
    fifoStatus = regs->ENETPORT[portNum].PN_FIFO_STATUS_REG;
    estStatus->activeBuffer     = CSL_FEXT(fifoStatus, XGE_CPSW_PN_FIFO_STATUS_REG_EST_BUFACT);
    estStatus->addrErr          = CSL_FEXT(fifoStatus, XGE_CPSW_PN_FIFO_STATUS_REG_EST_ADD_ERR);
    estStatus->fetchCntErr      = CSL_FEXT(fifoStatus, XGE_CPSW_PN_FIFO_STATUS_REG_EST_CNT_ERR);
    estStatus->macAllow         = CSL_FEXT(fifoStatus, XGE_CPSW_PN_FIFO_STATUS_REG_TX_E_MAC_ALLOW);
    estStatus->txPriorityActive = CSL_FEXT(fifoStatus, XGE_CPSW_PN_FIFO_STATUS_REG_TX_PRI_ACTIVE);
}
#endif
