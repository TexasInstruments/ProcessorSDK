/**
 *  \file   emac_drv_v5.c
 *
 *  \brief  emac driver API implementation for AM6 for ICSS-G dual mac use case
 *
 *  This file contains the driver APIs for EMAC peripheral for AM6
 */

/*
 * Copyright (C) 2018-2020 Texas Instruments Incorporated - http://www.ti.com/
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
/* CSL Headers */
#include <ti/csl/soc.h>
#include <ti/drv/udma/src/udmap/csl_udmap.h>

#include <ti/csl/cslr_mdio.h>
#include <ti/csl/cslr_icss.h>

#include <ti/drv/emac/emac_drv.h>
#include "ti/drv/emac/src/v5/emac_drv_v5.h"
#include "ti/drv/emac/src/v5/emac_utils.h"

/**********************************************************************
 *************************** EMAC Global Variables*********************
 **********************************************************************/
extern EMAC_MCB_V5_T      emac_mcb;
extern EMAC_PORT_INFO_T emac_port_info[];
extern EMAC_IOCTL_CMD_T cmd1Icssg;
extern EMAC_IOCTL_CMD_T cmd2Icssg;

/* EMAC_v5 local functions */
static EMAC_DRV_ERR_E emac_open_v5_icssg_dual_mac(uint32_t portNum,  EMAC_OPEN_CONFIG_INFO_T *pConfig);
static EMAC_DRV_ERR_E emac_config_v5_icssg_dual_mac(uint32_t portNum,  EMAC_CONFIG_INFO_T *pConfig);
static EMAC_DRV_ERR_E emac_close_v5_icssg_dual_mac(uint32_t portNum);
static EMAC_DRV_ERR_E emac_poll_pkt_v5_icssg_dual_mac(uint32_t portNum);
static EMAC_DRV_ERR_E emac_send_v5_icssg_dual_mac(uint32_t portNum, EMAC_PKT_DESC_T* pDesc);
static EMAC_DRV_ERR_E emac_poll_v5_icssg_dual_mac(uint32_t portNum, EMAC_LINK_INFO_T* p_info);
static EMAC_DRV_ERR_E emac_get_stats_v5_icssg_dual_mac(uint32_t portNum, EMAC_STATISTICS_T*      p_stats);
static EMAC_DRV_ERR_E emac_get_stats_icssg_v5_icssg_dual_mac(uint32_t portNum, EMAC_STATISTICS_ICSSG_T *p_stats, bool clear);
static EMAC_DRV_ERR_E emac_ioctl_v5_icssg_dual_mac(uint32_t portNum, EMAC_IOCTL_CMD emacIoctlCmd, EMAC_IOCTL_PARAMS *emacIoctlParams);
static EMAC_DRV_ERR_E emac_poll_ctrl_v5_icssg_dual_mac(uint32_t portNum, uint32_t rxPktRings, uint32_t rxMgmtRings, uint32_t txRings);

/* EMAC function table for EMAC_v5 implementation */
EMAC_FxnTable EMAC_FxnTable_ICSSG_DM_v5 =
{
    emac_open_v5_icssg_dual_mac,
    emac_config_v5_icssg_dual_mac,
    emac_close_v5_icssg_dual_mac,
    emac_send_v5_icssg_dual_mac,
    emac_poll_v5_icssg_dual_mac,
    emac_get_stats_v5_icssg_dual_mac,
    emac_poll_pkt_v5_icssg_dual_mac,
    emac_get_stats_icssg_v5_icssg_dual_mac,
    emac_ioctl_v5_icssg_dual_mac,
    emac_poll_ctrl_v5_icssg_dual_mac
};

/*
 *  ======== emac_open_v5_icssg_dual_mac ========
 */
