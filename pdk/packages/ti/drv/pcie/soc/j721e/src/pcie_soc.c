/**
 *   @file  j721e/src/pcie_soc.c
 *
 *   @brief
 *      This file contains the device specific configuration and initialization routines
 *      for pcie Low Level Driver.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2019, Texas Instruments, Inc.
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
 *
 *  \par
*/

/**
 * This file contains an example device configuration for the pcie LLD.
 * It is not precompiled to facilitate user modification of the file.
 */

#include <stdint.h>
#include <stdlib.h>

/* CSL RL includes */
#include <ti/csl/cslr_device.h>
#include <ti/csl/soc/j721e/src/cslr_soc_baseaddress.h>

/* pcie LLD includes */
#include <ti/drv/pcie/soc/pcie_soc.h>
#include <ti/drv/pcie/pcie.h>
#include <ti/drv/pcie/src/v3/pcie.h>

/** @addtogroup PCIE_LLD_DATASTRUCT
@{
*/

/** @brief PCIE v3 calltable */
Pcie_FxnTable pcieFxnTablev3 =
{
  /*! Function to set PCIE to EP or RC for one device */
  &Pciev3_setInterfaceMode,
  /*! Function to get the PCIE data area reserved size */
  &Pciev3_getMemSpaceReserved,
  /*! Function to get the PCIE data area base address & size */
  &Pciev3_getMemSpaceRange,
  /*! Function to read any PCIE register(s) */
  &Pciev3_readRegs,
  /*! Function to write any PCIE register(s) */
  &Pciev3_writeRegs,
  /*! Function to configure outbound translation registers */
  NULL, /* not supported */
  /*! Function to configure inbound translation registers */
  NULL, /* not supported */
  /*! Function to configure a BAR register */
  &Pciev3_cfgBar,
  /*! Function to configure an ATU region */
  &Pciev3_atuRegionConfig,
  /*! Function to read functional (MSI/INTX) pending bits with low overhead. */
  &Pciev3_getPendingFuncInts,
  /*! Function to clear functional (MSI/INTX) pending bits with low overhead. */
  &Pciev3_clrPendingFuncInts
};

Pciev3_DevParams pcieDevParamsDev1 =
{
  (volatile uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE0_CTRL),
  (volatile uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK0_CLKSEL),
  (void *)CSL_PCIE0_CORE_USER_CFG_USER_CFG_BASE,
  (void *)CSL_PCIE0_CORE_PCIE_INTD_CFG_INTD_CFG_BASE,
  1, /* one lane */
  3, /* default to GEN3 */
  0  /* use PF 0 */ 
};

Pciev3_DevParams pcieDevParamsDev2 =
{
  (volatile uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE1_CTRL),
  (volatile uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK1_CLKSEL),
  (void *)CSL_PCIE1_CORE_USER_CFG_USER_CFG_BASE,
  (void *)CSL_PCIE1_CORE_PCIE_INTD_CFG_INTD_CFG_BASE,
  1, /* one lane */
  3, /* default to GEN3 */
  0  /* use PF 0 */ 
};

Pciev3_DevParams pcieDevParamsDev3 =
{
  (volatile uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE2_CTRL),
  (volatile uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK2_CLKSEL),
  (void *)CSL_PCIE2_CORE_USER_CFG_USER_CFG_BASE,
  (void *)CSL_PCIE2_CORE_PCIE_INTD_CFG_INTD_CFG_BASE,
  1, /* one lane */
  3, /* default to GEN3 */
  0  /* use PF 0 */ 
};

Pciev3_DevParams pcieDevParamsDev4 =
{
  (volatile uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE3_CTRL),
  (volatile uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK3_CLKSEL),
  (void *)CSL_PCIE3_CORE_USER_CFG_USER_CFG_BASE,
  (void *)CSL_PCIE3_CORE_PCIE_INTD_CFG_INTD_CFG_BASE,
  1, /* one lane */
  3, /* default to GEN3 */
  0  /* use PF 0 */ 
};

Pciev3_DeviceCfgBaseAddrs pciev3CfgBaseAddrDev1 =
{
  (void *)CSL_PCIE0_CORE_DBN_CFG_PCIE_CORE_BASE,
/* The mapped offset is 0x10010000, but that points to registers
   that normally occur at base + 0x1000, hence the - 0x1000.
   No access to 0x0 to 0x0FFF is normal for other devices but
   they have separate structure that can be pointed here */
  (uint32_t)0x10000u
};

Pciev3_DeviceCfgBaseAddrs pciev3CfgBaseAddrDev2 =
{
  (void *)CSL_PCIE1_CORE_DBN_CFG_PCIE_CORE_BASE,
  (uint32_t)0x10000u
};

Pciev3_DeviceCfgBaseAddrs pciev3CfgBaseAddrDev3 =
{
  (void *)CSL_PCIE2_CORE_DBN_CFG_PCIE_CORE_BASE,
  (uint32_t)0x10000u
};

Pciev3_DeviceCfgBaseAddrs pciev3CfgBaseAddrDev4 =
{
  (void *)CSL_PCIE3_CORE_DBN_CFG_PCIE_CORE_BASE,
  (uint32_t)0x10000u
};

Pcie_DeviceCfgBaseAddr pcieBaseAddrDev1 =
{
  (void *)&pciev3CfgBaseAddrDev1,
  (void *)CSL_PCIE0_DAT0_BASE,
  0U,
  (void *)&pcieDevParamsDev1
};

Pcie_DeviceCfgBaseAddr pcieBaseAddrDev2 =
{
  (void *)&pciev3CfgBaseAddrDev2,
  (void *)CSL_PCIE1_DAT0_BASE,
  0U,
  (void *)&pcieDevParamsDev2
};
/*
Pcie_DeviceCfgBaseAddr pcieBaseAddrDev3 =
{
  (void *)&pciev3CfgBaseAddrDev3,
  (void *)CSL_PCIE2_DAT0_BASE,
  0U,
  (void *)&pcieDevParamsDev3
};

Pcie_DeviceCfgBaseAddr pcieBaseAddrDev4 =
{
  (void *)&pciev3CfgBaseAddrDev4,
  (void *)CSL_PCIE3_DAT0_BASE,
  0U,
  (void *)&pcieDevParamsDev4
};
*/

/** @brief PCIE LLD initialization parameters */
const Pcie_InitCfg pcieInitCfg =
{
  {
    {
      &pcieBaseAddrDev1,
      &pcieBaseAddrDev2,
      NULL, /* &pcieBaseAddrDev3, */
      NULL /* &pcieBaseAddrDev4 */
    },
    {
      &pcieFxnTablev3,
      &pcieFxnTablev3,
      NULL, /* &pcieFxnTablev3, */
      NULL /* &pcieFxnTablev3 */
    }
  }
};

/**
@}
*/

