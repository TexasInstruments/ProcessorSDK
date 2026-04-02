/*
 *  Copyright (c) Texas Instruments Incorporated 2020-2024
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
 * \file  cpsw_cpts.c
 *
 * \brief This file contains the implementation of the CPSW CPTS module.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x202

#include <stdint.h>
#include <stdarg.h>
#include <ti/csl/csl_cpswitch.h>
#include <ti/drv/enet/enet_cfg.h>
#include <ti/drv/enet/include/core/enet_utils.h>
#include <ti/drv/enet/include/core/enet_osal.h>
#include <ti/drv/enet/include/core/enet_soc.h>
#include <ti/drv/enet/include/mod/cpsw_cpts.h>
#include <ti/drv/enet/priv/core/enet_trace_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_cpts_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Supported J721E versions */
#define CPSW_CPTS_VER_REVMAJ_J721E            (0x00000001U)
#define CPSW_CPTS_VER_REVMIN_J721E            (0x0000000AU)
#define CPSW_CPTS_VER_REVRTL_J721E_2G         (0x00000000U)
#define CPSW_CPTS_VER_REVRTL_J721E_9G         (0x00000000U)
#define CPSW_HOSTPORT_VER_ID_J721E            (0x00004E8AU)

/* Supported J7200 versions */
#define CPSW_CPTS_VER_REVMAJ_J7200            (0x00000001U)
#define CPSW_CPTS_VER_REVMIN_J7200            (0x0000000BU)
#define CPSW_CPTS_VER_REVRTL_J7200_2G         (0x00000000U)
#define CPSW_CPTS_VER_REVRTL_J7200_5G         (0x00000000U)
#define CPSW_HOSTPORT_VER_ID_J7200            (0x00004E8AU)

/* Supported J784S4 versions */
#define CPSW_CPTS_VER_REVMAJ_J784S4           (0x00000001U)
#define CPSW_CPTS_VER_REVMIN_J784S4           (0x0000000CU)
#define CPSW_CPTS_VER_REVRTL_J784S4_9G        (0x00000000U)
#define CPSW_HOSTPORT_VER_ID_J784S4           (0x00004E8AU)

/* Supported J742S2 versions */
#define CPSW_CPTS_VER_REVMAJ_J742S2           (0x00000001U)
#define CPSW_CPTS_VER_REVMIN_J742S2           (0x0000000CU)
#define CPSW_CPTS_VER_REVRTL_J742S2_9G        (0x00000000U)
#define CPSW_HOSTPORT_VER_ID_J742S2           (0x00004E8AU)

/*! \brief Timestamp retrieval poll iteration count. */
#define CPSW_CPTS_POLL_INTERVAL               (10000U)

/*! \brief PPM conversion factor. */
#define CPSW_CPTS_PPM                         (1000000U)

/*! \brief PPM GHz conversion factor. */
#define CPSW_CPTS_PPM_GIGAHERTZ_VAL           (1000000000U)

/*! \brief Secs-to-hours conversion factor. */
#define CPSW_CPTS_SECONDS_PER_HOUR            (3600U)

/*! \brief Minimum PPM value. */
#define CPSW_CPTS_PPM_MIN_VAL                 (0x400U)

/*! \brief GENFn minimum length value. */
#define CPSW_CPTS_GENFN_LENGTH_MIN_VAL        (5U)

/*! \brief EST event sequence number mask in sequence_id field. */
#define CPSW_CPTS_EST_EVENT_SEQNUM_MASK       (0x00FFU)

/*! \brief EST event sequence number shift within sequence_id field. */
#define CPSW_CPTS_EST_EVENT_SEQNUM_SHIFT      (0U)

/*! \brief EST event receive port number shift within sequence_id field. */
#define CPSW_CPTS_EST_EVENT_RXPORT_MASK       (0xF000U)

/*! \brief EST event receive port number shift within sequence_id field. */
#define CPSW_CPTS_EST_EVENT_RXPORT_SHIFT      (12U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

#if ENET_CFG_IS_ON(DEV_ERROR)
static int32_t CpswCpts_isSupported(CSL_cptsRegs *regs);
#endif

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
static void CpswCpts_printRegs(CSL_cptsRegs *regs);
#endif

static void CpswCpts_printStats(const CpswCpts_EventStats *stats);

static uint64_t CpswCpts_calcPpmVal(uint64_t tsPpmVal,
                                    EnetTimeSync_AdjMode adjMode);

static void CpswCpts_clearEventMemPools(CpswCpts_Handle hCpts);

static int32_t CpswCpts_checkTsCompCompat(CSL_cptsRegs *regs);

static int32_t CpswCpts_checkGenfEstfErrata(EnetMod_Handle hMod,
                                            CSL_cptsRegs *regs,
                                            uint32_t index,
                                            bool isGenf);

static int32_t CpswCpts_lookUpEvent(CpswCpts_Handle hCpts,
                                    const CpswCpts_Event *matchEvent,
                                    CpswCpts_Event *event);

static int32_t CpswCpts_lookUpEstEvent(CpswCpts_Handle hCpts,
                                       Enet_MacPort macPort,
                                       uint32_t domain,
                                       CpswCpts_EstEvent *event);

static int32_t CpswCpts_handleEvents(CpswCpts_Handle hCpts,
                                     CSL_cptsRegs *regs);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if ENET_CFG_IS_ON(DEV_ERROR)
/*! \brief CPTS versions supported by this driver. */
static CSL_CPSW_VERSION CpswCpts_gSupportedVer[] =
{
    {   /* J721E CPSW_2G */
        .majorVer = CPSW_CPTS_VER_REVMAJ_J721E,
        .minorVer = CPSW_CPTS_VER_REVMIN_J721E,
        .rtlVer   = CPSW_CPTS_VER_REVRTL_J721E_2G,
        .id       = CPSW_HOSTPORT_VER_ID_J721E,
    },
    {   /* J721E CPSW_9G */
        .majorVer = CPSW_CPTS_VER_REVMAJ_J721E,
        .minorVer = CPSW_CPTS_VER_REVMIN_J721E,
        .rtlVer   = CPSW_CPTS_VER_REVRTL_J721E_9G,
        .id       = CPSW_HOSTPORT_VER_ID_J721E,
    },
    {   /* J7200 CPSW_5G */
        .majorVer = CPSW_CPTS_VER_REVMAJ_J7200,
        .minorVer = CPSW_CPTS_VER_REVMIN_J7200,
        .rtlVer   = CPSW_CPTS_VER_REVRTL_J7200_5G,
        .id       = CPSW_HOSTPORT_VER_ID_J7200,
    },
    {   /* J784S4 CPSW_9G */
        .majorVer = CPSW_CPTS_VER_REVMAJ_J784S4,
        .minorVer = CPSW_CPTS_VER_REVMIN_J784S4,
        .rtlVer   = CPSW_CPTS_VER_REVRTL_J784S4_9G,
        .id       = CPSW_HOSTPORT_VER_ID_J784S4,
    },
    {   /* J742S2 CPSW_9G */
        .majorVer = CPSW_CPTS_VER_REVMAJ_J742S2,
        .minorVer = CPSW_CPTS_VER_REVMIN_J742S2,
        .rtlVer   = CPSW_CPTS_VER_REVRTL_J742S2_9G,
        .id       = CPSW_HOSTPORT_VER_ID_J742S2,
    },
};

/* Public CPTS IOCTL validation data. */
static Enet_IoctlValidate gCpswCpts_ioctlValidate[] =
{
    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_REGISTER_STACK,
                          sizeof(CpswCpts_RegisterStackInArgs),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_UNREGISTER_STACK,
                          0U,
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_REGISTER_HWPUSH_CALLBACK,
                          sizeof(CpswCpts_RegisterHwPushCbInArgs),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_UNREGISTER_HWPUSH_CALLBACK,
                          sizeof(CpswCpts_HwPush),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_SET_TS_NUDGE,
                          sizeof(int32_t),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_SET_COMP,
                          sizeof(CpswCpts_SetCompValInArgs),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_SET_COMP_NUDGE,
                          sizeof(int32_t),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_SET_GENF,
                          sizeof(CpswCpts_SetFxnGenInArgs),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_SET_GENF_NUDGE,
                          sizeof(CpswCpts_SetFxnGenNudgeInArgs),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_SET_ESTF,
                          sizeof(CpswCpts_SetFxnGenInArgs),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_SET_ESTF_NUDGE,
                          sizeof(CpswCpts_SetFxnGenNudgeInArgs),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_SELECT_TS_OUTPUT_BIT,
                          sizeof(CpswCpts_OutputBitSel),
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_LOOKUP_EVENT,
                          sizeof(CpswCpts_Event),
                          sizeof(CpswCpts_Event)),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_LOOKUP_EST_EVENT,
                          sizeof(CpswCpts_EstEventMatchParams),
                          sizeof(CpswCpts_EstEvent)),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_GET_CPTS_REFCLK,
                          0U,
                          sizeof(uint64_t)),
};