static EMAC_DRV_ERR_E  emac_open_v5_icssg_dual_mac(uint32_t portNum, EMAC_OPEN_CONFIG_INFO_T *pOpenConfig)
{
    EMAC_HwAttrs_V5       *pHwAttrs;
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OPEN_PORT_ERR;
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    if (pOpenConfig != NULL)
    {
        retVal = emac_open_v5_common(portNum, pOpenConfig);
        if (retVal == EMAC_DRV_RESULT_OK)
        {
            pHwAttrs = (EMAC_HwAttrs_V5*)pOpenConfig->hwAttrs;
            if (pHwAttrs->portCfg[portNum].getFwCfg != NULL)
            {
                emac_mcb.port_cb[portNum].getFwCfg= pHwAttrs->portCfg[portNum].getFwCfg;
                emac_mcb.ioctl_cb.pCmd1Icssg = &cmd1Icssg;
                emac_mcb.port_cb[portNum].icssSharedRamBaseAddr = pHwAttrs->portCfg[portNum].icssSharedRamBaseAddr;
                emac_mcb.port_cb[portNum].icssgCfgRegBaseAddr = pHwAttrs->portCfg[portNum].icssgCfgRegBaseAddr;
                emac_mcb.port_cb[portNum].icssDram0BaseAddr = pHwAttrs->portCfg[portNum].icssDram0BaseAddr;
                emac_mcb.port_cb[portNum].icssDram1BaseAddr = pHwAttrs->portCfg[portNum].icssDram1BaseAddr;

                /* disable all classifiers */
                emac_classifier_disable(portNum);
                emac_config_icssg_fw(portNum, pHwAttrs);
                /* Update classifier/filters for PORT_MAC address */
                emac_ioctl_icss_add_mac(portNum,pOpenConfig->p_chan_mac_addr->p_mac_addr->addr);
                emac_mcb.port_cb[portNum].emacState =EMAC_PORT_STATE_OPEN;
            }
        }
    }
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",
                                        portNum, retVal);
    return retVal;
}

/*
 *  ======== Emac_config_v5_icssg_dual_mac ========
 */
static EMAC_DRV_ERR_E  emac_config_v5_icssg_dual_mac(uint32_t portNum,  EMAC_CONFIG_INFO_T *pConfig)
{
    return EMAC_DRV_RESULT_OK;
} /* Emac_config_v5_icssg_dual_mac */

/*
 *  ======== Emac_close_v5_icssg_dual_mac ========
 */
static EMAC_DRV_ERR_E  emac_close_v5_icssg_dual_mac(uint32_t portNum)
{
    EMAC_DRV_ERR_E retVal;
    retVal = emac_close_v5_common(portNum);
    return retVal; 
} /* Emac_close_v5_icssg_dual_mac */


/*
 *  ======== EMAC_get_stats_v5_icssg_dual_mac ========
 */
static EMAC_DRV_ERR_E emac_get_stats_v5_icssg_dual_mac(uint32_t portNum, EMAC_STATISTICS_T*      p_stats)
{
    return EMAC_DRV_RESULT_INVALID_PORT;
} /* EMAC_get_stats_v5_icssg_dual_mac */

/*
 *  ======== emac_read_icssg_hw_stats_icssg_dual_mac ========
 */
void emac_read_icssg_hw_stats_icssg_dual_mac(uintptr_t statsBaseAddr, uint32_t* statPtr, uint8_t statsOffset, bool clear)

{
    uint32_t *tempStatsPtr = (uint32_t*)statPtr;
    uint8_t i;
    for (i = statsOffset;  i < sizeof(EMAC_STATISTICS_ICSSG_T)/4; i++)
    {
        *tempStatsPtr = CSL_REG32_RD(statsBaseAddr+ (i *4));
        if (clear == TRUE)
        {
            CSL_REG32_WR(statsBaseAddr+ (i *4), *tempStatsPtr);
        }
        tempStatsPtr++;
    }
}

/*
 *  ======== EMAC_get_stats_icssg_v5_icssg_dual_mac ========
 */
static EMAC_DRV_ERR_E emac_get_stats_icssg_v5_icssg_dual_mac(uint32_t portNum, EMAC_STATISTICS_ICSSG_T *p_stats, bool clear)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_ERR_PORT_CLOSED;
    uint32_t * statPtr;

    if (p_stats != NULL)
    {
        statPtr = (uint32_t*)p_stats;
        if (portNum <=EMAC_ICSSG2_PORT1)
        {
            if ((portNum & 1) == 0)
            {
                emac_read_icssg_hw_stats_icssg_dual_mac(emac_mcb.port_cb[portNum].icssgCfgRegBaseAddr +  CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_STAT_GOOD_PRU0, statPtr, 0U, clear);
            }
            else
            {
                emac_read_icssg_hw_stats_icssg_dual_mac(emac_mcb.port_cb[portNum].icssgCfgRegBaseAddr +  CSL_ICSS_G_PR1_MII_RT_PR1_MII_RT_G_CFG_REGS_G_RX_STAT_GOOD_PRU1, statPtr, 0U, clear);
            }
                    retVal = EMAC_DRV_RESULT_OK;
        }
        else
        {
            retVal = EMAC_DRV_RESULT_INVALID_PORT;
        }
    }
    else
    {
        retVal = EMAC_DRV_RESULT_GET_STATS_ERR;
    }
    return retVal;
}

