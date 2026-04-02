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

/**
 *  \file   watchdog_soc.c
 *
 *  \brief  TPR12 device specific hardware attributes.
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

#include <ti/csl/soc.h>
#include <ti/drv/watchdog/soc/watchdog_soc.h>
#include <ti/drv/watchdog/src/v0/watchdog_rti.h>

#include <ti/csl/soc/tpr12/src/cslr_mss_rcm.h>
#include <ti/csl/soc/tpr12/src/cslr_dss_rcm.h>
#include <ti/csl/soc/tpr12/src/cslr_mss_toprcm.h>

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 * @brief
 *  Watchdog related SOC Hardware Atrributes
 *
 * @details
 *  SOC Driver Hardware Attributes which are used to reset and trigger
 *  warm reset for Watchdog
 *
 */
typedef struct SOC_HwAttrs_t
{
    /**
     * @brief   Base address of the RCM address space to be used.
     */
#if defined (__TI_ARM_V7R4__)
    CSL_mss_rcmRegs*   ptrRCMRegs;
#elif defined (_TMS320C6X)
    CSL_dss_rcmRegs*   ptrRCMRegs;
#endif

    /**
     * @brief   Base address of the top-level RCM address space to be used.
     */
    CSL_mss_toprcmRegs* ptrTopRCMRegs;
} SOC_HwAttrs;


/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/
/**************************************************************************
 * On TPR12, Watchdog is available on both the DSS and MSS. The Watchdog
 * platform configuration is MSS and DSS specific
 **************************************************************************/
/* Watchdog objects */
Watchdog_MCB watchdogObject;

#define CSL_WDT_PER_CNT    1U

#if defined (__TI_ARM_V7R4__)

/**
 * @brief   There is only 1 Watchdog instance
 * available. This should *NOT* be modified by the customer.
 */
Watchdog_HwAttrs gWatchdogHwCfg[CSL_WDT_PER_CNT] =
{
    /* Watchdog Hardware configuration:
     * - Base address of memory mapped registers
     * - ESM group number
     * - ESM error number
     * - ESM handle
     */
    {
        CSL_MSS_WDT_U_BASE,
        RTI_CLK_200M,
        2,
        ESMG2_WDT_NMI_REQ,
        NULL
    }
};

CSL_PUBLIC_CONST Watchdog_Config Watchdog_config[] =
{
    {
        &gWatchdogFxnTable,             /* Watchdog Driver Function Table:     */
        (void *)&watchdogObject,        /* Watchdog Driver Object:             */
        (void *)&gWatchdogHwCfg         /* Watchdog Hw configuration:          */
    },
    {
        NULL,
        NULL,
        NULL
    }
};

SOC_HwAttrs gSOCHwAttr =
{
    (CSL_mss_rcmRegs*)CSL_MSS_RCM_U_BASE,
    (CSL_mss_toprcmRegs*)CSL_MSS_TOPRCM_U_BASE
};

#elif defined (_TMS320C6X)
/**
 * @brief   There is only 1 Watchdog instance
 * available. This should *NOT* be modified by the customer.
 */
Watchdog_HwAttrs gWatchdogHwCfg[CSL_WDT_PER_CNT] =
{
    /* Watchdog Hardware configuration:
     * - Base address of memory mapped registers
     * - ESM group number
     * - ESM error number
     * - ESM handle
     */
    {
        CSL_DSS_WDT_U_BASE,
        RTI_CLK_200M,
        2,
        CSL_DSS_ESMG2_DSS_WDT_NMI_REQ,
        NULL
    }
};

CSL_PUBLIC_CONST Watchdog_Config Watchdog_config[] =
{
    {
        &gWatchdogFxnTable,             /* Watchdog Driver Function Table:     */
        (void *)&watchdogObject,        /* Watchdog Driver Object:             */
        (void *)&gWatchdogHwCfg         /* Watchdog Hw configuration:          */
    },
    {
        NULL,
        NULL,
        NULL
    }
};

SOC_HwAttrs gSOCHwAttr =
{
    (CSL_dss_rcmRegs*)CSL_DSS_RCM_U_BASE,
    (CSL_mss_toprcmRegs*)CSL_MSS_TOPRCM_U_BASE
};
#endif

/**
 *  @b Description
 *  @n
 *      This function is used to bring watchdog out of reset
 */
void RTI_socEnableWatchdog()
{
#if defined (__TI_ARM_V7R4__)
    CSL_mss_rcmRegs* ptrRCMRegs = gSOCHwAttr.ptrRCMRegs;
    if (CSL_FEXT(ptrRCMRegs->MSS_WDT_RST_CTRL,
                 MSS_RCM_MSS_WDT_RST_CTRL_MSS_WDT_RST_CTRL_ASSERT) != 0)
    {
        CSL_FINS(ptrRCMRegs->MSS_WDT_RST_CTRL,
                 MSS_RCM_MSS_WDT_RST_CTRL_MSS_WDT_RST_CTRL_ASSERT,
                 0x7U);
    }
#elif defined (_TMS320C6X)
    CSL_dss_rcmRegs* ptrRCMRegs = gSOCHwAttr.ptrRCMRegs;
    if (CSL_FEXT(ptrRCMRegs->DSS_WDT_RST_CTRL,
                 DSS_RCM_DSS_WDT_RST_CTRL_DSS_WDT_RST_CTRL_ASSERT) != 0)
    {
        CSL_FINS(ptrRCMRegs->DSS_WDT_RST_CTRL,
                 DSS_RCM_DSS_WDT_RST_CTRL_DSS_WDT_RST_CTRL_ASSERT,
                 0x7U);
    }
#endif
}

/**
 *  @b Description
 *  @n
 *      The function is used to trigger a warm reset.
 */
void RTI_socTriggerWatchdogWarmReset()
{
    CSL_mss_toprcmRegs* ptrTopRCMRegs = gSOCHwAttr.ptrTopRCMRegs;
    CSL_FINS(ptrTopRCMRegs->WARM_RESET_CONFIG,
             MSS_TOPRCM_WARM_RESET_CONFIG_WARM_RESET_CONFIG_WDOG_RST_EN,
             0x7U);
}

/**
 * \brief  This API gets the SoC level of Watchdog intial configuration
 *
 * \param  index     Watchdog instance index.
 * \param  cfg       Pointer to Watchdog SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t Watchdog_socGetInitCfg(uint32_t index, Watchdog_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_WDT_PER_CNT)
    {
        *cfg = gWatchdogHwCfg[index];
    }
    else
    {
        ret = (-((int32_t)1));
    }

    return ret;
}

/**
 * \brief  This API sets the SoC level of Watchdog intial configuration
 *
 * \param  index     Watchdog instance index.
 * \param  cfg       Pointer to Watchdog SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t Watchdog_socSetInitCfg(uint32_t index, const Watchdog_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_WDT_PER_CNT)
    {
        gWatchdogHwCfg[index] = *cfg;
    }
    else
    {
        ret = (-((int32_t)1));
    }

    return ret;
}

void Watchdog_socInit(Watchdog_Handle handle)
{
    return;
}
