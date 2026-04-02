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
 * \file  cpsw_est.c
 *
 * \brief This file contains the implementation of the global configuration
 *        needed for EST feature.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x103

#include <stdint.h>
#include <stdarg.h>
#include <enet.h>
#include <csl_cpswitch.h>
#include <enet_cfg.h>
#include <priv/per/cpsw_priv.h>
#include <include/per/cpsw.h>

#if ENET_CFG_IS_ON(CPSW_EST)

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

static int32_t Cpsw_setupEstf(Cpsw_Handle hCpsw,
                              Enet_MacPort macPort,
                              uint64_t cycleTimeNs,
                              uint64_t baseTimeNs);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Cpsw_enableEst(EnetPer_Handle hPer)
{
    Cpsw_Handle hCpsw = (Cpsw_Handle)hPer;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hPer->virtAddr;
    CSL_CPSW_CONTROL controlReg;
    uint32_t i;

    for (i = 0U; i < ENET_ARRAYSIZE(hCpsw->estState); i++)
    {
        hCpsw->estState[i].cycleTime = 0U;
        hCpsw->estState[i].state = ENET_TAS_RESET;
    }

    /* Enable EST (global config).  Per-port EST enable control is set via TAS ioctl */
    CSL_CPSW_getCpswControlReg(regs, &controlReg);
    controlReg.estEnable = UTRUE;
    CSL_CPSW_setCpswControlReg(regs, &controlReg);
}

void Cpsw_disableEst(EnetPer_Handle hPer)
{
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hPer->virtAddr;
    CSL_CPSW_CONTROL controlReg;

    /* Disable EST (global config) */
    CSL_CPSW_getCpswControlReg(regs, &controlReg);
    controlReg.estEnable = UFALSE;
    CSL_CPSW_setCpswControlReg(regs, &controlReg);
}