/* Private CPTS IOCTL validation data. */
static Enet_IoctlValidate gCpswCpts_privIoctlValidate[] =
{
    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_HANDLE_INTR,
                          0U,
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_ENABLE_INTR,
                          0U,
                          0U),

    ENET_IOCTL_VALID_PRMS(CPSW_CPTS_IOCTL_DISABLE_INTR,
                          0U,
                          0U),
};
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void CpswCpts_initCfg(CpswCpts_Cfg *cptsCfg)
{
    cptsCfg->hostRxTsEn     = BTRUE;
    cptsCfg->tsCompPolarity = BTRUE;
    cptsCfg->tsRxEventsDis  = BFALSE;
    cptsCfg->tsGenfClrEn    = BTRUE;
    cptsCfg->cptsRftClkFreq = CPSW_CPTS_RFTCLK_FREQ_200MHZ;
}

int32_t CpswCpts_open(EnetMod_Handle hMod,
                      Enet_Type enetType,
                      uint32_t instId,
                      const void *cfg,
                      uint32_t cfgSize)
{
    CpswCpts_Handle hCpts = (CpswCpts_Handle)hMod;
    const CpswCpts_Cfg *cptsCfg = (const CpswCpts_Cfg *)cfg;
    CSL_cptsRegs *regs = (CSL_cptsRegs *)hMod->virtAddr;
    CSL_CPTS_CONTROL control;
    uint32_t i;
    int32_t status = ENET_SOK;

    Enet_devAssert(cfgSize == sizeof(CpswCpts_Cfg),
                   "Invalid CPTS config params size %u (expected %u)",
                   cfgSize, sizeof(CpswCpts_Cfg));

    Enet_devAssert(regs != NULL, "CPSW CPTS regs address is not valid");

    /* Check supported CPTS module versions */
#if ENET_CFG_IS_ON(DEV_ERROR)
    status = CpswCpts_isSupported(regs);
    Enet_devAssert(status == ENET_SOK, "CPTS version is not supported");
#endif

    hCpts->tsPushInFifo = BFALSE;
    hCpts->activeGenfIdxMask = 0U;
    hCpts->activeEstfIdxMask = 0U;
    hCpts->tsAddVal = cptsCfg->cptsRftClkFreq;

    memset(&control, 0, sizeof(CSL_CPTS_CONTROL));

    /* Set default values for control register fields */
    control.cptsEn       = UTRUE;
    control.intTest      = UFALSE;
    control.ts64bMode    = UTRUE;
    control.tsOutputBitSel = CPSW_CPTS_TS_OUTPUT_BIT_DISABLED;
    control.seqEn        = UFALSE;
    control.tsCompToggle = UFALSE;

    for (i = 0U; i < hCpts->hwPushCnt; i++)
    {
        control.tsHwPushEn[i] = UTRUE;
    }

    /* Set application-based CPTS control configurations */
    control.tsCompPolarity    = cptsCfg->tsCompPolarity;
    control.tsDisableRxEvents = cptsCfg->tsRxEventsDis;
    control.tsGenfClrEn       = cptsCfg->tsGenfClrEn;
    control.tstampEn          = cptsCfg->hostRxTsEn;

    CSL_CPTS_disableCpts(regs);

    CSL_CPTS_setCntlReg(regs, &control);
    CSL_CPTS_enableCpts(regs);

    /* Configure timestamp add value to enable 1-ns operations */
    CSL_CPTS_setTSAddVal(regs, cptsCfg->cptsRftClkFreq);

    return status;
}

int32_t CpswCpts_rejoin(EnetMod_Handle hMod,
                        Enet_Type enetType,
                        uint32_t instId)
{
    return ENET_SOK;
}

void CpswCpts_saveCtxt(EnetMod_Handle hMod)
{
    CpswCpts_Handle hCpts = (CpswCpts_Handle)hMod;
    CSL_cptsRegs *regs = (CSL_cptsRegs *)hMod->virtAddr;
    uint32_t i;

    hCpts->ctxt.control      =  CSL_REG32_RD(&regs->CONTROL_REG);
    hCpts->ctxt.rftclkSel    =  CSL_REG32_RD(&regs->RFTCLK_SEL_REG);
    hCpts->ctxt.tsAddVal     =  CSL_REG32_RD(&regs->TS_ADD_VAL_REG);
    hCpts->ctxt.tsPpmLowVal  =  CSL_REG32_RD(&regs->TS_PPM_LOW_VAL_REG);
    hCpts->ctxt.tsPpmHighVal =  CSL_REG32_RD(&regs->TS_PPM_HIGH_VAL_REG);

    for (i = 0U; i < CPSW_CPTS_MAX_INSTANCE_GENF; i++)
    {
        hCpts->ctxt.genf[i].compLow  =  CSL_REG32_RD(&regs->TS_GENF[i].COMP_LOW_REG);
        hCpts->ctxt.genf[i].compHigh =  CSL_REG32_RD(&regs->TS_GENF[i].COMP_HIGH_REG);
        hCpts->ctxt.genf[i].control  =  CSL_REG32_RD(&regs->TS_GENF[i].CONTROL_REG);
        hCpts->ctxt.genf[i].length   =  CSL_REG32_RD(&regs->TS_GENF[i].LENGTH_REG);
        hCpts->ctxt.genf[i].ppmLow   =  CSL_REG32_RD(&regs->TS_GENF[i].PPM_LOW_REG);
        hCpts->ctxt.genf[i].ppmHigh  =  CSL_REG32_RD(&regs->TS_GENF[i].PPM_HIGH_REG);
        hCpts->ctxt.genf[i].nudge    =  CSL_REG32_RD(&regs->TS_GENF[i].NUDGE_REG);
    }

    for (i = 0U; i < CPSW_CPTS_MAX_INSTANCE_ESTF; i++)
    {
        hCpts->ctxt.estf[i].compLow  =  CSL_REG32_RD(&regs->TS_ESTF[i].COMP_LOW_REG);
        hCpts->ctxt.estf[i].compHigh =  CSL_REG32_RD(&regs->TS_ESTF[i].COMP_HIGH_REG);
        hCpts->ctxt.estf[i].control  =  CSL_REG32_RD(&regs->TS_ESTF[i].CONTROL_REG);
        hCpts->ctxt.estf[i].length   =  CSL_REG32_RD(&regs->TS_ESTF[i].LENGTH_REG);
        hCpts->ctxt.estf[i].ppmLow   =  CSL_REG32_RD(&regs->TS_ESTF[i].PPM_LOW_REG);
        hCpts->ctxt.estf[i].ppmHigh  =  CSL_REG32_RD(&regs->TS_ESTF[i].PPM_HIGH_REG);
        hCpts->ctxt.estf[i].nudge    =  CSL_REG32_RD(&regs->TS_ESTF[i].NUDGE_REG);
    }

    CSL_CPTS_disableInterrupt(regs);
    CSL_CPTS_disableCpts(regs);
}

