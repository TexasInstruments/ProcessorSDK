/**
 *  \file   GPMC_soc.c
 *
 *  \brief  AM64x device specific hardware attributes.
 *
 */

/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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
 *
 */

#include <ti/csl/soc.h>
#include <ti/drv/gpmc/GPMC.h>
#include <ti/drv/gpmc/soc/GPMC_soc.h>

#define CSL_GPMC_CNT        (1U)

/* GPMC configuration structure */
GPMC_v1_HwAttrs GPMCInitCfg[CSL_GPMC_CNT + 1] =
{
    {
        CSL_GPMC0_CFG_BASE,           /* gpmcBaseAddr */
        GPMC_DEVICETYPE_SRAMLIKE,     /* devType */
        GPMC_DEVICESIZE_16BITS,       /* devSize */
        GPMC_MUXADDDATA_NOMUX,        /* addrDataMux */
        GPMC_TIMEPARAGRANULARITY_X1,  /* timeLatency */
        0U,                           /* chipSel */
        0x10000000U,                  /* chipSelBaseAddr */
        GPMC_CS_SIZE_16MB,            /* chipSelAddrSize */
        0U,                           /* intrNum */
        0U,                           /* eventId */
        INVALID_INTC_MUX_NUM,         /* intcMuxNum */
        -1,                           /* intcMuxInEvent */
        -1,                           /* intcMuxOutEvent */
        false,                        /* intrEnable */
        0,                            /* waitPinNum */
        GPMC_WAIT_PIN_POLARITY_LOW,   /* waitPinPol */
        {
            0U,                       /* csOnTime */
            7U,                       /* csRdOffTime */
            7U,                       /* csWrOffTime */
            1U,                       /* advOnTime */
            3U,                       /* advRdOffTime */
            3U,                       /* advWrOffTime */
            3U,                       /* advAadMuxOnTime */
            2U,                       /* advAadMuxRdOffTime */
            2U,                       /* advAadMuxWrOffTime */
            5U,                       /* weOnTime */
            7U,                       /* weOffTime */
            5U,                       /* oeOnTime */
            7U,                       /* oeOffTime */
            1U,                       /* oeAadMuxOnTime */
            3U,                       /* oeAadMuxOffTime */
            1U,                       /* pageBurstAccessTime */
            6U,                       /* rdAccessTime */
            6U,                       /* wrAccessTime */
            8U,                       /* rdCycleTime */
            8U,                       /* wrCycleTime */
            5U,                       /* wrDataOnMuxBusTime */
            0U,                       /* cycle2CycleDelay */
            GPMC_CYCLE2CYCLESAMECSEN_NOC2CDELAY, /* cycleDelaySameChipSel */
            GPMC_CYCLE2CYCLEDIFFCSEN_NOC2CDELAY, /* cycleDelayDiffChipSel */
            0U                        /* busTurnAroundTime */
        },
        CSL_ELM0_BASE,                /* elmBaseAddr */
        GPMC_NAND_ECC_ALGO_NONE,      /* eccAlgo */
        FALSE,                        /* dmaMode */
        CSL_GPMC0_DATA_BASE,          /* dataBaseAddr */
        GPMC_READTYPE_SYNC,           /* readType */
        GPMC_WRITETYPE_SYNC,          /* writeType */
        GPMC_CS_EXTRA_DELAY,          /* csExDelay */
        GPMC_ACCESSTYPE_SINGLE,       /* accessType */
    },
};


/* GPMC objects */
GPMC_v1_Object GPMCObjects[CSL_GPMC_CNT + 1];


/* GPMC configuration structure */
GPMC_Config GPMC_config[CSL_GPMC_CNT + 1] = {
    {
        &GPMC_FxnTable_v1,
        &GPMCObjects[0],
        &GPMCInitCfg[0]
    },
    {
        NULL,
        NULL,
        NULL
    }
};

/**
 * \brief  This API gets the SoC level of GPMC intial configuration
 *
 * \param  index     GPMC instance index.
 * \param  cfg       Pointer to GPMC SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t GPMC_socGetInitCfg(uint32_t index, GPMC_v1_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_GPMC_CNT)
    {
        *cfg = GPMCInitCfg[index];
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/**
 * \brief  This API sets the SoC level of GPMC intial configuration
 *
 * \param  index     GPMC instance index.
 * \param  cfg       Pointer to GPMC SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t GPMC_socSetInitCfg(uint32_t index, const GPMC_v1_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_GPMC_CNT)
    {
        GPMCInitCfg[index] = *cfg;
    }
    else
    {
        ret = -1;
    }

    return ret;
}

