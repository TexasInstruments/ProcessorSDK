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
 * \file     cpsw_test_traffic_shaping.c
 *
 * \brief    This file contains the cpsw_test_traffic_shaping test implementation.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <ti/drv/uart/UART_stdio.h>
#include <ti/csl/csl_cpswitch.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_ethutils.h>

#include <ti/osal/osal.h>

#include <ti/board/board.h>

#include "enet_test_base.h"
#include "enet_test_entry.h"
#include "cpsw_test_traffic_shaping.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define ETH_TEST_MAC_PORT_VLAN_ID_BASE                        (100U)
#define ETH_TEST_HOST_PORT_VLAN_ID                            (200U)

#define ENET_TEST_TRAFFIC_SHAPING_MBPS(x)                     ((x) * 1000000U)
#define ENET_TEST_TRAFFIC_SHAPING_KBPS(x)                     ((x) * 1000U)

#define ENET_TEST_TRAFFIC_SHAPING_EIR_PRI0                    ENET_TEST_TRAFFIC_SHAPING_KBPS(50)
#define ENET_TEST_TRAFFIC_SHAPING_CIR_PRI0                    ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)

#define ENET_TEST_TRAFFIC_SHAPING_EIR_PRI1                    ENET_TEST_TRAFFIC_SHAPING_KBPS(50)
#define ENET_TEST_TRAFFIC_SHAPING_CIR_PRI1                    ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)

#define ENET_TEST_TRAFFIC_SHAPING_EIR_PRI2                    ENET_TEST_TRAFFIC_SHAPING_KBPS(50)
#define ENET_TEST_TRAFFIC_SHAPING_CIR_PRI2                    ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)

#define ENET_TEST_TRAFFIC_SHAPING_EIR_PRI3                    ENET_TEST_TRAFFIC_SHAPING_KBPS(50)
#define ENET_TEST_TRAFFIC_SHAPING_CIR_PRI3                    ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)

#define ENET_TEST_TRAFFIC_SHAPING_EIR_PRI4                    ENET_TEST_TRAFFIC_SHAPING_KBPS(50)
#define ENET_TEST_TRAFFIC_SHAPING_CIR_PRI4                    ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)

#define ENET_TEST_TRAFFIC_SHAPING_EIR_PRI5                    ENET_TEST_TRAFFIC_SHAPING_KBPS(50)
#define ENET_TEST_TRAFFIC_SHAPING_CIR_PRI5                    ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)

#define ENET_TEST_TRAFFIC_SHAPING_EIR_PRI6                    ENET_TEST_TRAFFIC_SHAPING_KBPS(50)
#define ENET_TEST_TRAFFIC_SHAPING_CIR_PRI6                    ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)

#define ENET_TEST_TRAFFIC_SHAPING_EIR_PRI7                    ENET_TEST_TRAFFIC_SHAPING_KBPS(50)
#define ENET_TEST_TRAFFIC_SHAPING_CIR_PRI7                    ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI0            ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI0            ENET_TEST_TRAFFIC_SHAPING_MBPS(3)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI1            ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI1            ENET_TEST_TRAFFIC_SHAPING_MBPS(3)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI2            ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI2            ENET_TEST_TRAFFIC_SHAPING_MBPS(3)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI3            ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI3            ENET_TEST_TRAFFIC_SHAPING_MBPS(3)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI4            ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI4            ENET_TEST_TRAFFIC_SHAPING_MBPS(3)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI5            ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI5            ENET_TEST_TRAFFIC_SHAPING_MBPS(3)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI6            ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI6            ENET_TEST_TRAFFIC_SHAPING_MBPS(3)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI7            ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI7            ENET_TEST_TRAFFIC_SHAPING_MBPS(3)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI0          ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI0          ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI1          ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI1          ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI2          ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI2          ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI3          ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI3          ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI4          ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI4          ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI5          ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI5          ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI6          ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI6          ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)

#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI7          ENET_TEST_TRAFFIC_SHAPING_MBPS(12.5)
#define ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI7          ENET_TEST_TRAFFIC_SHAPING_MBPS(6.25)