int32_t CpswCpts_restoreCtxt(EnetMod_Handle hMod)
{
    CpswCpts_Handle hCpts = (CpswCpts_Handle)hMod;
    CSL_cptsRegs *regs = (CSL_cptsRegs *)hMod->virtAddr;
    uint32_t i;

    CSL_REG32_WR(&regs->CONTROL_REG, hCpts->ctxt.control);
    CSL_REG32_WR(&regs->RFTCLK_SEL_REG, hCpts->ctxt.rftclkSel);
    CSL_REG32_WR(&regs->TS_ADD_VAL_REG,  hCpts->ctxt.tsAddVal);
    CSL_REG32_WR(&regs->TS_PPM_LOW_VAL_REG,hCpts->ctxt.tsPpmLowVal);
    CSL_REG32_WR(&regs->TS_PPM_HIGH_VAL_REG, hCpts->ctxt.tsPpmHighVal);

    for (i = 0U; i < CPSW_CPTS_MAX_INSTANCE_GENF; i++)
    {
        CSL_REG32_WR(&regs->TS_GENF[i].COMP_LOW_REG,  hCpts->ctxt.genf[i].compLow);
        CSL_REG32_WR(&regs->TS_GENF[i].COMP_HIGH_REG, hCpts->ctxt.genf[i].compHigh);
        CSL_REG32_WR(&regs->TS_GENF[i].CONTROL_REG,   hCpts->ctxt.genf[i].control);
        CSL_REG32_WR(&regs->TS_GENF[i].LENGTH_REG,    hCpts->ctxt.genf[i].length);
        CSL_REG32_WR(&regs->TS_GENF[i].PPM_LOW_REG,   hCpts->ctxt.genf[i].ppmLow);
        CSL_REG32_WR(&regs->TS_GENF[i].PPM_HIGH_REG,  hCpts->ctxt.genf[i].ppmHigh);
        CSL_REG32_WR(&regs->TS_GENF[i].NUDGE_REG,     hCpts->ctxt.genf[i].nudge);
    }

    for (i = 0U; i < CPSW_CPTS_MAX_INSTANCE_ESTF; i++)
    {
        CSL_REG32_WR(&regs->TS_ESTF[i].COMP_LOW_REG,  hCpts->ctxt.estf[i].compLow);
        CSL_REG32_WR(&regs->TS_ESTF[i].COMP_HIGH_REG, hCpts->ctxt.estf[i].compHigh);
        CSL_REG32_WR(&regs->TS_ESTF[i].CONTROL_REG,   hCpts->ctxt.estf[i].control);
        CSL_REG32_WR(&regs->TS_ESTF[i].LENGTH_REG,    hCpts->ctxt.estf[i].length);
        CSL_REG32_WR(&regs->TS_ESTF[i].PPM_LOW_REG,   hCpts->ctxt.estf[i].ppmLow);
        CSL_REG32_WR(&regs->TS_ESTF[i].PPM_HIGH_REG,  hCpts->ctxt.estf[i].ppmHigh);
        CSL_REG32_WR(&regs->TS_ESTF[i].NUDGE_REG,     hCpts->ctxt.estf[i].nudge);
    }

    CSL_CPTS_enableInterrupt(regs);
    CSL_CPTS_enableCpts(regs);

    return ENET_SOK;
}

void CpswCpts_close(EnetMod_Handle hMod)
{
    CSL_cptsRegs *regs = (CSL_cptsRegs *)hMod->virtAddr;

    /* Disable CPTS module to disable events*/
    CSL_CPTS_disableCpts(regs);
}

