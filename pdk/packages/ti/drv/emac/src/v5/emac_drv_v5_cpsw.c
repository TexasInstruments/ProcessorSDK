/**
 *  \file   emac_drv_v5_cpsw.c
 *
 *  \brief  emac driver API implementation for AM6 for CPSW use case
 *
 *  This file contains the driver APIs for EMAC peripheral for AM6
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
/* CSL Headers */
#include <ti/csl/soc.h>
#include <ti/drv/udma/src/udmap/csl_udmap.h>

#include <ti/csl/cslr_mdio.h>
#include <ti/csl/cslr_ale.h>
#include <ti/csl/csl_cpswitch.h>

#include <ti/drv/emac/emac_drv.h>
#include "ti/drv/emac/src/v5/emac_drv_v5.h"
#include "ti/drv/emac/src/v5/emac_utils.h"

extern EMAC_MCB_V5_T      emac_mcb;

void emac_setup_cpsw_ss(uint32_t portNum);

/**********************************************************************
 *************************** EMAC Global Variables*********************
 **********************************************************************/

/* EMAC_v5 local functions */
static EMAC_DRV_ERR_E emac_open_v5_cpsw(uint32_t portNum,  EMAC_OPEN_CONFIG_INFO_T *pConfig);

static EMAC_DRV_ERR_E emac_config_v5_cpsw(uint32_t portNum,  EMAC_CONFIG_INFO_T *pConfig);

static EMAC_DRV_ERR_E emac_close_v5_cpsw(uint32_t portNum);

static EMAC_DRV_ERR_E emac_poll_pkt_v5_cpsw(uint32_t portNum);

static EMAC_DRV_ERR_E emac_send_v5_cpsw(uint32_t portNum, EMAC_PKT_DESC_T* pDesc);

static EMAC_DRV_ERR_E emac_poll_v5_cpsw(uint32_t portNum, EMAC_LINK_INFO_T* p_info);

static EMAC_DRV_ERR_E emac_get_stats_v5_cpsw(uint32_t portNum, EMAC_STATISTICS_T*      p_stats);

static EMAC_DRV_ERR_E emac_get_stats_icssg_v5_cpsw(uint32_t portNum, EMAC_STATISTICS_ICSSG_T *p_stats, bool clear);

static EMAC_DRV_ERR_E emac_ioctl_v5_cpsw(uint32_t portNum, EMAC_IOCTL_CMD emacIoctlCmd, EMAC_IOCTL_PARAMS *emacIoctlParams);

static EMAC_DRV_ERR_E emac_poll_ctrl_v5_cpsw(uint32_t portNum, uint32_t rxPktRings, uint32_t rxMgmtRings, uint32_t txRings);


/* EMAC function table for EMAC_v5 implementation */
EMAC_FxnTable EMAC_FxnTable_CPSW_v5 =
{
    emac_open_v5_cpsw,
    emac_config_v5_cpsw,
    emac_close_v5_cpsw,
    emac_send_v5_cpsw,
    emac_poll_v5_cpsw,
    emac_get_stats_v5_cpsw,
    emac_poll_pkt_v5_cpsw,
    emac_get_stats_icssg_v5_cpsw,
    emac_ioctl_v5_cpsw,
    emac_poll_ctrl_v5_cpsw
};

/*
 *  ======== emac_open_v5_cpsw ========
 */
static EMAC_DRV_ERR_E  emac_open_v5_cpsw(uint32_t portNum, EMAC_OPEN_CONFIG_INFO_T *pConfig)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OPEN_PORT_ERR;
    UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: ENTER",portNum);

    if (pConfig != NULL)
    {
        retVal = emac_open_v5_common(portNum, pConfig);

        if (retVal == EMAC_DRV_RESULT_OK)
        {
            emac_setup_cpsw_ss(portNum);

            emac_mcb.port_cb[portNum].emacState =EMAC_PORT_STATE_OPEN;
            UTILS_trace(UTIL_TRACE_LEVEL_INFO, emac_mcb.drv_trace_cb, "port: %d: EXIT with status: %d",
                                                    portNum, retVal);
        }
    }
    return retVal;
}


/*
 *  ======== Emac_config_v5_cpsw ========
 */
static EMAC_DRV_ERR_E  emac_config_v5_cpsw(uint32_t portNum,  EMAC_CONFIG_INFO_T *pConfig)
{
    return EMAC_DRV_RESULT_OK;
} /* Emac_config_v5_cpsw */

/*
 *  ======== Emac_close_v5_cpsw ========
 */
static EMAC_DRV_ERR_E  emac_close_v5_cpsw(uint32_t portNum)
{
    EMAC_DRV_ERR_E retVal;
    retVal = emac_close_v5_common(portNum);
    return retVal; 
} /* Emac_close_v5_cpsw */


/*
 *  ======== EMAC_get_stats_v5_cpsw ========
 */