#define ENET_TEST_TRAFFIC_SHAPING_INGRESS_PORT                (ENET_MAC_PORT_4)
#define ENET_TEST_TRAFFIC_SHAPING_EGRESS_PORT                 (ENET_MAC_PORT_3)

#define ENET_TEST_TRAFFIC_SHAPING_ENET_RATE_LIMIT_DIVFACTOR   (32768)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
static int32_t EnetTestTrafficShaping_Config(EnetTestTaskObj *taskObj);

static int32_t EnetTestTrafficShaping_setRxPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestTrafficShaping_setRxDscpPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestTrafficShaping_setPolicerPriority(EnetTestTaskObj *taskObj);

static int32_t EnetTestTrafficShaping_setPolicerPriorityDstIpv4(EnetTestTaskObj *taskObj);

static int32_t EnetTestTrafficShaping_AddAleEntry(EnetTestTaskObj *taskObj);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static EnetPort_TrafficShapingRates gEnetTestTrafficShaping_priorityRate[ENET_PRI_NUM] =
{
    [0] = {.committedRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_CIR_PRI0, .excessRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_EIR_PRI0},
    [1] = {.committedRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_CIR_PRI1, .excessRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_EIR_PRI1},
    [2] = {.committedRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_CIR_PRI2, .excessRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_EIR_PRI2},
    [3] = {.committedRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_CIR_PRI3, .excessRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_EIR_PRI3},
    [4] = {.committedRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_CIR_PRI4, .excessRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_EIR_PRI4},
    [5] = {.committedRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_CIR_PRI5, .excessRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_EIR_PRI5},
    [6] = {.committedRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_CIR_PRI6, .excessRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_EIR_PRI6},
    [7] = {.committedRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_CIR_PRI7, .excessRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_EIR_PRI7},
};

typedef struct EnetTestTrafficShapingAleRate_s
{
    uint32_t committedInfoRateBitsPerSec;
    uint32_t peakInfoRateBitsPerSec;
} EnetTestTrafficShapingAleRate_t;

static EnetTestTrafficShapingAleRate_t gEnetTestTrafficShaping_priorityAleRate[ENET_PRI_NUM] =
{
    [0] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI0, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI0},
    [1] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI1, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI1},
    [2] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI2, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI2},
    [3] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI3, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI3},
    [4] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI4, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI4},
    [5] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI5, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI5},
    [6] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI6, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI6},
    [7] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_CIR_PRI7, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_PRI_PIR_PRI7},
};

static EnetTestTrafficShapingAleRate_t gEnetTestTrafficShaping_ipDstAleRate[ENET_PRI_NUM] =
{
    [0] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI0, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI0},
    [1] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI1, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI1},
    [2] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI2, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI2},
    [3] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI3, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI3},
    [4] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI4, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI4},
    [5] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI5, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI5},
    [6] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI6, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI6},
    [7] = {.committedInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_CIR_PRI7, .peakInfoRateBitsPerSec = ENET_TEST_TRAFFIC_SHAPING_ALE_IPDST_PIR_PRI7},
};

static uint8_t testEgressPortDestAddr[ENET_MAC_ADDR_LEN] =
{
    0x08, 0x00, 0x00, 0x00, 0x00, 0x02
};

static uint8_t testDestIpv4Addr[ENET_IPv4_ADDR_LEN] =
{
    128, 24, 190, 64
};