int32_t CpswCpts_ioctl(EnetMod_Handle hMod,
                       uint32_t cmd,
                       Enet_IoctlPrms *prms)
{
    CpswCpts_Handle hCpts = (CpswCpts_Handle)hMod;
    CSL_cptsRegs *regs = (CSL_cptsRegs *)hMod->virtAddr;
    int32_t status = ENET_SOK;

#if ENET_CFG_IS_ON(DEV_ERROR)
    /* Validate CPSW CPTS IOCTL parameters */
    if (ENET_IOCTL_GET_PER(cmd) == ENET_IOCTL_PER_CPSW)
    {
        if (ENET_IOCTL_GET_TYPE(cmd) == ENET_IOCTL_TYPE_PUBLIC)
        {
            status = Enet_validateIoctl(cmd, prms,
                                        gCpswCpts_ioctlValidate,
                                        ENET_ARRAYSIZE(gCpswCpts_ioctlValidate));
        }
        else
        {
            status = Enet_validateIoctl(cmd, prms,
                                        gCpswCpts_privIoctlValidate,
                                        ENET_ARRAYSIZE(gCpswCpts_privIoctlValidate));
        }

        ENETTRACE_ERR_IF(status != ENET_SOK, status, "IOCTL 0x%08x params are not valid", cmd);
    }
#endif

    if (status == ENET_SOK)
    {
        switch (cmd)
        {
            case ENET_TIMESYNC_IOCTL_GET_VERSION:
            {
                Enet_Version *version = (Enet_Version *)prms->outArgs;
                CSL_CPTS_VERSION ver;

                CSL_CPTS_getCptsVersionInfo(regs, &ver);
                version->maj = ver.majorVer;
                version->min = ver.minorVer;
                version->rtl = ver.rtlVer;
                version->id  = ver.id;
                version->other1 = ENET_VERSION_NONE;
                version->other2 = ENET_VERSION_NONE;
            }
            break;

            case ENET_TIMESYNC_IOCTL_PRINT_REGS:
            {
#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
                CpswCpts_printRegs(regs);
#endif
            }
            break;

            case ENET_TIMESYNC_IOCTL_PRINT_STATS:
            {
                CpswCpts_printStats(&hCpts->eventStats);
            }
            break;

            case ENET_TIMESYNC_IOCTL_GET_CURRENT_TIMESTAMP:
            {
                    uint64_t *tsVal = (uint64_t *)prms->outArgs;
                    int32_t loop = CPSW_CPTS_POLL_INTERVAL;

                    if (hCpts->tsPushInFifo == BFALSE)
                    {
                        hCpts->tsPushInFifo = BTRUE;
                        CSL_CPTS_TSEventPush(regs);

                        /* Poll until the Time stamp event occurs */
                        while (loop > 0U)
                        {
                            if (hCpts->tsPushInFifo == BFALSE)
                            {
                                *tsVal = hCpts->tsVal;
                                break;
                            }

                            loop--;
                        }

                        if (loop == 0U)
                        {
                            status = ENET_ENOTFOUND;
                            ENETTRACE_ERR(status, "Timed out retrieving current timestamp event");
                            *tsVal = 0ULL;
                        }
                    }
                    else
                    {
                        status = ENET_EBUSY;
                        ENETTRACE_WARN("Timestamp push event already in FIFO, try again later...");
                    }
            }
            break;

            case ENET_TIMESYNC_IOCTL_SET_TIMESTAMP:
            {
                uint64_t tsLoadVal = *(uint64_t *)prms->inArgs;
                uint32_t tsLoadValHi = 0U;
                uint32_t tsLoadValLo = 0U;

                /* Disable CPTS interrupt */
                CSL_CPTS_disableInterrupt(regs);

                tsLoadValHi = (uint32_t)(tsLoadVal >> 32U);
                tsLoadValLo = (uint32_t)(tsLoadVal & 0xFFFFFFFFU);
                CSL_CPTS_setTSVal(regs, tsLoadValLo, tsLoadValHi);

                /* Re-enable CPTS interrupt */
                CSL_CPTS_enableInterrupt(regs);
            }
            break;

            case ENET_TIMESYNC_IOCTL_ADJUST_TIMESTAMP:
            {
                const EnetTimeSync_TimestampAdj *tsAdj = (const EnetTimeSync_TimestampAdj *)prms->inArgs;
                CSL_CPTS_TS_PPM_DIR ppmDir;
                CSL_CPTS_ESTF_PPM_DIR estfPpmDir = 0U;
                CSL_CPTS_GENF_PPM_DIR genfPpmDir = 0U;
                uint32_t adjOffset;
                uint64_t adjVal;
                uint32_t tsPpmValHi;
                uint32_t tsPpmValLo;
                uint32_t idx = 0U;
                uint32_t estfIdxMask = 0U;
                uint64_t estfAdjVal = 0U;
                uint32_t estfPpmValHi = 0U;
                uint32_t estfPpmValLo = 0U;
                uint32_t genfIdxMask = 0U;
                uint64_t genfAdjVal = 0U;
                uint32_t genfPpmValHi = 0U;
                uint32_t genfPpmValLo = 0U;

                if (tsAdj->intervalInNsecs == 0U)
                {
                    status = ENET_EINVALIDPARAMS;
                    ENETTRACE_ERR(status, "Adjust time interval cannot be 0");
                }

                estfIdxMask = hCpts->activeEstfIdxMask;
                genfIdxMask = hCpts->activeGenfIdxMask;
                if (status == ENET_SOK)
                {
                    if (tsAdj->adjValInNsecs == 0)
                    {
                        CSL_CPTS_setTSPpm(regs, 0U, 0U, CSL_CPTS_TS_PPM_DIR_INCREASE);
                        for(idx = 0U; idx < ENET_CFG_CPSW_ESTF_NUM; idx++)
                        {
                            if ((estfIdxMask & ENET_BIT(idx)) != 0U)
                            {
                                CSL_CPTS_setESTFnPpm(regs, idx, 0U, 0U, 0U);
                            }
                        }
                        for(idx = 0U; idx < ENET_CFG_CPSW_GENF_NUM; idx++)
                        {
                            if ((genfIdxMask & ENET_BIT(idx)) != 0U)
                            {
                                CSL_CPTS_setGENFnPpm(regs, idx, 0U, 0U, 0U);
                            }
                        }
                    }
                    else
                    {
                        if (tsAdj->adjValInNsecs > 0)
                        {
                            ppmDir = CSL_CPTS_TS_PPM_DIR_INCREASE;
                            estfPpmDir = CSL_CPTS_ESTF_PPM_DIR_INCREASE;
                            genfPpmDir = CSL_CPTS_GENF_PPM_DIR_INCREASE;
                            adjOffset = tsAdj->adjValInNsecs;
                        }
                        else
                        {
                            ppmDir = CSL_CPTS_TS_PPM_DIR_DECREASE;
                            estfPpmDir = CSL_CPTS_ESTF_PPM_DIR_DECREASE;
                            genfPpmDir = CSL_CPTS_GENF_PPM_DIR_DECREASE;
                            adjOffset = (uint64_t)(-1 * tsAdj->adjValInNsecs);
                        }

                        adjVal = (uint64_t)tsAdj->intervalInNsecs / adjOffset;
                        if (adjVal <= (uint64_t)CPSW_CPTS_PPM_MIN_VAL)
                        {
                            ENETTRACE_WARN("Setting PPM to %u, as %llu is less than minimum value (%u)",
                                           CPSW_CPTS_PPM_MIN_VAL, adjVal, CPSW_CPTS_PPM_MIN_VAL);
                            adjVal = CPSW_CPTS_PPM_MIN_VAL;
                        }

                        tsPpmValHi = (uint32_t)(adjVal >> 32U);
                        tsPpmValLo = (uint32_t)(adjVal & 0xFFFFFFFFU);
                        if(estfIdxMask != 0U)
                        {
                            /* GenF/ESTFn PPM will do correction using cpts refclk tick which is
                             * (cpts->ts_add_val + 1) ns, so GenF/ESTFn length PPM adj period
                             * need to be corrected.
                             */
                            estfAdjVal = adjVal * (hCpts->tsAddVal + 1);
                            estfPpmValHi = (uint32_t)(estfAdjVal >> 32U);
                            estfPpmValLo = (uint32_t)(estfAdjVal & 0xFFFFFFFFU);
                        }
                        if(genfIdxMask != 0U)
                        {
                            /* GenF/ESTFn PPM will do correction using cpts refclk tick which is
                             * (cpts->ts_add_val + 1) ns, so GenF/ESTFn length PPM adj period
                             * need to be corrected.
                             */
                            genfAdjVal = adjVal * (hCpts->tsAddVal + 1);
                            genfPpmValHi = (uint32_t)(genfAdjVal >> 32U);
                            genfPpmValLo = (uint32_t)(genfAdjVal & 0xFFFFFFFFU);
                        }
                        CSL_CPTS_setTSPpm(regs, tsPpmValLo, tsPpmValHi, ppmDir);
                        if (estfIdxMask != 0U)
                        {
                           for(idx = 0U; idx < ENET_CFG_CPSW_ESTF_NUM; idx++)
                           {
                               if ((estfIdxMask & ENET_BIT(idx)) != 0U)
                               {
                                   CSL_CPTS_setESTFnPpm(regs, idx, estfPpmValLo, estfPpmValHi, estfPpmDir);
                               }
                           }
                        }
                        if (genfIdxMask != 0U)
                        {
                           for(idx = 0U; idx < ENET_CFG_CPSW_GENF_NUM; idx++)
                           {
                               if ((genfIdxMask & ENET_BIT(idx)) != 0U)
                               {
                                   CSL_CPTS_setGENFnPpm(regs, idx, genfPpmValLo, genfPpmValHi, genfPpmDir);
                               }
                           }
                        }
                    }
                }
            }
            break;

            case ENET_TIMESYNC_IOCTL_GET_ETH_RX_TIMESTAMP:
            {
                const EnetTimeSync_GetEthTimestampInArgs *inArgs =
                    (const EnetTimeSync_GetEthTimestampInArgs *)prms->inArgs;
                uint64_t *tsVal = (uint64_t *)prms->outArgs;
                CpswCpts_Event matchEvent;
                CpswCpts_Event event;

                matchEvent.eventType = CPSW_CPTS_EVENTTYPE_ETH_RECEIVE;
                matchEvent.msgType   = inArgs->msgType;
                matchEvent.seqId     = inArgs->seqId;
                matchEvent.domain    = inArgs->domain;
                matchEvent.portNum   = inArgs->portNum;
                matchEvent.tsVal     = 0U;

                status = CpswCpts_lookUpEvent(hCpts, &matchEvent, &event);
                if (status == ENET_SOK)
                {
                    *tsVal = event.tsVal;
                }
                else if (status == ENET_ENOTFOUND)
                {
                    ENETTRACE_ERR(status,
                                  "Eth RX event not found (msgType=%u, seqId=%u, dom=%u, portNum=%u)",
                                  matchEvent.msgType,
                                  matchEvent.seqId,
                                  matchEvent.domain,
                                  matchEvent.portNum);
                }
                else
                {
                    ENETTRACE_ERR(status, "Failed to lookup for Ethernet RX event");
                }
            }
            break;

            case ENET_TIMESYNC_IOCTL_GET_ETH_TX_TIMESTAMP:
            {
                const EnetTimeSync_GetEthTimestampInArgs *inArgs =
                    (const EnetTimeSync_GetEthTimestampInArgs *)prms->inArgs;
                uint64_t *tsVal = (uint64_t *)prms->outArgs;
                CpswCpts_Event matchEvent;
                CpswCpts_Event event;

                matchEvent.eventType = CPSW_CPTS_EVENTTYPE_ETH_TRANSMIT;
                matchEvent.msgType   = inArgs->msgType;
                matchEvent.seqId     = inArgs->seqId;
                matchEvent.domain    = inArgs->domain;
                matchEvent.portNum   = inArgs->portNum;
                matchEvent.tsVal     = 0U;

                status = CpswCpts_lookUpEvent(hCpts, &matchEvent, &event);
                if (status == ENET_SOK)
                {
                    *tsVal = event.tsVal;
                }
                else if (status == ENET_ENOTFOUND)
                {
                    ENETTRACE_ERR(status,
                                  "Eth TX event not found (msgType=%u, seqId=%u, dom=%u, portNum=%u)",
                                  matchEvent.msgType,
                                  matchEvent.seqId,
                                  matchEvent.domain,
                                  matchEvent.portNum);
                }
                else
                {
                    ENETTRACE_ERR(status, "Failed to lookup for Ethernet TX event");
                }
            }
            break;

            case ENET_TIMESYNC_IOCTL_RESET:
            {
                CpswCpts_clearEventMemPools(hCpts);
            }
            break;

            case CPSW_CPTS_IOCTL_REGISTER_STACK:
            {
                CpswCpts_RegisterStackInArgs *stackInArgs = (CpswCpts_RegisterStackInArgs *)prms->inArgs;
                if (hCpts->eventNotifyCb == NULL)
                {
                    if (stackInArgs->eventNotifyCb != NULL)
                    {
                        hCpts->eventNotifyCb = stackInArgs->eventNotifyCb;
                        hCpts->eventNotifyCbArg = stackInArgs->eventNotifyCbArg;
                    }
                    else
                    {
                        status = ENET_EINVALIDPARAMS;
                        ENETTRACE_ERR(status, "Invalid event notification params");
                    }
                }
                else
                {
                    status = ENET_EALREADYOPEN;
                    ENETTRACE_ERR(status, "Stack already registered");
                }
            }
            break;

            case CPSW_CPTS_IOCTL_UNREGISTER_STACK:
            {
                /* Check if the stack is registered or not */
                if (hCpts->eventNotifyCb != NULL)
                {
                    hCpts->eventNotifyCb = NULL;
                    hCpts->eventNotifyCbArg = NULL;
                }
                else
                {
                    status = ENET_EUNEXPECTED;
                    ENETTRACE_ERR(status, "No stack is registered");
                }
            }
            break;

            case CPSW_CPTS_IOCTL_REGISTER_HWPUSH_CALLBACK:
            {
                const CpswCpts_RegisterHwPushCbInArgs *inArgs = (const CpswCpts_RegisterHwPushCbInArgs *)prms->inArgs;
                uint32_t hwPushIdx = 0U;

                if ((inArgs->hwPushNum <= hCpts->hwPushCnt) &&
                    (inArgs->hwPushNum >= CPSW_CPTS_HWPUSH_FIRST))
                {
                    hwPushIdx = CPSW_CPTS_HWPUSH_NORM(inArgs->hwPushNum);
                    if ((hwPushIdx < CPSW_CPTS_HWPUSH_COUNT_MAX) &&
                        (hCpts->hwPushNotifyCb[hwPushIdx] == NULL))
                    {
                        if (inArgs->hwPushNotifyCb != NULL)
                        {
                            hCpts->hwPushNotifyCb[hwPushIdx] = inArgs->hwPushNotifyCb;
                            hCpts->hwPushNotifyCbArg[hwPushIdx] = inArgs->hwPushNotifyCbArg;
                        }
                        else
                        {
                            status = ENET_EINVALIDPARAMS;
                            ENETTRACE_ERR(status, "Invalid hardware push notification callback params");
                        }
                    }
                    else
                    {
                        status = ENET_EALREADYOPEN;
                        ENETTRACE_ERR(status, "Hardware push callback for instance %u already registered",
                                      inArgs->hwPushNum);
                    }
                }
                else
                {
                    status = ENET_EINVALIDPARAMS;
                    ENETTRACE_ERR(status, "Invalid hardware push instance number %u", inArgs->hwPushNum);
                }
            }
            break;

            case CPSW_CPTS_IOCTL_UNREGISTER_HWPUSH_CALLBACK:
            {
                CpswCpts_HwPush hwPushNum = *(CpswCpts_HwPush *)prms->inArgs;
                uint32_t hwPushIdx = 0U;

                if ((hwPushNum <= hCpts->hwPushCnt) &&
                    (hwPushNum >= CPSW_CPTS_HWPUSH_FIRST))
                {
                    hwPushIdx = CPSW_CPTS_HWPUSH_NORM(hwPushNum);

                    /* Check if the callback is registered or not */
                    if ((hwPushIdx < CPSW_CPTS_HWPUSH_COUNT_MAX) &&
                        (hCpts->hwPushNotifyCb[hwPushIdx] != NULL))
                    {
                        hCpts->hwPushNotifyCb[hwPushIdx] = NULL;
                        hCpts->hwPushNotifyCbArg[hwPushIdx] = NULL;
                    }
                    else
                    {
                        status = ENET_SOK;
                        ENETTRACE_WARN("No callback for hardware push instance %u is registered", hwPushNum);
                    }
                }
                else
                {
                    status = ENET_EINVALIDPARAMS;
                    ENETTRACE_ERR(status, "Invalid hardware push instance number %u", hwPushNum);
                }
            }
            break;

            case CPSW_CPTS_IOCTL_SET_TS_NUDGE:
            {
                int32_t tsNudge = *(int32_t *)prms->inArgs;

                if ((tsNudge >= CPSW_CPTS_NUDGE_MIN_VAL) &&
                    (tsNudge <= CPSW_CPTS_NUDGE_MAX_VAL))
                {
                    CSL_CPTS_setTSNudge(regs, tsNudge);
                }
                else
                {
                    status = ENET_EINVALIDPARAMS;
                    ENETTRACE_ERR(status, "Nudge value %u is out of range (%u, %u)",
                                  tsNudge, CPSW_CPTS_NUDGE_MIN_VAL, CPSW_CPTS_NUDGE_MAX_VAL);
                }
            }
            break;

            case CPSW_CPTS_IOCTL_SET_COMP:
            {
                status = CpswCpts_checkTsCompCompat(regs);
                if (status == ENET_SOK)
                {
                    const CpswCpts_SetCompValInArgs *inArgs = (const CpswCpts_SetCompValInArgs *)prms->inArgs;
                    CSL_CPTS_CONTROL control;
                    uint32_t tsCompValHi = 0U;
                    uint32_t tsCompValLo = 0U;

                    CSL_CPTS_getCntlReg(regs, &control);
                    control.tsCompToggle = inArgs->tsCompToggle;
                    CSL_CPTS_setCntlReg(regs, &control);

                    tsCompValHi = (uint32_t)(inArgs->tsCompVal >> 32U);
                    tsCompValLo = (uint32_t)(inArgs->tsCompVal & 0xFFFFFFFFU);
                    CSL_CPTS_setTSCompVal(regs, tsCompValLo, tsCompValHi, inArgs->tsCompLen);
                }
                else
                {
                    status = ENET_ENOTSUPPORTED;
                    ENETTRACE_ERR(status, "SET_COMP not supported with TS add value or TS PPM, try GENFn instead");
                }
            }
            break;

            case CPSW_CPTS_IOCTL_SET_COMP_NUDGE:
            {
                int32_t tsCompNudge = *(int32_t *)prms->inArgs;

                if ((tsCompNudge >= CPSW_CPTS_NUDGE_MIN_VAL) &&
                    (tsCompNudge <= CPSW_CPTS_NUDGE_MAX_VAL))
                {
                    CSL_CPTS_setTSCompNudge(regs, tsCompNudge);
                }
                else
                {
                    status = ENET_EINVALIDPARAMS;
                    ENETTRACE_ERR(status, "Nudge value %u is out of range (%u, %u)",
                                  tsCompNudge, CPSW_CPTS_NUDGE_MIN_VAL, CPSW_CPTS_NUDGE_MAX_VAL);
                }
            }
            break;

            case CPSW_CPTS_IOCTL_SET_GENF:
            {
                const CpswCpts_SetFxnGenInArgs *inArgs = (const CpswCpts_SetFxnGenInArgs *)prms->inArgs;
                uint64_t adjVal;

                status = CpswCpts_checkGenfEstfErrata(hMod, regs, inArgs->index, BTRUE);
                ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to set GENFn due to reconfig errata");

                if (status == ENET_SOK)
                {
                    if (inArgs->ppmMode != ENET_TIMESYNC_ADJMODE_DISABLE)
                    {
                        adjVal = CpswCpts_calcPpmVal(inArgs->ppmVal, inArgs->ppmMode);
                        if (inArgs->length < CPSW_CPTS_GENFN_LENGTH_MIN_VAL)
                        {
                            status = ENET_EINVALIDPARAMS;
                            ENETTRACE_ERR_IF(status != ENET_SOK, status,
                                             "Invalid GENFn length value of %u, it should be > %u",
                                             inArgs->length, CPSW_CPTS_GENFN_LENGTH_MIN_VAL);
                        }
                    }
                    else
                    {
                        adjVal = 0ULL;
                    }
                }

                if (status == ENET_SOK)
                {
                    status = CSL_CPTS_setupGENFn(regs,
                                                 inArgs->index,
                                                 inArgs->length,
                                                 inArgs->compare,
                                                 inArgs->polarityInv,
                                                 adjVal,
                                                 inArgs->ppmDir);
                    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Wrong GENFn index value");
                    if (status == ENET_SOK)
                    {
                        if (inArgs->length != 0U)
                        {
                            hCpts->activeGenfIdxMask |= ENET_BIT(inArgs->index);
                        }
                        else
                        {
                            /* Zero length means Genf is disabled */
                            hCpts->activeGenfIdxMask &= ~(ENET_BIT(inArgs->index));
                        }
                    }
                }
            }
            break;

            case CPSW_CPTS_IOCTL_SET_GENF_NUDGE:
            {
                const CpswCpts_SetFxnGenNudgeInArgs *inArgs = (const CpswCpts_SetFxnGenNudgeInArgs *)prms->inArgs;

                if ((inArgs->tsNudge >= CPSW_CPTS_NUDGE_MIN_VAL) &&
                    (inArgs->tsNudge <= CPSW_CPTS_NUDGE_MAX_VAL))
                {
                    status = CSL_CPTS_setGENFnNudge(regs, inArgs->index, inArgs->tsNudge);
                    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Wrong GENFn index value");
                }
                else
                {
                    status = ENET_EINVALIDPARAMS;
                    ENETTRACE_ERR(status, "Nudge value %u is out of range (%u, %u)",
                                  inArgs->tsNudge, CPSW_CPTS_NUDGE_MIN_VAL, CPSW_CPTS_NUDGE_MAX_VAL);
                }
            }
            break;

            case CPSW_CPTS_IOCTL_SET_ESTF:
            {
                CpswCpts_SetFxnGenInArgs *inArgs = (CpswCpts_SetFxnGenInArgs *)prms->inArgs;
                uint64_t adjVal;

                status = CpswCpts_checkGenfEstfErrata(hMod, regs, inArgs->index, BFALSE);
                ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to set EstFn due to reconfig errata");

                if (status == ENET_SOK)
                {
                    if (inArgs->ppmMode != ENET_TIMESYNC_ADJMODE_DISABLE)
                    {
                        adjVal = CpswCpts_calcPpmVal(inArgs->ppmVal, inArgs->ppmMode);
                    }
                    else
                    {
                        adjVal = 0ULL;
                    }
                }

                if (status == ENET_SOK)
                {
                    status = CSL_CPTS_setupESTFn(regs,
                                                 inArgs->index,
                                                 inArgs->length,
                                                 inArgs->compare,
                                                 inArgs->polarityInv,
                                                 adjVal,
                                                 inArgs->ppmDir);
                    if (status == ENET_SOK)
                    {
                        if (inArgs->length != 0U)
                        {
                            hCpts->activeEstfIdxMask |= ENET_BIT(inArgs->index);
                        }
                        else
                        {
                            /* Zero length means Estfn is disabled */
                            hCpts->activeEstfIdxMask &= ~(ENET_BIT(inArgs->index));
                        }
                    }
                    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Wrong ESTFn index value");
                }
            }
            break;

            case CPSW_CPTS_IOCTL_SET_ESTF_NUDGE:
            {
                const CpswCpts_SetFxnGenNudgeInArgs *inArgs = (const CpswCpts_SetFxnGenNudgeInArgs *)prms->inArgs;

                if ((inArgs->tsNudge >= CPSW_CPTS_NUDGE_MIN_VAL) &&
                    (inArgs->tsNudge <= CPSW_CPTS_NUDGE_MAX_VAL))
                {
                    status = CSL_CPTS_setESTFnNudge(regs, inArgs->index, inArgs->tsNudge);
                    ENETTRACE_ERR_IF(status != ENET_SOK, status, "Wrong ESTFn index value");
                }
                else
                {
                    status = ENET_EINVALIDPARAMS;
                    ENETTRACE_ERR(status, "Nudge value %u is out of range (%u, %u)",
                                  inArgs->tsNudge, CPSW_CPTS_NUDGE_MIN_VAL, CPSW_CPTS_NUDGE_MAX_VAL);
                }
            }
            break;

            case CPSW_CPTS_IOCTL_SELECT_TS_OUTPUT_BIT:
            {
                CpswCpts_OutputBitSel bitSelect = *(CpswCpts_OutputBitSel *)prms->inArgs;
                CSL_CPTS_CONTROL control;

                CSL_CPTS_getCntlReg(regs, &control);

                if (control.tsOutputBitSel != CPSW_CPTS_TS_OUTPUT_BIT_DISABLED)
                {
                    control.tsOutputBitSel = CPSW_CPTS_TS_OUTPUT_BIT_DISABLED;
                    CSL_CPTS_setCntlReg(regs, &control);
                }

                control.tsOutputBitSel = bitSelect;
                CSL_CPTS_setCntlReg(regs, &control);
            }
            break;

            case CPSW_CPTS_IOCTL_LOOKUP_EVENT:
            {
                CpswCpts_Event *matchEvent = (CpswCpts_Event *)prms->inArgs;
                CpswCpts_Event *event = (CpswCpts_Event *)prms->outArgs;

                status = CpswCpts_lookUpEvent(hCpts, matchEvent, event);
                ENETTRACE_ERR_IF(status != ENET_SOK, status, "Failed to lookup for event");
            }
            break;

            case CPSW_CPTS_IOCTL_LOOKUP_EST_EVENT:
            {
                CpswCpts_EstEventMatchParams *matchParams = (CpswCpts_EstEventMatchParams *)prms->inArgs;
                CpswCpts_EstEvent *event = (CpswCpts_EstEvent *)prms->outArgs;

                status = CpswCpts_lookUpEstEvent(hCpts, matchParams->macPort, matchParams->domain, event);
                ENETTRACE_ERR_IF(((status != ENET_SOK) && (status != ENET_ENOTFOUND)), status,
                                 "Failed to lookup for EST event");
            }
            break;

            case CPSW_CPTS_IOCTL_HANDLE_INTR:
            {
                /* Handle events and notify the respective callbacks */
                status = CpswCpts_handleEvents(hCpts, regs);
                ENETTRACE_ERR_IF(status != ENET_SOK, status, "Error in handling CPTS interrupt");
            }
            break;

            case CPSW_CPTS_IOCTL_ENABLE_INTR:
            {
                CSL_CPTS_enableInterrupt(regs);
            }
            break;

            case CPSW_CPTS_IOCTL_DISABLE_INTR:
            {
                CSL_CPTS_disableInterrupt(regs);
            }
            break;

            case CPSW_CPTS_IOCTL_GET_CPTS_REFCLK:
            {
                uint32_t tsAddVal = CSL_CPTS_getTSAddVal(regs);
                uint64_t *cptsRefClk = (uint64_t *)prms->outArgs;

                *cptsRefClk = (CPSW_CPTS_PPM_GIGAHERTZ_VAL)/(tsAddVal + 1);
            }
            break;

            default:
            {
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_ERR(status, "Invalid IOCTL cmd 0x%08x", cmd);
            }
            break;
        }
    }

    return status;
}

