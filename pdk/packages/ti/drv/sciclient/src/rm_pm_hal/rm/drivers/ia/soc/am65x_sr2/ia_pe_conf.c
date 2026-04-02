/*
 * System Firmware Source File
 *
 * Static IA IRQ maps for Processing Entities for AM6 device
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
 * \file ia_pe_conf.c
 *
 * \brief Static IA IRQ maps for Processing Entities for AM6 device
 */

#include <devices.h>

#include "../../include/ia_init.h"
#include "ia_pe_conf.h"

const struct ia_init ia_soc_pe_init_list[IA_SOC_PE_INIT_NUM] = {
	[0] =  {
		/* For:  r5_0_notify_err_event r5_0_notify_thr_event */
		.id		= AM6_DEV_MCU_NAVSS0_INTR_AGGR_0,
		.event_id	= 0x4000U,
		.vint		= 0x00U,
	},
	[1] =  {
		/* For:  r5_0_response_err_event r5_0_response_thr_event */
		.id		= AM6_DEV_MCU_NAVSS0_INTR_AGGR_0,
		.event_id	= 0x4001U,
		.vint		= 0x01U,
	},
	[2] =  {
		/* For:  r5_1_notify_err_event r5_1_notify_thr_event */
		.id		= AM6_DEV_MCU_NAVSS0_INTR_AGGR_0,
		.event_id	= 0x4002U,
		.vint		= 0x02U,
	},
	[3] =  {
		/* For:  r5_1_response_err_event r5_1_response_thr_event */
		.id		= AM6_DEV_MCU_NAVSS0_INTR_AGGR_0,
		.event_id	= 0x4003U,
		.vint		= 0x03U,
	},
	[4] =  {
		/* For:  r5_2_notify_err_event r5_2_notify_thr_event */
		.id		= AM6_DEV_MCU_NAVSS0_INTR_AGGR_0,
		.event_id	= 0x4004U,
		.vint		= 0x04U,
	},
	[5] =  {
		/* For:  r5_2_response_err_event r5_2_response_thr_event */
		.id		= AM6_DEV_MCU_NAVSS0_INTR_AGGR_0,
		.event_id	= 0x4005U,
		.vint		= 0x05U,
	},
	[6] =  {
		/* For:  r5_3_notify_err_event r5_3_notify_thr_event */
		.id		= AM6_DEV_MCU_NAVSS0_INTR_AGGR_0,
		.event_id	= 0x4006U,
		.vint		= 0x06U,
	},
	[7] =  {
		/* For:  r5_3_response_err_event r5_3_response_thr_event */
		.id		= AM6_DEV_MCU_NAVSS0_INTR_AGGR_0,
		.event_id	= 0x4007U,
		.vint		= 0x07U,
	},
	[8] =  {
		/* For:  a53_0_notify_err_event a53_0_notify_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x0000U,
		.vint		= 0x00U,
	},
	[9] =  {
		/* For:  a53_0_response_err_event a53_0_response_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x0001U,
		.vint		= 0x01U,
	},
	[10] = {
		/* For:  a53_1_notify_err_event a53_1_notify_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x0002U,
		.vint		= 0x02U,
	},
	[11] = {
		/* For:  a53_1_response_err_event a53_1_response_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x0003U,
		.vint		= 0x03U,
	},
	[12] = {
		/* For:  a53_2_notify_err_event a53_2_notify_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x0004U,
		.vint		= 0x04U,
	},
	[13] = {
		/* For:  a53_2_response_err_event a53_2_response_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x0005U,
		.vint		= 0x05U,
	},
	[14] = {
		/* For:  a53_3_notify_err_event a53_3_notify_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x0006U,
		.vint		= 0x06U,
	},
	[15] = {
		/* For:  a53_3_response_err_event a53_3_response_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x0007U,
		.vint		= 0x07U,
	},
	[16] = {
		/* For:  a53_4_notify_err_event a53_4_notify_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x0008U,
		.vint		= 0x08U,
	},
	[17] = {
		/* For:  a53_4_response_err_event a53_4_response_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x0009U,
		.vint		= 0x09U,
	},
	[18] = {
		/* For:  a53_5_notify_err_event a53_5_notify_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x000AU,
		.vint		= 0x0AU,
	},
	[19] = {
		/* For:  a53_5_response_err_event a53_5_response_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x000BU,
		.vint		= 0x0BU,
	},
	[20] = {
		/* For:  a53_6_notify_err_event a53_6_notify_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x000CU,
		.vint		= 0x0CU,
	},
	[21] = {
		/* For:  a53_6_response_err_event a53_6_response_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x000DU,
		.vint		= 0x0DU,
	},
	[22] = {
		/* For:  a53_7_notify_err_event a53_7_notify_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x000EU,
		.vint		= 0x0EU,
	},
	[23] = {
		/* For:  a53_7_response_err_event a53_7_response_thr_event */
		.id		= AM6_DEV_NAVSS0_UDMASS_INTA0,
		.event_id	= 0x000FU,
		.vint		= 0x0FU,
	},
};
