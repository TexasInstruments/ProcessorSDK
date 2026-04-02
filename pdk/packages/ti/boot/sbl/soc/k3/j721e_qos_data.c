/*
 * Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/
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


#include "sbl_qos.h"

void setup_navss_nb(void)
{
    /* Map orderid 8-15 to VBUSM.C thread 2 (real-time traffic) */
    CSL_REG32_WR(NAVSS0_NBSS_NB0_CFG_NB_THREADMAP, 2);
    CSL_REG32_WR(NAVSS0_NBSS_NB1_CFG_NB_THREADMAP, 2);
}

void setup_vpac_qos(void)
{
    unsigned int channel, group;

    /* vpac data master 0  */
    for (channel = 0; channel < QOS_VPAC0_DATA0_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_VPAC0_DATA0_CBASS_MAP(channel), (QOS_VPAC0_DATA0_ATYPE << 28));
    }

    /* vpac data master 1  */
    for (channel = 0; channel < QOS_VPAC0_DATA1_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_VPAC0_DATA1_CBASS_MAP(channel), (QOS_VPAC0_DATA1_ATYPE << 28));
    }

    /* vpac ldc0  */
    for (group = 0; group < QOS_VPAC0_LDC0_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_VPAC0_LDC0_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_VPAC0_LDC0_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_VPAC0_LDC0_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_VPAC0_LDC0_CBASS_MAP(channel), (QOS_VPAC0_LDC0_ATYPE << 28) | (QOS_VPAC0_LDC0_PRIORITY << 12) | (QOS_VPAC0_LDC0_ORDER_ID << 4));
    }

}

void setup_dmpac_qos(void)
{
    unsigned int channel;

    /* dmpac data  */
    for (channel = 0; channel < QOS_DMPAC0_DATA_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_DMPAC0_DATA_CBASS_MAP(channel), (QOS_DMPAC0_DATA_ATYPE << 28));
    }
}

void setup_dss_qos(void)
{
    unsigned int channel, group;

    /* two master ports: dma and fbdc */
    /* two groups: SRAM and DDR */
    /* 10 channels: (pipe << 1) | is_second_buffer */

    /* master port 1 (dma) */
    for (group = 0; group < QOS_DSS0_DMA_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_DSS0_DMA_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_DSS0_DMA_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_DSS0_DMA_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_DSS0_DMA_CBASS_MAP(channel), (QOS_DSS0_DMA_ATYPE << 28) | (QOS_DSS0_DMA_PRIORITY << 12) | (QOS_DSS0_DMA_ORDER_ID << 4));
    }

    /* master port 2 (fbdc) */
    for (group = 0; group < QOS_DSS0_FBDC_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_DSS0_FBDC_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_DSS0_FBDC_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_DSS0_FBDC_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_DSS0_FBDC_CBASS_MAP(channel), (QOS_DSS0_FBDC_ATYPE << 28) | (QOS_DSS0_FBDC_PRIORITY << 12) | (QOS_DSS0_FBDC_ORDER_ID << 4));
    }
}