static uint8_t testSrcAddr[ENET_MAC_ADDR_LEN] =
{
    0x02, 0x00, 0x00, 0x00, 0x00, 0x08
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t EnetTestTrafficShaping_Run(EnetTestTaskObj *taskObj)
{
    int32_t status             = ENET_SOK;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    status = EnetTestTrafficShaping_Config(taskObj);
    if (status == ENET_SOK)
    {
        status = EnetTestTrafficShaping_AddAleEntry(taskObj);
    }

    if (status == ENET_SOK)
    {
        status = EnetTestTrafficShaping_setRxPriority(taskObj);
    }

    if (status == ENET_SOK)
    {
        status = EnetTestTrafficShaping_setRxDscpPriority(taskObj);
    }

    if (status == ENET_SOK)
    {
        status = EnetTestTrafficShaping_setPolicerPriority(taskObj);
    }

    if (status == ENET_SOK)
    {
        status = EnetTestTrafficShaping_setPolicerPriorityDstIpv4(taskObj);
    }

    if (ENET_SOK == status)
    {
        Enet_IoctlPrms prms;

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_TABLE,
                            &prms);
       EnetAppUtils_assert(status == ENET_SOK);

        ENET_IOCTL_SET_NO_ARGS(&prms);
        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_DUMP_POLICER_ENTRIES,
                            &prms);

       EnetAppUtils_assert(status == ENET_SOK);

        status = EnetTestCommon_Run(taskObj);
    }

    return status;
}

static int32_t EnetTestTrafficShaping_Config(EnetTestTaskObj *taskObj)
{
    int32_t status = ENET_EFAIL;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    Enet_IoctlPrms prms;
    EnetMacPort_EnableEgressTrafficShapingInArgs setInArgs;
    EnetMacPort_GenericInArgs getInArgs;
    EnetPort_TrafficShapingCfg getOutArgs;
    uint32_t cppiClkFreqHz;
    uint32_t cir1;
    uint32_t cir2;
    uint32_t eir1;
    uint32_t eir2;
    uint32_t i;
    uint32_t roundingError;
    int32_t eirDiff;
    int32_t cirDiff;

    cppiClkFreqHz = EnetSoc_getClkFreq(taskObj->taskCfg->enetType, taskObj->taskCfg->instId, CPSW_CPPI_CLK);
    roundingError = cppiClkFreqHz / ENET_TEST_TRAFFIC_SHAPING_ENET_RATE_LIMIT_DIVFACTOR;

    /* Set traffic shaping CIR/EIR rates for all priorities */
    setInArgs.macPort = ENET_TEST_TRAFFIC_SHAPING_EGRESS_PORT;
    memcpy(&setInArgs.trafficShapingCfg.rates[0U],
           &gEnetTestTrafficShaping_priorityRate[0U],
           sizeof(setInArgs.trafficShapingCfg.rates));
    ENET_IOCTL_SET_IN_ARGS(&prms, &setInArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId,
                        ENET_MACPORT_IOCTL_ENABLE_EGRESS_TRAFFIC_SHAPING,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTestTrafficShaping_Config() failed ENET_MACPORT_IOCTL_ENABLE_EGRESS_TRAFFIC_SHAPING: %d\n",
                           status);
    }

    /* Get traffic shaping rates from the driver */
    if (status == ENET_SOK)
    {
        getInArgs.macPort = ENET_TEST_TRAFFIC_SHAPING_EGRESS_PORT;
        ENET_IOCTL_SET_INOUT_ARGS(&prms, &getInArgs, &getOutArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId,
                            ENET_MACPORT_IOCTL_GET_EGRESS_TRAFFIC_SHAPING,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestTrafficShaping_Config() failed ENET_MACPORT_IOCTL_GET_EGRESS_TRAFFIC_SHAPING: %d\n",
                               status);
        }
    }

    /* Compare set and retrieved traffic shaping configuration */
    if (status == ENET_SOK)
    {
       EnetAppUtils_assert(setInArgs.macPort == getInArgs.macPort);

        for (i = 0U; i < ENET_PRI_NUM; i++)
        {
            cir1 = setInArgs.trafficShapingCfg.rates[i].committedRateBitsPerSec;
            eir1 = setInArgs.trafficShapingCfg.rates[i].excessRateBitsPerSec;
            cir2 = getOutArgs.rates[i].committedRateBitsPerSec;
            eir2 = getOutArgs.rates[i].excessRateBitsPerSec;
            eirDiff = eir1 - eir2;
            cirDiff = cir1 - cir2;

           EnetAppUtils_assert(abs(cirDiff) <= roundingError);
           EnetAppUtils_assert(abs(eirDiff) <= roundingError);

            if (cir1 != cir2)
            {
               EnetAppUtils_print("Traffic Shaping CIR configured to %u due to rounding error\n",
                                   cir2);
            }

            if (eir1 != eir2)
            {
               EnetAppUtils_print("Traffic Shaping EIR configured to %u due to rounding error\n",
                                   eir2);
            }
        }
    }

    return status;
}