#if ENET_CFG_IS_ON(DEV_ERROR)
static int32_t CpswCpts_isSupported(CSL_cptsRegs *regs)
{
    CSL_CPTS_VERSION version;
    uint32_t i;
    int32_t status = ENET_ENOTSUPPORTED;

    CSL_CPTS_getCptsVersionInfo(regs, &version);

    for (i = 0U; i < ENET_ARRAYSIZE(CpswCpts_gSupportedVer); i++)
    {
        if ((version.majorVer == CpswCpts_gSupportedVer[i].majorVer) &&
            (version.minorVer == CpswCpts_gSupportedVer[i].minorVer) &&
            (version.rtlVer == CpswCpts_gSupportedVer[i].rtlVer) &&
            (version.id == CpswCpts_gSupportedVer[i].id))
        {
            status = ENET_SOK;
            break;
        }
    }

    return status;
}
#endif

#if (ENET_CFG_TRACE_LEVEL >= ENET_CFG_TRACE_LEVEL_INFO)
static void CpswCpts_printRegs(CSL_cptsRegs *cptsRegs)
{
    uint32_t *regAddr = (uint32_t *)cptsRegs;
    uint32_t regIdx = 0U;

    while (((uintptr_t)regAddr) < ((uintptr_t)cptsRegs + sizeof(CSL_cptsRegs)))
    {
        if (*regAddr != 0U)
        {
            ENETTRACE_INFO("CPTS: %u: 0x%08x", regIdx, *regAddr);
        }

        regAddr++;
        regIdx++;
    }
}
#endif