/*
 *  ======== emac_send_v5_icssg_dual_mac ========
 */
static EMAC_DRV_ERR_E emac_send_v5_icssg_dual_mac(uint32_t portNum, EMAC_PKT_DESC_T* pDesc)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;
    EMAC_CPPI_DESC_T *pCppiDesc = NULL;

    if (pDesc->PktChannel > (EMAC_TX_MAX_CHANNELS_PER_PORT -1))
    {
        UTILS_trace(UTIL_TRACE_LEVEL_ERR,emac_mcb.drv_trace_cb,"port: %d: TX channel number invalid during emac_send: %d",portNum);
        retVal = EMAC_DRV_RESULT_ERR_INVALID_CHANNEL;
    }
    else
    {
        emac_get_hw_cppi_tx_desc(portNum, pDesc->PktChannel, &pCppiDesc);
        if (pCppiDesc != NULL)
        {
            /* bits 0-7: specify the port number to direct the packet out, DstTag Low */
            /* bits 8-15: specify the tx port queue to use, DstTag High */
            pCppiDesc->hostDesc.srcDstTag = 0;
            pCppiDesc->hostDesc.srcDstTag = pDesc->TxPktTc <<8;

            /* clear flag for TX timestamp and then check to set it  */
            pCppiDesc->hostDesc.descInfo &= 0xDFFFFFFF;
            if (pDesc->Flags & EMAC_PKT_FLAG_TX_TS_REQ)
            {
                pCppiDesc->hostDesc.descInfo |= 0x20000000;
                pCppiDesc->psinfo[0] = pDesc->TxtimestampId;
                pCppiDesc->psinfo[1] = 0x80000000; /*tx_ts_request */
            }

            retVal = EMAC_send_v5_common(portNum, pDesc, pCppiDesc);
        }
        else
        {
            retVal = EMAC_DRV_RESULT_ERR_NO_FREE_DESC;
        }
    }
    return retVal;
} /* EMAC_send_v5 */

/*
 *  ======== EMAC_poll_v5_icssg_dual_mac ========
 */
static  EMAC_DRV_ERR_E emac_poll_v5_icssg_dual_mac(uint32_t portNum, EMAC_LINK_INFO_T* p_info)
{
    EMAC_DRV_ERR_E      result = EMAC_DRV_RESULT_OK;
    uint32_t              phy_addr;
    uint32_t mdioStatus;

    if (p_info==NULL)
    {
        result = EMAC_DRV_RESULT_POLL_ERR;
    }
    else
    {
        p_info->link_status_change = FALSE;

        if( EMAC_MDIO(portNum) )
        {
            /* Signal the MDIO*/
            mdioStatus = EMAC_mdioTimerTick( &emac_mcb.port_cb[portNum].MdioDev);

            /* Track new or lost link */
            if( mdioStatus == EMAC_MDIO_EVENT_LINKDOWN || mdioStatus == EMAC_MDIO_EVENT_LINKUP )
            {
                p_info->link_status_change = TRUE;

                EMAC_mdioGetStatus( &emac_mcb.port_cb[portNum].MdioDev, &phy_addr, &p_info->link_status );
                emac_icssg_update_link_params(portNum, p_info);
            }
        }
    }
    return result;
} /* EMAC_poll_v5_icssg_dual_mac */

/*
 *  ======== EMAC_poll_pkt_v5_icssg_dual_mac ========
 */
static EMAC_DRV_ERR_E emac_poll_pkt_v5_icssg_dual_mac(uint32_t portNum)
{
    EMAC_DRV_ERR_E retVal = emac_poll_pkt_v5_common(portNum);
    return retVal;
}


/*
 *  ======== emac_ioctl_v5_icssg_dual_mac ========
 */