static int32_t EnetTestTrafficShaping_AddAleEntry(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    uint32_t setUcastOutArgs;
    CpswAle_SetUcastEntryInArgs setUcastInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;

    memset(&setUcastInArgs, 0, sizeof(setUcastInArgs));
    memcpy(&setUcastInArgs.addr.addr[0U], testEgressPortDestAddr,
           sizeof(setUcastInArgs.addr.addr));
    setUcastInArgs.addr.vlanId  = 0U;
    setUcastInArgs.info.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_TRAFFIC_SHAPING_EGRESS_PORT);
    setUcastInArgs.info.blocked = BFALSE;
    setUcastInArgs.info.secure  = BFALSE;
    setUcastInArgs.info.super   = BFALSE;
    setUcastInArgs.info.ageable = BFALSE;
    setUcastInArgs.info.trunk   = BFALSE;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &setUcastInArgs, &setUcastOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_ADD_UCAST,
                        &prms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTestTrafficShaping_AddAleEntry() failed CPSW_ALE_IOCTL_ADD_UCAST: %d\n",
                           status);
    }

    return status;
}

void EnetTestTrafficShaping_setOpenPrms(EnetTestTaskObj *taskObj,
                                        Cpsw_Cfg *pCpswCfg,
                                        EnetUtils_Cfg *pUtilsPrms)
{
    Enet_MacPort i;

    pCpswCfg->aleCfg.policerGlobalCfg.policingEn = BTRUE;
    pCpswCfg->hostPortCfg.passPriorityTaggedUnchanged   = BTRUE;

    pCpswCfg->aleCfg.modeFlags                             = CPSW_ALE_CFG_MODULE_EN;
    pCpswCfg->aleCfg.vlanCfg.unknownUnregMcastFloodMask = 0x0;
    pCpswCfg->aleCfg.vlanCfg.unknownRegMcastFloodMask   = 0x0;
    pCpswCfg->aleCfg.vlanCfg.unknownVlanMemberListMask  = CPSW_ALE_ALL_PORTS_MASK;
    pCpswCfg->aleCfg.vlanCfg.aleVlanAwareMode           = BTRUE;
    pCpswCfg->aleCfg.vlanCfg.autoLearnWithVlan          = BFALSE;
    pCpswCfg->aleCfg.vlanCfg.cpswVlanAwareMode          = BTRUE;
    pCpswCfg->aleCfg.nwSecCfg.vid0ModeEn                = BTRUE;

    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].learningCfg.noLearn              = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].vlanCfg.dropUntagged             = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.unregMcastFloodMask      = 0x0;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.regMcastFloodMask        = CPSW_ALE_ALL_PORTS_MASK;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.forceUntaggedEgressMask  = 0U;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.noLearnMask              = 0x0;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.vidIngressCheck          = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.limitIPNxtHdr            = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.disallowIPFrag           = BFALSE;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.vlanIdInfo.vlanId        = ETH_TEST_HOST_PORT_VLAN_ID;
    pCpswCfg->aleCfg.portCfg[CPSW_ALE_HOST_PORT_NUM].pvidCfg.vlanMemberList           = CPSW_ALE_ALL_PORTS_MASK;

    for (i = ENET_MAC_PORT_FIRST; i < CPSW_ALE_NUM_MAC_PORTS; i++)
    {
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].learningCfg.noLearn              = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].vlanCfg.dropUntagged             = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.unregMcastFloodMask      = 0x0;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.regMcastFloodMask        = CPSW_ALE_ALL_PORTS_MASK;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.forceUntaggedEgressMask  = 0U;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.noLearnMask              = 0x0;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vidIngressCheck          = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.limitIPNxtHdr            = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.disallowIPFrag           = BFALSE;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vlanIdInfo.tagType       = ENET_VLAN_TAG_TYPE_INNER;
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vlanIdInfo.vlanId        = ETH_TEST_MAC_PORT_VLAN_ID_BASE + ENET_MACPORT_NORM(i);
        pCpswCfg->aleCfg.portCfg[CPSW_ALE_MACPORT_TO_ALEPORT(i)].pvidCfg.vlanMemberList           = CPSW_ALE_ALL_PORTS_MASK;
    }

    pCpswCfg->hostPortCfg.vlanCfg.portPri = 7U;
    pCpswCfg->hostPortCfg.vlanCfg.portCfi = 0U;
    pCpswCfg->hostPortCfg.vlanCfg.portVID = ETH_TEST_HOST_PORT_VLAN_ID;
    pCpswCfg->vlanCfg.vlanAware           = BTRUE;

    pCpswCfg->aleCfg.policerGlobalCfg.policingEn     = BTRUE;
    pCpswCfg->aleCfg.policerGlobalCfg.yellowDropEn   = BFALSE;
    pCpswCfg->aleCfg.policerGlobalCfg.redDropEn      = BTRUE;
    pCpswCfg->aleCfg.policerGlobalCfg.policerNoMatchMode = CPSW_ALE_POLICER_NOMATCH_MODE_GREEN;
}

