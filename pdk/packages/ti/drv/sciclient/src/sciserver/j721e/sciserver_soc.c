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
#include <string.h>

static void Sciserver_enableAcspcie (void);
static void J721E_ClearBootInterrupts (void);

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

#define MAIN2MCU_LVL_INTRTR0_OUTL_32 (32U)
#define MAIN2MCU_LVL_INTRTR0_OUTL_33 (33U)

static void J721E_ClearBootInterrupts(void)
{
    struct tisci_msg_rm_irq_set_req     rmIrqReq;
    struct tisci_msg_rm_irq_set_resp    rmIrqResp;
    struct tisci_msg_rm_irq_release_req rmIrqRelease;
    struct tisci_msg_rm_get_resource_range_resp res = {0};
    struct tisci_msg_rm_get_resource_range_req  req = {0};
    uint16_t rangeStart = 0;
    uint16_t rangeNum   = 0;
    int32_t  retVal = CSL_PASS;

    /* Use sciclient APIs to set/clear the eMMC & SD boot ints previously routed by ROM via MAIN2MCU_LVL_INTRTR, for free use later */

    /* Check resource range to see if peripheral IRQs (coming from MAIN2MCU level int router) fall in the range of MCU1_0 or MCU1_1 */
    req.type           = TISCI_DEV_MAIN2MCU_LVL_INTRTR0;
    req.subtype        = (uint8_t)TISCI_RESASG_SUBTYPE_IR_OUTPUT;
    req.secondary_host = (uint8_t)TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

    res.range_num = 0;
    res.range_start = 0;
    /* Get interrupt number range for MCU1_0 (all others after this fall to MCU1_1) */
    retVal = Sciclient_rmGetResourceRange(&req, &res, SCICLIENT_SERVICE_WAIT_FOREVER);

    if ((CSL_PASS != retVal) || (res.range_num == 0U))
    {
        /* Try with HOST_ID_ALL */
        req.secondary_host = TISCI_HOST_ID_ALL;

        retVal = Sciclient_rmGetResourceRange(&req, &res, SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if(retVal == CSL_PASS)
    {
        rangeStart = res.range_start;
        rangeNum = res.range_num;
    }

    /* Set & release interrupt route for the MMCSD0 interrupt */
    memset(&rmIrqReq,0,sizeof(rmIrqReq));
    memset(&rmIrqRelease,0,sizeof(rmIrqRelease));

    rmIrqReq.src_id = TISCI_DEV_MMCSD0;
    rmIrqReq.src_index = 0;
    rmIrqRelease.src_id = TISCI_DEV_MMCSD0;
    rmIrqRelease.src_index = 0;

    rmIrqReq.valid_params |= TISCI_MSG_VALUE_RM_DST_ID_VALID;
    rmIrqReq.valid_params |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
    rmIrqRelease.valid_params |= TISCI_MSG_VALUE_RM_DST_ID_VALID;
    rmIrqRelease.valid_params |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;

    if ((uint16_t)MAIN2MCU_LVL_INTRTR0_OUTL_32 < (rangeStart + rangeNum))
    {
        /* Int Router output for eMMC is within MCU1_0 int range */
        rmIrqReq.dst_id = TISCI_DEV_MCU_R5FSS0_CORE0;
        rmIrqReq.dst_host_irq = CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_32;
        rmIrqReq.secondary_host = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

        rmIrqRelease.dst_id = TISCI_DEV_MCU_R5FSS0_CORE0;
        rmIrqRelease.dst_host_irq = CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_32;
        rmIrqRelease.secondary_host = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;
    }
    else
    {
        /* Int Router output for eMMC is within the 2nd core's int range */
        rmIrqReq.dst_id = TISCI_DEV_MCU_R5FSS0_CORE1;
        rmIrqReq.dst_host_irq = CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_32;
        rmIrqReq.secondary_host = TISCI_HOST_ID_MCU_0_R5_2;
        rmIrqReq.valid_params |= TISCI_MSG_VALUE_RM_SECONDARY_HOST_VALID;

        rmIrqRelease.dst_id = TISCI_DEV_MCU_R5FSS0_CORE1;
        rmIrqRelease.dst_host_irq = CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_32;
        rmIrqRelease.secondary_host = TISCI_HOST_ID_MCU_0_R5_2;
        rmIrqRelease.valid_params |= TISCI_MSG_VALUE_RM_SECONDARY_HOST_VALID;
    }

    retVal = Sciclient_rmIrqSet(
                (const struct tisci_msg_rm_irq_set_req *)&rmIrqReq,
                &rmIrqResp,
                SCICLIENT_SERVICE_WAIT_FOREVER);

    /* Go ahead and release the MAIN2MCU IR route for MMCSD0 now */
    if(retVal == CSL_PASS)
    {
        retVal = Sciclient_rmIrqRelease(
                (const struct tisci_msg_rm_irq_release_req *)&rmIrqRelease,
                SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    /* Set & release interrupt route for the MMCSD1 interrupt */
    memset(&rmIrqReq,0,sizeof(rmIrqReq));
    memset(&rmIrqRelease,0,sizeof(rmIrqRelease));

    rmIrqReq.src_id = TISCI_DEV_MMCSD1;
    rmIrqReq.src_index = 0;
    rmIrqRelease.src_id = TISCI_DEV_MMCSD1;
    rmIrqRelease.src_index = 0;

    rmIrqReq.valid_params |= TISCI_MSG_VALUE_RM_DST_ID_VALID;
    rmIrqReq.valid_params |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
    rmIrqRelease.valid_params |= TISCI_MSG_VALUE_RM_DST_ID_VALID;
    rmIrqRelease.valid_params |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;

    if ((uint16_t)MAIN2MCU_LVL_INTRTR0_OUTL_33 < (rangeStart + rangeNum))
    {
        /* Int Router output for MMCSD1 is within MCU1_0 int range */
        rmIrqReq.dst_id = TISCI_DEV_MCU_R5FSS0_CORE0;
        rmIrqReq.dst_host_irq = CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_33;
        rmIrqReq.secondary_host = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

        rmIrqRelease.dst_id = TISCI_DEV_MCU_R5FSS0_CORE0;
        rmIrqRelease.dst_host_irq = CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_33;
        rmIrqRelease.secondary_host = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;
    }
    else
    {
        /* Int Router output for MMCSD1 is within the 2nd core's int range */
        rmIrqReq.dst_id = TISCI_DEV_MCU_R5FSS0_CORE1;
        rmIrqReq.dst_host_irq = CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_33;
        rmIrqReq.secondary_host = TISCI_HOST_ID_MCU_0_R5_2;
        rmIrqReq.valid_params |= TISCI_MSG_VALUE_RM_SECONDARY_HOST_VALID;

        rmIrqRelease.dst_id = TISCI_DEV_MCU_R5FSS0_CORE1;
        rmIrqRelease.dst_host_irq = CSLR_MCU_R5FSS0_CORE1_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_33;
        rmIrqRelease.secondary_host = TISCI_HOST_ID_MCU_0_R5_2;
        rmIrqRelease.valid_params |= TISCI_MSG_VALUE_RM_SECONDARY_HOST_VALID;
    }

    retVal = Sciclient_rmIrqSet(
                (const struct tisci_msg_rm_irq_set_req *)&rmIrqReq,
                &rmIrqResp,
                SCICLIENT_SERVICE_WAIT_FOREVER);

    /* Go ahead and release the MAIN2MCU IR route for MMCSD1 now */
    if(retVal == CSL_PASS)
    {
        retVal = Sciclient_rmIrqRelease(
                (const struct tisci_msg_rm_irq_release_req *)&rmIrqRelease,
                SCICLIENT_SERVICE_WAIT_FOREVER);
    }
}

void Sciserver_socInit(void)
{
    /** Set OUT_CLK_EN bit for ACSPCIE Buffer **/
    Sciserver_enableAcspcie();

    J721E_ClearBootInterrupts();
}