static void CpswCpts_printStats(const CpswCpts_EventStats *stats)
{
#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
    ENETTRACE_INFO("CPSW CPTS statistics counters");
    ENETTRACE_INFO("-------------------------------------------------------------");
    ENETTRACE_INFO("Software timestamp event count                : %lld", stats->swTsPushEventCnt);
    ENETTRACE_INFO("Hardware timestamp push event count           : %lld", stats->hwTsPushEventCnt);
    ENETTRACE_INFO("Compare timestamp event count                 : %lld", stats->cmpEventCnt);
    ENETTRACE_INFO("Ethernet Rx timestamp count                   : %lld", stats->ethRxEventCnt);
    ENETTRACE_INFO("Ethernet Tx timestamp count                   : %lld", stats->ethTxEventCnt);
    ENETTRACE_INFO("Ethernet host Tx timestamp count              : %lld", stats->ethHostTxEventCnt);
    ENETTRACE_INFO("Hardware timestamp lookup count               : %lld", stats->hwTsPushLookupCnt);
    ENETTRACE_INFO("Compare event lookup count                    : %lld", stats->cmpEventLookupCnt);
    ENETTRACE_INFO("Ethernet Rx timestamp lookup event count      : %lld", stats->ethRxEventLookupCnt);
    ENETTRACE_INFO("Ethernet Tx timestamp lookup event count      : %lld", stats->ethTxEventLookupCnt);
    ENETTRACE_INFO("Ethernet host Tx timestamp lookup event count : %lld", stats->ethHostTxEventLookupCnt);
    ENETTRACE_INFO("Hardware timestamp discard count              : %lld", stats->hwTsPushDiscardCnt);
    ENETTRACE_INFO("Compare event discard count                   : %lld", stats->cmpEventDiscardCnt);
    ENETTRACE_INFO("Ethernet Rx timestamp discard event count     : %lld", stats->ethRxEventDiscardCnt);
    ENETTRACE_INFO("Ethernet Tx timestamp discard event count     : %lld", stats->ethTxEventDiscardCnt);
    ENETTRACE_INFO("Ethernet host Tx timestamp discard event count: %lld", stats->ethHostTxEventDiscardCnt);
#endif
}

