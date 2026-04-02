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

struct k3_qos_data j721s2_qos_data[] = {
	/* modules_qosConfig0 - 2 endpoints, 10 channels */
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_DMA + 0x100 + 0x4 * 0,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_DMA + 0x100 + 0x4 * 1,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_DMA + 0x100 + 0x4 * 2,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_DMA + 0x100 + 0x4 * 3,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_DMA + 0x100 + 0x4 * 4,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_DMA + 0x100 + 0x4 * 5,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_DMA + 0x100 + 0x4 * 6,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_DMA + 0x100 + 0x4 * 7,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_DMA + 0x100 + 0x4 * 8,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_DMA + 0x100 + 0x4 * 9,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_FBDC + 0x100 + 0x4 * 0,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_FBDC + 0x100 + 0x4 * 1,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_FBDC + 0x100 + 0x4 * 2,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_FBDC + 0x100 + 0x4 * 3,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_FBDC + 0x100 + 0x4 * 4,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_FBDC + 0x100 + 0x4 * 5,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_FBDC + 0x100 + 0x4 * 6,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_FBDC + 0x100 + 0x4 * 7,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_FBDC + 0x100 + 0x4 * 8,
		.val = ATYPE_3 | ORDERID_15,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_FBDC + 0x100 + 0x4 * 9,
		.val = ATYPE_3 | ORDERID_15,
	},


	/* Following registers set 1:1 mapping for orderID MAP1/MAP2
	 * remap registers. orderID x is remapped to orderID x again
	 * This is to ensure orderID from MAP register is unchanged
	 */

	/* K3_DSS_MAIN_0_DSS_INST0_VBUSM_DMA - 1 groups */
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_DMA + 0,
		.val = 0x76543210,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_DMA + 4,
		.val = 0xfedcba98,
	},

	/* K3_DSS_MAIN_0_DSS_INST0_VBUSM_FBDC - 1 groups */
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_FBDC + 0,
		.val = 0x76543210,
	},
	{
		.reg = K3_DSS_MAIN_0_DSS_INST0_VBUSM_FBDC + 4,
		.val = 0xfedcba98,
	},

	/* The following registers map the VBUSM source interfaces to 
	 * VBUSM.C threads. Each bit is for each VBUSM source.
	 * A bit of 0 maps to VBUSM.C Thread0 (NRT)
	 * A bit of 1 maps to VBUSM.C Thread2 (RT)
	 */

	/* NAVSS0_NORTH_0_NBSS_NB0_CFG_MMRS VBUSM source to VBUSM.C thread mapping */
	{
		.reg = NAVSS0_NORTH_0_NBSS_NB0_CFG_MMRS + 0x10,
		.val = NB_THREADMAP_2,
	},
	/* NAVSS0_NORTH_1_NBSS_NB1_CFG_MMRS VBUSM source to VBUSM.C thread mapping */
	{
		.reg = NAVSS0_NORTH_1_NBSS_NB1_CFG_MMRS + 0x10,
		.val = NB_THREADMAP_4,
	},
};

uint32_t j721s2_qos_count = sizeof(j721s2_qos_data) / sizeof(j721s2_qos_data[0]);

/* This function is to be called from other apps (e.g., mcusw boot app) to set QoS settings */
void SBL_SetQoS(){
	uint32_t i;

	for (i = 0; i < j721s2_qos_count; i++){
        CSL_REG32_WR(j721s2_qos_data[i].reg, j721s2_qos_data[i].val);
    }
}