static EMAC_DRV_ERR_E emac_ioctl_v5_icssg_dual_mac(uint32_t portNum, EMAC_IOCTL_CMD emacIoctlCmd, EMAC_IOCTL_PARAMS *emacIoctlParams)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_IOCTL_ERR_IN_PROGRESS;
    void *ioctlData = emacIoctlParams->ioctlVal;

    if (emac_mcb.ioctl_cb.ioctlInProgress == false)
    {
        switch (emacIoctlCmd)
        {
            case EMAC_IOCTL_PROMISCOUS_MODE_CTRL:
                retVal = emac_ioctl_icss_promiscous_ctrl(portNum, ioctlData);
                break;
            case EMAC_IOCTL_VLAN_CTRL:
                switch (emacIoctlParams->subCommand)
                {
                    case EMAC_IOCTL_VLAN_SET_DEFAULT_TBL:
                        retVal = emac_ioctl_vlan_ctrl_set_default_tbl(portNum, ioctlData);
                        break;
                    case EMAC_IOCTL_VLAN_SET_ENTRY:
                        retVal = emac_ioctl_vlan_ctrl_set_entry(portNum, ioctlData);
                        break;
                    case EMAC_IOCTL_VLAN_SET_DEFAULT_VLAN_ID:
                            retVal = emac_ioctl_vlan_ctrl_set_default_vlan_id(portNum, ioctlData);
                        break;
                    case EMAC_IOCTL_VLAN_GET_ENTRY:
                        retVal = emac_ioctl_vlan_ctrl_get_entry(portNum, ioctlData);
                        break;
                    default:
                        retVal = EMAC_DRV_RESULT_IOCTL_ERR;
                        break;
                }
                break;
            case EMAC_IOCTL_PORT_PRIO_MAPPING_CTRL:
                retVal = emac_ioctl_port_prio_mapping_ctrl(portNum, ioctlData);
                break;
            case EMAC_IOCTL_PORT_STATE_CTRL:
                switch (emacIoctlParams->subCommand)
                {
                    case EMAC_IOCTL_PORT_STATE_DISABLE:
                    case EMAC_IOCTL_PORT_STATE_BLOCKING:
                    case EMAC_IOCTL_PORT_STATE_FORWARD:
                    case EMAC_IOCTL_PORT_STATE_FORWARD_WO_LEARNING:
                    case EMAC_IOCTL_PORT_STATE_TAS_TRIGGER:
                    case EMAC_IOCTL_PORT_STATE_TAS_ENABLE:
                    case EMAC_IOCTL_PORT_STATE_TAS_RESET:
                    case EMAC_IOCTL_PORT_STATE_TAS_DISABLE:
                        retVal = emac_ioctl_port_state_ctrl(portNum, (void*)emacIoctlParams);
                    break;
                    default:
                        retVal = EMAC_DRV_RESULT_IOCTL_ERR;
                        break;
                }
                break;
            case EMAC_IOCTL_FDB_ENTRY_CTRL:
                switch (emacIoctlParams->subCommand)
                {
                    case EMAC_IOCTL_FDB_ENTRY_ADD:
                    case EMAC_IOCTL_FDB_ENTRY_DELETE:
                        retVal = emac_ioctl_fdb_entry_ctrl(portNum, (void*)emacIoctlParams);
                        break;
                    case EMAC_IOCTL_FDB_ENTRY_DELETE_ALL:
                    case EMAC_IOCTL_FDB_ENTRY_DELETE_ALL_AGEABLE:
                        retVal = emac_ioctl_fdb_del_all(portNum,(void*)emacIoctlParams);
                        break;
                    default:
                         retVal = EMAC_DRV_RESULT_IOCTL_ERR;
                        break;
                }
                break;
            case EMAC_IOCTL_ACCEPTABLE_FRAME_CHECK_CTRL:
                switch (emacIoctlParams->subCommand)
                {
                    case EMAC_IOCTL_ACCEPTABLE_FRAME_CHECK_ONLY_VLAN_TAGGED:
                    case EMAC_IOCTL_ACCEPTABLE_FRAME_CHECK_ONLY_UN_TAGGED_PRIO_TAGGED:
                    case EMAC_IOCTL_ACCEPTABLE_FRAME_CHECK_ALL_FRAMES:
                        retVal = emac_ioctl_accept_frame_check_ctrl(portNum, (void*)emacIoctlParams);
                        break;
                    default:
                        retVal = EMAC_DRV_RESULT_IOCTL_ERR;
                        break;
                }
                break;
            case EMAC_IOCTL_PRIO_REGEN_CTRL:
                retVal = emac_ioctl_prio_regen_mapping_ctrl(portNum, ioctlData);
                break;
            case EMAC_IOCTL_UC_FLOODING_CTRL:
                switch (emacIoctlParams->subCommand)
                {
                    case EMAC_IOCTL_PORT_UC_FLOODING_ENABLE:
                    case EMAC_IOCTL_PORT_UC_FLOODING_DISABLE:
                        retVal = emac_ioctl_uc_flooding_ctrl(portNum, (void*)emacIoctlParams);
                        break;
                    default:
                        retVal = EMAC_DRV_RESULT_IOCTL_ERR;
                        break;
                }
                break;
            case EMAC_IOCTL_MC_FLOODING_CTRL:
                switch (emacIoctlParams->subCommand)
                {
                    case EMAC_IOCTL_PORT_MC_FLOODING_ENABLE:
                    case EMAC_IOCTL_PORT_MC_FLOODING_DISABLE:
                        retVal = emac_ioctl_mc_flooding_ctrl(portNum, (void*)emacIoctlParams);
                        break;
                    default:
                        retVal = EMAC_DRV_RESULT_IOCTL_ERR;
                        break;
                }
                break;
            case EMAC_IOCTL_INTERFACE_MAC_CONFIG:
                switch (emacIoctlParams->subCommand)
                {
                    case EMAC_IOCTL_INTERFACE_MAC_ADD:
                        retVal = emac_ioctl_configure_interface_mac_ctrl(portNum, (void*)emacIoctlParams);
                        break;
                    default:
                        retVal = EMAC_DRV_RESULT_IOCTL_ERR;
                        break;
                }
                break;
            case EMAC_IOCTL_FRAME_PREEMPTION_CTRL:
                switch (emacIoctlParams->subCommand)
                {
                    case EMAC_IOCTL_PREEMPT_TX_ENABLE:
                    case EMAC_IOCTL_PREEMPT_TX_DISABLE:
                    case EMAC_IOCTL_PREEMPT_GET_TX_ENABLE_STATUS:
                    case EMAC_IOCTL_PREEMPT_GET_TX_ACTIVE_STATUS:
                    case EMAC_IOCTL_PREEMPT_VERIFY_ENABLE:
                    case EMAC_IOCTL_PREEMPT_VERIFY_DISABLE:
                    case EMAC_IOCTL_PREEMPT_GET_VERIFY_STATE:
                    case EMAC_IOCTL_PREEMPT_GET_MIN_FRAG_SIZE_LOCAL:
                    case EMAC_IOCTL_PREEMPT_SET_MIN_FRAG_SIZE_REMOTE:
                        retVal = emac_ioctl_frame_premption_ctrl(portNum, (void*)emacIoctlParams);
                        break;
                     default:
                        retVal = EMAC_DRV_RESULT_IOCTL_ERR;
                        break;
                }
                break;
            case EMAC_IOCTL_CUT_THROUGH_PREEMPT_SELECT:
                retVal = emac_ioctl_configure_cut_through_or_prempt_select_ctrl(portNum, (void*)emacIoctlParams);
                break;
            case EMAC_IOCTL_SPECIAL_FRAME_PRIO_CONFIG:
                retVal = emac_ioctl_configure_special_frame_prio_ctrl(portNum, (void*)emacIoctlParams);
                break;
            case EMAC_IOCTL_FDB_AGEING_TIMEOUT_CTRL:
                retVal = emac_ioctl_configure_fdb_ageing_interval(portNum, (void*)emacIoctlParams);
                break;
            case EMAC_IOCTL_SPEED_DUPLEXITY_CTRL:
                switch (emacIoctlParams->subCommand)
                {
                    case EMAC_IOCTL_SPEED_DUPLEXITY_10HD:
                    case EMAC_IOCTL_SPEED_DUPLEXITY_10FD:
                    case EMAC_IOCTL_SPEED_DUPLEXITY_100HD:
                    case EMAC_IOCTL_SPEED_DUPLEXITY_100FD:
                    case EMAC_IOCTL_SPEED_DUPLEXITY_GIGABIT:
                    case EMAC_IOCTL_SPEED_DUPLEXITY_DISABLE:
                        retVal = emac_ioctl_speed_duplexity_cfg(portNum, (void*)emacIoctlParams);
                        break;
                    default:
                        retVal = EMAC_DRV_RESULT_IOCTL_ERR;
                        break;
                }
                break;
            case EMAC_IOCTL_INGRESS_RATE_LIMIT_CTRL:
                retVal = emac_ioctl_ingress_rate_limiter_ctrl(portNum, (void*)emacIoctlParams);
                break;
            default:
                break;
        }
    }
    return retVal;
}

/*
 *  ======== EMAC_poll_ctrl_v5_icssg_dual_mac ========
 */
static EMAC_DRV_ERR_E emac_poll_ctrl_v5_icssg_dual_mac(uint32_t portNum, uint32_t rxPktRings, uint32_t rxMgmtRings, uint32_t txRings)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;
    retVal = emac_poll_ctrl_v5_common(portNum,rxPktRings, rxMgmtRings, txRings);
    return retVal;
}


/* Nothing past this point */