void setup_gpu_qos(void)
{
    unsigned int channel, group;

    /* gpu m0 rd */
    for (group = 0; group < QOS_GPU0_M0_RD_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_GPU0_M0_RD_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_GPU0_M0_RD_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_GPU0_M0_RD_NUM_I_CH; ++channel)
    {
        if(channel == 0)
        {
            CSL_REG32_WR(QOS_GPU0_M0_RD_CBASS_MAP(channel), (QOS_GPU0_M0_RD_ATYPE << 28) | (QOS_GPU0_M0_RD_MMU_PRIORITY << 12) | (QOS_GPU0_M0_RD_ORDER_ID << 4));
        }
        else
        {
            CSL_REG32_WR(QOS_GPU0_M0_RD_CBASS_MAP(channel), (QOS_GPU0_M0_RD_ATYPE << 28) | (QOS_GPU0_M0_RD_PRIORITY << 12) | (QOS_GPU0_M0_RD_ORDER_ID << 4));
        }
    }

    /* gpu m0 wr */
    for (group = 0; group < QOS_GPU0_M0_WR_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_GPU0_M0_WR_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_GPU0_M0_WR_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_GPU0_M0_WR_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_GPU0_M0_WR_CBASS_MAP(channel), (QOS_GPU0_M0_WR_ATYPE << 28) | (QOS_GPU0_M0_WR_PRIORITY << 12) | (QOS_GPU0_M0_WR_ORDER_ID << 4));
    }

    /* gpu m1 rd */
    for (group = 0; group < QOS_GPU0_M1_RD_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_GPU0_M1_RD_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_GPU0_M1_RD_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_GPU0_M1_RD_NUM_I_CH; ++channel)
    {
        if(channel == 0)
        {
            CSL_REG32_WR(QOS_GPU0_M1_RD_CBASS_MAP(channel), (QOS_GPU0_M1_RD_ATYPE << 28) | (QOS_GPU0_M1_RD_MMU_PRIORITY << 12) | (QOS_GPU0_M1_RD_ORDER_ID << 4));
        }
        else
        {
            CSL_REG32_WR(QOS_GPU0_M1_RD_CBASS_MAP(channel), (QOS_GPU0_M1_RD_ATYPE << 28) | (QOS_GPU0_M1_RD_PRIORITY << 12) | (QOS_GPU0_M1_RD_ORDER_ID << 4));
        }
    }

    /* gpu m1 wr */
    for (group = 0; group < QOS_GPU0_M1_WR_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_GPU0_M1_WR_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_GPU0_M1_WR_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_GPU0_M1_WR_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_GPU0_M1_WR_CBASS_MAP(channel), (QOS_GPU0_M1_WR_ATYPE << 28) | (QOS_GPU0_M1_WR_PRIORITY << 12) | (QOS_GPU0_M1_WR_ORDER_ID << 4));
    }
}

void setup_encoder_qos(void)
{
    unsigned int channel, group;

    /* encoder rd */
    for (group = 0; group < QOS_ENCODER0_RD_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_ENCODER0_RD_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_ENCODER0_RD_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_ENCODER0_RD_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_ENCODER0_RD_CBASS_MAP(channel), (QOS_ENCODER0_RD_ATYPE << 28) | (QOS_ENCODER0_RD_PRIORITY << 12) | (QOS_ENCODER0_RD_ORDER_ID << 4));
    }

    /* encoder wr */
    for (group = 0; group < QOS_ENCODER0_WR_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_ENCODER0_WR_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_ENCODER0_WR_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_ENCODER0_WR_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_ENCODER0_WR_CBASS_MAP(channel), (QOS_ENCODER0_WR_ATYPE << 28) | (QOS_ENCODER0_WR_PRIORITY << 12) | (QOS_ENCODER0_WR_ORDER_ID << 4));
    }
}

void setup_decoder_qos(void)
{
    unsigned int channel, group;

    /* decoder rd */
    for (group = 0; group < QOS_DECODER0_RD_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_DECODER0_RD_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_DECODER0_RD_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_DECODER0_RD_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_DECODER0_RD_CBASS_MAP(channel), (QOS_DECODER0_RD_ATYPE << 28) | (QOS_DECODER0_RD_PRIORITY << 12) | (QOS_DECODER0_RD_ORDER_ID << 4));
    }

    /* decoder wr */
    for (group = 0; group < QOS_DECODER0_WR_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_DECODER0_WR_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_DECODER0_WR_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_DECODER0_WR_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_DECODER0_WR_CBASS_MAP(channel), (QOS_DECODER0_WR_ATYPE << 28) | (QOS_DECODER0_WR_PRIORITY << 12) | (QOS_DECODER0_WR_ORDER_ID << 4));
    }
}

void setup_c66_qos(void)
{
    unsigned int channel, group;

    /* c66_0 mdma */
    for (group = 0; group < QOS_C66SS0_MDMA_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_C66SS0_MDMA_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_C66SS0_MDMA_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_C66SS0_MDMA_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_C66SS0_MDMA_CBASS_MAP(channel), (QOS_C66SS0_MDMA_ATYPE << 28) | (QOS_C66SS0_MDMA_PRIORITY << 12) | (QOS_C66SS0_MDMA_ORDER_ID << 4));
    }

    /* c66_1 mdma */
    for (group = 0; group < QOS_C66SS1_MDMA_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_C66SS1_MDMA_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_C66SS1_MDMA_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_C66SS1_MDMA_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_C66SS1_MDMA_CBASS_MAP(channel), (QOS_C66SS1_MDMA_ATYPE << 28) | (QOS_C66SS1_MDMA_PRIORITY << 12) | (QOS_C66SS1_MDMA_ORDER_ID << 4));
    }
}

