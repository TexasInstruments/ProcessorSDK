/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 * \file     cpsw_test_bcastmcast_limit.c
 *
 * \brief    This file contains the cpsw_test_BCASTMCAST_LIMIT test implementation.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_ethutils.h>

#include "enet_test_base.h"
#include "enet_test_entry.h"
#include "cpsw_test_bcastmcast_limit.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define ENET_TEST_BCASTMCAST_LIMIT_INGRESS_PORT    (ENET_MAC_PORT_4)
#define ENET_TEST_BCASTMCAST_LIMIT_EGRESS_PORT     (ENET_MAC_PORT_3)

#define ENET_TEST_BCASTMCAST_LIMIT_MCASTLIMIT_INPORT_NUMPKTSPERSEC      (40000)
#define ENET_TEST_BCASTMCAST_LIMIT_BCASTLIMIT_INPORT_NUMPKTSPERSEC      (20000)

#define ENET_TEST_BCASTMCAST_LIMIT_MCASTLIMIT_OUTPORT_NUMPKTSPERSEC     (60000)
#define ENET_TEST_BCASTMCAST_LIMIT_BCASTLIMIT_OUTPORT_NUMPKTSPERSEC     (30000)

#define ENET_TEST_BCASTMCAST_LIMIT_RATELIMIT_AT_TX                      (BTRUE)


/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static int32_t EnetTestBcastMcastLimit_Config(EnetTestTaskObj *taskObj);

static int32_t EnetTestBcastMcastLimit_AddAleEntry(EnetTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
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
int32_t EnetTestBcastMcastLimit_Run(EnetTestTaskObj *taskObj)
{
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    status = EnetTestBcastMcastLimit_Config(taskObj);

    if (status == ENET_SOK)
    {
        status = EnetTestBcastMcastLimit_AddAleEntry(taskObj);
    }

    if (ENET_SOK == status)
    {
        Enet_IoctlPrms prms;

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_TABLE,
                            &prms);
       EnetAppUtils_assert(status == ENET_SOK);

        status = EnetTestCommon_Run(taskObj);
    }

    return status;
}

/* TODO - Prescale should be part of CpswAle_Cfg which is set by the UT app */
#define CPSW_ALE_CFG_BCAST_MCAST_PRESCALE       (250000U)

static uint32_t EnetTestBcastMcastLimit_mapNumPktsPerSec2RateLimit(EnetTestTaskObj *taskObj,
                                                                   uint32_t numPktsPerSec)
{
    int32_t status = ENET_EFAIL;
    uint32_t alePrescale;
    uint32_t preScalerPulseFreq;
    uint32_t cppiClkFreqHz;

    alePrescale = CPSW_ALE_CFG_BCAST_MCAST_PRESCALE;
    if (alePrescale == 0)
    {
        alePrescale = 1;
    }

    cppiClkFreqHz = EnetSoc_getClkFreq(taskObj->taskCfg->enetType, taskObj->taskCfg->instId, CPSW_CPPI_CLK);
    preScalerPulseFreq = cppiClkFreqHz / alePrescale;
    status = ((numPktsPerSec / preScalerPulseFreq) * preScalerPulseFreq);

    return status;
}