static uint64_t CpswCpts_calcPpmVal(uint64_t tsPpmVal,
                                    EnetTimeSync_AdjMode adjMode)
{
    uint64_t adjVal = 0U;

    if (adjMode == ENET_TIMESYNC_ADJMODE_PPM)
    {
        adjVal = (CPSW_CPTS_PPM / tsPpmVal);
    }

    if (adjMode == ENET_TIMESYNC_ADJMODE_PPH)
    {
        adjVal = (CPSW_CPTS_PPM_GIGAHERTZ_VAL / tsPpmVal) * CPSW_CPTS_SECONDS_PER_HOUR;
    }

    if (adjVal <= CPSW_CPTS_PPM_MIN_VAL)
    {
        ENETTRACE_WARN("Setting PPM to %u, as %u is less than minimum value (%u)",
                       CPSW_CPTS_PPM_MIN_VAL, adjVal, CPSW_CPTS_PPM_MIN_VAL);
        adjVal = CPSW_CPTS_PPM_MIN_VAL;
    }

    return adjVal;
}

static void CpswCpts_clearEventMemPools(CpswCpts_Handle hCpts)
{
    uintptr_t key = EnetOsal_disableAllIntr();

    /* TODO: Do we have to clear the pool buffers or just the indexes? */

    memset(&hCpts->hwPushEventPool.eventMemPool[0U], 0, sizeof(hCpts->hwPushEventPool.eventMemPool));
    hCpts->hwPushEventPool.index = 0U;

    memset(&hCpts->ethTxEventPool.eventMemPool[0U], 0, sizeof(hCpts->ethTxEventPool.eventMemPool));
    hCpts->ethTxEventPool.index = 0U;

    memset(&hCpts->ethRxEventPool.eventMemPool[0U], 0, sizeof(hCpts->ethRxEventPool.eventMemPool));
    hCpts->ethRxEventPool.index = 0U;

    memset(&hCpts->cmpEventPool.eventMemPool[0U], 0, sizeof(hCpts->cmpEventPool.eventMemPool));
    hCpts->cmpEventPool.index = 0U;

    memset(&hCpts->hostTxEventPool.eventMemPool[0U], 0, sizeof(hCpts->hostTxEventPool.eventMemPool));
    hCpts->hostTxEventPool.index = 0U;

    EnetOsal_restoreAllIntr(key);
}

static int32_t CpswCpts_checkTsCompCompat(CSL_cptsRegs *regs)
{
    uint32_t tsAddVal = 0U;
    uint32_t tsPpmVal[] = {0U, 0U};
    int32_t status = ENET_SOK;

    tsAddVal = CSL_CPTS_getTSAddVal(regs);
    CSL_CPTS_getTSPpm(regs, tsPpmVal);

    /* Timestamp compare feature is not compatible with
     * timestamp add value or time stamp PPM */
    if ((tsAddVal != 0U) ||
        (tsPpmVal[0U] != 0U) ||
        (tsPpmVal[1U] != 0U))
    {
        status = ENET_ENOTSUPPORTED;
    }

    return status;
}

static int32_t CpswCpts_checkGenfEstfErrata(EnetMod_Handle hMod,
                                            CSL_cptsRegs *regs,
                                            uint32_t index,
                                            bool isGenf)
{
    uint32_t length = 0U;
    int32_t status = ENET_SOK;

    if (ENET_ERRATA_IS_EN(hMod->errata, CPSW_CPTS_ERRATA_GENFN_RECONFIG))
    {
        if (isGenf == BTRUE)
        {
            CSL_CPTS_getGENFnLength(regs, index, &length);
        }
        else
        {
            CSL_CPTS_getESTFnLength(regs, index, &length);
        }

        if (length != 0U)
        {
            status = ENET_ENOTSUPPORTED;
            ENETTRACE_WARN("Re-configuring GENFn/ESTFn is not supported for this CPTS IP version");
        }
    }

    return status;
}

static int32_t CpswCpts_lookUpEvent(CpswCpts_Handle hCpts,
                                    const CpswCpts_Event *matchEvent,
                                    CpswCpts_Event *event)
{
    CpswCpts_Event *evtPool = NULL;
    CpswCpts_Event *evt;
#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
    CpswCpts_EventStats *stats = &hCpts->eventStats;
#endif
    bool found = BFALSE;
    uint32_t i = 0U;
    uintptr_t key;
    int32_t status = ENET_SOK;

    key = EnetOsal_disableAllIntr();

    switch (matchEvent->eventType)
    {
        case CPSW_CPTS_EVENTTYPE_HW_TS_PUSH:
            evtPool = &hCpts->hwPushEventPool.eventMemPool[0U];
            break;

        case CPSW_CPTS_EVENTTYPE_ETH_RECEIVE:
            evtPool = &hCpts->ethRxEventPool.eventMemPool[0U];
            break;

        case CPSW_CPTS_EVENTTYPE_ETH_TRANSMIT:
            evtPool = &hCpts->ethTxEventPool.eventMemPool[0U];
            break;

        case CPSW_CPTS_EVENTTYPE_TS_COMP:
            evtPool = &hCpts->cmpEventPool.eventMemPool[0U];
            break;

        case CPSW_CPTS_EVENTTYPE_TS_HOST_TX:
            evtPool = &hCpts->hostTxEventPool.eventMemPool[0U];
            break;

        default:
            status = ENET_EINVALIDPARAMS;
            break;
    }

    if (status == ENET_SOK)
    {
        /* Traverse through memory pool for a matching event */
        for (i = 0U; i < ENET_CFG_CPSW_CPTS_EVENTS_POOL_SIZE; i++)
        {
            evt = &evtPool[i];

            switch (evt->eventType)
            {
                case CPSW_CPTS_EVENTTYPE_HW_TS_PUSH:
                {
                    if (matchEvent->hwPushNum == evt->hwPushNum)
                    {
                        found = BTRUE;
#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
                        stats->hwTsPushLookupCnt++;
#endif
                    }
                }
                break;

                case CPSW_CPTS_EVENTTYPE_ETH_RECEIVE:
                case CPSW_CPTS_EVENTTYPE_ETH_TRANSMIT:
                case CPSW_CPTS_EVENTTYPE_TS_HOST_TX:
                {
                    if ((matchEvent->msgType == evt->msgType) &&
                        (matchEvent->seqId == evt->seqId) &&
                        (matchEvent->domain == evt->domain) &&
                        (matchEvent->portNum == (evt->portNum - 1U)))
                    {
                        found = BTRUE;
#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
                        if (matchEvent->eventType == CPSW_CPTS_EVENTTYPE_ETH_RECEIVE)
                        {
                            stats->ethRxEventLookupCnt++;
                        }
                        else if (matchEvent->eventType == CPSW_CPTS_EVENTTYPE_ETH_TRANSMIT)
                        {
                            stats->ethTxEventLookupCnt++;
                        }
                        else if (matchEvent->eventType == CPSW_CPTS_EVENTTYPE_TS_HOST_TX)
                        {
                            stats->ethHostTxEventLookupCnt++;
                        }
#endif
                    }
                }
                break;

                case CPSW_CPTS_EVENTTYPE_TS_COMP:
                {
                    if (matchEvent->eventType == evt->eventType)
                    {
                        found = BTRUE;
#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
                        stats->cmpEventLookupCnt++;
#endif
                    }
                }
                break;

                case CPSW_CPTS_EVENTTYPE_TS_PUSH:
                case CPSW_CPTS_EVENTTYPE_TS_ROLLOVER:
                case CPSW_CPTS_EVENTTYPE_TS_HALFROLLOVER:
                case CPSW_CPTS_EVENTTYPE_INVALID:
                {
                    status = ENET_ENOTSUPPORTED;
                }
                break;
            }

            if (found)
            {
                break;
            }
        }

        if (found)
        {
            /* Copy the event information */
            *event = *evt;

            /* Remove the event from pool after copying */
            memset(evt, 0, sizeof(CpswCpts_Event));
            status = ENET_SOK;
        }
        else
        {
            status = ENET_ENOTFOUND;
        }
    }

    EnetOsal_restoreAllIntr(key);

    return status;
}