int32_t Cpsw_ioctlEst(EnetPer_Handle hPer,
                      uint32_t cmd,
                      Enet_IoctlPrms *prms)
{
    Cpsw_Handle hCpsw = (Cpsw_Handle)hPer;
    /* Note: Typecast to GenericInArgs is possible because all public
     *       TAS IOCTL input args have macPort as their first member */
    EnetTas_GenericInArgs *genInArgs = (EnetTas_GenericInArgs *)prms->inArgs;
    Enet_MacPort macPort = genInArgs->macPort;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    int32_t status = ENET_SOK;

    switch (cmd)
    {
        case ENET_TAS_IOCTL_SET_ADMIN_LIST:
        {
            EnetTas_SetAdminListInArgs *inArgs = (EnetTas_SetAdminListInArgs *)prms->inArgs;

            if (hCpsw->estState[portNum].state == ENET_TAS_ENABLE)
            {
                if (inArgs->adminList.cycleTime != hCpsw->estState[portNum].cycleTime)
                { 
                    status = ENET_ENOTSUPPORTED;
                    ENETTRACE_ERR(status,
                                  "Port %u: Admin cycle time (%llu) and oper (%llu) must be same",
                                  ENET_MACPORT_ID(macPort),
                                  inArgs->adminList.cycleTime,
                                  hCpsw->estState[portNum].cycleTime);
                }
                else if (inArgs->adminList.baseTime != 0ULL)
                {
                    status = ENET_ENOTSUPPORTED;
                    ENETTRACE_ERR(status,
                                  "Port %u: AdminBasetime in the future is not supported, must pass 0",
                                  ENET_MACPORT_ID(macPort));
                }
            }

            /* CPSW MAC port will take care of writing new admin list to EST RAM */
            if (status == ENET_SOK)
            {
                status = EnetMod_ioctl(hCpsw->hMacPort[portNum], cmd, prms);
            }

            /* Save cycle time so CPTS ESTF can be setup accordingly */
            if (status == ENET_SOK)
            {
                hCpsw->estState[portNum].cycleTime     = inArgs->adminList.cycleTime;
                hCpsw->estState[portNum].adminBaseTime = inArgs->adminList.baseTime;
            }
        }
        break;

        case ENET_TAS_IOCTL_SET_STATE:
        {
            EnetTas_SetStateInArgs *inArgs = (EnetTas_SetStateInArgs *)prms->inArgs;

            if (hCpsw->estState[portNum].state != inArgs->state)
            {
                if (inArgs->state == ENET_TAS_ENABLE)
                {
                    /* Enable port-specific EST functionality */
                    status = EnetMod_ioctl(hCpsw->hMacPort[portNum], cmd, prms);

                    /* Enable ESTF with saved cycle time */
                    if (status == ENET_SOK)
                    {
                        status = Cpsw_setupEstf(hCpsw, macPort,
                                                hCpsw->estState[portNum].cycleTime,
                                                hCpsw->estState[portNum].adminBaseTime);
                        ENETTRACE_ERR_IF((status != ENET_SOK), status,
                                         "Port %u: Failed to enable ESTF",
                                         ENET_MACPORT_ID(macPort));
                    }
                }
                else
                {
                    /* Disable ESTF for that port */
                    status = Cpsw_setupEstf(hCpsw, macPort, 0U, 0U);
                    ENETTRACE_ERR_IF((status != ENET_SOK), status,
                                     "Port %u: Failed to disable ESTF",
                                     ENET_MACPORT_ID(macPort));

                    /* Disable (or reset) port-specific EST */
                    if (status == ENET_SOK)
                    {
                        status = EnetMod_ioctl(hCpsw->hMacPort[portNum], cmd, prms);
                    }
                }

                if (status == ENET_SOK)
                {
                    hCpsw->estState[portNum].state = inArgs->state;
                }
            }
        }
        break;

        case ENET_TAS_IOCTL_GET_ADMIN_LIST:
        {
            EnetTas_ControlList *controlList = (EnetTas_ControlList *)prms->outArgs;

            status = EnetMod_ioctl(hCpsw->hMacPort[portNum], cmd, prms);

            controlList->cycleTime = hCpsw->estState[portNum].cycleTime;
            controlList->baseTime  = hCpsw->estState[portNum].adminBaseTime;
        }
        break;

        case ENET_TAS_IOCTL_GET_OPER_LIST:
        {
            EnetTas_ControlList *controlList = (EnetTas_ControlList *)prms->outArgs;

            status = EnetMod_ioctl(hCpsw->hMacPort[portNum], cmd, prms);

            controlList->cycleTime = hCpsw->estState[portNum].cycleTime;
            controlList->baseTime  = hCpsw->estState[portNum].operBaseTime;
        }
        break;

        case ENET_TAS_IOCTL_GET_OPER_LIST_STATUS:
        {
            EnetTas_OperStatus *operStatus = (EnetTas_OperStatus *)prms->outArgs;

            /* Update operBaseTime when oper list has been updated */
            status = EnetMod_ioctl(hCpsw->hMacPort[portNum], cmd, prms);
            if ((status == ENET_SOK) &&
                (*operStatus == ENET_TAS_OPER_LIST_UPDATED))
            {
                hCpsw->estState[portNum].operBaseTime = hCpsw->estState[portNum].adminBaseTime;
            }
        }
        break;

        case ENET_TAS_IOCTL_GET_STATE:
        case ENET_TAS_IOCTL_CONFIG_CHANGE_STATUS_PARAMS:
        {
            /* Pass through */
            status = EnetMod_ioctl(hCpsw->hMacPort[portNum], cmd, prms);
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

static int32_t Cpsw_setupEstf(Cpsw_Handle hCpsw,
                              Enet_MacPort macPort,
                              uint64_t cycleTimeNs,
                              uint64_t baseTimeNs)
{
    Enet_IoctlPrms prms;
    CpswCpts_SetFxnGenInArgs setGenFInArgs;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t cptsClkPeriod;
    int32_t status;

    /* CPTS RFT clock frequency is an enumeration whose numeric value
     * represents the CPTS clock period - 1 */
    cptsClkPeriod = (uint32_t)hCpsw->cptsRftClkFreq + 1U;

    /* Enable ESTF for the given MAC port */
    setGenFInArgs.index   = portNum;
    setGenFInArgs.length  = (uint32_t)cycleTimeNs / cptsClkPeriod;
    setGenFInArgs.compare = baseTimeNs;
    setGenFInArgs.polarityInv = BFALSE;
    setGenFInArgs.ppmVal  = 0U;
    setGenFInArgs.ppmDir  = ENET_TIMESYNC_ADJDIR_INCREASE;
    setGenFInArgs.ppmMode = ENET_TIMESYNC_ADJMODE_DISABLE;

    ENET_IOCTL_SET_IN_ARGS(&prms, &setGenFInArgs);
    status = CpswCpts_ioctl(hCpsw->hCpts, CPSW_CPTS_IOCTL_SET_ESTF, &prms);

    return status;
}
#endif