void EnetTestTrafficShaping_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                              Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg             = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    macCfg->loopbackEn                  = BFALSE;
    macCfg->passPriorityTaggedUnchanged = BTRUE;

    macCfg->vlanCfg.portPri = ENET_MACPORT_NORM(portNum);
    macCfg->vlanCfg.portCfi = 0U;
    macCfg->vlanCfg.portVID = ETH_TEST_MAC_PORT_VLAN_ID_BASE + ENET_MACPORT_NORM(portNum);
}

static int32_t EnetTestTrafficShaping_setRxDscpPriority(EnetTestTaskObj *taskObj)
{
    EnetPort_DscpPriorityMap setHostInArgs;
    EnetPort_DscpPriorityMap getHostOutArgs;
    Enet_IoctlPrms hostPrms;
    EnetMacPort_SetIngressDscpPriorityMapInArgs setMacInArgs;
    EnetMacPort_GenericInArgs getMacInArgs;
    EnetPort_DscpPriorityMap getMacOutArgs;
    Enet_IoctlPrms macPrms;
    uint32_t i;
    int32_t status;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint32_t portIter;
    EnetTestMacPortList_t enabledPorts;

    setHostInArgs.dscpIPv4En = BTRUE;
    setHostInArgs.dscpIPv6En = BTRUE;
    for (i = 0U; i < ENET_ARRAYSIZE(setHostInArgs.tosMap); i++)
    {
        setHostInArgs.tosMap[i] = (i / 8U);
    }

    ENET_IOCTL_SET_IN_ARGS(&hostPrms, &setHostInArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_HOSTPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP,
                        &hostPrms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTestTrafficShaping_setRxDscpPriority() failed ENET_HOSTPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP: %d\n",
                           status);
    }

    ENET_IOCTL_SET_OUT_ARGS(&hostPrms, &getHostOutArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_HOSTPORT_IOCTL_GET_INGRESS_DSCP_PRI_MAP,
                        &hostPrms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTestTrafficShaping_setRxDscpPriority() failed ENET_HOSTPORT_IOCTL_GET_INGRESS_DSCP_PRI_MAP: %d\n",
                           status);
    }
    else
    {
       EnetAppUtils_assert(setHostInArgs.dscpIPv4En ==
                            getHostOutArgs.dscpIPv4En);
       EnetAppUtils_assert(setHostInArgs.dscpIPv6En ==
                            getHostOutArgs.dscpIPv6En);
        for (i = 0U; i < ENET_ARRAYSIZE(setHostInArgs.tosMap); i++)
        {
           EnetAppUtils_assert(setHostInArgs.tosMap[i] ==
                                getHostOutArgs.tosMap[i]);
        }
    }

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
    /* Set Mac Port DSCP priority mapping for all mac ports */
    for (portIter = 0U; portIter < enabledPorts.numMacPorts; portIter++)
    {
        setMacInArgs.macPort                     = enabledPorts.macPortList[portIter];
        setMacInArgs.dscpPriorityMap.dscpIPv4En = BTRUE;
        setMacInArgs.dscpPriorityMap.dscpIPv6En = BTRUE;
        for (i = 0U; i < ENET_ARRAYSIZE(setMacInArgs.dscpPriorityMap.tosMap); i++)
        {
            setMacInArgs.dscpPriorityMap.tosMap[i] = (i / 8U);
        }

        ENET_IOCTL_SET_IN_ARGS(&macPrms, &setMacInArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_MACPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP,
                            &macPrms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestTrafficShaping_setRxDscpPriority() failed ENET_MACPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP: %d\n",
                               status);
        }

        getMacInArgs.macPort = enabledPorts.macPortList[portIter];
        ENET_IOCTL_SET_INOUT_ARGS(&macPrms, &getMacInArgs, &getMacOutArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_MACPORT_IOCTL_GET_INGRESS_DSCP_PRI_MAP,
                            &macPrms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestTrafficShaping_setRxDscpPriority() failed ENET_MACPORT_IOCTL_GET_INGRESS_DSCP_PRI_MAP: %d\n",
                               status);
        }
        else
        {
           EnetAppUtils_assert(setMacInArgs.dscpPriorityMap.dscpIPv4En ==
                                getMacOutArgs.dscpIPv4En);
           EnetAppUtils_assert(setMacInArgs.dscpPriorityMap.dscpIPv6En ==
                                getMacOutArgs.dscpIPv6En);
            for (i = 0U; i < ENET_ARRAYSIZE(setMacInArgs.dscpPriorityMap.tosMap); i++)
            {
               EnetAppUtils_assert(setMacInArgs.dscpPriorityMap.tosMap[i] ==
                                    getMacOutArgs.tosMap[i]);
            }
        }
    }

    return status;
}

