/**
 * @file   emac_utils.h
 */
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

#ifndef EMAC_UTILS_H
#define EMAC_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/drv/emac/emac_drv.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define EMAC_ICSSG_CONFIG_TX_IPG_960_NS_PG1     ((uint32_t)(0x166))     /* 960 NS TX IPG for PG1 */
#define EMAC_ICSSG_CONFIG_TX_IPG_960_NS         ((uint32_t)(0x17))      /* 960 NS TX IPG for PG2 */
#define EMAC_ICSSG_CONFIG_TX_IPG_104_NS_PG1     ((uint32_t)(0x1A))      /* 104 NS TX IPG for PG1 */
#define EMAC_ICSSG_CONFIG_TX_IPG_104_NS         ((uint32_t)(0xB))       /* 104 NS TX IPG for PG2 */

extern void emac_hw_mem_write(uintptr_t addr, const void *ptr, uint32_t element_count);
extern EMAC_DRV_ERR_E emac_close_v5_common( uint32_t port_num);
extern int32_t emac_close_tx_subsystem (uint32_t port_num);
extern int32_t emac_close_rx_subsystem (uint32_t port_num);
extern int32_t emac_udma_ring_dequeue(Udma_RingHandle ringHandle, EMAC_CPPI_DESC_T **pHostDescriptor);
extern EMAC_DRV_ERR_E emac_udma_ring_enqueue(Udma_RingHandle ringHandle, EMAC_CPPI_DESC_T *pHostDescriptor,uint32_t packetSize);
extern EMAC_DRV_ERR_E emac_setup_tx_subsystem (uint32_t port_num, EMAC_OPEN_CONFIG_INFO_T *p_config, EMAC_HwAttrs_V5 *pHwAttrs);
extern void emac_poll_tx_complete(uint32_t port_num, uint32_t ringNum);
extern EMAC_DRV_ERR_E emac_poll_tx_ring(uint32_t port_num, Udma_RingHandle compRingHandle, uint32_t ringNum);
extern void emac_poll_tx_ts_resp(uint32_t port_num, uint32_t ringNum);

extern int32_t emac_open_udma_channel_tx(uint32_t portNum, EMAC_PER_CHANNEL_CFG_TX*pChCfg);
extern void emac_tx_ready_push(uint32_t port_num, uint32_t size, physptr_t phys, uint32_t ringNum, Udma_RingHandle rxRingHandle);

extern EMAC_DRV_ERR_E emac_setup_rx_subsystem (uint32_t port_num, EMAC_OPEN_CONFIG_INFO_T *p_config, EMAC_HwAttrs_V5 *pHwAttrs);
extern int32_t emac_setup_udma_channel_rx(uint32_t portNum,EMAC_PER_CHANNEL_CFG_RX* pChCfg, uint32_t channelType, EMAC_CHANNEL_RX_T* pRxChannel);

extern void emac_poll_mgmt_pkts(uint32_t port_num, uint32_t ringNum);
extern void emac_poll_rx_pkts(uint32_t port_num, uint32_t ringNum);

extern void emac_rx_free_push(uint32_t port_num, uint32_t size, physptr_t phys, uint32_t ringNum, Udma_RingHandle rxRingHandle);

extern int32_t emac_setup_additional_flows(uint32_t portNum, EMAC_PER_CHANNEL_CFG_RX*pChCfg, uint32_t channelType, EMAC_CHANNEL_RX_T* pRxChannel);
extern int32_t emac_setup_udma_channel_interrupt(uint32_t portNum, EMAC_PER_CHANNEL_CFG_RX*pChCfg, uint32_t eventType, EMAC_CHANNEL_RX_T* pRxChannel);

extern void emac_free_hw_cppi_tx_desc(uint32_t p1, uint32_t ch1, EMAC_CPPI_DESC_T* pCppiDesc);
extern void emac_get_hw_cppi_tx_desc(uint32_t p1, uint32_t ch1, EMAC_CPPI_DESC_T** pCppiDesc);
extern bool emac_get_hw_cppi_tx_descs(uint32_t p1, uint32_t ch1, uint32_t p2, uint32_t ch2, EMAC_CPPI_DESC_T** pCppiDesc1, EMAC_CPPI_DESC_T** pCppiDesc2);
EMAC_DRV_ERR_E EMAC_send_v5_common(uint32_t port_num, EMAC_PKT_DESC_T* p_desc, EMAC_CPPI_DESC_T* p_tx_cppi_desc);

extern EMAC_DRV_ERR_E emac_poll_ctrl_v5_common(uint32_t port_num, uint32_t rxPktRings, uint32_t rxMgmtRings, uint32_t txRings);
extern EMAC_DRV_ERR_E emac_poll_pkt_v5_common(uint32_t port_num);
extern EMAC_DRV_ERR_E  emac_open_v5_common(uint32_t port_num, EMAC_OPEN_CONFIG_INFO_T *p_config);

extern void emac_config_icssg_fw(uint32_t portNum, EMAC_HwAttrs_V5 *hwAttrs);

void emac_R30_cmd_init(uint32_t portNum);
int32_t emac_R30_is_done(uint32_t portNum);
EMAC_DRV_ERR_E emac_send_R30_cmd(EMAC_IOctlR30Cmd cmd, uint32_t port, uint8_t sequenceNumber);
extern uintptr_t emac_get_icssg_cfg_base_addr(uint32_t portNum);
extern uintptr_t emac_get_icssg_rgmii_cfg_base_addr(uint32_t portNum);
extern uintptr_t emac_get_icssg_tx_ipg_cfg_base_addr(uint32_t portNum);
extern void emac_icssg_update_rgmii_cfg_100hd(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr);
extern void emac_icssg_update_rgmii_cfg_100fd(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr);
extern void emac_icssg_update_rgmii_cfg_100MB(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr, uint32_t link_status);
extern void emac_icssg_update_rgmii_cfg_1G(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr);
extern void emac_icssg_update_rgmii_cfg_10hd(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr);
extern void emac_icssg_update_rgmii_cfg_10fd(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr);
extern void emac_configure_link_speed_duplexity(uint32_t portNum, uint32_t val);
extern void emac_icssg_update_link_speed_100MB(uint32_t portNum, uint32_t link_status);
extern void emac_icssg_update_link_speed_1G(uint32_t portNum);
extern void emac_icssg_update_rgmii_cfg_10MB(uint32_t portNum, uintptr_t icssgRgmiiCfgBaseAddr, uint32_t link_status);
extern void emac_icssg_update_link_speed_10MB(uint32_t portNum, uint32_t link_status);
extern void emac_icssg_link_down(uint32_t portNum);
extern void emac_icssg_update_link_params(uint32_t portNum, EMAC_LINK_INFO_T *p_info);

#ifdef __cplusplus
}
#endif
#endif