static int32_t CpswCpts_lookUpEstEvent(CpswCpts_Handle hCpts,
                                       Enet_MacPort macPort,
                                       uint32_t domain,
                                       CpswCpts_EstEvent *event)
{
    CpswCpts_Event *evtPool = &hCpts->hostTxEventPool.eventMemPool[0U];
    CpswCpts_Event *evt;
#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
    CpswCpts_EventStats *stats = &hCpts->eventStats;
#endif
    bool found = BFALSE;
    uint32_t i = 0U;
    uintptr_t key;
    int32_t status = ENET_SOK;

    key = EnetOsal_disableAllIntr();

    /* Traverse through memory pool for a matching event */
    for (i = 0U; i < ENET_CFG_CPSW_CPTS_EVENTS_POOL_SIZE; i++)
    {
        evt = &evtPool[i];

        if ((evt->portNum == ENET_MACPORT_ID(macPort)) &&
            (evt->domain == domain))
        {
            found = BTRUE;
#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
            stats->ethHostTxEventLookupCnt++;
#endif
            break;
        }
    }

    EnetOsal_restoreAllIntr(key);

    if (found)
    {
        event->tsVal    = evt->tsVal;
        event->domain   = evt->domain;
        event->priority = (uint32_t)evt->msgType;
        event->seqNum   = (evt->seqId & CPSW_CPTS_EST_EVENT_SEQNUM_MASK) >> CPSW_CPTS_EST_EVENT_SEQNUM_SHIFT;
        event->ingressPort = (evt->seqId & CPSW_CPTS_EST_EVENT_RXPORT_MASK) >> CPSW_CPTS_EST_EVENT_RXPORT_SHIFT;
        event->egressPort  = evt->portNum;

        /* Remove the event from pool after copying */
        memset(evt, 0, sizeof(CpswCpts_Event));
        status = ENET_SOK;
    }
    else
    {
        status = ENET_ENOTFOUND;
    }

    return status;
}

static int32_t CpswCpts_handleEvents(CpswCpts_Handle hCpts,
                                     CSL_cptsRegs *regs)
{
    CpswCpts_Event *evt;
#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
    CpswCpts_EventStats *stats = &hCpts->eventStats;
#endif
    CSL_CPTS_EVENTINFO eventInfo;
    volatile bool intrPendStatus = BFALSE;
    uint32_t *idx = NULL;
    uint64_t tsVal = 0U;
    uint32_t hwPushIdx = 0U;
    int32_t status = ENET_SOK;

    /* Disable CPTS interrupt */
    CSL_CPTS_disableInterrupt(regs);

    do
    {
        CSL_CPTS_getEventInfo(regs, &eventInfo);

        /* Pop the previously read value off of the event FIFO */
        CSL_CPTS_popEvent(regs);

        tsVal = ((((uint64_t)(eventInfo.timeStampHi)) << 32U) |
                 ((uint64_t)(eventInfo.timeStamp)));

        if (eventInfo.eventType == CPSW_CPTS_EVENTTYPE_TS_PUSH)
        {
            hCpts->tsVal = tsVal;
            /* Clear the flag that indicates a TS_PUSH event is in the FIFO */
            hCpts->tsPushInFifo = BFALSE;
#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
            stats->swTsPushEventCnt++;
#endif
        }
        else
        {
            switch (eventInfo.eventType)
            {
                case CPSW_CPTS_EVENTTYPE_HW_TS_PUSH:
                {
                    idx = &hCpts->hwPushEventPool.index;
                    evt = &hCpts->hwPushEventPool.eventMemPool[*idx];

                    /* Get the HW PUSH input number (1-8) which caused the event */
                    evt->eventType = (CpswCpts_EventType)eventInfo.eventType;
                    evt->hwPushNum = (CpswCpts_HwPush)eventInfo.portNo;

                    /* These fields are meaningless for this event type */
                    evt->seqId   = 0U;
                    evt->domain  = 0U;
                    evt->msgType = ENET_TIMESYNC_MESSAGE_INVALID;

                    /* Check for registered calback and call it */
                    hwPushIdx = CPSW_CPTS_HWPUSH_NORM(evt->hwPushNum);
                    if ((hwPushIdx < CPSW_CPTS_HWPUSH_COUNT_MAX) &&
                        (hCpts->hwPushNotifyCb[hwPushIdx] != NULL))
                    {
                        hCpts->hwPushNotifyCb[hwPushIdx](hCpts->hwPushNotifyCbArg[hwPushIdx],
                                                         evt->hwPushNum);
                    }
#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
                    stats->hwTsPushEventCnt++;
                    if (evt->tsVal != 0U)
                    {
                        stats->hwTsPushDiscardCnt++;
                    }
#endif
                }
                break;

                case CPSW_CPTS_EVENTTYPE_ETH_RECEIVE:
                {
                    idx = &hCpts->ethRxEventPool.index;
                    evt = &hCpts->ethRxEventPool.eventMemPool[*idx];

                    /* For Ethernet events, get the messageType, sequence ID, and port number */
                    evt->eventType = (CpswCpts_EventType)eventInfo.eventType;
                    evt->msgType   = (EnetTimeSync_MsgType)eventInfo.msgType;
                    evt->seqId     = eventInfo.seqId;
                    evt->portNum   = eventInfo.portNo;
                    evt->domain    = eventInfo.domain;

                    /* These fields are meaningless for this event type */
                    evt->hwPushNum = CPSW_CPTS_HWPUSH_INVALID;

#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
                    stats->ethRxEventCnt++;
                    if (evt->tsVal != 0U)
                    {
                        stats->ethRxEventDiscardCnt++;
                    }
#endif
                }
                break;

                case CPSW_CPTS_EVENTTYPE_ETH_TRANSMIT:
                {
                    idx = &hCpts->ethTxEventPool.index;
                    evt = &hCpts->ethTxEventPool.eventMemPool[*idx];

                    /* For Ethernet events, get the messageType, sequence ID, and port number */
                    evt->eventType = (CpswCpts_EventType)eventInfo.eventType;
                    evt->msgType   = (EnetTimeSync_MsgType)eventInfo.msgType;
                    evt->seqId     = eventInfo.seqId;
                    evt->portNum   = eventInfo.portNo;
                    evt->domain    = eventInfo.domain;

                    /* These fields are meaningless for this event type */
                    evt->hwPushNum = CPSW_CPTS_HWPUSH_INVALID;

#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
                    stats->ethTxEventCnt++;
                    if (evt->tsVal != 0U)
                    {
                        stats->ethTxEventDiscardCnt++;
                    }
#endif
                }
                break;

                case CPSW_CPTS_EVENTTYPE_TS_COMP:
                {
                    idx = &hCpts->cmpEventPool.index;
                    evt = &hCpts->cmpEventPool.eventMemPool[*idx];

                    evt->eventType = (CpswCpts_EventType)eventInfo.eventType;

                    /* These fields are meaningless for this event type */
                    evt->portNum   = 0U;
                    evt->seqId     = 0U;
                    evt->domain    = 0U;
                    evt->hwPushNum = CPSW_CPTS_HWPUSH_INVALID;
                    evt->msgType   = ENET_TIMESYNC_MESSAGE_INVALID;

#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
                    stats->cmpEventCnt++;
                    if (evt->tsVal != 0U)
                    {
                        stats->cmpEventDiscardCnt++;
                    }
#endif
                }
                break;

                case CPSW_CPTS_EVENTTYPE_TS_HOST_TX:
                {
                    idx = &hCpts->hostTxEventPool.index;
                    evt = &hCpts->hostTxEventPool.eventMemPool[*idx];

                    evt->eventType = (CpswCpts_EventType)eventInfo.eventType;
                    evt->msgType   = (EnetTimeSync_MsgType)eventInfo.msgType;
                    evt->seqId     = eventInfo.seqId;
                    evt->domain    = eventInfo.domain;
                    evt->portNum   = eventInfo.portNo;

                    /* These fields are meaningless for this event type */
                    evt->hwPushNum = CPSW_CPTS_HWPUSH_INVALID;

#if ENET_CFG_IS_ON(ENET_CFG_CPSW_CPTS_STATS)
                    stats->ethHostTxEventCnt++;
                    if (evt->tsVal != 0U)
                    {
                        stats->ethHostTxEventDiscardCnt++;
                    }
#endif
                }
                break;

                default:
                {
                    status = ENET_EINVALIDPARAMS;
                }
                break;
            }

            if (status == ENET_SOK)
            {
                /* Copy the 64-bit timestamp value to memory pool */
                evt->tsVal = tsVal;

                /* Notify the registered stack that a CPTS event has been detected */
                if (hCpts->eventNotifyCb != NULL)
                {
                    (*hCpts->eventNotifyCb)(hCpts->eventNotifyCbArg, evt);
                }

                /* Increment the index */
                *idx = (((*idx) + 1U) % ENET_CFG_CPSW_CPTS_EVENTS_POOL_SIZE);
            }
        }

        intrPendStatus = CSL_CPTS_isRawInterruptStatusBitSet(regs);
    }
    while (intrPendStatus == BTRUE);

    /* Re-enable CPTS interrupt */
    CSL_CPTS_enableInterrupt(regs);

    return status;
}