static EMAC_DRV_ERR_E emac_get_stats_v5_cpsw(uint32_t portNum, EMAC_STATISTICS_T*      p_stats)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_GET_STATS_ERR;
    CSL_CPSW_STATS stats[CSL_CPSW_NUMSTATBLOCKS];
    CSL_Xge_cpswRegs *pHwCpswRegs = emac_mcb.port_cb[portNum].hCpswRegs;
    if (p_stats != NULL)
    {
        if (emac_mcb.port_cb[EMAC_CPSW_PORT1].emacState == EMAC_PORT_STATE_OPEN)
        {
            memset (&stats, 0, sizeof(CSL_CPSW_STATS)*CSL_CPSW_NUMSTATBLOCKS);
            CSL_CPSW_getStats(pHwCpswRegs,(CSL_CPSW_STATS*)&stats[0]);
            p_stats->RxGoodFrames += stats[1].RxGoodFrames;
            p_stats->RxBCastFrames += stats[1].RxBCastFrames;
            p_stats->RxMCastFrames += stats[1].RxMCastFrames;
            p_stats->RxPauseFrames += stats[1].RxPauseFrames;
            p_stats->RxCRCErrors += stats[1].RxCRCErrors;
            p_stats->RxAlignCodeErrors += stats[1].RxAlignCodeErrors;
            p_stats->RxJabber += stats[1].RxJabber;
            p_stats->RxUndersized += stats[1].RxUndersized;
            p_stats->RxFragments += stats[1].RxFragments;
            p_stats->RxOctets += stats[1].RxOctets;
            p_stats->TxGoodFrames += stats[1].TxGoodFrames;
            p_stats->TxBCastFrames += stats[1].TxBCastFrames;
            p_stats->TxMCastFrames += stats[1].TxMCastFrames;
            p_stats->TxDeferred += stats[1].TxDeferred;
            p_stats->TxCollision += stats[1].TxCollision;
            p_stats->TxSingleColl += stats[1].TxSingleColl;
            p_stats->TxMultiColl += stats[1].TxMultiColl;
            p_stats->TxExcessiveColl += stats[1].TxExcessiveColl;
            p_stats->TxLateColl += stats[1].TxLateColl;
            p_stats->TxCarrierSLoss += stats[1].TxCarrierSLoss;
            p_stats->TxOctets += stats[1].TxOctets;
            retVal = EMAC_DRV_RESULT_OK;
        }
        else
        {
            retVal = EMAC_DRV_RESULT_ERR_PORT_CLOSED;
        }
    }
    return retVal;
} /* EMAC_get_stats_v5_cpsw */

/*
 *  ======== EMAC_get_stats_icssg_v5_cpsw ========
 */
static EMAC_DRV_ERR_E emac_get_stats_icssg_v5_cpsw(uint32_t portNum, EMAC_STATISTICS_ICSSG_T *p_stats, bool clear)
{
    return EMAC_DRV_RESULT_INVALID_PORT;
}



uint32_t gTxSendError = 0;
/*
 *  ======== emac_send_v5_cpsw ========
 */
static EMAC_DRV_ERR_E emac_send_v5_cpsw(uint32_t portNum, EMAC_PKT_DESC_T* pDesc)
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
            /* For CPSW port, always direct packet out of MAC port 1 */
            pCppiDesc->hostDesc.srcDstTag =1;
            retVal = EMAC_send_v5_common(portNum, pDesc, pCppiDesc);
            
        }
        else
        {
            gTxSendError++;
            retVal = EMAC_DRV_RESULT_ERR_NO_FREE_DESC;
        }
    }
    return retVal;
} /* EMAC_send_v5 */



static void emac_cpsw_update_link_params(uint32_t portNum, EMAC_LINK_INFO_T *p_info)
{
    if (p_info->link_status == EMAC_MDIO_LINKSTATUS_FD10     ||
        p_info->link_status == EMAC_MDIO_LINKSTATUS_FD100    ||
        p_info->link_status == EMAC_MDIO_LINKSTATUS_FD1000)
    {
        CSL_CPGMAC_SL_enableFullDuplex(emac_mcb.port_cb[portNum].hCpswRegs,0);
        if (p_info->link_status == EMAC_MDIO_LINKSTATUS_FD1000)
        {
            CSL_CPGMAC_SL_enableGigabit(emac_mcb.port_cb[portNum].hCpswRegs,0);
        }
        else
        {
            CSL_CPGMAC_SL_disableGigabit(emac_mcb.port_cb[portNum].hCpswRegs,0);
        }
    }
    else
    {
        CSL_CPGMAC_SL_disableFullDuplex(emac_mcb.port_cb[portNum].hCpswRegs,0);
        CSL_CPGMAC_SL_disableGigabit(emac_mcb.port_cb[portNum].hCpswRegs,0);
    }
}

/*
 *  ======== EMAC_poll_v5_cpsw ========
 */