static int32_t EnetTestTrafficShaping_setRxPriority(EnetTestTaskObj *taskObj)
{
    EnetPort_PriorityMap setHostInArgs;
    Enet_IoctlPrms hostPrms;
    EnetMacPort_SetPriorityRegenMapInArgs setMacInArgs;
    Enet_IoctlPrms macPrms;
    uint32_t i;
    int32_t status;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint32_t portIter;
    EnetTestMacPortList_t enabledPorts;

    for (i = 0U; i < ENET_ARRAYSIZE(setHostInArgs.priorityMap); i++)
    {
        setHostInArgs.priorityMap[i] = i;
    }

    ENET_IOCTL_SET_IN_ARGS(&hostPrms, &setHostInArgs);

    status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_HOSTPORT_IOCTL_SET_PRI_REGEN_MAP,
                        &hostPrms);
    if (status != ENET_SOK)
    {
       EnetAppUtils_print("EnetTestTrafficShaping_setRxPriority() failed ENET_HOSTPORT_IOCTL_SET_PRI_REGEN_MAP: %d\n",
                           status);
    }

    EnetTestCommon_getMacPortList(taskObj->taskCfg->macPortMask, &enabledPorts);
    /* Set Mac Port DSCP priority mapping for all mac ports */
    for (portIter = 0U; portIter < enabledPorts.numMacPorts; portIter++)
    {
        setMacInArgs.macPort = enabledPorts.macPortList[portIter];
        for (i = 0U; i < ENET_ARRAYSIZE(setMacInArgs.priorityRegenMap.priorityMap); i++)
        {
            setMacInArgs.priorityRegenMap.priorityMap[i] = i;
        }

        ENET_IOCTL_SET_IN_ARGS(&macPrms, &setMacInArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, ENET_MACPORT_IOCTL_SET_PRI_REGEN_MAP,
                            &macPrms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestTrafficShaping_setRxPriority() failed ENET_MACPORT_IOCTL_SET_PRI_REGEN_MAP: %d\n",
                               status);
        }
    }

    return status;
}

