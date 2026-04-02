/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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
#define J7200_CONFIG_MAX (0xFFU)

const uint16_t sproxy_route_data_count = J7200_CONFIG_MAX;

const struct sproxy_host_tx_thread_data sproxy_route_data[J7200_CONFIG_MAX] = {
	/*
	 * Non Secure Host MCU_0_R5_0[3]
	 */
	[HOST_ID_MCU_0_R5_0] = {
		.sproxy_resp_conf_id = J7200_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_MCU_0_R5_0_RESPONSE_TX_THR076_CONF057,
		.sproxy_ntfy_conf_id = J7200_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_MCU_0_R5_0_NOTIFY_TX_THR077_CONF056,
	},
	/*
	 * Non Secure Host MCU_0_R5_2[5]
	 */
	[HOST_ID_MCU_0_R5_2] = {
		.sproxy_resp_conf_id = J7200_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_MCU_0_R5_2_RESPONSE_TX_THR074_CONF059,
		.sproxy_ntfy_conf_id = J7200_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_MCU_0_R5_2_NOTIFY_TX_THR075_CONF058,
	},
	/*
	 * Non Secure Host A72_2[12]
	 */
	[HOST_ID_A72_2] = {
		.sproxy_resp_conf_id = J7200_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_2_RESPONSE_TX_THR144_CONF009,
		.sproxy_ntfy_conf_id = J7200_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_2_NOTIFY_TX_THR145_CONF008,
	},
	/*
	 * Non Secure Host A72_3[13]
	 */
	[HOST_ID_A72_3] = {
		.sproxy_resp_conf_id = J7200_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_3_RESPONSE_TX_THR142_CONF011,
		.sproxy_ntfy_conf_id = J7200_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_3_NOTIFY_TX_THR143_CONF010,
	},
	/*
	 * Non Secure Host A72_4[14]
	 */
	[HOST_ID_A72_4] = {
		.sproxy_resp_conf_id = J7200_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_4_RESPONSE_TX_THR140_CONF013,
		.sproxy_ntfy_conf_id = J7200_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_4_NOTIFY_TX_THR141_CONF012,
	},
    /*
     * Non Secure Host A72_5[30]
     */
    [HOST_ID_A72_5] =    {
        .sproxy_resp_conf_id    = J7200_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_5_RESPONSE_TX_THR134_CONF019,
        .sproxy_ntfy_conf_id    = J7200_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_A72_5_NOTIFY_TX_THR135_CONF018,
    },
    /*
	 * Non Secure Host MAIN_0_R5_0[35]
	 */
	[HOST_ID_MAIN_0_R5_0] = {
		.sproxy_resp_conf_id = J7200_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_0_R5_0_RESPONSE_TX_THR138_CONF015,
		.sproxy_ntfy_conf_id = J7200_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_0_R5_0_NOTIFY_TX_THR139_CONF014,
	},
	/*
	 * Non Secure Host MAIN_0_R5_2[37]
	 */
	[HOST_ID_MAIN_0_R5_2] = {
		.sproxy_resp_conf_id = J7200_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_0_R5_2_RESPONSE_TX_THR136_CONF017,
		.sproxy_ntfy_conf_id = J7200_NAVSS0_SEC_PROXY_0_TX_DM_NONSEC_MAIN_0_R5_2_NOTIFY_TX_THR137_CONF016,
	},
	/*
	 * Non Secure Host DMSC2DM[251]
	 */
	[HOST_ID_DMSC2DM] = {
		.sproxy_resp_conf_id = J7200_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_DMSC2DM_RESPONSE_TX_THR072_CONF061,
		.sproxy_ntfy_conf_id = J7200_MCU_NAVSS0_SEC_PROXY0_TX_DM_NONSEC_DMSC2DM_NOTIFY_TX_THR073_CONF060,
	},
};