static  EMAC_DRV_ERR_E emac_poll_v5_cpsw(uint32_t portNum, EMAC_LINK_INFO_T* p_info)
{
    EMAC_DRV_ERR_E      result = EMAC_DRV_RESULT_OK;
    uint32_t              phy_addr;
    uint32_t mdioStatus;

    if (p_info==NULL)
    {
        return EMAC_DRV_RESULT_POLL_ERR;
    }

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
            emac_cpsw_update_link_params(portNum, p_info);
        }
    }
    return result;
} /* EMAC_poll_v5_cpsw */

/*
 *  ======== EMAC_poll_pkt_v5_cpsw ========
 */
static EMAC_DRV_ERR_E emac_poll_pkt_v5_cpsw(uint32_t portNum)
{
    EMAC_DRV_ERR_E retVal = emac_poll_pkt_v5_common(portNum);
    return retVal;

} /* EMAC_poll_pkt_v5_cpsw */

/*
 *  ======== EMAC_ioctl_v5_cpsw ========
 */
static EMAC_DRV_ERR_E emac_ioctl_v5_cpsw(uint32_t portNum, EMAC_IOCTL_CMD emacIoctlCmd, EMAC_IOCTL_PARAMS *emacIoctlParams)
{
    return EMAC_DRV_RESULT_API_NOT_SUPPORTED;
}

/*
 *  ======== EMAC_poll_ctrl_v5_cpsw ========
 */
static EMAC_DRV_ERR_E emac_poll_ctrl_v5_cpsw(uint32_t portNum, uint32_t rxPktRings, uint32_t rxMgmtRings, uint32_t txRings)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_OK;
    retVal = emac_poll_ctrl_v5_common(portNum,rxPktRings, 0, txRings);
    return retVal;

}

void emac_setup_cpsw_ss(uint32_t portNum)
{
    CSL_CPSW_PORTSTAT portStatEnable;
    CSL_CPSW_ALE_PORTCONTROL portCtrl;
    CSL_Xge_cpswRegs *hCpswRegs;
    CSL_AleRegs *hCpswAleRegs;
    CSL_CPSW_ALE_POLICER_GLOB_CONFIG policerGblCfg;

#ifdef EMAC_DEBUG
     uint32_t returnVal;
#endif
    hCpswRegs = emac_mcb.port_cb[portNum].hCpswRegs;
    hCpswAleRegs = emac_mcb.port_cb[portNum].hCpswAleRegs;

    /*CPSW2U  Host port0 & Mac port1 Statistics Enable configuration */
    memset(&portStatEnable, 0, sizeof(CSL_CPSW_PORTSTAT));
    portStatEnable.p0StatEnable = 1U;
    portStatEnable.p1StatEnable = 1U;
    CSL_CPSW_setPortStatsEnableReg (hCpswRegs,&portStatEnable);

    /* ALE Configuration */
    CSL_CPSW_enableAle(hCpswAleRegs);

#ifdef EMAC_DEBUG
    returnVal = CSL_CPSW_isAleEnabled(hCpswAleRegs);
#endif
    CSL_CPSW_enableAleBypass(hCpswAleRegs);

#ifdef EMAC_DEBUG
    uint32_t ale;
    /* Wait for some time to check */
    for(ale = 0; ale < 100; ale++);
    returnVal = CSL_CPSW_isAleBypassEnabled(hCpswAleRegs);
#endif

    /*Enable packet forwarding on all ports */
    memset(&portCtrl, 0, sizeof(CSL_CPSW_ALE_PORTCONTROL));
    portCtrl.portState = CSL_ALE_PORTSTATE_FORWARD;
    CSL_CPSW_setAlePortControlReg(hCpswAleRegs,0U, &portCtrl);
    CSL_CPSW_setAlePortControlReg(hCpswAleRegs,1U, &portCtrl);
    /* CPGMAC configuration */
    CSL_CPGMAC_SL_enableFullDuplex(hCpswRegs,0);
    if (EMAC_CFG_LOOPBACK(portNum))
    {
        CSL_CPGMAC_SL_enableLoopback(hCpswRegs,0);
    }
    CSL_CPGMAC_SL_enableGMII(hCpswRegs,0);
    CSL_CPGMAC_SL_enableGigabit(hCpswRegs,0);
    CSL_CPGMAC_SL_enableGigForceMode(hCpswRegs,0);

    /* Setting MAX RX length to 2016 */
    CSL_CPSW_setPortRxMaxLen(hCpswRegs,0,0x7E0);
    CSL_CPSW_setPortRxMaxLen(hCpswRegs,1,0x7E0);

    /* Setup Thread Map Register for flow configuration*/
    policerGblCfg.defThreadEnable = 1;
    policerGblCfg.defThread = emac_mcb.port_cb[portNum].rxPktCh.rxChHandle->defaultFlowObj.flowStart;
    CSL_CPSW_setAlePolicerGlobConfig(emac_mcb.port_cb[portNum].hCpswAleRegs,&policerGblCfg);

    /* Enable Host port 0 */
    CSL_CPSW_enablePort0(hCpswRegs);

}



/* Nothing past this point */