void setup_main_r5f_qos(void)
{
    unsigned int channel, group;

    /* R5FSS0 core0 - read */
    for (group = 0; group < QOS_R5FSS0_CORE0_MEM_RD_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_R5FSS0_CORE0_MEM_RD_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_R5FSS0_CORE0_MEM_RD_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_R5FSS0_CORE0_MEM_RD_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_R5FSS0_CORE0_MEM_RD_CBASS_MAP(channel), (QOS_R5FSS0_CORE0_MEM_RD_ATYPE << 28) | (QOS_R5FSS0_CORE0_MEM_RD_PRIORITY << 12) | (QOS_R5FSS0_CORE0_MEM_RD_ORDER_ID << 4));
    }

    /* R5FSS0 core0 - write */
    for (group = 0; group < QOS_R5FSS0_CORE0_MEM_WR_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_R5FSS0_CORE0_MEM_WR_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_R5FSS0_CORE0_MEM_WR_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_R5FSS0_CORE0_MEM_WR_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_R5FSS0_CORE0_MEM_WR_CBASS_MAP(channel), (QOS_R5FSS0_CORE0_MEM_WR_ATYPE << 28) | (QOS_R5FSS0_CORE0_MEM_WR_PRIORITY << 12) | (QOS_R5FSS0_CORE0_MEM_RD_ORDER_ID << 4));
    }

    /* R5FSS0 core1 - read */
    for (group = 0; group < QOS_R5FSS0_CORE1_MEM_RD_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_R5FSS0_CORE1_MEM_RD_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_R5FSS0_CORE1_MEM_RD_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_R5FSS0_CORE1_MEM_RD_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_R5FSS0_CORE1_MEM_RD_CBASS_MAP(channel), (QOS_R5FSS0_CORE1_MEM_RD_ATYPE << 28) | (QOS_R5FSS0_CORE1_MEM_RD_PRIORITY << 12) | (QOS_R5FSS0_CORE0_MEM_RD_ORDER_ID << 4));
    }

    /* R5FSS0 core1 - write */
    for (group = 0; group < QOS_R5FSS0_CORE1_MEM_WR_NUM_J_CH; ++group)
    {
        CSL_REG32_WR(QOS_R5FSS0_CORE1_MEM_WR_CBASS_GRP_MAP1(group), 0x76543210);
        CSL_REG32_WR(QOS_R5FSS0_CORE1_MEM_WR_CBASS_GRP_MAP2(group), 0xfedcba98);
    }

    for (channel = 0; channel < QOS_R5FSS0_CORE1_MEM_WR_NUM_I_CH; ++channel)
    {
        CSL_REG32_WR(QOS_R5FSS0_CORE1_MEM_WR_CBASS_MAP(channel), (QOS_R5FSS0_CORE1_MEM_WR_ATYPE << 28) | (QOS_R5FSS0_CORE1_MEM_WR_PRIORITY << 12) | (QOS_R5FSS0_CORE1_MEM_RD_ORDER_ID << 4));
    }
}

/* This function is to be called from other apps (e.g., mcusw boot app) to set QoS settings */
void SBL_SetQoS()
{
    setup_navss_nb();
    setup_c66_qos();
    /* Workaround to unblock PDK-8359 .
     * setup_main_r5f_qos() results in crashing the UDMA DRU examples on
     * mcu2_0(with SBL uart boot mode) during CSL_REG64_WR(&pRegs->DRUQueues.CFG[queueId], regVal);
     * Hence commenting out the following. */
    //setup_main_r5f_qos();
    setup_vpac_qos();
    setup_dmpac_qos();
    setup_dss_qos();
    setup_gpu_qos();
    setup_encoder_qos();
}