static int32_t EnetTestBcastMcastLimit_Config(EnetTestTaskObj *taskObj)
{
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    Enet_IoctlPrms prms;
    CpswAle_SetBcastMcastRateLimitInArgs setInArgs;
    CpswAle_GetBcastMcastRateLimitOutArgs getOutArgs;
    int32_t status;

    /* CPSW_ALE_IOCTL_DISABLE_BCAST_MCAST_LIMIT
     */
    memset(&setInArgs, 0, sizeof(setInArgs));
    setInArgs.numPorts = 2;
    setInArgs.rateLimitAtTxPort = ENET_TEST_BCASTMCAST_LIMIT_RATELIMIT_AT_TX;

    /* BCAST/MCAST Rate Limit for Ingress Port */
    setInArgs.portPrms[0].portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_BCASTMCAST_LIMIT_INGRESS_PORT);
    setInArgs.portPrms[0].bcastRateLimitForPortEn = BTRUE;
    setInArgs.portPrms[0].mcastRateLimitForPortEn = BTRUE;
    setInArgs.portPrms[0].mcastLimitNumPktsPerSec = ENET_TEST_BCASTMCAST_LIMIT_MCASTLIMIT_INPORT_NUMPKTSPERSEC;
    setInArgs.portPrms[0].bcastLimitNumPktsPerSec = ENET_TEST_BCASTMCAST_LIMIT_BCASTLIMIT_INPORT_NUMPKTSPERSEC;

    /* BCAST/MCAST Rate Limit for Egress Port */
    setInArgs.portPrms[1].portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_BCASTMCAST_LIMIT_EGRESS_PORT);
    setInArgs.portPrms[1].bcastRateLimitForPortEn = BTRUE;
    setInArgs.portPrms[1].mcastRateLimitForPortEn = BTRUE;
    setInArgs.portPrms[1].mcastLimitNumPktsPerSec = ENET_TEST_BCASTMCAST_LIMIT_MCASTLIMIT_OUTPORT_NUMPKTSPERSEC;
    setInArgs.portPrms[1].bcastLimitNumPktsPerSec = ENET_TEST_BCASTMCAST_LIMIT_BCASTLIMIT_OUTPORT_NUMPKTSPERSEC;


    ENET_IOCTL_SET_IN_ARGS(&prms, &setInArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId,
                        CPSW_ALE_IOCTL_SET_BCAST_MCAST_LIMIT,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTestBcastMcastLimit_Config() failed CPSW_ALE_IOCTL_SET_BCAST_MCAST_LIMIT: %d\n",
                           status);
    }

    if (status == ENET_SOK)
    {
        memset(&getOutArgs, 0, sizeof(getOutArgs));
        ENET_IOCTL_SET_OUT_ARGS(&prms, &getOutArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId,
                            CPSW_ALE_IOCTL_GET_BCAST_MCAST_LIMIT,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestBcastMcastLimit_Config() failed ENET_MACPORT_IOCTL_GET_BCASTMCAST_LIMIT: %d\n",
                               status);
        }
    }
    /* Compare set and retrieved Bcast Mcast Rate Limit configuration */
    if (status == ENET_SOK)
    {
       EnetAppUtils_assert(getOutArgs.numPorts == 2);
       EnetAppUtils_assert(getOutArgs.rateLimitEn == BTRUE);
       EnetAppUtils_assert(getOutArgs.rateLimitAtTxPort == ENET_TEST_BCASTMCAST_LIMIT_RATELIMIT_AT_TX);

       EnetAppUtils_assert(getOutArgs.portPrms[0].mcastRateLimitForPortEn == BTRUE);
       EnetAppUtils_assert(getOutArgs.portPrms[0].bcastRateLimitForPortEn == BTRUE);
       EnetAppUtils_assert((getOutArgs.portPrms[0].portNum == CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_BCASTMCAST_LIMIT_INGRESS_PORT))
                            ||
                            (getOutArgs.portPrms[0].portNum == CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_BCASTMCAST_LIMIT_EGRESS_PORT)));
        if (getOutArgs.portPrms[0].portNum == CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_BCASTMCAST_LIMIT_INGRESS_PORT))
        {
           EnetAppUtils_assert(getOutArgs.portPrms[0].bcastLimitNumPktsPerSec == EnetTestBcastMcastLimit_mapNumPktsPerSec2RateLimit(taskObj, ENET_TEST_BCASTMCAST_LIMIT_BCASTLIMIT_INPORT_NUMPKTSPERSEC));
           EnetAppUtils_assert(getOutArgs.portPrms[0].mcastLimitNumPktsPerSec == EnetTestBcastMcastLimit_mapNumPktsPerSec2RateLimit(taskObj, ENET_TEST_BCASTMCAST_LIMIT_MCASTLIMIT_INPORT_NUMPKTSPERSEC));

        }
        else
        {
           EnetAppUtils_assert(getOutArgs.portPrms[0].portNum == CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_BCASTMCAST_LIMIT_EGRESS_PORT));
           EnetAppUtils_assert(getOutArgs.portPrms[0].bcastLimitNumPktsPerSec == EnetTestBcastMcastLimit_mapNumPktsPerSec2RateLimit(taskObj, ENET_TEST_BCASTMCAST_LIMIT_BCASTLIMIT_OUTPORT_NUMPKTSPERSEC));
           EnetAppUtils_assert(getOutArgs.portPrms[0].mcastLimitNumPktsPerSec == EnetTestBcastMcastLimit_mapNumPktsPerSec2RateLimit(taskObj, ENET_TEST_BCASTMCAST_LIMIT_MCASTLIMIT_OUTPORT_NUMPKTSPERSEC));
        }

       EnetAppUtils_assert(getOutArgs.portPrms[1].mcastRateLimitForPortEn == BTRUE);
       EnetAppUtils_assert(getOutArgs.portPrms[1].bcastRateLimitForPortEn == BTRUE);
       EnetAppUtils_assert((getOutArgs.portPrms[1].portNum == CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_BCASTMCAST_LIMIT_INGRESS_PORT))
                            ||
                            (getOutArgs.portPrms[1].portNum == CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_BCASTMCAST_LIMIT_EGRESS_PORT)));
        if (getOutArgs.portPrms[1].portNum == CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_BCASTMCAST_LIMIT_INGRESS_PORT))
        {
           EnetAppUtils_assert(getOutArgs.portPrms[1].bcastLimitNumPktsPerSec == EnetTestBcastMcastLimit_mapNumPktsPerSec2RateLimit(taskObj, ENET_TEST_BCASTMCAST_LIMIT_BCASTLIMIT_INPORT_NUMPKTSPERSEC));
           EnetAppUtils_assert(getOutArgs.portPrms[1].mcastLimitNumPktsPerSec == EnetTestBcastMcastLimit_mapNumPktsPerSec2RateLimit(taskObj, ENET_TEST_BCASTMCAST_LIMIT_MCASTLIMIT_INPORT_NUMPKTSPERSEC));

        }
        else
        {
           EnetAppUtils_assert(getOutArgs.portPrms[1].portNum == CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_BCASTMCAST_LIMIT_EGRESS_PORT));
           EnetAppUtils_assert(getOutArgs.portPrms[1].bcastLimitNumPktsPerSec == EnetTestBcastMcastLimit_mapNumPktsPerSec2RateLimit(taskObj, ENET_TEST_BCASTMCAST_LIMIT_BCASTLIMIT_OUTPORT_NUMPKTSPERSEC));
           EnetAppUtils_assert(getOutArgs.portPrms[1].mcastLimitNumPktsPerSec == EnetTestBcastMcastLimit_mapNumPktsPerSec2RateLimit(taskObj, ENET_TEST_BCASTMCAST_LIMIT_MCASTLIMIT_OUTPORT_NUMPKTSPERSEC));
        }
    }

    return status;
}

