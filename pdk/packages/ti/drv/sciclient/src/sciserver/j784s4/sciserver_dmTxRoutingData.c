/*
 * Copyright (c) 2022 Texas Instruments Incorporated
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
 * \file dm_tx_routing_data.c
 *
 * \brief DM Tx Routing Data
 */

#include <hosts.h>
#include <sciserver_secproxyConfigData.h>
#include <sciserver_secproxyRoutingDescription.h>

/** Maximum Host Entries */
#define J784S4_CONFIG_MAX (0xFFU)

const uint16_t sproxy_route_data_count = J784S4_CONFIG_MAX;

const struct sproxy_host_tx_thread_data sproxy_route_data[J784S4_CONFIG_MAX] = {
	/*
	 * Non Secure Host MCU_0_R5_0[3]
	 */
	[HOST_ID_MCU_0_R5_0] = {
		.sproxy_resp_conf_id = J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_MCU_0_R5_0_RESPONSE_TX_THR076_CONF152,
		.sproxy_ntfy_conf_id = J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_MCU_0_R5_0_NOTIFY_TX_THR077_CONF151,
	},
	/*
	 * Non Secure Host MCU_0_R5_2[5]
	 */
	[HOST_ID_MCU_0_R5_2] = {
		.sproxy_resp_conf_id = J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_MCU_0_R5_2_RESPONSE_TX_THR074_CONF154,
		.sproxy_ntfy_conf_id = J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_MCU_0_R5_2_NOTIFY_TX_THR075_CONF153,
	},
	/*
	 * Non Secure Host A72_2[12]
	 */
	[HOST_ID_A72_2] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_2_RESPONSE_TX_THR338_CONF025,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_2_NOTIFY_TX_THR339_CONF024,
	},
	/*
	 * Non Secure Host A72_3[13]
	 */
	[HOST_ID_A72_3] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_3_RESPONSE_TX_THR336_CONF027,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_3_NOTIFY_TX_THR337_CONF026,
	},
	/*
	 * Non Secure Host A72_4[14]
	 */
	[HOST_ID_A72_4] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_4_RESPONSE_TX_THR334_CONF029,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_4_NOTIFY_TX_THR335_CONF028,
	},
	/*
	 * Non Secure Host A72_5[15]
	 */
	[HOST_ID_A72_5] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_5_RESPONSE_TX_THR332_CONF031,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_5_NOTIFY_TX_THR333_CONF030,
	},
	/*
	 * Non Secure Host A72_6[16]
	 */
	[HOST_ID_A72_6] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_6_RESPONSE_TX_THR330_CONF033,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_6_NOTIFY_TX_THR331_CONF032,
	},
	/*
	 * Non Secure Host A72_7[17]
	 */
	[HOST_ID_A72_7] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_7_RESPONSE_TX_THR328_CONF035,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_7_NOTIFY_TX_THR329_CONF034,
	},
	/*
	 * Non Secure Host C7X_0_1[21]
	 */
	[HOST_ID_C7X_0_1] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_0_1_RESPONSE_TX_THR326_CONF037,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_0_1_NOTIFY_TX_THR327_CONF036,
	},
	/*
	 * Non Secure Host C7X_1_1[23]
	 */
	[HOST_ID_C7X_1_1] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_1_1_RESPONSE_TX_THR324_CONF039,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_1_1_NOTIFY_TX_THR325_CONF038,
	},
	/*
	 * Non Secure Host C7X_2_1[25]
	 */
	[HOST_ID_C7X_2_1] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_2_1_RESPONSE_TX_THR322_CONF041,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_2_1_NOTIFY_TX_THR323_CONF040,
	},
	/*
	 * Non Secure Host C7X_3_1[27]
	 */
	[HOST_ID_C7X_3_1] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_3_1_RESPONSE_TX_THR320_CONF043,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_C7X_3_1_NOTIFY_TX_THR321_CONF042,
	},
	/*
	 * Non Secure Host A72_8[30]
	 */
	[HOST_ID_A72_8] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_8_RESPONSE_TX_THR318_CONF045,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_8_NOTIFY_TX_THR319_CONF044,
	},
	/*
	 * Non Secure Host MAIN_0_R5_0[35]
	 */
	[HOST_ID_MAIN_0_R5_0] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_0_R5_0_RESPONSE_TX_THR316_CONF047,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_0_R5_0_NOTIFY_TX_THR317_CONF046,
	},
	/*
	 * Non Secure Host MAIN_0_R5_2[37]
	 */
	[HOST_ID_MAIN_0_R5_2] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_0_R5_2_RESPONSE_TX_THR314_CONF049,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_0_R5_2_NOTIFY_TX_THR315_CONF048,
	},
	/*
	 * Non Secure Host MAIN_1_R5_0[40]
	 */
	[HOST_ID_MAIN_1_R5_0] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_1_R5_0_RESPONSE_TX_THR312_CONF051,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_1_R5_0_NOTIFY_TX_THR313_CONF050,
	},
	/*
	 * Non Secure Host MAIN_1_R5_2[42]
	 */
	[HOST_ID_MAIN_1_R5_2] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_1_R5_2_RESPONSE_TX_THR310_CONF053,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_1_R5_2_NOTIFY_TX_THR311_CONF052,
	},
	/*
	 * Non Secure Host MAIN_2_R5_0[45]
	 */
	[HOST_ID_MAIN_2_R5_0] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_2_R5_0_RESPONSE_TX_THR308_CONF055,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_2_R5_0_NOTIFY_TX_THR309_CONF054,
	},
	/*
	 * Non Secure Host MAIN_2_R5_2[47]
	 */
	[HOST_ID_MAIN_2_R5_2] = {
		.sproxy_resp_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_2_R5_2_RESPONSE_TX_THR306_CONF057,
		.sproxy_ntfy_conf_id = J784S4_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_2_R5_2_NOTIFY_TX_THR307_CONF056,
	},
	/*
	 * Non Secure Host TIFS2DM[251]
	 */
	[HOST_ID_TIFS2DM] = {
		.sproxy_resp_conf_id = J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_TIFS2DM_RESPONSE_TX_THR072_CONF156,
		.sproxy_ntfy_conf_id = J784S4_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_TIFS2DM_NOTIFY_TX_THR073_CONF155,
	},
};

