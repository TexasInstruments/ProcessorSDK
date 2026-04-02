/*
 * System Firmware Source File
 *
 * Static IR IRQ maps for Processing Entities for AM6 device
 *
 * Copyright (C) 2018-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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
 * \file ir_pe_conf.c
 *
 * \brief Static IR IRQ maps for Processing Entities for AM6 device
 */

#include <devices.h>

#include "../../include/ir_init.h"
#include "ir_pe_conf.h"

const struct ir_init ir_soc_pe_init_list[IR_SOC_PE_INIT_NUM] = {
	[0] =  {
		/*
		 * For: r5_0_notify_err_event r5_0_notify_thr_event:
		 * mcu_armss0_cpu0/bus_intr_64
		 */
		.id	= AM6_DEV_MCU_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x00U,
		.outp	= 0x00U,
	},
	[1] =  {
		/*
		 * For: r5_0_response_err_event r5_0_response_thr_event:
		 * mcu_armss0_cpu0/bus_intr_65
		 */
		.id	= AM6_DEV_MCU_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x01U,
		.outp	= 0x01U,
	},
	[2] =  {
		/*
		 * For: r5_1_notify_err_event r5_1_notify_thr_event:
		 * mcu_armss0_cpu0/bus_intr_66
		 */
		.id	= AM6_DEV_MCU_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x02U,
		.outp	= 0x02U,
	},
	[3] =  {
		/*
		 * For: r5_1_response_err_event r5_1_response_thr_event:
		 * mcu_armss0_cpu0/bus_intr_67
		 */
		.id	= AM6_DEV_MCU_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x03U,
		.outp	= 0x03U,
	},
	[4] =  {
		/*
		 * For: r5_2_notify_err_event r5_2_notify_thr_event:
		 * mcu_armss0_cpu1/bus_intr_64
		 */
		.id	= AM6_DEV_MCU_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x04U,
		.outp	= 0x20U,
	},
	[5] =  {
		/*
		 * For: r5_2_response_err_event r5_2_response_thr_event:
		 * mcu_armss0_cpu1/bus_intr_65
		 */
		.id	= AM6_DEV_MCU_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x05U,
		.outp	= 0x21U,
	},
	[6] =  {
		/*
		 * For: r5_3_notify_err_event r5_3_notify_thr_event:
		 * mcu_armss0_cpu1/bus_intr_66
		 */
		.id	= AM6_DEV_MCU_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x06U,
		.outp	= 0x22U,
	},
	[7] =  {
		/*
		 * For: r5_3_response_err_event r5_3_response_thr_event:
		 * mcu_armss0_cpu1/bus_intr_67
		 */
		.id	= AM6_DEV_MCU_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x07U,
		.outp	= 0x23U,
	},
	[8] =  {
		/*
		 * For: a53_0_notify_err_event a53_0_notify_thr_event:
		 * gic500ss_main_0/bus_spi_64
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x00U,
		.outp	= 0x00U,
	},
	[9] =  {
		/*
		 * For: a53_0_response_err_event a53_0_response_thr_event:
		 * gic500ss_main_0/bus_spi_65
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x01U,
		.outp	= 0x01U,
	},
	[10] = {
		/*
		 * For: a53_1_notify_err_event a53_1_notify_thr_event:
		 * gic500ss_main_0/bus_spi_66
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x02U,
		.outp	= 0x02U,
	},
	[11] = {
		/*
		 * For: a53_1_response_err_event a53_1_response_thr_event:
		 * gic500ss_main_0/bus_spi_67
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x03U,
		.outp	= 0x03U,
	},
	[12] = {
		/*
		 * For: a53_2_notify_err_event a53_2_notify_thr_event:
		 * gic500ss_main_0/bus_spi_68
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x04U,
		.outp	= 0x04U,
	},
	[13] = {
		/*
		 * For: a53_2_response_err_event a53_2_response_thr_event:
		 * gic500ss_main_0/bus_spi_69
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x05U,
		.outp	= 0x05U,
	},
	[14] = {
		/*
		 * For: a53_3_notify_err_event a53_3_notify_thr_event:
		 * gic500ss_main_0/bus_spi_70
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x06U,
		.outp	= 0x06U,
	},
	[15] = {
		/*
		 * For: a53_3_response_err_event a53_3_response_thr_event:
		 * gic500ss_main_0/bus_spi_71
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x07U,
		.outp	= 0x07U,
	},
	[16] = {
		/*
		 * For: a53_4_notify_err_event a53_4_notify_thr_event:
		 * gic500ss_main_0/bus_spi_72
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x08U,
		.outp	= 0x08U,
	},
	[17] = {
		/*
		 * For: a53_4_response_err_event a53_4_response_thr_event:
		 * gic500ss_main_0/bus_spi_73
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x09U,
		.outp	= 0x09U,
	},
	[18] = {
		/*
		 * For: a53_5_notify_err_event a53_5_notify_thr_event:
		 * gic500ss_main_0/bus_spi_74
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x0AU,
		.outp	= 0x0AU,
	},
	[19] = {
		/*
		 * For: a53_5_response_err_event a53_5_response_thr_event:
		 * gic500ss_main_0/bus_spi_75
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x0BU,
		.outp	= 0x0BU,
	},
	[20] = {
		/*
		 * For: a53_6_notify_err_event a53_6_notify_thr_event:
		 * gic500ss_main_0/bus_spi_76
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x0CU,
		.outp	= 0x0CU,
	},
	[21] = {
		/*
		 * For: a53_6_response_err_event a53_6_response_thr_event:
		 * gic500ss_main_0/bus_spi_77
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x0DU,
		.outp	= 0x0DU,
	},
	[22] = {
		/*
		 * For: a53_7_notify_err_event a53_7_notify_thr_event:
		 * gic500ss_main_0/bus_spi_78
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x0EU,
		.outp	= 0x0EU,
	},
	[23] = {
		/*
		 * For: a53_7_response_err_event a53_7_response_thr_event:
		 * gic500ss_main_0/bus_spi_79
		 */
		.id	= AM6_DEV_NAVSS0_INTR_ROUTER_0,
		.inp	= 0x0FU,
		.outp	= 0x0FU,
	},
};