static int32_t EnetTestBcastMcastLimit_AddAleEntry(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setMcastOutArgs;
    CpswAle_SetMcastEntryInArgs setMcastInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memset(&setMcastInArgs, 0, sizeof(setMcastInArgs));
    memcpy(&setMcastInArgs.addr.addr[0U], testMCastAddr,
           sizeof(setMcastInArgs.addr.addr));
    setMcastInArgs.addr.vlanId  = 0U;
    setMcastInArgs.info.super      = BFALSE;
    setMcastInArgs.info.numIgnBits = 8U;
    setMcastInArgs.info.fwdState = CPSW_ALE_FWDSTLVL_FWD;
    setMcastInArgs.info.portMask = EnetTestCommon_getMacPortAleMask(taskObj);
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setMcastInArgs, &setMcastOutArgs);
    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_MCAST,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTestBcastMcastLimit_AddAleEntry() failed CPSW_ALE_IOCTL_ADD_MCAST: %d\n",
                           status);
    }
    if (status == ENET_SOK)
    {
        memset(&setMcastInArgs, 0, sizeof(setMcastInArgs));
        memcpy(&setMcastInArgs.addr.addr[0U], testBCastAddr,
               sizeof(setMcastInArgs.addr.addr));
        setMcastInArgs.addr.vlanId  = 0U;
        setMcastInArgs.info.super      = BFALSE;
        setMcastInArgs.info.numIgnBits = 8U;
        setMcastInArgs.info.fwdState = CPSW_ALE_FWDSTLVL_FWD;
        setMcastInArgs.info.portMask = EnetTestCommon_getMacPortAleMask(taskObj);
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &setMcastInArgs, &setMcastOutArgs);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_MCAST, &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestBcastMcastLimit_AddAleEntry() failed CPSW_ALE_IOCTL_ADD_MCAST: %d\n",
                               status);
        }
    }
    return status;
}

void EnetTestBcastMcastLimit_setOpenPrms(EnetTestTaskObj *taskObj,
                                        Cpsw_Cfg *pCpswCfg,
                                        EnetUtils_Cfg *pUtilsPrms)
{

    pCpswCfg->aleCfg.policerGlobalCfg.policingEn = BTRUE;
    pCpswCfg->hostPortCfg.passPriorityTaggedUnchanged   = BTRUE;

    pCpswCfg->aleCfg.modeFlags                             = CPSW_ALE_CFG_MODULE_EN;
    pCpswCfg->aleCfg.vlanCfg.unknownUnregMcastFloodMask = 0x0;
    pCpswCfg->aleCfg.vlanCfg.unknownRegMcastFloodMask   = EnetTestCommon_getMacPortAleMask(taskObj);
    pCpswCfg->aleCfg.vlanCfg.unknownVlanMemberListMask  = (EnetTestCommon_getMacPortAleMask(taskObj) | CPSW_ALE_HOST_PORT_MASK);
    pCpswCfg->aleCfg.vlanCfg.aleVlanAwareMode           = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.autoLearnWithVlan          = BFALSE;
    pCpswCfg->aleCfg.vlanCfg.cpswVlanAwareMode          = BFALSE;
    pCpswCfg->aleCfg.nwSecCfg.vid0ModeEn                = BFALSE;

    pCpswCfg->vlanCfg.vlanAware                         = BFALSE;

    pCpswCfg->aleCfg.policerGlobalCfg.policingEn         = BFALSE;
    pCpswCfg->aleCfg.policerGlobalCfg.yellowDropEn       = BFALSE;
    pCpswCfg->aleCfg.policerGlobalCfg.redDropEn          = BFALSE;
    pCpswCfg->aleCfg.policerGlobalCfg.policerNoMatchMode = CPSW_ALE_POLICER_NOMATCH_MODE_GREEN;
}


