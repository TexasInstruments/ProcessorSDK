/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file sciserver_soc.c
 *
 * \brief Implements SoC specific configuration during sciserver_init
 *
 */
#include <ti/csl/soc.h>
#include <types/short_types.h>
#include <lib/mmr_lock.h>
#include <ti/drv/sciclient/sciserver.h>

static void Sciserver_powerOnLocalDru (void);

static void Sciserver_enableAcspcie (void);

static void Sciserver_enableAcspcie (void)
{
    mmr_unlock(CSL_CTRL_MMR0_CFG0_BASE, 2);

    /**
     * Enable the control bit for CTRLMMR_PCIE_REFCLK0
     */
    uint32_t *addr=(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK0_CLKSEL);
    CSL_REG32_FINS(addr,MAIN_CTRL_MMR_CFG0_PCIE_REFCLK0_CLKSEL_OUT_CLK_EN,1);

    /**
     * Enable the control bit for CTRLMMR_PCIE_REFCLK1
     */
    addr=(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK1_CLKSEL);
    CSL_REG32_FINS(addr,MAIN_CTRL_MMR_CFG0_PCIE_REFCLK1_CLKSEL_OUT_CLK_EN,1);

    /**
     * Enable the control bit for CTRLMMR_PCIE_REFCLK2
     */
    addr=(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK2_CLKSEL);
    CSL_REG32_FINS(addr,MAIN_CTRL_MMR_CFG0_PCIE_REFCLK2_CLKSEL_OUT_CLK_EN,1);

    /**
     * Enable the control bit for CTRLMMR_PCIE_REFCLK3
     */
    addr=(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK3_CLKSEL);
    CSL_REG32_FINS(addr,MAIN_CTRL_MMR_CFG0_PCIE_REFCLK3_CLKSEL_OUT_CLK_EN,1);

    mmr_lock(CSL_CTRL_MMR0_CFG0_BASE, 2);
}

static void Sciserver_powerOnLocalDru (void)
{
    Sciclient_pmSetModuleState(TISCI_DEV_COMPUTE_CLUSTER0_DRU4,
        TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,TISCI_MSG_FLAG_AOP,SCICLIENT_SERVICE_WAIT_FOREVER);
    Sciclient_pmSetModuleState(TISCI_DEV_COMPUTE_CLUSTER0_DRU5,
        TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,TISCI_MSG_FLAG_AOP,SCICLIENT_SERVICE_WAIT_FOREVER);
    Sciclient_pmSetModuleState(TISCI_DEV_COMPUTE_CLUSTER0_DRU6,
        TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,TISCI_MSG_FLAG_AOP,SCICLIENT_SERVICE_WAIT_FOREVER);
    Sciclient_pmSetModuleState(TISCI_DEV_COMPUTE_CLUSTER0_DRU7,
        TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,TISCI_MSG_FLAG_AOP,SCICLIENT_SERVICE_WAIT_FOREVER);
}

void Sciserver_socInit(void)
{
    /** Set OUT_CLK_EN bit for ACSPCIE Buffer **/
    Sciserver_enableAcspcie();

    /*  Turn on the local DRU devices. They are not part of the MAIN_ALWAYS_ON LPSC. */
    Sciserver_powerOnLocalDru();
}