static int32_t EnetTestTrafficShaping_setPolicerPriority(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_SetPolicerEntryOutArgs setPolicerOutArgs;
    CpswAle_SetPolicerEntryInArgs setPolicerInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint32_t i;

    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        setPolicerInArgs.policerMatch.policerMatchEnMask = CPSW_ALE_POLICER_MATCH_PRIORITY;
        setPolicerInArgs.policerMatch.priority               = i;

        setPolicerInArgs.policerMatch.policerMatchEnMask |= CPSW_ALE_POLICER_MATCH_MACSRC;
        memcpy(&setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr[0U], testSrcAddr,
               sizeof(setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.addr));
        setPolicerInArgs.policerMatch.srcMacAddrInfo.addr.vlanId    = 0U;
        setPolicerInArgs.policerMatch.srcMacAddrInfo.portNum = CPSW_ALE_MACPORT_TO_ALEPORT(ENET_TEST_TRAFFIC_SHAPING_INGRESS_PORT);

        setPolicerInArgs.threadIdEn         = BFALSE;
        setPolicerInArgs.threadId               = 0U;
        setPolicerInArgs.peakRateInBitsPerSec   = gEnetTestTrafficShaping_priorityAleRate[i].peakInfoRateBitsPerSec;
        setPolicerInArgs.commitRateInBitsPerSec = gEnetTestTrafficShaping_priorityAleRate[i].committedInfoRateBitsPerSec;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerInArgs, &setPolicerOutArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_POLICER,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestTrafficShaping_setPolicerPriority() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",
                               status);
            break;
        }
    }

    return status;
}

static int32_t EnetTestTrafficShaping_setPolicerPriorityDstIpv4(EnetTestTaskObj *taskObj)
{
    int32_t status;
    Enet_IoctlPrms prms;
    CpswAle_SetPolicerEntryOutArgs setPolicerOutArgs;
    CpswAle_SetPolicerEntryInArgs setPolicerInArgs;
    EnetTestStateObj *stateObj = &taskObj->stateObj;
    uint32_t i;

    for (i = 0U; i < ENET_PRI_NUM; i++)
    {
        /* Set Policer entry */
        memset(&setPolicerInArgs, 0, sizeof(setPolicerInArgs));
        setPolicerInArgs.policerMatch.policerMatchEnMask = (CPSW_ALE_POLICER_MATCH_IPDST);
        memcpy(&setPolicerInArgs.policerMatch.dstIpInfo.ipv4Info.ipv4Addr[0U], testDestIpv4Addr,
               sizeof(setPolicerInArgs.policerMatch.dstIpInfo.ipv4Info.ipv4Addr));
        setPolicerInArgs.policerMatch.dstIpInfo.ipv4Info.ipv4Addr[0U]    += i;
        setPolicerInArgs.policerMatch.dstIpInfo.ipv4Info.numLSBIgnoreBits = 0U;
        setPolicerInArgs.policerMatch.dstIpInfo.ipAddrType            = CPSW_ALE_IPADDR_CLASSIFIER_IPV4;

        setPolicerInArgs.threadIdEn         = BFALSE;
        setPolicerInArgs.peakRateInBitsPerSec   = gEnetTestTrafficShaping_ipDstAleRate[i].peakInfoRateBitsPerSec;
        setPolicerInArgs.commitRateInBitsPerSec = gEnetTestTrafficShaping_ipDstAleRate[i].committedInfoRateBitsPerSec;

        ENET_IOCTL_SET_INOUT_ARGS(&prms, &setPolicerInArgs, &setPolicerOutArgs);

        status = Enet_ioctl(stateObj->hEnet, stateObj->coreId, CPSW_ALE_IOCTL_SET_POLICER,
                            &prms);
        if (status != ENET_SOK)
        {
           EnetAppUtils_print("EnetTestTrafficShaping_setPolicerDstIp() failed CPSW_ALE_IOCTL_SET_POLICER: %d\n",
                               status);
            break;
        }
    }

    return status;
}
