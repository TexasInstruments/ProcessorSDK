/**
 *  \file   emac_hwq.c
 *
 *  \brief  EMAC driver interface to ICSSG hardware queues
 *
 *  This file contains internal functions to push/pop from ICSSG hardware queues
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
#include <ti/csl/cslr_icss.h>
#include <ti/drv/emac/src/v5/emac_drv_v5.h>

#define EMAC_HWQ_MAX         ((int32_t)64) /* 64 registers, 1 per queue */
#define EMAC_HWQ_PEEK_MAX    ((int32_t)16) /* 16 peekable hw queues, currently not used */

typedef struct EMAC_HWQ_IP_MMAP_S
{
    volatile uint32_t hwq[EMAC_HWQ_MAX];
    volatile uint32_t hwq_peek[EMAC_HWQ_PEEK_MAX];
    volatile uint32_t hwq_len[EMAC_HWQ_MAX]; // how many buffers in the hw queue 
    volatile uint32_t reset;
} EMAC_HWQ_IP_T;

/*desricptors are in smem, there are 8 per hw q in our implemenation */
#if defined (SOC_AM65XX)
static uint32_t emac_hwq_smem_start[EMAC_MAX_ICSSG_INSTANCES] = { CSL_PRU_ICSSG0_RAM_SLV_RAM_BASE, CSL_PRU_ICSSG1_RAM_SLV_RAM_BASE, CSL_PRU_ICSSG2_RAM_SLV_RAM_BASE};
static EMAC_HWQ_IP_T *emac_hwq_a[EMAC_MAX_ICSSG_INSTANCES] =
{
    (EMAC_HWQ_IP_T*) (CSL_PRU_ICSSG0_DRAM0_SLV_RAM_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0),
    (EMAC_HWQ_IP_T*) (CSL_PRU_ICSSG1_DRAM0_SLV_RAM_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0),
    (EMAC_HWQ_IP_T*) (CSL_PRU_ICSSG2_DRAM0_SLV_RAM_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0)
};
#else
static uint32_t emac_hwq_smem_start[EMAC_MAX_ICSSG_INSTANCES] = { CSL_PRU_ICSSG0_RAM_SLV_RAM_BASE, CSL_PRU_ICSSG1_RAM_SLV_RAM_BASE};
static EMAC_HWQ_IP_T *emac_hwq_a[EMAC_MAX_ICSSG_INSTANCES] =
{
    (EMAC_HWQ_IP_T*) (CSL_PRU_ICSSG0_DRAM0_SLV_RAM_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0),
    (EMAC_HWQ_IP_T*) (CSL_PRU_ICSSG1_DRAM0_SLV_RAM_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_REGS_BASE + CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_QUEUE0)
};
#endif

void *emac_hwq_pop(int32_t icssg, int32_t qn)
{
    if (qn >= EMAC_HWQ_MAX || icssg >= (int32_t)EMAC_MAX_ICSSG_INSTANCES )
        return (void *)NULL;

    /* (emac_hwq_a[icssg]->hwq[qn]  will get you a 16 bit address from the mmr of the descriptor, this is actually a pop */
    return (void*)((uintptr_t)(emac_hwq_a[icssg]->hwq[qn] | emac_hwq_smem_start[icssg]));
}

void emac_hwq_push(int32_t icssg, int32_t qn, void *p)
{
    if (qn >= EMAC_HWQ_MAX || icssg >= (int32_t)EMAC_MAX_ICSSG_INSTANCES )
        return;

    emac_hwq_a[icssg]->hwq[qn] = (uint32_t)(uintptr_t)p;
}

void *emac_hwq_peek(int32_t icssg, int32_t qn)
{
    if (qn >= EMAC_HWQ_PEEK_MAX || icssg >= (int32_t)EMAC_MAX_ICSSG_INSTANCES )
        return (void *)0;

    return (void *)(uintptr_t)((emac_hwq_a[icssg]->hwq_peek[qn] | emac_hwq_smem_start[icssg]));
}

int32_t emac_hwq_level(int32_t icssg, int32_t qn)
{
    if (qn >= EMAC_HWQ_MAX || icssg >= (int32_t)EMAC_MAX_ICSSG_INSTANCES )
        return 0;

    return emac_hwq_a[icssg]->hwq_len[qn];
}

